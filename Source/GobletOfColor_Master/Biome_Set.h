// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Biome_Set.generated.h"

UCLASS()
class GOBLETOFCOLOR_MASTER_API ABiome_Set : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABiome_Set();

	//ABiome_Set(const ABiome_Set &copyStuff);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = "Tiles", meta = (BlueprintProtected))
		TArray<TSubclassOf<class ATileParent>> Tile_Set;
	
	UPROPERTY(EditAnywhere, Category = "Tiles", meta = (BlueprintProtected))
		FString biomeName;

	UFUNCTION(BlueprintCallable, Category = "Tiles", meta = (BlueprintProtected))
		TSubclassOf<class ATileParent> GetTile(int index, bool rand);

	FString GetBiomeName() const;

	//ABiome_Set& operator=(const ABiome_Set &copyStuff);
};
