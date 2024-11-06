#include "BaseTimer1.h"
#include <bsp-interface/di/interrupt.h>
#include <stdexcept>

using namespace hal;

void BaseTimer1::Initialize(BaseTimerConfig const &config)
{
    __HAL_RCC_TIM6_CLK_ENABLE();
    _handle.Instance = TIM6;
    _handle.Init = config;
    HAL_TIM_Base_Init(&_handle);
    _handle.PeriodElapsedCallback = [](TIM_HandleTypeDef *handle)
    {
        if (BaseTimer1::Instance()._on_period_elapsed)
        {
            BaseTimer1::Instance()._on_period_elapsed();
        }
    };

    DI_IsrManager().AddIsr(
        static_cast<uint32_t>(IRQn_Type::TIM6_IRQn),
        []()
        {
            HAL_TIM_IRQHandler(&BaseTimer1::Instance()._handle);
        });
}

void hal::BaseTimer1::Open()
{
}

void hal::BaseTimer1::Start(std::chrono::milliseconds period)
{
    uint32_t timer_clock_signal_freq = HAL_RCC_GetPCLK1Freq();
    RCC_ClkInitTypeDef def;
    uint32_t flash_latency;
    HAL_RCC_GetClockConfig(&def, &flash_latency);
    if (def.APB1CLKDivider != RCC_HCLK_DIV1)
    {
        /*
         * PCLK1 的来源是 HCLK 分频后的输出。如果分频系数大于 1，
         * 则定时器会将 PCLK1 进行 2 倍频后作为自己的时钟信号。
         */
        timer_clock_signal_freq *= 2;
    }

    /*
     * 将定时器的 16 位分频器和 16 位计数器一起看成一个 32 位的计数器。
     * 这个 32 位计数器的时钟周期是
     *	1 / timer_clock_signal_freq
     * 设计数到 count 刚好达到 period 毫秒。则
     *	count * 1 / timer_clock_signal_freq = period / 1000
     *	count / timer_clock_signal_freq = period / 1000
     *	count = period * timer_clock_signal_freq / 1000
     * 因为 timer_clock_signal_freq 一般较大，所以先进行除法，于是：
     *	count = period * (timer_clock_signal_freq / 1000)
     */

    uint32_t count = period.count() * (timer_clock_signal_freq / 1000);

    /*
     * 定时器中，分频器相当于这个 32 位计数器的低 16 位，而原本的 16 位计数器相当于
     * 这个 32 位计数器的高 16 位，于是把 count 分成高低 16 位，分别赋予它们。
     */
    BaseTimerConfig options{};
    options._counter_mode = BaseTimerConfig::CounterModeOption::Up;
    options._prescaler = count & 0xffff;
    options._period = count >> 16;
    options._is_auto_reload_preload_enabled = true;
    Initialize(options);

    DI_InterruptSwitch().EnableInterrupt(TIM6_IRQn, 10);
    HAL_TIM_Base_Start_IT(&_handle);
}

void BaseTimer1::Stop()
{
    HAL_TIM_Base_Stop_IT(&_handle);
}

void hal::BaseTimer1::SetElapsedHandle(std::function<void()> func)
{
    DI_InterruptSwitch().DoGlobalCriticalWork(
        [&]()
        {
            _on_period_elapsed = func;
        });
}
