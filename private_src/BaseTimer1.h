#pragma once
#include <atomic>
#include <base/define.h>
#include <BaseTimerConfig.h>
#include <bsp-interface/timer/IBaseTimer.h>
#include <functional>

namespace hal
{
    /// @brief TIM6 和 TIM7 是基本定时器。具有 16 位的预分频器和 16 位的计数器。将分频器也看作计数器的话，
    /// 则是一个 32 位的计数器。其中，预分频器是低 16 位，16 位的计数器是高 16 位。
    class BaseTimer1 :
        public bsp::IBaseTimer
    {
    private:
        BaseTimer1() = default;

        TIM_HandleTypeDef _handle;
        std::function<void()> _on_period_elapsed;

        void Initialize(BaseTimerConfig const &config);

    public:
        static_function BaseTimer1 &Instance()
        {
            static BaseTimer1 o;
            return o;
        }

        /// @brief 初始化定时器
        /// @param period 定时周期
        void Open() override;

        /// @brief 启动定时器。
        void Start(std::chrono::milliseconds period) override;

        /// @brief 停止定时器。
        void Stop() override;

        /// @brief 设置定时时间到触发的回调。
        /// @note 设置为 nullptr 可以取消回调。
        /// @warning 实现者需要将本函数实现为线程安全和可重入。最简单的方法就是在本函数的
        /// 操作期间禁用全局中断。
        /// @param func
        void SetElapsedHandle(std::function<void()> func) override;
    };
} // namespace hal
