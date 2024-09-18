// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreMinimal.h"
#include "DataDefine/PlaybackDataDefine.h"

struct FDataGather;

//返回回放数据代理
DECLARE_DELEGATE_OneParam(FReturnFDataGatherDelegate, const FDataGather&);
DECLARE_DELEGATE(FPlaybackFinishedDelegate);

/**
 * 回放数据线程
 */
class QINGLONGXIA_API FPlaybackDataThread : public FRunnable
{
public:
	FReturnFDataGatherDelegate ReturnFDataGatherDelegate;
	FPlaybackFinishedDelegate PlaybackFinishedDelegate;

	FPlaybackDataThread();

	virtual ~FPlaybackDataThread() override;

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	void SetActivate(const bool InActivate);

	void AddFDataGatherList(const FDataGatherList& InFDataGatherList);

	void SetPause(const bool InPause);

	void SetSpeed(const float InSpeed);

	void StopPlaybackData();

private:
	/** 线程循环控制 */
	bool Stopping;

	/** 是否激活 */
	bool IsActivate;

	/** 是否暂停 */
	bool IsPause;

	/** 回放速度 */
	float Speed;

	/** 推演数据集数组 */
	FDataGatherList CacheDataGatherList;

	/** 当前索引 */
	int32 CurrentIndex;

	/** 最大索引 */
	int32 MaxIndex;
};
