// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaybackDataThread.h"

FPlaybackDataThread::FPlaybackDataThread()
{
	Stopping = false;
	IsActivate = true;
	IsPause = true;
	Speed = 1.0f;
	CacheDataGatherList = FDataGatherList();
	CurrentIndex = 0;
	MaxIndex = 0;
}

FPlaybackDataThread::~FPlaybackDataThread()
{
}

bool FPlaybackDataThread::Init()
{
	return !Stopping;
}

uint32 FPlaybackDataThread::Run()
{
	while (!Stopping)
	{
		if (!IsActivate)
		{
			continue;
		}

		if (IsPause)
		{
			continue;
		}

		if (CacheDataGatherList.TimeAndDataGatherArray.Num() > 0 && CurrentIndex <= MaxIndex)
		{
			FDataGather TempFDataGather = CacheDataGatherList.TimeAndDataGatherArray[CurrentIndex].DataGather;
			if (ReturnFDataGatherDelegate.IsBound())
			{
				ReturnFDataGatherDelegate.Execute(TempFDataGather);
			}

			FString TempTime1 = CacheDataGatherList.TimeAndDataGatherArray[CurrentIndex].Time;
			
			CurrentIndex++;

			if (CurrentIndex <= MaxIndex)
			{
				FString TempTime2 = CacheDataGatherList.TimeAndDataGatherArray[CurrentIndex].Time;

				const float WaitTime = UPlaybackDataDefine::FStringToTimeDifference(TempTime2, TempTime1) / Speed;
				
				FPlatformProcess::Sleep(WaitTime);
			}
		}
		else
		{
			SetPause(true);
			CacheDataGatherList = FDataGatherList();
			if (PlaybackFinishedDelegate.IsBound())
			{
				PlaybackFinishedDelegate.Execute();
			}
		}
	}

	return 0;
}

void FPlaybackDataThread::Stop()
{
	FRunnable::Stop();
	Stopping = true;
}

void FPlaybackDataThread::Exit()
{
	FRunnable::Exit();
	UE_LOG(LogTemp, Log, TEXT("%hs %s"), __FUNCTION__, TEXT("GC !"));
}

void FPlaybackDataThread::SetActivate(const bool InActivate)
{
	IsActivate = InActivate;
}

void FPlaybackDataThread::AddFDataGatherList(const FDataGatherList& InFDataGatherList)
{
	CacheDataGatherList = InFDataGatherList;
	CurrentIndex = 0;
	MaxIndex = CacheDataGatherList.TimeAndDataGatherArray.Num() - 1;
}

void FPlaybackDataThread::SetPause(const bool InPause)
{
	IsPause = InPause;
}

void FPlaybackDataThread::SetSpeed(const float InSpeed)
{
	Speed = InSpeed;
}

void FPlaybackDataThread::StopPlaybackData()
{
	IsPause = true;
	CacheDataGatherList = FDataGatherList();
}
