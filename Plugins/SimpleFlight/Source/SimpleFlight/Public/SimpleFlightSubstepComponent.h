// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "SimpleFlightInterface.h"
#include "SimpleFlightSubstepComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFLIGHT_API USimpleFlightSubstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleFlightSubstepComponent();
	FCalculateCustomPhysics CalculateCustomPhysics;
protected:
	// Called when the game starts

	TArray<UActorComponent*> simpleFlightComponents;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool enableSubstepCalculations;

	UPrimitiveComponent* PrimitiveParent;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FSFForce> GatherForces(FTransform transform);

	virtual void SubstepTick(float DeltaTime, FBodyInstance* BodyInstance);
};
