// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using System;

public class TapCommon : ModuleRules
{
    public TapCommon(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/Android")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public/iOS")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Tools")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "TDSNet")));

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
                "ApplicationCore",
                "HTTPServer",
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
                "Sockets",
                "UMG", 
                "WebBrowser",
                // ... add private dependencies that you statically link with here ...	
			}
            );

        AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );

        PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Analytics",
                    "Engine",
                    "RenderCore",
                    "RHI",
                    "Core",
                    "CoreUObject",
                    "Slate",
                    "SlateCore",
                    "UMG",
                    "Json",
                    "HTTP",
                    "JsonUtilities",
                    "ImageWrapper",
                }
            );

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicFrameworks.AddRange(
                new string[]{
                    "SystemConfiguration",
                    "WebKit",
                    "SystemConfiguration",
                    "CoreTelephony",
                    "MobileCoreServices",
                    "Security"
                });

            PublicAdditionalFrameworks.Add(
                new Framework(
                    "TapCommonSDK",
                    "../ThirdParty/iOS/Frameworks/TapCommonSDK.zip",
                    "TapCommonResource.bundle"
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
                Path.Combine(ModuleDirectory, "TapCommon_Android_UPL.xml")
            );
        }

    }
}
