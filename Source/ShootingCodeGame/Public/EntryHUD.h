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
	// TSubclassOf : ������ Ŭ������ ����� ��� Ŭ������ ��������.
	// CPP���� ������ �ٷ���� ��� �߰��� �ʿ��ϴ�. (ShootingCodeGame.Build.cs->"UMG" �߰�)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	// ������ ������ ������ ����
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HUDWidget;
};
