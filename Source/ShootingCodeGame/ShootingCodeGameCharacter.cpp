// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingCodeGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/MyPlayerState.h"
#include "Blueprints/Weapon.h"
//위젯
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AShootingCodeGameCharacter

AShootingCodeGameCharacter::AShootingCodeGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AShootingCodeGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME는 서버에서 변경된 값들을 클라이언트에게 동기화하기 위한 매크로이며, 네트워크 플레이 시에 서버와 클라이언트 양쪽에서 작동
	DOREPLIFETIME(AShootingCodeGameCharacter, ControlRot);
}

void AShootingCodeGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	check(NameTagClass);
	NameTagWidget = CreateWidget<UUserWidget>(GetWorld(), NameTagClass);
	NameTagWidget->AddToViewport();

	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(TH_NameTag, this, &AShootingCodeGameCharacter::EventUpdateNameTag, 0.01f, true);

	BindPlayerState();
}

void AShootingCodeGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// HasAuthority()는 true 일때 서버에서 작동/ 아닐시 클라이언트에서 작동
	if (HasAuthority() == true)
	{
		ControlRot = GetControlRotation();

	}
}

float AShootingCodeGameCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, FString::Printf(TEXT("TakeDamage DamageAmount %f EventInstigator=%s DamageCausor=%s"),DamageAmount, *EventInstigator->GetName(),*DamageCauser->GetName()));

	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	if (false == IsValid(MyPlayerState))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("PlayerState Failed!"));
		return 0.0f;
	}

	MyPlayerState->AddDamage(DamageAmount);

	return 0.0f;
}


/////////////////================네트워크================/////////////////
// Network

void AShootingCodeGameCharacter::ReqReloadAnim_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, TEXT("Reload_OnServer!"));
	ResReloadAnim();
}

void AShootingCodeGameCharacter::ResReloadAnim_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, TEXT("Reload_Multi!"));
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
	{
		return;
	}

	InterfaceObj->Execute_EventReload(m_EquipWeapon);
}

void AShootingCodeGameCharacter::ReqTriggerAnim_Implementation()
{
	ResTriggerAnim();
}

void AShootingCodeGameCharacter::ResTriggerAnim_Implementation()
{
	//PlayAnimMontage(ShootMontage);
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
	{
		return;
	}

	InterfaceObj->Execute_EventTrigger(m_EquipWeapon);
}

void AShootingCodeGameCharacter::ReqPressF_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("ReqPressF!"));
	AActor* pNearActor = FindNearestWeapon();
	if (false == IsValid(pNearActor))
	{
		return;
	}

	if (nullptr != m_EquipWeapon)
	{
		m_EquipWeapon->SetOwner(nullptr);
	}

	pNearActor->SetOwner(GetController());
	ResPressF(pNearActor);
}

void AShootingCodeGameCharacter::ResPressF_Implementation(AActor* PickUpActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("ResPressF!"));

	if (nullptr != m_EquipWeapon)
	{
		IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

		if (nullptr == InterfaceObj)
		{
			return;
		}

		InterfaceObj->Execute_EventDrop(m_EquipWeapon, this);
		m_EquipWeapon = nullptr;
	}

	m_EquipWeapon = PickUpActor;

	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
	{
		return;
	}

	InterfaceObj->Execute_EventPickUp(m_EquipWeapon, this);
}

void AShootingCodeGameCharacter::ResClientPressF_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, TEXT("ResClientPressF!"));
}

void AShootingCodeGameCharacter::ReqDrop_Implementation()
{
	if (false == IsValid(m_EquipWeapon))
	{
		return;
	}

	m_EquipWeapon->SetOwner(nullptr);
	ResDrop();
}

void AShootingCodeGameCharacter::ResDrop_Implementation()
{
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(m_EquipWeapon);

	if (nullptr == InterfaceObj)
	{
		return;
	}

	InterfaceObj->Execute_EventDrop(m_EquipWeapon, this);
	m_EquipWeapon = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShootingCodeGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShootingCodeGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShootingCodeGameCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(TriggerAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::Trigger);

		// "F"Press
		EnhancedInputComponent->BindAction(PressFAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::PressF);

		// Reloading
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::Reload);

		// Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &AShootingCodeGameCharacter::Drop);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AShootingCodeGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AShootingCodeGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShootingCodeGameCharacter::Trigger(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, TEXT("Shoot!"));
	
	ReqTriggerAnim();
}

void AShootingCodeGameCharacter::PressF(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, TEXT("PressF!"));
	ReqPressF();
}

void AShootingCodeGameCharacter::Reload(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, TEXT("Reload!"));
	ReqReloadAnim();
}

void AShootingCodeGameCharacter::Drop(const FInputActionValue& Value)
{
	ReqDrop();
}

void AShootingCodeGameCharacter::EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass)
{
	if (false == HasAuthority())
	{
		return;
	}
	// 아래 코드 설명 : GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector(0,0,0), FRotator(0,0,0)) 를 
	// 통해 WeaponClass 를 스폰하고, 그것을 m_EquipWeapon 에 넣어준다.
	m_EquipWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector(0,0,0), FRotator(0,0,0));

	// m_EquipWeapon 의 부모 클래스인 AWeapon 의 포인터를 pWeapon 에 넣어준다.
	AWeapon* pWeapon = Cast<AWeapon>(m_EquipWeapon);
	if(false == IsValid(pWeapon))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("pWeapon is not valid!"));
		return;
	}
	// pWeapon 의 m_pOwnCharacter 에 this 를 넣어준다.
	pWeapon->m_pOwnCharacter = this;
	
	TestEquipSetOwner();

	// m_EquipWeapon 을 GetMesh() 에 Attach 시킨다.
	m_EquipWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
}

void AShootingCodeGameCharacter::TestEquipSetOwner()
{
	if (IsValid(GetController()))
	{
		m_EquipWeapon->SetOwner(GetController());
	}

	FTimerManager& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(TH_BindSetOwner, this, &AShootingCodeGameCharacter::TestEquipSetOwner, 0.1f, false);
}

AActor* AShootingCodeGameCharacter::FindNearestWeapon()
{
	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, AWeapon::StaticClass());

	double NearDist = 999;
	AActor* pNearActor = nullptr;
	for (AActor* pTarget : OverlappingActors)
	{
		if (m_EquipWeapon == pTarget)
		{
			continue;
		}

		double Dist = FVector::Distance(GetActorLocation(), pTarget->GetActorLocation());
		// 만약, NearDist 가 0 이거나, Dist 가 NearDist 보다 작다면
		if (Dist >= NearDist)
		{
			continue;
		}

		NearDist = Dist;
		pNearActor = pTarget;
	}
	return pNearActor;
}

void AShootingCodeGameCharacter::EventUpdateNameTag_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, TEXT("EventUpdateNameTag!"));
}

void AShootingCodeGameCharacter::EventUpdateNameTagHP_Implementation(int CurHP, int MaxHP)
{
}

void AShootingCodeGameCharacter::BindPlayerState()
{
}
