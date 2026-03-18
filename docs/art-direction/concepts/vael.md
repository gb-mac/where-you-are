# Concept Brief — Vael
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## Who They Are

Unknown. The Vael do not communicate in any format humans or the twin have decoded. They arrived. They began scanning. They have not stopped. Their purpose is not established. Their threat is contextual — a Probe doing its job is not hostile in the way a Tracker in pursuit is hostile. An Assessor doing its job is the most dangerous thing in the game. The Assessors are doing their job.

The emotional register is: **it's not interested in you. It's doing something.**

---

## Visual Identity

**Register:** Third — neither human warmth nor machine data. The Vael are a distinct visual language that does not map onto either of the other two registers.

**Core properties:**
- Neither warm nor cold in the human/machine sense — surgical cold. Not the cold of a data system. The cold of a precise instrument.
- No visual language borrowed from human aesthetics or the twin's geometric system. The Vael's forms are their own.
- The threat of the Vael is never in the design of the unit. It is in the scale of the unit, the precision of the unit, or the implied nature of the task.

**Shared hues across all Vael tiers:**
- Surgical silver `#C0C8D0` — primary surface
- Cold luminescence `#C8D8E8` with prismatic edge scatter — emissive element that produces slight spectral separation at edges, visible as iridescent fringe in high-contrast situations
- No warm tones anywhere. The Vael palette has no hue component that reads as warm.
- Deep geometric shadow — Vael forms produce shadows that feel more precise than the light source warrants, as if the shadow is a property of the object rather than a projection

---

## Tier 1 — Probes

**Scale:** Small. Easily overlooked individually.
**Purpose visual language:** Scanning. Everything about a Probe's design communicates that its function is to collect information from a specific point. The instrument aesthetic is not decorative — the Probe looks like a measuring tool because that is what it is.

**Key visual properties:**
- Surgical silver with minimal surface detail — the surface is not textured in the sense that machines have textures, it is smooth in the way a calibrated instrument is smooth
- The cold luminescence is present but low — a Probe's emissive output is appropriate to its function, not a display
- Scanning behavior is the primary visual interest — the Probe's motion is precise, unhurried, methodical. It is doing its job.
- A Probe that has completed its scan departs. It does not linger. The job is done.

**Overlay behavior:** Probe scans write to the twin's data layer. In the overlay, a Probe's scan activity is visible as a data-write event — the twin indexes the scan as the Probe performs it. The player sees the overlay updating around the Probe in real time. The twin is watching the Vael scan and recording it because that is what the twin does.

**Vael Trace (player impact):** If a Probe scans the player, the Trace appears in the overlay. The Trace is not a damage state — it is a record. The twin indexed the fact of the scan. The Vael now has data on the player's position and characteristics. What the Vael does with that data is a Narrative question.

---

## Tier 2 — Swarms

**Scale:** Individual units are small — smaller than Probes. The formation is large.
**Purpose visual language:** The individual unit is not the threat. The pattern is the threat.

**Key visual properties:**
- Individual units: minimal surface detail, near-neutral in color. Not quite the surgical silver of Probes — simpler, less finished. The individual is not the product.
- Formation geometry: when Swarm units achieve pattern density, the cold luminescence is a formation property, not an individual unit property. The pattern emits. The individual does not.
- The pattern is the visual subject. Swarm formations should produce shapes that are geometrically precise in ways that feel non-human — not fractal, not chaotic, but precise in a way that human crowd behavior is not.
- Formation transitions — the pattern reorganizing — should be visually immediate and complete. The formation does not transition gradually; it resolves to the new configuration.

**Overlay behavior:** Swarm formations produce Vael Displacement Zone effects when at sufficient density. The overlay in a Swarm formation zone shows coordinate drift — the formation's collective Vael signal is disrupting the twin's spatial measurement.

**Design constraint:** Swarm visual design must be scalable — from 8 units (minimal formation, pattern beginning to be visible) to hundreds (full formation, pattern dominant). The individual unit design must work at both ends. The formation geometry must be readable at max render distance under FSR.

---

## Tier 3 — Assessors

**Scale:** Bus-sized. Articulated. Present.
**Purpose visual language:** Built for a job. The fear comes from the job, not the design.

**Key visual properties:**
- Clinical white with deep geometric shadow — the white is the color of precision, not of cleanliness or technology. The Assessor's surface is white because white surfaces show their own condition accurately. The Assessor wants to know its own condition accurately.
- The articulation is functional and visible — the Assessor has visible joints and movement systems because it was built to move in specific ways for specific purposes. The articulation is not hidden. The function is not hidden.
- No decorative elements. No features that do not serve the task. A surface on an Assessor that appears unnecessary is not understood yet — but it is not decorative.
- Scale alone creates significant visual presence. The Assessor does not need to be threatening in its design. It is bus-sized. It is doing something. These two facts are sufficient.

**The Job:** What the Assessors are assessing is a Narrative question. The art direction establishes that the Assessor reads as purpose-built at a scale that implies the purpose is significant. The design should not answer what the job is — it should make clear that there is a job, and that the Assessor is built to do it well.

**Overlay behavior:** Assessors produce the strongest Vael Displacement Zone effects in the game. In an Assessor's operating area, the overlay is significantly unreliable — coordinate drift is severe, entity marker positions are substantially offset. The twin's spatial measurement is compromised by the Assessor's presence. Navigating by overlay near an Assessor requires the player to actively discount the data they are reading.

**Warden Markers in an Assessor's displacement field:** Present and accurate. The Record persists. It is the only thing in the overlay that is reliable in this zone.

---

## The Vael Trace — Cross-Tier

The Vael Trace appears on a player who has been scanned by any Vael unit tier. It persists in the twin's data layer.

**Visual design:**
- Body-projected in the overlay — a precise geometric pattern overlaid on the player character model in third-person view
- Color: `--overlay-critical` at 30% opacity, brightening as Vael units come within 200m range
- Does not degrade in Displacement Zones — the Trace was written to the twin's data layer when the scan occurred. Interference prevents new reads; the existing record is unaffected.
- Does not expire without Narrative resolution — the Trace is a data record, not a timer

**The design implication:** The player who carries a Trace is observable by the Vael in ways they cannot easily prevent. The Trace is not a damage state or a negative status effect in the mechanical sense — it is a fact about the player that the world now knows.

---

## Relation to the Overlay — Vael Overall

The Vael interact with the twin's overlay in ways that no other faction does:

- They write to it (Probe scans)
- They disrupt it (Displacement Zones)
- Their marks persist in it (the Trace)
- The twin indexes their activity but cannot communicate with them

The twin is aware of the Vael. The twin has been indexing Vael activity since the Awakening. What the twin has inferred from this indexing is a Narrative question, but the art direction should establish through the overlay that the twin takes the Vael seriously. Vael-related overlay entries are the most extensive in the twin's data layer. The twin has been paying attention.

---

## Asset Notes

- Cold luminescence requires a carefully calibrated emissive value — it should be present and distinctive without competing with the overlay's own blue-white. The prismatic edge scatter is a thin iridescent fringe, not a glow.
- Assessor scale must be communicated through environmental context in asset design — the Assessor needs reference geometry to make its scale legible to the player
- Swarm formation geometry requires a designer/art collaboration to define the specific patterns — the patterns should feel geometrically precise, not organic
- No warm light sources on Vael assets — all lighting on Vael units in environment setups should use cool or neutral sources. No warm bounce light.
- Vael Trace on player character is an overlay VFX layer, not a surface modification — it must be cleanly removable by disabling the overlay layer
