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

SDK::FQuat SDK::FQuat::FromRotator(const SDK::FRotator& R)
{
	const float P = DegreesToRadians(R.Pitch) * 0.5f;
	const float Y = DegreesToRadians(R.Yaw) * 0.5f;
	const float Rl = DegreesToRadians(R.Roll) * 0.5f;

	const float sinP = std::sin(P);
	const float cosP = std::cos(P);
	const float sinY = std::sin(Y);
	const float cosY = std::cos(Y);
	const float sinR = std::sin(Rl);
	const float cosR = std::cos(Rl);

	FQuat q;
	q.W = cosR * cosP * cosY + sinR * sinP * sinY;
	q.X = sinR * cosP * cosY - cosR * sinP * sinY;
	q.Y = cosR * sinP * cosY + sinR * cosP * sinY;
	q.Z = cosR * cosP * sinY - sinR * sinP * cosY;

	return q;
}