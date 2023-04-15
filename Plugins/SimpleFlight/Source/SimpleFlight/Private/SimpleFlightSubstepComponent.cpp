// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFlightSubstepComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
USimpleFlightSubstepComponent::USimpleFlightSubstepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//CalculateCustomPhysics.BindUObject(this, &USimpleFlightSubstepComponent::SubstepTick);
	enableSubstepCalculations = true;
	// ...
}


TArray<FSFForce> USimpleFlightSubstepComponent::GatherForces(FTransform transform) {

	TArray<FSFForce> forceArray;
	for (int i = 0; i < simpleFlightComponents.Num(); i++) {

		//forceArray.Add(ISimpleFlightInterface::Execute_ReportSimpleForce(simpleFlightComponents[i],transform, true));
		forceArray.Add(ISimpleFlightInterface::Execute_ReportSimpleForce(simpleFlightComponents[i],transform, true));
	}
	return forceArray;
}

// Called when the game starts
void USimpleFlightSubstepComponent::BeginPlay()
{
	Super::BeginPlay();
	CalculateCustomPhysics.BindUObject(this, &USimpleFlightSubstepComponent::SubstepTick);
	simpleFlightComponents = GetOwner()->GetComponentsByInterface(USimpleFlightInterface::StaticClass());
	// ...
	PrimitiveParent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	
}


// Called every frame
void USimpleFlightSubstepComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (enableSubstepCalculations) {
		if (PrimitiveParent) {
			if (PrimitiveParent->IsSimulatingPhysics()) {
				if (FBodyInstance* bodyInstance = PrimitiveParent->GetBodyInstance()) {
					bodyInstance->AddCustomPhysics(CalculateCustomPhysics);
				}
			}
		}
	}
	else {
		if (PrimitiveParent) {
			TArray<FSFForce> forces = GatherForces(PrimitiveParent->GetComponentTransform());
			for (int i = 0; i < forces.Num(); i++) {
				PrimitiveParent->AddForceAtLocation(forces[i].force, forces[i].worldPos);
				PrimitiveParent->AddTorqueInRadians(forces[i].torque);
			}
		}
	}
		//UE_LOG(LogTemp, Log, TEXT("test %f"), 10.f);
	//	 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("test = %f"), 100.f));
	// ...
	
}


void USimpleFlightSubstepComponent::SubstepTick(float DeltaTime, FBodyInstance* BodyInstance) {
	TArray<FSFForce> forces = GatherForces(BodyInstance->GetUnrealWorldTransform());
	
	for (int i = 0; i < forces.Num(); i++) {
	//	BodyInstance->AddForceAtPosition(forces[i].force,forces[i].worldPos,false);
	BodyInstance->AddForceAtPosition(forces[i].force*10.f,forces[i].worldPos, false);
	BodyInstance->AddTorqueInRadians(forces[i].torque, false);
	
		// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("test = %f"), forces[i].force.Z));
	}
	//	UE_LOG(LogTemp, Log, TEXT("Forces: %f"), forces[0].force.Z);
	
}
