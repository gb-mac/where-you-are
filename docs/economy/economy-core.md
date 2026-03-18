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

## The High Table

The High Table is the in-world faction that issues contracts and enforces the coin system. It is the lore justification for Gold's authority.

**What the High Table does:**
- Posts contracts (assassination, escort, retrieval, merchant protection)
- Pays out Gold on contract completion
- Operates safe zones — players inside cannot be attacked (costs Silver to enter)
- Takes the Market Board tax cut (lore)
- Can revoke safe zone access for players who break the rules

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

## Open Questions

- [ ] Consumption rates for food/water — real-time drain vs activity-based
- [ ] Death penalty for starvation/dehydration
- [ ] Exact Silver earn rates and caps (balance pass needed)
- [ ] Gold coin supply mechanics — fixed cap or milestone-minted?
