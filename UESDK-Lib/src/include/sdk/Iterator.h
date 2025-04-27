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
}
