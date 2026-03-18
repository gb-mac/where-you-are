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

### Revenue Model: Free-to-Play
**Decision:** F2P with in-game economy.
**Reasoning:** Maximum player acquisition for an AR game requiring real-world location.
