// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EntryHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCODEGAME_API AEntryHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay();

public:
	// TSubclassOf : 지정한 클래스와 상속한 모든 클래스를 지정가능.
	// CPP에서 위젯을 다룰려면 모듈 추가가 필요하다. (ShootingCodeGame.Build.cs->"UMG" 추가)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	// 생성한 위젯을 저장할 변수
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HUDWidget;
};
