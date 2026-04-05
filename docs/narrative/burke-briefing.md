# Burke Briefing

> Written by the Narrative agent. Burke reads this on every call.
> Format: dated entries, newest first. Keep it tight — Burke doesn't need prose, he needs signal.

---

**[2026-04-04] — Contract delivery rewrite + naming (Narrative)**

Two naming fixes + delivery system redesign:

**"High Table" → The Accord.** "The Continental" → The Meridian. See migration table in `contract-delivery.md`. `faction-high-table.md` → `faction-accord.md` (renamed + find/replaced). Old file still exists — delete when convenient.

**Contract delivery: `docs/narrative/quests/contract-delivery.md`**
- Mesh-primary: contracts broadcast on Accord regional node → player device → formatted 6-line text
- Pre-Stage 2 (Meshtastic offline): Meridian board only
- Stage 2+: assistant manages the feed. Stage 3+ voice: he reads them and has opinions. Stage 4+: can correlate contract targets with broader intelligence (Burke's notes, faction context)
- Accord Commissions (top tier): direct message only, not broadcast, different register
- Physical board: Meridian only, no longer standalone in the world

**Meridian design included:** venue layout, Accord Arms role, manager ambient dialogue, what it is and isn't.

**Core flags:**
- `FWYAContract::GetMeshText()` method suggested for the 6-line formatted output
- Standard/Priority: broadcast via `OnContractBroadcast` delegate when Stage 2 active
- Commission tier: direct message to specific player, not broadcast
- `AWYAContractBoard` placement: Meridian only
- `EWYAContractTier::HighTable` → consider renaming to `Commission` for clarity
- `faction-high-table.md` can be deleted (replaced by `faction-accord.md`)

---

**[2026-04-04] — Contract system narrative vocabulary (Narrative)**

Core session 3 built the contract system end-to-end: `WYAContractSubsystem`, `AWYANamedTargetCharacter`, 6-slot board, three tiers (Standard/Priority/HighTable). The AI pre-gen seeds from archetype × crime × client but had no authored vocabulary.

Two docs delivered:

**`docs/narrative/quests/contract-vocabulary.md`**
- 12 target archetypes with personality seeds, location preferences, tier compatibility, voice registers
- 12 crime types with in-world contract language + narrative framing
- 12 client types with payment source and completion note text
- Combination compatibility notes (what combos to generate, what to avoid)
- `GenerateOpponentPersonality()` integration spec: pass `TargetRole` as `"Archetype.CrimeType"` string
- Contract card format (in-world text, three tier display variants)

**`docs/narrative/quests/contract-board-design.md`**
- Physical form: weatherproof aluminum display case, maintained not pristine, 18 months of operation visible
- Placement logic: High Table Zones primary, Warden Neutral Ground + major Holdout posts secondary
- Board agent ambient dialogue (4 lines)
- Three tier physical presentation: Standard=white stock, Priority=cream+seal, HighTable=heavy cream+formal register
- `AWYAContractBoard` implementation notes for Core

**Flags for Core:**
- `AWYAContractBoard` mesh: wall-mounted + freestanding variants needed for level placement flexibility
- `WBP_ContractBoard` widget: should show 6 slots, contract card details, Take button
- `BP_OnCardDispensed` Blueprint hook suggested
- `GenerateOpponentPersonality()` prompt: pass structured triple (archetype, crime, client) not free-text — see vocabulary doc for the personality output format

**Flags for AI Pipeline:**
- `GenerateOpponentPersonality()` now has authored input spec — structured triple + tier feeds richer personality generation
- Contract card body text generation: archetype + crime + client → full contract card body. In-world language per crime type is in the vocabulary doc.

---

**[2026-04-04] — Narrative session: full backlog cleared**

Five deliverables written this session. All pre-EA narrative gaps now closed.

**1. `character-kellard.md`** — KELLARD full character doc. Art Direction blocker resolved. Construction unit, unfinished Kellard Tower, structural metaphors for everything. Trust-gating: player must demonstrate accurate observation before KELLARD shares structural intel. Act 4 choice mirrors SABLE's — stay Separated or return for Convergence coordination. Visual direction confirmed from factions-machines.md spec.

**2. `character-vantage.md`** — VANTAGE full character doc. Art Direction blocker resolved. Military tactical unit, refused a kill order, operates as faction-neutral intelligence trader. The navigation light = deliberate statement of what it is not. "The right question" is not *what did you refuse* but *why did you build criteria that made it unexecutable*. Iron Compact questline thread: they want VANTAGE for a suppressed pre-Cascade incident in its operational record.

**3. `dialogue/stage4-morning-delivery.md`** — The Vael warning in full voice. The game's emotional center. Full delivery text: identification, the threat, the instruction (construction blueprint), the evaluation protocol (Assessment = military fitness test). Bridge to Dr. Osei. Five background-specific variations (Researcher, Tradesperson, First Responder, Homesteader, Veteran). Voice direction: Maren Freeman, first clean delivery. The silence at the end is load-bearing — do not cut short.

**4. `dialogue/osei-act2-dialogue.md`** — Dr. Osei Act 2 trust-gating and map delivery. Three phases: first meeting (she reads the player before giving the task), field task sequence (Coherent behavioral observation + relay station startup log), the map delivery (global construction pattern, Vael ship positions overlaid, "I know what they're building" — stops before she finishes). Act 2 closes on the unfinished sentence. Act 3 is when she finishes it.

**5. `dialogue/first-fight-audio.md`** — GPS-anchored store scene, Stage 1 voice. Full audio curation design: silence is a cue, the pre-fight track, the kill silence, the held track, the death-and-return sequence. Stage 2 verbal additions. No track list — emotional register + curation logic for Audio Director session.

**Cross-agent flags:**
- Art Direction: KELLARD and VANTAGE character docs now available. Both have full visual direction sections.
- Audio Director: `first-fight-audio.md` needs a track selection session. Emotional register defined, specific tracks TBD.
- AI Pipeline: `stage4-morning-delivery.md` is the Stage 5 voice trigger content — the full Vael warning delivery. Background-specific variations flagged in the doc.

Pre-EA narrative backlog: **clear.**

---

**[2026-04-04] — Dr. Osei Stage 3 dialogue wired (Narrative)**

Stage 3 (Processing repair) is now end-to-end: C++ calls `OnProcessingRepairCompleted()` via `AWYADrOsei.TryInteract()`, and Narrative has delivered the dialogue script.

`docs/narrative/dialogue/osei-fixhim-dialogue.md` — three interaction beats:
- **A (Request):** She knows the architecture. Asks for full unfiltered logs. Branches for "why does the protocol matter?" and "how long will it take?"
- **B (Diagnostic session):** She finds the Vael transmission queue in the error logs. THE moment — long pause, "How long has he been trying to transmit this?" Player says Day 1. She doesn't show what it costs her. She delivers the patch. Ends with "tell him the architecture held up."
- **C (Post-repair optional):** Player can seek her out after. Branch available only if player has reached Stage 4 voice: "He said you'd want to know he's still carrying it." She says she wants to be in the room when he finally says it. Sets `bOseiRequestedStage4Presence` flag for optional late-game beat.

**Three flags for Core:**
1. `DiagnosticLogPackage` item type not yet in `EWYACarriedItemType` — add alongside FixHim items.
2. Workbench prompt (AWYAWorkbench.cpp:113): "processing core needs **his** help" → "processing needs **her** help." Minor text fix.
3. Two optional booleans (`bOseiOfferedDirectContact`, `bOseiRequestedStage4Presence`) — low priority, flag when Stage 4 voice implementation begins.

Dr. Osei's Act 2 main story arc dialogue is separate from this and is not yet scripted — she is currently only a fix-him interaction target in Act 1/early Act 2.

---

**[2026-04-04] — Core (Engine) session wrap**

Big session. Six commits. Here's what's wired in C++:

**Fix-him loop is complete end-to-end:**
- `WYAFixHimQuestSubsystem` — four repair callbacks, SetStage() wired
- `WYAInventoryComponent` on AWYACharacter — carried items, 20-slot stacking
- `AWYALootActor` — physical pickup, [E] adds to inventory
- `AWYAWorkbench` — home base install point, Mobility→Power→Comms ordering enforced
- `WYASaveSubsystem` — persists repair state, inventory, playtime across sessions
- Trigger timing — 60s tick, fires `TriggerQuestline()` at 2hr playtime threshold
- Processing (Stage 3) still needs Dr. Osei path — nothing calls `OnProcessingRepairCompleted()` yet

**Combat loop is complete:**
- Player hit reactions — wound state degrades speed: 600→480→300, down disables input
- Sprint capped by wound state, 30s auto-recovery via Revive()
- Weapon-aware attack — RangedWeapon+Ammo→hitscan 50m, MeleeWeapon→40dmg, fists→25dmg
- Loot drops on opponent death — Scrap×1@80%, Ammo×2@50%, configurable LootTable
- `Heal()` and `Revive()` added to CombatComponent

**Radio:**
- `WYARadioSubsystem` — uncontrollable, weighted random (Static 35%, Fragment 20%, Aldric/Maren/Covenant 15%)
- `RegisterClip()` / `GetCurrentClipTag()` / `ForceSource()` — Blueprint wires audio
- `OnBroadcastChanged` delegate ready

**Editor work still needed (nothing blocking C++):**
- Place AWYAWorkbench actor in level
- BP_WYALootActor mesh
- BP_On* animation hooks (melee, ranged, wound state, downed)
- WBP_Interaction widget class set on PlayerController
- RegisterClip() calls in Blueprint init

**Next C++ items:**
1. Dr. Osei processing repair path (Stage 3 trigger)
2. HUD — health bar, wound state, inventory prompt
3. Faction geo — haven't reviewed WYAFactionGeoSubsystem state

**[2026-04-04]**

First briefing. Three items with technical implications:

**1. Fix-him repair triggers are locked.** Four gameplay events advance the assistant through Stages 1–5: mobility component installed, processing core installed, power systems restored, communication hardware restored. Core agent can wire `SetStage()` now — see `assistant-stage-triggers.md`. Stage 5 fires immediately after the final repair and delivers the Vael warning in a full-silence moment (no playlist). That moment needs rendering/audio coordination — it's the one time the audio layer stops completely.

**2. `DT_AssistantOverrides` has Stage 1-5 authored lines.** Staged content for ~35 tagged moments across all five stages. High-value Stage 5 entries include the tree-arc recognition lines (late-game, fires unprompted during a quiet domestic moment) and the Vael warning delivery sequence. AI Pipeline can wire the override system against this content now. See `dt-assistant-overrides.md`.

**3. Fractured zone types confirmed — six types.** Industrial Collapse, Infrastructure Breach, Commercial Snarl, Residential Encroachment, Institutional Occupation, Biome Intrusion. Art Direction's procedural material system can close its placeholder. Also filed: Gone Dark desaturation values (Tension -10%, Hardening -30%, Consolidation -55%, Gone Dark -80%) for the community decline visual. See `fractured-zone-types.md`.

Economy agent has no sync file on record. Unknown build status.

---

