// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "CoreMinimal.h"
#include "SimpleFlightInterface.generated.h"

USTRUCT(BlueprintType)
struct FEnvironmentData {
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float airDensity = 1.225f;

	
};


USTRUCT(BlueprintType)
struct FSFForce {
	
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector force;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector torque;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector worldPos;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USimpleFlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEFLIGHT_API ISimpleFlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DrawSFDebug();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FSFForce ReportSimpleForce(FTransform overrideTransform, bool substep);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FVector VelocityAtPoint(FVector worldPos);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		float GetInputChannel(int channel);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetInputChannel(int channel, float value);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetCameraDirectionValues(FVector directionalValues);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FEnvironmentData GetEnvironmentData();

};
