// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerState/MyPlayerState.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	// hudwidgetclass가 있는지 체크
	check(HudWidgetClass);

	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);

	HUDWidget->AddToViewport();

	// 델리게이트 바인딩
	BindMyPlayerState();
}

// 설명: 델리게이트를 바인딩하는 함수
void AMyHUD::BindMyPlayerState()
{
	// GetWorld()->GetFirstPlayerController() : 현재 월드에 있는 플레이어 컨트롤러를 가져온다.
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (IsValid(PC))
	{
		// 플레이어 컨트롤러의 플레이어 스테이트를 가져온다.
		AMyPlayerState* PS = Cast<AMyPlayerState>(PC->PlayerState);
		if (IsValid(PS))
		{
			// 플레이어 스테이트를 지정해서 캐스트 후 플레이어 컨트롤러의 플레이어 스테이트를 가져온다.
			PS->Dele_UpdateHP.AddDynamic(this, &AMyHUD::OnUpdateMyHP);

			// 플레이어 스테이트의 현재 체력과 최대 체력을 넘겨준다.
			OnUpdateMyHP(PS->m_CurHP, 100.f);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("HUD Bind Success!")));
			return;
		}
	}

	// 플레이어 스테이트가 없으면 타이머 핸들을 이용해서 바인딩을 다시 시도한다.
	// 설명 : FTimerManager& TM = GetWorld()->GetTimerManager(); : 타이머 핸들을 가져온다.
	FTimerManager& TM = GetWorld()->GetTimerManager();
	// 설명 : SetTimer(FTimerHandle& Handle, UObject* Obj, FName FuncName, float Time, bool Loop) : 타이머를 설정한다.
	TM.SetTimer(TH_BindMyPlayerState, this, &AMyHUD::BindMyPlayerState, 0.1f, false);
}

void AMyHUD::OnUpdateMyHP_Implementation(float CurHP, float MaxHP)
{
}

void AMyHUD::OnUpdateMyAmmo_Implementation(int Ammo)
{
}
