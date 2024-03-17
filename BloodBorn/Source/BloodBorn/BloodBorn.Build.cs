// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BloodBorn : ModuleRules
{
	public BloodBorn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem" });//NavigationSystem�� �߰��ؾ� AI�� �׺���̼� �Ž��ȿ��� ��ã�Ⱑ ������.
    }
}
