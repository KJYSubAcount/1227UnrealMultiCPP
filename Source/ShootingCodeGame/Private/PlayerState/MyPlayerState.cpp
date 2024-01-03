// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/MyPlayerState.h"
#include "Net/UnrealNetwork.h" // DOREPLIFETIME 쓰려면 필요함
#include "HUD/MyHUD.h"

AMyPlayerState::AMyPlayerState()
{
	m_CurHP = 100.f;
	m_Mag = 0;
}

// Replicated 쓰려면 필요함
void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME는 서버에서 변경된 값들을 클라이언트에게 동기화하기 위한 매크로이며, 네트워크 플레이 시에 서버와 클라이언트 양쪽에서 작동
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

// 설명: OnRep_CurHP() 함수는 m_CurHP의 값이 변경될 때마다 호출되는 함수이며, 이 함수를 통해 델리게이트를 호출한다.
void AMyPlayerState::OnRep_CurHP()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Silver, FString::Printf(TEXT("OnRep_CurHP! = %f"), m_CurHP));
	
	// 델리게이트가 바인딩 되어있으면 델리게이트를 호출한다.
	if (Dele_UpdateHP.IsBound()) 
	{
		// 델리게이트를 호출하면서 현재 체력과 최대 체력을 넘겨준다.
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
