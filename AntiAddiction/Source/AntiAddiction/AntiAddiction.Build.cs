// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using System;

public class AntiAddiction : ModuleRules
{
	public AntiAddiction(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		string configFile = Path.Combine(PluginDirectory, "Content/Assets/**");
		RuntimeDependencies.Add(configFile);
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"InputCore",
				"TapCommon",
				"Json",
				"JsonUtilities",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{

			PublicAdditionalFrameworks.Add(
				new Framework(
					"AntiAddictionService",
					"../ThirdParty/iOS/Frameworks/AntiAddictionService.zip"
				)
			);
			
			PublicAdditionalFrameworks.Add(
				new Framework(
					"AntiAddictionUI",
					"../ThirdParty/iOS/Frameworks/AntiAddictionUI.zip",
					"AntiAdictionResources.bundle"
				)
			);
		}
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Launch"
				}
			);
			AdditionalPropertiesForReceipt.Add(
				"AndroidPlugin",
				Path.Combine(ModuleDirectory, "AntiAddiction_Android_UPL.xml")
			);
		}
	}
}
