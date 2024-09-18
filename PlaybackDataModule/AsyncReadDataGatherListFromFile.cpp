// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncReadDataGatherListFromFile.h"
#include "QingLongXia/GeneralBlueprintFunctionLibrary.h"
#include "QingLongXia/DataInterface/PlaybackDataConvert.h"

UAsyncReadDataGatherListFromFile::UAsyncReadDataGatherListFromFile(const FObjectInitializer& ObjectInitializer)
{
	FileName = TEXT("");
}

void UAsyncReadDataGatherListFromFile::SetReadyToDestroy()
{
	Super::SetReadyToDestroy();
	UE_LOG(LogTemp, Log, TEXT("%hs %s"), __FUNCTION__, TEXT("GC !"));
}

UAsyncReadDataGatherListFromFile* UAsyncReadDataGatherListFromFile::AsyncReadDataGatherListFromFile(UObject* WorldContextObject, const FString& InFileName)
{
	UAsyncReadDataGatherListFromFile* SaveDataGatherListToFileObject = NewObject<UAsyncReadDataGatherListFromFile>();
	SaveDataGatherListToFileObject->FileName = InFileName;
	SaveDataGatherListToFileObject->RegisterWithGameInstance(WorldContextObject);
	return SaveDataGatherListToFileObject;
}

void UAsyncReadDataGatherListFromFile::Activate()
{
	Super::Activate();

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]()
	{
		if (FPaths::FileExists(FileName))
		{
			FString TempString = TEXT("");
			bool bSuccess = FFileHelper::LoadFileToString(TempString, *FileName);

			FDataGatherList TempDataGatherList = FDataGatherList();

			// std::string TempStdString = UPlaybackDataConvert::FStringToStdString(TempString);
			// Playback_Data::DataGatherList TempDataGatherList44;
			// TempDataGatherList44.ParseFromString(TempStdString);
			// for (Playback_Data::TimeAndDataGather Element : TempDataGatherList44.timeanddatagatherarray())
			// {
			// 	TempDataGatherList.TimeAndDataGatherArray.Add(UPlaybackDataConvert::ConvertProtobufTimeAndDataGatherToFTimeAndDataGather(Element));
			// }
			
			bool bSuccess1 = UGeneralBlueprintFunctionLibrary::LocalConvertJsonStringToStruct<FDataGatherList>(TempString, TempDataGatherList);
	
			AsyncTask(ENamedThreads::GameThread, [this, bSuccess, bSuccess1, TempDataGatherList]()
			{
				if (bSuccess && bSuccess1)
				{
					OnSuccess.Broadcast(TempDataGatherList);
				}
				else
				{
					OnFailed.Broadcast(FDataGatherList());
				}
	
				SetReadyToDestroy();
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				OnFailed.Broadcast(FDataGatherList());
				SetReadyToDestroy();
			});
		}
	});
}


