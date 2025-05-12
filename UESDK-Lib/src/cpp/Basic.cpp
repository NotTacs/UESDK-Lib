#include "../include/SDK.h"
#include <sdk/Basic.h>


SDK::UField* SDK::UField::Next()
{
	static int NextAddress = -1;

	if (NextAddress == -1)
	{
		NextAddress = 0x28;
	}

	return *reinterpret_cast<UField**>(__int64(this) + NextAddress);
}

SDK::FUObjectArray::FUObjectArray(uintptr_t ObjectAddress, bool bIsChunked)
	: ObjObjectsAddr(ObjectAddress), bUseChunkArray(bIsChunked)
{
	if (ObjectAddress == 0)
		throw std::runtime_error("FUObjectArray::FUObjectArray constructer recieved a null address");
}

SDK::FUObjectItem* SDK::FUObjectArray::GetObjectPtr(int32 Index)
{
	if (this->bUseChunkArray)
		return reinterpret_cast<FChunkedFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);
	else
		return reinterpret_cast<FFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);

	return nullptr;
}

SDK::FUObjectItem const* SDK::FUObjectArray::GetObjectPtr(int32 Index) const
{
	if (this->bUseChunkArray)
		return reinterpret_cast<FChunkedFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);
	else
		return reinterpret_cast<FFixedUObjectArray*>(ObjObjectsAddr)->GetObjectPtr(Index);

	return nullptr;
}

SDK::int32 SDK::FUObjectArray::Num() const
{
	if (this->bUseChunkArray)
		return reinterpret_cast<FChunkedFixedUObjectArray*>(ObjObjectsAddr)->Num();
	else
		return reinterpret_cast<FFixedUObjectArray*>(ObjObjectsAddr)->Num();

	return 0;
}

std::vector<SDK::UObject*> SDK::FUObjectArray::GetObjectsOfClass(UClass* Class)
{
	std::vector<UObject*> Objects;
	for (int i = 0; i < Num(); i++)
	{
		UObject* Object = GetObjectPtr(i)->Object;
		if (!Object) continue;
		if (Object->IsA(Class))
		{
			Objects.push_back(Object);
		}
	}
	return Objects;
}

SDK::FString SDK::FName::ToString() const
{
	static FString& (*FName__ToString)(const FName * thisPtr, FString & retstr) = nullptr;

	if (!FName__ToString)
		FName__ToString = decltype(FName__ToString)(SDK::Addresses::FNameToString);

	FString ReturnValue = FName__ToString(this, ReturnValue);
	return ReturnValue;
}

void SDK::FName::ToString(FString& OutString) const
{
	static FString& (*FName__ToStringVoid)(const FName * thisPtr, FString & retstr) = nullptr;

	if (!FName__ToStringVoid)
		FName__ToStringVoid = decltype(FName__ToStringVoid)(SDK::Addresses::FNameToStringVoid);

	FName__ToStringVoid(this, OutString);
}

SDK::UStruct* SDK::UStruct::SuperStruct() const
{
	static int SuperStructOffset = SDK::MemberOffsets::UStruct_SuperStruct;
	return *reinterpret_cast<UStruct**>(__int64(this) + SuperStructOffset);
}

SDK::UField* SDK::UStruct::Children() const
{
	static int ChildrenOffset = SDK::MemberOffsets::UStruct_Children;
	return *reinterpret_cast<UField**>(__int64(this) + ChildrenOffset);
}

SDK::FField* SDK::UStruct::ChildrenProperties() const
{

	return *reinterpret_cast<FField**>(__int64(this) + SDK::MemberOffsets::UStruct_ChildProperties);
}

SDK::int32 SDK::UStruct::Size() const
{
	static int SizeOffset = SDK::MemberOffsets::UStruct_Size;
	return *reinterpret_cast<int32*>(__int64(this) + SizeOffset);
}

SDK::int32 SDK::UStruct::MinAlignment() const
{
	static int MinAlignmentOffset = SDK::MemberOffsets::UStruct_MinAllignment;
	return *reinterpret_cast<int32*>(__int64(this) + MinAlignmentOffset);
}

SDK::TArray<uint8_t>& SDK::UStruct::Script() const
{
	return *reinterpret_cast<TArray<uint8>*>(__int64(this) + SDK::MemberOffsets::UStruct_Script);
}

SDK::UProperty* SDK::UStruct::PropertyLink() const
{
	static int PropertyLinkOffset = SDK::MemberOffsets::UStruct_PropertyLink;

	return *reinterpret_cast<UProperty**>(__int64(this) + PropertyLinkOffset);
}

SDK::UProperty* SDK::UStruct::RefLink() const
{
	static int NextRef = SDK::MemberOffsets::UStruct_RefLink;

	return *reinterpret_cast<UProperty**>(__int64(this) + NextRef);
}

SDK::UProperty* SDK::UStruct::DestructorLink() const
{
	static int DestructorLinkOffset = SDK::MemberOffsets::UStruct_DestructorLink;
	return *reinterpret_cast<UProperty**>(__int64(this) + DestructorLinkOffset);
}

SDK::UProperty* SDK::UStruct::PostConstructLink() const
{
	static int PostConstructLinkOffset = SDK::MemberOffsets::UStruct_PostConstructLink;
	return *reinterpret_cast<UProperty**>(__int64(this) + PostConstructLinkOffset);
}

SDK::int32 SDK::UProperty::Offset_Internal()
{
	static int OffsetInternalOffset = SDK::MemberOffsets::UProperty_Offset_Internal;
	return *reinterpret_cast<int32*>(__int64(this) + OffsetInternalOffset);
}

SDK::UProperty* SDK::UProperty::PropertyLinkNext()
{
	static int PropertyLinkNext = SDK::MemberOffsets::UProperty_Offset_Internal + 0xC;
	return *reinterpret_cast<UProperty**>(__int64(this) + PropertyLinkNext);
}

int SDK::UProperty::ElementSize()
{
	static int ElementSizeOffset = SDK::MemberOffsets::UProperty_Offset_Internal - 0x10;
	return *reinterpret_cast<int*>(__int64(this) + ElementSizeOffset);
}

SDK::UClass* SDK::UObjectProperty::PropertyClass()
{
	static int PropertyClassOffset = (SDK::MemberOffsets::UProperty_Offset_Internal + 0x2C);
	return *reinterpret_cast<UClass**>(__int64(this) + PropertyClassOffset);
}

SDK::UClass* SDK::USoftObjectProperty::PropertyClass()
{
	static int PropertyClassOffset = (SDK::MemberOffsets::UProperty_Offset_Internal + 0x2C);
	return *reinterpret_cast<UClass**>(__int64(this) + PropertyClassOffset);
}

SDK::UProperty* SDK::UArrayProperty::Inner()
{
	static int InnerOffset = (SDK::MemberOffsets::UProperty_Offset_Internal + 0x2C);
	return *reinterpret_cast<UProperty**>(__int64(this) + InnerOffset);
}

SDK::UScriptStruct* SDK::UStructProperty::Struct()
{
	static int StructOffset = (SDK::MemberOffsets::UProperty_Offset_Internal + 0x2C);
	return *reinterpret_cast<UScriptStruct**>(__int64(this) + StructOffset);
}

std::string SDK::UProperty::GetPropName()
{
	return SDK::UE::GetFortniteVersion() >= 12.1 ? (*(FName*)(__int64(this) + 0x28)).ToString().ToString() : GetName().ToString();
}

std::string SDK::UProperty::GetPropCPPType()
{
	if (UBoolProperty* BoolProp = CastField<UBoolProperty>(this))
	{
		return "bool";
	}
	else if (UIntProperty* IntProp = CastField<UIntProperty>(this))
	{
		return "int32_t";
	}
	else if (UFloatProperty* FloatProp = CastField<UFloatProperty>(this))
	{
		return "float";
	}
	else if (UNameProperty* NameProp = CastField<UNameProperty>(this))
	{
		return "FName";
	}
	else if (UStrProperty* StrProp = CastField<UStrProperty>(this))
	{
		return "FString";
	}
	else if (UObjectProperty* ObjProp = CastField<UObjectProperty>(this))
	{
		FString ObjClassName = ObjProp->PropertyClass()->GetName();
		return std::string("class ") + ObjClassName.ToString() + "*";
	}
	else if (UStructProperty* StructProp = CastField<UStructProperty>(this))
	{
		FString StructName = StructProp->Struct()->GetName();
		return std::string("struct ") + StructName.ToString();
	}
	else if (UByteProperty* ByteProp = CastField<UByteProperty>(this))
	{
		return "uint8_t";
	}
	else if (UUInt16Property* UInt16Prop = CastField<UUInt16Property>(this))
	{
		return "uint16_t";
	}
	else if (UUInt32Property* UInt32Prop = CastField<UUInt32Property>(this))
	{
		return "uint32_t";
	}
	else if (UUInt64Property* UInt64Prop = CastField<UUInt64Property>(this))
	{
		return "uint64_t";
	}
	else if (UInt8Property* Int8Prop = CastField<UInt8Property>(this))
	{
		return "int8_t";
	}
	else if (UInt16Property* Int16Prop = CastField<UInt16Property>(this))
	{
		return "int16_t";
	}
	else if (UInt64Property* Int64Prop = CastField<UInt64Property>(this))
	{
		return "int64_t";
	}
	else if (USoftObjectProperty* SoftObjProp = CastField<USoftObjectProperty>(this))
	{
		FString ObjClassName = SoftObjProp->PropertyClass()->GetName();
		return std::string("TSoftObjectPtr<class ") + ObjClassName.ToString() + ">";
	}
	else if (UArrayProperty* ArrayProp = CastField<UArrayProperty>(this))
	{
		std::string InnerType = ArrayProp->Inner()->GetPropCPPType();
		return std::string("TArray<") + InnerType + ">";
	}
	
	return "uint8 " + this->GetPropName() + "[" + std::to_string(this->Offset_Internal()) + "]; /* Failed to get property type */";
}

SDK::UProperty* SDK::UStruct::FindPropertyByName(std::string PropertyName, bool bUseNext)
{
	UProperty* result = nullptr;

	if (bUseNext)
	{
		if (this->IsA(UFunction::StaticClass()))
		{
			for (UField* Next = this->Children(); Next != nullptr; Next = Next->Next())
			{
				UProperty* NextProp = reinterpret_cast<UProperty*>(Next);
				//std::cout << NextProp->GetPropName() << std::endl;
				result = NextProp;
				if (NextProp->GetPropName() == PropertyName)
					return NextProp;
			}
			return result;
		}

		for (UStruct* Struct = this; Struct != nullptr; Struct = Struct->SuperStruct())
		{
			static bool bUseChildrenProperties = SDK::MemberOffsets::UStruct_ChildProperties != -1;
			if (bUseChildrenProperties)
			{
				for (FField* Next = Struct->ChildrenProperties(); Next; Next = Next->Next)
				{
					UProperty* NextProp = reinterpret_cast<UProperty*>(Next);
					result = NextProp;
					if (NextProp->GetPropName() == PropertyName)
					{
						return result;
					}
				}
			}
			else {
				for (UField* Next = Struct->Children(); Next != nullptr; Next = Next->Next())
				{
					if (!Next->GetClass()) continue;
					if (Next->IsA<UFunction>()) continue;
					UProperty* NextProp = reinterpret_cast<UProperty*>(Next);
					result = NextProp;
					if (NextProp->GetPropName() == PropertyName)
						return result;
				}
			}
		}

		return result;
	}

	result = this->PropertyLink();
	if (!result)
	{
		std::cout << "Failed to get valid PropertyLink from Class: " << this->GetName().ToString() << std::endl;
		return nullptr;
	}
	while (result->GetPropName() != PropertyName)
	{
		result = result->PropertyLinkNext();
		if (!result)
		{
			std::cout << "Failed to get NextProperty from PropertyLink in Class: " << this->GetName().ToString() << std::endl;
			return nullptr;
		}
	}

	return result;
}

void SDK::UObject::ProcessEvent(UFunction* Function, void* Params)
{
	static void (*ProcessEventOriginal)(UObject * thisPtr, UFunction * Function, void* Params) = nullptr;

	if (!ProcessEventOriginal)
		ProcessEventOriginal = decltype(ProcessEventOriginal)(SDK::Addresses::UObjectProcessEvent);

	return ProcessEventOriginal(this, Function, Params);
}

SDK::UObject* SDK::FUObjectArray::FindObjectFast(std::string Name)
{
	for (int i = 0; i < this->Num(); i++)
	{
		FUObjectItem* ObjectItemPtr = GetObjectPtr(i);

		if (!ObjectItemPtr) continue;

		if (!ObjectItemPtr->Object) continue;

		if (ObjectItemPtr->Object->GetName().ToString() == Name)
			return ObjectItemPtr->Object;
	}

	return nullptr;
}
SDK::UFunction* SDK::UStruct::FindFunctionByName(std::string FunctionName)
{
	for (const UStruct* Class = this; Class; Class = Class->SuperStruct())
	{
		for (UField* Next = Class->Children(); Next != nullptr; Next = Next->Next())
		{
			if (!Next->GetClass()) continue;
			if (Next->GetClass() != SDK::UE::Core::GObjects->FindObjectFast("Function")) continue;
			if (Next->GetName() == FunctionName)
			{
				return reinterpret_cast<UFunction*>(Next);
			}
		}
	}

	return nullptr;
}

SDK::UFunction::FNativeFuncPtr& SDK::UFunction::Func()
{
	return *reinterpret_cast<FNativeFuncPtr*>(__int64(this) + SDK::MemberOffsets::UFunction_Func);
}

SDK::uint32& SDK::UFunction::FunctionFlags()
{
	static int FunctionFlagsOffset = SDK::MemberOffsets::UFunction_Func - 0x28;
	return *reinterpret_cast<uint32*>(__int64(this) + FunctionFlagsOffset);
}

std::string SDK::UFunction::FunctionFlagsToString()
{
	std::string result;

	auto Flags = FunctionFlags();

	if (Flags & FUNC_BlueprintCallable) result += "FUNC_BlueprintCallable | ";
	if (Flags & FUNC_BlueprintEvent) result += "FUNC_BlueprintEvent | ";
	if (Flags & FUNC_Exec) result += "FUNC_Exec | ";
	if (Flags & FUNC_Net) result += "FUNC_Net | ";
	if (Flags & FUNC_NetClient) result += "FUNC_NetClient | ";
	if (Flags & FUNC_NetServer) result += "FUNC_NetServer | ";
	if (Flags & FUNC_NetMulticast) result += "FUNC_NetMulticast | ";
	if (Flags & FUNC_Private) result += "FUNC_Private | ";
	if (Flags & FUNC_Protected) result += "FUNC_Protected | ";
	if (Flags & FUNC_Public) result += "FUNC_Public | ";
	if (Flags & FUNC_BlueprintAuthorityOnly) result += "FUNC_BlueprintAuthorityOnly | ";
	if (Flags & FUNC_NetResponse) result += "FUNC_NetResponse | ";
	if (Flags & FUNC_Static) result += "FUNC_Static | ";
	if (Flags & FUNC_Native) result += "FUNC_Native | ";
	if (Flags & FUNC_Const) result += "FUNC_Const | ";
	if (Flags & FUNC_AllFlags) result += "FUNC_AllFlags | ";

	if (!result.empty() && result.substr(result.length() - 3) == " | ")
	{
		result = result.substr(0, result.length() - 3);
	}

	return result;
}

SDK::UObject* SDK::UClass::GetDefaultObj()
{
	std::string Name = "Default__" + this->GetName().ToString();
	return SDK::UE::Core::GObjects->FindObjectFast(Name);
}

bool SDK::UObjectBaseUtility::IsDefaultObject()
{
	return (GetFlags() & EObjectFlags::RF_ClassDefaultObject);
}

std::string SDK::UObjectBaseUtility::FlagsToString()
{
	std::string result;

	auto& Flags = GetFlags();

	if (Flags & RF_Public) result += "RF_Public | ";
	if (Flags & RF_Standalone) result += "RF_Standalone | ";
	if (Flags & RF_MarkAsNative) result += "RF_MarkAsNative | ";
	if (Flags & RF_Transactional) result += "RF_Transactional | ";
	if (Flags & RF_ClassDefaultObject) result += "RF_ClassDefaultObject | ";
	if (Flags & RF_ArchetypeObject) result += "RF_ArchetypeObject | ";
	if (Flags & RF_Transient) result += "RF_Transient | ";
	if (Flags & RF_MarkAsRootSet) result += "RF_MarkAsRootSet | ";
	if (Flags & RF_TagGarbageTemp) result += "RF_TagGarbageTemp | ";
	if (Flags & RF_NeedInitialization) result += "RF_NeedInitialization | ";
	if (Flags & RF_NeedLoad) result += "RF_NeedLoad | ";
	if (Flags & RF_KeepForCooker) result += "RF_KeepForCooker | ";
	if (Flags & RF_NeedPostLoad) result += "RF_NeedPostLoad | ";
	if (Flags & RF_NeedPostLoadSubobjects) result += "RF_NeedPostLoadSubobjects | ";
	if (Flags & RF_NewerVersionExists) result += "RF_NewerVersionExists | ";
	if (Flags & RF_BeginDestroyed) result += "RF_BeginDestroyed | ";
	if (Flags & RF_FinishDestroyed) result += "RF_FinishDestroyed | ";
	if (Flags & RF_BeingRegenerated) result += "RF_BeingRegenerated | ";
	if (Flags & RF_DefaultSubObject) result += "RF_DefaultSubObject | ";
	if (Flags & RF_WasLoaded) result += "RF_WasLoaded | ";
	if (Flags & RF_TextExportTransient) result += "RF_TextExportTransient | ";
	if (Flags & RF_LoadCompleted) result += "RF_LoadCompleted | ";
	if (Flags & RF_InheritableComponentTemplate) result += "RF_InheritableComponentTemplate | ";
	if (Flags & RF_DuplicateTransient) result += "RF_DuplicateTransient | ";
	if (Flags & RF_StrongRefOnFrame) result += "RF_StrongRefOnFrame | ";
	if (Flags & RF_NonPIEDuplicateTransient) result += "RF_NonPIEDuplicateTransient | ";
	if (Flags & RF_WillBeLoaded) result += "RF_WillBeLoaded | ";
	if (Flags & RF_HasExternalPackage) result += "RF_HasExternalPackage | ";
	if (Flags & RF_MirroredGarbage) result += "RF_MirroredGarbage | ";
	if (Flags & RF_AllocatedInSharedPage) result += "RF_AllocatedInSharedPage | ";

	if (!result.empty() && result.substr(result.length() - 3) == " | ")
	{
		result = result.substr(0, result.length() - 3);
	}

	return result;
}

bool SDK::UClass::IsChildOf(const SDK::UStruct* Base) const
{
	if (!Base)
		return false;

	if (this == Base)
		return true;

	for (const UStruct* Struct = this; Struct; Struct = Struct->SuperStruct())
	{
		if (Struct == Base)
			return true;
	}

	return false;
}

SDK::uint8 SDK::UBoolProperty::FieldMask()
{
	if (SDK::UE::GetEngineVersion() <= 4.24 || SDK::UE::GetFortniteVersion() >= 20)
		return *(uint8_t*)(__int64(this) + (112 + 3 + 0));
	else if (SDK::UE::GetEngineVersion() >= 4.25)
		return *(uint8_t*)(__int64(this) + (120 + 3 + 0));
}

bool SDK::UBoolProperty::ReadBitFieldValue(void* Object)
{
	auto Addr = (void*)((PlaceholderBitfield*)(__int64(Object) + this->Offset_Internal()));

	auto BitField = (PlaceholderBitfield*)Addr;

	uint8 Mask = FieldMask();

	for (int i = 0; i < 8; ++i)
	{
		if (Mask == (1 << i))
		{
			switch (i)
			{
			case 0: return BitField->First;
			case 1: return BitField->Second;
			case 2: return BitField->Third;
			case 3: return BitField->Fourth;
			case 4: return BitField->Fifth;
			case 5: return BitField->Sixth;
			case 6: return BitField->Seventh;
			case 7: return BitField->Eighth;
			}
		}
	}

	return false;
}

void SDK::UBoolProperty::SetBitFieldValue(void* Object, bool NewVal)
{
	auto Addr = (void*)((PlaceholderBitfield*)(__int64(Object) + this->Offset_Internal()));

	auto BitField = (PlaceholderBitfield*)Addr;

	uint8 Mask = FieldMask();

	for (int i = 0; i < 8; ++i)
	{
		if (Mask == (1 << i))
		{
			switch (i)
			{
			case 0: BitField->First = NewVal; break;
			case 1: BitField->Second = NewVal; break;
			case 2: BitField->Third = NewVal; break;
			case 3: BitField->Fourth = NewVal; break;
			case 4: BitField->Fifth = NewVal; break;
			case 5: BitField->Sixth = NewVal; break;
			case 6: BitField->Seventh = NewVal; break;
			case 7: BitField->Eighth = NewVal; break;
			}
		}
	}
}

SDK::UClass* SDK::FSoftObjectPath::GetStructClass()
{
	static UClass* Class = nullptr;
	if (!Class)
		Class = reinterpret_cast<UClass*>(SDK::UE::Core::GObjects->FindObjectFast("SoftObjectPath"));
	return Class;
}

class SDK::UClass* SDK::StaticClassImpl(const char* ClassName)
{
	static std::unordered_map<std::string, SDK::UClass*> ClassCache;
	auto It = ClassCache.find(ClassName);
	if (It != ClassCache.end())
	{
		return It->second;
	}
	auto Class = reinterpret_cast<SDK::UClass*>(SDK::UE::Core::GObjects->FindObjectFast(ClassName));
	if (!Class)
	{
		std::cout << "Failed to find class: " << ClassName << std::endl;
		return nullptr;
	}
	ClassCache[ClassName] = Class;
	return Class;
}