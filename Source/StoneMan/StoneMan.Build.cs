// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StoneMan : ModuleRules
{
	public StoneMan(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GeometryCollectionEngine", "Niagara"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[]
		{
			"StoneMan/Public",
			"StoneMan/Public/Components",
			"StoneMan/Public/Interfaces",
			"StoneMan/Public/Environments",
			"StoneMan/Public/Environments/Doors",
			"StoneMan/Public/Weapon",
			"StoneMan/Public/Weapon/DamageType",
			"StoneMan/Public/Actors",
			"StoneMan/Public/Animation",
			"StoneMan/Public/Animation/Notify",
			"StoneMan/Public/Character",
			"StoneMan/Public/Character/AICharacter",
			"StoneMan/Public/Character/PlayerCharacter",
			"StoneMan/Public/Character/Components",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
