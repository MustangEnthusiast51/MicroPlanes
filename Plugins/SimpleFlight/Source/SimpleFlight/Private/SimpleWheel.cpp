// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleWheel.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values for this component's properties
USimpleWheel::USimpleWheel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	springLength = 100.f;
	wheelRadius = 50.f;
	restLength = 1.f;
	springForce = 4000.f;
	dampingForce = 2000.f;
	physicsTimestep = 1.f / 60.f;
	// ...
}



FSFForce USimpleWheel::ReportSimpleForce_Implementation(FTransform overrideTransform, bool substep) {

	FTransform myTransform = GetComponentTransform();

	if (substep) {
		
		myTransform = relTransform*overrideTransform;
	}

	return CalculateForces(myTransform);

}

void USimpleWheel::DrawSFDebug_Implementation() {
	DrawDebugLine(GetWorld(), this->GetComponentLocation(), this->GetComponentTransform().TransformPosition(-FVector(0.f, 0.f, springLength + wheelRadius)), FColor::Magenta, true, 100.f, 0, 2.f);
	DrawDebugSphere(GetWorld(), this->GetComponentTransform().TransformPosition(-FVector(0.f, 0.f, springLength + wheelRadius)), wheelRadius, 12, FColor::Magenta, true, 100.f, 0, 2.f);
}
// Called when the game starts
void USimpleWheel::BeginPlay()
{
	Super::BeginPlay();

	
	relTransform = GetComponentTransform()*GetOwner()->GetRootComponent()->GetComponentTransform().Inverse() ;
	implementsInterface = UKismetSystemLibrary::DoesImplementInterface(this->GetOwner(), USimpleFlightInterface::StaticClass());
	// ...
	
}


// Called every frame
void USimpleWheel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FSFForce USimpleWheel::CalculateForces(FTransform transform) {

	FVector start = transform.GetLocation();
	FVector end = transform.TransformPosition(-FVector(0.f,0.f,springLength+wheelRadius));
	FHitResult hit;
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(GetOwner());
	FSFForce force;
	force.force = FVector(0.f);
	force.torque = FVector(0.f);
	force.worldPos = FVector(0.f);
	lastLength = currentLength;
	currentLength = springLength;

	
	bool traceResult = this->GetWorld()->LineTraceSingleByChannel(hit, start, end, channel, params);
	//FTraceHandle handle = this->GetWorld()->AsyncLineTraceByChannel(EAsyncTraceType::Single, start, end, channel, params);

	
	if (traceResult) {
		
		
		currentLength = FMath::Clamp(hit.Distance-wheelRadius,0.f,springLength);
		float netSpringForce = ((springLength * restLength) - currentLength)*springForce;
		netSpringForce = FMath::Max(0.f, netSpringForce);
		float netDampingForce = dampingForce*(lastLength - currentLength) / physicsTimestep;

		netSpringForce += netDampingForce;
		
		float gripForce = 0.f;
		
		if (implementsInterface) {
			FVector velocity = ISimpleFlightInterface::Execute_VelocityAtPoint(GetOwner(), hit.Location);
			velocity = transform.InverseTransformVector(velocity);
			gripForce = velocity.Y * -sideGrip;
		}

	DrawDebugSphere(GetWorld(), hit.Location, wheelRadius, 12, FColor::Magenta, false, 1.f/60.f, 0, 2.f);



		zeroToOneLength = currentLength / springLength;
		//GEngine->AddOnScreenDebugMessage(-1, 1.f/60.f, FColor::Red, TEXT("hit"));
		force.worldPos = hit.Location;

		force.force = transform.TransformVector(FVector(0.f,gripForce,0.f))+hit.Normal*netSpringForce;

	}
	return force;

}