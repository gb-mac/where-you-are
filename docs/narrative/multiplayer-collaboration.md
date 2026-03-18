# Multiplayer Collaboration — Design v0.1

The world is built around real geography. So is the social layer. You don't just group up — you find each other, the same way you'd find someone in a real collapsed world: you move toward them, or you plan to be in the same place at the same time.

---

## Starting Conditions

### Solo Start
You spawn at your real-world GPS home location. Your neighborhood, your streets, your local threats. The world you know best is the world you start in.

### Grouped Start — Same-Time Login
If two or more players log in within a short window of each other and choose to group before spawning, they get to pick a shared starting point. Options the game offers:

- **Any player's home GPS** — one player hosts, others spawn at their location
- **A known Neutral Ground** — a Warden estate between your real-world positions (the game calculates the midpoint and finds the nearest Warden-controlled location)
- **A designated Holdout community** either player has standing with
- **Midpoint spawn** — the GPS coordinate equidistant between all grouping players, adjusted to the nearest safe zone

The shared start is a privilege of coordination. If you planned to log in together, you get to begin together. If you didn't — if one of you is already in the world — the other starts at home and has to travel.

### Grouped Start — Different-Time Login
One player is already in the world. The second player logs in later. They spawn at their own home GPS. The gap between them is the first obstacle.

This is intentional. The journey to meet is content. Both players are fighting through their local area, building resources, managing survival tracks, and moving toward a rendezvous. When they finally reach each other, they've both brought something — local knowledge, local resources, local faction relationships. The meeting is a merging of two separate contexts.

---

## Finding Each Other

### The AR Layer — Friend Markers
Friend positions are visible on the AR mobile app as distinct markers — not real-time to the meter, updated on a short interval. You can see roughly where your friend is in the real world, how they're moving, and what direction that is from you.

This creates a natural coordination loop: check the AR app, see your friend's marker, head toward it in-game. When your in-game positions converge, you're grouped. The AR layer shows it happening in real-world space before the game registers it — you can watch each other's dots closing the gap.

### The Rendezvous System
Players can set a **Rendezvous Point** — a named GPS coordinate or a known location (a Warden estate, a Holdout community, a landmark the game recognizes) — that other grouped or friend players can see as a waypoint.

Rendezvous points can be set from the AR mobile app before either player logs into the desktop game. Friends coordinate in the AR layer — *"meet me at the central Holdout, I'll be there in an hour"* — and both navigate to it from their respective starts.

### Same City, Different Neighborhood
Friends in the same city still don't spawn together. Your home is your home — the 3km between you is real in-game distance, real threats, real travel. The first collaborative act for same-city friends is the convergence itself.

Same-city convergence is faster and lower-risk than cross-city travel. It's still not trivial. A friend across town might be in Hollow-controlled territory. The two-block gap between your home turf and theirs might be Fractured territory. The first time you link up with a same-city friend, you've already learned something about each other's neighborhood.

### Cross-City Friends
Friends in different cities are separated by genuine distance. Three options:

**1. Travel** — one or both players moves toward the other through the world. Cross-city travel is a major undertaking: Hollow-controlled highways, Compact checkpoints, Fractured migration paths. It's also the richest content in the game — the road between two cities is a story.

**2. The Snatch** — let the Vael close the gap. If one player wants to reach a friend in another city, they can deliberately submit to a Snatch Event and accept the drop location. It won't be exact — the Vael drop you near the nearest urban center to the Ship, not at your friend's address — but it can dramatically reduce travel distance. The cost is inventory loss and the Vael Trace. The benefit is a jump across geography that would otherwise take hours.

**3. The Waiting Game** — one player travels to the other's city and both grow their local presence independently until they converge. This is the long-term collaboration model for distant friends: you each know your own city, you eventually meet in one of them, and you bring everything you've built.

---

## The Convoy — Group Travel

Once players are grouped, moving through the world together is a distinct mode with its own mechanics.

**Why convoys matter:**
The world is dangerous solo. It's dangerous in a group too, but differently. A convoy of three players moving through Hollow-held territory is a harder target than one player — and a more visible one. The Hollow notice convoys. The Compact notices convoys. So does the Coherent.

**Convoy roles:**
In any group, players naturally fill different functions. The game doesn't force role assignment but designs around emergent specialization:

| Role | What they do | Why it matters |
|---|---|---|
| **Scout** | Moves ahead, AR layer active, reads machine signatures and faction activity | The group doesn't walk into ambushes |
| **Medic** | Carries surplus medical supplies, monitors group wound states | Survival tracks managed for everyone |
| **Heavy** | Primary combat, carries the gear that breaks Fractured systems | Boss fights and hostile encounters |
| **Quartermaster** | Manages group inventory, handles AR layer drops and pickups, tracks supplies | Convoy doesn't run out |
| **Diplomat** | Highest faction standing, handles checkpoint negotiations and Holdout contact | The group gets through without fighting everything |

No class system. No locked roles. Players with high Signal standing naturally fill the Scout/intelligence role. Players who've invested in Separated relationships naturally fill the Medic role. The roles emerge from the choices players made before the convoy formed.

**Convoy survival tracks:**
The group shares a visible survival state — each player's hunger, hydration, and wound status is visible to the group. A good Quartermaster notices that the Scout is dehydrated before the Scout does. The group can redistribute supplies. The medic can treat a wound before it compounds.

**Convoy stealth vs. speed:**
A convoy moves slower than a solo player. Four people moving through Fractured territory generate more noise signature than one. The group has to decide: move carefully and slowly, or move fast and accept that they'll be noticed. Fractured territorial responses to convoys scale with group size — a Behemoth that ignores a solo player might react to a group of four crossing its boundary.

**Convoy camping:**
Groups can establish a **Camp** — a temporary safe location that functions like a field Brace. The Camp requires resources to set up (materials, time, a defensible position) and provides a shared respawn anchor, a supply depot, and a rest point that advances hunger and hydration recovery. A well-set Camp in a good position is worth hours of travel time.

---

## Collaboration in Boss Fights

The Brace becomes a shared resource. Roles become explicit. The 1–2 hour fight has room for different players doing different things simultaneously.

**Natural fight roles in a group:**

*The Anchor* — the player who draws and holds the boss's primary attention. Not a tank in the traditional MMO sense — the boss doesn't target-lock — but the player who positions aggressively and keeps the boss's threat behavior focused. The Anchor takes the most damage and the most wound accumulation. They need the Medic close.

*The Breaker* — focused on Ring 1 arena systems. While the Anchor keeps the boss engaged, the Breaker is targeting incorporated components, destroying systems, triggering phase transitions. A good Breaker shortens the fight significantly by advancing phases faster.

*The Medic* — manages wound states for the group during the fight. Positioned to reach any player quickly, carries surplus medical supplies, calls out when a player's wound accumulation is reaching a dangerous threshold. In extended fights, the Medic is the reason the Anchor can keep going into the final phase.

*The Quartermaster* — manages The Brace. Restocks it from staged supplies as it depletes. Defends it from any minions or environmental pressure. Coordinates respawning players' return to the fight. The Quartermaster is rarely in the boss fight directly — they're in the logistics of it.

*The Scout* — in fights with environmental components, the Scout reads the arena. Where are the supply caches? What's the safest approach to the next system? What's the Assessor's current objective and how do we interrupt it? The Scout communicates this so the group isn't fighting blind.

**Asymmetric contribution:**
Not every player has to be fighting to be contributing. A player on the AR mobile layer during a fight can see the broader zone — incoming threats, faction activity near The Brace, whether the fight is attracting outside attention. Calling this in to the group is a genuine contribution. A player who can't commit to the desktop game during the session can provide meaningful AR-layer support.

---

## Long-Distance Collaboration — Friends in Different Cities, Countries, Continents

Friends separated by distance — whether across a city, across a country, or across an ocean — are in the same world but different regions of it. The game handles each scale differently.

**The parallel world model:**
Each player's world is centered on their GPS location. The game world is the real world, overlaid — one continuous world, not sharded instances. What happens in Tokyo affects the global faction picture. What happens in Lagos does too. Friends in different countries are not in different games. They're in different parts of the same one.

**Faction ripples — global world state:**
Every region contributes to a global faction balance. A player who tips the Compact/Warden relationship in their city is contributing to a picture that players in other countries can see shifting. The Vael's Assessment phase is global — Ships are positioned across the world, Displacement Zones appear in every region, Assessor events happen everywhere. Players in different countries are experiencing the same alien presence, each through their own local lens.

Major world events — a Behemoth Walk that reaches a city center, a regional Warden House going excommunicado, a Signal broadcast about Dr. Osei's research — propagate globally through the Signal network. International players receive these as intelligence, not as immediate local events. The world is connected. It just takes time for news to travel, same as it would in the actual post-Cascade world.

**The regional flavor:**
Each country and region has its own version of the Cascade. The Fractured in rural Japan incorporate different Remnant Automation than the Fractured in rural Brazil. Warden Houses in Scotland have different histories than Houses in South Africa. The Iron Compact operates differently in regions where pre-Cascade military and law enforcement had different structures. Holdout communities reflect the culture of the place they formed in.

International players aren't playing the same game with different skins. They're playing a world that remembers where it is.

**Playing together across distance:**

*Option 1 — The Shared Session:*
Players in different countries can group in a shared session — the game creates a regional blend zone centered between their GPS coordinates. Both players experience a version of the world that draws from both their home regions. The Fractured types are a mix. The faction balance reflects both areas. The terrain is a hybrid. This is the most accessible international option — same session, same fight, no physical travel required.

The shared session is explicitly flagged as a **Coalition** — both players are operating outside their home territory, with reduced home-turf advantages but the combined skills and faction standings they've each built.

*Option 2 — Visiting:*
One player "visits" the other's region — spawning in their friend's home zone instead of their own. They lose their home-turf knowledge and local faction relationships. They gain their friend as a guide through an entirely different version of the post-Cascade world. A player visiting a friend in another country is a tourist in the apocalypse, in the best possible way.

The visiting player's AR layer shows their friend's city as an unfamiliar map. The Fractured types are different. The Holdout communities have different leadership, different needs, different standing requirements. The Vael Ships are positioned differently. Everything familiar is shifted.

*Option 3 — The Displacement Bridge:*
If a Vael Ship is positioned between two players' regions, a deliberate Snatch can move a player toward their international friend faster than any travel mechanic. The Vael don't respect borders. A Ship over the Atlantic drops you at the nearest urban center — which might be a city your international friend knows well. They can be there to meet you when you arrive.

This is the most chaotic option and produces the best stories.

**The AR item bridge — international dead drops:**
Players can view AR caches anywhere in the world on the mobile app — only collection requires physical presence. Friends who travel internationally in the real world bring items between cities, literally. A player visiting a friend's country in real life picks up the cache their friend left for them. The game world and the real world overlap in exactly this way.

International players who are part of the same guild or faction may coordinate AR drops across borders — leaving high-value items in cities where allies are based, knowing those allies will collect them. This is the Signal's international information network made physical: a dead drop in a specific city that a specific contact picks up.

**Language and faction:**
The Wardens, Holdouts, Iron Compact, Covenant, Hollow, and Signal all exist in every country — but they speak the player's language, reflect local culture, and have local history. The Warden code is universal but the Houses are regional. The Iron Compact's structure reflects the pre-Cascade military of each country. The Hollow look different in different cities.

International players meeting in a Coalition session encounter each other's faction contacts speaking different languages — the game handles translation as an in-world artifact of the Signal's communication network. The Signal has always been multilingual. It's how they operate across borders.

**The displacement meet — international version:**
A player Snatched and dropped in an unfamiliar city abroad is the most dramatic version of the displacement arc. Working your way home across international distance — potentially across language barriers, different faction structures, unfamiliar Fractured types — is a full campaign in itself.

An international friend who happens to be based in that city is the best possible outcome of a bad Snatch. Suddenly the nightmare is an adventure and the friend you've only ever played Coalition sessions with is showing you their actual home turf.

---

## Group Dynamics — The Tension the Story Creates

The same faction tensions that affect solo players affect groups — but with more texture because group members may have different faction standings and histories.

**Mismatched faction standing:**
One player has high Compact standing. Another is excommunicado from the Wardens. The third has deep Covenant connections. Moving through a Compact checkpoint together: the first player gets waved through, the second is questioned, the third is watched with suspicion. The group negotiates this in real time.

**The mixed-mode problem:**
Players can be on different difficulty modes in the same group — but the group experience adjusts to the highest mode present. A Survivor-mode player grouping with a Contractor-mode player gets Contractor rules (the world doesn't punish the Contractor player for grouping with someone harder). The Survivor player may choose to apply their own mode's standards to their own behavior — managing their drops, staging their own Brace resources — even when the game's mechanics don't force it.

**Group decisions with world consequences:**
Major decisions that affect the world state can be made by groups just as by solo players — and the Warden Record attributes them to all participants. A group that saves a Holdout community gets a joint Record entry. A group that makes a decision the Wardens consider a violation gets a joint accounting. The Record doesn't distinguish between the player who made the call and the players who were present.

This creates genuine group accountability. Being in the convoy when the decision was made means your name is on what follows. Players who care about their Record think about who they're traveling with.

---

## The First Meeting — A Moment Worth Marking

When two players who have been converging finally reach each other in-game, it's worth a beat.

The game doesn't force ceremony. But if both players are Sworn or above with the Wardens, and an Arbiter is within the region, the meeting can be formally witnessed — a Record entry that marks where two players first joined forces. Date, location, faction states, current world conditions.

Some players will never notice or care. Some will specifically arrange their first meeting to happen at a meaningful location — at a Warden estate, at the site of a significant battle, at a Holdout community they both helped build.

The AR mobile app marks the meeting location for both players as a personal marker — invisible to others, just for them. *This is where we found each other.*

The world doesn't require it to matter. Most players will just start playing together and that's fine. But the infrastructure for it mattering is there, for the players who want it.

---

*v0.1 — multiplayer collaboration design. Decisions flagged to decisions-log.md.*
