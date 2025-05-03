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
		//UGameViewportClient* GameViewport();
		DECLARE_INLINEPROP_WITH_OFFSET(UGameViewportClient*, UEngine, GameViewport);
	public:
		DECLARE_STATIC_CLASS(UEngine);
		DECLARE_DEFAULT_OBJECT(UEngine);
		static UEngine* GetEngine();
	};

	class UGameViewportClient : public UObject
	{
	public:
		DECLARE_INLINEPROP_WITH_OFFSET(UWorld*, UGameViewportClient, World);
	public:
		DECLARE_STATIC_CLASS(UGameViewportClient);
		DECLARE_DEFAULT_OBJECT(UGameViewportClient);
	};


	class ULocalPlayer : public UObject
	{
	public:
		DECLARE_INLINEPROP_WITH_OFFSET(APlayerController*, ULocalPlayer, PlayerController);
	public:
		DECLARE_STATIC_CLASS(ULocalPlayer);
		DECLARE_DEFAULT_OBJECT(ULocalPlayer);
	};

	class UGameInstance : public UObject
	{
	public:
		DECLARE_INLINEPROP_WITH_OFFSET(ULocalPlayer*, UGameInstance, LocalPlayers);
	public:
		DECLARE_STATIC_CLASS(UGameInstance);
		DECLARE_DEFAULT_OBJECT(UGameInstance);
	};

	class AActor : public UObject
	{
	private:

	public:

	public:
		DECLARE_STATIC_CLASS(AActor);
		DECLARE_DEFAULT_OBJECT(AActor);
	};


	class APlayerController : public AActor
	{
	private:

	public:
		DECLARE_STATIC_CLASS(APlayerController);
		DECLARE_DEFAULT_OBJECT(APlayerController);
	};

	class UNetDriver : public UObject
	{
	public:
		
	public:
		DECLARE_STATIC_CLASS(UNetDriver);
		DECLARE_DEFAULT_OBJECT(UNetDriver);
	};

	class UWorld : public UObject
	{
	public:
		DECLARE_INLINEPROP_WITH_OFFSET(AFortGameStateAthena*, UWorld, GameState);
		DECLARE_INLINEPROP_WITH_OFFSET(UGameInstance*, UWorld, OwningGameInstance);
		DECLARE_INLINEPROP_WITH_OFFSET(UNetDriver*, UWorld, NetDriver);

	public:
		DECLARE_STATIC_CLASS(UWorld);
		DECLARE_DEFAULT_OBJECT(UWorld);
		static UWorld* GetWorld();
	};

	class UKismetSystemLibrary : public UObject
	{
	public:
	public:
		DECLARE_STATIC_CLASS(UKismetSystemLibrary);
		DECLARE_DEFAULT_OBJECT(UKismetSystemLibrary);
	};

	class UKismetStringLibrary : public UObject
	{
	private:
		struct KismetStringLibrary_ConvStringToName
		{
		public:
			FString InString;
			FName ReturnValue;
		};
	public:
		DECLARE_STATIC_CLASS(UKismetStringLibrary);
		DECLARE_DEFAULT_OBJECT(UKismetStringLibrary);
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

	struct FRotator;

	struct FQuat {
		float X, Y, Z, W;

		FQuat() : X(0), Y(0), Z(0), W(1) {}
		FQuat(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

		FQuat Conjugate() const
		{
			return FQuat(-X, -Y, -Z, W);
		}

		FQuat Inverse() const
		{
			float Norm = X * X + Y * Y + Z * Z + W * W;
			if (Norm > 0.f)
			{
				float InvNorm = 1.f / Norm;
				return FQuat(-X * InvNorm, -Y * InvNorm, -Z * InvNorm, W * InvNorm);
			}
			return FQuat(0.f, 0.f, 0.f, 1.f);
		}

		FQuat Normalize() const
		{
			float Norm = std::sqrt(X * X + Y * Y + Z * Z + W * W);
			if (Norm > 0.f)
			{
				float InvNorm = 1.f / Norm;
				return FQuat(X * InvNorm, Y * InvNorm, Z * InvNorm, W * InvNorm);
			}
			return FQuat(0.f, 0.f, 0.f, 1.f);
		}

		FRotator ToRotator() const;


		FQuat operator*(const FQuat& Q) const
		{
			return FQuat(
				W * Q.X + X * Q.W + Y * Q.Z - Z * Q.Y,
				W * Q.Y - X * Q.Z + Y * Q.W + Z * Q.X,
				W * Q.Z + X * Q.Y - Y * Q.X + Z * Q.W,
				W * Q.W - X * Q.X - Y * Q.Y - Z * Q.Z
			);
		}

		static FQuat Slerp(const FQuat& A, const FQuat& B, float Alpha)
		{
			float Dot = A.X * B.X + A.Y * B.Y + A.Z * B.Z + A.W * B.W;
			FQuat BCopy = B;

			if (Dot < 0.f)
			{
				Dot = -Dot;
				BCopy = FQuat(-B.X, -B.Y, -B.Z, -B.W);
			}

			if (Dot > 0.9995f)
			{
				return FQuat(
					A.X + Alpha * (BCopy.X - A.X),
					A.Y + Alpha * (BCopy.Y - A.Y),
					A.Z + Alpha * (BCopy.Z - A.Z),
					A.W + Alpha * (BCopy.W - A.W)
				).Normalize();
			}

			float Theta = std::acos(Dot);
			float SinTheta = std::sin(Theta);
			float ScaleA = std::sin((1.f - Alpha) * Theta) / SinTheta;
			float ScaleB = std::sin(Alpha * Theta) / SinTheta;

			return FQuat(
				ScaleA * A.X + ScaleB * BCopy.X,
				ScaleA * A.Y + ScaleB * BCopy.Y,
				ScaleA * A.Z + ScaleB * BCopy.Z,
				ScaleA * A.W + ScaleB * BCopy.W
			);
		}

		static FQuat FromRotator(const FRotator& R);
	};

	struct FRotator
	{
		float Pitch;
		float Yaw;
		float Roll;

		FRotator(float InPitch = 0, float InYaw = 0, float InRoll = 0)
			: Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {
		}

		void Normalize()
		{
			Pitch = NormalizeAxis(Pitch);
			Yaw = NormalizeAxis(Yaw);
			Roll = NormalizeAxis(Roll);
		}

		bool Equals(const FRotator& Other, float Tolerance = 1e-4f) const
		{
			return std::abs(NormalizeAxis(Pitch - Other.Pitch)) < Tolerance &&
				std::abs(NormalizeAxis(Yaw - Other.Yaw)) < Tolerance &&
				std::abs(NormalizeAxis(Roll - Other.Roll)) < Tolerance;
		}

		FVector Vector() const
		{
			float CP = std::cos(DegreesToRadians(Pitch));
			float SP = std::sin(DegreesToRadians(Pitch));
			float CY = std::cos(DegreesToRadians(Yaw));
			float SY = std::sin(DegreesToRadians(Yaw));

			return FVector(CP * CY, CP * SY, SP);
		}

		FQuat Quaternion() const
		{
			return FQuat::FromRotator(*this);
		}
	private:
		static float NormalizeAxis(float Angle)
		{
			Angle = std::fmod(Angle + 180.f, 360.f);
			if (Angle < 0.f)
				Angle += 360.f;
			return Angle - 180.f;
		}
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