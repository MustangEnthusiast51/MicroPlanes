// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftingLineAirfoil.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values for this component's properties
ULiftingLineAirfoil::ULiftingLineAirfoil()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	wingSpan = 200.f;
	rootChord = 100.f;
	tipChord = 100.f;
	tipChordOffset = 0.f;
	wingArea = 20000.f;
	ControlFactors = FVector();
	stallAngleDeg = 11.f;
	zeroLiftDegHigh = 60.f;
	maxFlapAngle = 30.f;
	temporalBlend = 0.5f;
	liftFactor = 1.f;
	dragFactor = 1.f;
	zeroLiftDegLow = -1.f;
	// ...
}

//alpha in radians
FVector ULiftingLineAirfoil::Coefficients(float alpha) {
	wingArea = FMath::Abs(wingSpan * (rootChord + tipChord) * 0.5f);
	float ar = wingSpan * wingSpan / wingArea;
	const float deg2rad = PI / 180.f;
	float alphaOffset = -zeroLiftDegLow*deg2rad;
	
	if (UKismetSystemLibrary::DoesImplementInterface(this->GetOwner(), USimpleFlightInterface::StaticClass())) {
		
		alphaOffset += ControlFactors.X *ISimpleFlightInterface::Execute_GetInputChannel(this->GetOwner(), 0)*maxFlapAngle*deg2rad;
		alphaOffset += ControlFactors.Y *ISimpleFlightInterface::Execute_GetInputChannel(this->GetOwner(), 1)*maxFlapAngle*deg2rad;
		alphaOffset += ControlFactors.Z *ISimpleFlightInterface::Execute_GetInputChannel(this->GetOwner(), 2)*maxFlapAngle*deg2rad;
	}
	
	float correctedAlpha = alpha +alphaOffset;
	
	float cl = correctedAlpha * PI * 2.f * (ar / (ar + 2.f));
	float cd = cl * cl / (PI * ar * 0.6f) + 0.025f;
	
	if (correctedAlpha>stallAngleDeg*deg2rad) {
		float y = stallAngleDeg * deg2rad * PI * 2.f * (ar / (ar + 2.f));
		float m = -y / (stallAngleDeg * deg2rad - zeroLiftDegHigh * deg2rad);
		cl = m * (correctedAlpha - stallAngleDeg * deg2rad) + y;

	}
	return FVector(cl,cd,0.f);
}


float ULiftingLineAirfoil::LiftEquation(float airDensity, float cl, float velocity) {


	return cl * velocity * velocity * airDensity * wingArea * 0.5f;

}

// Called when the game starts
void ULiftingLineAirfoil::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ULiftingLineAirfoil::DrawDebug() {
	FTransform transform = this->GetComponentTransform();
	//leading edge
	DrawDebugLine(GetWorld(), transform.TransformPosition(FVector(rootChord, 0.f, 0.f)), transform.TransformPosition(FVector(tipChord+tipChordOffset, wingSpan, 0.f)), FColor::Emerald, true, 100.f, 0, 2.f);
	// trailing edge
	DrawDebugLine(GetWorld(), transform.TransformPosition(FVector(0.f, 0.f, 0.f)), transform.TransformPosition(FVector(tipChordOffset, wingSpan, 0.f)), FColor::Emerald, true, 100.f, 0, 2.f);
	//root chord
	DrawDebugLine(GetWorld(), transform.TransformPosition(FVector(rootChord, 0.f, 0.f)), transform.TransformPosition(FVector(0.f, 0.f, 0.f)), FColor::Emerald, true, 100.f, 0, 2.f);
	//tip chord
	DrawDebugLine(GetWorld(), transform.TransformPosition(FVector(tipChord+tipChordOffset, wingSpan, 0.f)), transform.TransformPosition(FVector(tipChordOffset, wingSpan, 0.f)), FColor::Emerald, true, 100.f, 0, 2.f);
	
	localForcePos = FVector((rootChord + tipChord+tipChordOffset)/4.f,wingSpan/2.f,0.f);
	DrawDebugSphere(GetWorld(), transform.TransformPosition(localForcePos), 25.f, 12, FColor::Red, true, 1000.f, 0, 2.f);

}

FVector ULiftingLineAirfoil::NetForces() {
	FVector worldVel = FVector();
	FTransform transform = this->GetComponentTransform();
	FEnvironmentData envData = FEnvironmentData();
	if (UKismetSystemLibrary::DoesImplementInterface(this->GetOwner(), USimpleFlightInterface::StaticClass())) {
		worldVel = ISimpleFlightInterface::Execute_VelocityAtPoint(this->GetOwner(),transform.TransformPosition(localForcePos));
		envData = ISimpleFlightInterface::Execute_GetEnvironmentData(this->GetOwner());
		
	}
	FVector localVel = transform.InverseTransformVector(worldVel);
	float alpha = FMath::Atan2(-localVel.Z, localVel.X);
	alphaDeg = alpha * 180.f / PI;
	FVector coefficients = Coefficients(alpha);
	FVector worldDir = worldVel;
	worldDir.Normalize();
	float lift = LiftEquation(envData.airDensity,coefficients.X,worldVel.Length())*liftFactor;
	float drag = LiftEquation(envData.airDensity,coefficients.Y,worldVel.Length())*dragFactor;
	FVector liftDir = FVector::CrossProduct(worldDir, this->GetRightVector());
	//DrawDebugLine(GetWorld(), transform.GetLocation(), transform.GetLocation() + liftDir*lift, FColor::Emerald, true, .01f, 0, 2.f);
//	DrawDebugLine(GetWorld(), transform.GetLocation(), transform.GetLocation() -drag*worldDir, FColor::Emerald, true, .01f, 0, 2.f);

	totalForces = FMath::Lerp(liftDir*lift+drag*-worldDir,totalForces,temporalBlend);
	return totalForces;
}


// Called every frame
void ULiftingLineAirfoil::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FSFForce ULiftingLineAirfoil::ReportSimpleForce_Implementation() {
	FSFForce force = FSFForce();
	force.torque = FVector();
	force.force = NetForces();
	force.worldPos = this->GetComponentTransform().TransformPosition(localForcePos);
	return force;
}


void ULiftingLineAirfoil::DrawSFDebug_Implementation() {
	DrawDebug();
}


