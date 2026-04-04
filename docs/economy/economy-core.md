# Economy Core Design

**Where You Are** — Player-driven economy specification.

---

## Overview

Three-layer economy:

1. **Survival layer** — food, water, ammo. Consumable resources required to stay alive. The foundation of all trade.
2. **Barter layer** — player-to-player trading of resources and crafted items, denominated in survival commodities or Silver.
3. **Prestige layer** — two coins (Gold, Silver) for higher-tier transactions, gear, cosmetics, and services.

---

## Layer 1: Survival Resources

Three core consumables required to survive. Running out has real consequences.

| Resource | Value | Notes |
|----------|-------|-------|
| Water | Highest | No substitute. Drains constantly. |
| Food | High | Drains during activity. |
| Ammo | Situational | Not needed to survive — fist fighting is viable. Spikes in value during conflict, deflates in peace. |

Water and food are the de facto **commodity money** of day-to-day barter. Players quote prices in water/food because everyone always needs them.

### Geography-Driven Scarcity

Resources spawn at GPS coordinates tied to real-world biome tags:

| Biome Tag | Real-World Source | Primary Spawns |
|-----------|-------------------|----------------|
| Urban dense | City centers | Salvage Metal, Data Chips, Fiber |
| Industrial | Ports, factories | Heavy Alloy, Fuel Cells, Polymers |
| Rural/Green | Parks, forests | Organic Compounds, Rare Earth, Food |
| Water | Rivers, coastlines | Water, Coolant, Bio-material |
| Transit | Roads, rail | Logistics Parts, Signal Crystals |

Players in different regions are structurally surplus in some resources and scarce in others. This is the engine of trade.

### Carry Limits

Carry weight applies to all resources. Vehicles extend carry capacity significantly — a player with a vehicle can act as a trader/hauler archetype.

---

## Layer 2: Barter Economy

Players trade resources and crafted items directly. The barter system is the primary source of **Silver** (see Layer 3).

### Trade Mechanisms

**Direct Trade** — two players in range. Both confirm; server executes atomically. Can be denominated in any item or coin.

**Market Board** — player-placed structure (Town Hub or Faction Base). Sellers list items with an ask price. Server holds items in escrow. Listings expire if unclaimed.

**Dead Drop** (cross-platform) — mobile player places an item at a GPS coordinate with a price tag. Desktop player claims it at the equivalent world-space position. Server transfers payment on claim. Uses shared item state backend.

**Barter Toggle** — any trade can be quoted in items instead of coins. UI shows Silver equivalent for reference; players negotiate freely.

### Market Board Tax

5–10% Silver burned on all Market Board sales. Controls Silver supply and funds the in-world economy (lore: a cut taken by the High Table for use of their infrastructure).

### Anti-Exploitation

- Server authoritative on all transactions — no client-side economy logic
- Rate limits on listings per account per day (prevents bot flooding)
- Price history visible to all players (transparency fights manipulation)
- No bulk API — prevents scripted arbitrage bots

---

## Layer 3: Two-Coin System

### Gold Coin

| Property | Value |
|----------|-------|
| Earned by | Gameplay only — milestones, achievements, High Table contracts, faction rank-ups |
| Purchasable with real money | No |
| Tradeable | Yes — player-to-player in-game only |
| Buys | Weapons, armor, gear |

Gold is scarce and prestigious. It cannot be purchased with real money, only earned through meaningful gameplay. This keeps gear off the pay-to-win track.

**Gold is not exchangeable for Silver** (and vice versa) to prevent real-money → gear laundering via Silver.

### Silver Token

| Property | Value |
|----------|-------|
| Earned by | Barter participation + real-money purchase |
| Purchasable with real money | Yes |
| Tradeable | No (or capped) |
| Buys | Cosmetics, safe zone access, contract posting, services |

Silver is the monetisation vehicle. It funds development without gating power behind a paywall.

A small Silver drip is earnable in-game through barter so the economy never feels like a pure cash shop.

### Silver Earn Mechanics (Barter Rewards)

Silver drops from barter participation via a mixed system:

| Mechanic | How it works |
|----------|-------------|
| **Fixed drip** | Every completed trade generates a small Silver reward for both parties |
| **Random bonus** | Chance of a larger Silver drop on any trade — lottery feel, keeps players engaged |
| **Volume milestones** | Completing X trades unlocks better drop rates — rewards dedicated traders |
| **Reputation multiplier** | Higher reputation score = better Silver yield per trade |

### Merchant Reputation

- Both parties rate each trade (thumbs up/down)
- Disputes flagged server-side — repeated flags throttle listings
- High-rep players earn **Trusted Merchant** status, visible to other players
- Rep is tied to trade volume over time — cannot be faked quickly
- High-rep merchants are valuable targets for High Table contracts, protection deals, and extortion — entangles the social and combat economies

---

## Safe Zones — Two Distinct Types

There are two separate safe zone systems with different controllers, locations, and access mechanics.

### High Table Zones
Indoor, building-specific safe zones operated by the High Table. Think a secure bar, a fixer's office, a fortified market floor.

- **Access:** Costs Silver to enter
- **Coverage:** The building interior only — stepping outside ends protection
- **Authority:** High Table can revoke access for players who break the rules inside
- **Distribution:** Urban-dense and Industrial biomes — wherever the High Table has established a footprint

### Neutral Ground (Warden-Controlled)
Outdoor, world-space safe zones maintained by the Warden Vanguard. Specific locations in the open world — crossroads, bridges, established waypoints — designated as unconditional no-combat zones.

- **Access:** Free by default — but excommunicado players are barred entirely
- **Coverage:** A defined outdoor area (no building required)
- **Authority:** Wardens enforce it physically — Vanguard present or on-call
- **Distribution:** GPS-anchored to real-world landmarks and transit nodes; more common in rural and transit biomes, present but sparser in dense urban areas

**Key difference:** High Table zones are a purchased service. Neutral Ground is a social institution. One costs Silver; the other costs reputation — you lose it the moment you're excommunicado.

---

## The High Table

The High Table is the in-world faction that issues contracts and enforces the coin system. It is the lore justification for Gold's authority.

**What the High Table does:**
- Posts contracts (assassination, escort, retrieval, merchant protection)
- Pays out Gold on contract completion
- Operates High Table Zones (indoor safe zones, Silver entry fee)
- Takes the Market Board tax cut (lore)
- Can revoke High Table Zone access for players who break the rules

**Contract posting** — any player can post a contract (costs Silver). The High Table validates it. Fulfillment pays Gold to the contractor.

---

## Player Archetypes the Economy Supports

| Archetype | Primary activity | Primary earner |
|-----------|-----------------|----------------|
| Combatant | PvP, contracts | Gold |
| Trader/Merchant | Barter, Market Board | Silver |
| Crafter | Resource processing, item production | Silver + barter |
| Explorer/Hauler | Resource gathering, logistics (vehicle) | Survival resources |
| Faction player | Faction rank-ups, territory | Gold + faction rewards |

No archetype is locked out of progression. Combat skill and economic skill are parallel paths.

---

## Crafting (Summary — full spec TBD)

Three tiers:

- **T1** — basic gear, consumables. Common materials, no skill gate.
- **T2** — specialist items. Rare materials + crafting skill unlocked by playtime.
- **T3** — high-end gear. Rare materials + player-owned Workshop + location requirement (e.g., Heavy Armor only at an Industrial-zone Workshop).

Crafted items feed the barter layer. Blueprints/patterns for high-tier gear are purchasable with Gold — you still need the materials to craft, so Gold doesn't skip the economy.

---

## Survival Resource Consumption Rates

Consumption is **activity-based with a real-time floor** — resources drain faster during exertion, but also drain slowly while idle. Running out has real consequences; the rates are tuned so that active play without resupply creates meaningful pressure within a session.

### Baseline Rates (Operative Mode)

| Resource | Full depletion (active play) | Full depletion (idle) | Activity multiplier |
|----------|-----------------------------|-----------------------|--------------------|
| Water | 90 minutes | 4 hours | 2× during combat/sprint |
| Food | 3 hours | 12 hours | 1.5× during sustained activity |

**Environmental modifiers:**
- Heat zones (urban summer, near industrial Fractured): water drain ×1.5
- Cold zones (rural winter): food drain ×1.2
- Vael Displacement Zone proximity: water drain ×1.3 (disrupted biology)

**Wound modifiers:**
- Any active bleed: water drain ×1.2
- Toxin exposure: food drain ×1.3
- Multiple wounds stacked: both resources drain faster

### Mode Adjustments

| Mode | Water rate | Food rate |
|------|-----------|-----------|
| Contractor | 50% of baseline | 50% of baseline |
| Operative | Baseline | Baseline |
| Survivor | 150% of baseline | 130% of baseline |

### Death Penalty for Starvation/Dehydration

- **Dehydration to zero:** Debuff cascade (reduced stamina → impaired accuracy → movement penalty). Does not cause instant death. Gives a 10-minute grace window before the next tier applies. Sustained dehydration causes Severe Dehydration wound state — requires treatment, not just drinking.
- **Starvation to zero:** Slower cascade, longer grace window (30 minutes). Severe Hunger wound state at bottom.
- **Combined zero:** Both hitting zero simultaneously accelerates the cascade significantly. This is the kill condition for neglect, not a single resource running out.

Design intent: death from survival neglect should feel like a slow accumulation of bad decisions, not a surprise. Players get plenty of warning.

---

## Gold Supply Mechanics

Gold is **milestone-minted** — new Gold enters the economy when players complete qualifying events. There is no fixed global supply cap, but there is a per-player weekly earning cap that prevents farming.

### Per-Player Weekly Cap: 100 Gold

Active players can earn up to 100 Gold per week through normal play. The cap resets weekly. It is not possible to earn more than 100 Gold in a week regardless of activity level — surplus milestone completions are logged and awarded in the following week's cap.

### Gold Sources

| Source | Amount | Notes |
|--------|--------|-------|
| High Table contract completion | 20–50 Gold | Primary ongoing source |
| Faction rank-up (each tier) | 25–100 Gold | One-time per tier per faction |
| Major milestone/achievement | 50–200 Gold | One-time, narrative milestones |
| Player-to-player trade | Variable | Gold can change hands; no new Gold created |

### Gold Sinks

| Sink | Cost |
|------|------|
| T3 blueprint purchase | 50–300 Gold |
| High Table bounty posting | 20–100 Gold |
| Player-to-player trade | Variable |

### Anti-Inflation Design

The weekly cap prevents Gold farming. Faction rank-ups and narrative milestones are finite per player. The primary ongoing source (High Table contracts) is limited by contract availability per region — not infinite on demand. Gold supply grows with the active player base, but not faster than new players completing milestones.

Gold is not exchangeable for Silver. Silver cannot be converted to Gold. These rails are hard and do not bend.

---

## Open Questions

- [ ] Exact Silver earn rates — final balance pass needed once playtesting begins
- [ ] High Table contract Gold amounts — needs calibration against T3 blueprint costs
- [ ] Whether Survivor mode players earn bonus Gold (currently: they earn enhanced loot, not bonus Gold — confirm this is right)
