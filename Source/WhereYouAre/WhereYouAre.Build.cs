using UnrealBuildTool;

public class WhereYouAre : ModuleRules
{
	public WhereYouAre(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"HTTP",
			"Json",
			"JsonUtilities",
		});

		// Location/GPS services (platform-specific additions later)
		// AI inference module (NNE) - added when AI pipeline is wired in
		// PrivateDependencyModuleNames.Add("NNERuntimeRDG");
	}
}
