// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleEngineInterface.generated.h"


USTRUCT(BlueprintType)
struct FSimpleEngineState {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float rpm = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float fuelConsumption = 0.f;
	
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USimpleEngineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLEFLIGHT_API ISimpleEngineInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FSimpleEngineState GetSimpleEngineState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void TakeEngineDamage(float damage, float health);
};
