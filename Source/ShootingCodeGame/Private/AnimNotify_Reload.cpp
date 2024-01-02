// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Reload.h"
#include "ShootingCodeGame/ShootingCodeGameCharacter.h"
#include "Blueprints/WeaponInterface.h"

void UAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
	InterfaceObj->Execute_EventResetAmmo(pCharacter->m_EquipWeapon);
}
