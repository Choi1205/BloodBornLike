// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BloodBorn : ModuleRules
{
	public BloodBorn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem" });//NavigationSystem을 추가해야 AI가 네비게이션 매쉬안에서 길찾기가 가능함.
    }
}
