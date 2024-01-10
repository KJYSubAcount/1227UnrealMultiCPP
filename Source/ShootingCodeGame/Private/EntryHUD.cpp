// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryHUD.h"
#include "Blueprint/UserWidget.h"

void AEntryHUD::BeginPlay()
{
	Super::BeginPlay();

	// hudwidgetclass�� �ִ��� üũ
	check(HudWidgetClass);

	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);

	HUDWidget->AddToViewport();

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();
	if (IsValid(pPlayer0) == false)
	{
		return;
	}
	pPlayer0->SetInputMode(FInputModeUIOnly());
	pPlayer0->bShowMouseCursor = true;

}