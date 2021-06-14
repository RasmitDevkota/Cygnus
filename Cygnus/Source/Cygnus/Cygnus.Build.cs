using UnrealBuildTool;

public class Cygnus : ModuleRules
{
	public Cygnus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Http", "Json", "JsonUtilities", "UMG", "Slate", "SlateCore" });
		
		 MinFilesUsingPrecompiledHeaderOverride = 1;
         bUseUnity = true;
	}
}
