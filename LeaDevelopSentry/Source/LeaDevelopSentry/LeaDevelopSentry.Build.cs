// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

using UnrealBuildTool;

public class LeaDevelopSentry : ModuleRules
{
	public LeaDevelopSentry(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject", 
				"Engine"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"DeveloperSettings",
				"Json",
				"Sentry",
				"RHI"
			}
		);

		PublicDefinitions.Add("WITH_LEADEVELOP_SENTRY=1");
	}
}