// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaybackComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "QingLongXia/GeneralBlueprintFunctionLibrary.h"
#include "QingLongXia/Manager/GameManager.h"
#include "QingLongXia/PlaybackDataModule/DataDefine/PlaybackDataDefine.h"

UPlaybackComponent::UPlaybackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	GameManagerPtr = nullptr;
	IsBeBeingPlayback = false;
	IsDataCollection = false;
	IsSelectedData = false;
	StartTime = FDateTime();
	EndTime = FDateTime();
	MaxID = 0;
	DataGatherList = FDataGatherList();
	PlaybackDataIndexArray = FPlaybackDataIndexArray();
	PlaybackDataIndex = FPlaybackDataIndex();

	PlaybackDataThread_Handle = nullptr;
	PlaybackDataThread = nullptr;
}

void UPlaybackComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//设置Tick组为TG_PostUpdateWork，用于控制组件TickComponent在所有默认主Tick执行完成后在执行
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostUpdateWork;
}

void UPlaybackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsDataCollection && GameManagerPtr->IsValidLowLevel())
	{
		FTimeAndDataGather TempTimeAndDataGather = FTimeAndDataGather();
		const FTimespan TempTimespan = FDateTime::Now() - StartTime;
		const FDateTimeStringStruct TempTimespanStruct = UGeneralBlueprintFunctionLibrary::ConvertFTimespanToDateTimeStringStruct(TempTimespan);
		const FString TempTimespanString = TempTimespanStruct.Hour + TIME_DATAGATHER + TempTimespanStruct.Minute + TIME_DATAGATHER + TempTimespanStruct.Second + TIME_DATAGATHER + TempTimespanStruct.Millisecond;
		TempTimeAndDataGather.Time = TempTimespanString;
		TempTimeAndDataGather.DataGather.TerminalStateInfoArray = GameManagerPtr->Cache4TerminalStateInfo;
		TempTimeAndDataGather.DataGather.BattleAreaInfo = GameManagerPtr->Cache4BattleAreaInfo;
		TempTimeAndDataGather.DataGather.CannoBallFallInfo = GameManagerPtr->Cache4CannoBallFallInfo;
		TempTimeAndDataGather.DataGather.BunkerInfo = GameManagerPtr->Cache4BunkerInfo;
		TempTimeAndDataGather.DataGather.DodgePointInfo = GameManagerPtr->Cache4DodgePointInfo;
		TempTimeAndDataGather.DataGather.ConstractionInfo = GameManagerPtr->Cache4ConstractionInfo;
		TempTimeAndDataGather.DataGather.ForceDispositionInfo = GameManagerPtr->Cache4ForceDispositionInfo;
		TempTimeAndDataGather.DataGather.DrillStageInfo = GameManagerPtr->Cache4DrillStageInfo;
		TempTimeAndDataGather.DataGather.DrillPlanInfo = GameManagerPtr->Cache4DrillPlanInfo;
		TempTimeAndDataGather.DataGather.XdxxArray = GameManagerPtr->XdxxArray;
		TempTimeAndDataGather.DataGather.DaJiArray = GameManagerPtr->DaJiArray;
		TempTimeAndDataGather.DataGather.MarshallingInfo = GameManagerPtr->Cache4MarshallingInfo;
		TempTimeAndDataGather.DataGather.TerminalDistribution = GameManagerPtr->Cache4TerminalDistribution;
		TempTimeAndDataGather.DataGather.VerdictInfoArray = GameManagerPtr->Cache4VerdictEntity;
		TempTimeAndDataGather.DataGather.TerminalControlCommand = GameManagerPtr->Cache4TerminalControlCommand;
		TempTimeAndDataGather.DataGather.SystemStatusReply = GameManagerPtr->Cache4SystermStatusReply;
		TempTimeAndDataGather.DataGather.EventInfoArray = GameManagerPtr->Cache4EventInfo;
		TempTimeAndDataGather.DataGather.FZ_TYJGXX = GameManagerPtr->FZ_TYJGXX;
		TempTimeAndDataGather.DataGather.GroupInfo = GameManagerPtr->GroupInfo;
		TempTimeAndDataGather.DataGather.GroupInfo.ReplaceInline(L"\r\n",L"");
		TempTimeAndDataGather.DataGather.BarrierCreateAry = GameManagerPtr->CacheBarrierCreateAry;
		DataGatherList.TimeAndDataGatherArray.Add(TempTimeAndDataGather);
	}
}

bool UPlaybackComponent::SavePlaybackDataIndexArray(const FString InFileName)
{
	FString TempFString = TEXT("");
	UPlaybackDataDefine::FPlaybackDataIndexArrayToJsonString(PlaybackDataIndexArray, TempFString);
	if (!TempFString.IsEmpty())
	{
		if (UGeneralBlueprintFunctionLibrary::SaveStringToFile(TempFString, InFileName))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UPlaybackComponent::LoadPlaybackDataIndexArray(const FString InFileName)
{
	FString TempFString = TEXT("");
	if (UGeneralBlueprintFunctionLibrary::LoadFileToString(InFileName, TempFString))
	{
		if (TempFString.Len() > 0)
		{
			if (UPlaybackDataDefine::JsonStringToFPlaybackDataIndexArray(TempFString, PlaybackDataIndexArray))
			{
				UpdatePlaybackDataIndexMaxID();
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UPlaybackComponent::UseDataIdDeleteData(const int32 InDataID)
{
	if (PlaybackDataIndexArray.PlaybackDataIndexArray.Num()> 0)
	{
		for (int32 i = PlaybackDataIndexArray.PlaybackDataIndexArray.Num() - 1; i >= 0; i--)
		{
			if (PlaybackDataIndexArray.PlaybackDataIndexArray[i].DataID == InDataID)
			{
				FString TempFileName = PlaybackDataIndexArray.PlaybackDataIndexArray[i].DataFileName;
				PlaybackDataIndexArray.PlaybackDataIndexArray.RemoveAt(i);
				FString TempFullFileName = FPaths::ConvertRelativePathToFull(TempFileName);
				if (FPaths::FileExists(TempFullFileName))
				{
					/*
					IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
					if (PlatformFile.DeleteFile(*TempFullFileName))
					{
						UE_LOG(LogTemp, Log, TEXT("%hs %s"), __FUNCTION__, TEXT("UseDataIdDeleteData Success !"));
					}
*/
					IFileManager& FileManager = IFileManager::Get();
					if (FileManager.Delete(*TempFullFileName))
					{
						UE_LOG(LogTemp, Log, TEXT("%hs %s"), __FUNCTION__, TEXT("UseDataIdDeleteData Success !"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("%hs %s"), __FUNCTION__, TEXT("UseDataIdDeleteData Failed !"));
					}
				}
			}
		}
	}
}

void UPlaybackComponent::UseDataIdSetCurrentSelect(const bool ClidkState, const int32 InDataID)
{
	if (ClidkState)
	{
		if (PlaybackDataIndexArray.PlaybackDataIndexArray.Num() > 0)
		{	
			for (FPlaybackDataIndex Element : PlaybackDataIndexArray.PlaybackDataIndexArray)
			{
				if (Element.DataID == InDataID)
				{
					IsSelectedData = true;
					PlaybackDataIndex = Element;
				}
			}
		}
	}
	else
	{
		PlaybackDataIndex = FPlaybackDataIndex();
		IsSelectedData = false;
	}
}

void UPlaybackComponent::UpdatePlaybackDataIndexMaxID()
{
	if (PlaybackDataIndexArray.PlaybackDataIndexArray.Num() > 0)
	{
		for (FPlaybackDataIndex Element : PlaybackDataIndexArray.PlaybackDataIndexArray)
		{
			if (Element.DataID > MaxID)
			{
				MaxID = Element.DataID;
			}
		}
	}
}

void UPlaybackComponent::StartDataCollection()
{
	DataGatherList = FDataGatherList();
	IsDataCollection = true;
	StartTime = UKismetMathLibrary::Now();
}

void UPlaybackComponent::EndDataCollection()
{
	IsDataCollection = false;
	EndTime = UKismetMathLibrary::Now();
}

void UPlaybackComponent::InitPlaybackDataThread()
{
	PlaybackDataThread = new FPlaybackDataThread();
	PlaybackDataThread_Handle = FRunnableThread::Create(PlaybackDataThread, TEXT("PlaybackDataThread"));

	PlaybackDataThread->ReturnFDataGatherDelegate.BindUObject(this, &UPlaybackComponent::ReceivedFDataGather);
	PlaybackDataThread->PlaybackFinishedDelegate.BindUObject(this, &UPlaybackComponent::BroadCastPlaybackResult);
}

void UPlaybackComponent::EndPlaybackDataThread()
{
	if (PlaybackDataThread)
	{
		PlaybackDataThread->Stop();
		PlaybackDataThread_Handle->WaitForCompletion();
		delete PlaybackDataThread;
		PlaybackDataThread = nullptr;
		PlaybackDataThread_Handle = nullptr;
	}
}

void UPlaybackComponent::AddFDataGatherListToThread(const FDataGatherList& InFDataGatherList)
{
	if (PlaybackDataThread)
	{
		PlaybackDataThread->AddFDataGatherList(InFDataGatherList);
	}
}

void UPlaybackComponent::StartPlayback()
{
	if (PlaybackDataThread)
	{
		PlaybackDataThread->SetPause(false);
	}
}

void UPlaybackComponent::PausePlayback()
{
	if (PlaybackDataThread)
	{
		PlaybackDataThread->SetPause(true);
	}
}

void UPlaybackComponent::SetPlaybackSpeed(const float InSpeed)
{
	if (PlaybackDataThread)
	{
		PlaybackDataThread->SetSpeed(InSpeed);
	}
}

void UPlaybackComponent::StopPlaybackData()
{
	if (PlaybackDataThread)
	{
		PlaybackDataThread->StopPlaybackData();
	}
}

void UPlaybackComponent::ReceivedFDataGather(const FDataGather& InFDataGather) const
{
	if (ReturnPlaybackFDataGather_Delegate.IsBound())
	{
		ReturnPlaybackFDataGather_Delegate.Broadcast(InFDataGather);
	}
}

void UPlaybackComponent::BroadCastPlaybackResult()
{
	//当前状态非回放；非回放
	IsBeBeingPlayback = false;
	//未选择数据
	IsSelectedData = false;
	if (PlaybackFinishDelegate.IsBound())
	{	
		PlaybackFinishDelegate.Broadcast();
	}
}

void UPlaybackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	EndPlaybackDataThread();
	
	UE_LOG(LogTemp, Log, TEXT("%hs GC!"), __FUNCTION__);
}

