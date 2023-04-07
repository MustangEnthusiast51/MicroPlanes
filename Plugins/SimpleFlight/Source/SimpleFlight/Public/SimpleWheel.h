// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SimpleFlightInterface.h"
#include "SimpleWheel.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEFLIGHT_API USimpleWheel : public USceneComponent, public ISimpleFlightInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleWheel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float springLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float restLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float wheelRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float springForce;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float dampingForce;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float sideGrip;




	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<ECollisionChannel> channel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float physicsTimestep;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float currentLength;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float lastLength;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float zeroToOneLength;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float rpm;

	bool implementsInterface;

	FTransform relTransform;
	FTransform parentTransform;
	UPrimitiveComponent* rootComp;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FSFForce CalculateForces(FTransform transform);
	virtual void DrawSFDebug_Implementation();
	virtual FSFForce ReportSimpleForce_Implementation(FTransform overrideTransform, bool substep);
};
