// Copyright (c) 2025 Sentry. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class USentrySubsystem;

/**
 * Simple utility for testing crash reporting via command line arguments.
 * Usage: prototype.exe CrashMe=60
 */
class FSentryCrashTester
{
public:
	/** Initialize crash testing from command line */
	static void InitializeFromCommandLine(USentrySubsystem* SentrySubsystem);

private:
	/** Execute the crash test */
	static void ExecuteCrashTest();
	
	/** Static reference to Sentry subsystem */
	static USentrySubsystem* SentryRef;
	
	/** Crash type counter */
	static int32 CrashType;
};

#if !UE_BUILD_SHIPPING
#define SENTRY_INIT_CRASH_TESTER(SentrySubsystem) FSentryCrashTester::InitializeFromCommandLine(SentrySubsystem)
#else
#define SENTRY_INIT_CRASH_TESTER(SentrySubsystem) 
#endif