// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataDefine/PlaybackDataDefine.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncReadDataGatherListFromFile.generated.h"

//代理返回读取文件结果
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReadDataGatherListFromFile_Delegate, FDataGatherList, OutDataGatherList);

/**
 * 异步从文件读取数据集列表
 */
UCLASS()
class QINGLONGXIA_API UAsyncReadDataGatherListFromFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UAsyncReadDataGatherListFromFile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#pragma region 输出节点
public:
	//成功
	UPROPERTY(BlueprintAssignable)
	FReadDataGatherListFromFile_Delegate OnSuccess;

	//失败
	UPROPERTY(BlueprintAssignable)
	FReadDataGatherListFromFile_Delegate OnFailed;
	
	UPROPERTY()
	FString FileName;
#pragma endregion

#pragma region 复写父类虚函数
public:
	virtual void SetReadyToDestroy() override;

	virtual void Activate() override;
#pragma endregion

#pragma region C++和蓝图可调用
public:
	/**
	 * 异步保存数据集列表到文件
	 * @param WorldContextObject			输入实例化到当前运行时项目的任意对象，用于通过该对象获取UWorld指针
	 * @param InFileName					输入要读取的文件完整路径含后缀
	 * @param OnSuccess						输出节点成功
	 * @param OnFailed						输出节点失败
	 * @param OutFileName					输出文件完整路径
	 * @return								返回静态的实例化异步对象
	 */
	UFUNCTION(BlueprintCallable, Category="AsyncReadDataGatherListFromFile", meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"))
	static UAsyncReadDataGatherListFromFile* AsyncReadDataGatherListFromFile(UObject* WorldContextObject, const FString& InFileName);
#pragma endregion 
};
