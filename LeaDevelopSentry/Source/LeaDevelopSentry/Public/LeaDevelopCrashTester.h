// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class LEADEVELOPSENTRY_API FLeaDevelopCrashTester
{
public:
    static void InitializeFromCommandLine();

private:
    static void ExecuteCrashTest();
    static int32 CrashType;
};