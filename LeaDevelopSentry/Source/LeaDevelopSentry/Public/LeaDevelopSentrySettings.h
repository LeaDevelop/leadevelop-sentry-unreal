// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LeaDevelopSentrySettings.generated.h"

UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="Sentry Enhancements"))
class LEADEVELOPSENTRY_API ULeaDevelopSentrySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }
	virtual FName GetSectionName() const override { return TEXT("LeaDevelop Sentry Enhancements"); }

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote changelist", ToolTip="Adds Unreal Engine changelist as tag to crash reports"))
	bool bPromoteChangelist = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote UE version", ToolTip="Adds Unreal Engine (UE) changelist as tag to crash reports"))
	bool bPromoteEngineVersion = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote level name", ToolTip="Adds map name as tag to crash reports"))
	bool bPromoteLevelName = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote game name", ToolTip="Adds project name as tag to crash reports"))
	bool bPromoteGameName = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote build configuration", ToolTip="Adds build config (Development/Shipping/etc) as tag to crash reports"))
	bool bPromoteBuildConfiguration = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote engine mode", ToolTip="Adds engine mode (Editor/Game) as tag to crash reports"))
	bool bPromoteEngineMode = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote platform", ToolTip="Adds platform name as tag to crash reports"))
	bool bPromotePlatform = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote CPU brand", ToolTip="Adds CPU brand as tag to crash reports"))
	bool bPromoteCPUBrand = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote GPU brand", ToolTip="Adds GPU name as tag to crash reports"))
	bool bPromoteGPUBrand = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags", meta = (DisplayName="Promote GPU driver version", ToolTip="Adds GPU driver version as tag to crash reports"))
	bool bPromoteGPUDriverVersion = true;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};