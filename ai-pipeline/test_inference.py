#!/usr/bin/env python3
"""
CPU inference test for Where You Are AI pipeline.
Tests ollama/phi3.5 with a location-based quest generation prompt.
"""

import json
import time
import urllib.request
import urllib.error

OLLAMA_URL = "http://127.0.0.1:11434/api/generate"
MODEL = "phi3.5"

QUEST_PROMPT = """You are a quest generator for an open-world game. The player is at a real-world location.

Location: Central Park, New York City (40.7851° N, 73.9683° W)
Nearby landmarks: Bethesda Fountain, The Ramble, Bow Bridge
Time of day: dusk
Faction context: The player is aligned with the Wardens (urban preservationists vs. Scavengers)

Generate a short quest (3-5 sentences) that:
- Ties to the real location and its landmarks
- Fits the Wardens faction theme
- Has a clear objective and reward hint
- Feels grounded, not generic

Quest:"""


def test_inference():
    payload = json.dumps({
        "model": MODEL,
        "prompt": QUEST_PROMPT,
        "stream": False,
    }).encode()

    print(f"Model: {MODEL}")
    print(f"Prompt tokens (approx): {len(QUEST_PROMPT.split())}")
    print("Sending request...\n")

    start = time.time()
    try:
        req = urllib.request.Request(
            OLLAMA_URL,
            data=payload,
            headers={"Content-Type": "application/json"},
        )
        with urllib.request.urlopen(req, timeout=120) as resp:
            result = json.loads(resp.read())
    except urllib.error.URLError as e:
        print(f"ERROR: Could not reach ollama at {OLLAMA_URL}")
        print(f"  {e}")
        print("  Is ollama running? Try: systemctl start ollama")
        return
    elapsed = time.time() - start

    response_text = result.get("response", "").strip()
    eval_count = result.get("eval_count", 0)
    eval_duration_ns = result.get("eval_duration", 0)
    tokens_per_sec = eval_count / (eval_duration_ns / 1e9) if eval_duration_ns else 0

    print("=== RESPONSE ===")
    print(response_text)
    print("\n=== STATS ===")
    print(f"  Total time:     {elapsed:.2f}s")
    print(f"  Output tokens:  {eval_count}")
    print(f"  Tokens/sec:     {tokens_per_sec:.1f}")
    print(f"  GPU offload:    {result.get('model', MODEL)}")

    # Write results to doc
    doc_path = "docs/ai-pipeline/inference-test-results.md"
    with open(doc_path, "w") as f:
        f.write(f"# Inference Test Results\n\n")
        f.write(f"**Date:** 2026-03-17  \n")
        f.write(f"**Hardware:** Beelink SER5850U, Vega 8 iGPU (CPU inference)  \n")
        f.write(f"**Model:** {MODEL}  \n\n")
        f.write(f"## Stats\n\n")
        f.write(f"| Metric | Value |\n|--------|-------|\n")
        f.write(f"| Total time | {elapsed:.2f}s |\n")
        f.write(f"| Output tokens | {eval_count} |\n")
        f.write(f"| Tokens/sec | {tokens_per_sec:.1f} |\n\n")
        f.write(f"## Prompt\n\n```\n{QUEST_PROMPT.strip()}\n```\n\n")
        f.write(f"## Response\n\n{response_text}\n\n")
        f.write(f"## Notes\n\n")
        f.write(f"- ollama ROCm build installed; Vega 8 GPU offload TBD\n")
        f.write(f"- Acceptable for background NPC generation; too slow for real-time\n")
        f.write(f"- Re-test on main system (RX 5700, ROCm) when hardware arrives\n")
    print(f"\nResults written to {doc_path}")


if __name__ == "__main__":
    test_inference()
