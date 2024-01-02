// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "HUD/MyHUD.h"

// Sets default values
AWeapon::AWeapon():m_Ammo(30)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetCollisionProfileName("Weapon");
	WeaponMesh->SetSimulatePhysics(true);
	SetRootComponent(WeaponMesh);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventTrigger_Implementation()
{
	m_pOwnCharacter->PlayAnimMontage(m_ShootMontage);
}

void AWeapon::EventShoot_Implementation()
{
	if (false == IsCanShoot())
		return;

	// 총구 이펙트 크기는 0.1f 로 설정
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_MuzzleFX, WeaponMesh->GetSocketLocation("Muzzle"), WeaponMesh->GetSocketRotation("Muzzle"), FVector(0.1f));
	// 사운드 추가
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_ShootSound, WeaponMesh->GetSocketLocation("Muzzle"));

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (pPlayer0 != m_pOwnCharacter->GetController())
	{
		return;
	}

	// 시작점 설정
	FVector CameraLocation = pPlayer0->PlayerCameraManager->GetCameraLocation();
	FVector CameraForwardVector = pPlayer0->PlayerCameraManager->GetActorForwardVector();
	// 끝점 설정
	FVector StartLocation = (CameraForwardVector * GetFireStartLength()) + CameraLocation;
	FVector EndLocation = CameraLocation + (CameraForwardVector * 5000.0f);

	// 서버에 총알 발사 요청
	ReqShoot(StartLocation, EndLocation);
}

void AWeapon::EventReload_Implementation()
{
	m_pOwnCharacter->PlayAnimMontage(m_ReloadMontage);

	// 사운드 추가
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_ReloadSound, WeaponMesh->GetSocketLocation("Muzzle"));


}

void AWeapon::EventPickUp_Implementation(ACharacter* pOwnChar)
{

	m_pOwnCharacter = pOwnChar;
	m_pOwnCharacter->bUseControllerRotationYaw = true;
	// 무기를 픽업한 캐릭터의 소켓에 무기를 붙인다.
	WeaponMesh->SetSimulatePhysics(false);
	AttachToComponent(m_pOwnCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));

	OnUpdateAmmoToHud(m_Ammo);
}

void AWeapon::EventResetAmmo_Implementation()
{
	SetAmmo(30);
}

void AWeapon::EventDrop_Implementation(ACharacter* pOwnChar)
{
	OnUpdateAmmoToHud(0);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	WeaponMesh->SetSimulatePhysics(true);

	m_pOwnCharacter->bUseControllerRotationYaw = false;
	m_pOwnCharacter = nullptr;

}


void AWeapon::ReqShoot_Implementation(FVector StartLocation, FVector EndLocation)
{
	if (false == UseAmmo())
	{
		return;
	}

	// HitResult 설정
	FHitResult m_HitResult;
	// 콜리전 채널 설정
	FCollisionObjectQueryParams m_CollisionParams;
	m_CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	m_CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	m_CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	m_CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);

	// 무시할 콜리전 채널 설정
	FCollisionQueryParams m_CollisionIgnoreParams;
	m_CollisionIgnoreParams.AddIgnoredActor(this);
	m_CollisionIgnoreParams.AddIgnoredActor(m_pOwnCharacter);

	// 라인트레이스로 히트 체크
	bool bHit = GetWorld()->LineTraceSingleByObjectType(m_HitResult, StartLocation, EndLocation, m_CollisionParams, m_CollisionIgnoreParams);
	// 디버그라인
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f);

	if (false == bHit)
	{
		return;
	}

	// 히트한 액터가 캐릭터일 경우
	ACharacter* pHitCharacter = Cast<ACharacter>(m_HitResult.GetActor());
	if (false == IsValid(pHitCharacter))
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::White, FString::Printf(TEXT("HitCharacter : %s"), *pHitCharacter->GetName()));

	// Apply Damage
	UGameplayStatics::ApplyDamage(pHitCharacter, 10.0f, m_pOwnCharacter->GetController(), this, UDamageType::StaticClass());
}

float AWeapon::GetFireStartLength()
{
	if (false == IsValid(m_pOwnCharacter))   
		return 0.0f;       

	USpringArmComponent* pArm = Cast< USpringArmComponent>(m_pOwnCharacter->GetComponentByClass(USpringArmComponent::StaticClass()));     
	if (false == IsValid(pArm))        
		return 0.0f;      
	return pArm->TargetArmLength + 100;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME는 서버에서 변경된 값들을 클라이언트에게 동기화하기 위한 매크로이며, 네트워크 플레이 시에 서버와 클라이언트 양쪽에서 작동
	DOREPLIFETIME(AWeapon, m_Ammo);
}

bool AWeapon::IsCanShoot()
{
	if (m_Ammo <= 0)
		return false;

	return true;
}

bool AWeapon::UseAmmo()
{
	if (false == IsCanShoot())
		return false;

	m_Ammo = m_Ammo - 1;
	// clamp : 최소값과 최대값을 설정하여 그 사이의 값만을 반환한다.
	m_Ammo = FMath::Clamp(m_Ammo, 0, 30);

	OnRep_Ammo();

	return true;
}

void AWeapon::SetAmmo(int32 Ammo)
{
	m_Ammo = Ammo;
	
	OnRep_Ammo();
}

void AWeapon::OnUpdateAmmoToHud(int Ammo)
{
	if (nullptr == m_pOwnCharacter)
	{
		return;
	}
	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (m_pOwnCharacter->GetController() != pPlayer0)
	{
		return;
	}
	AMyHUD* pHUD = Cast<AMyHUD>(pPlayer0->GetHUD());
	if (nullptr == pHUD)
	{
		return;
	}
	pHUD->OnUpdateMyAmmo(Ammo);

}

void AWeapon::OnRep_Ammo()
{
	OnUpdateAmmoToHud(m_Ammo);
}
	//if(nullptr == m_pOwnCharacter)
	//	return;

	//APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	//if (m_pOwnCharacter->GetController() != pPlayer0)
	//{
	//	return;
	//}
	//AMyHUD* pHUD = Cast<AMyHUD>(pPlayer0->GetHUD());
	//if (nullptr == pHUD)
	//{
	//	return;
	//}
	//pHUD->OnUpdateMyAmmo(m_Ammo);