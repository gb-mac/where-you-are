#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WYASaveSubsystem.generated.h"

class UWYASaveGame;

/** Fired after LoadGame() completes (even when no save data exists). */
DECLARE_MULTICAST_DELEGATE(FOnSaveLoaded);

/**
 * Manages saving and loading game state.
 * Automatically loads on Initialize() so state is ready before SpawnPlayer.
 *
 * Gathers fix-him repair state from UWYAFixHimQuestSubsystem and inventory
 * from the local player's pawn UWYAInventoryComponent.
 */
UCLASS()
class WHEREYOUARE_API UWYASaveSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** Async save — gathers current state and writes to disk. */
    UFUNCTION(BlueprintCallable, Category = "WYA|Save")
    void SaveGame();

    /**
     * Synchronous load at startup.
     * Restores fix-him quest state and fires OnSaveLoaded.
     * Inventory is restored onto the pawn later in SpawnPlayer (see WYAGameMode).
     */
    UFUNCTION(BlueprintCallable, Category = "WYA|Save")
    void LoadGame();

    /** True if a save file exists on disk. */
    UFUNCTION(BlueprintPure, Category = "WYA|Save")
    bool HasSaveData() const;

    /** Get the loaded save object. May be null if no save exists. */
    UWYASaveGame* GetSaveGame() const { return CachedSave; }

    /** Fired after LoadGame() completes (even when no save data was found). */
    FOnSaveLoaded OnSaveLoaded;

private:
    UPROPERTY()
    TObjectPtr<UWYASaveGame> CachedSave;
};
