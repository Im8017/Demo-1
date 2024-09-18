// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QingLongXia/PlaybackDataModule/PlaybackDataThread.h"
#include "QingLongXia/PlaybackDataModule/DataDefine/PlaybackDataDefine.h"
#include "Windows/WindowsPlatformFile.h"
#include "PlaybackComponent.generated.h"

//代理返回数据集
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReturnPlaybackFDataGather_Delegate, FDataGather, OutFDataGather);
//是否回放完毕
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlaybackFinishDelegate);

class AGameManager;
struct FBattleAreaInfo;
/**
 * 回放功能组件
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QINGLONGXIA_API UPlaybackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlaybackComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FReturnPlaybackFDataGather_Delegate ReturnPlaybackFDataGather_Delegate;

	UPROPERTY(BlueprintAssignable)
	FPlaybackFinishDelegate PlaybackFinishDelegate;

#pragma region 蓝图变量
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	AGameManager* GameManagerPtr;
	
	/** 是否正在回放 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	bool IsBeBeingPlayback;

	/** 是否正在采集数据 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	bool IsDataCollection;

	/** 是否已选择数据 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	bool IsSelectedData;

	/** 开始采集数据时的时间 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	FDateTime StartTime;

	/** 结束采集数据时的时间 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	FDateTime EndTime;

	/** 最大索引ID */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	int32 MaxID;

	/** 推演数据集列表 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	FDataGatherList DataGatherList;
	
	/** 回放数据索引数组 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	FPlaybackDataIndexArray PlaybackDataIndexArray;

	/** 当前正在回放的数据索引 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="C++_Variate")
	FPlaybackDataIndex PlaybackDataIndex;
#pragma endregion

#pragma region C++变量
protected:
	/* 回放数据线程句柄 */
	FRunnableThread* PlaybackDataThread_Handle;

	/* 回放数据线程 */
	FPlaybackDataThread* PlaybackDataThread;
#pragma endregion 

#pragma region 蓝图函数
public:
	/**
	 * 保存回放数据索引数组
	 * @param InFileName			输入回放数据索引数组文件名
	 * @return						返回是否保存成功
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	bool SavePlaybackDataIndexArray(const FString InFileName);

	/**
	 * 加载回放数据索引数组
	 * @param InFileName			输入回放数据索引数组文件名
	 * @return						返回是否成功加载
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	bool LoadPlaybackDataIndexArray(const FString InFileName);

	/**
	 * 使用数据ID删除索引数组中对应项，并删除对应的数据文件
	 * @param InDataID				输入数据ID
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void UseDataIdDeleteData(const int32 InDataID);

	/**
	 * 使用数据ID设置当前选中的数据索引
	 * @param InDataID				输入数据ID
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void UseDataIdSetCurrentSelect(const bool ClidkState, const int32 InDataID);

	/**
	 * 更新回放数据索引最大ID
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void UpdatePlaybackDataIndexMaxID();

	/**
	 * 开始采集数据
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void StartDataCollection();

	/**
	 * 结束采集数据
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void EndDataCollection();

	/**
	 * 初始化创建回放数据线程
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void InitPlaybackDataThread();

	/**
	 * 结束回放数据线程
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void EndPlaybackDataThread();

	/**
	 * 添加推演数据集数组到线程
	 * @param InFDataGatherList		输入推演数据集数组
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void AddFDataGatherListToThread(const FDataGatherList& InFDataGatherList);

	/**
	 * 开始回放
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void StartPlayback();

	/**
	 * 暂停回放
	 * @param InPause			输入暂停状态
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void PausePlayback();

	/**
	 * 设置回放倍速
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void SetPlaybackSpeed(const float InSpeed);

	/**
	 * 停止回放
	 */
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void StopPlaybackData();
#pragma endregion
	
#pragma region C++函数
protected:
	/**
	 * 接收数据集
	 * @param InFDataGather		输入数据集
	 */
	void ReceivedFDataGather(const FDataGather& InFDataGather) const;

	/**
	 * 广播是否完成回放
	 */
	void BroadCastPlaybackResult();
#pragma endregion 

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
