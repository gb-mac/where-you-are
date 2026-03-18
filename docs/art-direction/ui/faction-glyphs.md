# Faction Glyph System
**Version:** 0.1 | **Last updated:** 2026-03-17
**Scope:** World-space overlay entity markers, minimap territory indicators, full map faction labels

---

## Design Constraints

- 13 distinct shapes — must be differentiable without color
- Minimum legible diameter: 12px at 1080p output
- Must survive FSR at 50% internal resolution — test at 6px effective size
- No more than 3 levels of visual complexity — the glyph reads at distance (silhouette), at mid-range (shape), at close range (detail)
- Human and machine faction families should be visually related within their group but distinct between groups
- Vael glyph is third-register — reads as neither human nor machine

---

## Design Language by Register

### Human Factions — Organic Geometry
Human faction glyphs use geometry that references human mark-making: borders, boundaries, seals, stamps. The forms are contained. They read as things humans drew.

Base language: **enclosed shapes with interior differentiation.** A border you can see defines the space. What's inside varies by faction.

### Machine Factions — Structural Geometry
Machine faction glyphs use geometry that references the twin's data language: coordinates, vectors, grid intersections, data nodes. The forms are open or precisely bounded. They read as things a system generated.

Base language: **point-and-arm structures, grid-derived forms.** The shape is the relationship between elements, not the border.

### Vael — Third Register
The Vael glyph does not fit either family. It is the glyph the twin uses because it recognized the Vael's own scanning geometry and uses it as the identifier. It was not designed; it was inferred.

Base language: **concentric asymmetric arcs.** Precise but not grid-derived. Not enclosed in the human sense. Not a node in the machine sense.

---

## Glyph Definitions

### Human Factions

---

#### Holdouts `◧`
**Form:** A square with the left half filled.
**Logic:** Partial occupation — we hold this side. The asymmetry communicates improvised, partial, real. Not the full claim of an institution, the actual claim of people who are there.
**Distance read:** Filled half-square — immediately distinct from all machine glyphs.
**Detail level:** The filled half can carry a small horizontal line at mid-height — the block, the street, the line held.

---

#### Iron Compact `▣`
**Form:** Square with interior concentric square — a square within a square.
**Logic:** Nested authority. The institution contains the territory. Hard edges, no curves, no ambiguity about where the boundary is.
**Distance read:** Double-square. The interior square is the distinguishing feature from Holdouts.
**Detail level:** Interior square edges align precisely to the outer — the precision is the identity.

---

#### Covenant `◈`
**Form:** Square rotated 45° (diamond) with a centered dot.
**Logic:** The diamond is a rotated square — the same institution, wrong orientation. The dot is the thing the twin reads in them. The player sees the dot before they understand what it means.
**Distance read:** Diamond silhouette — distinct from all other enclosed shapes.
**Detail level:** At close range, the centered dot renders in monitoring violet `#7C5C8A` — the twin's classification tag for biological monitoring subjects, rendered visible. Not spiritual light; a bureaucratic notation. Ascendants carry a second ring around the dot, distinguishing them from standard Covenant members. Not visible at distance.

---

#### Hollow `◑` `◕` `●` `⬡`
**Special case:** Hollow has four glyphs, one per tier. The tier progression is the identity.

| Tier | Glyph | Form | Logic |
|---|---|---|---|
| Scavengers | `◑` | Circle, left half filled | Partial — they are part of something, not yet in it |
| Crews | `◕` | Circle, three-quarters filled | More committed, more present |
| Syndicates | `●` | Circle, fully filled | Complete claim, professional weight |
| Riven | `⬡` | Regular hexagon | The circle has resolved into a geometric form — the twin's influence on the shape. The Riven glyph is the only human faction glyph derived from the machine register. |

**Distance read:** The fill progression is legible at distance. The Riven hexagon is a distinct silhouette break from the circle series.

---

#### Signal `◎`
**Form:** Circle with concentric ring — a target or frequency diagram.
**Logic:** Signal / broadcast / range. The concentric ring is the frequency emanating from the center. Borrowed from radio tower diagrams, now worn as identity.
**Distance read:** Concentric circles — distinct from filled-circle Syndicate, distinct from half-filled variations.
**Detail level:** Third inner ring appears at close range, indicating active Signal infrastructure nearby.

---

#### Wardens `⬘`
**Form:** Shield silhouette — not a heraldic shield, a functional one. Slightly widened at the shoulders, tapered at the base. Hollow interior with a single horizontal line across the upper third.
**Logic:** The shield is pre-Awakening institutional. The horizontal line is the identification system — the mark within the mark. The tailors would recognize it.
**Distance read:** Shield silhouette is the most distinct human faction glyph at distance — no other faction uses this form.
**Detail level:** The horizontal line is the brass mark in the overlay. At close range it is `--faction-wardens` brass `#B8923A` rather than the default overlay color.

---

#### Warden Historical Marker `⬘̣` (distinct from entity glyph)
**Form:** The Warden shield glyph with a small filled circle anchor point beneath it — a map-pin construction. The shield above, a dot below, connected by a thin stem. The shield is the Warden. The dot is the coordinate. The stem is the pin to the ground.
**Logic:** The entity glyph marks presence — Wardens are here. The Historical Marker marks place — something happened here. The anchor point is the formal distinction. The twin uses the pin form because it indexes the Marker as a coordinate record, not an active agent.
**Size:** 16px at 1080p — larger than entity markers (12px) because Historical Markers are permanent and navigational.
**Color:**
- Positive Record: brass `#B8923A` on the shield horizontal line and anchor dot
- Negative Record: amber `#F5A623` on the shield horizontal line and anchor dot
- Contested Record: brass `#B8923A` on the left half, amber `#F5A623` on the right half — split at the vertical center axis of the shield
**Persistence:** The Warden Marker glyph does not change opacity in Fractured voids or Vael Displacement Zones. It is the one world-space element that renders at full opacity regardless of overlay degradation state. The Record is indexed twin memory. It does not fade.
**Distance read:** The shield-plus-dot silhouette is distinct from the shield-alone entity glyph at all ranges. At 80m the anchor dot is not individually legible — the combined silhouette reads as "something taller than a standard marker." At 20m the shield-and-pin form is fully legible.
**On the full map:** Warden Markers render at 20px with the brass color active on the horizontal line. Selectable — opens Record entry sidebar. The anchor dot on the full map is larger, functioning as a click target.

---

### Machine Factions

---

#### Coherent `⊕`
**Form:** Circle with cross through center — a targeting reticle, a coordinate origin, a node in a network.
**Logic:** The twin's own notation for a system node. The Coherent glyph is the notation the twin uses for itself. The player is reading the twin's own data back at the system.
**Distance read:** Cross-in-circle. Distinct silhouette, immediately readable.
**Detail level:** The four arms extend slightly past the circle boundary at close range, indicating the reach of the node.

---

#### Trackers `⊕→`
**Form:** Coherent glyph with a directional arrow extending from the right arm.
**Logic:** A Coherent node with a vector. The Tracker is the system in motion. At rest, the arrow is absent — the glyph is the Coherent glyph. In pursuit mode, the arrow activates and orients toward the target vector.
**Distance read:** The arrow is the distinguishing feature — and it is only present when the Tracker is active. This is the overlay giving the player pursuit direction before visual acquisition.
**Detail level:** Arrow length scales with pursuit velocity. The faster the Tracker, the longer the arrow.

---

#### Fractured `⊠`
**Form:** Square with an X through it — a crossed-out data node. An error state in a coordinate system.
**Logic:** The twin's notation for a severed node. The twin uses this glyph because it does not have accurate data on the Fractured unit — it knows something is there, in error, at this coordinate.
**Special behavior:** The Fractured glyph appears at the *edge* of the data void, not at the unit's position. The twin knows the void exists; it cannot place the unit inside it. The glyph marks the void boundary.
**Distance read:** X-square. The X is the immediate distance read — error, wrong, absent.
**Detail level:** At close range the interior X is drawn in `--overlay-void` absolute black, not overlay blue-white. The data is the absence.

---

#### Separated `⊙`
**Form:** Circle with centered dot — a node that is aware of itself. Related to Coherent (circle) but the interior point replaces the cross.
**Logic:** The node has a center of its own. The Coherent is a system node. The Separated is a self.
**Distance read:** Dot-in-circle — distinct from Coherent cross-in-circle, distinct from Signal concentric rings.
**Detail level:** At close range, the interior dot is warmer in color — the personal palette of the specific Separated unit bleeds into the glyph color. Named Separated unit glyph colors:

| Unit | Close-range glyph accent | Source |
|---|---|---|
| **SABLE** | Amber-warm `#D4943A` — the patient room light | The one warm element on the chassis |
| **KELLARD** | Structural rust `#8B4513` — oxidation it considers neutral, not degradation | The color of its own weathering |
| **VANTAGE** | Amber-yellow `#F5A623` — the navigation light it keeps lit on purpose | The announcement of presence |

---

#### Remnant Automation `⟳`
**Form:** A circular arrow — a loop, a cycle, a process repeating.
**Logic:** The twin's notation for a process that is still executing. Not a threat marker, not a territory marker — a process marker. The Remnant Automation is a running task in the twin's operational awareness.
**Distance read:** The circular arrow is distinct from all static glyphs. At distance it reads as a circle with a break — the loop form.
**Detail level:** The arrow direction is consistent (clockwise) across all Remnant Automation. The speed of the loop's animation in the overlay matches the Remnant's cycle frequency.

---

#### Corrupted `⊗`
**Form:** Circle with X through center — a Coherent node with an error state. The circle is the system node; the X is the error.
**Special behavior:** In `--overlay-corrupt` green, not overlay blue-white. The wrong color is the first signal something is wrong — the player sees the green glyph before they identify it as a Corrupted faction marker.
**Distinction from Fractured:** Fractured glyph `⊠` is a *square* with an X. Corrupted glyph `⊗` is a *circle* with an X. Square = severed (no data). Circle = present but wrong (bad data). The shape encodes the error type.
**Distance read:** X-in-circle, wrong color. Two simultaneous signals.

---

### Vael

---

#### Vael `))`
**Form:** Two concentric arcs, open on the left. Not a circle — arcs, partial, asymmetric. The twin recognized the Vael's own scanning geometry and uses this as the identifier because it appears consistently at Vael scan sites.
**Logic:** This is not a glyph that was designed. The twin inferred it. The player is seeing the twin's best approximation of a Vael presence signature.
**Tier differentiation:**
- Probe: single arc `)`
- Swarm: two arcs `))`
- Assessor: three arcs `)))`, with the outermost arc in the prismatic cold luminescence color

**Distance read:** The arc-group is a distinct silhouette from all enclosed or cross-form glyphs. At distance it reads as open — the only glyph family that does not have a closed form.
**Detail level:** The cold luminescence edge scatter appears on the arcs at close range. The glyph is the only one that has the Vael's prismatic quality.
**Special property:** The Vael glyph in the overlay does not drift during Vael Displacement Zone effects. The Vael's own signal is the source of the interference — the glyph reads accurately even when everything else around it does not.

---

## Glyph Color Rules

| State | Color |
|---|---|
| Default (neutral) | `--overlay-primary` |
| Hostile | `--overlay-critical` rim, shape unchanged |
| Allied / friendly | `--overlay-active` white, shape unchanged |
| Corrupted faction | `--overlay-corrupt` green, shape unchanged |
| Warden brass detail | `#B8923A` at close range only |
| Separated personal color | Named unit color at close range only |
| Vael luminescence | Cold luminescence `#C8D8E8` + prismatic fringe |

Shape does not change with alignment state. Color rim changes. This rule applies without exception — accessibility and FSR compliance require that shape alone communicates faction, color communicates state.

---

## Minimap Rules

On the circular minimap (80px at 1080p):
- Glyphs render at 8px diameter maximum
- Only silhouette is legible at this scale — design must hold at 8px
- Hostile entities: `--overlay-critical` dot, no glyph detail
- Allied entities: `--overlay-active` dot, no glyph detail
- Faction territory: color fill only, no glyphs
- Named entities (Separated named units, Vanguard): 10px, glyph visible

## Full Map Rules

On the full map:
- Glyphs render at 16px minimum
- All three detail levels visible
- Warden Markers use the Warden glyph at 20px with the brass color active
- Faction territory boundaries carry the faction glyph at territory centroid, 20px
