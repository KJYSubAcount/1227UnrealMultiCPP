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

	// �������� �޾��� �� ȣ��Ǵ� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


//==========��Ʈ��ũ �ڵ� ����==========//
public:

// ���������� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(Server, Reliable)
	void ReqPressF();

// ��� Ŭ���̾�Ʈ���� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void ResPressF(AActor* PickUpActor);

// Ŭ���̾�Ʈ���� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(Client, Reliable)
	void ResClientPressF();

public:
	// ���������� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(Server, Reliable)
	void ReqTriggerAnim();

	// ��� Ŭ���̾�Ʈ���� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void ResTriggerAnim();

	// ���������� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(Server, Reliable)
	void ReqReloadAnim();

	// ��� Ŭ���̾�Ʈ���� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(NetMulticast, Reliable)
	void ResReloadAnim();

	// ���������� �ݵ�� ����Ǵ� �Լ�
	UFUNCTION(Server, Reliable)
	void ReqDrop();

	// ��� Ŭ���̾�Ʈ���� �ݵ�� ����Ǵ� �Լ�
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
	//====== ������ �������̽� ���� ======//
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventGetItem(EItemType itemType);

	void EventGetItem_Implementation(EItemType itemType) override;

public:
	//���� ������ ����
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

