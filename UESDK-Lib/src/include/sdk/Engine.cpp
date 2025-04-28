#include "../SDK.h"
#include "Engine.h"

SDK::UEngine* SDK::UEngine::GetEngine()
{
	if (!SDK::UE::Core::GEngine)
	{
		for (SDK::FUObjectItem* Object : *SDK::UE::Core::GObjects)
		{
			if (!Object) continue;
			if (!Object->Object) continue;
			if (Object->Object->GetName().ToString().contains("FortEngine_"))
			{
				SDK::UE::Core::GEngine = (SDK::UEngine*)Object->Object;
			}
		}
	}

	return SDK::UE::Core::GEngine;
}

static inline void (*Step)(SDK::FFrame* Stack, SDK::UObject*, void* const) = decltype(Step)(SDK::Addresses::Step);
static inline void (*StepExplicitProperty)(SDK::FFrame*, void* const, SDK::UField*) = decltype(StepExplicitProperty)(SDK::Addresses::StepExplicitProperty);

void SDK::FFrame::StepCompiledIn(void* const Result, bool ForceExplicitProp)
{
	if (Code && !ForceExplicitProp)
	{
		Step(this, Object, Result);
	}
	else
	{
		UField* _Prop = PropertyChainForCompiledIn;
		PropertyChainForCompiledIn = _Prop->Next();
		StepExplicitProperty(this, Result, _Prop);
	}
}

void SDK::FFrame::IncrementCode()
{
	Code = (uint8_t*)(__int64(Code) + (bool)Code);
}
