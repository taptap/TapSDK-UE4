// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using System;

public class TapSupport : ModuleRules
{
	public TapSupport(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
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
				"Json",
				"JsonUtilities",
				"UMG",
				"HTTP",
				"TapCommon"
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
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			
			PublicAdditionalFrameworks.Add(
				new Framework(
					"TapSupportSDK",
					"../ThirdParty/iOS/Frameworks/TapSupportSDK.zip",
					"TapSupportResource.bundle"
				)
			);
		}

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
				Path.Combine(ModuleDirectory, "TapSupport_Android_UPL.xml")
			);
		}
	}
}
