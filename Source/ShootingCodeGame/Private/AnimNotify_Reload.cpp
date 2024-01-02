// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Reload.h"
#include "ShootingCodeGame/ShootingCodeGameCharacter.h"
#include "Blueprints/WeaponInterface.h"

void UAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// ĳ���� Ŭ���� ��������
	AShootingCodeGameCharacter* pCharacter = Cast<AShootingCodeGameCharacter>(MeshComp->GetOwner());
	if (false == IsValid(pCharacter))
	{
		return;
	}

	// ���� �������̽� ��������
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(pCharacter->m_EquipWeapon);
	if (nullptr == InterfaceObj)
	{
		return;
	}

	// ���� �������̽��� �Ѿ� �߻� �Լ� ȣ��
	InterfaceObj->Execute_EventResetAmmo(pCharacter->m_EquipWeapon);
}
