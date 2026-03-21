# Bittensor Subnet: Migration Architecture

**Status:** Planning — local ollama pipeline must be validated first
**Target:** Production inference for quest/dialogue/personality generation
**Decision log:** "AI Pipeline: Local First → Bittensor"

---

## Why Bittensor

Local inference works for dev and sub-minimum hardware but has a ceiling:
- Beelink (current): 11.6 t/s, async-only, one request at a time
- RX 5700 (arriving): ~40–60 t/s est., on-demand viable for single player
- **At scale (100+ concurrent players):** local inference per player is not feasible; a shared inference network is required

Bittensor provides a decentralised GPU inference market. Miners (GPU operators) compete to serve inference requests; validators score quality and set rewards. This avoids running centralised inference servers while accessing aggregate GPU capacity.

---

## What moves to Bittensor

| System | Local (now) | Bittensor (production) |
|--------|-------------|------------------------|
| Quest pre-gen | ollama/phi3.5, 1 player | Subnet miners, N players concurrent |
| NPC dialogue | ollama/phi3.5, on-demand | Subnet miners, low-latency |
| Opponent personality | ollama/phi3.5, at spawn | Subnet miners, at spawn |
| Assistant lines | ollama/phi3.5, ambient | Subnet miners OR stays local (latency-sensitive) |

**Assistant lines** are the exception: Stage 1–2 lines are short and latency-critical (player expects near-instant response). Keep those local. Stage 3–5 ambient lines are candidates for subnet if local GPU is loaded.

---

## Architecture

### Current (local)

```
Game Client
    └── UWYAAISubsystem
            └── HTTP POST → ollama (127.0.0.1:11434)
                    └── phi3.5 / Mistral-7B (local GPU/CPU)
```

### Target (Bittensor)

```
Game Client
    └── UWYAAISubsystem
            └── HTTP POST → WYA Inference Gateway (fly.dev or self-hosted)
                    └── Bittensor Validator Node
                            └── Subnet miners (distributed GPU network)
```

The **WYA Inference Gateway** is a thin proxy:
- Accepts the same JSON schema as ollama (`{model, prompt, stream}`) → drop-in swap for `UWYAAISubsystem`
- Authenticates the request (player session token from backend)
- Routes to the Bittensor validator
- Returns `{response: "..."}` in ollama-compatible format

`UWYAAISubsystem::OllamaURL` is already a configurable `FString` — changing the endpoint is a config change, not a code change.

---

## Subnet design options

### Option A: Use an existing text generation subnet
- Subnet 18 (Cortex.t) or Subnet 4 (Manifold) offer open text generation
- Pros: no mining infrastructure to build; can test immediately
- Cons: no model control, no quality SLA, output not filtered for game content
- **Verdict:** viable for early testing only

### Option B: Deploy a WYA-specific subnet (TAO)
- Register a new subnet with game-specific validator logic
- Validator scores miner responses on: format compliance, location relevance, faction consistency
- Miners run Mistral-7B or Llama-3.1-8B Q4_K_M (matches minimum spec target)
- Pros: full control over model, prompts, and quality scoring
- Cons: requires TAO stake to register subnet (~1000 TAO at time of writing); significant infra work
- **Verdict:** production target, post-Early Access

### Option C: Hybrid — subnet for quests, local for combat
- Quest/dialogue gen (latency-tolerant, async) → subnet
- Opponent personality (spawn-time, ~20s budget) → local GPU
- Assistant lines (Stage 1–2) → local GPU always
- **Verdict:** recommended migration path

---

## Migration path

### Phase 1 — Local validated (current)
- [x] ollama/phi3.5 CPU inference working
- [x] `UWYAAISubsystem` HTTP interface complete
- [x] Async pre-gen buffer (DesiredBufferSize=3)
- [ ] RX 5700 GPU offload test (Mistral-7B Q4_K_M, ROCm)
- [ ] Measure t/s and latency at minimum spec

### Phase 2 — Gateway + external subnet (pre-EA)
- [ ] Deploy WYA Inference Gateway (fly.dev, same infra as `where-you-are-api`)
- [ ] Implement ollama-compatible API in gateway
- [ ] Test with Subnet 18 (Cortex.t) — validate format compliance
- [ ] Add content filter layer in gateway (block raw LLM output with no game context)
- [ ] Swap `OllamaURL` to gateway endpoint in shipping config
- [ ] Fallback: if gateway unreachable, fall back to local ollama

### Phase 3 — WYA subnet (post-EA, if warranted)
- [ ] Evaluate TAO registration cost vs. server cost at player scale
- [ ] Design validator scoring rubric (format + location relevance + faction tone)
- [ ] Set up miner reference implementation (Mistral-7B Q4_K_M, ollama-based)
- [ ] Register subnet, onboard initial miners
- [ ] Migrate gateway to route to WYA subnet

---

## Content filter (required before any subnet use)

CLAUDE.md: "All AI output must be filterable — no raw LLM output direct to players"

The gateway must filter all responses before returning to the client:

```
Gateway filter pipeline:
    1. Length check — reject if < 10 chars or > 1000 chars
    2. Format check — quest/personality responses must contain expected keys (TITLE:/BODY:, AGGRESSION:, etc.)
    3. Content filter — block responses containing slurs, real-world harm, PII patterns
    4. Fallback — if filter rejects, return a pre-authored fallback response (not an error)
```

Pre-authored fallbacks per type:
- Quest: `{Title: "Local Disturbance", Body: "Warden scouts have reported unusual activity nearby. Investigate and report back."}`
- Personality: default `FWYAOpponentPersonality{}` struct values
- Dialogue: `"Stay sharp. This place isn't safe."` (generic Warden line)
- Assistant: stage-appropriate static fallback line

---

## Cost model (rough)

| Scenario | Inference source | Est. cost |
|----------|-----------------|-----------|
| Dev / sub-minimum player | Local CPU | $0 (electricity) |
| Minimum spec player | Local GPU (RX 5700) | $0 |
| Server-side (Subnet 18) | ~$0.0001–0.001 per request | ~$0.01–0.10 per player per session |
| WYA subnet miners | TAO token rewards | TBD at registration |

At 10,000 DAU × 10 quest requests/session = 100,000 requests/day.
At $0.001/request = $100/day = $3,000/month — comparable to a mid-tier GPU server.
Local GPU inference keeps this at $0 for minimum-spec players, which is the majority.

**Decision deferred:** Don't optimise subnet costs until player scale is known.
