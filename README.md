# UESDK-Lib

# HOW TO USE INITALIZE A CLASS WITH PROPERTIES
first make your class inside a header

```C++
class Inherit;

class ClassNameHere  : public Inherit //put any inherits here
{
public: 
/*
For The properties you can use either inline or non inline
if your using non inline you will have to first define the prop here (as a function ofc) and then use DECLARE_PROP_WITH_OFFSET(ReturnType, ClassName, PropertyName) in the cpp file
then for inline all you have to do is DECLARE_INLINEPROP_WITH_OFFSET(ReturnType, ClassName, PropertyName) inside the class and it will work
*/
public:
DECLARE_STATIC_CLASS(ClassNameHere) //Makes the StaticClass for you
};
```

# HOW TO USE A STRUCT PROPERLY
So lets say we have a FGameplayAbilitySpec& to our activateabilities our something.
what we need to do next is use the function SDK::UE::Memory::ResizeVirtualMemory before we actually write to it or read to it.
so lets use it.
```C++
struct ScriptStructShit
{
public:
//ofc have props here but please remember they do not change our size of the class so this is why we are doing this in the first place
};

ScriptStructShit& MagicalReference = some shit;
static int Size = ScriptStructShit::StaticClass()->Size();
void* ScriptStructPtr = SDK::UE::Memory::ResizeVirtualMemory(&MagicalReference, sizeof(MagicalReference), Size);
// you could then go on to changing that to the actual struct by casting ofc like (ScriptStructShit*)ScriptStructPtr and it would work fine
```