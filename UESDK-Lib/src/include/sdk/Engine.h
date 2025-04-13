#pragma once
namespace SDK
{
	class UGameViewportClient;
	class UWorld;
	class AActor;
	class AFortGameStateAthena;
	class APlayerController;
	struct FVector;
	struct FQuat;
	struct FTransform;

	class UEngine : public UObject
	{
	public:
		UGameViewportClient* GameViewport();
	public:
		static UEngine* GetEngine();
	};

	class UGameViewportClient : public UObject
	{
	public:
		UWorld* World();
	};


	class ULocalPlayer : public UObject
	{
	public:
		APlayerController* PlayerController();
	};

	class UGameInstance : public UObject
	{
	public:
		TArray<ULocalPlayer*>& LocalPlayers();
	};

	class AActor : public UObject
	{
	public:

	public:
		FTransform GetTransform();
	};

	class APlayerController : public AActor
	{
	public:
		void SwitchLevel(SDK::FString URL);
	};

	class UWorld : public UObject
	{
	public:
		AFortGameStateAthena* GameState();
		UGameInstance* OwningGameInstance();
	};

	class UKismetStringLibrary : public UObject
	{
	public:
		static FName Conv_StringToName(const FString& InString);
	public:
		static UClass* StaticClass();
	};

	enum class EFastArraySerializerDeltaFlags : uint8
	{
		None = 0,
		HasBeenSerialized = 1,
		HasDeltaBeenRequested = 2,
		IsUsingDeltaSerialization = 4,
		EFastArraySerializerDeltaFlags_MAX = 5,
	};

	struct FFastArraySerializerItem
	{
	public:
		int32 ReplicationID;
		int32 ReplicationKey;
		int32 MostRecentArrayReplicationKey;
	};

	struct FFastArraySerializer
	{
	public:
		uint8 ItemMap[80];
		int32 IDCounter;
		int32 ArrayReplicationKey;
		char GuidReferencesMap[0x50];
		char GuidReferencesMap_StructDelta[0x50];

		int32 CachedNumItems;
		int32 CachedNumItemsToConsiderForWriting;

		EFastArraySerializerDeltaFlags DeltaFlags;

		uint8 _Padding1[0x7];

		void MarkItemDirty(FFastArraySerializerItem& Item, bool markArrayDirty = true)
		{
			if (Item.ReplicationID == -1)
			{
				Item.ReplicationID = ++IDCounter;
				if (IDCounter == -1)
				{
					IDCounter++;
				}
			}

			Item.ReplicationKey++;
			if (markArrayDirty) MarkArrayDirty();
		}

		void MarkArrayDirty()
		{
			ArrayReplicationKey++;
			if (ArrayReplicationKey == -1)
			{
				ArrayReplicationKey++;
			}

			CachedNumItems = -1;
			CachedNumItemsToConsiderForWriting = -1;
		}
	};

	struct FFrame : public FOutputDevice
	{
		void* VFT;
		UFunction* Node;
		UObject* Object;
		uint8* Code;
		uint8* Locals;
		UProperty* MostRecentProperty;
		uint8_t* MostRecentPropertyAddress;
		uint8 FlowStack[0x30];
		FFrame* PreviousFrame;
		void* OutParms;
		UField* PropertyChainForCompiledIn;
		UFunction* CurrentNativeFunction;
		bool bArrayContextFailed;

	public:
		void StepCompiledIn(void* const Result, bool ForceExplicitProp = false);
		void IncrementCode();
	};

	struct FVector {
		float X, Y, Z;

		FVector() : X(0), Y(0), Z(0) {}
		FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
	};

	struct FQuat {
		float X, Y, Z, W;

		FQuat() : X(0), Y(0), Z(0), W(1) {}
		FQuat(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
	};

	struct FTransform {
		FQuat Rotation;
		FVector Translation;
		FVector Scale3D;

		FTransform()
			: Rotation(0, 0, 0, 1), Translation(0, 0, 0), Scale3D(1, 1, 1) {
		}

		FTransform(const FQuat& InRotation, const FVector& InTranslation, const FVector& InScale3D)
			: Rotation(InRotation), Translation(InTranslation), Scale3D(InScale3D) {
		}

	};
}