// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/USBossBase.h"
#include "UI/BossFightHUD.h"

// Sets default values
AUSBossBase::AUSBossBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsBattleStart = false;
	MaxHp = 0;
	CurrentHp = MaxHp;
	BossFightHUDClass = nullptr;
	BossFightHUD = nullptr;
	BattleZoneBPClass = nullptr;
	BattleZone = nullptr;
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
		// ���߿� ���� �ϳ� �������� ��������Ʈ�� ����
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
	if (BossFightHUDClass)
	{
		BossFightHUD = CreateWidget<UBossFightHUD>(GetWorld()->GetFirstPlayerController(), BossFightHUDClass);
		if (nullptr != BossFightHUD)
		{
			BossFightHUD->SetHealthBar(CurrentHp, MaxHp);
			BossFightHUD->AddToPlayerScreen();
		}
	}
}

float AUSBossBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsBattleStart)
	{
		if (CurrentHp <= DamageAmount)
		{
			CurrentHp = 0;
			UE_LOG(LogTemp, Warning, TEXT("Boss Die"));
			
			if (BossFightHUD)
			{
				BossFightHUD->RemoveFromParent();
				BossFightHUD = nullptr;
			}
			if (BattleZone)
			{
				BattleZone->Destroy();
			}
			Destroy();
		}
		else
		{
			CurrentHp -= DamageAmount;
			UE_LOG(LogTemp, Warning, TEXT("Boss Hp : %d"), CurrentHp);
		}

		if (BossFightHUD)
		{
			BossFightHUD->SetHealthBar(CurrentHp, MaxHp);
		}

		return DamageAmount;
	}
	return 0.0f;
}

