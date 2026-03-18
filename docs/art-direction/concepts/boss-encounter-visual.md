# Concept Brief — Boss Encounter Visual Design
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## Design Premise

A boss fight is a story. The visual system must tell it — the opening read, the damage that accumulates, the phase transitions, and the aftermath. Players who just beat a boss should look like they beat a boss.

Two visual layers run simultaneously:
1. **The boss** — cumulative damage visible in the physical form, phase transitions that change what the player is reading
2. **The player** — wound states accumulating, gear showing the fight's history

---

## Boss Damage — Cumulative and Specific

**The rule:** Every significant damage event is visible in the boss's form for the rest of the fight. Damage does not disappear. Damage changes the encounter.

### What "Cumulative and Specific" Means

*Incorrect:* Hit points deplete → boss plays a damage flash animation → boss continues unchanged

*Correct:* A significant damage event destroys or disables a specific component → the boss's silhouette changes → its behavior changes → the rest of the fight is different from how it started

**Examples by faction:**

**Fractured Behemoth (primary boss design case):**
- Destroying an accumulated arm removes that arm from the boss's attack repertoire. The stump is visible. If that arm was providing reach, the boss's effective range has changed. The Behemoth compensates by using its remaining mass differently — the compensation is behavioral and visible.
- Damaging the original Coherent chassis core (buried inside the accumulation) produces a different response than damaging the accumulated exterior. The core has different movement consequences — damaging it affects the base behavior, not just the additions.
- Penetrating the outer accumulation exposes the chassis inside. The exposed chassis is a new vulnerability — and the player can now see what the Coherent unit was before it became this.

**Iron Compact boss (named NPC encounter):**
- Armor damaged in specific areas shows progressive degradation — the Compact's maintenance ethos means their armor fails systematically, not randomly. A plate blown off exposes the underlayer. The underlayer fails differently.
- Injury to the NPC visible in movement — a damaged leg changes gait. The boss adapts their tactics to compensate. Players read the adaptation.

**Vael Assessor encounter:**
- Component damage changes the Assessor's articulation — a disabled arm segment means the job the Assessor is doing is incomplete. The Assessor attempts to complete the task with remaining components. The attempt is visible and changes the geometry of the encounter.
- The Assessor does not perform pain or rage. It reconfigures. The phase transitions are reconfiguration events.

### Phase Transition Visual Language

Phase transitions are triggered by events, not health thresholds. The visual marks the transition:

**Mechanical reconfiguration:** The boss's form changes in response to the fight's events. A Behemoth that has lost exterior accumulation is operating closer to its core chassis. A reconfigured Assessor is operating with reduced component set. The transition is not a cutscene — it is the fight's physical state reflecting the fight's history.

**Behavioral shift:** Phase 2 is not phase 1 with more health. The boss is doing something different because the fight has changed what it can do, or because its assessment of the situation has changed. The visual cue is behavioral — the player reads that they are now in a different fight.

**Overlay transition:** Boss phase shifts are visible in the overlay before they're fully visible in the world. The twin's data on the boss updates — a significant component loss registers as a system change in the overlay. Players who are reading the overlay get leading information about the phase transition.

---

## The Player in the Boss Fight

Wound states are not suppressed during boss encounters. The player character looks like they are in a fight.

**Progressive visual state:**
- Early fight: character at incoming condition — pre-existing wounds visible if present
- Mid fight: fight-inflicted wounds accumulating — bleeds from hits, burn signatures from Vael or incendiary attacks, impact effects from Behemoth strikes
- Late fight: the accumulation is legible — the player character shows the cost of being this deep into this encounter

**The three-ring architecture visual implications:**

*Ring 1 (arena systems — boss components, lieutenant health):*
Component health visible in the component's physical state. A Behemoth crane arm at low health shows structural stress — deformation, progressive failure, the visual prelude to the break.

*Ring 2 (attrition — ammo, armor, wounds, hydration):*
Armor degradation visible on the player character. The armor at the start of the fight and the armor thirty minutes in should look different. Hits land and leave marks. The armor's cumulative damage state is the visual record of the fight's attrition cost.

*Ring 3 (boss health pool):*
Boss health loss drives the cumulative damage visible on the boss form. The bar and the body tell the same story — the body tells it more vividly.

---

## The Aftermath Aesthetic

After the boss is down, the player character should communicate that they were there.

**The visual state at completion is its own reward.**

**What should be visible:**
- All wounds acquired during the fight: active bleeds, burn marks, splinted if applied, pallor if toxin/radiation exposure occurred
- Armor damage from the fight's attrition: the gear looks used. Plates cracked, surfaces burned, impacts marked. This is not cosmetic damage — it is the same armor degradation system that ran during the fight, visible in completion state.
- Clothing damage: burns stay burned, blood stays dried. The character looks like the fight lasted as long as it did.

**What should not be cleaned up:**
- Health is not restored to full at kill — the player is at whatever health state they reached the end with
- Wounds are not healed at kill — they still need treatment
- Armor state is at fight's-end state — degraded accordingly

**The Brace:**
The player placed the Brace at encounter start — their spawn anchor. A player who died multiple times during the fight respawned near the Brace. Their character's visible wound state at fight's end reflects the final life's accumulation. A player who died twice and then finished cleanly looks like the final life. A player who barely died at all looks like the whole fight — the accumulation is longer.

**Group aftermath:**
In a group fight, the visual diversity of aftermath states tells the story of who did what. The player who tanked the Behemoth's forward arms looks different from the player who stayed at range. The medic who spent the fight treating others has a different wear state than the player who burned through attrition without being treated.

---

## Boss-Specific Aftermath Notes

**Fractured Behemoth aftermath:**
The Behemoth's remains are environmental. Whatever zone-typed accumulation it carried is now in pieces in the environment. The fight location looks like what happens when a thing that big stops. For a Rural Behemoth in a residential street: a combine harvester chassis in pieces across several residential blocks. The scale that made it terrifying when it was moving is now terrifying as wreckage. The wreckage is also a resource — players who scavenge the aftermath recover significant zone-typed materials.

**Warden Record:**
If a Warden (player or NPC) was present for the boss encounter, the Record is updated. The twin indexes the event at the GPS location. The Historical Marker that appears at that coordinate is permanent — it marks where a Behemoth was stopped, and by whom. Players who return to that location find the Record in the overlay.

---

## Asset Notes

- Component-based damage system requires modular boss meshes — each destructible component is an independent mesh with its own LOD and damage state sequence
- Damage states are art-authored, not procedural — the specific failure mode of each component is designed (crack pattern, deformation, exposed interior)
- Player armor degradation is a shader parameter (damage accumulation value) plus a separate wound layer — two independent systems that both run during a boss fight
- Aftermath environmental state (Behemoth wreckage) requires a post-fight environment pass — the fight zone is a distinct location state that persists after kill
