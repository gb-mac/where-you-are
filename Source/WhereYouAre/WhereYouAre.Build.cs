using UnrealBuildTool;

public class WhereYouAre : ModuleRules
{
	public WhereYouAre(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Allow subdirectory includes like "Location/WYAGeoTypes.h"
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"NetCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"HTTP",
			"Json",
			"JsonUtilities",
			"UMG",
			"CesiumRuntime",
			"AIModule",
			"NavigationSystem",
			"ChaosVehicles",
			"PhysicsCore",
		});

		// Platform-specific GPS (mobile only)
		if (Target.Platform == UnrealTargetPlatform.Android ||
		    Target.Platform == UnrealTargetPlatform.IOS)
		{
			PrivateDependencyModuleNames.Add("LocationServices");
		}

		// AI inference module (NNE) - reserved for on-device neural net inference
		// PrivateDependencyModuleNames.Add("NNERuntimeRDG");
	}
}
