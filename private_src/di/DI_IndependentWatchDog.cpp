#include <IndependentWatchDog.h>
#include <bsp-interface/di.h>

bsp::IIndependentWatchDog &DI_IndependentWatchDog()
{
	return hal::IndependentWatchDog::Instance();
}
