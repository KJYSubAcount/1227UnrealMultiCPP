// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/MyPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME ������ �ʿ���

AMyPlayerState::AMyPlayerState()
{
	m_CurHP = 100.f;
}

// Replicated ������ �ʿ���
void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME�� �������� ����� ������ Ŭ���̾�Ʈ���� ����ȭ�ϱ� ���� ��ũ���̸�, ��Ʈ��ũ �÷��� �ÿ� ������ Ŭ���̾�Ʈ ���ʿ��� �۵�
	DOREPLIFETIME(AMyPlayerState, m_CurHP);
}

void AMyPlayerState::AddDamage(float Damage)
{
	m_CurHP = m_CurHP - Damage;

	OnRep_CurHP();
}

// ����: OnRep_CurHP() �Լ��� m_CurHP�� ���� ����� ������ ȣ��Ǵ� �Լ��̸�, �� �Լ��� ���� ��������Ʈ�� ȣ���Ѵ�.
void AMyPlayerState::OnRep_CurHP()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, FString::Printf(TEXT("OnRep_CurHP! = %f"), m_CurHP));
	
	// ��������Ʈ�� ���ε� �Ǿ������� ��������Ʈ�� ȣ���Ѵ�.
	if (Dele_UpdateHP.IsBound()) 
	{
		// ��������Ʈ�� ȣ���ϸ鼭 ���� ü�°� �ִ� ü���� �Ѱ��ش�.
		Dele_UpdateHP.Broadcast(m_CurHP, 100.f);
	}
}
