#include "IndependentWatchDogConfig.h"
#include <stdexcept>

bsp::IndependentWatchDogConfig::IndependentWatchDogConfig(IWDG_InitTypeDef const &o)
{
    *this = o;
}

bsp::IndependentWatchDogConfig &bsp::IndependentWatchDogConfig::operator=(IWDG_InitTypeDef const &o)
{
    _config_handle = o;
    return *this;
}

#pragma region 分频系数

bsp::IndependentWatchDogConfig_PrescalerOption bsp::IndependentWatchDogConfig::Prescaler() const
{
    return static_cast<IndependentWatchDogConfig_PrescalerOption>(_config_handle.Prescaler);
}

void bsp::IndependentWatchDogConfig::SetPrescaler(IndependentWatchDogConfig_PrescalerOption value)
{
    _config_handle.Prescaler = static_cast<uint32_t>(value);
}

uint8_t bsp::IndependentWatchDogConfig::GetPrescalerByPow() const
{
    switch (Prescaler())
    {
    case IndependentWatchDogConfig_PrescalerOption::Div4:
        {
            return 2;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div8:
        {
            return 3;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div16:
        {
            return 4;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div32:
        {
            return 5;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div64:
        {
            return 6;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div128:
        {
            return 7;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div256:
        {
            return 8;
        }
    default:
        {
            throw std::invalid_argument{"Prescaler() 的值非法"};
        }
    }
}

void bsp::IndependentWatchDogConfig::SetPrescalerByPow(uint8_t pow)
{
    auto power_to_prescaler = [](uint8_t pow) -> IndependentWatchDogConfig_PrescalerOption
    {
        switch (pow)
        {
        case 2:
            {
                return IndependentWatchDogConfig_PrescalerOption::Div4;
            }
        case 3:
            {
                return IndependentWatchDogConfig_PrescalerOption::Div8;
            }
        case 4:
            {
                return IndependentWatchDogConfig_PrescalerOption::Div16;
            }
        case 5:
            {
                return IndependentWatchDogConfig_PrescalerOption::Div32;
            }
        case 6:
            {
                return IndependentWatchDogConfig_PrescalerOption::Div64;
            }
        case 7:
            {
                return IndependentWatchDogConfig_PrescalerOption::Div128;
            }
        case 8:
            {
                return IndependentWatchDogConfig_PrescalerOption::Div256;
            }
        default:
            {
                throw std::invalid_argument{"pow 的值非法"};
            }
        }
    };

    SetPrescaler(power_to_prescaler(pow));
}

uint32_t bsp::IndependentWatchDogConfig::GetPrescalerByUint32() const
{
    switch (Prescaler())
    {
    case IndependentWatchDogConfig_PrescalerOption::Div4:
        {
            return 4;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div8:
        {
            return 8;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div16:
        {
            return 16;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div32:
        {
            return 32;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div64:
        {
            return 64;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div128:
        {
            return 128;
        }
    case IndependentWatchDogConfig_PrescalerOption::Div256:
        {
            return 256;
        }
    default:
        {
            throw std::invalid_argument{"Prescaler() 的值非法"};
        }
    }
}

void bsp::IndependentWatchDogConfig::SetPrescalerByUint32(uint32_t value)
{
    switch (value)
    {
    case 4:
        {
            SetPrescaler(IndependentWatchDogConfig_PrescalerOption::Div4);
            break;
        }
    case 8:
        {
            SetPrescaler(IndependentWatchDogConfig_PrescalerOption::Div8);
            break;
        }
    case 16:
        {
            SetPrescaler(IndependentWatchDogConfig_PrescalerOption::Div16);
            break;
        }
    case 32:
        {
            SetPrescaler(IndependentWatchDogConfig_PrescalerOption::Div32);
            break;
        }
    case 64:
        {
            SetPrescaler(IndependentWatchDogConfig_PrescalerOption::Div64);
            break;
        }
    case 128:
        {
            SetPrescaler(IndependentWatchDogConfig_PrescalerOption::Div128);
            break;
        }
    case 256:
        {
            SetPrescaler(IndependentWatchDogConfig_PrescalerOption::Div256);
            break;
        }
    default:
        {
            throw std::invalid_argument{"SetPrescalerByUint32 的 value 非法"};
        }
    }
}

#pragma endregion

uint32_t bsp::IndependentWatchDogConfig::ReloadValue() const
{
    return _config_handle.Reload;
}

void bsp::IndependentWatchDogConfig::SetReloadValue(uint32_t value)
{
    _config_handle.Reload = value;
}
