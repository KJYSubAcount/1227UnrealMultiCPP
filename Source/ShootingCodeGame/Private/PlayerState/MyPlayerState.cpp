// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/MyPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME ������ �ʿ���
#include "HUD/MyHUD.h"

AMyPlayerState::AMyPlayerState()
{
	m_CurHP = 100.f;
	m_Mag = 0;
}

// Replicated ������ �ʿ���
void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME�� �������� ����� ������ Ŭ���̾�Ʈ���� ����ȭ�ϱ� ���� ��ũ���̸�, ��Ʈ��ũ �÷��� �ÿ� ������ Ŭ���̾�Ʈ ���ʿ��� �۵�
	DOREPLIFETIME(AMyPlayerState, m_CurHP);
	DOREPLIFETIME(AMyPlayerState, m_Mag);
}

void AMyPlayerState::AddDamage(float Damage)
{
	m_CurHP = m_CurHP - Damage;

	OnRep_CurHP();
}

void AMyPlayerState::AddHeal(float Heal)
{
	m_CurHP = m_CurHP + Heal;

	OnRep_CurHP();
}

void AMyPlayerState::AddMag()
{
	m_Mag = m_Mag + 1;

	OnRep_Mag();
}

void AMyPlayerState::UseMag()
{
	m_Mag = m_Mag - 1;

	OnRep_Mag();
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

void AMyPlayerState::OnRep_Mag()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, FString::Printf(TEXT("OnRep_Mag! = %d"), m_Mag));

	if (Dele_UpdateMag.IsBound())
	{
		Dele_UpdateMag.Broadcast(m_Mag);
	}
}
