# LeaDevelop sentry-unreal enhancements - UE plugin

During summer 2025 I was exploring the source of the Sentry Unreal plugin and started adding custom tags to my self-hosted instance where I'm experimenting and prototyping different options. I started by editing a Sentry's subsystem but the more I changed, the more I realized this is not ideal for maintenance, which is what lead to creating own plugin.

> **Disclaimer**: The use of code, scripts, or shared content is solely at your own risk. I do not guarantee its accuracy, reliability, or suitability for your specific needs. No responsibility is taken for any damages or losses that may result from its use. It is recommended that you carefully review and test the content before implementation!

---

# LeaDevelop Sentry Plugin Enhancements

This plugin extends the default Sentry plugin for Unreal Engine projects, providing enhanced crash reporting and analytics capabilities with custom LeaDevelop features.

## Features

- **Enhanced Tag Promotion**: Automatically promotes changelist, engine version, and level name information to Sentry tags
    - Unpublished tags in this project but sharing for awareness and ideas you can incorporate: Steam ID, player nicknames (both Steam and EGS), capturing nicknames in early development via different approaches depending on studio setup. Enables engineers to search for crashes by human-readable names.
    - World Context Tracking: Tags whether UE developer crashed in Editor, PIE, or Game mode (currently disabled)
- **Automatic Breadcrumb Tracking**: Enhanced breadcrumbs for map loading events and world initialization
- **Configurable Settings**: Easy-to-configure settings that extend Sentry defaults
- **Crash Testing**: Built-in command-line crash tester for validating your Sentry integration

## Requirements

- Unreal Engine 5.6.1 or later (high chance it works with 5.5 didn't have time to validate yet, try at your own risk!)
- Sentry Unreal Engine SDK plugin (available on Epic Marketplace)
- Windows 64-bit (additional platforms can be configured)

## Installation

### 1. Install the Plugin Files
1. Download and extract the plugin package
2. Copy the `LeaDevelopSentry` folder to your project's `Plugins` directory
    - If the `Plugins` folder doesn't exist, create it in your project root
    - Path should be: `YourProject/Plugins/LeaDevelopSentry/`
3. **Do NOT enable the plugin yet**

### 2. Configure Your Game Instance or use auto enable SDK option

You have to choose which way you'll use it. I advise reading through official documentaition and you try things around.

You need to set up the Sentry integration in your project's GameInstance class **before enabling the plugin**.

#### If you already have a custom GameInstance:

Add the following to your **GameInstance.h**:
```cpp
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "YourGameInstance.generated.h"

UCLASS()
class YOURPROJECT_API UYourGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

private:
	UPROPERTY()
	FTimerHandle SentryTagsTimerHandle;    
};
```

Add the following to your **GameInstance.cpp**:
```cpp
#include "YourGameInstance.h"
#include "SentrySubsystem.h"
#include "SentrySettings.h"
#include "LeaDevelopBeforeSendHandler.h"

void UYourGameInstance::Init()
{
    Super::Init();
    
    // Configure Sentry to use LeaDevelop's custom handler
    if (USentrySettings* Settings = GetMutableDefault<USentrySettings>())
    {
        Settings->BeforeSendHandler = ULeaDevelopBeforeSendHandler::StaticClass();
    }
    
    // Test event
    FTimerHandle TestTimer;
    GetTimerManager().SetTimer(TestTimer, []()
    {
        if (USentrySubsystem* Sentry = GEngine->GetEngineSubsystem<USentrySubsystem>())
        {
            Sentry->CaptureMessage(TEXT("Test tags"), ESentryLevel::Warning);
        }
    }, 3.0f, false);
}
```

#### If you don't have a custom GameInstance yet:

1. Create **YourGameInstance.h** in your project's `Source/YourProject/` folder (replace `YourProject` with your actual project name)
2. Create **YourGameInstance.cpp** in the same folder
3. Use the code examples above, replacing `YOURPROJECT_API` with your project's API macro

Then:

3. **Set the GameInstance in Project Settings:**
    - Open your project in Unreal Editor
    - Go to **Edit > Project Settings**
    - Navigate to **Maps & Modes**
    - Under **Game Instance**, select your custom **YourGameInstance** class
    - Save and close the editor

### 3. Update Build Configuration

Add the plugin dependencies to your project's **Build.cs** file:
```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core", 
    "CoreUObject", 
    "Engine",
    "Sentry"  // Add this if not already present
});

PrivateDependencyModuleNames.AddRange(new string[]
{
    "LeaDevelopSentry"  // Add this
});
```

### 4. Regenerate Project Files

1. **Close Unreal Editor completely**
2. Right-click your `.uproject` file
3. Select **Generate Visual Studio project files**

### 5. Enable the Plugin

1. Open your project in Unreal Engine
2. Go to **Edit > Plugins**
3. Search for "LeaDevelop Sentry"
4. Check the **Enabled** checkbox
5. Click **Restart Now** when prompted

### 6. Build Your Project

1. After restart, the editor may prompt you to rebuild
2. Click **Yes** to build
3. Alternatively, build from your IDE:
    - Open the `.sln` file (Visual Studio) or `.uproject` (Rider/other IDEs)
    - **Build > Build Solution**
4. Launch your project

### 7. Configure Plugin Settings

1. In Unreal Editor, go to **Edit > Project Settings**
2. Navigate to **Plugins > LeaDevelop Sentry**
3. Configure which tags you want to promote:
    - **Promote Changelist**: Include engine changelist in events
    - **Promote Engine Version**: Include engine version in events
    - **Promote Level Name**: Include current level name in events

<img width="631" height="108" alt="LeaDevelop_sentry-custom-tags" src="https://github.com/user-attachments/assets/f663372c-9e14-4087-ac00-ffca4d01e4de" />

### 8. Configure Sentry SDK Settings

1. Go to **Edit > Project Settings**
2. Navigate to **Plugins > Sentry**
3. Configure your Sentry DSN and other settings
4. The LeaDevelop Sentry handler will automatically be applied

## Testing the Installation

To verify the plugin is working:

1. Launch your project in the editor
2. Check the **Output Log** for messages like:
```
   LogTemp: Warning: LeaDevelopSentry module starting up
   LogTemp: Warning: Set BeforeSend handler: SUCCESS
   LogTemp: Warning: Applied startup global tags
```

3. To test crash reporting in a packaged build using the command line:
```cmd
   YourGame.exe -CrashMe=5
```
This will trigger a test crash after 5 seconds. Check your Sentry dashboard to verify the crash was captured with custom tags.

<img width="1288" height="140" alt="Sentry Dashboard Example" src="https://github.com/user-attachments/assets/6f56fd0c-4748-4655-b1d5-25244b430795" />

## Plugin Structure
```
Plugins/LeaDevelopSentry/
├── LeaDevelopSentry.uplugin                      # Plugin descriptor
├── Source/
│   └── LeaDevelopSentry/
│       ├── LeaDevelopSentry.Build.cs             # Build configuration
│       ├── Public/
│       │   ├── LeaDevelopSentryModule.h          # Main module header
│       │   ├── LeaDevelopSentrySettings.h        # Configuration class header
│       │   ├── LeaDevelopBeforeSendHandler.h     # Event filter header
│       │   └── LeaDevelopCrashTester.h           # Crash testing utility header
│       └── Private/
│           ├── LeaDevelopSentryModule.cpp        # Main module implementation
│           ├── LeaDevelopSentrySettings.cpp      # Configuration class
│           ├── LeaDevelopBeforeSendHandler.cpp   # Event filter implementation
│           └── LeaDevelopCrashTester.cpp         # Crash testing utility
```

## Enhanced Features

### Automatic Tag Promotion

The plugin automatically adds custom tags to every Sentry event:
- **LevelName**: Current map/level name (e.g., `Lvl_Default`)
- **Changelist**: Engine changelist number (e.g., `123456`)
- **EngineVersion**: Unreal Engine version (e.g., `5.6.1`)

These tags make it easier to:
- Filter crashes by specific maps/levels
- Track issues across different engine versions
- Correlate crashes with specific builds

### Automatic Breadcrumbs

Enhanced breadcrumb tracking for debugging:
- PreLoadMap events with formatted map names
- PostLoadMapWithWorld events with world context
- World initialization with change detection
- Game state changes with engine metadata

### Crash Testing

Built-in command-line crash tester for validation:
```cmd
YourGame.exe -CrashMe=5
```
Triggers a controlled crash after the specified delay (in seconds) to test your Sentry integration.

## Dependencies

- Sentry plugin (automatically loaded as dependency)
- Core Unreal Engine modules (Core, CoreUObject, Engine)

## Usage Notes

1. The plugin automatically applies enhancements on startup
2. It extends the original Sentry plugin's configuration without replacing it
3. All custom tags are applied both to real-time events and crash reports
4. Level name formatting preserves your custom mappings and prefixes
5. The BeforeSend handler runs for every event, allowing you to customize event data before sending

## Troubleshooting

**"LeaDevelopBeforeSendHandler not found" error:**
- Make sure you enabled the plugin AFTER creating the GameInstance files
- Verify `LeaDevelopSentry` is added to your Build.cs `PrivateDependencyModuleNames`
- Regenerate project files and rebuild

**Plugin doesn't appear in Plugin Manager:**
- Verify the plugin is in `YourProject/Plugins/LeaDevelopSentry/`
- Check that `LeaDevelopSentry.uplugin` exists in the plugin folder
- Restart the editor

**Build errors:**
- Ensure you've added the dependencies to your Build.cs file
- Regenerate project files after making changes
- Clean and rebuild (delete `Binaries/`, `Intermediate/`, and `Saved/` folders)

**Tags not appearing in Sentry:**
- Verify the plugin settings are enabled in **Project Settings > Plugins > LeaDevelop Sentry**
- Check that your GameInstance properly sets the BeforeSend handler
- Ensure the Sentry SDK plugin is also installed and configured with a valid DSN
- Check the Output Log for "Applied global tags" messages

**Crash tester not working:**
- Only works in packaged builds (Shipping and Development configurations)
- Disabled in editor builds for safety
- Requires the `-CrashMe=X` command-line parameter (where X is delay in seconds)

## Extending the Plugin

The plugin is designed to be easily extensible for additional Sentry customizations. Key extension points:

- **LeaDevelopBeforeSendHandler**: Modify event data before sending to Sentry
- **LeaDevelopSentrySettings**: Add new configurable options
- **LeaDevelopSentryModule**: Hook into engine lifecycle events

## Resources

- **Unreal Plugin Documentation**: https://dev.epicgames.com/documentation/en-us/unreal-engine/plugins-in-unreal-engine
- **Sentry Unreal Documentation**: https://docs.sentry.io/platforms/unreal
- **Blog Post**: [Monitor Unreal Projects in Sentry](https://leadevelop.net/blog/monitor-unreal-projects-in-sentry/)

## License

[MIT license](https://github.com/LeaDevelop/leadevelop-sentry-unreal?tab=MIT-1-ov-file)
