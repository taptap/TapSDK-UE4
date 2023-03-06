// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
public class LeanCloudPush : ModuleRules
{
	public LeanCloudPush(ReadOnlyTargetRules Target) : base(Target)
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
				"Http",
				"Json",
				"JsonUtilities",
				"LeanCloud"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
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
				Path.Combine(ModuleDirectory, "LeanCloudPush_Android_UPL.xml")
			);
		}
	}
}
