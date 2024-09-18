// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlaybackDataModule/PlaybackManager.h"
#include "GeneralBlueprintFunctionLibrary.generated.h"

#pragma region 时间字符串结构体
//时间字符串结构体
USTRUCT(BlueprintType)
struct FDateTimeStringStruct
{
	GENERATED_BODY()

	//年
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="年"))
	FString Year = "";

	//月
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="月"))
	FString Month = "";

	//日
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="日"))
	FString Day = "";

	//时
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="时"))
	FString Hour = "";

	//分
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="分"))
	FString Minute = "";

	//秒
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="秒"))
	FString Second = "";

	//毫秒
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="毫秒"))
	FString Millisecond = "";

	FDateTimeStringStruct() {};
};
#pragma endregion

#pragma region 字符串键值对结构体（委托不支持TMap类型，所以使用该结构体）
//字符串键值对结构体（由于委托不支持TMap类型，所以使用该结构体）
USTRUCT(BlueprintType)
struct FFStringTMapStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName="字符串键值对"))
	TMap<FString, FString> FStringTMap = TMap<FString, FString>();

	FFStringTMapStruct() {};
};
#pragma endregion

/**
 * 一般蓝图方法库
 */
UCLASS()
class QINGLONGXIA_API UGeneralBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * 将FDateTime结构体转FString字符串
	 * @param InDateTime			输入FDateTime结构体
	 * @return						返回时间字符串
	 */
	UFUNCTION(BlueprintPure, Category="GeneralBlueprintFunction")
	static FString ConvertFDateTimeToFString(const FDateTime InDateTime);

	/**
	 * 将FDateTime结构体转DateTimeStringStruct结构体
	 * @param InDateTime			输入FDateTime结构体
	 * @return						返回时间字符串结构体
	 */
	UFUNCTION(BlueprintPure, Category="GeneralBlueprintFunction")
	static FDateTimeStringStruct ConvertFDateTimeToDateTimeStringStruct(const FDateTime InDateTime);

	/**
	 * 将FTimespan结构体转FDateTimeStringStruct结构体
	 * @param InTimespan			输入FTimespan结构体
	 * @return						返回时间字符串结构体
	 */
	UFUNCTION(BlueprintPure, Category="GeneralBlueprintFunction")
	static FDateTimeStringStruct ConvertFTimespanToDateTimeStringStruct(const FTimespan InTimespan);

	/**
	 * 获取两个时间的时间差
	 * @param InMaxTime				输入最大时间
	 * @param InMinTime				输入最小时间
	 * @return						返回最大到最小时间之间的时间差
	 */
	UFUNCTION(BlueprintPure, Category="GeneralBlueprintFunction")
	static FTimespan GetTwoFDateTimeDifference(const FDateTime InMaxTime, const FDateTime InMinTime);

	/**
	 * 将FTimespan结构体转FString字符串
	 * @param InTimespan			输入时间差结构体
	 * @return						返回时间字符串
	 */
	UFUNCTION(BlueprintPure, Category="GeneralBlueprintFunction")
	static FString ConvertFTimespanToFString(const FTimespan InTimespan);

	/**
	 * 获取中英文混合字符串的长度
	 * @param InString				输入字符串引用
	 * @return						返回字符串长度
	 */
	UFUNCTION(BlueprintPure, Category="GeneralBlueprintFunction")
	static int32 GetMixturesStringLength(const FString& InString);

	/**
	 * Json字符串转对应的结构体
	 * @tparam StructType			结构体类型
	 * @param InJsonString			输入Json字符串
	 * @param OutStruct				输出结构体
	 * @return						返回转换是否成功
	 */
	template <typename StructType>
	static FORCEINLINE_DEBUGGABLE bool LocalConvertJsonStringToStruct(FString& InJsonString, StructType& OutStruct)
	{
		if (InJsonString.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("%hs %s"), __FUNCTION__, TEXT("InJsonString Is Empty !"));
			return false;
		}
		else
		{
			return FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(InJsonString, &OutStruct, 0, 0);
		}
	}

	/**
	 * 结构体转对应的Json字符串
	 * @tparam StructType			结构体类型
	 * @param InStruct				输入结构体
	 * @param OutJsonString			输出Json字符串
	 * @return						返回转换是否成功
	 */
	template <typename StructType>
	static FORCEINLINE_DEBUGGABLE bool LocalConvertStructToJsonString(StructType& InStruct, FString& OutJsonString)
	{
		if (FJsonObjectConverter::UStructToJsonObjectString<StructType>(InStruct, OutJsonString, 0, 0, 0, nullptr, true))
		{
			if (OutJsonString.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("%hs %s"), __FUNCTION__, TEXT("OutJsonString Is Empty !"));
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%hs %s %s"), __FUNCTION__, TEXT("if (FJsonObjectConverter::UStructToJsonObjectString<StructType>(InStruct, OutJsonString)) == false !"), *OutJsonString);
			return false;
		}
	}

	/**
	 * 保存字符串到文件
	 * @param InString				输入字符串
	 * @param InFileName			输入完整文件路径含文件名和后缀
	 * @return						返回是否保存成功
	 */
	UFUNCTION(BlueprintCallable, Category="GeneralBlueprintFunction")
	static bool SaveStringToFile(const FString InString, const FString InFileName);

	/**
	 * 保存字符串到文件
	 * @param InString				输入字符串
	 * @param InFullPath			输入完整路径
	 * @param InFileName			输入文件名
	 * @param InFileSuffix			输入文件后缀
	 * @return						返回是否保存成功
	 */
	UFUNCTION(BlueprintCallable, Category="GeneralBlueprintFunction")
	static bool SaveStringToFile_K2(const FString InString, FString InFullPath, const FString InFileName, const FString InFileSuffix);

	/**
	 * 将指定文件加载为字符串
	 * @param InFileName			输入完整文件路径含文件名和后缀
	 * @param OutString				输出字符串
	 * @return						返回是否读取成功
	 */
	UFUNCTION(BlueprintCallable, Category="GeneralBlueprintFunction")
	static bool LoadFileToString(const FString InFileName, FString& OutString);

	/**
	 * 将指定文件加载为字符串
	 * @param InFullPath			输入完整路径
	 * @param InFileName			输入文件名
	 * @param InFileSuffix			输入文件后缀
	 * @param OutString				返回字符串
	 * @return						返回是否读取成功
	 */
	UFUNCTION(BlueprintCallable, Category="GeneralBlueprintFunction")
	static bool LoadFileToString_K2(const FString InFullPath, const FString InFileName, const FString InFileSuffix, FString& OutString);

	/**
	 * 将字符串键值对按分隔符转成对应字符串数组，Key转为分隔符左边，Value转为分隔附右边
	 * @param InFStringTMap			输入字符串键值对
	 * @param InSeparator			输入Kay与Value组合成字符串时，使用的分隔符号
	 * @param OutFStringTArray		输出字符串数组			
	 */
	UFUNCTION(BlueprintCallable, Category="GeneralBlueprintFunction")
	static void FStringTMapToFStringTArray(const TMap<FString, FString>& InFStringTMap, const FString InSeparator, TArray<FString>& OutFStringTArray);

	/**
	 * 将字符串数组按分隔符转为字符串键值对，分隔符左边转为Key，分隔符右边转为Value
	 * @param InFStringTArray		输入字符串数组
	 * @param InSeparator			输入拆分字符串的分隔符号
	 * @param OutFStringTMap		输出字符串键值对
	 */
	UFUNCTION(BlueprintCallable, Category="GeneralBlueprintFunction")
	static void FStringTArrayToFStringTMap(const TArray<FString>& InFStringTArray, const FString InSeparator, TMap<FString, FString>& OutFStringTMap);

	/**
	 * 使用完整文件名称含后缀，获取文件容量(单位MB)精确到小数点后两位
	 * @param InFileName			输入文件完整路径含名称后缀
	 * @return						返回文件容量
	 */
	UFUNCTION(BlueprintPure, Category="GeneralBlueprintFunction")
	static float GetFileCapacityMB(const FString& InFileName);
	
	/* 获取回放Manager */
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "获取回放管理类")
	static APlaybackManager* GetPlaybackManager();

	/* 生成GUID */
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static int32 GenerateGUID();

	/* 现在时间转字符串 */
	UFUNCTION(BlueprintPure,BlueprintCallable)
	static  FString ConvertNowToString();

#pragma region C++函数
public:
	/**
	 * 将字符串键值对按分隔符转成对应字符串数组，Key转为分隔符左边，Value转为分隔附右边
	 * @param InFStringTMultiMap			输入字符串键值对
	 * @param InSeparator					输入Kay与Value组合成字符串时，使用的分隔符号
	 * @param OutFStringTArray				输出字符串数组
	 */
	static void FStringTMultiMapToFStringTArray(const TMultiMap<FString, FString>& InFStringTMultiMap, const FString InSeparator, TArray<FString>& OutFStringTArray);

	
#pragma endregion 
};
