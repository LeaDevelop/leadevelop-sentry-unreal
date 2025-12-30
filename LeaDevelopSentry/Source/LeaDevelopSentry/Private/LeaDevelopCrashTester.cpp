// Copyright (c) 2025 LeaDevelop. All Rights Reserved.

#include "LeaDevelopCrashTester.h"

#if !UE_BUILD_SHIPPING
#include "LeaDevelopSentryLog.h"
#include "Async/Async.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "HAL/PlatformProcess.h"
#endif

void FLeaDevelopCrashTester::InitializeFromCommandLine()
{
#if !UE_BUILD_SHIPPING
   FString CrashMeValue;
   if (FParse::Value(FCommandLine::Get(), TEXT("CrashMe="), CrashMeValue))
   {
      int32 DelaySeconds = FCString::Atoi(*CrashMeValue);
       
      if (DelaySeconds > 0)
      {
         UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Crash test scheduled in %d seconds"), DelaySeconds);
          
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
   UE_LOG(LogLeaDevelopSentry, Warning, TEXT("Intended manual crash executing..."));

   int32* NullPtr = nullptr;
   *NullPtr = 42;
#endif
}