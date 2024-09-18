// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaybackDataDefine.h"
#include "QingLongXia/GeneralBlueprintFunctionLibrary.h"

bool UPlaybackDataDefine::JsonStringToFPlaybackDataIndex(FString InJsonString, FPlaybackDataIndex& OutStruct)
{
	return UGeneralBlueprintFunctionLibrary::LocalConvertJsonStringToStruct<FPlaybackDataIndex>(InJsonString, OutStruct);
}

bool UPlaybackDataDefine::JsonStringToFPlaybackDataIndexArray(FString InJsonString, FPlaybackDataIndexArray& OutStruct)
{
	return UGeneralBlueprintFunctionLibrary::LocalConvertJsonStringToStruct<FPlaybackDataIndexArray>(InJsonString, OutStruct);
}

bool UPlaybackDataDefine::FPlaybackDataIndexToJsonString(FPlaybackDataIndex InStruct, FString& OutJsonString)
{
	return UGeneralBlueprintFunctionLibrary::LocalConvertStructToJsonString<FPlaybackDataIndex>(InStruct, OutJsonString);
}

bool UPlaybackDataDefine::FPlaybackDataIndexArrayToJsonString(FPlaybackDataIndexArray InStruct, FString& OutJsonString)
{
	return UGeneralBlueprintFunctionLibrary::LocalConvertStructToJsonString<FPlaybackDataIndexArray>(InStruct, OutJsonString);
}

void UPlaybackDataDefine::FStringToHour_Minute_Second_Millisecond(const FString& InTimeStr, const FString& InSeparator, int32& OutHour, int32& OutMinute, int32& OutSecond, int32& OutMillisecond)
{
	if (InTimeStr.Len() > 0)
	{
		FString Hour, Minute, Second, Millisecond, ResidueStr, ResidueStr1, ResidueStr2;

		if (InTimeStr.Split(InSeparator, &Hour, &ResidueStr, ESearchCase::IgnoreCase, ESearchDir::FromStart))
		{
			if (ResidueStr.Split(InSeparator, &Minute, &ResidueStr1, ESearchCase::IgnoreCase, ESearchDir::FromStart))
			{
				if (ResidueStr1.Split(InSeparator, &Second, &Millisecond, ESearchCase::IgnoreCase, ESearchDir::FromStart))
				{
					OutHour = FCString::Atoi(*Hour);
					OutMinute = FCString::Atoi(*Minute);
					OutSecond = FCString::Atoi(*Second);
					OutMillisecond = FCString::Atoi(*Millisecond);
				}
			}
		}
	}
}

float UPlaybackDataDefine::FStringToTimeDifference(const FString& InTimeStr1, const FString& InTimeStr2)
{
	int32 Hour1 = 0;
	int32 Minute1 = 0;
	int32 Second1 = 0;
	int32 Millisecond1 = 0;
	FStringToHour_Minute_Second_Millisecond(InTimeStr1, TIME_DATAGATHER, Hour1, Minute1, Second1, Millisecond1);
	int32 Hour2 = 0;
	int32 Minute2 = 0;
	int32 Second2 = 0;
	int32 Millisecond2 = 0;
	FStringToHour_Minute_Second_Millisecond(InTimeStr2, TIME_DATAGATHER, Hour2, Minute2, Second2, Millisecond2);

	int32 Hour3 = Hour1 - Hour2;
	int32 Minute3 = Minute1 - Minute2;
	int32 Second3 = Second1 - Second2;
	int32 Millisecond3 = Millisecond1 - Millisecond2;

	int32 Hour4 = Hour3 * 60;
	int32 Minute4 = Hour4 + (Minute3 * 60);
	int32 Second4 = Minute4 + Second3;
	float Tempfloat1 = Millisecond3;
	float Tempfloat2 = Tempfloat1 / 1000;
	float Tempfloat3 = Second4 + Tempfloat2;

	return Tempfloat3;
}
