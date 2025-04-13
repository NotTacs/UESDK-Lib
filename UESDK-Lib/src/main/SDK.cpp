#include "../include/SDK.h"

uintptr_t SDK::Addresses::ObjObjects = 0;
uintptr_t SDK::Addresses::FNameToString = 0;
uintptr_t SDK::Addresses::FNameToStringVoid = 0;
uintptr_t SDK::Addresses::UObjectProcessEvent = 0;
uintptr_t SDK::Addresses::Step = 0;
uintptr_t SDK::Addresses::StepExplicitProperty = 0;

int SDK::MemberOffsets::UStruct_SuperStruct = -1;
int SDK::MemberOffsets::UStruct_Children = -1;
int SDK::MemberOffsets::UStruct_MinAllignment = -1;
int SDK::MemberOffsets::UStruct_Script = -1;
int SDK::MemberOffsets::UStruct_PropertyLink = -1;
int SDK::MemberOffsets::UStruct_RefLink = -1;
int SDK::MemberOffsets::UStruct_DestructorLink = -1;
int SDK::MemberOffsets::UStruct_PostConstructLink = -1;
int SDK::MemberOffsets::UStruct_ScriptObjectReferences = -1;
int SDK::MemberOffsets::UProperty_Offset_Internal = -1;
int SDK::MemberOffsets::UFunction_Func = -1;

std::string SDK::UE::EngineVersion = "";
std::unique_ptr<SDK::FUObjectArray> SDK::UE::Core::GObjects = nullptr;
SDK::UEngine* SDK::UE::Core::GEngine = nullptr;
SDK::UWorld* SDK::UE::Core::GWorld = nullptr;

bool SDK::UE::Core::InitGObjects()
{
	Memcury::Scanner Scanner = Memcury::Scanner::FindPattern("48 8B 05 ? ? ? ? 48 8B 0C C8 48 8B 04 D1");

	bool bChunked = true;

	if (Scanner.Get() == 0)
	{
		bChunked = false;

		Scanner = Memcury::Scanner::FindPattern("48 8B 05 ? ? ? ? 48 8D 14 C8 EB 03 49 8B D6 8B 42 08 C1 E8 1D A8 01 0F 85 ? ? ? ? F7 86 ? ? ? ? ? ? ? ?");

		if (Scanner.Get() == 0)
		{
			Scanner = Memcury::Scanner::FindPattern("48 8B 05 ? ? ? ? 48 8D 1C C8 81 4B ? ? ? ? ? 49 63 76 30");
		}
	}

	if (Scanner.Get() != 0)
		SDK::Addresses::ObjObjects = Scanner.RelativeOffset(3).Get();

	if (SDK::Addresses::ObjObjects == 0)
	{
		std::cout << "Failed to find ObjObjectsAddress" << std::endl;
		return false;
	}

	try
	{
		SDK::UE::Core::GObjects = std::make_unique<FUObjectArray>(SDK::Addresses::ObjObjects, bChunked);
	}
	catch (const std::exception& e)
	{
		std::cout << "Unexpected Exception" << std::endl;
	}

	if (!SDK::UE::Core::GObjects)
	{
		std::cout << "failed to initalize GObjects" << std::endl;
		return false;
	}

	return true;
}

bool SDK::UE::Core::InitMemberOffsets()
{
	MemberOffsets::UStruct_SuperStruct = SDK::UE::GetEngineVersion() >= 4.22 ? 0x40 : 0x30;
	MemberOffsets::UStruct_Children = MemberOffsets::UStruct_SuperStruct + 0x8;
	MemberOffsets::UStruct_MinAllignment = MemberOffsets::UStruct_Children + 0xC;
	MemberOffsets::UStruct_Script = MemberOffsets::UStruct_Children + 0x10;
	MemberOffsets::UStruct_PropertyLink = MemberOffsets::UStruct_SuperStruct + 0x30;
	MemberOffsets::UStruct_RefLink = MemberOffsets::UStruct_PropertyLink + 0x8;
	MemberOffsets::UStruct_DestructorLink = MemberOffsets::UStruct_RefLink + 0x8;
	MemberOffsets::UStruct_PostConstructLink = MemberOffsets::UStruct_DestructorLink + 0x8;
	MemberOffsets::UStruct_ScriptObjectReferences = MemberOffsets::UStruct_PostConstructLink + 0x8;
	MemberOffsets::UProperty_Offset_Internal = SDK::UE::GetFortniteVersion() >= 12.10 && std::floor(SDK::UE::GetFortniteVersion()) < 20 ? 0x4C : 0x44;

	return true;
}

bool SDK::UE::Core::InitFName()
{
	Memcury::Scanner Scanner = Memcury::Scanner::FindPattern("E8 ? ? ? ? 83 7C 24 ? ? 48 8D 3D ? ? ? ? 48 8B EF 48 8D 8E");

	if (Scanner.Get() == 0)
	{
		Scanner = Memcury::Scanner::FindStringRef(L"Material: '%s'").ScanFor({ 0xE8 }, false);
	}

	if (Scanner.Get() != 0)
		SDK::Addresses::FNameToString = Scanner.RelativeOffset(1).Get();

	if (SDK::Addresses::FNameToString == 0)
	{
		std::cout << "Failed to initalize FString& FName::ToString(FName* thisPtr, FString&)" << std::endl;
		return false;
	}

	Scanner = Memcury::Scanner(SDK::Addresses::FNameToString).ScanFor({ 0xE8 });

	if (Scanner.Get() != 0)
		SDK::Addresses::FNameToStringVoid = Scanner.RelativeOffset(1).Get();

	if (SDK::Addresses::FNameToStringVoid == 0)
	{
		std::cout << "Failed to initalize void FName::ToString(FName* thisPtr, FString&)" << std::endl;
		return false;
	}

	return true;
}

bool SDK::UE::Core::SetupEngineVersion()
{
	UFunction* GetEngineVersionFN = reinterpret_cast<UFunction*>(SDK::UE::Core::GObjects->FindObjectFast("GetEngineVersion"));

	Memcury::Scanner Scanner = Memcury::Scanner(uintptr_t(GetEngineVersionFN->Func())).ScanFor({ 0xE8 });

	if (!Scanner.Get())
		return false;

	static FString& (*GetEngineVersion)(FString & retstr) = decltype(GetEngineVersion)(Scanner.RelativeOffset(1).Get());

	FString TempString = GetEngineVersion(TempString);

	SDK::UE::EngineVersion = TempString.ToString();

	UFunction* SwitchLevelFN = reinterpret_cast<UFunction*>(SDK::UE::Core::GObjects->FindObjectFast("SwitchLevel"));

	Scanner = Memcury::Scanner(uintptr_t(SwitchLevelFN->Func())).ScanFor({ 0xE8 });

	SDK::Addresses::Step = Scanner.RelativeOffset(1).Get();

	Scanner = Memcury::Scanner(uintptr_t(SwitchLevelFN->Func())).ScanFor({ 0xE8 }, true, 1);

	SDK::Addresses::StepExplicitProperty = Scanner.RelativeOffset(1).Get();

	return true;
}

bool SDK::UE::Core::InitProcessEvent()
{
	Memcury::Scanner Scanner = Memcury::Scanner::FindPattern("41 FF 92 ? ? ? ? F6 C3", true);

	if (SDK::UE::GetFortniteVersion() >= 19.1) //idk mate
	{
		Scanner = Memcury::Scanner::FindPattern("41 FF 92 ? ? ? ? E9 ? ? ? ? 49 8B C8", true);
	}

	if (Scanner.Get())
	{
		SDK::Addresses::UObjectProcessEvent = Scanner.ScanFor({ 0x40, 0x55 }, false).Get();
	}
	else
	{
		return false;
	}

	return true;
}

bool SDK::UE::Core::InitGEngine()
{
	SDK::UE::Core::GEngine = SDK::UEngine::GetEngine();
	if (!SDK::UE::Core::GEngine)
	{
		return false;
	}
	return true;
}
bool SDK::UE::Core::InitGWorld()
{
	SDK::UE::Core::GWorld = SDK::UEngine::GetEngine()->GameViewport()->World();
	if (!SDK::UE::Core::GWorld)
	{
		return false;
	}
	return true;
}
bool SDK::Init()
{
	if (!SDK::UE::Core::InitGObjects())
	{
		std::cout << "Failed to initalize GObjects" << std::endl;
		return false;
	}
	Memcury::Scanner Scanner = Memcury::Scanner::FindPattern("FF 95 ? ? ? ? 48 8B 6C 24");

	if (Scanner.Get() == 0)
	{
		Scanner = Memcury::Scanner::FindPattern("FF 97 ? ? ? ? 48 8B 6C 24");

		if (Scanner.Get() == 0)
		{
			Scanner = Memcury::Scanner::FindPattern("FF 95 ? ? ? ? 48 8B 6C 24", true);
		}
	}

	if (Scanner.Get() == 0)
		return false;

	MemberOffsets::UFunction_Func = *Scanner.AbsoluteOffset(2).GetAs<int*>();
	if (!SDK::UE::Core::InitFName())
	{
		std::cout << "Failed to initalize FName" << std::endl;
		return false;
	}
	if (!SDK::UE::Core::SetupEngineVersion())
	{
		std::cout << "Failed to initalize EngineVersion" << std::endl;
		return false;
	}
	if (!SDK::UE::Core::InitMemberOffsets())
	{
		std::cout << "Failed to initalize MemberOffsets" << std::endl;
		return false;
	}

	if (!SDK::UE::Core::InitProcessEvent())
	{
		std::cout << "Failed to initalize ProcessEvent" << std::endl;
		return false;
	}

	if (!SDK::UE::Core::InitGEngine())
	{
		std::cout << "Failed to initalize GEngine" << std::endl;
		return false;
	}

	if (!SDK::UE::Core::InitGWorld())
	{
		std::cout << "Failed to initalize GWorld" << std::endl;
		return false;
	}

	return true;
}

uintptr_t SDK::UE::Memory::GetBaseAddress()
{
	return uintptr_t(GetModuleHandle(0));
}

float SDK::UE::GetFortniteVersion()
{
	size_t LastDot = EngineVersion.find('-', EngineVersion.find('-') + 1);
	std::string ParsedString = EngineVersion.substr(LastDot + 1);

	if (ParsedString.contains("Cert") || ParsedString.contains("Live"))
	{
		switch (SDK::UE::GetFortniteCL()) //prob wanna use cl for most of these builds
		{
		case 3541083:
			ParsedString = "1.2";
			break;
		case 3700114:
			ParsedString = "1.72";
			break;
		case 3724489:
			ParsedString = "1.8";
			break;
		case 3729133:
			ParsedString = "1.81";
			break;
		case 3741772:
			ParsedString = "1.82";
			break;
		case 3757339:
			ParsedString = "1.9";
			break;
		case 3775276:
			ParsedString = "1.91";
			break;
		case 3790078:
			ParsedString = "1.10";
			break;
		case 3807424:
			ParsedString = "1.11";
			break;
		}
	}

	return std::stof(ParsedString);
}

double SDK::UE::GetEngineVersion()
{
	size_t FirstDotPosition = EngineVersion.find('.');
	size_t SecondDotPostition = EngineVersion.find('.', FirstDotPosition + 1);
	std::string ParsedString = EngineVersion.substr(0, SecondDotPostition);

	return std::stod(ParsedString);
}

int SDK::UE::GetFortniteCL()
{
	size_t dashPos = EngineVersion.find('-');
	size_t plusPos = EngineVersion.find('+', dashPos);

	std::string ParsedString = "";

	if (dashPos != std::string::npos && plusPos != std::string::npos) {
		ParsedString = EngineVersion.substr(dashPos + 1, plusPos - dashPos - 1);
	}

	return std::stoi(ParsedString);
}
