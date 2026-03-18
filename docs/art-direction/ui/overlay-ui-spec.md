# Where You Are — Overlay UI Specification
**Version:** 0.1
**Agent:** Art Direction
**Last updated:** 2026-03-17
**Scope:** Desktop UE5 game only. Mobile AR overlay is a separate specification (`~/Projects/game-mobile`).

---

## Design Premise

The UI is not layered on top of the game. The UI **is** the overlay — the twin's own spatial data architecture rendered over physical reality. There is no HUD in the traditional sense. There is a data layer the player has learned to read.

This has two implications:

1. **Diegetic by default.** Every UI element exists because the twin put it there. If the twin cannot see something, the UI cannot show it. If the twin's data is corrupted, the UI shows corrupted data.

2. **The overlay fails with the world.** When degradation occurs (Fractured voids, Vael drift, Corrupted wrong-data), UI elements in that zone degrade with it. The HUD is not immune to the world state.

---

## Typography System

**One typeface. Monospace only.**

The twin communicates in data. Data has no serif, no display type, no expressive weight. Every character occupies the same space because measurement requires consistency.

| Role | Size (1080p output) | Weight | Usage |
|---|---|---|---|
| **Primary readout** | 20px | Regular | Health values, ammo count, coordinates |
| **Secondary readout** | 16px | Regular | Status labels, faction identifiers |
| **Tertiary / annotation** | 14px | Regular | World-space distance markers, supplementary data |
| **Critical alert** | 24px | Regular | Fatal threat, Vael scan warning |
| **Minimum legible** | 14px | — | Never go below this at 1080p output |

**FSR rule:** All primary and secondary readouts must be legible at 50% internal resolution. At 540p internal, 20px output characters must still parse. Test at FSR Quality mode minimum.

No italics. No bold for emphasis — hierarchy is communicated by size and color, not weight variation.

---

## Color Tokens

All UI elements use tokens, not raw hex values. This allows global degradation and mode shifts to apply consistently.

| Token | Default value | Role |
|---|---|---|
| `--overlay-primary` | `#C8DCF0` | Primary data elements, grid lines, default glyph color |
| `--overlay-glyph` | `#E8F4FF` | High-contrast readouts — health, ammo, critical values |
| `--overlay-scrim` | `rgba(8, 18, 28, 0.72)` | Background behind screen-space readouts |
| `--overlay-active` | `#FFFFFF` | Currently selected / focused element |
| `--overlay-warning` | `#F5A623` | Sub-50% health, threat proximity, Signal-frequency interference |
| `--overlay-critical` | `#E84040` | Sub-25% health, Vael scan active, imminent death state |
| `--overlay-void` | `#000000` | Fractured data voids — absolute black, not dark gray |
| `--overlay-drift` | `#8BA8C8` | Vael displacement — desaturated, slightly shifted from primary |
| `--overlay-corrupt` | `#4AC870` | Corrupted machine proximity — wrong-hue, the twin is lying |
| `--faction-[id]` | Per faction | Faction color for territory, markers, affiliation readout |

**Note on `--overlay-corrupt`:** The green value is intentional. The twin's blue-white shifted to green is visually wrong in a specific, readable way. The player learns that green-shifted overlay means corrupted data source proximity.

---

## Information Hierarchy

Three tiers. Tier visibility responds to game state.

### Tier 1 — Always Visible
The minimum the player needs to not die. Present in all states including cutscenes and menus (except full-screen map).

- **Health bar / value** — bottom-left, horizontal bar + numeric readout
- **Fatal threat indicator** — directional damage arc at screen edge, `--overlay-critical` color
- **Active objective marker** — world-space, single marker only, primary mission objective

### Tier 2 — Context-Sensitive
Visible during active gameplay. Fades (not disappears — the data is still there, just deprioritized) during dialogue, at rest in safe zones.

- **Ammo / weapon state** — bottom-right
- **Stamina / action resource** — adjacent to health, smaller
- **Minimap** — top-right, circular, 80px diameter at 1080p
- **Faction affiliation indicator** — current zone faction ownership, minimap adjacent
- **Secondary objective markers** — world-space, dimmer than primary, max 4 visible simultaneously
- **Loot indicators** — world-space, distance-faded, only visible within 40m

### Tier 3 — On-Demand
Opened by player input. Never intrudes on gameplay.

- **Full inventory** — screen-space panel, accessed via input
- **Full map** — full-screen, overlays all other UI
- **Faction standing** — accessed via inventory or dedicated input
- **Twin data readout** — location data, coordinate display, environmental readings for current position

---

## Screen-Space HUD Layout

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│  [ZONE / FACTION OWNER]                          [MINIMAP — 80px circular] │
│  [COORDINATE READOUT]                            [faction territory rings]  │
│                                                                             │
│                                                                             │
│                                                                             │
│                   · · · [CROSSHAIR] · · ·                                  │
│                                                                             │
│                                                                             │
│                                                                             │
│                                                                             │
│  [HEALTH BAR ████████░░░░  124 / 200]       [WEAPON — AMMO  28 / 90]      │
│  [STATUS FLAGS — active effects]            [SECONDARY AMMO / ALT STATE]  │
│                                                                             │
│  [ACTIVE OBJECTIVE — truncated to 60 chars]                                │
└─────────────────────────────────────────────────────────────────────────────┘
```

**Margins:** 24px from screen edge on all sides at 1080p. Scale proportionally.

**Health bar:** Horizontal, 200px wide, 6px tall. Bar fill color transitions:
- `> 50%` → `--overlay-primary`
- `25–50%` → `--overlay-warning`
- `< 25%` → `--overlay-critical` with 0.5s pulse (not blink — pulse, opacity 100%→70%→100%)

**No stacking notifications.** Event notifications (item picked up, objective updated, faction standing changed) appear as a single line below the objective readout, auto-dismiss after 3 seconds. Never more than one visible simultaneously — queue if multiple fire at once.

---

## World-Space Overlay Elements

Elements that exist in 3D space, attached to world geometry or entities.

### Entity Markers

Enemies, NPCs, and interactive objects are tagged in the twin's data layer. Markers:

- **Shape encodes faction:** Each faction has a distinct geometric marker — not just color. Shape must be readable without color for accessibility and FSR degradation tolerance.
- **Distance fading:** Markers fade linearly from 100% at 10m to 0% at 80m. No pop-in.
- **Vertical offset:** Markers float 0.3m above entity head height. No overlap with character geometry.
- **Hostile vs neutral vs allied:** Rim color only — `--overlay-critical`, `--overlay-primary`, `--overlay-active`. Shape does not change.

### Territory Boundaries

Faction-controlled territory is visible as a ground-projected boundary in the overlay:

- **Line weight:** 2px, world-space projected
- **Color:** `--faction-[id]` at 60% opacity
- **Pulse:** Contested territory boundaries pulse at 1Hz
- **Distance:** Visible from 0–150m. Fade 120–150m.

### Warden Historical Markers

The twin's memory of documented events. These are the Record.

- **Icon:** Distinct from entity markers — a specific Warden-faction glyph (defined in concept brief)
- **Color:** `--faction-wardens` + `--overlay-primary` dual-color treatment
- **Interaction:** Approach within 5m → Tier 2 text readout expands with the record entry
- **Permanence:** Warden Markers do not degrade in Fractured voids or Vael Displacement Zones. The Record is the twin's own indexed memory — it persists where active data rendering does not. This should feel significant.

### Item Drop Markers (Cross-Platform)

Items placed at GPS coordinates via the mobile app are visible in the desktop overlay.

- **Icon:** Neutral — not faction-affiliated unless the item carries faction identity
- **Color:** `--overlay-primary` default, `--faction-[id]` if faction item
- **Label:** Item name, truncated to 24 chars, + distance readout
- **Visibility:** Only within 60m. No minimap display beyond 200m.

### Vael Trace Indicator

If the player has been scanned by a Vael Probe, the trace is visible in the overlay because the Vael's scan wrote to the twin's data layer.

- **Placement:** Body-projected — a faint geometric overlay on the player's own character model, visible in third-person
- **Color:** `--overlay-critical` at 30% opacity, brightens when Vael units are within 200m
- **Behavior:** Does not degrade in Displacement Zones. The trace is in the twin's data. The Displacement Zone affects new reads, not existing records.

---

## Overlay Degradation in UI

When the world state degrades the overlay, the UI degrades with it. This is not a visual glitch — it is accurate data reporting.

### Fractured Proximity — Data Voids

The Fractured's severed connection leaves nothing. The twin cannot see that space.

- **World-space elements** in void radius: replaced with `--overlay-void` (absolute black patches)
- **Entity markers** for entities inside a void: absent. The twin does not know they are there.
- **Territory boundaries** in void: absent, not grayed — the boundary data does not exist
- **Minimap** coverage over void zones: black fill, no data
- **HUD elements** (Tier 1): unaffected — health and threat data comes from the player's own systems, not external twin reads
- **Visual radius:** Scales with Fractured unit tier. Void edges are hard — data stops at the boundary, not gradual fade.

### Vael Displacement Zone — Coordinate Drift

The Vael's interference causes measurement errors. The data is present but wrong.

- **World-space elements:** Drift by 0.5–3m from actual position, with 0.3Hz oscillation. The markers are showing where the twin *thinks* things are, which is not where they are.
- **Entity markers:** Present but offset. Combat in a DZ requires learning to discount the overlay.
- **Coordinate readout** (Tier 3): Shows values that increment incorrectly, stutter, display measurement errors in-line
- **Minimap:** Territory boundaries drift. Faction ownership display becomes unreliable.
- **HUD Tier 1:** Unaffected — except health bar, which shows a 1px drift shimmer to indicate active DZ. Health values remain accurate.
- **Text readouts:** Character substitution errors — `0` becomes `O`, numerals swap — to indicate the data layer is being misread

### Corrupted Machine Proximity — Wrong Data

The Corrupted machine's transmissions are being indexed by the twin as valid data. The twin cannot tell the difference.

- **Token shift:** `--overlay-primary` transitions toward `--overlay-corrupt` (the wrong green) within 30m radius. Full shift at 10m.
- **World-space elements:** Present, positioned accurately, but in the wrong color. The data is structurally correct; the source is corrupted.
- **Entity markers:** Corrupted units may display as wrong faction — their identity data is broken at source. The twin reports what they transmit.
- **HUD Tier 1:** Health bar border shifts toward `--overlay-corrupt`. Value readouts remain accurate — the corruption is in the overlay display layer, not the player's own data.
- **Recovery:** Token shift reverses linearly as distance from Corrupted units increases.

---

## Combat State Shifts

When combat initiates, the overlay reprioritizes.

| Element | Exploration state | Combat state |
|---|---|---|
| Loot markers | Visible within 40m | Hidden — noise reduction |
| Territory boundaries | Full opacity | 30% opacity |
| Coordinate readout | Visible | Hidden |
| Zone / faction owner | Visible | Hidden |
| Entity markers — hostiles | Standard | Brightened to 100%, shape scales up 20% |
| Entity markers — neutrals | Standard | Dimmed to 40% |
| Health bar | Standard | Moves to screen center-bottom, 120% scale |
| Objective marker | Standard | Suppressed unless within 10m |

Combat state exits 4 seconds after last hostile engagement. Re-entry is immediate.

---

## Full Map

Full-screen. Suspends gameplay (not a pause — the world continues, the player is reading the twin's spatial data).

- **Background:** `--overlay-scrim` full-screen
- **Geometry:** Real-world street/building geometry rendered as twin wire-frame — `--overlay-primary` at 40% opacity
- **Faction territory:** Color-fill per `--faction-[id]` at 30% opacity, boundary lines at 100%
- **Player position:** `--overlay-active` point marker
- **Markers:** All Tier 2 world-space elements visible at correct coordinates
- **Warden Markers:** Visible. Selectable — opens Record entry as sidebar panel.
- **Void zones:** Black fill, hard edges. The twin knows the void exists even if it cannot see inside it.
- **DZ zones:** Marked boundary, interior labeled `MEASUREMENT INTERFERENCE` in `--overlay-drift`
- **Typography:** Monospace, 14px minimum for all map labels

---

## FSR-Specific UI Rules

Summary of all FSR constraints in one place for implementation reference:

1. Minimum text size: 14px at 1080p output, 18px for primary readouts
2. Health bar: 6px tall minimum — test readability at FSR Performance mode
3. Entity marker shapes: minimum 12px diameter at 1080p — shape must be identifiable, not just present
4. Territory boundary lines: 2px minimum world-space projection
5. Crosshair: 1px center dot + 4px gap + 8px arms — total 18px span. No sub-pixel elements.
6. All screen-space elements positioned on 4px grid
7. Scrim opacity minimum 0.65 — lower values become invisible at reduced resolution

---

## Pending

- [x] Faction marker shape system — defined in `ui/faction-glyphs.md`
- [x] Warden Marker glyph design — defined in `ui/faction-glyphs.md`
- [ ] Full map: zone label placement rules (density management for real-world urban areas)
- [x] Accessibility pass: `--overlay-corrupt` — see Accessibility section below
- [ ] Mobile AR overlay spec (separate repo, separate document)

---

## Accessibility — `--overlay-corrupt` Color-Blind Safe Treatment

### The Problem

`--overlay-corrupt` is `#4AC870` — a green hue shift from the normal blue-white overlay. The design intent is "visibly wrong" — a hue that reads as incorrect against the normal data layer. Green against blue-white achieves this for standard vision. For deuteranopia (red-green color blindness, ~8% of male players), green and red are both desaturated — the green shift may not read as wrong.

### The Solution: Dual Signal

Keep `#4AC870` as the default. Add a **pattern signal** that encodes the corrupt state independent of color.

**Pattern:** In corrupt-state overlay elements, a fine diagonal hatch — 1px lines at 45°, 4px spacing — overlays the element surface. The hatch is `--overlay-scrim` dark at 40% opacity, visible against both the normal overlay blue-white and the corrupt green.

This means:
- **Standard vision:** reads corrupt by color (green = wrong) *and* pattern (hatched = wrong)
- **Deuteranopia:** reads corrupt by pattern alone (hatched = wrong) — color distinction not required
- **Both:** unambiguous at FSR reduced resolution — 4px hatch spacing survives aggressive upscaling

### Implementation Scope

The pattern applies to:
- Territory fill in Corrupted proximity
- Entity marker backgrounds in Corrupted proximity
- Health bar border when `--overlay-corrupt` shift is active
- Any overlay element whose token has shifted to `--overlay-corrupt`

The pattern does **not** apply to:
- The Corrupted faction glyph `⊗` itself — the glyph shape is the identifier; the pattern on the glyph body would obscure the form
- The `--overlay-corrupt` color token in `faction-glyphs.md` color rules — the glyph stays solid, reads by shape and color

### Additional Color-Blindness Coverage

| Type | `--overlay-corrupt` legibility | Mitigation |
|---|---|---|
| Deuteranopia | Green desaturated, may not read as wrong | Pattern signal |
| Protanopia | Green retained, reads differently from red — may be adequate | Pattern signal as redundant confirmation |
| Tritanopia (rare) | Green-blue distinction affected | Pattern signal; `#4AC870` is sufficiently saturated to retain some distinction |
| Achromatopsia (very rare) | No color — pattern signal only | Pattern signal is primary signal; consider value contrast audit |

### Testing Requirement

Before shipping: run the corrupt-state overlay through a deuteranopia simulation (Coblis or equivalent) and confirm the hatch pattern is legible at FSR Quality mode internal resolution.
