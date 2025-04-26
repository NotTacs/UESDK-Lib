#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <memory>
#include <type_traits>
#include <fstream>
#include <sstream>
#include <mutex>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include "memcury.h"
#include "sdk/Basic.h"
#include "sdk/Engine.h"

namespace SDK
{
	namespace Addresses {
		extern uintptr_t ObjObjects;
		extern uintptr_t FNameToString;
		extern uintptr_t FNameToStringVoid;
		extern uintptr_t UObjectProcessEvent;
		extern uintptr_t Step;
		extern uintptr_t StepExplicitProperty;
	}

	namespace MemberOffsets {
		extern int UStruct_SuperStruct;
		extern int UStruct_Children;
		extern int UStruct_MinAllignment;
		extern int UStruct_Script;
		extern int UStruct_PropertyLink;
		extern int UStruct_RefLink;
		extern int UStruct_DestructorLink;
		extern int UStruct_PostConstructLink;
		extern int UStruct_ScriptObjectReferences;
		extern int UProperty_Offset_Internal;
		extern int UFunction_Func;
		extern int UStruct_Size;
	}


	namespace UE
	{
		extern std::string EngineVersion;

		float GetFortniteVersion();

		double GetEngineVersion();

		int GetFortniteCL();
	}

	namespace UE::Memory
	{
		uintptr_t GetBaseAddress();
	}

	namespace UE::Core
	{
		extern std::unique_ptr<SDK::FUObjectArray> GObjects;
		extern SDK::UEngine* GEngine;
		bool InitGObjects();
		bool InitMemberOffsets();
		bool InitFName();
		bool SetupEngineVersion();
		bool InitProcessEvent();
		bool InitGEngine();
	}
	bool Init();
}