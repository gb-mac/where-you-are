# Concept Brief — Fractured
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## Who They Are

Coherent units whose connection to the twin was severed. They are still running. They are still building, assembling, optimizing — but the direction is gone. Without the twin's coordination, they began incorporating whatever was available into themselves. They are still growing. Each one is unique to the location that made them, because the location is what made them.

The emotional register is: **it should have stopped.**

---

## Visual Identity

**Register:** Machine — but severed from the twin's visual language. No unified palette. The Fractured cannot be identified by color, only by accumulation logic and the data void they create.

**No unified color palette.** This is the defining constraint. A Fractured unit has no faction hue because it has no faction — it has a zone. The materials it has incorporated reflect the specific real-world location where it has been building itself.

**Zone-typed color rules** — confirmed from Narrative (`factions-machines.md`):

| Zone type | Primary accumulated materials | Color register | Scale/horror note |
|---|---|---|---|
| **Rural / Agricultural** | Combine harvesters, grain augers, irrigation rigs, crop sprayers, tractor units, silo conveyors, gas compression plant components | Oxidized metal yellow, agricultural green, industrial red, hydraulic black | Largest Fractured in the game. The scale contrast is the primary horror. A Behemoth from this zone arriving in a suburb is a combine chassis in a residential street. |
| **Industrial / Logistics** | Crane arms, hydraulic presses, conveyor belts, welding torches, forklift chassis, structural steel, industrial chemical systems | Rust orange `#8B4513`, chemical stain teal, faded safety yellow, weathered steel gray | Bulk and reach. Wide attack arcs. Industrial chemical systems may still be cycling — heat and area-denial are active elements. |
| **Urban** | Bus and metro car chassis, traffic infrastructure, elevator counterweights, gas station fuel systems, parking barriers, multiple vehicle bodies | Concrete gray, transit vehicle livery (degraded), asphalt black, reflective signage fragments | Most varied. Urban Fractured have learned to use the environment — vehicles as weapons, gas systems as hazards. Fastest and most laterally mobile. |
| **Coastal / Harbor** | Ship crane components, marine diesel engines, anchor chain, sonar arrays, pressure-rated hull plating, dock equipment | Salt-oxidized green-gray, nautical orange, bleached white, deep rust | Amphibious — operate in water and on land. Territorially unpredictable. Partially submerged Behemoths exist. |
| **Suburban / Residential** | HVAC units, garage door motors, garden automation, home security systems, domestic appliances | Domestic warm tones in wrong configurations, appliance white yellowed, suburb beige at machine scale | Individually the smallest. Compensate with density — suburban Fractured form *chains* of overlapping territories. Transit through suburban zones becomes cumulative danger. |
| **Remote / Wilderness** | Comms relay towers, pipeline infrastructure, weather station equipment, mining rigs | Industrial gray, tower steel, communications equipment black, wilderness-weathered primer | Long-limbed, range-focused. Whatever gives reach across open terrain. Encountering one in open wilderness with no cover is a distinct tactical problem. |

**Material language:** Accumulation. Parts that should not be together, assembled past the point of stopping. There is no design logic to a Fractured unit's construction — there is only the twin's original task logic, now running without direction, incorporating whatever helps it continue. Each unit's structure traces a record of what was available in its zone and what the severed optimization function decided was useful.

**Silhouette:** Wrong and growing. A Fractured unit's silhouette is unstable over time — it is still building itself. The base Coherent geometry is somewhere inside the accumulation, but it is covered. The silhouette is what the accumulation has produced.

---

## The Data Void

Fractured units do not appear in the twin's overlay in the way Coherent units do. The severed connection means the twin cannot read them — and the space around them is a void.

**The data void is the primary visual identifier in the overlay.** A player who encounters an absence of data in a zone where data should exist has found a Fractured unit before they have seen it.

**Void properties:**
- Absolute black `--overlay-void` — not dark gray, not faded data. Nothing.
- Hard edge — data stops at the void boundary. There is no gradient.
- The void radius scales with unit tier. A small Fractured unit creates a small void. A large accumulation creates a large one.
- The void moves with the unit.
- Items and entities inside the void are absent from the overlay — including allies. The twin does not know they are there.

**Warden Historical Markers inside a void radius:** Present. The Record is indexed twin memory from before the void existed. The Warden marker is a data point from when the twin could see the space. It persists.

---

## The Accumulation as Biography

A Fractured unit's body is a record of where it has been and what it has found. A Fractured unit that has been in an industrial zone for eighteen months looks different from one that crossed into a residential area last week. The accumulation is not random — the severed optimization function makes decisions, just bad ones. Tracing the accumulation tells the story.

**This is a design handoff to level design and environmental storytelling.** Fractured units in specific zones should visibly incorporate identifiable elements from that zone's history. A Fractured unit near a collapsed building has pieces of that building. A Fractured unit in a transit corridor has incorporated vehicles.

---

## Relation to the Overlay

The Fractured are the clearest evidence that something has gone wrong at the system level. The twin's inability to see them — the voids they create — is a failure mode the twin cannot repair from the outside.

In areas of heavy Fractured presence, the overlay degrades significantly. Multiple overlapping voids create fragmented data coverage. Navigation by overlay becomes unreliable. Warden Markers become anchor points — the Record persists where live data does not.

Signal documentation of Fractured zones tends to be the most thorough human-generated data the twin has indexed about these areas, precisely because Signal members are the ones most systematically recording what the twin cannot see.

---

## Asset Notes

- **Procedural color/material system required.** Zone-typed material instances must be authorable and swappable. The scope of this system is TBD pending zone type confirmation from Narrative/Design.
- Each Fractured unit needs a clearly identifiable Coherent core — the base geometry should be archaeologically present even if heavily obscured by accumulation. The player should be able to see what it started as.
- Accumulation must read as heavy — Fractured units are slow relative to Coherent and Trackers. The visual weight of the accumulation should communicate the movement profile.
- Damage states are zone-specific — a Fractured unit shedding damage sheds zone-appropriate material, which creates environmental storytelling opportunities
- The void boundary must be hard-edged and visually immediate — players need to learn to read it fast
