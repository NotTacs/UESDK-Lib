#include "../SDK.h"
#include "Engine.h"

SDK::UGameViewportClient* SDK::UEngine::GameViewport()
{
	return GET_PROPERTY_VALUE<UGameViewportClient*>(this, "GameViewport");
}

SDK::UWorld* SDK::UGameViewportClient::World()
{
	return GET_PROPERTY_VALUE<UWorld*>(this, "World");
}

SDK::AFortGameStateAthena* SDK::UWorld::GameState()
{
	return GET_PROPERTY_VALUE<AFortGameStateAthena*>(this, "GameState");
}

SDK::UGameInstance* SDK::UWorld::OwningGameInstance()
{
	return GET_PROPERTY_VALUE<UGameInstance*>(this, "OwningGameInstance");
}

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

SDK::UClass* SDK::UKismetStringLibrary::StaticClass()
{
	static UClass* Class = nullptr;
	if (!Class)
		Class = reinterpret_cast<UClass*>(SDK::UE::Core::GObjects->FindObjectFast("KismetStringLibrary"));
	return Class;
}

SDK::FName SDK::UKismetStringLibrary::Conv_StringToName(const FString& InString)
{
	static UFunction* Func = nullptr;
	if (!Func)
		Func = StaticClass()->FindFunctionByName("Conv_StringToName");

	struct KismetStringLibrary_Conv_StringToName
	{
		FString InString;
		FName ReturnValue;
	};

	KismetStringLibrary_Conv_StringToName Params;

	Params.InString = std::move(InString);

	StaticClass()->GetDefaultObj()->ProcessEvent(Func, &Params);

	return Params.ReturnValue;
}

SDK::APlayerController* SDK::ULocalPlayer::PlayerController()
{
	return GET_PROPERTY_VALUEREF<SDK::APlayerController*>(this, "PlayerController");
}
SDK::TArray<SDK::ULocalPlayer*>& SDK::UGameInstance::LocalPlayers()
{
	return GET_PROPERTY_VALUEREF<TArray<SDK::ULocalPlayer*>>(this, "LocalPlayers");
}

void SDK::APlayerController::SwitchLevel(SDK::FString URL)
{
	static UFunction* Func = nullptr;

	if (!Func)
	{
		Func = this->GetClass()->FindFunctionByName("SwitchLevel");
	}

	struct PlayerController_SwitchLevel
	{
		SDK::FString URL;
	} Params;

	Params.URL = URL;

	this->ProcessEvent(Func, &Params);
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

SDK::FTransform SDK::AActor::GetTransform()
{
	static UFunction* Func = nullptr;
	if (!Func)
		Func = this->GetClass()->FindFunctionByName("GetTransform");

	struct
	{
		FTransform ReturnValue;
	} Params;

	this->ProcessEvent(Func, &Params);

	return Params.ReturnValue;
}