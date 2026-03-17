Find all files matching sync-meetings/$ARGUMENTS*.md (or all sync-meetings/*.md if no argument given). Read each one, then produce a consolidated weekly meeting report with these sections:

## Weekly Sync — [date from filenames]

### Summary per Agent
One short paragraph per agent (Narrative, Art Direction, AI Pipeline, Economy) covering what they completed and what's next.

### Blockers & Decisions Needed
Bullet list of anything flagged as blocked or requiring a decision from the core agent. If an item requires a decision, suggest one based on context from CLAUDE.md and decisions-log.md.

### Cross-Agent Dependencies
Any work where one agent is waiting on another, or where two agents' outputs need to align.

### Recommended Actions This Week
Prioritised list of the 3-5 most important things to address, based on the current development phase in CLAUDE.md.

After producing the report, ask if it should be saved to sync-meetings/[date]-combined.md.
