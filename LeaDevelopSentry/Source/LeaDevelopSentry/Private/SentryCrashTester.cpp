// Copyright (c) 2025 Sentry. All Rights Reserved.

#include "SentryCrashTester.h"
#include "SentrySubsystem.h"
#include "Async/Async.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "HAL/PlatformProcess.h"
#include "Misc/DateTime.h"
#include "Async/AsyncWork.h"

// Use LogTemp instead of LogSentrySdk to avoid linking issues

// Static members
USentrySubsystem* FSentryCrashTester::SentryRef = nullptr;
int32 FSentryCrashTester::CrashType = 0;

void FSentryCrashTester::InitializeFromCommandLine(USentrySubsystem* SentrySubsystem)
{
#if !UE_BUILD_SHIPPING
    if (!SentrySubsystem)
    {
       return;
    }

    SentryRef = SentrySubsystem;

    FString CrashMeValue;
    if (FParse::Value(FCommandLine::Get(), TEXT("CrashMe="), CrashMeValue))
    {
       int32 DelaySeconds = FCString::Atoi(*CrashMeValue);
       
       if (DelaySeconds > 0)
       {
          // Only log locally, don't send to Sentry yet
          UE_LOG(LogTemp, Warning, TEXT("Crash test scheduled in %d seconds"), DelaySeconds);
          
          // Simple async task to crash after delay
          AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [DelaySeconds]()
          {
             FPlatformProcess::Sleep(DelaySeconds);
             ExecuteCrashTest();
          });
       }
    }
#endif
}

void FSentryCrashTester::ExecuteCrashTest()
{
#if !UE_BUILD_SHIPPING
    if (SentryRef)
    {
       // Set context tags that will be included with the crash report
       SentryRef->SetTag(TEXT("CrashTest"), TEXT("CommandLine"));
       SentryRef->SetTag(TEXT("CrashType"), TEXT("NullPointerDereference"));
       
       // Add breadcrumb for crash context (these don't send events, just context)
       SentryRef->AddBreadcrumbWithParams(
          TEXT("Executing crash test"), 
          TEXT("CrashTest"), 
          TEXT("Debug"), 
          {}, 
          ESentryLevel::Info
       );
    }
    
    // The crash itself - this will be automatically captured by Sentry
    UE_LOG(LogTemp, Fatal, TEXT("CRASH TEST EXECUTING!"));
    int32* NullPtr = nullptr;
    *NullPtr = 42; // This crash will be automatically sent to Sentry
#endif
}