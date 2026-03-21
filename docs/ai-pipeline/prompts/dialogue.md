# Prompt: NPC Dialogue

**Used by:** `UWYAAISubsystem::GenerateDialogue()`
**Model:** phi3.5 (dev) → Mistral-7B / Llama-3.1-8B Q4_K_M (minimum spec)
**Output format:** Plain prose, 2-3 sentences, no stage directions or quotes

---

## Template

```
You are writing dialogue for an NPC in an open-world game.

NPC: {NPCName} ({NPCRole})
Situation: {PlayerContext}

Write 2-3 sentences of in-character dialogue. No stage directions, no quotes. Dialogue:
```

**Variables:**
- `{NPCName}` — NPC identifier, e.g. `"Mira"` or `"Warden Scout"`
- `{NPCRole}` — short role description, e.g. `"Warden scout, suspicious of newcomers"`
- `{PlayerContext}` — what the player just did or said, e.g. `"Player just arrived at the checkpoint for the first time"`

---

## Tuning notes

- "No stage directions, no quotes" is load-bearing — without it phi3.5 tends to wrap output in `"..."` or add `*nods*`
- Keep `{NPCRole}` to 5–8 words — longer descriptions cause the model to summarise rather than embody
- `{PlayerContext}` should describe the *situation*, not script a response — let the model infer the tone
- 2-3 sentences is calibrated for subtitle display; 1 sentence is often too thin, 4+ is too long for the UI budget

---

## Example

**Input:**
- NPCName: `Kael`
- NPCRole: `Iron Compact trader, guarded but fair`
- PlayerContext: `Player is trying to barter water rations for ammo, first meeting`

**Expected output style:**
```
Water's worth something out here, I'll give you that. Show me what you've got
and we'll see if it's worth my last clip. Don't waste my time with the cheap stuff.
```
