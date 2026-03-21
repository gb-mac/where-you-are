# Inference Test Results

**Date:** 2026-03-17  
**Hardware:** Beelink SER5850U, Vega 8 iGPU (CPU inference)  
**Model:** phi3.5  

## Stats

| Metric | Value |
|--------|-------|
| Total time | 21.49s |
| Output tokens | 163 |
| Tokens/sec | 11.3 |

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

As dusk falls over Central Park, you notice Bethesda Fountain's lights flickering irregularly. A message from The Warden Council has been received; they suspect someone is sabotaging the park’thy cherished water source to rally more scavengers into their ranks against urban preservationists like yourself. Your mission: Investigate and restore Bethesda Fountain's lights, ensuring its protection as a symbol of hope for those who value nature amidst concrete jungles. Successful completion will earn you the honorary title "Guardian" along with rare warden coins hidden within your local Warden outpost tonight.

Quest: Investigate and Fix Bethesda Fountain's Light Sabotage

## Notes

- ollama ROCm build installed; Vega 8 GPU offload TBD
- Acceptable for background NPC generation; too slow for real-time
- Re-test on main system (RX 5700, ROCm) when hardware arrives
