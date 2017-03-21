// Fill out your copyright notice in the Description page of Project Settings.

#include "GobletOfColor_Master.h"
#include "TileParent.h"


// Sets default values
ATileParent::ATileParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATileParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATileParent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FVector ATileParent::GetLocation() {
	return GetActorLocation();
}

void ATileParent::DestroyThis() {
	Destroy();
}
