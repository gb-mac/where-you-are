#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WYARadioSubsystem.generated.h"

/**
 * Broadcast sources the radio cycles between.
 *
 * The radio is uncontrollable infrastructure — it comes on when it comes on.
 * No player on/off switch, no channel control. Occasionally a liability during missions.
 */
UENUM(BlueprintType)
enum class EWYABroadcastSource : uint8
{
    None,       // radio is off / between broadcasts
    Static,     // most common state — broken signal
    Fragment,   // partial pre-Cascade transmissions that never stopped
    Aldric,     // Reckoner broadcast — all-faction news, commentary
    Maren,      // mutual aid network — recipes, community updates, seasonal
    Covenant,   // Shepherd-run outreach — calm, certain, propagandistic
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBroadcastChanged, EWYABroadcastSource, NewSource);

/**
 * Manages the in-world radio: randomly cycles between broadcast sources and static.
 *
 * Blueprint registers audio clip tags via RegisterClip() at init. The subsystem
 * schedules source switches autonomously; there is no player-facing control.
 *
 * ForceSource() is provided for scripted mission moments only.
 */
UCLASS()
class WHEREYOUARE_API UWYARadioSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // ── Delegate ─────────────────────────────────────────────────────────────

    /** Fired on every source switch (including switches to None/Static). */
    UPROPERTY(BlueprintAssignable, Category = "WYA|Radio")
    FOnBroadcastChanged OnBroadcastChanged;

    // ── Tuning parameters ────────────────────────────────────────────────────

    /** Minimum seconds a source plays before switching. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Radio")
    float MinBroadcastDuration = 60.f;

    /** Maximum seconds a source plays before switching (4 min). */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Radio")
    float MaxBroadcastDuration = 240.f;

    /** Minimum seconds of silence/None between broadcasts. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Radio")
    float MinSilenceDuration = 30.f;

    /** Maximum seconds of silence/None between broadcasts. */
    UPROPERTY(EditDefaultsOnly, Category = "WYA|Radio")
    float MaxSilenceDuration = 120.f;

    // ── Blueprint API ────────────────────────────────────────────────────────

    /** The currently active broadcast source. */
    UFUNCTION(BlueprintPure, Category = "WYA|Radio")
    EWYABroadcastSource GetCurrentSource() const { return CurrentSource; }

    /** True if the current source is a live broadcast (not None or Static). */
    UFUNCTION(BlueprintPure, Category = "WYA|Radio")
    bool IsLive() const;

    /**
     * Returns a tag Blueprint uses to look up the audio clip for the current source,
     * e.g. "radio_aldric_01". Cycles through registered clips per source.
     * Returns empty string if no clips are registered for the current source.
     */
    UFUNCTION(BlueprintPure, Category = "WYA|Radio")
    FString GetCurrentClipTag() const;

    /**
     * Register an audio clip tag for a broadcast source.
     * Call from Blueprint at init. Clips are cycled through in registration order.
     *
     * @param Source   The broadcast source this clip belongs to
     * @param ClipTag  Tag used by the audio layer to identify the sound asset
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Radio")
    void RegisterClip(EWYABroadcastSource Source, const FString& ClipTag);

    /**
     * Override the scheduler for scripted moments (mission sequences etc.).
     * Fires OnBroadcastChanged immediately. Auto-returns to normal scheduling
     * after the current broadcast duration expires.
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Radio")
    void ForceSource(EWYABroadcastSource Source);

private:
    EWYABroadcastSource CurrentSource = EWYABroadcastSource::None;

    FTimerHandle BroadcastHandle;

    /** Audio clip tags per source. */
    TMap<EWYABroadcastSource, TArray<FString>> RegisteredClips;

    /** Current playback cursor per source — advances each BeginBroadcast. */
    TMap<EWYABroadcastSource, int32> ClipCursors;

    /** Tag of the clip currently playing (set in BeginBroadcast). */
    FString CurrentClipTag;

    /**
     * Weighted random pick:
     *   Static 35%, Fragment 20%, Aldric 15%, Maren 15%, Covenant 15%
     */
    EWYABroadcastSource PickNextSource() const;

    /**
     * Schedule BeginBroadcast after a random silence gap
     * [MinSilenceDuration, MaxSilenceDuration].
     */
    void ScheduleNextBroadcast();

    /**
     * Set CurrentSource, advance clip cursor, fire OnBroadcastChanged,
     * then schedule EndBroadcast after a random duration.
     */
    void BeginBroadcast(EWYABroadcastSource Source);

    /** Set source to None, fire OnBroadcastChanged, schedule next broadcast. */
    void EndBroadcast();
};
