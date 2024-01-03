// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ShootingCodeGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AShootingCodeGameCharacter : public ACharacter, public IItemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TriggerAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PressFAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropAction;

public:
	AShootingCodeGameCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Movement Shoot input */
	void Trigger(const FInputActionValue& Value);

	/** Called for PressF input */
	void PressF(const FInputActionValue& Value);

	/** Called for PressF input */
	void Reload(const FInputActionValue& Value);

	void Drop(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	// 데미지를 받았을 때 호출되는 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


//==========네트워크 코드 영역==========//
public:

// 서버에서만 반드시 실행되는 함수
	UFUNCTION(Server, Reliable)
	void ReqPressF();

// 모든 클라이언트에게 반드시 실행되는 함수
	UFUNCTION(NetMulticast, Reliable)
	void ResPressF(AActor* PickUpActor);

// 클라이언트에서 반드시 실행되는 함수
	UFUNCTION(Client, Reliable)
	void ResClientPressF();

public:
	// 서버에서만 반드시 실행되는 함수
	UFUNCTION(Server, Reliable)
	void ReqTriggerAnim();

	// 모든 클라이언트에게 반드시 실행되는 함수
	UFUNCTION(NetMulticast, Reliable)
	void ResTriggerAnim();

	// 서버에서만 반드시 실행되는 함수
	UFUNCTION(Server, Reliable)
	void ReqReloadAnim();

	// 모든 클라이언트에게 반드시 실행되는 함수
	UFUNCTION(NetMulticast, Reliable)
	void ResReloadAnim();

	// 서버에서만 반드시 실행되는 함수
	UFUNCTION(Server, Reliable)
	void ReqDrop();

	// 모든 클라이언트에게 반드시 실행되는 함수
	UFUNCTION(NetMulticast, Reliable)
	void ResDrop();

public:
	UFUNCTION(BlueprintCallable)
	void EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable)
	void TestEquipSetOwner();

	AActor* FindNearestWeapon();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void EventUpdateNameTag();

	void EventUpdateNameTag_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventUpdateNameTagHP(float CurHP, float MaxHP);

	void EventUpdateNameTagHP_Implementation(float CurHP, float MaxHP);

	void BindPlayerState();

public:
	//====== 아이템 인터페이스 구현 ======//
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventGetItem(EItemType itemType);

	void EventGetItem_Implementation(EItemType itemType) override;

public:
	//에임 오프셋 적용
	UPROPERTY(Replicated, BlueprintReadWrite)
	FRotator ControlRot;

	UPROPERTY(BlueprintReadWrite)
	AActor* m_EquipWeapon;

	UPROPERTY(BlueprintReadWrite)
	AActor* m_EquipMag;

	FTimerHandle TH_BindSetOwner;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> NameTagClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* NameTagWidget;

	FTimerHandle TH_NameTag;

	FTimerHandle TH_BindPlayerState;
};

