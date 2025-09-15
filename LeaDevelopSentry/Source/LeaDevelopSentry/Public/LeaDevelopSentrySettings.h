#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LeaDevelopSentrySettings.generated.h"

class USentrySubsystem;

UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="LeaDevelop Sentry"))
class LEADEVELOPSENTRY_API ULeaDevelopSentrySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }
	virtual FName GetSectionName() const override { return TEXT("LeaDevelop Sentry"); }

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags")
	bool bPromoteChangelist = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags")
	bool bPromoteEngineVersion = true;

	UPROPERTY(Config, EditAnywhere, Category = "Custom Tags")
	bool bPromoteLevelName = true;

	void ApplyCustomTags(USentrySubsystem* Sentry) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};