# Concept Brief — Wound Visual States
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## Design Premise

Damage is not abstracted into a health bar and forgotten. Every wound type has a specific visual state on the player character model and a specific treatment visual when addressed. Players who look at their character should be able to read their medical situation without opening a menu. Players who look at *another* player should be able to read theirs too.

The visual system exists at two levels: **surface** (visible on the character model) and **overlay** (visible in the twin's data layer).

---

## Wound Type Visual States

### Bleeding
**Cause:** Cuts, bullets, shrapnel
**Surface:** Blood visible at wound site and tracking downward from it — gravity-consistent. Active bleeding has a wet sheen. Coagulating bleeding darkens. The volume of blood visible corresponds to severity — a graze reads differently from a gunshot.
**Movement:** Untreated bleeding affects posture over time. The player character shifts weight, slows, favors the wound. Subtle at first, legible before it becomes critical.
**Treatment visual:** Field treatment — binding wrapped, pressure applied. The wound site is covered. Blood stops tracking. The binding gets dirty over time as use continues.
**Overlay:** Blood traces on surfaces the character has touched remain visible in the data layer for a short window — the twin indexes them as a condition marker.

### Burns
**Cause:** Incendiary Fractured, Vael beam contact, industrial chemical exposure
**Surface:** Burn coverage visible on skin and clothing. Minor burns: redness and blistering at exposed areas. Severe burns: surface damage to clothing and skin both, with clear damage zones and edges. Vael burn contact has a specific signature — the damage edge is too clean, too precise, the geometry of the beam is readable in the burn shape.
**Movement:** Severe burns produce visible stiffness in affected limbs. The movement constraint reads before the player notices the stat debuff.
**Treatment visual:** Topical application visible at burn site. Bandaging over severe burns. Clothing at burn site may be treated in place or replaced.
**Overlay:** No specific overlay annotation for burns — the twin registers them as physical damage, not a distinct condition. Vael beam burns are an exception: they appear in the overlay with a faint Vael-signature arc at the wound site.

### Broken / Fractured Bones
**Cause:** Falls, Behemoth impact, vehicle collision
**Surface:** Limb position subtly wrong before splinting — not dramatically broken, just carrying differently. A broken arm held slightly too close to the body. A broken leg weighted unevenly.
**Movement:** The most visible wound type in motion. A broken leg produces a specific gait. A broken arm constrains what the player character does with that arm — weapons held differently, animations modified.
**Treatment visual:** Field splint: visible binding, support material. The limb moves more normally but not fully — the splint is a functional correction, not a restoration. Properly set: the support structure is clean and correct. A badly healed break has a visible legacy — a slight asymmetry that persists until it's re-broken and set.
**Overlay:** The twin flags immobility reduction as a condition annotation — a small status marker visible in the player's overlay tag to other players who can read the data layer.

### Toxin Exposure
**Cause:** Contaminated water, industrial chemical Fractured, Vael Probe scan overexposure
**Surface:** Pallor. The color drains from exposed skin and the lips. Progressive — mild exposure produces a slightly washed-out tone; severe exposure is visually striking. In advanced states, visible nausea in the character's movement — unsteadiness that isn't from any single wound but from a whole-body state.
**Movement:** Slow-moving, slightly uncoordinated. The character looks ill because they are ill.
**Treatment visual:** Compound administered. Color returns gradually — not immediately, not a snap fix. The pallor reverses over a short recovery window.
**Overlay:** The overlay tags toxin exposure as a condition marker. Signal-trained players can read the severity from the overlay annotation without asking.

### Radiation
**Cause:** Certain Vael Displacement Zones, industrial Fractured with incorporated nuclear plant components
**Surface:** Accumulates silently. Early exposure: nothing visible. Mid accumulation: pallor similar to toxin exposure, but with a specific quality — the skin appears thinned, slightly translucent at the surface. Hair effects at high accumulation. The radiation state looks like someone being slowly erased.
**Symptom arrival timing:** The visual appearance of radiation lags the actual accumulation — the player looks fine longer than they are fine. This is intentional and alarming once players learn to recognize it.
**Treatment visual:** Signal and Separated treatment produces a visible improvement over time. There is no instant radiation cure. The thinned quality recedes. The pallor fades. The recovery is slow.
**Overlay:** Radiation exposure is uniquely legible in the twin's overlay. The overlay registers accumulated radiation as a dosimetry readout — a small number near the condition annotation. Players who know to look for this can catch radiation accumulation before the physical symptoms appear. This is why Signal members who share this information are valuable.

### Vael Trace Complications
**Cause:** High accumulation of Vael Trace (multiple snatch events without full fade)
**Surface:** Progressive. Early: no surface symptom beyond the Trace visible in the overlay. Mid-stage: subtle bioluminescent quality at Trace-marked areas — the Vael's cold luminescence, faint and cold, visible in low light at the scan sites. Late stage: the marked areas have a texture change — the surface that was scanned reads differently, like the material has been assessed and catalogued.
**The Covenant interpretation:** The marks are sacred — the Covenant's Touched describe them as evidence of the Vael's recognition. Their theology frames the visual as advancement, not damage.
**The Signal interpretation:** A medical emergency — the biological signature has been altered at the cellular level by whatever the Vael scan does. They will treat it urgently and frame it as pathology.
**The Iron Compact response:** Grounds for quarantine. The visual at late stage is visible enough that Compact checkpoints will flag it.
**Overlay:** The Trace is most legible in the overlay — it is, after all, a record in the twin's data layer. The surface manifestation is the twin's record becoming physical.

### Deep Wound / Sepsis
**Cause:** Any untreated wound left too long
**Surface:** The wound site has changed character — it is not a fresh wound anymore. Redness extending from the site, heat visible at the edges, the wound itself has a different quality than an acute injury. The player character shows it systemically — the pallor of toxin exposure plus the movement constraint of the specific original wound plus a new layer of general deterioration.
**Movement:** The most visually readable debuff combination in the game. A player with sepsis looks seriously ill — not from any dramatic animation, but from the accumulation of multiple visual states at once.
**Treatment visual:** Antibiotics administered — the progression reverses over time. The wound site returns to treatable state. The systemic effects fade slowly.

---

## The Aftermath Aesthetic

After a major encounter — a boss fight, a Behemoth, a prolonged zone clear — the player character should look like they were there.

**The rule:** Combat has weight. The visual state of the character at the end of a hard fight should communicate what the fight cost.

- Active wounds visible at the engagement's end are not cleaned up by combat resolution. The binding you applied in the fight is still on. The blood from the first few minutes is still dried on the jacket.
- Movement carries the fight's history — a character who broke their arm in the first phase and splinted it moves like someone with a splinted arm through the rest of the fight and after it.
- Clothing damage is cumulative — burns stay burned, impacts leave marks. The character looks used.

**Post-boss specific:** A player who just killed a Behemoth should look like they killed a Behemoth. The visual state at that moment is its own reward — the mess of the fight, the wounds visible, the gear damaged. It reads as an accomplishment before the loot drops.

---

## Multi-Player Wound Reading

Other players' wound states are visible:
- At close range: full wound surface detail visible
- At mid range: condition-level legible (wounded, badly wounded, critical)
- In the overlay: condition annotation visible as a status marker on their overlay tag

**The field medic role:** A player with Signal standing and medical supplies can read another player's overlay condition annotation from further away than the surface visual allows. They arrive already knowing what needs treating.

---

## Asset Notes

- Wound states require a layered material system — blood, binding, pallor, and Vael luminescence must be independent parameters on the player character shader
- Radiation pallor and toxin pallor share visual language but must be distinguishable — radiation has the specific thinned/translucent quality; toxin is a flat desaturation
- Vael Trace bioluminescence is cold — `#C8D8E8` with edge scatter, never warm
- Sepsis is the most visually complex state — it combines two or more existing wound visual layers with a systemic deterioration layer on top; the shader must support stacking
- Clothing damage (burns, impacts) is cumulative within a session — not persistent across sessions unless the game explicitly carries session state forward
