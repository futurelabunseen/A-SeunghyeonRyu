// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectilePool.h"

// Sets default values
AProjectilePool::AProjectilePool()
{
	InitialPoolSize = 0;
	ExpandSize = 0;

}

// Called when the game starts or when spawned
void AProjectilePool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectClass)
	{
		for (int i = 0; i < InitialPoolSize; i++)
		{
			AUS_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AUS_ProjectileBase>(PooledObjectClass, FVector(0.0f, 0.0f, 1000.0f), FRotator().ZeroRotator);
			SpawnedProjectile->SetProjectileActive(false);
			Pool.Push(SpawnedProjectile);
		}
	}
}

void AProjectilePool::PushProjectileInPool(AUS_ProjectileBase* Projectile)
{
	Pool.Push(Projectile);
}

AUS_ProjectileBase* AProjectilePool::PullProjectile()
{
	if (Pool.Num() == 0)
	{
		Expand();
	}

	return Pool.Pop();
}

void AProjectilePool::Expand()
{
	if (PooledObjectClass)
	{
		for (int i = 0; i < ExpandSize; i++)
		{
			AUS_ProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AUS_ProjectileBase>(PooledObjectClass, FVector().ZeroVector, FRotator().ZeroRotator);
			SpawnedProjectile->SetProjectileActive(false);
			Pool.Push(SpawnedProjectile);
		}
	}
}

