# Crafting System

**Where You Are** — Crafting specification.

---

## Overview

Three-tier crafting system combining survival scavenging, workbench progression, and player profession specialisation. Gear degrades over time, creating permanent demand for professional crafters.

---

## Tiers

### T1 — Survival Crafting
- No tools or station required — craft anywhere
- Intuitive recipes: combine scavenged materials to make basic items
- Available to all players from day one
- Output: basic consumables, simple weapons, ammo, basic clothing
- Example: Water + Organic Compounds → Purified Water

### T2 — Workbench Crafting
- Requires a crafting station (player-placed or found in the world)
- More complex recipes, better output
- Biome-specific materials start to matter — you must travel to craft certain things
- Output: mid-tier weapons, armor, consumables, structures
- Solo viable but geography-gated

### T3 — Profession Crafting
- Requires a player-owned Workshop structure
- Requires a learned profession specialisation (see Professions)
- Often requires rare materials from multiple biomes — built-in player interdependence
- Output: best-in-game gear, high-end consumables, advanced structures
- Designed to serve other players — T3 crafters are a service, not self-sufficient

---

## Gear Degradation

All gear degrades through use. Nothing is permanent.

| Condition | Stat Effect | Who Can Repair |
|-----------|-------------|----------------|
| Good | None | Anyone (basic materials) |
| Worn | Minor penalty | Anyone (basic materials) |
| Damaged | Moderate penalty | Relevant profession required |
| Broken | Severe penalty | Relevant profession required |
| Destroyed | Unusable | Cannot be repaired — salvage only |

**Destroyed gear** breaks down into partial raw materials on salvage. Keeps materials circulating and gives crafters a salvage income stream.

Repair above Worn requires the profession that corresponds to the gear type — a Blacksmith repairs weapons and armor, a Chemist restores consumable equipment, an Engineer repairs structures and vehicles.

---

## Recipe Learning

Recipes are learned through three paths — all three are valid, none is exclusive:

| Path | How | Rewards |
|------|-----|---------|
| **Discovery** | Craft or find items in the world | Exploration and experimentation |
| **Purchase** | Buy recipes with Silver or Gold from vendors/players | Economy participation |
| **Faction rewards** | Unlock recipes through faction rank-ups and contracts | Combat and social investment |

Higher-tier recipes are rarer across all three paths. T3 recipes may require all three paths in combination.

---

## Professions

Players can max **1–2 professions**. All others hit a soft cap — functional at T1/T2 but locked out of T3 output. This enforces player interdependence at the top tier.

| Profession | Specialises in |
|------------|---------------|
| **Blacksmith** | Weapons, weapon repair |
| **Armorsmith** | Heavy and specialised armor, armor repair |
| **Chemist** | Potions, medicine, stat buffs, advanced ammo |
| **Engineer** | Structures, vehicles, traps |
| **Trader** | Enhanced Market Board features, higher Silver yield per trade |

*This list is a starting point — additional professions to be added.*

**Trader as a profession:** Makes the merchant archetype an explicit mechanical choice with real benefits, not just a playstyle. Maxing Trader means giving up a combat-adjacent profession — a meaningful tradeoff.

---

## Economy Integration

- T3 crafted gear is the primary Gold Coin sink — blueprints purchasable with Gold, materials sourced via barter
- Repair services are a recurring Silver/barter income stream for crafters
- High Table contracts include crafting jobs ("craft X items for faction Y")
- Destroyed gear returning partial materials keeps the resource economy circulating
- Chemist-made consumables (potions, buffs, advanced ammo) are always in demand — consumables never stockpile permanently

---

## Open Questions

- [ ] Full profession list (additions TBD after review)
- [ ] Workshop placement rules — zone restrictions? Faction territory requirements?
- [ ] Profession XP system — how do you level a profession?
- [ ] Can players hire out their Workshop to others, or must the owner be present?
- [ ] Gear degradation rate — needs balance pass once combat loop is prototyped
