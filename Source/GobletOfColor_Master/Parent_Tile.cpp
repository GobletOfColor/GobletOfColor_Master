// Fill out your copyright notice in the Description page of Project Settings.

#include "GobletOfColor_Master.h"
#include "Parent_Tile.h"


// Sets default values
AParent_Tile::AParent_Tile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParent_Tile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParent_Tile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

