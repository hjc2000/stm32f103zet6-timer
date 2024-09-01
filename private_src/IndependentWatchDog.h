#pragma once
#include <base/SingletonGetter.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/IIndependentWatchDog.h>
#include <chrono>
#include <IndependentWatchDogConfig.h>

namespace hal
{
    /// @brief 独立看门狗。
    /// @note 所谓独立看门狗就是具有自己的内部时钟源，不依赖单片机的系统时钟。
    /// 在系统时钟失效时仍然能工作。
    class IndependentWatchDog final :
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

        void Initialize();

    public:
        static IndependentWatchDog &Instance()
        {
            class Getter : public base::SingletonGetter<IndependentWatchDog>
            {
            public:
                std::unique_ptr<IndependentWatchDog> Create() override
                {
                    return std::unique_ptr<IndependentWatchDog>{new IndependentWatchDog{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }

        IWDG_TypeDef *HardwareInstance()
        {
            return IWDG;
        }

        std::chrono::milliseconds WatchDogTimeoutDuration() const override;
        void SetWatchDogTimeoutDuration(std::chrono::milliseconds value) override;
        void Feed() override;
    };
} // namespace hal
