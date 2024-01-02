// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprints/WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SHOOTINGCODEGAME_API AWeapon : public AActor , public IWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 설명 : 무기를 트리거 했을 때 호출되는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventTrigger();

	virtual void EventTrigger_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventShoot();
	// Implementation 은 블루프린트에서 구현한 함수를 호출할 때 사용한다.
	virtual void EventShoot_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventReload();

	virtual void EventReload_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventPickUp(ACharacter* pOwnChar);

	virtual void EventPickUp_Implementation(ACharacter* pOwnChar) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventResetAmmo();

	virtual void EventResetAmmo_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventDrop(ACharacter* pOwnChar);

	virtual void EventDrop_Implementation(ACharacter* pOwnChar) override;

public:
	UFUNCTION(Server, Reliable)
	void ReqShoot(FVector StartLocation, FVector EndLocation);

public:
	float GetFireStartLength();

	// pure를 붙이면 블루프린트에서 구현한 함수를 호출할 때 사용한다.
	UFUNCTION(BlueprintPure)
	bool IsCanShoot();

	bool UseAmmo();

	void SetAmmo(int32 Ammo);

	UFUNCTION(BlueprintCallable)
	void OnUpdateAmmoToHud(int Ammo);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FX")
	class UParticleSystem* m_MuzzleFX;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FX")
	class USoundBase* m_ShootSound;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "FX")
	class USoundBase* m_ReloadSound;

	// 캐릭터 지정
	UPROPERTY(Replicated, EditAnyWhere, BlueprintReadWrite, Category = "Character")
	ACharacter* m_pOwnCharacter;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UAnimMontage* m_ReloadMontage;

	// 총알 발사 애니메이션 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UAnimMontage* m_ShootMontage;

public:
	UFUNCTION()
	void OnRep_Ammo();

	UPROPERTY(ReplicatedUsing = OnRep_Ammo)
	int32 m_Ammo;
};
