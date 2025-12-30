// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#include "LeaDevelopSentryModule.h"

#if WITH_LEADEVELOP_SENTRY
#include "LeaDevelopSentrySettings.h"
#include "LeaDevelopSentryLog.h"
#include "LeaDevelopCrashTester.h"
//#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Misc/App.h"
#include "Misc/EngineVersion.h"
#include "HAL/PlatformMisc.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "RHI.h"
#endif

#define LOCTEXT_NAMESPACE "FLeaDevelopSentryModule"

void FLeaDevelopSentryModule::StartupModule()
{
#if WITH_LEADEVELOP_SENTRY
    // Skips during cook / package commandlets
    if (IsRunningCommandlet())
    {
        UE_LOG(LogLeaDevelopSentry, Log, TEXT("Skipping initialization (commandlet)"));
        return;
    }
    
    // Initialize crash tester
    FLeaDevelopCrashTester::InitializeFromCommandLine();

    // TODO This is not optimal solution, follow UE conventions, delegate or research other options
    // Set global tags early for crashes using AsyncTask
    AsyncTask(ENamedThreads::GameThread, []()
    {
        FPlatformProcess::Sleep(1.0f);
        
        USentrySubsystem* Sentry = GEngine->GetEngineSubsystem<USentrySubsystem>();
        if (Sentry && Sentry->IsEnabled())
        {
            // Skips during in-editor cooking
            if (!World || GIsCookerLoadingPackage)
            {
                return;
            }

            FString LevelName = World->GetMapName();
            LevelName.RemoveFromStart(World->StreamingLevelsPrefix);
			
            if (LastLevelName != LevelName)
            {
                LastLevelName = LevelName;
                SetCustomCrashTags(LevelName);
            }
        });

#if !UE_BUILD_SHIPPING
    FLeaDevelopCrashTester::InitializeFromCommandLine();
#endif
#endif
}

void FLeaDevelopSentryModule::ShutdownModule()
{
#if WITH_LEADEVELOP_SENTRY
    if (WorldInitializedDelegate.IsValid())
    {
        FWorldDelegates::OnPostWorldInitialization.Remove(WorldInitializedDelegate);
    }
	
    UE_LOG(LogLeaDevelopSentry, Log, TEXT("Module shutting down"));
#endif
}

#if WITH_LEADEVELOP_SENTRY
void FLeaDevelopSentryModule::SetCustomCrashTags(const FString& LevelName)
{
    const ULeaDevelopSentrySettings* Settings = GetDefault<ULeaDevelopSentrySettings>();
    if (!Settings)
    {
        return;
    }
    
    TSharedPtr<FJsonObject> Config = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> Tags = MakeShareable(new FJsonObject);
    
    if (Settings->bPromoteChangelist)
    {
        Tags->SetStringField(TEXT("Changelist"),
            FString::FromInt(FEngineVersion::Current().GetChangelist()));
    }

    if (Settings->bPromoteEngineVersion)
    {
        Tags->SetStringField(TEXT("EngineVersion"),
            FEngineVersion::Current().ToString(EVersionComponent::Patch));
    }
    
    if (Settings->bPromoteLevelName && !LevelName.IsEmpty())
    {
        Tags->SetStringField(TEXT("Map"), LevelName);
    }

    if (Settings->bPromoteGameName)
    {
        Tags->SetStringField(TEXT("GameName"), FApp::GetProjectName());
    }

    if (Settings->bPromoteBuildConfiguration)
    {
        FString BuildConfig;
#if UE_BUILD_DEBUG
        BuildConfig = TEXT("Debug");
#elif UE_BUILD_DEVELOPMENT
        BuildConfig = TEXT("Development");
#elif UE_BUILD_TEST
        BuildConfig = TEXT("Test");
#elif UE_BUILD_SHIPPING
        BuildConfig = TEXT("Shipping");
#else
        BuildConfig = TEXT("Unknown");
#endif
        Tags->SetStringField(TEXT("BuildConfig"), BuildConfig);
    }

    if (Settings->bPromoteEngineMode)
    {
        Tags->SetStringField(TEXT("EngineMode"), GIsEditor ? TEXT("Editor") : TEXT("Game"));
    }

    if (Settings->bPromotePlatform)
    {
        Tags->SetStringField(TEXT("Platform"), FPlatformProperties::PlatformName());
    }

    if (Settings->bPromoteCPUBrand)
    {
        Tags->SetStringField(TEXT("CPUBrand"), FPlatformMisc::GetCPUBrand());
    }
    
    if (Settings->bPromoteGPUBrand)
    {
        Tags->SetStringField(TEXT("GPUBrand"), GRHIAdapterName);
    }

    if (Settings->bPromoteGPUDriverVersion)
    {
        Tags->SetStringField(TEXT("GPUDriverVersion"), GRHIAdapterUserDriverVersion);
    }
    
    Config->SetObjectField(TEXT("tags"), Tags);

    FString JsonConfig;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonConfig);
    FJsonSerializer::Serialize(Config.ToSharedRef(), JsonWriter);

    FGenericCrashContext::SetGameData(TEXT("__sentry"), JsonConfig);
}
#endif

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLeaDevelopSentryModule, LeaDevelopSentry)
