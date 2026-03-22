#include "Items/AWYAWorldItem.h"
#include "Location/WYAGeoMath.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"

AWYAWorldItem::AWYAWorldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
	RootSphere->SetSphereRadius(50.f);
	RootSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	SetRootComponent(RootSphere);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootSphere);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Default to engine sphere mesh — Blueprint override with proper model
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
		Mesh->SetRelativeScale3D(FVector(0.5f));
	}

	NamePlate = CreateDefaultSubobject<UWidgetComponent>(TEXT("NamePlate"));
	NamePlate->SetupAttachment(RootSphere);
	NamePlate->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	NamePlate->SetWidgetSpace(EWidgetSpace::World);
	NamePlate->SetDrawSize(FVector2D(200.f, 60.f));
}

void AWYAWorldItem::BeginPlay()
{
	Super::BeginPlay();
}

void AWYAWorldItem::SetItemData(const FWYAItemData& Data, const FWYAGeoCoord& WorldOrigin)
{
	ItemData = Data;

	// Reposition to GPS-mapped world location
	FWYAGeoCoord Coord;
	Coord.Latitude  = Data.Lat;
	Coord.Longitude = Data.Lon;
	Coord.Altitude  = Data.Alt;

	const FVector WorldPos = FWYAGeoMath::GeoToWorld(Coord, WorldOrigin);

	// Trace to terrain surface — GeoToWorld Z is relative to the georeference origin
	// but Cesium tiles sit at a different absolute Z depending on tile geometry.
	FVector FinalPos = WorldPos + FVector(0.f, 0.f, 150.f); // fallback: 1.5m above computed pos
	if (UWorld* World = GetWorld())
	{
		const FVector TraceStart = FVector(WorldPos.X, WorldPos.Y, WorldPos.Z + 500000.f);
		const FVector TraceEnd   = FVector(WorldPos.X, WorldPos.Y, WorldPos.Z - 100000.f);
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		// Ignore all pawns — player is parked at Z=500000 while Cesium loads
		for (TActorIterator<APawn> It(World); It; ++It)
			Params.AddIgnoredActor(*It);
		if (World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params))
		{
			FinalPos = Hit.ImpactPoint + FVector(0.f, 0.f, 100.f); // hover 1m above ground
			UE_LOG(LogTemp, Log, TEXT("WYAItem: placed %s at Z=%.0f (terrain hit)"), *Data.Id, FinalPos.Z);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WYAItem: terrain trace missed for %s — tiles not loaded yet, placed at Z=%.0f"), *Data.Id, FinalPos.Z);
		}
	}
	SetActorLocation(FinalPos);

	// Tint claimed items grey so they're visually distinct
	if (!Data.IsAvailable())
	{
		Mesh->SetVectorParameterValueOnMaterials(TEXT("BaseColor"), FVector(0.3f, 0.3f, 0.3f));
	}

	OnItemDataUpdated();
}

float AWYAWorldItem::DistanceTo(const FVector& WorldPos) const
{
	return FVector::Dist(GetActorLocation(), WorldPos);
}
