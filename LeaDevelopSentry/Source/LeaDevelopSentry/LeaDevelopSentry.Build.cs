// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

// TODO revise the list

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
                "Engine",
                "Sentry"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "EngineSettings",
                "Projects"
            }
        );
    }
}