// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/US_ProjectileBase.h"
#include "ProjectilePool.generated.h"

UCLASS()
class UNSEEN_API AProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectilePool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UFUNCTION()
	void PushProjectileInPool(AUS_ProjectileBase* Projectile);
	
	UFUNCTION()
	AUS_ProjectileBase* PullProjectile();

protected:
	TArray<class AUS_ProjectileBase*> Pool;
	
	// ó�� ��������� ������
	UPROPERTY()
	int InitialPoolSize;
	// 0���϶� pull�ϸ� ��������� ����
	UPROPERTY()
	int ExpandSize;

	UPROPERTY()
	TSubclassOf<class AUS_ProjectileBase> PooledObjectClass;

	UFUNCTION()
	void Expand();
};
