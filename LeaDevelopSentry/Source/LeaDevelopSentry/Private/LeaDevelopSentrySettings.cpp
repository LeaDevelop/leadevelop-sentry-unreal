// Copyright (c) 2025 LeaDevelop. All Rights Reserved.
#include "LeaDevelopSentrySettings.h"
#include "SentrySubsystem.h"
#include "Misc/EngineVersion.h"
#include "Engine/Engine.h"

void ULeaDevelopSentrySettings::ApplyCustomTags(USentrySubsystem* Sentry) const
{
	if (!Sentry || !Sentry->IsEnabled()) return;

	TMap<FString, FSentryVariant> CustomContext;
    
	if (bPromoteChangelist)
	{
		FString Changelist = FString::FromInt(FEngineVersion::Current().GetChangelist());
		Sentry->SetTag(TEXT("Changelist"), Changelist);
		CustomContext.Add(TEXT("Changelist"), Changelist);
	}
    
	if (bPromoteEngineVersion)
	{
		FString EngineVersion = FEngineVersion::Current().ToString(EVersionComponent::Patch);
		Sentry->SetTag(TEXT("EngineVersion"), EngineVersion);
		CustomContext.Add(TEXT("Engine Version"), EngineVersion);
	}
    
	if (bPromoteLevelName && GWorld)
	{
		FString LevelName = GWorld->GetMapName();
		LevelName.RemoveFromStart(GWorld->StreamingLevelsPrefix);
		Sentry->SetTag(TEXT("LevelName"), LevelName);
		CustomContext.Add(TEXT("Level Name"), LevelName);
	}

	if (!CustomContext.IsEmpty())
	{
		Sentry->SetContext(TEXT("LeaDevelop"), CustomContext);
	}
}

#if WITH_EDITOR
void ULeaDevelopSentrySettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
    
	if (USentrySubsystem* Sentry = GEngine->GetEngineSubsystem<USentrySubsystem>())
	{
		ApplyCustomTags(Sentry);
	}
}
#endif