# LeaDevelop Sentry Enhancements - Unreal Engine Plugin

Extends the Sentry Unreal plugin with enhanced crash reporting and custom tag promotion.

> **Disclaimer**: Use at your own risk. No guarantees of accuracy, reliability, or suitability. Review and test before implementation into production or other environments of your project/s.

---

## Status

- ✅ Release v1.4.0 tested on UE 5.6.1
- ✅ Compatible with Sentry SDK v1.2.0, v1.3.0, v1.4.0
- 🔄 UE 5.7.1 testing in progress
- 📦 Focuses on Unreal Crash Reporter 
  - Sentry Native SDK integration is currently not primary focus, I confirmed it's working for all automated unattended crashes and will potentially re-add that at later time again.

I see downloads but no stars — is anyone out there? 👀 A star, some [feedback](https://github.com/LeaDevelop/leadevelop-sentry-unreal/discussions/6), a friendly wave, any sign of life helps me keep 😎 maintaining the repo. 

---

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

### 1. Install the Plugin
1. Download and extract the plugin package
2. Copy `LeaDevelopSentry` folder to `YourProject/Plugins/`
3. Regenerate project files

### 2. Enable the Plugin
1. Open project in Unreal Editor
2. **Edit > Plugins** > Search "LeaDevelop Sentry"
3. Check **Enabled** and restart

### 3. Configure Settings
1. **Edit > Project Settings**
2. Navigate to **Plugins > Sentry - LeaDevelop Enhancements**
3. Enable/disable tags as needed

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
- Pre-release versions focused on initial functionality, are not production ready. I have removed them as I'd not recommend to use them.

---

## Resources
- [Blog: Monitor Unreal Projects in Sentry](https://leadevelop.net/blog/monitor-unreal-projects-in-sentry/)
- [Sentry Unreal Documentation by Sentry.io](https://docs.sentry.io/platforms/unreal)
- [Unreal Crash Reporter Setup by Sentry.io](https://docs.sentry.io/platforms/unreal/configuration/setup-crashreporter/)
- [Unreal Plugin Documentation by Epic Games](https://dev.epicgames.com/documentation/en-us/unreal-engine/plugins-in-unreal-engine)
- [Maintain smooth game play with Sentry's game engine support by Bruno Garcia](https://blog.sentry.io/maintain-smooth-game-play-with-sentrys-game-engine-support/)
- [Unreal Engine crash reporting now available on gaming consoles with trace-connected logs by Ivan Tustanivskyi, Steve Zegalia](https://blog.sentry.io/unreal-engine-crash-reporting-now-available-on-gaming-consoles/)

---

## License

[MIT license](https://github.com/LeaDevelop/leadevelop-sentry-unreal?tab=MIT-1-ov-file)
