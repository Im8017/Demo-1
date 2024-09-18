// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncSaveDataGatherListToFile.h"

#include "QingLongXia/GeneralBlueprintFunctionLibrary.h"
#include "QingLongXia/DataInterface/PlaybackDataConvert.h"

UAsyncSaveDataGatherListToFile::UAsyncSaveDataGatherListToFile(const FObjectInitializer& ObjectInitializer)
{
	DataGatherList = FDataGatherList();
	FileName = TEXT("");
}

void UAsyncSaveDataGatherListToFile::SetReadyToDestroy()
{
	Super::SetReadyToDestroy();
	UE_LOG(LogTemp, Log, TEXT("%hs %s"), __FUNCTION__, TEXT("GC !"));
}

UAsyncSaveDataGatherListToFile* UAsyncSaveDataGatherListToFile::AsyncSaveDataGatherListToFile(UObject* WorldContextObject, const FDataGatherList& InDataGatherList, const FString& InFileName)
{
	UAsyncSaveDataGatherListToFile* SaveDataGatherListToFileObject = NewObject<UAsyncSaveDataGatherListToFile>();
	SaveDataGatherListToFileObject->DataGatherList = InDataGatherList;
	SaveDataGatherListToFileObject->FileName = InFileName;
	SaveDataGatherListToFileObject->RegisterWithGameInstance(WorldContextObject);
	return SaveDataGatherListToFileObject;
}

void UAsyncSaveDataGatherListToFile::Activate()
{
	Super::Activate();

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]()
	{
		FString TempStr = TEXT("");
		
		bool bSuccess = UGeneralBlueprintFunctionLibrary::LocalConvertStructToJsonString<FDataGatherList>(DataGatherList, TempStr);

		// std::string TempString = "";
		// Playback_Data::DataGatherList* TempDataGatherList = new Playback_Data::DataGatherList();
		// UPlaybackDataConvert::ConvertFDataGatherListToProtobufDataGatherList(DataGatherList, TempDataGatherList);
		// TempDataGatherList->SerializeToString(&TempString);
		// TempStr = UPlaybackDataConvert::StdStringToFString(TempString);
		
		bool bSuccess1 = FFileHelper::SaveStringToFile(TempStr, *FileName, FFileHelper::EEncodingOptions::ForceUTF8);

		AsyncTask(ENamedThreads::GameThread, [this, bSuccess, bSuccess1]()
		{
			if (bSuccess && bSuccess1)
			{
				OnSuccess.Broadcast(FileName);
			}
			else
			{
				OnFailed.Broadcast(TEXT(""));
			}

			SetReadyToDestroy();
		});
	});
}


