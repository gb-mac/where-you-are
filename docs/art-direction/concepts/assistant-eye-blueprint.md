# Eye State Blueprint — The Assistant
*UE5 Blueprint implementation for eye color state transitions.*

---

## Overview

The eye material (`M_AssistantEye`) is driven by a Dynamic Material Instance (MID) on the Assistant character Blueprint. A Timeline node handles the normal cross-fade; the instant red flip bypasses the Timeline entirely and sets parameters directly.

---

## Enum — EWYAEyeState

Create this enum in the project (Content/Characters/Assistant/):

```
EWYAEyeState:
  Red    // default, threat/unknown
  Green  // resolved, coherent
```

*Post-trailer: add Amber, Off, Damaged as needed. Red and Green are the only states for the announcement.*

---

## Variables (on BP_Assistant)

| Name | Type | Default | Notes |
|------|------|---------|-------|
| `MID_Eye` | Material Instance Dynamic | — | Created in BeginPlay |
| `CurrentEyeState` | EWYAEyeState | Red | Tracks current state |
| `EyeColor_Red` | Linear Color | (0.527, 0.044, 0.026, 1.0) | From shader spec |
| `EyeColor_Green` | Linear Color | (0.021, 0.439, 0.128, 1.0) | From shader spec |
| `Multiplier_Red` | Float | 4.0 | |
| `Multiplier_Green` | Float | 3.5 | Slightly softer than red |
| `EyeColor_From` | Linear Color | — | Lerp source, set before playing Timeline |
| `Multiplier_From` | Float | — | Lerp source |
| `EyeColor_To` | Linear Color | — | Lerp target |
| `Multiplier_To` | Float | — | Lerp target |

---

## BeginPlay

```
Event BeginPlay
  → CreateDynamicMaterialInstance(M_AssistantEye)
      → Set MID_Eye
  → MID_Eye → SetVectorParameterValue("EyeColor_Linear", EyeColor_Red)
  → MID_Eye → SetScalarParameterValue("EmissiveMultiplier", Multiplier_Red)
  → MID_Eye → SetScalarParameterValue("RingOpacity", 1.0)
  → Assign MID_Eye to iris mesh slot (SetMaterial on Iris mesh component, index 0)
```

---

## Custom Event — SetEyeState

```
Custom Event: SetEyeState
  Inputs:
    NewState  (EWYAEyeState)
    bInstant  (bool, default: false)

  → Set CurrentEyeState = NewState

  → Switch on NewState:

    [Red]
      Set EyeColor_To   = EyeColor_Red
      Set Multiplier_To = Multiplier_Red

    [Green]
      Set EyeColor_To   = EyeColor_Green
      Set Multiplier_To = Multiplier_Green

  → Branch: bInstant?

    [True — instant flip]
      MID_Eye → SetVectorParameterValue("EyeColor_Linear", EyeColor_To)
      MID_Eye → SetScalarParameterValue("EmissiveMultiplier", Multiplier_To)

    [False — cross-fade]
      Set EyeColor_From   = current MID EyeColor_Linear   // GetVectorParameterValue
      Set Multiplier_From = current MID EmissiveMultiplier // GetScalarParameterValue
      → Stop Timeline (EyeTransition) if already playing
      → Play from Start (EyeTransition)
```

*The trailer's last-second red flip calls `SetEyeState(Red, bInstant: true)`. All other transitions call with bInstant: false.*

---

## Timeline — EyeTransition

```
Name:     EyeTransition
Length:   0.4s
Looping:  false

Float track: "Alpha"
  Key 0:    value 0.0, tangent = Auto (ease-in-out S-curve)
  Key 0.4:  value 1.0, tangent = Auto

// Update (fires every tick during playback)
Event Update (Alpha: float)
  → LerpLinearColor(EyeColor_From, EyeColor_To, Alpha)
      → MID_Eye → SetVectorParameterValue("EyeColor_Linear", result)
  → Lerp(Multiplier_From, Multiplier_To, Alpha)
      → MID_Eye → SetScalarParameterValue("EmissiveMultiplier", result)

// Finished (fires once at end — prevents float drift)
Event Finished
  → MID_Eye → SetVectorParameterValue("EyeColor_Linear", EyeColor_To)
  → MID_Eye → SetScalarParameterValue("EmissiveMultiplier", Multiplier_To)
```

*The ease-in-out S-curve on the Alpha track is what produces the amber midpoint — the linear lerp at 50% blend hits (R:1.091, G:0.857, B:0.276), which lands near the warm lamp color. Don't flatten the curve.*

---

## Transition at 50% — Amber Verification

At Alpha=0.5 the lerp produces:

| Channel | Value |
|---------|-------|
| R | (2.108 + 0.074) / 2 = **1.091** |
| G | (0.176 + 1.537) / 2 = **0.857** |
| B | (0.104 + 0.448) / 2 = **0.276** |

Result: warm amber-gold, close to the lamp's color temperature. Verify this looks right in PIE before locking the transition duration. If it reads too hot or too cool, adjust `Multiplier_Red` / `Multiplier_Green` — don't adjust the curve shape.

---

## LOD — RingOpacity

Set `RingOpacity` on the MID based on LOD level. Wire to the character's LOD Changed callback:

```
LOD 0–2: RingOpacity = 1.0  (full ring)
LOD 3+:  RingOpacity = 0.5  (ring softens but doesn't disappear)
```

*Never set RingOpacity = 0. The ring is the FSR anchor. Without it the emissive bleeds into the face geometry at lower internal resolutions.*

---

## Call Sites (trailer sequence)

| Moment | Call |
|--------|------|
| Scene start | `SetEyeState(Red, false)` — already default, or explicit reset |
| Voice becomes coherent | `SetEyeState(Green, false)` — 0.4s cross-fade |
| Last frame before cut to black | `SetEyeState(Red, true)` — instant flip |

---

*v0.1 — Blueprint spec. Material graph in assistant-eye-shader.md v0.3.*
