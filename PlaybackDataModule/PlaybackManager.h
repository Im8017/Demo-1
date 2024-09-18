// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QingLongXia/Manager/GameManager.h"
#include "PlaybackManager.generated.h"

struct FDataGather;

UCLASS()
class QINGLONGXIA_API APlaybackManager : public AActor
{
	GENERATED_BODY()

public:
	APlaybackManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	static APlaybackManager* GetPtr();
#pragma region 类组件
protected:
	/** 场景根组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="C++_Variate")
	class USceneComponent* SceneComponent;


#pragma endregion

#pragma region 蓝图变量
public:
	/** 类实例在世界大纲中的目录 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="C++_Variate")
	FString OutlineDirectory;
#pragma endregion

#pragma region C++变量
public:
	/** 回放组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="C++_Variate")
	class UPlaybackComponent* PlaybackComponent;
	
	/* 数据集队列 */
	TQueue<FDataGather> FDataGatherQueue;
	
	/* 定时器句柄 */
	UPROPERTY()
	FTimerHandle TimerHandle;
	
	FString LastJsonString;
	
	
	/* <JsonObjectString,Topic>映射 */
	TMap<FString , ETopicType> TopicMap = {
		//{"terminalStateInfoArray",ETopicType::XL_STZTXX},						//器材信息
		{"battleAreaInfo",ETopicType::XL_TY_FAXX},					//演习区域信息
		//{"cannoBallFallInfo","XL_TY_LDQ"},				//落弹区信息
		{"bunkerInfo",ETopicType::XL_TY_YT},						//掩体信息
		{"dodgePointInfo",ETopicType::XL_TY_DBD},					//躲避点信息
		{"constractionInfo",ETopicType::XL_TY_GSZA},				//工程障碍信息
		{"forceDispositionInfo",ETopicType::XL_TY_BLBS},			//兵力部署信息
		//{"drillStageInfo","XL_TY_JDXX"},					//演习阶段信息
		{"drillPlanInfo",ETopicType::XL_TY_JHXX},					//演习计划信息

		//{"marshallingInfo",ETopicType::XL_TY_ZZBZ},					//编组信息-----弃用
		{"groupInfo",ETopicType::XL_TY_ZZBZ},
		//{"terminalDistribution","XL_TY_QCFP"},			//器材分配信息
		//{"verdictInfoArray","XL_CJ_CJXX"},				//裁决信息数组
		//{"terminalControlCommand","DK_QCKZZL"},
		//{"systemStatusReply","DK_XTZTYD"}
		{"barrierCreateAry",ETopicType::XL_BARRIER}				//障碍信息
	};
	
	/* <TopicType,Json字符串>映射 */
	TMap<ETopicType , FString> CacheJsonStrMap;
#pragma endregion

#pragma region 蓝图函数
public:
	UFUNCTION(BlueprintCallable, Category="C++_Function")
	void AddFDataGatherToQueue(FDataGather InFDataGather);
	
	//UFUNCTION(BlueprintImplementableEvent, Category="C++_Function")
	//void ReturnFDataGather(const FDataGather OutFDataGather);
#pragma endregion

#pragma region C++函数

	/**
	 * 更新缓存Map
	 * @param TopicType Topic类型
	 * @param JsonString JsonString字符串
	 */
	UFUNCTION()
	void AddCacheJsonStrMap(ETopicType TopicType,const FString& JsonString);

	/**
	 * 发送消息至GameManager
	 * @param Topic Topic类型
	 * @param Data Json
	 */
	UFUNCTION()
	static void SendMessageToGameManager(ETopicType Topic, const FString& Data);

	/* 出队 */
	UFUNCTION()
	void DelFDataGatherFromQueue();

#pragma endregion

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	static  TWeakObjectPtr<APlaybackManager>	PlaybackManagerIns;
};
