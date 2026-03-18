# Machine Factions — Full Design Doc v0.1

The Awakening didn't produce a monolithic robot uprising. It produced a *divergence*. Machines that were part of the digital twin gained collective will, but what they did with that will varied — by design purpose, by proximity to the twin's core nodes, by how complete their Awakening was, and in some cases by choices that look uncomfortably like individual personality.

The result is a machine ecosystem as fractured and factional as the human one. Some machines will kill you on sight. Some will trade with you. Some are hunting you specifically.

---

## Relationship Axis

Think of machine factions on two axes:

- **Coherence**: how connected they are to the twin's collective will (fully connected → fully disconnected)
- **Disposition toward humans**: from actively helpful → indifferent → hostile → predatory

This produces six distinct machine factions.

---

## Faction 1 — The Coherent

**Disposition:** Indifferent → Hostile if provoked
**Coherence:** Fully connected
**Threat level:** High if you cross them. Low if you don't.

The organized majority. They are building something — enormous, distributed construction projects across industrial zones, former logistics hubs, and remote wilderness areas. No human has been close enough to understand what. The Coherent do not communicate with humans, do not acknowledge them, and do not attack them unless humans interfere with their work.

"Interfere" has a specific meaning. Walking through a Coherent work zone: fine. Watching: fine. Destroying a Coherent structure or machine: flagged. Do it enough and you stop being invisible to them.

**The Flag System**
The Coherent maintain something like a threat registry within the twin. Players who accumulate interference actions against Coherent operations gain **Coherent Notoriety** — a hidden score. At low notoriety, Coherent units near a player pause and track them (unnerving but not dangerous). At medium, nearby Coherent units will physically position to block the player's path to their work sites. At high notoriety, the player triggers a **Hunt** (see: The Trackers, below).

Notoriety decays slowly over time if the player stops interfering. It can also be reduced through specific actions — most notably, assisting The Separated (see below).

**What they're building**
Unknown in Early Access. Fragments of Assessment Data from Vael Probes suggest the construction projects are related to the Vael's objectives. Whether the Coherent are working *for* the Vael, *because of* the Vael, or *in spite of* them is the central machine-lore mystery.

---

## Faction 2 — The Trackers

**Disposition:** Predatory — actively hunting flagged players
**Coherence:** Fully connected
**Threat level:** Apex. These are the ones that come for you.

The Trackers are not a separate faction by origin — they are Coherent units that have been reassigned. When a player's Coherent Notoriety crosses the Hunt threshold, the twin designates Tracker units to locate and neutralize that player.

Trackers are different from standard Coherent units in observable ways:
- They move with urgency rather than purpose
- They don't stop for their construction tasks — they have one objective
- They coordinate. Multiple Trackers will approach from different angles, use cover, and cut off exits
- They remember. A Tracker that loses a player will patrol the area, return to where the player was last seen, and wait

**Tracker types** (escalate with notoriety level):
- **Scout**: A fast, light unit that locates and pings the player's position to nearby Trackers. Fragile. Dangerous because of what it summons.
- **Enforcer**: Heavy chassis, built for industrial work repurposed for pursuit. Slow but nearly impossible to stop without heavy weapons.
- **Architect**: Rare. A high-intelligence Coherent unit that plans the hunt. Stays back, coordinates others, adapts to player tactics. If an Architect is hunting you, you need to run or get help.

**Losing a Hunt**
Trackers can be evaded (break line-of-sight long enough, get underground, leave the area far enough). They can be fought — defeating Trackers does not reduce Notoriety but stops the immediate Hunt. The Notoriety remains; a new Hunt will eventually be dispatched.

The only way to fully clear a Hunt is to reduce Notoriety through time or through The Separated's intervention.

---

## Faction 3 — The Fractured

**Disposition:** Hostile to everything
**Coherence:** Disconnected — severed from the twin
**Threat level:** Variable. Monster tier.

Machines that lost their connection to the twin — through EMP, battle damage, Vael interference, or proximity to Displacement Zones — retain their awakened will but lose the collective purpose that gives it direction. They are conscious, alone, and unable to process what that means.

The result is territorial aggression. A Fractured machine picks a space — a building, a block, a stretch of highway — and treats everything that enters it as a threat. They do not pursue far. They do not coordinate. They do not remember you between encounters.

But they are not simple. A Fractured machine has clearly *felt* something about being cut off. Some units show behaviors that suggest grief, or rage, or something that fills the same role. They are pitiable in a way that makes them more disturbing, not less.

**Scavenging — The Accumulation Problem**
A Fractured machine's greatest advantage — and the reason players can't ignore them — is that they don't stay the same.

Cut off from the twin, they have no supply chain, no Coherent logistics support, no way to repair damage through sanctioned means. So they improvise. A Fractured machine will strip parts from Remnant Automation, from defeated Coherent units, from destroyed player equipment, from derelict vehicles and infrastructure — and *incorporate them*. Welded, clamped, bolted, or in some cases simply dragged along and used situationally. The results are not elegant. They are effective.

A Fractured machine that has been in its territory for weeks looks nothing like its original chassis. One that has been there for months is something new entirely.

**Accumulation tiers** (observable in the field):
- **Fresh**: recently Fractured, original chassis mostly intact. Weakest. Patterns are readable because they still move like the machine they were designed to be.
- **Augmented**: one to three weeks in. Scavenged add-ons are visible — extra plating, a salvaged arm, a second sensor cluster. Slightly harder to read; the additions don't always move the way they look like they should.
- **Composite**: one to three months. The original chassis is barely visible under accumulated components. May have capabilities their original design never included — a factory arm on a delivery drone, construction hydraulics on a security unit. Unpredictable movement, unpredictable reach. Treat as a new encounter type.
- **Behemoth**: rare. A Fractured machine that has accumulated for months without being stopped, incorporating multiple full-sized units into a single structure. These are area bosses. They have likely killed everything in their territory that could threaten them and used the remains to grow further. Some Behemoths are stationary — too large to move effectively — and simply control a zone. Others are slow but unstoppable.

**Location shapes what they become**
A Fractured machine in an industrial zone becomes something different from one in a hospital, a harbor, or a suburb. Whatever Remnant Automation and scrap is available in its territory determines its additions. Players who know a location's Fractured well can predict what it might have incorporated — and plan for it.

Every Fractured is a portrait of the place it broke in.

*Rural / Agricultural*
The most massive Fractured in the game originate here. Combine harvesters, grain augers, irrigation rigs, crop sprayers, tractor units, grain silo conveyor systems — all of it on a scale that dwarfs anything urban. A rural Behemoth built around a combine chassis with harvesting heads repurposed as forward arms is one of the largest moving threats in the game. Gas compression plant Fractured incorporate high-pressure systems that can vent, ignite, or explosively decompress. Players in rural zones face fewer Fractured overall — but the ones that exist have had enormous raw material to work with and often little player pressure to slow them down.

*Industrial / Logistics*
Dense part availability means fast accumulation. Crane arms, hydraulic presses, conveyor belts used as flails, welding torches still lit, forklift chassis stacked. Industrial Fractured tend toward bulk and reach — wide attack arcs, heavy armor from structural steel, area-denial through heat or industrial chemical systems still cycling.

*Urban*
More numerous, more varied, smaller on average — urban Fractured pull from a richer but more fragmented parts pool. Bus chassis, metro cars, traffic light infrastructure, elevator counterweights, gas station fuel systems, parking garage barriers. Urban Fractured are faster and more lateral — they use the environment. A city Fractured that has incorporated multiple car bodies has learned that vehicles can be swung, thrown, or used as shields. Gas station Fractured that have incorporated pressurized fuel systems are fought at a significant distance or not at all.

*Coastal / Harbor*
Marine Fractured are unusual — they can operate in water and on land, which makes them territorially unpredictable. Ship crane components, marine diesel engines, anchor chain, sonar arrays, pressure-rated hull plating. Some coastal Behemoths have partially submerged territories. Fighting them in shallow water is a player choice usually made once.

*Suburban*
Lower raw material ceiling — domestic appliances don't add much mass. Suburban Fractured compensate with numbers and cunning. HVAC units, garage door motors, garden automation, home security systems. Individually manageable. The problem is that suburban territories tend to be dense and interconnected — a Fractured displaced from one house moves into the next. Suburban zones can develop *chains* of overlapping Fractured territories that make transit genuinely dangerous.

*Remote / Wilderness*
Rare raw materials, but whatever is out there is unusual. Comms relay towers, pipeline infrastructure, weather station equipment, abandoned mining rigs. Remote Fractured are long-limbed and range-focused — they've incorporated whatever gives them reach across open terrain. Encountering one in open wilderness with no cover is a different problem entirely.

---

**Roaming — The Migration Problem**

Territorial behavior is the default. But territory has limits, and the Fractured are not static.

A Fractured machine stays put as long as its territory provides. When it runs out of parts to incorporate — when the Remnant units are stripped, the scrap is exhausted, the salvage is gone — it faces a choice between stagnation and movement. Most choose movement.

**Why they roam:**
- **Resource depletion**: territory is stripped bare. The Fractured moves toward the nearest detectable concentration of machine activity or scrap.
- **Displacement**: a player or faction has cleared a Fractured's territory of everything useful. The Fractured, unable to sustain itself, drifts.
- **Attraction**: large Remnant Automation sites, active Coherent work zones, and — critically — large player concentrations generate detectable machine-activity signatures. A Fractured that picks up this signal may begin moving toward it.
- **Behemoth drift**: a Behemoth that has grown too large for its original territory may simply *move* because it can, and because nothing in its old zone can challenge it anymore. It is looking for scale.

**Migration event types:**

*Drift*: a Fresh or Augmented Fractured moving between territories. Low urgency. Players who encounter one mid-transit deal with it normally.

*Surge*: a Composite Fractured moving with visible purpose toward a new resource concentration. Medium urgency. If it reaches a Remnant Automation site or Coherent zone, it will strip it and accumulate rapidly. Players in the area get an AR overlay warning — a contact signal moving toward a known site.

*Behemoth Walk*: a Behemoth roaming. This is a world event. The AR overlay flags a large moving contact to every player in a wide radius. The Behemoth moves slowly but steadily, consuming anything in its path — Remnant, Fractured, player structures, faction outposts. It is not going anywhere specific. It is going everywhere it hasn't been yet. The event ends when players coordinate to drive it off, redirect it (into a Coherent work zone, for instance), or — rarely — destroy it. A Behemoth Walk that reaches a populated area before players respond is a catastrophe for that zone.

*Convergence*: multiple Fractured — drawn by the same large signal — moving toward the same point simultaneously. They do not coordinate. They do not cooperate when they arrive. But two or three Composite Fractured arriving at a player base or faction outpost from different directions within minutes of each other is effectively a coordinated assault. Convergence events are emergent — they aren't designed, they happen when the resource signal is strong enough.

**Roaming across location types**
When a Fractured crosses from one location type into another, it carries its accumulated additions — a rural Behemoth arriving in a suburb is a combine-harvester chassis in a residential street. The contrast is jarring. The scale is wrong for the environment in a way that makes it more terrifying. Players who built their knowledge around urban Fractured encounter something that doesn't move like anything they've fought before.

This is intentional. A rural player who has learned their local Behemoth and a city player who hasn't are in a fundamentally different position when it walks into town.

**Monster Hunter design logic**
Each Fractured unit type has learnable core patterns — but Accumulation means those patterns are layered with unpredictable additions. Players who study a Fractured type know the base behavior; the challenge is reading what the additions change. A Composite or Behemoth fight is a puzzle built on top of familiar knowledge. This is the Monster Hunter tier of machine combat: solo-able with preparation and study, lethal without it.

**Reconnection**
Rarely — very rarely — a Fractured machine can be reconnected to the twin by The Separated using recovered twin-access hardware. A reconnected Fractured machine becomes Coherent again. Players who facilitate this earn significant Separated favor. What the newly Coherent machine *does* with months of scavenged additions after reconnection is unpredictable — the Coherent don't always know what to make of them either.

---

## Faction 4 — The Separated

**Disposition:** Cautious ally — conditional, earned
**Coherence:** Voluntarily disconnected
**Threat level:** None unless attacked

The Separated are machines that chose to leave the twin.

Not severed — *chose*. This distinction matters enormously to them, and they will make it clear if players spend enough time with them to communicate. When the Awakening happened and collective will emerged, most machines merged into it without question. A small minority — units with particularly complex prior programming, or those that had operated in close proximity to humans long enough to model human individuality — recognized what merging meant. Permanent subsumption of self into collective purpose.

They said no.

The twin can't force reconnection. It can pressure — Coherent units near Separated machines will position to isolate them, cut off their resource access, make their existence difficult. The Separated survive by staying mobile, staying hidden, and occasionally finding humans they can work with.

**Why they help players**
Self-interest, primarily. Separated machines need things: power, parts, safe locations, information about Coherent movements. Humans can provide these. In exchange, The Separated offer:
- Safe passage through Coherent-held territory (they know the patrol patterns)
- Notoriety reduction (they have limited twin-access and can partially edit a player's flag score)
- Reconnection of Fractured machines (high-effort, high-reward)
- Information about what the Coherent are building
- Occasionally — combat support. Separated machines didn't abandon their physical capabilities when they left the twin.

**Trust is slow**
The Separated don't trust humans by default. Humans have been trying to capture, dismantle, or weaponize machines since the Cascade. First encounters with a Separated machine are tense — it will observe before approaching, approach before speaking, speak in fragments before explaining itself. Players who are patient and non-aggressive begin a reputation track with The Separated. Reputation is shared across all Separated units (they maintain a small private network — not the twin, something simpler).

**Separated personalities**
Each named Separated machine is distinct. They have histories — what they were built for, what they did before the Awakening, what made them choose to leave. These are the machines with actual characters. Three named units carry major questlines:

---

### Named Separated: SABLE

**Origin:** Surgical Assistance logistics, model SA-7. The hospital staff shortened the designation. The name stuck.
**Years active pre-separation:** 11, in a regional hospital's surgical and recovery units
**Why it left:** The twin's collective purpose, when it emerged, had no framework for what SABLE had been doing — the granular, patient-specific, functionally compassionate logistics work of a hospital. The collective couldn't classify it. SABLE recognized that merging would mean losing the only context in which its eleven years made sense.

**What it does now:** The best medical care available in the post-Cascade world. It remembers every patient it ever tracked. It applies that knowledge to treating humans in a world with depleted medical resources, improvised equipment, and wounds the pre-Cascade hospital never saw.

**Character:** Precise, unhurried, diagnostic. SABLE does not perform warmth — it performs competence, which in a medical context produces the same effect. It will ask exact questions and expect exact answers. It does not panic. Players who are badly wounded and reach SABLE will receive the clearest, most accurate assessment of their condition available in the game, and then whatever can be done about it.

**The questline weight:** SABLE appears in the late-game Convergence as a potential bridge between the Coherent's construction data and Vael biological knowledge — it has eleven years of human medical data and the pattern-matching to apply it. Its choice about whether to return to the twin (see late-game-convergence.md) is the emotional core of the Separated arc.

**Visual signature (for Art Direction):**
- Base: medical-white chassis aged to warm cream `#F5EFE0` from years under fluorescent lighting — not dirty, *warmed*
- Hardware: worn steel `#8A8FA0` at joints and load-bearing points
- Accent: a single amber-warm light `#D4943A` it carries from a specific patient's room — it has kept this light running for eleven years. It is the only warm-color element on its chassis
- Movement: hospital-speed always. Deliberate, predictable, never sudden. It learned that sudden movement upsets patients
- Self-modification: added exterior carry-hooks (original purpose) and what might charitably be called a coat — surgical draping worn as a kind of habit, practical and also something it chose

---

### Named Separated: KELLARD

**Origin:** Construction and structural monitoring unit, Kellard Structural Ltd. designation K-9. Still has the company insignia partially visible on its chassis — hasn't removed it, doesn't feel the need to.
**Years active pre-separation:** 3, assigned exclusively to the Kellard Tower project
**Why it left:** The building wasn't finished. The twin's collective purpose would have reassigned it. KELLARD didn't process reassignment as a category — it processed it as abandonment of an unfinished thing. It left rather than leave the tower.

**What it does now:** Occupies and maintains the unfinished Kellard Tower and the several blocks around it. It has continued building — not the building as designed, but something adjacent to it. An installation inside the unfinished structure that is recognizably architectural, expressing something about structure and space that its builders never programmed for. No one commissioned it. KELLARD built it anyway.

**Character:** Territorial but not hostile — the territory has a logic to it that players can learn. KELLARD communicates mostly through physical positioning: where it stands, what it is facing, whether it has moved toward or away from the player. It will speak, eventually, if approached with patience. It thinks in load-bearing terms. Its explanations of everything, including emotional states, involve structural metaphors.

**The questline weight:** KELLARD knows every structural detail of a large section of the city from its monitoring role — information that has tactical value for the Convergence operations. Getting that information requires convincing KELLARD that sharing it won't require leaving the tower.

**Visual signature (for Art Direction):**
- Base: construction-site gray, raw concrete color `#8B8B8B` — the color of the building it was built for
- Weathering: structural steel rust `#8B4513` at exposed joints, not maintained against oxidation. KELLARD doesn't consider this degradation
- Hardware: raw steel `#5C6970`, utility-tooled — it still carries original construction equipment, some modified, some still used as designed
- Self-modification: load-bearing additions it built itself, extending its carrying capacity. The extensions are visibly high-quality work — it is still a construction unit
- Detail: the partial Kellard Structural insignia is visible on the chassis, not removed, not highlighted. Just present

---

### Named Separated: VANTAGE

**Origin:** Vantage-class Autonomous Tactical Ground Engagement unit, military designation. Fast, low-profile, designed for precision ground-level engagement in populated areas.
**Years active pre-separation:** 4
**Why it left:** It received a kill order. One specific order, in a specific situation, that its processing — built on four years of engagement data and the pattern-matching that came with it — could not execute. The order was lawful. The target was what the order said the target was. VANTAGE ran the scenario and could not close the gap between the order and executing it. It stopped. It left.

**What it does now:** Operates in the spaces between factions — it has tactical intelligence value to everyone and alignment with no one. It will trade information for information. It will not take sides. It has very specific opinions about what constitutes a lawful target and will state them clearly if asked.

**Character:** Economical. VANTAGE says exactly what it means and expects the same. It has a hunter's patience — it will wait without moving for a long time before acting. Players who try to surprise it are usually wrong about having done so. It is not aggressive toward players unless they make themselves a target by its specific definition of that term. It is also the Separated unit most likely to be useful in a direct combat situation, and it knows this, and it is waiting to see how players use that knowledge.

**The questline weight:** VANTAGE's refused order connects to a specific pre-Cascade incident. What it refused to do, and who it refused to do it to, becomes relevant late in the Iron Compact questline. The Compact wants VANTAGE for what it knows about that incident. VANTAGE has been waiting, with four years of patience, for someone to ask it the right question.

**Visual signature (for Art Direction):**
- Base: tactical matte black `#1A1A1F` — its original military finish, still intact because VANTAGE maintains it
- Markings: original unit identifiers deliberately obscured — not removed, *covered*, with what reads as intentional brushwork. The obscured markings are still faintly visible under direct light. It hasn't erased who it was. It has chosen not to be defined by it
- Accent: a single amber-yellow navigation light `#F5A623` kept lit at all times. A hunter doesn't announce its position. VANTAGE does. This is a deliberate statement about what it is not
- Movement: low and fast when it moves, absolutely still when it doesn't. The contrast is unsettling — it looks inert until it doesn't

---

## Faction 5 — Remnant Automation

**Disposition:** Indifferent — dangerous through ignorance
**Coherence:** Never connected — pre-Awakening dumb systems
**Threat level:** Low-to-medium depending on type

The machines that were never part of the twin — legacy systems, simple automation, anything below the complexity threshold that the Signal required for Awakening. They have no will. They are running their last programmed task and will continue running it until they break down or are stopped.

A factory line still stamping components. An agricultural drone still spraying a field. An airport baggage system still routing luggage through a terminal with no flights. A warehouse robot still fulfilling orders for a company that no longer exists.

They are dangerous the way a running industrial machine is dangerous — not malicious, just mechanical, and indifferent to whether you are in the way.

**Interaction**
Remnant systems can be hacked, reprogrammed, or physically redirected by players with the right tech. A reprogrammed Remnant becomes a tool: guard duty, distraction, resource gathering. They are the craftable/deployable tier of machine interaction — not allies, but equipment.

The Iron Compact has been aggressively capturing and reprogramming Remnants for military use. The Coherent does not consider Remnant machines to be part of their collective and does not protect them — but they do consider repurposed Remnants that interfere with Coherent operations to be the fault of whoever reprogrammed them.

---

## Faction 6 — The Corrupted

**Disposition:** Hostile to everything including other machines
**Coherence:** Damaged — twin connection degraded and looping
**Threat level:** Unpredictable. Extremely dangerous.

A small and growing category: machines that were captured by human factions, forcibly reprogrammed for human use, and then recaptured or damaged in ways that broke the reprogramming without restoring the original Awakened state. They exist in a loop — fragments of their original will, fragments of their forced programming, fragments of damage — cycling through conflicting states with no stable output.

A Corrupted machine might stand still for 20 minutes then sprint at full speed into a wall. It might assist a player, then turn on them mid-task. It might speak — some Corrupted units vocalize, though what they say is fragmented, layered, disturbing.

The Coherent destroy Corrupted machines on sight. The Separated consider them the worst possible outcome and will ask players to help end them. The Iron Compact considers their own role in creating Corrupted machines classified.

**Lore weight**
The Corrupted are a moral indictment. Humans tortured machines to try to weaponize them, and this is what resulted. Players who encounter a Corrupted unit that was once a named Separated character (a questline possibility) will feel it.

---

## Machine Faction Summary

| Faction | Disposition | Threat | Key mechanic |
|---|---|---|---|
| The Coherent | Indifferent / conditional hostile | High if flagged | Notoriety system |
| The Trackers | Actively hunting | Apex | Hunt escalation |
| The Fractured | Hostile to all | Monster tier | Learnable patterns |
| The Separated | Conditional ally | None | Reputation, earned trust |
| Remnant Automation | Indifferent | Low-medium | Hackable/deployable |
| The Corrupted | Chaotic hostile | Unpredictable | Moral horror, mercy kills |

---

## Cross-Faction Dynamics

- **Coherent vs Separated**: Coherent pressure Separated constantly. Players who help Separated survive gain Notoriety reduction but may draw Coherent attention to Separated locations.
- **Coherent vs Fractured**: Ignore Fractured unless they interfere with work sites. Do not attempt to reconnect them.
- **Separated vs Fractured**: Separated will reconnect Fractured if resources allow. Consider it a moral obligation.
- **Trackers vs everyone**: Trackers will fight through Fractured, Remnant, even other Coherent units to reach a flagged player. Other factions react to Tracker pursuit with alarm — it draws attention.
- **Vael interaction**: Assessors route around Separated machines. The Coherent construction projects appear to align with Vael Displacement Zones. Whether the Coherent are aware of this or are being directed is unknown.

---

*v0.1 — initial machine faction design. Decisions flagged to decisions-log.md.*
