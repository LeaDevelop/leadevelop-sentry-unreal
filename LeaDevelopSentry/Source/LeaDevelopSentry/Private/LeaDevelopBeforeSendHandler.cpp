// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#include "LeaDevelopBeforeSendHandler.h"
#include "LeaDevelopSentryLog.h"
#include "SentryEvent.h"
#include "SentryHint.h"
#include "SentrySubsystem.h"
#include "LeaDevelopSentrySettings.h"
#include "Misc/EngineVersion.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

USentryEvent* ULeaDevelopBeforeSendHandler::HandleBeforeSend_Implementation(USentryEvent* Event, USentryHint* Hint)
{
    // UE_LOG(LogLeaDevelopSentry, Error, TEXT("=== BeforeSend Handler Called ==="));
    
    if (!Event) 
    {
        UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Event is null!"));
        return Event;
    }

    // Get settings to check what tags to apply
    const ULeaDevelopSentrySettings* Settings = GetDefault<ULeaDevelopSentrySettings>();
    if (!Settings) 
    {
        UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Settings is null!"));
        return Event;
    }

    // Add custom tags based on settings
    if (Settings->bPromoteChangelist)
    {
        FString Changelist = FString::FromInt(FEngineVersion::Current().GetChangelist());
        Event->SetTag(TEXT("Changelist"), Changelist);
        UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Added changelist tag: %s"), *Changelist);
    }
    
    if (Settings->bPromoteEngineVersion)
    {
        FString EngineVersion = FEngineVersion::Current().ToString(EVersionComponent::Patch);
        Event->SetTag(TEXT("EngineVersion"), EngineVersion);
        UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Added engine version tag: %s"), *EngineVersion);
    }
    
    if (Settings->bPromoteLevelName)
    {
        if (GWorld)
        {
            FString LevelName = GWorld->GetMapName();
            LevelName.RemoveFromStart(GWorld->StreamingLevelsPrefix);
            Event->SetTag(TEXT("LevelName"), LevelName);
            UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Added level tag: %s"), *LevelName);
        }
    }

    // Also set global tags so they persist for crashes
    USentrySubsystem* Sentry = GEngine->GetEngineSubsystem<USentrySubsystem>();
    if (Sentry && Sentry->IsEnabled())
    {
        Settings->ApplyCustomTags(Sentry);
        UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Applied global tags for crashes"));
    }
    
    // Get current tags and log them
    TMap<FString, FString> CurrentTags = Event->GetTags();
    UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Event now has %d tags"), CurrentTags.Num());
    for (const auto& Tag : CurrentTags)
    {
        UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Tag: %s = %s"), *Tag.Key, *Tag.Value);
    }
    
    // UE_LOG(LogLeaDevelopSentry, Error, TEXT("=== BeforeSend Handler Complete ==="));
    
    return Event;
}