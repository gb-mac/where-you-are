# Concept Brief — Historical Marker Visual States & Territory Overlays
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## Historical Markers

Warden Historical Markers are entries in the twin's indexed memory — the Record of what happened at a GPS location. They persist through overlay degradation states. They are the most reliable information source in the game.

Markers have three content states based on the Record's entries:

---

### Positive Marker

**What it records:** Something that held. A community defended, a group that coordinated under pressure, a sacrifice documented, a decision that cost something and was made anyway.

**Visual:**
- Glyph: Warden shield glyph at standard size
- Color: `--faction-wardens` deep forest `#2D5A27` with brass `#B8923A` active on the horizontal identification line
- Overlay quality: the Marker has a subtle positive data density visible in the twin's layer — the Record is extensive. Players who approach get a full readout.
- Distance read: The brass element catches light and is visible at the Warden-standard 80m range.

**Text readout on approach (within 5m):**
The Record entry expands as a sidebar. Participants listed. Event documented in the Warden's observational language — functional, precise, not sentimental. The twin indexed it verbatim.

---

### Negative Marker

**What it records:** Something that failed. A community that fell, a decision that cost more than it should have, a Behemoth that reached a populated area, a Gone Dark that wasn't stopped in time.

**Visual:**
- Glyph: Warden shield glyph, same size
- Color: `--overlay-warning` amber `#F5A623` replacing the usual brass — the identification line is amber, not brass. The forest green base remains.
- Overlay quality: the data density is present but the Record entries are marked differently — the twin's indexing format flags the event type as loss/failure.
- Distance read: The amber reads at the same range as the brass, but differently. Players learn to distinguish the two at distance.

**Text readout on approach:**
The Record entry expands. The Warden documentation of a negative event is as precise and functional as a positive one — the Wardens do not editorialize. What happened is documented. Who was there is documented. What it cost is documented.

**Design intent:** A negative Marker is not a failure state of the UI. It is the Record being honest. The world has negative Records because things went wrong in it. The visual acknowledges this without dramatizing it.

---

### Contested Marker

**What it records:** Multiple Record entries that conflict. Two accounts of the same event that disagree. An ongoing dispute about who held a location, what happened here, whether the documented outcome was accurate.

**Visual:**
- Glyph: Warden shield glyph with a vertical split — the left half in brass `#B8923A`, the right half in amber `#F5A623`. The split is clean, not blended.
- Color: The forest green base remains. The identification line carries both colors.
- Overlay quality: the data density is high — multiple Record entries, multiple indexing events. The twin has more information about this location than either entry alone represents.
- Distance read: The split-color identification line is distinct at distance from both the brass (positive) and amber (negative) single-color reads.
- Pulse: contested Markers pulse at a slow rate — not the urgency pulse of contested territory boundaries, but a slow acknowledgment that this Record is not resolved.

**Text readout on approach:**
Both Record entries expand in the sidebar, in sequence. The conflict between them is visible in the documentation. The Wardens documented both because both happened — or both were reported — and the twin indexed both.

**Design intent:** Contested Markers are the most narratively rich Markers in the game. The conflict in the Record is the story. Players who read contested Markers are reading history with a dispute in it, which is what history mostly is.

---

## Territory Overlay States

### Faction-Held Territory

Covered in `overlay-ui-spec.md`. Faction color at 30% fill, 100% boundary line, faction glyph at territory centroid.

### Contested Territory

Two factions actively disputing an area. The boundary between them is the contested zone.

**Visual:**
- Boundary lines between the two factions flicker at the contested edge — the twin does not know who owns this space, and shows both claims
- The contested zone between the two faction color-fills: both colors desaturate toward `--overlay-primary` blue-white. The twin's data layer shows both claims losing their confident color in the space of conflict.
- Contested zone width: proportional to the intensity of dispute — a freshly contested border is a thin line; a long-running dispute has a wider contested zone where neither faction's claim is confident
- No faction glyph in the contested zone — both faction glyphs appear at the edge of each faction's uncontested territory, not in the contested zone itself

---

### Untenanted Territory

Space between faction claims. No one owns it.

**Visual:**
- No territory fill color
- No boundary line
- `--overlay-primary` at 50% opacity for the base overlay — the twin is present, the data is there, the human annotation is sparse
- Entity markers for machine faction activity are present and accurate — the twin indexes machine presence reliably regardless of human faction absence
- No faction glyph at the territory centroid — untenanted territory has no centroid marker
- A specific quality of overlay sparseness: the information density is lower than faction-held territory. Players learn to read this as "no one holds this" rather than as a rendering failure.

**Untenanted territory and the Historical Marker:**
If something significant happened at a coordinate in untenanted territory, the Warden Marker is still present. The territory is untenanted now — it may not have always been. The Marker's presence in sparse overlay is one of the few visually prominent elements in an untenanted zone.

**Untenanted territory becoming held:**
As a player establishes Holdout presence in untenanted territory — stays, builds standing, has the twin index their activity — the overlay data density increases. The fill color begins to appear at low opacity. A faction glyph starts to form at the territory centroid as the twin builds enough confidence to classify the space. Players can watch the overlay reflect the claim they're building.

---

## Gone Dark Communities

Holdout communities that have completed the decline arc into Gone Dark hostility have a specific overlay state that is distinct from standard Holdout territory:

**Visual:**
- The Holdout territory fill color (`--faction-holdouts` ochre tones) desaturates progressively as the community slides through decline stages
- At Gone Dark: the fill color is fully desaturated — a gray fill at 30% opacity, the color of the Holdout faction that has left. The boundary line remains but in the desaturated tone.
- The community's Historical Markers remain — the Record of what this community was before is still in the twin's data layer. A positive Marker from before the decline next to a gray fill is the overlay telling the story.
- No faction glyph update — the glyph at the territory centroid remains the Holdout glyph. The glyph does not change to reflect Gone Dark status. The twin records the faction, not the faction's current moral state.

**Why no special Gone Dark glyph:**
The twin does not evaluate. The twin indexes. The Holdout community that became Gone Dark is still, technically, a Holdout community at the administrative level the twin uses. The gray fill communicates the data-layer truth — the activity signature of this territory has changed in a way the twin registers as reduced human-coordination activity. But it does not editorialize. The Warden Record does.

---

## Asset Notes

- Contested Marker split-color treatment requires a specific shader for the glyph — two-tone horizontal split, not a gradient
- Contested territory boundary flicker is an animated overlay state — the two faction colors alternating at the boundary line, not blending
- Untenanted territory data sparseness is communicated through overlay opacity only — no special texture or pattern, purely the absence of annotation density
- Gone Dark desaturation is a progressive parameter on the territory fill color, not a discrete state change — the gray approaches as the community declines
