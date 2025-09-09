// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SentrySubsystem.h"
#include "LeaDevelopSentry.h"
#include "Modules/ModuleManager.h"

class FLeaDevelopSentryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
