#pragma once
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/timer/ISysTick.h>
#include <chrono>
#include <hal.h>
#include <stdint.h>

extern "C"
{
    void SysTick_Handler();
}

namespace bsp
{
    class SysTickClock :
        public bsp::ISysTick
    {
    private:
        SysTickClock() = default;

        std::function<void()> _elapsed_handler;

        friend void ::SysTick_Handler();

    public:
        static_function SysTickClock &Instance();

        enum class SysTickClockSourceOption
        {
            HCLK = SYSTICK_CLKSOURCE_HCLK,
            HCLK_DIV8 = SYSTICK_CLKSOURCE_HCLK_DIV8,
        };

        SysTickClock::SysTickClockSourceOption ClockSource() const;
        void SetClockSource(SysTickClock::SysTickClockSourceOption value);

        /// @brief 当前的实际频率
        /// @return
        uint32_t Frequency() const override;

        /// @brief 当前的计数值。
        /// @return
        uint32_t CurrentValue() const override;

        /// @brief 当前的重装载值。
        /// @return
        uint32_t ReloadValue() const override;

        /// @brief 定时时间到处理函数。
        /// @warning 实现者需要将本函数实现为线程安全和可重入。最简单的方法就是在本函数的
        /// 操作期间禁用全局中断。
        /// @param func
        void SetElapsedHandler(std::function<void()> func) override;
    };
} // namespace bsp
