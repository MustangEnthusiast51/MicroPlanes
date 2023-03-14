// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
//#include "CollisionChannel.h"
#include "SimpleWeaponsInterface.h"
#include "SimpleFlightGun.generated.h"


USTRUCT(BlueprintType)
struct FBullet {
	GENERATED_BODY()
	FVector pos = FVector();
	FVector direction = FVector();
	float speed = 0.f;
	FVector startPos = FVector();

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFLIGHT_API USimpleFlightGun : public USceneComponent
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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector prevPos;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector prevDir;

	TArray<FBullet> bullets;

	bool fired;
	float timer;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnBullet();
	void OnHit(FVector pos, FBullet bullet, FHitResult hit);
		
};
