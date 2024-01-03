// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "ItemInterface.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(m_Scene);
	m_StaticMesh->SetupAttachment(m_Scene);
	m_StaticMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	bReplicates = true;
	SetReplicateMovement(true);

	m_StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemBeginOverlap);
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_StaticMesh->AddRelativeRotation(FRotator(0, 1, 0));
}

void ABaseItem::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult)
{
	IItemInterface* InterfaceObj = Cast<IItemInterface>(OtherActor);

	if (nullptr == InterfaceObj)
	{
		return;
	}

	InterfaceObj->Execute_EventGetItem(OtherActor, m_EItemType);
	Destroy();
}


