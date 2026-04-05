# Decisions Log

Record of major architectural and design decisions. Add an entry whenever a significant choice is made.

---

## 2026-03-17 (Economy)

### Two-Coin Economy System
**Decision:** Gold Coin (gameplay-earned only, in-game P2P tradeable, buys gear) + Silver Token (earnable via barter + purchasable with real money, buys cosmetics/services).
**Reasoning:** Keeps gear off the pay-to-win track while providing a real monetisation path. No crypto or blockchain integration — pure in-game currencies.
**Supersedes:** Earlier design considered crypto wallet / Bittensor integration for Gold. Dropped — adds legal risk, platform friction, and engineering complexity without improving the core game.

### Survival Resources as Commodity Money
**Decision:** Food, water, and ammo are required to survive and serve as the base barter currency. Water > Food > Ammo in baseline value; ammo is volatile (spikes during conflict).
**Reasoning:** Constant organic demand makes them natural commodity money. Geography-driven scarcity (GPS biome spawning) forces trade between players in different regions.

### The High Table
**Decision:** In-world faction that issues Gold-paying contracts, operates safe zones (Silver entry fee), validates player-posted contracts, and takes the Market Board tax cut.
**Reasoning:** Provides lore justification for the coin system and entangles the combat and economic layers.

### Silver Earned via Barter
**Decision:** Silver drops from barter participation via a mixed mechanic — fixed drip per trade, random bonus drop, volume milestones, and reputation multiplier.
**Reasoning:** Keeps Silver earnable in-game so it never feels like a pure cash shop. Rewards the merchant archetype with a distinct progression path.

### Gold ↔ Silver Exchange Blocked
**Decision:** Gold and Silver are not directly exchangeable between players.
**Reasoning:** Prevents real-money → Silver → Gold → gear laundering, which would recreate P2W through the back door.

## 2026-03-17

### Engine: Unreal Engine 5.5
**Decision:** UE5.5 as primary engine.
**Reasoning:** Native Linux support, Vulkan renderer, Nanite/Lumen for world fidelity, C++ module system.
**Alternatives considered:** Unity, Godot.

### Renderer: Vulkan + FSR
**Decision:** Vulkan on Linux, DX12 on Windows. FSR for upscaling (not DLSS).
**Reasoning:** AMD hardware (RX 5700) — no DLSS support.

### Network: Hybrid P2P
**Decision:** Centralized servers for matchmaking/anti-cheat, P2P for gameplay with host migration.
**Reasoning:** Reduces server costs for F2P while maintaining anti-cheat integrity.

### Location System: ENU Projection
**Decision:** WGS84 → ECEF → ENU local tangent plane for GPS-to-world conversion.
**Reasoning:** True metric distances in all directions; accurate to ~2m within 5km radius. Better than Mercator for world-space placement.

### Location Fallback Chain
**Decision:** Cache → IP geolocation (ip-api.com) → CLI args → DefaultGame.ini → hardcoded default.
**Reasoning:** Supports offline play; no API key required; testable without GPS hardware.

### Minimum System Specifications (Steam Hardware Survey, Feb 2026)
**Decision:** Minimum spec targets the median Steam player as of Feb 2026.

| Component | Minimum | Recommended |
|-----------|---------|-------------|
| GPU VRAM | 8 GB | 16 GB |
| RAM | 16 GB | 32 GB |
| CPU | 6-core, ~2.3 GHz | 8-core |
| Resolution | 1080p | 1440p |
| OS | Windows 10 64-bit | Windows 11 64-bit |

**Reasoning:** Steam Hardware Survey Feb 2026 — 8GB VRAM is the most common bracket (29.37%), 16GB RAM covers 27% of users (32GB is now majority at 56.93%), 6-core CPU is most common (29.87%), 1080p still leads at 45% but 1440p is closing fast (38.64%). Windows is 96.6% of Steam.

**AI pipeline impact:** Minimum-spec players have 8GB VRAM, making Q4_K_M 7B model GPU inference viable for everyone at minimum spec (~40–60 t/s estimated). phi3.5 3B / CPU inference is the sub-minimum fallback only. On-demand quest generation (not just async pre-gen) is viable at minimum spec.

**Note:** Current dev hardware (Beelink SER5850U, Vega 8) is sub-minimum. All AI pipeline work on Beelink is fallback-tier validation only.

### AI Pipeline: Local First → Bittensor
**Decision:** Local LLM inference first, migrate to Bittensor subnet later.
**Reasoning:** Validate pipeline on CPU before committing to decentralized infra costs.

### AI Quests Are Side Quests Only — Main Story Is Static
**Decision:** All LLM-generated quests are side quests. The main story questline uses hand-authored static quests only.
**Reasoning:** LLM output is non-deterministic and cannot guarantee narrative coherence, pacing, or lore consistency required for a main storyline. Side quests are low-stakes, location-flavoured, and repeatable — a good fit for generative content. Main story beats need writer control.
**Impact:** `UWYAAISubsystem` generates and caches side quests only. A separate static quest system (to be built) owns the main story. The two systems must never mix quest types at the assignment layer.

### World Axis Mapping
**Decision:** UE5 X=North, Y=East, Z=Up (1 UU = 1cm).
**Reasoning:** Standard ENU convention; consistent with navigation/AR frameworks.

### AR is a Companion Mobile App, Not Part of the Desktop Game
**Decision:** AR overlay features are a separate mobile app (`~/Projects/game-mobile`), not part of the UE5 desktop game.
**Reasoning:** Desktop UE5 cannot render AR camera overlays. The Ingress/Pokémon Go layer (dead drops, GPS item placement, Warden Markers, faction territory, Displacement Zones) requires a mobile device with a camera and GPS.
**Impact:** Desktop game uses GPS only to set the world spawn origin. Mobile app handles all real-world AR interaction.
**Cross-platform mechanic:** Items placed at GPS coordinates via the mobile app are claimable in the desktop game at the corresponding world-space position (via `FWYAGeoMath::GeoToWorld`), and desktop players can place items retrievable by mobile players. Both platforms read/write from the same shared item state backend — this is a core gameplay loop, not an edge case.

### Art Direction: Visual Style Guide v0.1
**Decision:** Three visual language system — Human / Twin-Machine / Vael. Full faction color axis established for all 13 factions + Vael 3 tiers. See `docs/art-direction/style-guide.md`.
**Performance budget decisions:**
- Vega 8 fallback: baked lighting, 3-tier LOD chain, single-layer PBR, 50% particle cap
- LOD must preserve faction silhouette and primary identification color at all tiers
- Fractured faction requires procedural color/material system (zone-typed) — asset budget TBD
- FSR rule: faction identification must survive 50% internal resolution; no world-space text below 18px at 1080p output
**Flagged for budget review:** Fractured procedural material system scope.

### AI Pipeline: Pre-Gen Buffer Size = 3, Refill on Pop
**Decision:** `UWYAAISubsystem` maintains a buffer of 3 pre-generated side quests (ready + in-flight). On location resolve, `RefillBuffer()` schedules up to 3 concurrent ollama requests. Each `TryPopPregeneratedQuest()` call triggers `RefillBuffer()` to schedule a replacement, keeping the buffer perpetually topped up.
**Reasoning:** 11.6 t/s on Beelink is too slow for on-demand generation (~25s/quest) but fine for background pre-gen. Buffer depth of 3 ensures the player almost never waits — first quest is ready in ~25s after spawn; subsequent quests are pre-warmed before the player finishes the first. Replacement is scheduled immediately on pop so the queue self-heals without a timer.
**Note:** All 3 requests are generated for the spawn-origin location. Location-change-triggered re-generation (e.g. after 500m travel) is deferred — Beelink is sub-minimum hardware and can't sustain continuous generation. Re-evaluate on RX 5700.

### Revenue Model: Free-to-Play
**Decision:** F2P with in-game economy.
**Reasoning:** Maximum player acquisition for an AR game requiring real-world location.

## 2026-04-04 (Economy)

### Two Distinct Safe Zone Types
**Decision:** High Table Zones and Neutral Ground are separate systems with different controllers, locations, and access mechanics.
- **High Table Zones** — indoor, building-specific, Silver entry fee, operated by the High Table, access revocable
- **Neutral Ground** — outdoor, world-space, free access, enforced by Warden Vanguard, barred to excommunicado players
**Reasoning:** They serve different gameplay functions. High Table zones are a purchased economic service (monetisation-adjacent). Neutral Ground is a social institution gated by Warden standing/conduct, not payment.
**Impact:** Economy docs updated. Silver is the currency for High Table zone access only — Neutral Ground has no Silver cost. Excommunicado economic consequences (loss of Neutral Ground) are a reputation penalty, not a monetary one.

### Gold Supply: Milestone-Minted, No Cap
**Decision:** Gold is milestone-minted (new Gold enters when players complete qualifying events). No per-player cap — if you want to run contracts all week, you earn Gold. No fixed global supply cap.
**Reasoning:** A weekly cap punishes dedicated players and undermines the High Table contractor archetype. The natural limit is contract availability per region and the time cost of completing them — that's enough friction. Gold/Silver exchange remains blocked.
**Sources:** High Table contracts (20–50 Gold), faction rank-ups (25–100 Gold, one-time per tier), major milestones (50–200 Gold, one-time).
**Sinks:** T3 blueprints (50–300 Gold), High Table bounty posting (20–100 Gold), player-to-player trade.
**Supersedes:** Earlier design had a 100 Gold/week per-player cap. Dropped — artificial ceiling that punishes the hitman playstyle the game is built around.

### Survival Consumption: Activity-Based with Real-Time Floor
**Decision:** Food and water drain faster during combat/exertion, slowly while idle. Operative baseline: water depletes in 90 minutes active / 4 hours idle; food in 3 hours active / 12 hours idle. Environmental and wound modifiers apply. Starvation/dehydration causes debuff cascades, not instant death. Contractor: 50% rates. Survivor: 150%/130% rates.
**Reasoning:** Real-time floor ensures the system always presses; activity multiplier rewards active play management without punishing casual sessions in Contractor mode.

### T3 Crafting: Gold Blueprint + Maker Standing for Full Output
**Decision:** T3 blueprints require Gold to purchase AND Maker peer standing to execute at full T3 quality. Without Maker standing, output is capped at T2.5. With Maker standing, full T3 unlocked.
**Reasoning:** Reconciles economy-core.md (Gold gates blueprints) with arc-tradesperson.md (Maker network gates technique). Gold buys the document; relationship earns the understanding. Two different barriers serving different functions — one economic, one social/narrative.

### Bounty System: Two Types
**Decision:** Player-posted bounties (Silver-funded, 30 Silver posting fee, server escrow, 7-day expiry) and High Table-mediated bounties (Gold-funded, institutional, no expiry). Warden Hunter registration gives AR tracking advantages for excommunicado targets. Fulfillment inside Neutral Ground is a code violation regardless of bounty status.
**Impact:** Silver sink (posting fees burned), Gold source (High Table bounties). Informal Hollow bounty system operates in parallel with no server escrow.

### Shadow Economy: Syndicate Black Market
**Decision:** Shadow Market Boards exist in Hollow Syndicate territory. No High Table tax, no server escrow, fenced goods accepted, Syndicate protection fee instead of tax. Access requires Hollow standing. Item provenance metadata is preserved server-side — fenced items are never "clean." Compact raids create periodic disruption. Builds Hollow standing, noted by Wardens/Compact.
**Reasoning:** Parallel economy should have genuine texture and real costs — not a simple tax avoidance vehicle. Designed so the math only works if you're already in the Hollow world.

### Workshop Placement: Biome Affinity + Territory Effects, No Hard Zone Lock
**Decision:** Workshops can be placed anywhere the player controls territory. Biome affinity affects output efficiency (off-biome T3 costs more materials). Territory faction affects access and risk (Warden territory = Maker network access; Hollow territory = Shadow Market access + raid risk). Hire-out permitted in two modes: private (designated accounts) and open station (Silver fee, public access). Ownership/access enforced by reputation, not server lock.

### Profession Progression: Activity XP to T2, Instruction Required for T3
**Decision:** T1→T2 is activity-based XP (craft things, get better). T2→T3 requires an instruction event — a Maker, a faction questline unlock, or another T3 crafter teaching the technique. T3 knowledge is held in people, not grindable.
**Reasoning:** Consistent with post-Cascade information economy. Prevents T3 from being purely time-gated while keeping it narratively earned.

### Dead Drop Gap: Payment Infrastructure Missing (Flagged to Core)
**Decision:** Dead Drop payment flow is unimplemented. `FWYAItemData` has no price field; `ClaimItem` sends no payment; backend has no payment transfer logic. Spec documented in `docs/economy/dead-drop-audit.md`. Recommended approach: Option A (backend-implicit, 402 on insufficient funds). Not a blocker today; must be addressed before vertical slice economy ships.

## 2026-04-04 (Core)

### Side Mode: GPS Centroid PvP Arena

**Decision:** Build a standalone PvP side mode where the match map is determined by the real-world GPS centroid of all participating players, with centroid-snapping to the nearest landmass.

**How it works:**
1. Players queue from anywhere in the world. Their real GPS coordinates are submitted to the backend.
2. Backend calculates the geographic centroid (average lat/lon) of all queued players.
3. If the centroid falls over ocean, it snaps to the nearest landmass coastline — players in Calgary vs. players in Tokyo might land anywhere: central Africa, a Pacific island, Antarctica. Nobody knows in advance.
4. That GPS coordinate becomes the `CesiumGeoreference` world origin. Cesium loads whatever real terrain exists there — the terrain IS the map.
5. All players spawn at randomised positions within a radius of the centroid (e.g. 200–500m). Real-world distance between players is irrelevant — it only selects the terrain.
6. When a player drops in or out mid-match, backend recalculates the centroid, snaps to nearest landmass, and smoothly lerps the world origin to the new position over 2–3 seconds.

**Design intent:** The terrain wildcard is the feature, not a side effect. A mountain range, a salt flat, an Antarctic research station — nobody picks the map. Their combined real-world locations do. No repeated maps, no memorised callouts.

**Option A vs B:** Players spawn near the centroid regardless of real-world distance (Option A). Option B (preserving relative distances) was considered but rejected — a Calgary/Tokyo pair would spawn thousands of km apart in-game. Option A is correct.

**Landmass snap:** Required. Ocean centroids are unplayable. Backend uses a coastline proximity lookup (e.g. Natural Earth dataset or equivalent) to find the nearest land point. Antarctica and remote islands are valid — that's a feature.

**Technical build order:**
1. Backend: `/pvp/match` endpoint — accept join/leave, compute centroid, snap to nearest land, return result + spawn offsets
2. `WYAPvPSessionSubsystem` — manages match state, polls for centroid updates, shifts CesiumGeoreference, handles player reposition on centroid change
3. `AWYAPvPGameMode` — kill tracking, match end conditions, reward payout in Gold
4. Smooth centroid shift — lerp geo origin to avoid jarring snaps on late joins

**Scope:** This is a side game — a discrete session mode players opt into, separate from the persistent open world. It does not affect the main game's economy, factions, or world state. Gold rewards from PvP should be modest to prevent it from becoming the primary Gold farm over contracts.

**Status:** Logged for future build. Not blocking current work. Flag to backend agent when ready to spec the `/pvp/match` endpoint.

---

### Bittensor Integration: Fully Deferred Post-EA
**Decision:** No Bittensor/blockchain integration in scope for Early Access. Placeholder doc created at `docs/economy/blockchain/bittensor-placeholder.md`. Any future design requires legal review in all operating jurisdictions before work begins.
**Legal flags logged:** Securities law (TAO classification), gambling regulations, platform terms (Steam policy), tax reporting obligations.
