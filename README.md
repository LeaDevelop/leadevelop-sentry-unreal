# LeaDevelop sentry-unreal enhancements - UE plugin

Early summer 2025 I was exploring source of Sentry unreal plugin and I started adding custom tags to my self-hosted instance where I'm experimenting and prototyping different options. 
I am continuing that [adventure](https://leadevelop.net/blog/monitor-unreal-projects-in-sentry/) and am at the point of creating custom plugin. This will make updates between official Sentry plugin and own easier and makes it easier for anybody else to pick it up as well.

Custom tags has been confirmed working, but the plugin creation including that source code will be my first Plugin journey from ground up in UE world. I'll be on in coming weekends and see how far I get.
Until you don't see release version in the repository, the plugin is not functional. Stay tuned 🤓 and feel free to reach out in Discord - LeaDevelop. I'm probably listening to music and reading up [Plugins](https://dev.epicgames.com/documentation/en-us/unreal-engine/plugins-in-unreal-engine) docs at the moment.

>The use of code, scripts or shared content is solely at your own risk. I do not guarantee its accuracy, reliability, or suitability for your specific needs. No responsibility is taken for any damages or losses that may result from its use. It is recommended that you carefully review and test the content before implementation!

Current work in progress:
# LeaDevelop Sentry plugin overrides

This plugin extends and overrides the default Sentry plugin configuration for Unreal Engine projects, providing enhanced crash reporting and analytics capabilities with custom LeaDevelop features.
I'll update to latest sentry unreal version in second half of January 2026, unless one of my weekends frees up.

## Features

- **Enhanced Tag Promotion**: Automatically promotes changelist, engine version, and level name information to Sentry tags
  - unpublished tags in this project but sharing for awareness and ideas you can incorporate into your own projects: steam id, player nicknames both for steam and EGS, including any capturing nicknames in early development via different approaches depending on studio setup. Enables engineers searching for crashes by human readable names. World Context Tracking: Tags if UE developer crashed in Editor, PIE, or Game mode (currently disabled it)
- **Automatic Breadcrumb Tracking**: Enhanced breadcrumbs for map loading events
- **Configurable Overrides**: Easy-to-configure settings that override Sentry defaults

## Installation

1. Place the plugin folder in your project's `Plugins` directory
2. Enable the plugin in your project's `.uproject` file or through the Plugin Manager
3. Update GameInstance.cpp
4. Regenerate project files
5. Build your project
   
## Plugin Structure

```
Plugins\LeaDevelopSentry\
├── LeaDevelopSentry.uplugin                      # Plugin descriptor
├── Source\
│   └── LeaDevelopSentry\
│       ├── LeaDevelopSentry.Build.cs             # Build configuration
│       ├── Public\
│       │   ├── LeaDevelopSentryModule.h          # Main module header
│       │   ├── LeaDevelopSentrySettings.h        # Configuration class header
│       │   └── LeaDevelopBeforeSendHandler.h     # Filter event before sending header
│       └── Private\
│           ├── LeaDevelopBeforeSendHandler.cpp    # Filter event before sending
│           ├── LeaDevelopSentryModule.cpp         # Main module implementation
│           └── LeaDevelopSentrySettings.cpp       # Configuration class
```
<img width="631" height="108" alt="LeaDevelop_sentry-custom-tags" src="https://github.com/user-attachments/assets/f663372c-9e14-4087-ac00-ffca4d01e4de" />

<img width="1288" height="140" alt="15-09-2025-030525-Lea" src="https://github.com/user-attachments/assets/6f56fd0c-4748-4655-b1d5-25244b430795" />

## Enhanced Features

### Tag tracking

Automatically sets tags for:
- Level Name aka Map: Lvl_Default
- Changelist: 123456
- Engine Version: 5.6.1

### Automatic Breadcrumbs

Enhanced breadcrumb tracking for:
- PreLoadMap events with formatted names
- PostLoadMapWithWorld events with context
- World initialization with change detection

## Dependencies

- Sentry plugin (automatically loaded)
- Core Unreal Engine modules (Core, CoreUObject, Engine)

## Usage Notes

1. The plugin automatically applies overrides on startup
2. It respects the original Sentry plugin's configuration while enhancing it
3. All overrides are applied after the Sentry plugin initializes
4. Level name formatting preserves your custom mappings and prefixes

## Troubleshooting

- Ensure the Sentry plugin is enabled before this plugin loads
- Check logs for "LeaDevelopSentry" entries to verify initialization
- Verify that your DSN is properly configured in the base Sentry plugin settings

The plugin is designed to be easily extensible for additional Sentry customizations specific to project needs.

## Unreal Plugin docs
https://dev.epicgames.com/documentation/en-us/unreal-engine/plugins-in-unreal-engine

## Sentry docs
https://docs.sentry.io/platforms/unreal
