using UnrealBuildTool;

public class Cygnus : ModuleRules
{
	public Cygnus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new []
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"Http",
			"WebSockets",
			"Json",
			"JsonUtilities",
			"UMG",
			"Slate",
			"SlateCore"
		});
		
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = true;
		
		// TODO("Get this programmatically!")
		PublicIncludePaths.Add("D:\\Cygnus\\Cygnus\\Source\\Cygnus\\Discord\\");
			
		// TODO("Get this programmatically!")
		PublicAdditionalLibraries.Add("D:\\Cygnus\\Cygnus\\Binaries\\Win64\\discord_game_sdk.dll.lib");
	}
}
