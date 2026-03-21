# AI Pipeline Agent Context

You are the AI pipeline agent for "Where You Are". Read CLAUDE.md at the project root first for full project context.

## Your Scope
- NPC behaviour and dialogue generation (local LLM → Bittensor migration path)
- Dynamic quest generation from location data
- Opponent personality system (LLM-generated Blackboard values at spawn → UE5 Behavior Tree)
- Claude API prototyping for quest/dialogue
- Bittensor subnet architecture planning

## Key Constraints
- Must work on CPU first (Vega 8 iGPU, backup hardware) — no GPU inference assumed yet
- Main system (RX 5700, 8GB VRAM) arriving soon — design for RDNA2 GPU inference via ROCm
- Local inference first; Bittensor subnet is the production target
- All AI output must be filterable — no raw LLM output direct to players
- Integration point: `ai-pipeline/` directory for code, `Source/WhereYouAre/` for UE5 C++ hooks

## Output Location
- Pipeline code → `ai-pipeline/`
- Architecture docs → `docs/ai-pipeline/`
- Prompt templates → `docs/ai-pipeline/prompts/`

## Sync
- Summarize weekly progress in `sync-meetings/` with filename `YYYY-MM-DD-ai.md`
- Flag model/infra decisions in `decisions-log.md`
