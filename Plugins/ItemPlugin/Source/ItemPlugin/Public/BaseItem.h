// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

UCLASS()
class ITEMPLUGIN_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	UStaticMeshComponent* m_StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	USceneComponent* m_Scene;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	EItemType m_EItemType;

public:
	UFUNCTION()
	void OnItemBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult);
};
