#include "SysTickClock.h"
#include <bsp-interface/di/delayer.h>

extern "C"
{
    /// @brief 重写 __weak 的 HAL_Delay 函数
    /// @param ms 要延时的毫秒数。
    void HAL_Delay(uint32_t ms)
    {
        DI_Delayer().Delay(std::chrono::milliseconds{ms});
    }

    void SysTick_Handler()
    {
        HAL_IncTick();
        if (bsp::SysTickClock::Instance()._elapsed_handler)
        {
            bsp::SysTickClock::Instance()._elapsed_handler();
        }
    }
}

bsp::SysTickClock &bsp::SysTickClock::Instance()
{
    class Getter :
        public base::SingletonGetter<SysTickClock>
    {
    public:
        std::unique_ptr<SysTickClock> Create() override
        {
            return std::unique_ptr<SysTickClock>{new SysTickClock{}};
        }

        void Lock() override
        {
            DI_DisableGlobalInterrupt();
        }

        void Unlock() override
        {
            DI_EnableGlobalInterrupt();
        }
    };

    Getter g;
    return g.Instance();
}

bsp::SysTickClock::SysTickClockSourceOption bsp::SysTickClock::ClockSource() const
{
    uint32_t masked = SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk;
    if (masked)
    {
        return SysTickClockSourceOption::HCLK;
    }

    return SysTickClockSourceOption::HCLK_DIV8;
}

void bsp::SysTickClock::SetClockSource(SysTickClock::SysTickClockSourceOption value)
{
    HAL_SYSTICK_CLKSourceConfig(static_cast<uint32_t>(value));
}

uint32_t bsp::SysTickClock::Frequency() const
{
    if (ClockSource() == SysTickClockSourceOption::HCLK_DIV8)
    {
        return HAL_RCC_GetHCLKFreq() / 8;
    }

    return HAL_RCC_GetHCLKFreq();
}

uint32_t bsp::SysTickClock::CurrentValue() const
{
    uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
    return masked >> SysTick_VAL_CURRENT_Pos;
}

uint32_t bsp::SysTickClock::ReloadValue() const
{
    uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
    return masked >> SysTick_LOAD_RELOAD_Pos;
}

void bsp::SysTickClock::SetElapsedHandler(std::function<void()> func)
{
    DI_DoGlobalCriticalWork(
        [&]()
        {
            _elapsed_handler = func;
        });
}
