// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFlightGun.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values for this component's properties
USimpleFlightGun::USimpleFlightGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ammoCount = 500;
	roundsPerSecond = 7.5f;
	muzzleVelocity = 84000.f;
	fire = false;
	range = 183000.f;
	damage = 5.f;
	fired = true;
	// ...
}


// Called when the game starts
void USimpleFlightGun::BeginPlay()
{
	Super::BeginPlay();
	this->SetComponentTickInterval(1.f/20.f);
	// ...
	

}


void USimpleFlightGun::OnHit(FVector pos, FBullet bullet, FHitResult hit) {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = StringVariable"), "hit"));
	//	DrawDebugSphere(GetWorld(), hit.Location, 100.f, 12, FColor::Emerald, false, 1.f / 20.f, 0, 2.f);
	if (UKismetSystemLibrary::DoesImplementInterface(hit.GetActor(), USimpleWeaponsInterface::StaticClass())) {
		ISimpleWeaponsInterface::Execute_TakeHitDamage(hit.GetActor(), pos, damage);
	}
}


void USimpleFlightGun::SpawnBullet() {
	float lerpFac = 1.f;
	FVector interpPos = FMath::Lerp(prevPos, this->GetComponentLocation(), lerpFac);
	FVector interpDir = FMath::Lerp(prevDir, this->GetForwardVector(), lerpFac);
	FBullet newBullet = FBullet();
	newBullet.direction = interpDir;
	newBullet.pos = interpPos;
	newBullet.speed = muzzleVelocity;
	newBullet.startPos = interpPos;

	bullets.Add(newBullet);
	ammoCount -= 1;
	fired = true;
}

// Called every frame
void USimpleFlightGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	int roundsPerFrame = FMath::RoundToInt(roundsPerSecond * DeltaTime);
	if (!fire || ammoCount <= 0) {
		return;
	}
	timer += DeltaTime;
	if (timer>=1.f/roundsPerSecond) {
		SpawnBullet();
		timer = 0.f;
		fired = false;
	}
	/*
	if (fired) {
		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle,this, &USimpleFlightGun::SpawnBullet, 1.f / roundsPerSecond, true,1.f/roundsPerSecond);
		fired = false;
	}
	*/
	//roundsPerFrame = 1;
	/*
	for (int i = 0; i < roundsPerFrame; i++) {
		float lerpFac = (float)i / (float)roundsPerFrame;
		FVector interpPos = FMath::Lerp(prevPos, this->GetComponentLocation(), lerpFac);
		FVector interpDir = FMath::Lerp(prevDir, this->GetForwardVector(), lerpFac);
		FBullet newBullet = FBullet();
		newBullet.direction = interpDir;
		newBullet.pos = interpPos;
		newBullet.speed = muzzleVelocity;
		newBullet.startPos = interpPos;

		bullets.Add(newBullet);
	}
	*/

	prevPos = this->GetComponentLocation();
	prevDir = this->GetForwardVector();
	//FlushPersistentDebugLines(GetWorld());
	for (int o = 0; o < bullets.Num(); o++)
	{

		FVector newPos = bullets[o].pos + bullets[o].direction * bullets[o].speed*DeltaTime;
		if (FVector::Distance(newPos, bullets[o].startPos) >= range) {
			bullets.RemoveAt(o);
		}
		else {
			//DrawDebugLine(GetWorld(), bullets[o].pos, newPos, FColor::Red, true, DeltaTime, 0, 10.f);
			FHitResult hit;
			FCollisionQueryParams collisionParams;
			collisionParams.AddIgnoredActor(this->GetOwner());
			if (GetWorld()->LineTraceSingleByChannel(hit, bullets[o].pos, newPos, collisionChannel, collisionParams)) {
				
				OnHit(hit.Location,bullets[o],hit);
				bullets.RemoveAt(o);

			}
			else {
				bullets[o].pos = newPos;
			}
		}
		
	}


	// ...
}

