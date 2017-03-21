// Fill out your copyright notice in the Description page of Project Settings.

#include "GobletOfColor_Master.h"
#include "Biome_Set.h"
#include "TileParent.h"
#include <stdlib.h>
#include <time.h>

// Sets default values
ABiome_Set::ABiome_Set()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	biomeName = "";
	srand(time(NULL));
}

ABiome_Set::ABiome_Set(const ABiome_Set &copyStuff) {
	Tile_Set = copyStuff.Tile_Set;
	biomeName = copyStuff.GetBiomeName();
}

// Called when the game starts or when spawned
void ABiome_Set::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABiome_Set::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

TSubclassOf<ATileParent>  ABiome_Set::GetTile(int index, bool isRand) {
	TSubclassOf<ATileParent> rVal = NULL;
	if (!isRand) {
		if (Tile_Set.Num() < index) {
			rVal = Tile_Set[index];
		}
	}
	else {
		int randVal = rand() % Tile_Set.Num();
		rVal = Tile_Set[randVal];
	}
		return rVal;
}

FString ABiome_Set::GetBiomeName() const{
	return biomeName;
}

/*ABiome_Set& ABiome_Set::operator=(const ABiome_Set& copyStuff) {
	Tile_Set = copyStuff.Tile_Set;
	biomeName = copyStuff.GetBiomeName();
	return *this;
}*/