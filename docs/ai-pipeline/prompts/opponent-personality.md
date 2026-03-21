# Prompt: Opponent Personality

**Used by:** `UWYAAISubsystem::GenerateOpponentPersonality()`
**Called from:** `AWYAOpponentController::OnPossess()` — once per spawned opponent
**Model:** phi3.5 (dev) → Mistral-7B / Llama-3.1-8B Q4_K_M (minimum spec)
**Output format:** Strict key:value lines — parsed into `FWYAOpponentPersonality`

---

## Template

```
You are configuring an enemy NPC for an open-world shooter game.

Faction: {FactionTag}
Location: {LocationContext}

Respond with exactly these lines and nothing else:
AGGRESSION: <0.0 to 1.0>
RANGE: <close|mid|long>
RETREAT: <0.0 to 1.0>
FLANK: <0.0 to 1.0>
TAUNT: <short combat line>
TAUNT: <short combat line>
TAUNT: <short combat line>

AGGRESSION:
```

**Variables:**
- `{FactionTag}` — faction name, e.g. `"Scavenger"`, `"Iron Compact"`, `"Warden Enforcer"`
- `{LocationContext}` — from `WYALocationNameResolver`, e.g. `"industrial district, Calgary"`

---

## Parsed fields → Blackboard keys

| Prompt key | Type | BB key | Notes |
|------------|------|--------|-------|
| `AGGRESSION` | Float 0–1 | `AggressionLevel` | 0=passive/defensive, 1=berserker |
| `RANGE` | Enum | `PreferredRange` | close=0, mid=1, long=2 |
| `RETREAT` | Float 0–1 | `RetreatThreshold` | Health fraction to trigger retreat |
| `FLANK` | Float 0–1 | `FlankingTendency` | 0=straight charge, 1=always flanks |
| `TAUNT` (×3) | String[] | (audio/subtitle) | Combat callouts |
| *(implicit)* | Bool | `PersonalityReady` | Set true when all fields parsed |

**Fallback defaults** (written to BB immediately on possess, before LLM responds):
- AggressionLevel = 0.5, Range = Mid, RetreatThreshold = 0.25, FlankingTendency = 0.5
- TauntLines = `["You shouldn't have come here."]`

---

## Faction personality guidelines

These are suggestions for the LLM — not hardcoded. The model should vary within each faction.

| Faction | Expected bias |
|---------|---------------|
| Scavenger | High aggression (0.7–0.9), close/mid range, low retreat (fights dirty) |
| Iron Compact | Mid aggression (0.4–0.6), mid/long range, moderate flank tendency |
| Warden Enforcer | Low aggression (0.3–0.5), mid range, high retreat (disciplined, not suicidal) |
| Fractured | Unpredictable — all values randomised by zone type |

---

## Tuning notes

- Trailing `AGGRESSION:` in the prompt forces the model to complete the format (same technique as quest prompt)
- The parser is lenient — if a line is missing or malformed, the field keeps its fallback default
- TAUNTs should be short (<8 words) for subtitle display; longer lines are truncated at the audio layer
- The BT **must** gate personality-driven decisions behind `BB_PersonalityReady` — at 11.6 t/s the LLM response arrives ~20–25s after spawn, well after combat may have started
