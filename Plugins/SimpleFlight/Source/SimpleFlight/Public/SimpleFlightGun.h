// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
//#include "CollisionChannel.h"
#include "SimpleWeaponsInterface.h"
#include "SimpleFlightGun.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
USTRUCT(BlueprintType)
struct FBullet {
	GENERATED_BODY()
	FVector pos = FVector();
	FVector direction = FVector();
	float speed = 0.f;
	FVector startPos = FVector();
	bool tracer;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFLIGHT_API USimpleFlightGun : public USceneComponent, public ISimpleWeaponsInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleFlightGun();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ammoCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float roundsPerSecond;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float muzzleVelocity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool fire;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float range;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float damage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<ECollisionChannel> collisionChannel;

	


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int roundsBwTracer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FColor tracerColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		uint8 tracerIntensity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UNiagaraSystem* WeaponsTracers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UNiagaraSystem* HitParticle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UNiagaraSystem* muzzleFlash;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USoundBase* gunSound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<EWeaponType> weaponType;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector prevPos;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector prevDir;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float seed;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UAudioComponent* gunSoundComponent;

	ULineBatchComponent* bulletRenderer;
	UNiagaraComponent* niagaraTracers;
	UNiagaraComponent* muzzleFlashComponent;
	TArray<FVector> bulletPositions;
	TArray<FBullet> bullets;
	TArray<FVector> bulletVelocities;


	bool fired;
	bool playing;
	float timer;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnBullet();
	void OnHit(FVector pos, FBullet bullet, FHitResult hit);
	virtual void FireWeapons_Implementation(bool triggered, bool& isFiring, EWeaponType weapon);
};
