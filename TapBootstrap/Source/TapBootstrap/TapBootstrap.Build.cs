// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using System;

public class TapBootstrap : ModuleRules
{
    public TapBootstrap(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));

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
				"TapCommon",
                "Json",
				"JsonUtilities",
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
            // PublicAdditionalFrameworks.Add(
            //     new Framework(
            //         "TapBootstrapSDK",
            //         "../TapBootstrap/ios/TapBoostrapSDK.embeddedframework.zip",
            //         "Resource/TapBootstrapResource.bundle"
            //     )
            // );
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
        }

    }
}
