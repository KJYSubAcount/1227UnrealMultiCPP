// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// 아이템 타입
UENUM(BlueprintType)
enum class EItemType : uint8
{
	IT_MAG UMETA(DisplayName = "Mag"),
	IT_HEAL UMETA(DisplayName = "Heal"),
	IT_Etc UMETA(DisplayName = "Etc")
};

UINTERFACE(Blueprintable)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class ITEMPLUGIN_API IItemInterface
{
public:
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventGetItem(EItemType itemType);
};
