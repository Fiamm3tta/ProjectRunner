// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectRunnerCharacter.h"
#include "ProjectRunnerGameMode.h"

// Sets default values
AClearZone::AClearZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 트리거 컴포넌트 생성과 루트 지정
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;
	
	// 트리거 콜리전 설정
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionObjectType(ECC_WorldDynamic);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);

	// 메시 설정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AClearZone::BeginPlay()
{
	Super::BeginPlay();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AClearZone::HandleOverlap);
}

bool AClearZone::IsPlayer(AActor* Other) const
{
	return Other && Other->IsA(AProjectRunnerCharacter::StaticClass());
}

void AClearZone::HandleOverlap(UPrimitiveComponent* OverlappedComp,
							  AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex,
							  bool bFromSweep,
							  const FHitResult& SweepResult)
{
	if(!OtherActor || OtherActor == this) return;
	if(!IsPlayer(OtherActor)) return;
	if(AlreadyCleared) return;

	AlreadyCleared = true;

	if (Trigger) Trigger->SetGenerateOverlapEvents(false);

	if (AProjectRunnerGameMode* GM = GetWorld()->GetAuthGameMode<AProjectRunnerGameMode>()) 
	{
		GM->HandleLevelClear();
	}
    else
	{
		UE_LOG(LogTemp, Warning, TEXT("ClearZone: GameMode not found"));
	}
}
