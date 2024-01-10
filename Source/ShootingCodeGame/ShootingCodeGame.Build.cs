// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootingCodeGame : ModuleRules
{
	public ShootingCodeGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "ItemPlugin", "OnlineSubsystem", "OnlineSubsystemUtils" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

		// 경로 추가
		PublicIncludePaths.AddRange(new string[] { "ShootingCodeGame", "ShootingCodeGame/Public/Blueprints" });
    }
}
