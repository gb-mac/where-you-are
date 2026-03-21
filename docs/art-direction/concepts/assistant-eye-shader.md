# Eye Shader Spec — The Assistant
*Color values, contrast, UE5 emissive material, FSR notes.*

---

## Surface Context

The eyes sit against matte gunmetal. Everything below is calibrated to that base.

| Surface | Hex | sRGB approx | Relative luminance |
|---------|-----|-------------|-------------------|
| Gunmetal base | #4A4E54 | 74, 78, 84 | ~8.5% |
| Gunmetal wear (light) | #7A8088 | 122, 128, 136 | ~24% |
| Eye surround ring | #0D0F10 | 13, 15, 16 | ~0.2% |

The dark ring around each iris punches a near-black border between eye and face. Without it, FSR upscaling bleeds the emissive color into the surrounding geometry. With it, the eye reads as a discrete object at any internal resolution.

**Ring sizing:** 8–10% of iris radius at LOD0. Scales with LOD — do not let it disappear below LOD2 or the eyes lose definition at distance.

---

## Eye States — Color Values

### Red State

| Property | sRGB Hex | Linear (pre-multiply) | Notes |
|----------|----------|----------------------|-------|
| Iris base | #C0392B | R: 0.527, G: 0.044, B: 0.026 | Amber-shifted — not pure red |
| Emissive multiplier | — | × 4.0 | |
| **Emissive output** | — | **R: 2.108, G: 0.176, B: 0.104** | HDR — blooms slightly in post |

Perceived: a warm, deep red with amber edge. Not alarm-red. Not blood-red. The kind of red that reads as *old* — like a warning light that's been on for a long time.

Contrast ratio vs gunmetal base (pre-emissive): **~1.4:1** — low on purpose. The emissive value does the work in HDR. In a non-HDR/LDR fallback, increase base luminance to #D44C3A to hold readability.

---

### Green State

| Property | sRGB Hex | Linear (pre-multiply) | Notes |
|----------|----------|----------------------|-------|
| Iris base | #27AE60 | R: 0.021, G: 0.439, B: 0.128 | Warm-shifted — pulled toward lamp amber |
| Emissive multiplier | — | × 3.5 | Slightly softer than red |
| **Emissive output** | — | **R: 0.074, G: 1.537, B: 0.448** | HDR |

Perceived: warmer than a typical screen green, cooler than the lamp. Almost calm. Almost safe.

Contrast ratio vs gunmetal base (pre-emissive): **~3.1:1**. Green naturally reads higher luminance than red against dark surfaces — the lower emissive multiplier compensates, keeps the two states perceptually matched in intensity.

---

### Transition Midpoint — Amber

The cross-fade between states passes through this value at ~50% blend. It is not a designed state — it emerges from the lerp — but it should be verified to look right.

| At 50% blend | Linear value |
|-------------|-------------|
| R | (2.108 + 0.074) / 2 = **1.091** |
| G | (0.176 + 1.537) / 2 = **0.857** |
| B | (0.104 + 0.448) / 2 = **0.276** |

Result: warm amber-gold — close to the lamp's color temperature. For roughly one frame of the 0.3–0.5s transition, the eyes match the lamp. Undesigned, but correct. Keep it.

---

## UE5 Material Setup

**Material type:** Unlit emissive on the iris mesh. The iris is a separate mesh element from the head — allows independent material control without recompiling the head material.

**Parameters to expose:**

```
// M_AssistantEye material parameters
EyeColor_Linear     (Vector3, default: Red state values)
EmissiveMultiplier  (Scalar, default: 4.0)
RingOpacity         (Scalar, default: 1.0 — reduce only for LOD3+)
```

**Material graph (simplified):**
```
[EyeColor_Linear] × [EmissiveMultiplier] → Emissive
[Depth texture / fake lens depth] → lerp → Emissive  (adds perceived depth behind iris)
[RingOpacity] → controls dark surround ring via masked black layer
```

**Fake depth note:** A simple radial gradient darkening toward the iris edge — darker rim, brighter center — sells the sense of depth behind the eye without raytracing. 20–30% darkening at the rim is enough.

---

## Transition Curve — UE5 Timeline

The state transition is driven by a Timeline node in the assistant's Blueprint, outputting a 0→1 float fed into a `Lerp` between Red and Green parameter sets.

**Normal transition (red → green):**
```
Duration: 0.4s
Curve shape: ease-in-out (slight S-curve)
             slow start, accelerates through midpoint, settles
```

**Last-second flip (green → red, trailer moment):**
```
Duration: 0.0s (instant — Set Parameter directly, no Timeline)
```

The asymmetry is the point. The return to red should feel like a switch thrown, not a fade. If it cross-fades, the tension collapses.

For gameplay (post-trailer), additional states can be added to the parameter set — but red and green are the only two states that exist in the announcement.

---

## Target Resolution

**Primary target: 1080p native.** Build and test here first. FSR upscaling, LOD fallbacks, and Vega 8 iGPU optimization are deferred — tackle those when the 1080p version is locked.

If performance requires dropping settings at 1080p, reduce in this order:
1. Disable fake depth texture (flat emissive iris, no depth gradient) — minimal visual cost
2. Reduce EmissiveMultiplier from 4.0 to 3.0 — eyes still read, slightly less bloom
3. Reduce iris mesh poly count — the dark ring and color carry the read, not geometry

Do not compromise the dark surround ring or the instant red-flip behavior to hit performance. Those are the two things that matter most on screen.

---

*v0.2 — 1080p native primary target. FSR/LOD/Vega 8 deferred.*
