# Inference Test Results

**Date:** 2026-03-18
**Hardware:** Beelink SER5850U, Vega 8 iGPU (CPU inference)  
**Model:** phi3.5  

## Stats

| Metric | Value |
|--------|-------|
| Total time | 24.36s |
| Output tokens | 178 |
| Tokens/sec | 11.6 |

## Prompt

```
You are a quest generator for an open-world game. The player is at a real-world location.

Location: Central Park, New York City (40.7851° N, 73.9683° W)
Nearby landmarks: Bethesda Fountain, The Ramble, Bow Bridge
Time of day: dusk
Faction context: The player is aligned with the Wardens (urban preservationists vs. Scavengers)

Generate a short quest (3-5 sentences) that:
- Ties to the real location and its landmarks
- Fits the Wardens faction theme
- Has a clear objective and reward hint
- Feels grounded, not generic

Quest:
```

## Response

As dusk sets over Central Park with Bethesda's fountain glistening in the waning light, you find yourself entrusted by your Wardens faction comrades to protect this green heart amidst urban decay. They task you with a covert operation: surveil The Ramble for any signs of Scavenger activity under the veil of twilight and report back within an hour using their secure channel—a flash drive hidden in one of your own shadows, if necessary. Your reward will be rare seeds from local flora that you'll plant at Bow Bridge to help restore its natural beauty as a symbolic gesture against Scavenger encroachment upon this sanctuary. Remember, vigilance is key; the Park breathes secrets in silence and dusk whispers them before nightfall descends.

## GPU Offload Investigation

- `/dev/kfd` present, AMDGPU kernel driver loaded
- Vega 8 is gfx90c (Cezanne APU, device 0x1638) — partial ROCm support
- ROCm userspace (`rocm-opencl-runtime`, `hip-runtime-amd`) not installed
- **Decision:** skip GPU offload on this hardware — Vega 8 APU ROCm is unreliable and this is backup hardware
- Re-test GPU offload on main system (RX 5700 / RDNA2, 8GB VRAM, dedicated) when it arrives

## Notes

- 11.6 t/s CPU-only is acceptable for **async pre-generation** (generate quests ahead of player position)
- Too slow for real-time / on-demand responses
- Output quality is good — location-aware, faction-appropriate
- Memory is tight on Beelink (3.5 GiB free / 11.6 GiB total); keep one model loaded at a time
