// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#include "LeaDevelopSentryModule.h"
#include "LeaDevelopSentryLog.h"
#include "SentrySettings.h"
#include "SentrySubsystem.h"
#include "LeaDevelopBeforeSendHandler.h"
#include "LeaDevelopCrashTester.h"
#include "Engine/Engine.h"
#include "Async/Async.h"
#include "HAL/PlatformProcess.h"
#include "LeaDevelopSentrySettings.h"
#include "Misc/EngineVersion.h"

#define LOCTEXT_NAMESPACE "FLeaDevelopSentryModule"

void FLeaDevelopSentryModule::StartupModule()
{
    //UE_LOG(LogLeaDevelopSentry, Warning, TEXT("LeaDevelopSentry module starting up"));
    
    // Auto-configure Sentry to use our handler
    if (USentrySettings* SentrySettings = GetMutableDefault<USentrySettings>())
    {
        SentrySettings->BeforeSendHandler = ULeaDevelopBeforeSendHandler::StaticClass();
        SentrySettings->SaveConfig();
        UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Set BeforeSend handler: %s"), 
            SentrySettings->BeforeSendHandler ? TEXT("SUCCESS") : TEXT("FAILED"));
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
            const ULeaDevelopSentrySettings* Settings = GetDefault<ULeaDevelopSentrySettings>();
            if (Settings)
            {
                Settings->ApplyCustomTags(Sentry);
                UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Applied startup global tags"));
            }
        }
    });
    
    // Add breadcrumb on game state changes
    GameStateDelegate = FCoreDelegates::GameStateClassChanged.AddLambda([](const FString& GameState)
    {
        USentrySubsystem* Sentry = GEngine->GetEngineSubsystem<USentrySubsystem>();
        if (Sentry && Sentry->IsEnabled())
        {
            TMap<FString, FSentryVariant> Data;
            Data.Add(TEXT("Changelist"), FString::FromInt(FEngineVersion::Current().GetChangelist()));
            Data.Add(TEXT("Engine Version"), FEngineVersion::Current().ToString(EVersionComponent::Patch));
            Data.Add(TEXT("Game State"), GameState);
            
            Sentry->AddBreadcrumbWithParams(TEXT("LeaDevelop Info"), TEXT("System"), TEXT("Info"), Data, ESentryLevel::Info);
        }
    });
}

void FLeaDevelopSentryModule::ShutdownModule()
{
    if (GameStateDelegate.IsValid())
    {
        FCoreDelegates::GameStateClassChanged.Remove(GameStateDelegate);
    }
    //UE_LOG(LogLeaDevelopSentry, Log, TEXT("LeaDevelopSentry module shutting down"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FLeaDevelopSentryModule, LeaDevelopSentry)
