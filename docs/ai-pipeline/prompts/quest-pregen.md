# Prompt: Pre-Generated Side Quest

**Used by:** `UWYAAISubsystem::ScheduleOnePregen()`
**Model:** phi3.5 (dev) → Mistral-7B / Llama-3.1-8B Q4_K_M (minimum spec)
**Output format:** TITLE: / BODY: two-line structured response

---

## Template

```
You are a quest generator for an open-world game. The player just spawned at their real-world location.

Location: {LocationDisplay}
Faction context: Wardens (urban preservationists fighting Scavenger encroachment)

Respond with exactly two lines and nothing else:
TITLE: a 2-5 word quest title
BODY: a 3-5 sentence quest description tied to this location, with a clear objective and reward hint.

TITLE:
```

**Variables:**
- `{LocationDisplay}` — from `WYALocationNameResolver`, e.g. `"Prince's Island Park, Calgary"`. Falls back to `"lat, lon"` if Nominatim unreachable.

---

## Parsing

The C++ parser (`UWYAAISubsystem::ParseTitleBody`) expects:
- Model continues from the trailing `TITLE:` in the prompt
- Line 1: the title text (may or may not echo `TITLE:` prefix — both handled)
- Line 2: `BODY:` followed by the quest description

If the model doesn't follow the format, the full response is used as the body with title fallback `"Local Disturbance"`.

---

## Tuning notes

- Keep `TITLE:` as the prompt continuation — forces the model to complete the format rather than invent a new one
- Wardens faction is hardcoded here; the on-demand `GenerateQuest()` API takes `FactionContext` as a parameter for multi-faction support
- 3-5 sentence body is the sweet spot for phi3.5 at 11.6 t/s — longer = slower, shorter = thin
- "reward hint" keeps quests feel purposeful without requiring the AI to invent game mechanics

---

## Example output (phi3.5, Calgary default, 2026-03-18)

**Location:** `51.0447, -114.0719` (coord fallback, no Nominatim in test)

```
TITLE: Shadows Over the Ramble
BODY: As dusk settles over the city, Warden scouts have gone silent near the
river path. Your objective: reach their last known position, recover the
encrypted data chip, and return to the safe house before Scavenger patrols
lock down the sector. Reward: Warden faction rep + salvage cache coordinates.
```

*(Actual test used Central Park prompt — see `inference-test-results.md`)*
