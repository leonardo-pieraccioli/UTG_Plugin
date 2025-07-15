using UnrealBuildTool;

public class Chemistry: ModuleRules
{
    public Chemistry(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
    }
}
