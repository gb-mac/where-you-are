#include "Radio/WYARadioSubsystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

void UWYARadioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Short random delay before the first broadcast so radio doesn't fire
    // immediately on game start.
    const float InitialDelay = FMath::FRandRange(10.f, 30.f);

    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().SetTimer(
            BroadcastHandle,
            this,
            &UWYARadioSubsystem::ScheduleNextBroadcast,
            InitialDelay,
            /*bLooping=*/false);
    }

    UE_LOG(LogTemp, Log, TEXT("WYARadioSubsystem: initialized — first broadcast in %.1fs"), InitialDelay);
}

void UWYARadioSubsystem::Deinitialize()
{
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().ClearTimer(BroadcastHandle);
    }

    Super::Deinitialize();
}

// ---------------------------------------------------------------------------
// Blueprint API
// ---------------------------------------------------------------------------

bool UWYARadioSubsystem::IsLive() const
{
    return CurrentSource != EWYABroadcastSource::None
        && CurrentSource != EWYABroadcastSource::Static;
}

FString UWYARadioSubsystem::GetCurrentClipTag() const
{
    return CurrentClipTag;
}

void UWYARadioSubsystem::RegisterClip(EWYABroadcastSource Source, const FString& ClipTag)
{
    if (ClipTag.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("WYARadioSubsystem: RegisterClip called with empty tag — ignored"));
        return;
    }

    RegisteredClips.FindOrAdd(Source).Add(ClipTag);

    // Ensure cursor entry exists
    if (!ClipCursors.Contains(Source))
    {
        ClipCursors.Add(Source, 0);
    }

    UE_LOG(LogTemp, Verbose, TEXT("WYARadioSubsystem: registered clip '%s' for source %d"),
        *ClipTag, static_cast<int32>(Source));
}

void UWYARadioSubsystem::ForceSource(EWYABroadcastSource Source)
{
    // Cancel any pending timer
    if (UWorld* World = GetGameInstance()->GetWorld())
    {
        World->GetTimerManager().ClearTimer(BroadcastHandle);
    }

    UE_LOG(LogTemp, Log, TEXT("WYARadioSubsystem: ForceSource → %d"), static_cast<int32>(Source));
    BeginBroadcast(Source);
}

// ---------------------------------------------------------------------------
// Internal scheduler
// ---------------------------------------------------------------------------

EWYABroadcastSource UWYARadioSubsystem::PickNextSource() const
{
    // Weighted table:
    //   Static   35
    //   Fragment 20
    //   Aldric   15
    //   Maren    15
    //   Covenant 15
    //   Total   100

    const float Roll = FMath::FRandRange(0.f, 100.f);

    if (Roll < 35.f)  return EWYABroadcastSource::Static;
    if (Roll < 55.f)  return EWYABroadcastSource::Fragment;
    if (Roll < 70.f)  return EWYABroadcastSource::Aldric;
    if (Roll < 85.f)  return EWYABroadcastSource::Maren;
    return                   EWYABroadcastSource::Covenant;
}

void UWYARadioSubsystem::ScheduleNextBroadcast()
{
    const float Delay = FMath::FRandRange(MinSilenceDuration, MaxSilenceDuration);

    UWorld* World = GetGameInstance()->GetWorld();
    if (!World) return;

    World->GetTimerManager().SetTimer(
        BroadcastHandle,
        FTimerDelegate::CreateUObject(this, &UWYARadioSubsystem::BeginBroadcast, PickNextSource()),
        Delay,
        /*bLooping=*/false);

    UE_LOG(LogTemp, Verbose, TEXT("WYARadioSubsystem: next broadcast in %.1fs"), Delay);
}

void UWYARadioSubsystem::BeginBroadcast(EWYABroadcastSource Source)
{
    CurrentSource = Source;

    // Advance clip cursor for this source and cache the tag
    CurrentClipTag.Empty();
    if (TArray<FString>* Clips = RegisteredClips.Find(Source))
    {
        if (Clips->Num() > 0)
        {
            int32& Cursor = ClipCursors.FindOrAdd(Source);
            CurrentClipTag = (*Clips)[Cursor];
            Cursor = (Cursor + 1) % Clips->Num();
        }
    }

    OnBroadcastChanged.Broadcast(CurrentSource);

    UE_LOG(LogTemp, Log, TEXT("WYARadioSubsystem: broadcast started — source=%d clip='%s'"),
        static_cast<int32>(Source), *CurrentClipTag);

    // Schedule end of broadcast
    const float Duration = FMath::FRandRange(MinBroadcastDuration, MaxBroadcastDuration);

    UWorld* World = GetGameInstance()->GetWorld();
    if (!World) return;

    World->GetTimerManager().SetTimer(
        BroadcastHandle,
        this,
        &UWYARadioSubsystem::EndBroadcast,
        Duration,
        /*bLooping=*/false);
}

void UWYARadioSubsystem::EndBroadcast()
{
    UE_LOG(LogTemp, Verbose, TEXT("WYARadioSubsystem: broadcast ended — source=%d"),
        static_cast<int32>(CurrentSource));

    CurrentSource = EWYABroadcastSource::None;
    CurrentClipTag.Empty();

    OnBroadcastChanged.Broadcast(CurrentSource);

    ScheduleNextBroadcast();
}
