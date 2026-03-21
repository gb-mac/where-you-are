# Art Direction — Session Index
*Last updated: 2026-03-20*

## Locked Assets
| File | Description |
|------|-------------|
| `assets/assistant-key-visual.png` | Character shot — gunmetal robot, red/green eyes, sniper scope reticle |
| `assets/title-treatment.png` | "Where You Are" mixed typeface title card (landscape, has debug metrics) |
| `assets/title-treatment-large.png` | Clean isolated title — no debug text. Use this for production. |
| `assets/assistant-poster.png` | Composite — character + title at shoulder height. Landscape. Announcement poster. |
| `assets/assistant-poster-mobile.png` | Portrait mobile variant — wider body shot, title mid-frame |
| `assets/assistant-poster-mobile-4k.png` | Portrait mobile 4K — close-up face crop, title at bottom. Primary mobile key visual. |
| `assets/assistant-screensaver.png` | Distant silhouette — full body, tiny in frame. Teaser / screensaver format. |

## Concepts & Specs
| File | Status |
|------|--------|
| `concepts/trailer-announcement.md` | v0.4 LOCKED — 30s TV spot, key visual locked |
| `concepts/trailer-storyboard.md` | v0.1 — full frame-by-frame, open items noted |
| `concepts/assistant-character-design.md` | v0.1 — Atom × Tesla Optimus, gunmetal, eye design |
| `concepts/assistant-chassis.md` | v0.1 — repair stages, the light, boxing config |
| `concepts/assistant-eye-shader.md` | v0.3 — Full UE5 node graph, fake depth, ring mask, LOD control |
| `concepts/assistant-eye-blueprint.md` | v0.1 — Blueprint: MID setup, SetEyeState event, Timeline, LOD ring control |
| `concepts/assistant-iris-mesh.md` | v0.1 — Iris geometry, UV spec, LOD counts, component setup |
| `concepts/audio-direction-trailer.md` | v0.1 — Voice processing, underscore brief, sync notes for 30s spot |
| `style-guide.md` | v0.1 — Three visual languages, faction identities, FSR/Vega 8 rules |

## Open Items
1. **Copywriter** — voice copy for 3 stages of the 30s spot (garbled / Yoda / coherent). Copy placeholder in trailer-announcement.md — pull in narrative agent to finalize.
2. **Title card font** — EB Garamond + Fira Sans ExtraLight locked as placeholder. Installed at `~/.local/share/fonts/`. Requires commercial license verification before ship.
3. **FSR/LOD/Vega 8** — eye shader performance work deferred. Revisit when RX 5700 arrives.
4. **Vael luminescence values** — emission values TBD, flagged in style-guide.md.
5. **Separated unit palette examples** — requires Narrative input on specific named units.
