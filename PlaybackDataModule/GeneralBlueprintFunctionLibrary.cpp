// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneralBlueprintFunctionLibrary.h"

#include <random>

FString UGeneralBlueprintFunctionLibrary::ConvertFDateTimeToFString(const FDateTime InDateTime)
{
	return InDateTime.ToString();
}

FDateTimeStringStruct UGeneralBlueprintFunctionLibrary::ConvertFDateTimeToDateTimeStringStruct(const FDateTime InDateTime)
{
	FDateTimeStringStruct TempFDateTimeStringStruct;
	
	TempFDateTimeStringStruct.Year = FString::Printf(TEXT("%04d"), InDateTime.GetYear());
	TempFDateTimeStringStruct.Month = FString::Printf(TEXT("%02d"), InDateTime.GetMonth());
	TempFDateTimeStringStruct.Day = FString::Printf(TEXT("%02d"), InDateTime.GetDay());
	TempFDateTimeStringStruct.Hour = FString::Printf(TEXT("%02d"), InDateTime.GetHour());
	TempFDateTimeStringStruct.Minute = FString::Printf(TEXT("%02d"), InDateTime.GetMinute());
	TempFDateTimeStringStruct.Second = FString::Printf(TEXT("%02d"), InDateTime.GetSecond());
	TempFDateTimeStringStruct.Millisecond = FString::Printf(TEXT("%03d"), InDateTime.GetMillisecond());
	return TempFDateTimeStringStruct;
}

FDateTimeStringStruct UGeneralBlueprintFunctionLibrary::ConvertFTimespanToDateTimeStringStruct(const FTimespan InTimespan)
{
	FDateTimeStringStruct TempFDateTimeStringStruct;

	TempFDateTimeStringStruct.Year = TEXT("0000");
	TempFDateTimeStringStruct.Month = TEXT("00");
	TempFDateTimeStringStruct.Day = FString::Printf(TEXT("%02d"), InTimespan.GetDays());
	TempFDateTimeStringStruct.Hour = FString::Printf(TEXT("%02d"), InTimespan.GetHours());
	TempFDateTimeStringStruct.Minute = FString::Printf(TEXT("%02d"), InTimespan.GetMinutes());
	TempFDateTimeStringStruct.Second = FString::Printf(TEXT("%02d"), InTimespan.GetSeconds());
	const int32 TempTotalMillisecond = InTimespan.GetTotalMilliseconds();
	const int32 TempMilli = TempTotalMillisecond;
	TempFDateTimeStringStruct.Millisecond = FString::Printf(TEXT("%03d"), TempMilli);
	return TempFDateTimeStringStruct;
}

FTimespan UGeneralBlueprintFunctionLibrary::GetTwoFDateTimeDifference(const FDateTime InMaxTime, const FDateTime InMinTime)
{
	return InMaxTime - InMinTime;
}

FString UGeneralBlueprintFunctionLibrary::ConvertFTimespanToFString(const FTimespan InTimespan)
{
	return InTimespan.ToString();
}

int32 UGeneralBlueprintFunctionLibrary::GetMixturesStringLength(const FString& InString)
{
	int32 TempLength = 0;

	for (const TCHAR TempElement : InString)
	{
		//判断字符是否为中文，如为中文字符则长度加2，否则长度加1
		if (((TempElement >= 0x4E00 && TempElement <= 0x9FF) || (TempElement >= 0x3400 && TempElement <= 0x4DBF)))
		{
			TempLength += 2;
		}
		else
		{
			TempLength += 1;
		}
	}
	
	return TempLength;
}

bool UGeneralBlueprintFunctionLibrary::SaveStringToFile(const FString InString, const FString InFileName)
{
	if (InFileName.IsEmpty() || InString.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%hs %s"), __FUNCTION__, TEXT("InFileName Or InString Is Empty !"));
		return false;
	}
	else
	{
		if (FFileHelper::SaveStringToFile(InString, *InFileName, FFileHelper::EEncodingOptions::ForceUTF8))
		{
			UE_LOG(LogTemp, Log, TEXT("%hs %s %s"), __FUNCTION__, *InFileName, TEXT("Succeed !"));
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%hs %s %s"), __FUNCTION__, *InFileName, TEXT("Fail !"));
			return false;
		}
	}
}

bool UGeneralBlueprintFunctionLibrary::SaveStringToFile_K2(const FString InString, FString InFullPath, const FString InFileName, const FString InFileSuffix)
{
	if (InFullPath.IsEmpty() || InFileName.IsEmpty() || InFileSuffix.IsEmpty() || InString.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%hs %s"), __FUNCTION__, TEXT("InFullPath Or InFileName Or InFileSuffix Or InString Is Empty !"));
		return false;
	}
	else
	{
		FString TempFullPath = "";
		if (!InFullPath.Right(1).Equals(TEXT("/")))
		{
			if (!InFullPath.Right(1).Equals(TEXT("\\")))
			{
				TempFullPath = InFullPath + TEXT("/");
			}
			else
			{
				TempFullPath = InFullPath.Replace(TEXT("\\"), TEXT("/"));
			}
		}
		else
		{
			TempFullPath = InFullPath;
		}

		FString TempFileSuffix = "";
		if (!InFileSuffix.Left(1).Equals(TEXT(".")))
		{
			TempFileSuffix = TEXT(".") + InFileSuffix;
		}
		else
		{
			TempFileSuffix = InFileSuffix;
		}
		
		const FString TempFileName = TempFullPath + InFileName + TempFileSuffix;

		return SaveStringToFile(InString, TempFileName);
	}
}

bool UGeneralBlueprintFunctionLibrary::LoadFileToString(const FString InFileName, FString& OutString)
{
	if (InFileName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%hs %s"), __FUNCTION__, TEXT("InFileName Is Empty !"));
		return false;
	}
	else
	{
		if (FPaths::FileExists(InFileName))
		{
			if (FFileHelper::LoadFileToString(OutString, *InFileName, FFileHelper::EHashOptions::EnableVerify))
			{
				UE_LOG(LogTemp, Log, TEXT("%hs %s %s %s"), __FUNCTION__, TEXT("LoadFileToString"), *InFileName, TEXT("Succeed !"));
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("%hs %s %s"), __FUNCTION__, TEXT("if (FFileHelper::LoadFileToString(OutString, *InFileName, FFileHelper::EHashOptions::EnableVerify)) == false !"), *InFileName);
				return false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%hs %s %s"), __FUNCTION__, TEXT("if (FPaths::FileExists(InFileName)) == false !"), *InFileName);
			return false;
		}
	}
}

bool UGeneralBlueprintFunctionLibrary::LoadFileToString_K2(const FString InFullPath, const FString InFileName, const FString InFileSuffix, FString& OutString)
{
	if (InFullPath.IsEmpty() || InFileName.IsEmpty() || InFileSuffix.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("%hs %s"), __FUNCTION__, TEXT("InFullPath Or InFileName Or InFileSuffix Is Empty !"));
		return false;
	}
	else
	{
		FString TempFullPath = "";
		if (!InFullPath.Right(1).Equals(TEXT("/")))
		{
			if (!InFullPath.Right(1).Equals(TEXT("\\")))
			{
				TempFullPath = InFullPath + TEXT("/");
			}
			else
			{
				TempFullPath = InFullPath.Replace(TEXT("\\"), TEXT("/"));
			}
		}
		else
		{
			TempFullPath = InFullPath;
		}

		FString TempFileSuffix = "";
		if (!InFileSuffix.Left(1).Equals(TEXT(".")))
		{
			TempFileSuffix = TEXT(".") + InFileSuffix;
		}
		else
		{
			TempFileSuffix = InFileSuffix;
		}
		
		const FString TempFileName = TempFullPath + InFileName + TempFileSuffix;

		return LoadFileToString(TempFileName, OutString);
	}
}

void UGeneralBlueprintFunctionLibrary::FStringTMapToFStringTArray(const TMap<FString, FString>& InFStringTMap, const FString InSeparator, TArray<FString>& OutFStringTArray)
{
	for (TMap<FString, FString>::TConstIterator It(InFStringTMap); It; ++It)
	{
		FString TempFString = It.Key() + InSeparator + It.Value();
		OutFStringTArray.Add(TempFString);
	}
}

void UGeneralBlueprintFunctionLibrary::FStringTArrayToFStringTMap(const TArray<FString>& InFStringTArray, const FString InSeparator, TMap<FString, FString>& OutFStringTMap)
{
	for (FString Element : InFStringTArray)
	{
		if (Element.Contains(InSeparator, ESearchCase::IgnoreCase))
		{
			FString TempLeft = TEXT("");
			FString TempRight = TEXT("");

			if (Element.Split(InSeparator, &TempLeft, &TempRight, ESearchCase::IgnoreCase, ESearchDir::FromStart))
			{
				if ((!TempLeft.IsEmpty()) && (!TempRight.IsEmpty()))
				{
					OutFStringTMap.Emplace(TempLeft, TempRight);
				}
			}
		}
	}
}

float UGeneralBlueprintFunctionLibrary::GetFileCapacityMB(const FString& InFileName)
{
	IFileManager& FileManager = IFileManager::Get();

	const int64 FileSizeInBytes = FileManager.FileSize(*InFileName);

	if (FileSizeInBytes == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("File Inexistence !"));
		return -1.0f;
	}
	else
	{
		const float FileSizeInMB = FileSizeInBytes / 1048576.0f;
		FString FormattedString = FString::Printf(TEXT("%.2f"), FileSizeInMB);
		return FCString::Atof(*FormattedString);
	}
}

APlaybackManager* UGeneralBlueprintFunctionLibrary::GetPlaybackManager()
{
	return APlaybackManager::GetPtr();
}
int32 UGeneralBlueprintFunctionLibrary::GenerateGUID()
{
	std::random_device rd;
	std::mt19937_64 eng(rd());
	std::uniform_int_distribution<int32_t> distr;
	int32 Guid = static_cast<int32_t>(distr(eng));
	return Guid;
}

FString UGeneralBlueprintFunctionLibrary::ConvertNowToString()
{
	return FDateTime::Now().ToString(L"%Y.%m.%d %H:%M:%S");
}

void UGeneralBlueprintFunctionLibrary::FStringTMultiMapToFStringTArray(const TMultiMap<FString, FString>& InFStringTMultiMap, const FString InSeparator, TArray<FString>& OutFStringTArray)
{
	for (TMultiMap<FString, FString>::TConstIterator It(InFStringTMultiMap); It; ++It)
	{
		FString TempFString = It.Key() + InSeparator + It.Value();
		OutFStringTArray.Add(TempFString);
	}
}

