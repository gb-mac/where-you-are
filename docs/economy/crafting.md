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
- Requires a T3 blueprint (purchased with Gold — see below)
- Requires Maker peer standing to execute at full T3 quality (see Maker Gate below)
- Often requires rare materials from multiple biomes — built-in player interdependence
- Output: best-in-game gear, high-end consumables, advanced structures
- Designed to serve other players — T3 crafters are a service, not self-sufficient

#### The T3 Blueprint and Maker Gate

T3 blueprints are purchased with Gold from High Table vendors or earned through faction rank-ups. A blueprint is the pattern and specifications — it tells you what to build.

Executing a T3 blueprint at full quality requires **Maker peer standing** — a recognized relationship with a Warden Maker (Armorer, Tailor, Gunsmith, or Bladesmith) who has observed and acknowledged the crafter's work. The Maker's acknowledgment is not a formal certification; it is a relationship that exists between two people who understand the same craft. See `docs/narrative/warden-makers.md` and `docs/narrative/arc-tradesperson.md`.

**Without Maker standing:** A T3 blueprint can be executed, but output quality is capped at T2.5 — better than standard T2 but not full T3. The crafter can read the instructions; they don't fully understand them yet.

**With Maker standing:** Blueprint unlocks its full specification. The Maker's knowledge fills the gaps between what the blueprint says and what the work actually requires. Full T3 output.

**Why this works narratively:** Gold buys the document. Relationship earns the understanding. These are different things. A T3 blueprint for Warden-grade plate armor is a technical specification — it tells you the materials, the geometry, the tolerances. What it doesn't tell you is what the Armorer knows from having outfitted Vanguard for Behemoth encounters: which stress points fail first, what the specific failure mode looks like, how to build the reinforcement so it holds under that specific load. That knowledge is in the Maker's hands. You get it by being seen.

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

## Workshop Placement Rules

Workshops are player-placed structures. Placement is unrestricted by zone — you can build a Workshop anywhere you control territory. However, **biome affects output quality** and **territory affects access and risk**.

### Biome Affinity

Each profession has a primary biome where its Workshop produces at maximum efficiency. Outside the affinity biome, T3 output is possible but material consumption is higher (you're working against the grain of the environment).

| Profession | Affinity Biome | Why |
|------------|---------------|-----|
| Blacksmith | Industrial | Heavy forging infrastructure, metal supply |
| Armorsmith | Industrial | Same as Blacksmith — shared infrastructure |
| Chemist | Rural/Green or Water | Organic precursor proximity |
| Engineer | Industrial or Transit | Tools, components, logistics infrastructure |
| Trader | Urban dense | Market density, foot traffic |

A Chemist Workshop in an urban zone works — it just costs more precursors per batch. A Trader Workshop in a rural zone functions but draws less organic Market Board traffic.

### Territory and Faction Effects

| Workshop location | Effect |
|------------------|--------|
| Warden estate or Warden-adjacent territory | Maker network access — enables full T3 with Maker standing |
| Hollow Syndicate territory | Shadow Market Board access, Compact raid risk |
| Compact-controlled zone | Protected from raids, but Compact may levy a "tax" (standing cost) |
| Untenanted/contested | No faction benefits, no faction risk — pure independence |
| Inside a High Table Zone | Safe from PvP, Silver overhead for zone access |

### Can Players Hire Out Their Workshop?

Yes. A Workshop owner can authorize specific players to use their station. Two modes:

**Private access** — owner designates specific accounts. Authorized players pay the owner a negotiated rate (Silver, items, or barter). Owner takes their cut; the Workshop serves as passive income infrastructure.

**Open station** — owner marks the Workshop as publicly accessible. Anyone can use it for a Silver fee the owner sets. Higher income potential, higher risk — strangers knowing your Workshop location creates a target.

The hire-out mechanic makes T3 crafters who don't own Workshops viable: they pay for access rather than building their own. It also makes Workshop ownership a meaningful investment beyond personal use.

Workshop ownership and access rules are enforced by social contract and reputation, not by server lock. A Workshop owner who charges and doesn't deliver access gets a reputation hit. A player who uses an open Workshop and then raids the location is violating Warden code if they're Sworn — and the Record notes it.

---

## Profession Progression

### T1 → T2: Activity-Based XP

Craft items → gain Profession XP → unlock T2 recipes and efficiency bonuses. This is straightforward: you get better by doing the work. No time gate, no quest requirement.

XP required for T2 unlock is calibrated so that a player actively using their profession for 2–3 weeks of real play reaches T2 naturally. Players who only casually craft take longer.

### T2 → T3: Instruction Required

The jump to T3 cannot be done through XP accumulation alone. It requires an **instruction event** — a specific moment where someone who already knows the technique teaches it.

**Sources of T3 instruction:**
- **Maker peer standing** (primary) — a Warden Maker observes your T2 work and, at the right moment, demonstrates the technique. This is the Tradesperson arc path but available to any profession background with sufficient Maker relationship.
- **Faction questline unlock** — some factions offer T3 instruction as a reward for specific questline completion (a Signal chemist teaching advanced synthesis, a Compact engineer sharing military-grade construction technique).
- **Another T3 crafter** — a player who already has T3 can teach it. This costs the teacher something (a specific resource, a favor, an NPC interaction to validate the transfer). Player-to-player T3 instruction is the rarest path and the most narratively interesting one.

**Why this gate:** T3 represents craft knowledge that isn't in any surviving manual. It's held in people. The jump from T2 to T3 is not a skill grind — it's finding the right person and building enough relationship that they share what they know. This is consistent with the post-Cascade world's information economy and prevents T3 from being purely time-gated.

---

## Open Questions

- [ ] Full profession list (additions TBD after review)
- [ ] Gear degradation rate — needs balance pass once combat loop is prototyped (Core shipped `380dc26`; calibration can begin)
