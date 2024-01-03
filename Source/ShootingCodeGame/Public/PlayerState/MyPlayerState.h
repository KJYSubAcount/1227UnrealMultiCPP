// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

// 델리게이트 선언 (클라이언트에서 서버로 델리게이트를 보낼 때 사용) 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP, float, CurHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateMag, int, Mag);
// Twoparams는 두개의 파라미터를 받는다는 뜻 (FDele_UpdateHP는? float형 CurHP와 float형 MaxHP를 받는다는 뜻)

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
