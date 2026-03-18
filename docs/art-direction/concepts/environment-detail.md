# Concept Brief — Environment Detail: The Before, Children, Untenanted Space
**Version:** 0.1 | **Last updated:** 2026-03-17

---

## The Before

The GPS home-spawn only works emotionally if the world feels like the player's actual neighborhood. Part of that is the architecture, the streets, the geography. Part of it is the accumulated evidence of the world that used to exist here — and doesn't anymore.

**Design principle:** Every environment should contain evidence of before. Not dramatically, not in a way that stops the player for a cutscene — as ambient fact. The world used to be something else. Whatever it's become now, the before is still in it.

### What "Before" Looks Like

**Persistent personal objects:**
Items with no current utility that someone kept anyway. A framed photo still on a wall because taking it down felt like giving up. A car in the driveway that hasn't moved in eighteen months, still with a parking permit on the dashboard. A school backpack hung by the door of a Holdout community space — not because they need it, because they haven't taken it down.

These objects are not highlighted. They're not quest markers. They are in the environment the way they would be in reality: present, personal, doing nothing.

**Commercial detritus:**
Pre-Cascade commercial landscape fossilized in place. Shop signage still mounted, for businesses that stopped operating on the day of the Cascade. A franchise coffee shop with the furniture still inside and the menu board still lit because the emergency power is still running. The corporate aesthetic of the before world — glossy, designed for a customer base that no longer exists — reads as profoundly strange against the current world.

**Infrastructure ongoing:**
Some infrastructure has kept running without human intervention. Traffic lights cycling on timers. Automated irrigation running on schedule in a park nobody maintains anymore. Vending machines with power still on, product still inside, unchanged prices. These run on their own logic. Nobody turned them off. Nobody is using them. They persist.

**The Remnant Automation connection:**
The pre-Cascade commercial landscape and the Remnant Automation units are related. Remnant machines were also running on their last program, in spaces designed for a world that ended. The thematic echo is intentional — the environment and some of the enemy type are part of the same category of thing: the world's ongoing processes, now serving nobody.

### Location-Specific Before

The before should be GPS-typed where possible:

| Real-world zone | Characteristic "before" elements |
|---|---|
| Urban residential | School run detritus, community notice boards with outdated notices, abandoned commuter infrastructure |
| Urban commercial | Frozen retail environments, delivery infrastructure mid-route, corporate signage |
| Suburban | Domestic life fossilized — lawns mid-maintenance, home improvement projects abandoned mid-task |
| Industrial | Operational logs cut off at the Cascade, safety records that stop on a specific date, personal lockers still with their occupants' things |
| Rural | Agricultural cycles that continued without oversight — a harvest that rotted, a planting that wasn't gathered |
| Coastal | Vessels that didn't return to port, maritime services that stopped mid-operation |

---

## Post-Cascade Children

The first generation growing up post-Cascade is old enough to matter. Some were born into this world. Some were young enough at the Cascade that they have no reliable memory of before.

**They are not traumatized in the way their parents are.** They've never known anything else. The overlay is normal to them. Fractured movement signatures are learnable, not terrifying. The twin's data layer is a fact of the world they grew up in, not an intrusion into it.

**What they're better at:**
- Reading machine signatures at a glance — they grew up doing it
- Navigating Fractured behavior patterns — they've been learning since before they were in danger
- The overlay — they treat it as a second layer of reality because for them it always has been
- Stillness and patience with machine-adjacent phenomena that make adults flinch

**What this looks like visually:**

Children in functional Holdout communities dress in the repaired and modified aesthetic of their parents, but with modifications that reflect *their* generation. They've incorporated overlay-referencing aesthetic elements into their personal gear. They carry things specifically for machine encounter prep — not weapons, but observational tools. A child in a healthy Holdout looks practical, slightly over-equipped for their age, and completely comfortable in the environment.

Children in communities that have stopped investing in normalcy look different. Quiet, watchful, carrying adult competencies in wrong proportions. Less personalization, more utility. The absence of childhood's visual markers is the tell.

**The comparison:**

| Community type | Child visual character |
|---|---|
| Invested in normalcy | Repaired, personalized, overlays-as-jewelry, practical additions to civilian base. Looks like a kid in a hard world. |
| Stopped trying | Utility-dominant, minimal personalization, watchful posture, adult competencies visible in gear choices. Looks like a small adult in a hard world. |

**Movement language:** Post-Cascade children move differently from adults in one consistent way — they are more comfortable with machine-proximity. An adult flinches at a Remnant Automation cycle. A child who grew up in that neighborhood learned the pattern at age six and routes around it automatically. This is expressed in animation: children move through machine-populated environments without the hesitation adults show.

---

## Untenanted Territory

The space between faction claims. No one owns it. The overlay reflects this.

**What untenanted territory looks like:**

**Physical:** Not ruined, necessarily — just unmanaged. The before-world's infrastructure continuing or decaying based purely on what it was built to do without maintenance. A road that's still passable but has plants coming through the asphalt at the edges. A building still standing but without the small maintenance actions that keep buildings habitable — a window broken and not replaced, gutters blocked, the small decay that accumulates without anyone caring.

**Overlay:** In untenanted territory, the twin's overlay is present but sparse. The twin indexes the space — it knows the coordinates, the structures, the physical fact of it. What's absent is the human annotation layer. No Warden Historical Markers unless something happened here worth documenting. No faction territory color. No community-generated data density. The overlay in untenanted territory reads like a place the twin knows about but has not been given context for.

**Visual language for the overlay in untenanted territory:**
- `--overlay-primary` at reduced opacity — the twin is present but not dense
- No territory boundary lines
- Entity markers for machine faction activity (Remnant, Fractured, Coherent construction if present) are present — the twin can see machines. It has less human data.
- A specific quality of emptiness in the overlay that players learn to read as "no one holds this"

**The untenanted space as frontier:**
Untenanted territory is both opportunity and hazard. Factions want to claim it. Fractured move into it when their territories expand. Players can build Holdout presence in it, which the twin begins to index as human activity. The overlay filling in as a community establishes itself is a visible progression — the data layer gains density as people arrive and stay.

**Contested territory:**
Territory being actively disputed between two factions has a specific overlay state — the territory boundary lines flicker between two faction colors at the boundary, with a contested zone between them where both colors desaturate toward the `--overlay-primary` base. The twin does not know who owns contested space. It shows both claims and their conflict.

---

## Environmental Storytelling Integration

The "before" elements, post-Cascade children, and untenanted territory are all environmental storytelling tools. They do not use UI. They use world geometry.

**Hierarchy:**
1. The world tells a story through what's in it
2. The overlay adds a data layer to that story
3. Warden Historical Markers provide documentary anchors where events happened
4. Players who read all three layers understand the specific history of a specific GPS location

A player returning to their real-world home coordinates in-game finds: the physical architecture of before, the accumulated decay or survival of eighteen months, the overlay's annotation of what happened here, and — if a Warden was present for anything significant — the Record.

This is the game's core emotional proposition. The visual system must serve it.
