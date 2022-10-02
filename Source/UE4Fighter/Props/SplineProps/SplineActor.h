// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/DataTable.h"

#include "SplineActor.generated.h"

USTRUCT(BlueprintType)
struct FSplineDetails : public FTableRowBase
{
	GENERATED_BODY()

	/** Mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
		UStaticMesh* Mesh;

	/** Mesh orientation  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
		TEnumAsByte<ESplineMeshAxis::Type> SplineMeshAxis;

	/** Material */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
		UMaterialInstance* MaterialInstance;

	FSplineDetails() :SplineMeshAxis(ESplineMeshAxis::Type::Z) {}

};

UCLASS()
class UE4FIGHTER_API ASplineActor : public AActor
{
	GENERATED_BODY()

	/** Spline sequence */
	UPROPERTY(VisibleAnywhere, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		USplineComponent* SplineComponent;

	/** Default static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* DefaultMesh;

	/** Default mesh orientation  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<ESplineMeshAxis::Type> SplineMeshAxis;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* DefaultMaterialInstance;

	/** In case we need override some of the spline meshes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		TMap<int, FSplineDetails> SplineOverrideMap;

public:
	// Sets default values for this character's properties
	ASplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

};
