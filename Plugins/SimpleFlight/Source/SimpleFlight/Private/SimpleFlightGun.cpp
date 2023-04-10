// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFlightGun.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Components/AudioComponent.h"
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
	//bulletRenderer = CreateDefaultSubobject<ULineBatchComponent>(TEXT("Bullet Tracer Renderer"));
	roundsBwTracer = 5;
	damage = 5.f;
	tracerColor = FColor::Orange;
	tracerIntensity = 10;
	fired = true;
	seed = 0.f;
	playing = false;
//	gunSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Gun Audio"));
//	gunSoundComponent->AdjustAttenuation(attenuationSettings); 
	
	// ...
}


// Called when the game starts
void USimpleFlightGun::BeginPlay()
{
	Super::BeginPlay();
	this->SetComponentTickInterval(1.f/20.f);
	if (WeaponsTracers) {

		niagaraTracers = UNiagaraFunctionLibrary::SpawnSystemAttached(WeaponsTracers, this, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
	
	}
	if (muzzleFlash) {
		muzzleFlashComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(muzzleFlash, this, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, false, false);
	}
	// ...
	seed = FMath::RandRange(0.f, 1.f/roundsPerSecond);
	timer = seed;
	
	gunSoundComponent = Cast < UAudioComponent>(this->GetChildComponent(1));
	//gunSoundComponent->SetSound(gunSound);

}


void USimpleFlightGun::FireWeapons_Implementation(bool triggered, bool& isFiring, EWeaponType weapon) {


	if (weapon == weaponType) {
		fire = triggered;
	}
	if (triggered && weapon==weaponType) {

	if (ammoCount > 0) {
		isFiring = true;
//	fire = true;
		
	}

	}else {
		isFiring = false;
		//fire = false;
	}


}



void USimpleFlightGun::OnHit(FVector pos, FBullet bullet, FHitResult hit) {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = StringVariable"), "hit"));
	//	DrawDebugSphere(GetWorld(), hit.Location, 100.f, 12, FColor::Emerald, false, 1.f / 20.f, 0, 2.f);
	if (HitParticle) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitParticle, hit.Location, FRotator(0.f), FVector(1.f), true, true);
	}
	
	if (UKismetSystemLibrary::DoesImplementInterface(hit.GetActor(), USimpleWeaponsInterface::StaticClass())) {
		ISimpleWeaponsInterface::Execute_TakeHitDamage(hit.GetActor(), pos, damage, hit);
	}
}


void USimpleFlightGun::SpawnBullet() {
	float lerpFac = 1.f;
	FVector interpPos = FMath::Lerp(prevPos, this->GetComponentLocation(), lerpFac);
	FVector interpDir = FMath::Lerp(prevDir, this->GetForwardVector(), lerpFac);
	FBullet newBullet = FBullet();
	newBullet.direction = interpDir;
	newBullet.speed = muzzleVelocity;
	newBullet.pos = interpPos+interpDir* 223.f*FMath::RandRange(0.f,1.f);
	newBullet.startPos = interpPos;

	if (bullets.Num()+1% roundsBwTracer==0) {
		newBullet.tracer = true;
	}
	bullets.Add(newBullet);
	bulletPositions.Add(newBullet.pos);
	bulletVelocities.Add(newBullet.direction);
	ammoCount -= 1;
	fired = true;
	//fire = false;
}

// Called every frame
void USimpleFlightGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	int roundsPerFrame = FMath::RoundToInt(roundsPerSecond * DeltaTime);
	if (fire&&ammoCount>0) {


		timer += DeltaTime;
		if (timer >= 1.f / roundsPerSecond) {
			SpawnBullet();
			timer = 0.f;
			fired = false;
		}
		if (muzzleFlashComponent) {
			muzzleFlashComponent->Activate();
		}
		if (!playing&&gunSoundComponent) {
			playing = true;
			gunSoundComponent->Activate();
			gunSoundComponent->SetVolumeMultiplier(1.f);
		gunSoundComponent->Play(FMath::RandRange(0.f,gunSound->GetDuration()));

		}
	}
	else {
		if (muzzleFlashComponent) {

		muzzleFlashComponent->Deactivate();
		}
		playing = false;
		if (gunSoundComponent) {
			gunSoundComponent->SetVolumeMultiplier(0.f);
		gunSoundComponent->Stop();
		gunSoundComponent->Deactivate();
		}
	}

	if (niagaraTracers) {

	niagaraTracers->Deactivate();
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
		bulletPositions[o] = newPos;
		if (true) {
		//	bulletRenderer->DrawLine(bullets[o].pos,newPos,FColor(tracerColor.R*tracerIntensity, tracerColor.G * tracerIntensity, tracerColor.B * tracerIntensity), 0, damage / 2.f, 1.f / 20.f);
		}

		if (FVector::Distance(newPos, bullets[o].startPos) >= range) {
			bullets.RemoveAt(o);
			bulletPositions.RemoveAt(o);
			bulletVelocities.RemoveAt(o);
		}
		else {
			//DrawDebugLine(GetWorld(), bullets[o].pos, newPos, FColor::Red, true, DeltaTime, 0, 10.f);
			FHitResult hit;
			FCollisionQueryParams collisionParams;
			collisionParams.AddIgnoredActor(this->GetOwner());
			if (GetWorld()->LineTraceSingleByChannel(hit, bullets[o].pos, newPos, collisionChannel, collisionParams)) {
				
				OnHit(hit.Location,bullets[o],hit);
				bullets.RemoveAt(o);
				bulletPositions.RemoveAt(o);
				bulletVelocities.RemoveAt(o);

			}
			else {
				bullets[o].pos = newPos;
			}
		}
		
	}
	if (niagaraTracers) {
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(niagaraTracers, FName("Positions"), bulletPositions);
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(niagaraTracers, FName("Velocities"), bulletVelocities);
		niagaraTracers->SetNiagaraVariableInt(FString("SpawnAmount"), bullets.Num());

		niagaraTracers->Activate();
	}
	// ...
}

