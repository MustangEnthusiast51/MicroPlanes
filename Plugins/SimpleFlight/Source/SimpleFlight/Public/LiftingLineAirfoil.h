// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleFlightInterface.h"
#include "Components/SceneComponent.h"
#include "LiftingLineAirfoil.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class SIMPLEFLIGHT_API ULiftingLineAirfoil : public USceneComponent, public ISimpleFlightInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULiftingLineAirfoil();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float wingSpan;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float rootChord;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float tipChord;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float temporalBlend;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float stallAngleDeg;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float zeroLiftDegHigh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float zeroLiftDegLow;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float maxFlapAngle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector ControlFactors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float tipChordOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float liftFactor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float dragFactor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float forcesFactor;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector localForcePos;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float wingArea;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float alphaDeg;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FVector totalForces;
	bool implementsInterface;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame

	UFUNCTION(BlueprintCallable)
		void DrawDebug();

	UFUNCTION(BlueprintCallable)
		FVector NetForces();

	UFUNCTION(BlueprintCallable)
		FVector Coefficients(float alpha);

	UFUNCTION(BlueprintCallable)
		float LiftEquation(float airDensity, float cl, float velocity);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void DrawSFDebug_Implementation();
		
	virtual FSFForce ReportSimpleForce_Implementation();
};
