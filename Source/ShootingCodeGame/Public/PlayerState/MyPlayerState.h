// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

// 델리게이트 선언 (클라이언트에서 서버로 델리게이트를 보낼 때 사용) 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP, float, CurHP, float, MaxHP);
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
