// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

// ��������Ʈ ���� (Ŭ���̾�Ʈ���� ������ ��������Ʈ�� ���� �� ���) 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP, float, CurHP, float, MaxHP);
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

public:
	//replicate function
	UFUNCTION()
	void OnRep_CurHP();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float m_CurHP;

	UPROPERTY(BlueprintAssignable, VisibleAnyWhere, BlueprintCallable)
	FDele_UpdateHP Dele_UpdateHP;
};
