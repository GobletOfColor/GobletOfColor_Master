// Fill out your copyright notice in the Description page of Project Settings.

#include "GobletOfColor_Master.h"
#include "Biome_Parent.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Engine.h"
#include "TileParent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Biome_Set.h"

// Sets default values
ABiome_Parent::ABiome_Parent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	repeat = 0;
	p = new int[512];
	for (int x = 0; x < 512; ++x) {
		p[x] = permutation[x % 256]; //fils a set of 512 random permutations (can add a permutation generator later)
	}			//useful for getermaning the gradient vector later

	Persistence = 1.0f;
	zScale = 100.000f;
	Octaves = 1;
	delimeter = 2000;
	sizeX = 10;
	sizeY = 10;
	zDiff = 1.0f;
	SpawnLocation.X = 1.0f;
	SpawnLocation.Y = 1.0f;
	SpawnLocation.Z = 1.0f;

	srand(time(NULL));
	//ABiome_Set* add = new ABiome_Set();
	//Biome_Set.Add(add);
}

// Called when the game starts or when spawned
void ABiome_Parent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABiome_Parent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

double ABiome_Parent::Fade(double t) {
	//if(!t) 	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("T is 0: ") + FString::SanitizeFloat(t));
	double result = (t * t * t * (t * (t * 6 - 15) + 10)); //6t^5 - 15t^4 + 10t^3 this is The Perlin Noise fade function
	//if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing Fade: ") + FString::SanitizeFloat(t));
	return result;
}

int ABiome_Parent::inc(int num) {
	num++;
	if (repeat > 0) num = (int)fmod(num,repeat);
	return num;
}

/*
There where two versions of this. Given that I know that binary conditionals work faster than logical conditions, especially with a switch statement since
it is constant runtime in C++ this is faster than the other algorithm:
	Take the hashed value and take the first 4 bits of it (15 == 0b1111)
	If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.
	
	If the first and second significant bits are 0
		v = y
	else if the first and second significant bits are 1
		v = x
	If the first and second significant bits are not equal (0/1, 1/0) 
		v = z
	//end if
	
	Use the last 2 bits to decide if u and v are positive or negative
		return the addition of the two.
*/
double ABiome_Parent::Gradient(int hash, double x, double y, double z) {
	switch (hash & 0xF) {
	case 0x0: return x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0; //will never happen
	}
}

// Using a different lerp to handle specification and to avoid abstraction of unknown inefficiencies
double ABiome_Parent::Lerp(double a, double b, double x) {
	double result = (a + x * (b - a));
	return result;
}

double ABiome_Parent::PerlinNoise(double x, double y, double z) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Passed into PerlinNoise: ") + FString::SanitizeFloat(x) + FString::FString(" Y: ") +
			FString::SanitizeFloat(y) + FString::FString(" Z: ") + FString::SanitizeFloat(z));
	}

	int xi, yi, zi;
	double xf, yf, zf, u, v, w; 
	if (repeat > 0) {
		x = fmod(x,repeat);
		y = fmod(y,repeat);
		z = fmod(z,repeat);
		if (GEngine) 
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Repeat > 0") );
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing x: ") + FString::SanitizeFloat(x));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing y: ") + FString::SanitizeFloat(x));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing z: ") + FString::SanitizeFloat(z));
	};
	xi = (int)floor(x) & 255; //calculate the unit cube where the point asked will be located in
	yi = (int)floor(y) & 255; //gives us the left bound and rightbound = leftbound + 1
	zi = (int)floor(z) & 255;
	xf = x - floor(x); //calculate location from 0.0 to 1.0
	yf = y - floor(y);
	zf = z - floor(z);

	u = Fade(xf); //get the fades from the Perlin Noise Function
	v = Fade(yf);
	w = Fade(zf);
// Issue is above here THANK GOD

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[p[p[    xi ] +     yi ] +     zi ];
	aba = p[p[p[    xi ] + inc(yi)] +     zi ];
	aab = p[p[p[    xi ] +     yi ] + inc(zi)];
	abb = p[p[p[    xi ] + inc(yi)] + inc(zi)];
	baa = p[p[p[inc(xi)] +     yi ] +     zi ];
	bba = p[p[p[inc(xi)] + inc(yi)] +     zi ];
	bab = p[p[p[inc(xi)] +     yi ] + inc(zi)];
	bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];

	double x1, x2, y1, y2;
	x1 = Lerp(Gradient(aaa, xf,     yf, zf),
			  Gradient(baa, xf - 1, yf, zf),
		      u);
	x2 = Lerp(Gradient(aba, xf,     yf - 1, zf),
		      Gradient(bba, xf - 1, yf - 1, zf),
		      u);
	y1 = Lerp(x1, x2, v);
	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing lerp x1: ") + FString::SanitizeFloat(x1 * 1000.0f));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing lerp x2: ") + FString::SanitizeFloat(x2 * 1000.0f));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing lerp y1: ") + FString::SanitizeFloat(y1 * 1000.0f));
	};
	x1 = Lerp(Gradient(aab, xf,     yf, zf - 1),
		      Gradient(bab, xf - 1, yf, zf - 1),
		      u);
	x2 = Lerp(Gradient(abb, xf,     yf - 1, zf - 1),
		      Gradient(bbb, xf - 1, yf - 1, zf - 1),
		      u);
	y2 = Lerp(x1, x2, v);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Noise_Value: ") + FString::SanitizeFloat((Lerp(y1, y2, w) + 1)));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("\n ====================="));
	}
	return (Lerp(y1, y2, w) + 1) / 2;
}

double ABiome_Parent::OctavePerlin(double x, double y, double z, int octaves, double persistence) {
	double total = 0;
	double temp;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
//	if (GEngine) {
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Octaves: ") + FString::SanitizeFloat(octaves));
//	}
	for (int i = 0; i<octaves; i++) {
		temp = PerlinNoise(x * frequency, y * frequency, z * frequency);
//		if (GEngine) {
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Noise_Value: ") + FString::SanitizeFloat(temp));
//		}
		total += temp * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}
//	if (GEngine) {
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Noise_Value: ") + FString::SanitizeFloat(total / maxValue) + FString::FString(" total: ") + FString::SanitizeFloat(total)
//			+ FString::FString(" MaxValue: ") + FString::SanitizeFloat(maxValue));
//	}
	return (total / maxValue);
}

void ABiome_Parent::BiomeGen(TArray<ABiome_Set*> biomes) {
	 //get the last spawn coords we should only be using z for the gradient x and y are determined by a delimeter
	float noise= 0.000f;
	BIOME_SELECT selector = FOREST;
	ABiome_Set* temp = biomes[selector];
	TSubclassOf<ATileParent> currentSpawnTile;
	for (int xPos = 0; xPos <= sizeX; ++xPos) {
		for (int yPos = 0; yPos <= sizeY; ++yPos) {
			noise = OctavePerlin(SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z , Octaves, Persistence);
			SpawnLocation.X = xPos * delimeter;
			SpawnLocation.Y = yPos * delimeter;
			SpawnLocation.Z = noise * zScale;
			temp = biomes[selector];
				if (temp) currentSpawnTile = temp->GetTile(0, true);
				else {
					currentSpawnTile = DefaultSpawn;
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Failed to grab biome"));
					}
				}
				if (!(SpawnTile(currentSpawnTile, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f)))) {
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::FString("Failed Spawn in BiomeGen"));
					}
				}
			if (xPos <= 7 && yPos <= 7) {
				selector = FOREST;
			}
			else if (xPos > 7 && yPos <= 7) {
				selector = AZTEC;
			}
			else if (xPos < 7 && yPos > 7) {
				selector = RIVER;
			}
			else
				selector = CAVE;
		}
	}
	if (GEngine) {
		//double test1 = 0.002f;
		//double test2 = 0.003f;
		//double result = test1*test2;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("Testing Float multiplication") + FString::SanitizeFloat(result * 100000));
	}
}

//Helper for BiomeGen
ATileParent* ABiome_Parent::GetTile(TArray<ATileParent *> Tiles) {
	int index = rand() % Tiles.Num() + 0;
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FString("index: ") + FString::SanitizeFloat(index));
	}
	return Tiles[index];
}

FVector ABiome_Parent::GetSpawnPosition() {
	return TileReferences[((int)TileReferences.Num()) / 2]->GetLocation();
}

double ABiome_Parent::Multiply_Double(double A, int by) {
	double rVal = 0.0f;
	for (int i = 0; i < by; ++i) {
		rVal += A;
	}
	return rVal;
}

bool ABiome_Parent::SwapTile(TSubclassOf<ATileParent> swapTo, int x, int y) {
	int width, index;
	bool rVal = false;
	width = TileReferences.Num() / sizeX;
	index = x + width*y;
	ATileParent* swapFrom = TileReferences[index];
	if (swapFrom) {
		FVector SpawnVec = swapFrom->GetLocation();
		swapFrom->DestroyThis();
		if (SpawnTile(swapTo, SpawnVec, FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f))) rVal = true;
	}
	return rVal;
}

bool ABiome_Parent::SpawnTile(TSubclassOf<ATileParent> SpawnMe, FVector location, FRotator rotation, FVector scale) {
	bool rVal = false;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	UWorld* const World = GetWorld();
	if (World) {
		if (SpawnMe) {
			ATileParent* currTile = NULL;
			currTile = World->SpawnActor<ATileParent>(SpawnMe, FTransform(rotation, location, scale), SpawnParams);
			if (currTile) {
				TileReferences.Add(currTile);
				rVal = true;
			}
		}
	}
	return rVal;
}

