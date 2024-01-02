// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */

UCLASS()
class SHOOTINGCODEGAME_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay();

public:
	void BindMyPlayerState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyHP(float CurHP, float MaxHP);

	void OnUpdateMyHP_Implementation(float CurHP, float MaxHP);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyAmmo(int Ammo);

	void OnUpdateMyAmmo_Implementation(int Ammo);

public:
	// TSubclassOf : ������ Ŭ������ ����� ��� Ŭ������ ��������.
	// CPP���� ������ �ٷ���� ��� �߰��� �ʿ��ϴ�. (ShootingCodeGame.Build.cs->"UMG" �߰�)
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HudWidgetClass;

	// ������ ������ ������ ����
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HUDWidget;

	// ��������Ʈ ���ε��� ���� Ÿ�̸� �ڵ�
	FTimerHandle TH_BindMyPlayerState;
};