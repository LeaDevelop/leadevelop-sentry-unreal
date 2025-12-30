// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FLeaDevelopSentryModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
#if WITH_LEADEVELOP_SENTRY
	void SetCustomCrashTags(const FString& LevelName = TEXT(""));
	
	FDelegateHandle WorldInitializedDelegate;
	FString LastLevelName;
#endif
};