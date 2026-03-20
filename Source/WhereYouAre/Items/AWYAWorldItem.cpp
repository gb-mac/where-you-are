#include "Items/AWYAWorldItem.h"
#include "Location/WYAGeoMath.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

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
	SetActorLocation(WorldPos);

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
