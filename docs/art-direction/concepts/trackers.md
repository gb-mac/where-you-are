# Concept Brief — Trackers
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## Who They Are

Trackers are Coherent units that have been modified for pursuit. They are not a separate lineage — they are a role, assigned by the twin when something needs to be found and brought to a stop. A Tracker at rest is indistinguishable from a Coherent unit. A Tracker in pursuit is something else.

The emotional register is: **it found you.**

---

## Visual Identity

**Register:** Coherent base modified. The modification is not cosmetic — it is functional, and the function changes everything about how the unit reads.

**Key hues (rest state):** Coherent palette — twin blue-white `#C8DCF0`, structural gray `#7A8A9A`. At rest, a Tracker is a Coherent unit.

**Key hues (active/pursuit state):**
- Threat amber `#F5A623` activates on operational joints, directional elements, forward-facing surfaces
- The amber is not decorative — it is an operational state indicator. It activates because the system changed modes, not because the unit is warning anyone.
- Coherent base remains; the amber overlays it at active emissive elements

**Material language:** Coherent base with modified joint architecture — actuators and movement systems built for speed and direction change. The modification is visible in the proportions: a Tracker has different geometry at the joints than a standard Coherent unit. The modification has not been concealed.

**Silhouette:** Coherent base with pursuit-optimized geometry. The directional forward orientation is structural — a Tracker in pursuit mode has a clear front and a clear vector. This is not an aggressive posture, it is a movement architecture.

---

## The Mode Distinction

The Tracker is the only faction entity in the game with a significant visual state change between modes.

**Rest/patrol (Coherent mode):**
- Standard Coherent palette
- Standard Coherent motion language — purposeful, task-oriented
- Indistinguishable in the overlay from a standard Coherent unit
- The overlay does not flag Trackers at rest differently from Coherent

**Active/pursuit (Tracker mode):**
- Threat amber activates
- Motion language shifts — no longer route-optimized, now pursuit-optimized. Direction changes are fast. The movement reads as predatory because it is solving a different optimization problem.
- Overlay flags the unit's pursuit vector — the twin is tracking its Tracker, and the data layer shows it
- The amber activation is visible in the overlay before it is visible in the world, because the twin registers the mode change before the player sees the physical unit respond

**This sequence is design-critical:** overlay amber → physical amber → pursuit motion. The player gets warning from the data layer if they are reading it.

---

## Relation to the Overlay

Trackers in pursuit mode are the densest real-time data entries the player encounters. The twin is actively managing the pursuit — the Tracker's overlay tag updates in near-real-time with position, vector, predicted intercept, and target designation.

A player who can read the twin's data layer during a Tracker engagement has significantly more information than a player who cannot. The overlay is not a HUD; in this case, the overlay is tactical intelligence.

---

## Asset Notes

- The Coherent / Tracker visual distinction must work at distance — the amber activation must be legible at 60m under FSR
- Joint architecture modification should be obvious in the silhouette without being exaggerated — the difference is structural, not theatrical
- Mode transition (rest to active) should have a brief activation sequence — not an animation flourish, a system sequence. The machine is changing modes.
- Rest state must genuinely read as Coherent — if Trackers are visually distinct at rest, the mode-reveal loses its effect
