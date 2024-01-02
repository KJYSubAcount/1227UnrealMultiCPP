// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "PlayerState/MyPlayerState.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	// hudwidgetclass�� �ִ��� üũ
	check(HudWidgetClass);

	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);

	HUDWidget->AddToViewport();

	// ��������Ʈ ���ε�
	BindMyPlayerState();
}

// ����: ��������Ʈ�� ���ε��ϴ� �Լ�
void AMyHUD::BindMyPlayerState()
{
	// GetWorld()->GetFirstPlayerController() : ���� ���忡 �ִ� �÷��̾� ��Ʈ�ѷ��� �����´�.
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (IsValid(PC))
	{
		// �÷��̾� ��Ʈ�ѷ��� �÷��̾� ������Ʈ�� �����´�.
		AMyPlayerState* PS = Cast<AMyPlayerState>(PC->PlayerState);
		if (IsValid(PS))
		{
			// �÷��̾� ������Ʈ�� �����ؼ� ĳ��Ʈ �� �÷��̾� ��Ʈ�ѷ��� �÷��̾� ������Ʈ�� �����´�.
			PS->Dele_UpdateHP.AddDynamic(this, &AMyHUD::OnUpdateMyHP);

			// �÷��̾� ������Ʈ�� ���� ü�°� �ִ� ü���� �Ѱ��ش�.
			OnUpdateMyHP(PS->m_CurHP, 100.f);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("HUD Bind Success!")));
			return;
		}
	}

	// �÷��̾� ������Ʈ�� ������ Ÿ�̸� �ڵ��� �̿��ؼ� ���ε��� �ٽ� �õ��Ѵ�.
	// ���� : FTimerManager& TM = GetWorld()->GetTimerManager(); : Ÿ�̸� �ڵ��� �����´�.
	FTimerManager& TM = GetWorld()->GetTimerManager();
	// ���� : SetTimer(FTimerHandle& Handle, UObject* Obj, FName FuncName, float Time, bool Loop) : Ÿ�̸Ӹ� �����Ѵ�.
	TM.SetTimer(TH_BindMyPlayerState, this, &AMyHUD::BindMyPlayerState, 0.1f, false);
}

void AMyHUD::OnUpdateMyHP_Implementation(float CurHP, float MaxHP)
{
}

void AMyHUD::OnUpdateMyAmmo_Implementation(int Ammo)
{
}
