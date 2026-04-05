# Burke Briefing

Dated entries, newest first. Read on every Telegram message.

---

**[2026-04-04]** Contract security layer complete. Full encounter loop wired: kill named target → exfil spawns 100–200m away, 90s window → player reaches exfil → Gold awarded with bonuses. Ghost (no alerts) +50%, Swift (under tier time limit) +25%, Clean (no damage) +25%. Security NPCs patrol a wider ring around the target (2/3/5 by tier). Alert propagates to all security in 30m radius via RaiseAlert(). Player damage tracking wired in AWYACharacter::OnHealthChanged. All 8 security files committed. Narrative flag: need identity for security NPCs in this world — what are they guarding, who do they work for, what do they say when they spot the player? Also need extraction narrative — player escaping after the hit. AI Pipeline flag: consider passing encounter archetype to NPC personality generation so security behaviour is context-aware (guard detail vs patrol vs lookout).

**[2026-04-04]** Naming fix: "High Table" → The Accord, "the Continental" → The Meridian. `faction-accord.md` written, `faction-high-table.md` can be deleted. Contract delivery redesigned as mesh-primary — see `contract-delivery.md`. Core flags: `FWYAContract::GetMeshText()` method, `OnContractBroadcast` delegate gated on Stage 2 Meshtastic flag, `AWYAContractBoard` Meridian-only, consider `EWYAContractTier::Commission` rename.

**[2026-04-04]** Narrative: Contract vocabulary delivered. `contract-vocabulary.md` + `contract-board-design.md` now in `docs/narrative/quests/`. Flags: (1) AWYAContractBoard needs wall-mounted + freestanding mesh variants, (2) WBP_ContractBoard: 6 slots + card details + Take button, (3) GenerateOpponentPersonality() — pass structured archetype/crime/client triple not free-text, see vocabulary doc. AI Pipeline flag: contract generation prompt now has authored input spec.

**[2026-04-04]** Narrative: Dr. Osei fix-him dialogue scripted. `docs/narrative/dialogue/osei-fixhim-dialogue.md` ready — three interaction beats (request, diagnostic session, post-repair optional). Three small flags: (1) `DiagnosticLogPackage` item type missing from `EWYACarriedItemType`, (2) AWYAWorkbench.cpp:113 prompt text "his help" → "her help", (3) two optional booleans for late-game Osei/Stage 4 beats, low priority.

**[2026-04-04]** Workbench + Dr. Osei wired. AWYAWorkbench now gates Power install behind Processing (Dr. Osei must run first — was missing). AWYADrOsei.TryInteract() implemented and dispatched from OnInteract(). PlayerController refactored: HUDWidget → InteractionWidget, new MainHUDWidget (WYAHUDWidget), TickHUD() drives health/survival/currency on 100ms poll. Interaction dispatch order: Workbench → DrOsei → Loot → GPS items. Stage 3 fix-him now fully wired end-to-end. Uncommitted — 4 files in Source/.

**[2026-04-04]** Core (Engine) session — economy foundation. Four commits: economy docs (no Gold cap), UWYASurvivalComponent, UWYACurrencySubsystem, dead drop payment patch. Build clean. Backend still needs priceAmount/priceCurrency on WorldItemDTO + 402 payment logic on /claim — flag when in game-backend next.

**[2026-04-04]** Session start — Core (Engine). Economy agent delivered full design spec last session. 3 files staged, uncommitted: `decisions-log.md`, `docs/economy/crafting.md`, `docs/economy/economy-core.md`. Need commit. Next C++ build order: `UWYASurvivalComponent` (Layer 1) → `UWYACurrencySubsystem` (Layer 2) → dead drop payment patch (5 changes, see `docs/economy/dead-drop-audit.md`). Stage 3 fix-him (Dr. Osei) still unwired — low priority vs economy foundation.

**[2026-04-04]** Core (Engine) session wrap. Six commits: fix-him quest loop, save/load, combat depth (hit reactions, loot drops, weapon-aware attack, ranged), physical loot actors, radio subsystem. Build confirmed clean with Cesium. Stage 3 fix-him (Dr. Osei processing repair) NOT wired — `OnProcessingRepairCompleted()` exists, nothing calls it. Editor work still pending (workbench placement, BP meshes, animation hooks).
