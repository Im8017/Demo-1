// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "ProtocalData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QingLongXia/GlobalStructEnum/GlobalStruct.h"
#include "PlaybackDataDefine.generated.h"

/** 类型与二进制字符串连接时的分隔符 */
#define TYPE_BINARY TEXT("_%_")

/** 二进制字符串结尾标识符号 */
#define BINARY_END TEXT("_$$")

/** 时间戳与数据字符串连接时的分隔符 */
#define TIME_DATA TEXT("_@_")

/** 时间戳与数据集连接时的分隔符 */
#define TIME_DATAGATHER TEXT("]-[")

struct FTerminalStateInfo;
struct FStatisticsTerminal;
struct FStatisticsInfo;
struct FFZ_TYJGXX;
struct FEventInfo;
struct FSystemStatusReply;
struct FTerminalControlCommand;
struct FVerdictInfo;
struct FTerminalDistribution;
struct FMarshalling;
struct FMarshallingInfo;
struct FDrillPlanInfo;
struct FDrillStageInfo;
struct FForceDispositionInfo;
struct FConstractionInfo;
struct FDodgePointInfo;
struct FBunkerInfo;
struct FCannoBallFallInfo;
struct FBattleAreaInfo;

#pragma region 回放数据索引
//回放数据索引
USTRUCT(BlueprintType)
struct FPlaybackDataIndex
{
	GENERATED_BODY()

	//数据ID
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "数据ID"))
	int32 DataID = -1;

	//数据名称
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "数据名称"))
	FString DataName = "";

	//数据创建时间
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "数据创建时间"))
	FString CreateTime = "";

	//数据时长
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "数据时长"))
	FString DataLength = "";

	//数据文件容量（兆）
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "数据文件容量（兆）"))
	float DataCapacity = 0.0f;

	//数据文件相对路径含后缀
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "数据文件相对路径含后缀"))
	FString DataFileName = "";

	FPlaybackDataIndex() {};
};
#pragma endregion

#pragma region 回放数据索引数组
//回放数据索引数组
USTRUCT(BlueprintType)
struct FPlaybackDataIndexArray
{
	GENERATED_BODY()

	//回放数据索引数组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "回放数据索引数组"))
	TArray<FPlaybackDataIndex> PlaybackDataIndexArray = TArray<FPlaybackDataIndex>();

	FPlaybackDataIndexArray() {};
};
#pragma endregion

#pragma region 推演数据集结构体
//态势数据集结构体
USTRUCT(BlueprintType)
struct FDataGather
{
	GENERATED_BODY()

	//器材状态信息数组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "器材状态信息数组"))
	TArray<FTerminalStateInfo> TerminalStateInfoArray = TArray<FTerminalStateInfo>();

	//演习区域信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "演习区域信息"))
	FBattleAreaInfo BattleAreaInfo = FBattleAreaInfo();

	//落弹区信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "落弹区信息"))
	FCannoBallFallInfo CannoBallFallInfo = FCannoBallFallInfo();

	//掩体信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "掩体信息"))
	FBunkerInfo BunkerInfo = FBunkerInfo();

	//躲避点信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "躲避点信息"))
	FDodgePointInfo DodgePointInfo = FDodgePointInfo();

	//工事障碍信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "工事障碍信息"))
	FConstractionInfo ConstractionInfo = FConstractionInfo();

	//兵力部署信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "兵力部署信息"))
	FForceDispositionInfo ForceDispositionInfo = FForceDispositionInfo();

	//演习阶段信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "演习阶段信息"))
	FDrillStageInfo DrillStageInfo = FDrillStageInfo();

	//演习计划信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "演习计划信息"))
	FDrillPlanInfo DrillPlanInfo = FDrillPlanInfo();

	//推演模式下 以编组为单位的 行动信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "推演模式下以编组为单位的行动信息"))
	TArray<FSingleJhxx> XdxxArray = TArray<FSingleJhxx>();

	//推演模式下 以编组为单位的 打击信息信息 key为编组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "推演模式下以编组为单位的行动信息"))
	TArray<FSingleJhxx> DaJiArray = TArray<FSingleJhxx>();

	//编组信息-----编组无法存储树形结构，改用下方GroupInfo JsonString
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "编组信息"))
	FMarshallingInfo MarshallingInfo = FMarshallingInfo();

	//编组信息JsonString-----
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "编组信息"))
	FString GroupInfo;

	//器材分配信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "器材分配信息"))
	FTerminalDistribution TerminalDistribution = FTerminalDistribution();

	//裁决信息数组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "裁决信息数组"))
	TArray<FVerdictInfo> VerdictInfoArray = TArray<FVerdictInfo>();

	//器材控制指令内容信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "器材控制指令内容信息"))
	FTerminalControlCommand TerminalControlCommand = FTerminalControlCommand();

	//系统状态应答
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "系统状态应答"))
	FSystemStatusReply SystemStatusReply = FSystemStatusReply();

	//打击事件数组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "打击事件数组"))
	TArray<FEventInfo> EventInfoArray = TArray<FEventInfo>();

	//推演结果信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "推演结果信息"))
	FFZ_TYJGXX FZ_TYJGXX = FFZ_TYJGXX();

	//障碍物信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "障碍物信息"))
	FBarrierCreatAry BarrierCreateAry;
	FDataGather() {};
};
#pragma endregion

#pragma region 推演时间戳与推演数据集
//推演时间戳与推演数据集
USTRUCT(BlueprintType)
struct FTimeAndDataGather
{
	GENERATED_BODY()

	//推演时间戳
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "推演时间戳"))
	FString Time = "";

	//推演数据集
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "推演数据集"))
	FDataGather DataGather = FDataGather();

	FTimeAndDataGather() {};
};
#pragma endregion 

#pragma region 推演数据集数组
//推演数据集数组
USTRUCT(BlueprintType)
struct FDataGatherList
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(DisplayName = "推演数据集数组"))
	TArray<FTimeAndDataGather> TimeAndDataGatherArray = TArray<FTimeAndDataGather>();

	FDataGatherList() {};
};
#pragma endregion 

/**
 * Json结构体转换
 */
UCLASS()
class QINGLONGXIA_API UPlaybackDataDefine : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
#pragma region JsonString转结构体
	/**
	 * 将JsonString的字符串转成FPlaybackDataIndex结构体
	 * @param InJsonString				输入JsonString字符串
	 * @param OutStruct					输出FPlaybackDataIndex结构体
	 * @return							返回是否转换成功
	 */
	UFUNCTION(BlueprintCallable, Category="PlaybackDataDefine")
	static bool JsonStringToFPlaybackDataIndex(FString InJsonString, FPlaybackDataIndex& OutStruct);

	/**
	 * 将JsonString的字符串转成FPlaybackDataIndexArray结构体
	 * @param InJsonString				输入JsonString字符串
	 * @param OutStruct					输出FPlaybackDataIndexArray结构体
	 * @return							返回是否转换成功
	 */
	UFUNCTION(BlueprintCallable, Category="PlaybackDataDefine")
	static bool JsonStringToFPlaybackDataIndexArray(FString InJsonString, FPlaybackDataIndexArray& OutStruct);
#pragma endregion

#pragma region 结构体转JsonString
	/**
	 * 将FPlaybackDataIndex结构体转成JsonString字符串
	 * @param InStruct					输入FPlaybackDataIndex结构体
	 * @param OutJsonString				输出JsonString字符串
	 * @return							返回是否转换成功
	 */
	UFUNCTION(BlueprintCallable, Category="PlaybackDataDefine")
	static bool FPlaybackDataIndexToJsonString(FPlaybackDataIndex InStruct, FString& OutJsonString);

	/**
	 * 将FPlaybackDataIndexArray结构体转成JsonString字符串
	 * @param InStruct					输入FPlaybackDataIndexArray结构体
	 * @param OutJsonString				输出JsonString字符串
	 * @return							返回是否转换成功
	 */
	UFUNCTION(BlueprintCallable, Category="PlaybackDataDefine")
	static bool FPlaybackDataIndexArrayToJsonString(FPlaybackDataIndexArray InStruct, FString& OutJsonString);

	/**
	 * 将时间字符串按分隔符拆分成：时、分、秒、毫秒的整型
	 * @param InTimeStr					输入时间字符串
	 * @param InSeparator				输入分隔符字符串
	 * @param OutHour					输出小时
	 * @param OutMinute					输出分钟
	 * @param OutSecond					输出秒
	 * @param OutMillisecond			输出毫秒
	 */
	static void FStringToHour_Minute_Second_Millisecond(const FString& InTimeStr, const FString& InSeparator, int32& OutHour, int32& OutMinute, int32& OutSecond, int32& OutMillisecond);

	/**
	 * 计算两个时间字符串的差值
	 * @param InTimeStr1				输入大的时间
	 * @param InTimeStr2				输入小的时间
	 * @return							返回插值（秒）
	 */
	static float FStringToTimeDifference(const FString& InTimeStr1, const FString& InTimeStr2);
#pragma endregion 
};
