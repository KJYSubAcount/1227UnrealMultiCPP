// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/AnimNotify_Shoot.h"
#include "ShootingCodeGame/ShootingCodeGameCharacter.h"
#include "Blueprints/WeaponInterface.h"

// 아래 함수는 애니메이션 노티파이를 통해 호출되는 함수입니다.
void UAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 캐릭터 클래스 가져오기
	AShootingCodeGameCharacter* pCharacter = Cast<AShootingCodeGameCharacter>(MeshComp->GetOwner());
	if (false == IsValid(pCharacter))
	{
		return;
	}

	// 무기 인터페이스 가져오기
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(pCharacter->m_EquipWeapon);
	if (nullptr == InterfaceObj)
	{
		return;
	}

	// 무기 인터페이스의 총알 발사 함수 호출
	InterfaceObj->Execute_EventShoot(pCharacter->m_EquipWeapon);
}
