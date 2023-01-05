// Some copyright should be here...

using UnrealBuildTool;
using System.IO;

public class TapBootstrap : ModuleRules
{
	public TapBootstrap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Android")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/iOS")));
		PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "ApplicationCore",
                "UMG",
                "Json",
                "JsonUtilities",
                "TapCommon",
                "TapLogin",
                "LeanCloud",
                "HTTP"
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
                    "TapBootstrapSDK",
                    "../ThirdParty/iOS/Frameworks/TapBootstrapSDK.zip"
                )
            );
            PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/IOS")));

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
                Path.Combine(ModuleDirectory, "TapBootstrap_Android_UPL.xml")
            );
            PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private/Android")));

        }

    }
}

