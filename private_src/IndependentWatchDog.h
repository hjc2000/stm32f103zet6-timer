#pragma once
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/timer/IIndependentWatchDog.h>
#include <chrono>
#include <IndependentWatchDogConfig.h>

namespace hal
{
    /// @brief 独立看门狗。
    /// @note 所谓独立看门狗就是具有自己的内部时钟源，不依赖单片机的系统时钟。
    /// 在系统时钟失效时仍然能工作。
    class IndependentWatchDog :
        public bsp::IIndependentWatchDog
    {
    private:
        IWDG_HandleTypeDef _handle;
        IndependentWatchDogConfig _config;

        /// @brief 内部时钟信号的频率。还要经过预分频才会输入到计数器。
        /// @return
        uint32_t InnerClockSourceFreq_Hz() const
        {
            // 独立看门狗具有 40 kHz 的内部时钟。
            return 40 * 1000;
        }

    public:
        static_function IndependentWatchDog &Instance();

        IWDG_TypeDef *HardwareInstance()
        {
            return IWDG;
        }

        /// @brief 打开看门狗定时器。
        /// @param timeout 看门狗超时时间。
        void Open(std::chrono::milliseconds timeout);

        /// @brief 关闭看门狗定时器。
        /// @note 有的单片机的独立看门狗一旦开启后就关不掉了，调用本函数不会起作用。
        void Close();

        /// @brief 看门狗超时时间。
        /// @return
        std::chrono::milliseconds Timeout() const;

        /// @brief 喂狗。
        void Feed() override;
    };
} // namespace hal
