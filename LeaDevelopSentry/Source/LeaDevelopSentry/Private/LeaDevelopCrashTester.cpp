// Copyright (c) 2025 LeaDevelop. All Rights Reserved.
// TODO still needs to be connected

#include "LeaDevelopCrashTester.h"
#include "Async/Async.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "HAL/PlatformProcess.h"

// Static members
int32 FLeaDevelopCrashTester::CrashType = 0;

void FLeaDevelopCrashTester::InitializeFromCommandLine()
{
// TODO introduce WITH_SENTRY_TESTING
#if !UE_BUILD_SHIPPING
   FString CrashMeValue;
   if (FParse::Value(FCommandLine::Get(), TEXT("CrashMe="), CrashMeValue))
   {
      int32 DelaySeconds = FCString::Atoi(*CrashMeValue);
       
      if (DelaySeconds > 0)
      {
         UE_LOG(LogTemp, Warning, TEXT("LeaDevelop: Crash test scheduled in %d seconds"), DelaySeconds);
          
         AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [DelaySeconds]()
         {
            FPlatformProcess::Sleep(DelaySeconds);
            ExecuteCrashTest();
         });
      }
   }
#endif
}

void FLeaDevelopCrashTester::ExecuteCrashTest()
{
#if !UE_BUILD_SHIPPING
   UE_LOG(LogTemp, Fatal, TEXT("LEADEVELOP CRASH TEST EXECUTING!"));
   int32* NullPtr = nullptr;
   *NullPtr = 42;
#endif
}