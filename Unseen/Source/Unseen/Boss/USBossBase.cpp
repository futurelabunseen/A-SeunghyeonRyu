// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/USBossBase.h"

// Sets default values
AUSBossBase::AUSBossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsBattleStart = false;
}

// Called when the game starts or when spawned
void AUSBossBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSBossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsBattleStart)
	{
		FVector CharacterLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		float Distance = FVector::Dist(GetActorLocation(), CharacterLocation);
		if (Distance < 3000.0f)
		{
			bIsBattleStart = true;
			LimitBattleZone();
		}
		// ���� �ȿ� �÷��̾� ���Դ��� üũ
		// ������ LimitBattleZone(��������)�ϰ� bIsBattleStart = true;
	}

}

// Called to bind functionality to input
void AUSBossBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUSBossBase::LimitBattleZone()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Battle Zone"));
	// �� ���׶��� ������ ���� ����
}

