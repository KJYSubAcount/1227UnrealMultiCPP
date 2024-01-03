// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

// ��������Ʈ ���� (Ŭ���̾�Ʈ���� ������ ��������Ʈ�� ���� �� ���) 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP, float, CurHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateMag, int, Mag);
// Twoparams�� �ΰ��� �Ķ���͸� �޴´ٴ� �� (FDele_UpdateHP��? float�� CurHP�� float�� MaxHP�� �޴´ٴ� ��)

UCLASS()
class SHOOTINGCODEGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();

public:
	UFUNCTION()
	void AddDamage(float Damage);

	UFUNCTION()
	void AddHeal(float Heal);

	UFUNCTION()
	void AddMag();

	UFUNCTION()
	void UseMag();

public:
	//replicate function
	UFUNCTION()
	void OnRep_CurHP();

	UFUNCTION()
	void OnRep_Mag();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float m_CurHP;

	UPROPERTY(ReplicatedUsing = OnRep_Mag)
	int m_Mag;

	UPROPERTY(BlueprintAssignable, VisibleAnyWhere, BlueprintCallable)
	FDele_UpdateHP Dele_UpdateHP;

	UPROPERTY(BlueprintAssignable, VisibleAnyWhere, BlueprintCallable)
	FDele_UpdateMag Dele_UpdateMag;
};
