// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "SentryBeforeSendHandler.h"
#include "LeaDevelopBeforeSendHandler.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LEADEVELOPSENTRY_API ULeaDevelopBeforeSendHandler : public USentryBeforeSendHandler
{
	GENERATED_BODY()

public:
	virtual USentryEvent* HandleBeforeSend_Implementation(USentryEvent* Event, USentryHint* Hint) override;
};