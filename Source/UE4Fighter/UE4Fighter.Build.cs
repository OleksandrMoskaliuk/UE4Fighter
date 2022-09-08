// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4Fighter : ModuleRules
{
	public UE4Fighter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
