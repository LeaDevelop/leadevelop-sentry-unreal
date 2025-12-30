**Status update** 
- release version on the way for unreal crash reporter on Unreal Engine version 5.7.1. 
  - If there is any interest either expressed directly or via stars I'll continue expand else I'll public archive the version after December 2026.
- pre-release versions are functional but not optimal. Initial development focused on getting it working and me getting better understanding; future versions will follow UE conventions better 🤞🤓
  - I'll expand existing blog post with key takeaways ETA February 2026 if not earlier.


# LeaDevelop sentry-unreal enhancements - UE plugin

I started looking into custom attributes in early spring and continue the journey during summer 2025 when I explored the source of the Sentry Unreal plugin and started adding custom tags to my self-hosted instance where I'm experimenting and prototyping different options. I started by editing a Sentry's subsystem but the more I changed, the more I realized this is not ideal for maintenance, which is what lead to creating seperate plugin. 

My C++ experience is limisted as my previous work rarely required it but I'm learning through building. This project has helped me dive into UE subsystems, modules, and plugins. I'm now at a point where I consider the plugin production-ready (release v1.4.0 for 5.6.1 on the way), though pre-release versions used suboptimal approaches.

> **Disclaimer**: The use of code, scripts, or shared content is solely at your own risk. I do not guarantee its accuracy, reliability, or suitability for your specific needs. No responsibility is taken for any damages or losses that may result from its use. It is recommended that you carefully review and test the content before implementation!

---

# LeaDevelop Sentry Enhancements - Unreal Engine Plugin

Extends the Sentry Unreal plugin with enhanced crash reporting and custom tag promotion.

> **Disclaimer**: Use at your own risk. No guarantees of accuracy, reliability, or suitability. Review and test before implementation into production or other environments of your project/s.

This plugin extends and overrides the default Sentry plugin configuration for Unreal Engine projects, providing enhanced crash reporting and analytics capabilities with custom LeaDevelop features.
I'll update to latest sentry unreal version in second half of January 2026, unless one of my weekends frees up.

## Features

### Custom tag promotion
Automatically promotes Unreal Crash Reporter metadata to Sentry tags for easier filtering:

| Tag | Description |
|-----|-------------|
| `Changelist` | UE changelist number |
| `EngineVersion` | UE version (e.g., 5.6.1) |
| `Map` | Current level/map name |
| `GameName` | Project name |
| `BuildConfig` | Debug/Development/Test/Shipping |
| `EngineMode` | Editor/Game |
| `Platform` | Platform name (e.g., Windows) |
| `CPUBrand` | Processor name |
| `GPUBrand` | Graphics card name |
| `GPUDriverVersion` | Graphics driver version |

### Additional features
- **Configurable Settings**: Enable/disable each tag in Project Settings
- **Level change tracking**: Tags update automatically on map transitions
- **Crash Tester**: Built-in command-line crash testing for validation of crash pipeline and enabling the filter out intended crashes
- **Minimal Performance Impact**: Tags set only at startup and level changes

### Future ideas (not yet implemented)
- Steam ID / Player nickname tagging
- Sentry Native SDK integration
- Additional platform support

---

## Requirements

- Unreal Engine 5.6.1+
- Sentry Unreal SDK plugin
- Windows 64-bit (additional platforms can be configured)

---

## Installation

1. Place the plugin folder in your project's `Plugins` directory
2. Enable the plugin in your project's `.uproject` file or through the Plugin Manager
3. Update GameInstance.cpp or skip this part if you use auto enabling of SDK (official docs will clarify this one for you)
4. Regenerate project files
5. Build your project
   
## Plugin Structure

All tags are enabled by default.

### 4. Verify Installation
Check **Output Log** for:
```
LogLeaDevelopSentry: Log: Module starting up
```

---

## Crash Testing

Test your Sentry integration with a controlled crash.

**In a Development packaged build:**
```cmd
YourGame.exe -CrashMe=10
```

Triggers a crash after 10 seconds. Verify custom tags appear in your Sentry dashboard.

> **Note:** Crash tester is disabled in Shipping builds.

---

## Plugin Structure
```
Plugins/LeaDevelopSentry/
├── LeaDevelopSentry.uplugin
├── Source/
│   └── LeaDevelopSentry/
│       ├── LeaDevelopSentry.Build.cs
│       ├── Public/
│       │   ├── LeaDevelopSentryModule.h
│       │   ├── LeaDevelopSentrySettings.h
│       │   ├── LeaDevelopSentryLog.h
│       │   └── LeaDevelopCrashTester.h
│       └── Private/
│           ├── LeaDevelopSentryModule.cpp
│           ├── LeaDevelopSentrySettings.cpp
│           ├── LeaDevelopSentryLog.cpp
│           └── LeaDevelopCrashTester.cpp
```

---

## How it works

The plugin uses `FGenericCrashContext::SetGameData` to inject custom tags into the Unreal Crash Reporter context. This follows the [official Sentry documentation](https://docs.sentry.io/platforms/unreal/configuration/setup-crashreporter/) for crash reporter configuration.

Tags are set:
1. Once at module startup
2. On each level change

No per-frame operations. Minimal performance impact.

---

## Troubleshooting

**Plugin doesn't appear in Plugin Manager:**
- Verify plugin is in `YourProject/Plugins/LeaDevelopSentry/`
- Check `LeaDevelopSentry.uplugin` exists
- Restart the editor

**Tags not appearing in Sentry:**
- Verify settings enabled in **Project Settings > Plugins > Sentry - LeaDevelop Enhancements**
- Ensure Sentry SDK is configured with valid DSN
- Check Output Log for startup messages

**Crash tester not working:**
- Only works in Development packaged builds
- Disabled in Shipping builds
- Requires `-CrashMe=X` parameter (X = seconds delay)

**Build errors:**
- Regenerate project files
- Clean rebuild (delete `Binaries/`, `Intermediate/`, `Saved/` folders)

---

## Changelog

### v1.4.0
- Expanded tag promotion (GameName, BuildConfig, EngineMode, Platform, CPU, GPU)
- Introduced `WITH_LEADEVELOP_SENTRY` preprocessor guard
- Crash tester excluded from Shipping builds
- Module skips initialization during cook/package commandlets
- Removed BeforeSendHandler (now uses Crash Reporter context only)
- Renamed internal function to `SetCustomCrashTags`

### Previous Versions
- Pre-release versions focused on initial functionality, are not production ready.

---

## Resources

- [Sentry Unreal Documentation by Sentry.io](https://docs.sentry.io/platforms/unreal)
- [Unreal Crash Reporter Setup by Sentry.io](https://docs.sentry.io/platforms/unreal/configuration/setup-crashreporter/)
- [Unreal Plugin Documentation by Epic Games](https://dev.epicgames.com/documentation/en-us/unreal-engine/plugins-in-unreal-engine)
- [Blog: Monitor Unreal Projects in Sentry](https://leadevelop.net/blog/monitor-unreal-projects-in-sentry/)
- [Maintain smooth game play with Sentry's game engine support by Bruno Garcia](https://blog.sentry.io/maintain-smooth-game-play-with-sentrys-game-engine-support/)
- [Unreal Engine crash reporting now available on gaming consoles with trace-connected logs by Ivan Tustanivskyi, Steve Zegalia](https://blog.sentry.io/unreal-engine-crash-reporting-now-available-on-gaming-consoles/)

---

## License

[MIT license](https://github.com/LeaDevelop/leadevelop-sentry-unreal?tab=MIT-1-ov-file)
