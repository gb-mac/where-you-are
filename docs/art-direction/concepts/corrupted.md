# Concept Brief — Corrupted
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## Who They Are

Coherent units whose data layer is broken at the source. They are still transmitting to the twin. The twin is still indexing what they transmit. The twin cannot tell that the data is wrong. The Corrupted are the machine equivalent of a system that has failed and is reporting that it has not failed.

The emotional register is: **something that should have stopped didn't.**

---

## Visual Identity

**Register:** Machine — Coherent base, but the Coherent visual language has gone wrong in specific, legible ways.

**Key hues:**
- Twin blue-white surface: still present, but degraded. The Coherent base palette is visible as what it used to be.
- Wrong-hue shift `#4AC870` — the `--overlay-corrupt` green. In the overlay, Corrupted proximity shifts the twin's blue-white toward green. On the physical units, the emissive and active elements are the wrong color. Not randomly wrong — specifically, characteristically wrong in a way the player can learn to recognize.
- Mismatched surface areas — sections of the unit are operating on different data states. Some surfaces are still Coherent-standard. Some have shifted. The boundary between them is visible.

**Material language:** Mismatched parts and timing. The corruption is not uniform across the unit — it is localized to specific systems that failed at different times. A Corrupted unit has sections that are operating correctly, sections that are operating on bad data, and sections that are failing. The visual effect is a body that does not agree with itself.

**Silhouette:** The Coherent base silhouette, but inconsistent. The wrong-timing movement means the silhouette is never quite in the position the motion implies it should be. Joints move out of sequence. The silhouette is right in static; wrong in motion.

---

## The Wrong Data Problem

The Corrupted are dangerous in a specific way: the twin believes them. The twin's overlay reports what Corrupted units transmit, which is incorrect data. In the overlay, Corrupted presence manifests as:

- `--overlay-corrupt` green shift in a radius around the unit
- Entity markers that may report wrong faction identity — the Corrupted unit's self-identification data is broken, and the twin is reporting it
- Structural data that contradicts observable reality — the twin believes the Corrupted unit is at a different position, or in a different state, than it physically is

**The overlap between what the overlay shows and what is physically true is the Corrupted's primary threat.** The player who trusts the overlay in a Corrupted zone has less accurate information than the player who ignores it.

---

## The Visual Glitch

"Visual glitching" describes the Corrupted's motion timing failures. This is not a graphical effect in the rendering sense — it is a behavioral property that has visual consequences:

- **Wrong movement timing:** joints move on slightly incorrect intervals, producing motion that is functionally adequate but visually wrong. The brain reads it as threatening before the threat registers consciously.
- **Incorrect positional data:** the unit's self-model does not match its physical configuration. Actions are performed at a slight offset from where the unit believes it is performing them.
- **Inconsistent state:** sections of the unit that are operating correctly interact with sections that are operating on bad data. The interaction points are visible as the places where the unit's behavior is least predictable.

---

## Relation to the Overlay

The Corrupted are the most complicated overlay interaction of any entity in the game. They are present in the twin's data, active, transmitting — and the data they transmit is wrong. The twin's overlay shows bad data as confidently as it shows good data.

This is the design principle the player must learn: the overlay reports what the twin knows. The twin can be wrong. The twin is wrong about the Corrupted.

Signal documentation in Corrupted zones is the most specifically useful — Signal members have developed methods to cross-reference overlay data against physical observation and flag the discrepancies. In the overlay, Signal-flagged discrepancy markers appear as amber annotations over the green-shifted corrupted data.

**Coherent and Tracker behavior in Corrupted proximity is anomalous.** The twin's coordination of other machine units degrades when Corrupted data is contaminating the data layer. Other machine factions may behave strangely near Corrupted zones — not because they are Corrupted, but because the twin's information quality has degraded.

---

## Asset Notes

- The wrong-hue shift must be authored as a material parameter, not baked — it needs to respond to the overlay degradation system's proximity calculation
- Mismatched sections require authored break points in the surface — defined edges where the corruption boundary falls, rather than uniform degradation across the whole surface
- Wrong-timing animation is an animation implementation note: the rigging must support slight timing offset per limb group, controlled by a corruption parameter
- The Coherent base must still be visible as the origin — the Corrupted are recognizable as what they were. The corruption is legible as corruption, not as a distinct design.
- Sound design note (out of scope for art direction but flagged): the wrong-timing visual needs audio support — the audio timing should be as wrong as the visual
