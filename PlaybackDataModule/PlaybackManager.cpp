// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaybackManager.h"
#include "Component/PlaybackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QingLongXia/Manager/GameManager.h"
#include "QingLongXia/Subsystem/LoadActorSubsystem.h"

TWeakObjectPtr<APlaybackManager> APlaybackManager::PlaybackManagerIns = nullptr;

APlaybackManager::APlaybackManager()
{
	PrimaryActorTick.bCanEverTick = true;

	PlaybackManagerIns = MakeWeakObjectPtr(this);

	SceneComponent = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	PlaybackComponent = CreateOptionalDefaultSubobject<UPlaybackComponent>(TEXT("PlaybackComponent"));

	FDataGatherQueue.Empty();
	
}

void APlaybackManager::BeginPlay()
{
	Super::BeginPlay();
	
	//创建回放线程
	PlaybackComponent->InitPlaybackDataThread();
	
	//入队列
	PlaybackComponent->ReturnPlaybackFDataGather_Delegate.AddDynamic(this,&APlaybackManager::AddFDataGatherToQueue);

	//定时器出队
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&APlaybackManager::DelFDataGatherFromQueue,0.01f,true);

#if UE_EDITOR
	SetFolderPath(FName(OutlineDirectory));
#endif
}

void APlaybackManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if 1
	DelFDataGatherFromQueue();
#endif
}

APlaybackManager* APlaybackManager::GetPtr()
{
	return PlaybackManagerIns.Get();
}

void APlaybackManager::AddFDataGatherToQueue(FDataGather InFDataGather)
{
	FDataGatherQueue.Enqueue(InFDataGather);
}

void APlaybackManager::AddCacheJsonStrMap(ETopicType TopicType, const FString& JsonString)
{
	switch (TopicType)
	{
	//编组信息 销毁场景Actor
	case ETopicType::XL_TY_BLBS:
		{
			GetWorld()->GetGameInstance()->GetSubsystem<ULoadActorSubsystem>()->DestoryAllEntity();
			break;
		}
	default:
		break;
	}
	//更新Map
	CacheJsonStrMap.Add(TopicType,JsonString);
	//发送消息至GameManager
	SendMessageToGameManager(TopicType,JsonString);

}

void APlaybackManager::SendMessageToGameManager(ETopicType Topic, const FString& Data)
{
	AGameManager::GetPtr()->RecevData(*AGameManager::GetPtr()->TopicMap.FindKey(Topic),Data);
}

void APlaybackManager::DelFDataGatherFromQueue()
{
	if (!FDataGatherQueue.IsEmpty())
	{
		FDataGather LocalDataGather;
		FDataGatherQueue.Dequeue(LocalDataGather);

		FString RstString;
		FJsonObjectConverter::UStructToJsonObjectString<FDataGather>(LocalDataGather,RstString,0,0,0);

		RstString.ReplaceInline(L"\r\n\t",L"");
		RstString.ReplaceInline(L"\r\n",L"");
		RstString.ReplaceInline(L"\t",L"");

		if (RstString != LastJsonString)
		{
			UVaRestSubsystem* VaRestSubsystem = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
			UVaRestJsonObject* VaRestJsonObject = VaRestSubsystem->ConstructVaRestJsonObject();
			//ReturnFDataGather(TempFDataGather)
			VaRestJsonObject->DecodeJson(RstString);
			//遍历解析Map.key对象
			for (auto Item : TopicMap)
			{
				FString	LocalJSonString;
				//编组信息存储为字符串，非对象，特殊处理

				switch (Item.Value)
				{
				case ETopicType::XL_TY_ZZBZ:
						LocalJSonString = VaRestJsonObject->GetStringField(Item.Key);
						break;
					default:
						LocalJSonString = VaRestJsonObject->GetObjectField(Item.Key)->EncodeJsonToSingleString();
						break;
				}

				//Topic 是否存在于JsonString缓存池中
				if (CacheJsonStrMap.Contains(Item.Value))
				{
					//Key存在且Value发生改变
					if (LocalJSonString != CacheJsonStrMap[Item.Value])
					{
						AddCacheJsonStrMap(Item.Value,LocalJSonString);
					}
				}
				else
				{
					AddCacheJsonStrMap(Item.Value,LocalJSonString);
				}
			}
		}
		LastJsonString = RstString;
	}
}

void APlaybackManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Log, TEXT("%hs %s"), __FUNCTION__, TEXT("GC !"));
}

