#include <SDK.h>
#include <sdk/Engine.h>



SDK::UEngine* SDK::UEngine::GetEngine()
{
	if (!SDK::UE::Core::GEngine)
	{
		for (SDK::FUObjectItem* Object : *SDK::UE::Core::GObjects)
		{
			if (!Object) continue;
			if (!Object->Object) continue;
			if (Object->Object->GetName().ToString().contains("FortEngine_") && !Object->Object->IsDefaultObject())
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

SDK::UWorld* SDK::UWorld::GetWorld() 
{
	static UEngine* GEngine = nullptr;

	if (!GEngine)
		GEngine = UEngine::GetEngine();

	if (UGameViewportClient* GameViewportClient = GEngine->GameViewport())
	{
		return GameViewportClient->World();
	}
}

SDK::FRotator SDK::FQuat::ToRotator() const
{
	FRotator Rotator;
	float SingularityTest = Z * X - W * Y;
	float YawY = 2.f * (W * Z + X * Y);
	float YawX = 1.f - 2.f * (Y * Y + Z * Z);
	float SingularityThreshold = 0.4999995f;

	if (SingularityTest < -SingularityThreshold)
	{
		Rotator.Pitch = -90.f;
		Rotator.Yaw = RadiansToDegrees(std::atan2(YawY, YawX));
		Rotator.Roll = 0.f;
	}
	else if (SingularityTest > SingularityThreshold)
	{
		Rotator.Pitch = 90.f;
		Rotator.Yaw = RadiansToDegrees(std::atan2(YawY, YawX));
		Rotator.Roll = 0.f;
	}
	else
	{
		Rotator.Pitch = RadiansToDegrees(std::asin(2.f * SingularityTest));
		Rotator.Yaw = RadiansToDegrees(std::atan2(YawY, YawX));
		Rotator.Roll = RadiansToDegrees(std::atan2(-2.f * (W * X + Y * Z), 1.f - 2.f * (X * X + Y * Y)));
	}

	return Rotator;
}