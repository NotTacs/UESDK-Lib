#pragma once

namespace SDK
{
	class TFieldIterator
	{
	public:
		TFieldIterator(const UStruct* InStruct)
			: Struct(InStruct)
		{
			if (Struct)
			{
				CurrentProperty = Struct->PropertyLink();
			}
		}

		bool IsValid() const
		{
			return CurrentProperty != nullptr;
		}

		void Next()
		{
			if (CurrentProperty)
			{
				CurrentProperty = CurrentProperty->PropertyLinkNext();
			}
		}

		UProperty* GetCurrent() const
		{
			return CurrentProperty;
		}

	private:
		const UStruct* Struct;
		UProperty* CurrentProperty;
	};

	class TNextIterator
	{
	public:
		TNextIterator(const UFunction* InStruct)
			: Struct(InStruct)
		{
			if (Struct)
			{
				CurrentProperty = reinterpret_cast<SDK::UProperty*>(Struct->Children());
			}
		}

		bool IsValid() const
		{
			return CurrentProperty != nullptr;
		}

		void Next()
		{
			if (CurrentProperty)
			{
				CurrentProperty = reinterpret_cast<SDK::UProperty*>(CurrentProperty->Next());
			}
		}

		UProperty* GetCurrent() const
		{
			return CurrentProperty;
		}
	private:
		const UFunction* Struct;
		UProperty* CurrentProperty;
	};
}
