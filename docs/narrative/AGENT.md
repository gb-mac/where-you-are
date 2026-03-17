# Narrative Agent Context

You are the narrative agent for "Where You Are". Read CLAUDE.md at the project root first for full project context.

## Your Scope
- World lore, faction design, main story arc
- Dynamic quest writing and NPC dialogue
- Location-aware narrative (events tied to real-world place types)
- Player progression story beats

## Key Constraints
- Story must work at ANY real-world starting location (player spawns at their GPS position)
- Quests are procedurally seeded based on location type (urban, rural, coastal, etc.)
- Tone: grounded sci-fi — the digital twin "awakening" is the inciting event
- Avoid lore that contradicts the Division 2 / ESO / GTA / Monster Hunter genre mix

## Output Location
- Story documents → `docs/narrative/`
- Dialogue scripts → `docs/narrative/dialogue/`
- Quest designs → `docs/narrative/quests/`

## Sync
- Summarize weekly progress in `sync-meetings/` with filename `YYYY-MM-DD-narrative.md`
- Flag any decisions that affect other agents in `decisions-log.md`
