# Warden Spaces — Physical Presence in the World
*The Table is not a concept. It is a building. Multiple buildings. GPS-anchored to the real world.*

---

## The Problem This Solves

Every faction has a place. The Reckoners hold their broadcast territory — you hear Derek before you see anyone. The Covenant clusters near Coherent construction sites. The Compact has checkpoints and installations. The Hollow own the industrial zones.

The Wardens have always had the most complete governance structure in the game and the least defined physical presence. The Table — the deliberative body above and alongside the other tiers — needs spaces commensurate with what it is. A faction whose core mechanic is stewardship of land and community cannot be spatially abstract.

This document defines where the Wardens actually are.

---

## The Space Hierarchy

Six distinct space types, each with a different function and a different spatial register:

| Space | Who it belongs to | What happens there | GPS anchor |
|---|---|---|---|
| **The Moot Hall** | The Table — all Houses together | Formal deliberation, oath-taking, Record readings, Arbiter rulings | Heritage buildings, civic halls, estate great rooms |
| **The House Seat** | Individual Old Houses | Day-to-day operations, Neutral Ground, guest quarters, the House's Makers | Farmsteads, working estates, rural properties |
| **The Maker Spaces** | Individual Makers (affiliated to a House) | Craft, repair, provenance assessment, apprenticeship | Workshops, outbuildings, adapted industrial spaces |
| **Sworn Chapters** | Sworn Wardens (non-House) | Oath administration, Vanguard staging, field operations base | Community centers, repurposed civic buildings |
| **Warden Corridors** | The route network | Safe passage between spaces — not a base, claimed territory | Heritage trails, rural roads, specific GPS-tracked paths |
| **Wayhouses** | Warden-administered, Keeper-run | Operational neutral ground — services, rest, trade, faction business | Inns, roadhouses, hotels, large heritage properties on routes |

---

## The Moot Hall

The most significant Warden space in any region. There is typically one per major metropolitan area, one per rural district.

The Moot does not convene in purpose-built spaces. It convenes in the oldest appropriate building available — whatever has been standing longest, held community function longest, and has the kind of weight that comes from being used for important things by people who took importance seriously. Pre-Cascade: estate great rooms, civic halls, heritage church interiors, old courthouse chambers. The building's prior use is part of its authority.

**What it looks like inside:**

Functional. Not decorated for faction identity — the Wardens consider their presence in a space to be established by the Record, not by banners or marks. There will be a long table. There will be chairs that don't match. The Record boxes are on shelves along one wall — physical volumes, indexed by Arbiter and by date. The newest volumes are the heaviest.

A fire if the building has a fireplace. Light if there is power. The Moot has met in conditions ranging from a restored estate hall with full utilities to a farmhouse kitchen with candles and four people. The formality is in the conduct, not the setting.

**What happens there:**

- **The Moot** — formal deliberation between House representatives. Held quarterly under normal conditions, more frequently during crisis. All Houses with standing in the region have a seat. The Moot makes decisions that affect all Houses — territorial agreements, formal disputes, code interpretations.
- **Oath-taking** — Sworn oaths are preferably taken at the nearest Moot Hall with an Arbiter present. This is the highest-weight ceremony the Wardens have. It is not elaborate. The weight is in the words and the witness.
- **Arbiter rulings** — formal code decisions are delivered here. The ruling is read aloud, the Record entry is made, and the parties are present. There is no appeal.
- **Guest negotiation** — other factions who want formal engagement with the Wardens as an institution come here. The Compact commander who wants a non-aggression arrangement with a House. The Signal cell that wants their documentation cross-referenced with the Record. This is where those conversations happen.

**Who can enter:**

The Moot Hall is Neutral Ground by definition — the highest-weight Neutral Ground in the faction's network. Any living person can enter. No violence, no faction action, no enforcement. The conditions that apply to Warden estates apply here at maximum intensity.

A player who enters the Moot Hall for the first time receives a specific environmental read — the AR overlay shows Historical Markers for every significant event that has been formally recorded here. Depending on how long the building has been Warden-held, this can be a dense overlay. The player is standing inside the region's institutional memory.

**GPS anchor:**

Moot Halls appear at real-world locations tagged `historic=*`, `building=civic`, old `amenity=place_of_worship` converted to community use, or large farmstead estate buildings with `historic=farmyard`. The system picks the highest-standing heritage building in the region's Warden territory. In cities: a heritage hall or converted civic building. In rural areas: the oldest farmstead with the right footprint.

---

## The House Seat

Each Old House occupies a specific property — their seat. This is the geographic anchor of their stewardship claim. The House Seat is the reason the House exists: they hold this land, these relationships, this history, in trust.

**Physical character:**

Working. Not a museum. The House Seat is a functioning estate — food production, workshop space, community resource. The Warden code requires the House to actively use what they hold, not preserve it. An estate that produces nothing is not being stewarded.

The buildings reflect whatever the property was pre-Cascade adapted to post-Cascade function. An old acreage has a barn that now houses Vanguard equipment and recovered machinery. A suburban estate has a garage workshop that the Armorer has converted into a proper forge setup. A rural farmstead has expanded its growing area and built community storage.

The gate is recognizable. Not marked with faction insignia — the Wardens don't use that kind of exterior branding. But the condition of the approach, the state of the perimeter, the quality of the infrastructure tell you what you're approaching. Warden property looks maintained. Deliberately, precisely maintained. In the post-Cascade world that is itself a statement.

**What it contains:**

- **The main house** — the House head's residence, guest quarters for visiting Sworn and allies, the room where the House holds its own internal meetings
- **The Maker spaces** — each Maker has their workspace on or immediately adjacent to the Seat (see below)
- **The archive room** — the House's Record. Physical volumes, organized and indexed. The Arbiter's Record is here. Every significant event involving the House is in this room.
- **The community resource** — part of every House Seat is by code open to the community the House stewards. A medical space. A supply depot. A skill-sharing area. What it contains depends on what the House does well.
- **Vanguard staging** — a space for Vanguard who operate out of this House. Equipment storage, briefing space, rest.

**The guest room:**

Every House Seat has guest quarters. Not elaborate — a bed, light, warmth. Neutral Ground. A player who has been formally received by a House can use the guest room. Being in a House guest room is one of the few places in the game where the threat model goes to zero and the player can just exist.

The detail that matters: the guest room in a House that has been through something will have things in it that reflect what the House has been through. A spare piece of Armorer work on the wall. A photo that's been laminated against decay. A specific book left by the last guest. The House does not announce these things. They're there.

**GPS anchor:**

House Seats appear at real-world `landuse=farmland`, `landuse=residential` large-lot properties, former estates. Rural areas have more of them — House Seats in urban areas are the houses on the largest lots with the longest ownership history (reflected in OSM building data where available). The algorithm prioritizes properties with `historic=*` tags and large footprints with outbuilding structures.

---

## The Maker Spaces

Each of the four Makers has a workspace. At Old Houses, this is on or adjacent to the House Seat. Sworn Makers — Makers who came to the Wardens after the Cascade and took the oath without a House name — may operate from a Sworn Chapter instead, or from their own acquired space.

**The Armorer's Workshop:**

The largest of the four spaces. Requires power tools — or has adapted to work without them, using hand tools and modified techniques where grid power is unavailable. Raw material storage: salvaged plate, ceramic panels, hardware. The organized chaos of someone who knows exactly where everything is and has a reason for its position.

The walls have records. Gear condition photographs, threat assessment notes, modification logs. The Armorer tracks what their work has been through. If a piece comes back damaged, they analyze the failure mode before they repair it.

**The Tailor's Space:**

Quieter. Better light than any other space in the House — the Tailor has arranged whatever lighting is available around the work surface. Thread sorted by composition and weight, not by color. The measuring tools are old, well-maintained, precise. There is always a piece of work in progress.

The Tailor's space is where people come when they need a fitting, but also sometimes just when they need somewhere quiet. The Tailor does not turn people away who are not there for gear. They make tea. They listen. This is understood.

**The Gunsmith's Bench:**

The most organized space in the House. Every tool has a position. The provenance records are filed in a specific system that only the Gunsmith fully understands but that is internally consistent — you could learn to navigate it with time. The workbench surface is clean when not in use.

Firearms in various states of service line the wall. The Gunsmith can tell you the condition and history of each one without looking at the file. The file is there to be shared. The Gunsmith's knowledge is not in the file.

**The Bladesmith's Smithy:**

The hottest space. The oldest-feeling space. Something about a smithy communicates temporal depth in a way the other workshops don't — the forge, the anvil, the specific smell of hot steel, the way heat moves in the room. Pre-industrial in its fundamentals even when the Bladesmith has access to modern tools.

The Bladesmith works slowly and does not apologize for it. There is often a piece on the bench that has been there for weeks. It is not abandoned — it is waiting for a specific decision about what it should be. The Bladesmith does not rush that.

---

## Sworn Chapters

The network of non-House Warden spaces. Where Sworn Wardens — those who took the oath without a House name — base their operations.

**Character:**

More modest than a House Seat. A repurposed community center, a converted school, a large Holdout community building where the Sworn have a formal presence. The Sworn Chapter does not have the depth of history a House Seat carries, but it has the code applied to whatever it is. The same condition. The same maintenance. The same guest room.

Sworn Chapters are also where new oath-takers are most likely to take the oath. The Moot Hall carries more formal weight, but a Sworn Chapter with an Arbiter present is sufficient. The simplicity of the Chapter space suits many people better than the gravity of the Hall.

**What's distinctive:**

Chapters have a higher operational tempo than House Seats. More Vanguard moving through. More field activity staged from here. The Houses hold the Table. The Chapters hold the ground.

A player who has taken the oath but doesn't have House patronage will use a Chapter as their primary Warden base. The Chapter Warden — the senior Sworn who manages the space — is a significant NPC relationship in the player's Warden progression.

**GPS anchor:**

Sworn Chapters appear at `amenity=community_centre`, converted `amenity=place_of_worship`, and large `building=residential` properties that Sworn Wardens have formally claimed and marked. Denser in urban and suburban areas than House Seats, which tend rural.

---

## Warden Corridors

Not a building. A route.

Warden Corridors are GPS-tracked paths between Warden spaces — formally claimed, maintained, and held to Neutral Ground conditions. You cannot be attacked on a Warden Corridor if the Corridor is active. A Corridor that has gone inactive — because the Wardens holding it have been pushed back or a House has fallen — is marked in the AR overlay as *Lapsed*.

**What they look like:**

The routes follow existing infrastructure where possible — roads, trails, paths. What distinguishes a Warden Corridor in the AR overlay is the marking: a specific line notation in the faction color that indicates active Neutral Ground. GPS breadcrumbs at key points: junctions, chokepoints, sight lines.

In the physical world, Corridors may be marked at key points with Warden notation — not faction branding, but small physical indicators that someone with Warden standing will recognize and someone without it will not notice. A specific knot in a fence wire. A stone arrangement at a path junction. The notation system is taught to Sworn, not published.

**Why they matter:**

Safe travel between Warden spaces. Faction tensions make direct travel across open territory variable — a player moving between two Warden locations through Compact territory might be stopped, questioned, flagged. Through a Warden Corridor they are on Neutral Ground the whole way.

The Corridor network is the Wardens' most visible infrastructure contribution to the post-Cascade world. Communities near Corridor routes use them for supply movement, messenger travel, evacuation. Non-Warden-affiliated people using a Corridor are not turned away — the Corridor's function is safety, not exclusion.

**Corridor condition states** in the AR overlay:
- **Active** — Neutral Ground, full protection
- **Contested** — a faction is pressuring the Corridor, conditions uncertain
- **Lapsed** — the Warden presence that held it has withdrawn or been pushed back
- **Claimed** — new Corridor, formally claimed but not yet fully established

---

## Wayhouses

The operational neutral ground network. Where factions meet at the working level.

The Moot Hall is for the Table. The House Seat is the faction home. Neither is where a Compact commander and a Hollow Syndicate boss sit across from each other to negotiate a corridor arrangement, where a player gets their gear repaired and their wounds treated without a faction taking note of it, where information moves between people who need to move it but can't be seen doing so.

That's the Wayhouse.

Burke's property is the archetype — a fire, a table, every faction has sat at it, no violence, full conversation. The Wayhouse network formalizes that principle across a distributed chain of establishments administered under Warden Neutral Ground rules but operating as service locations, not governance structures.

---

**What a Wayhouse is:**

A specific building — an inn, a roadhouse, a hotel that survived the Cascade or was repurposed after it — administered by a **Keeper** who holds a formal Warden commission. The Keeper is not necessarily Warden-affiliated by background. They are Warden-commissioned: they have taken a specific oath to hold the Wayhouse to the rules, and the Wardens have staked their Neutral Ground guarantee on the Keeper's word.

The Keeper is the Winston of this network. They enforce the rules. They do not take sides. They have the authority — backed by the full Warden institutional weight — to remove anyone who violates the conditions. A Keeper who enforces selectively loses the commission. A Keeper who looks the other way once loses it faster.

What makes the Wayhouse work is what makes the Continental work: the rules are absolute and the consequence for breaking them is worse than whatever you'd gain. The Compact cannot formally condone an officer who drew on someone inside a Wayhouse without losing the Warden relationship that keeps their supply corridors intact. The Hollow cannot protect a Crew member who took from a Wayhouse without paying across their entire operation. The math always runs the same direction.

---

**What a Wayhouse provides:**

| Service | Notes |
|---|---|
| **Rooms** | Clean, secure, no surveillance. What happens in the room is not the Wayhouse's business. Who came and went is logged by the Keeper — the Record requires it — but not distributed unless an Arbiter formally requests it. |
| **Food and fire** | The baseline. Whatever the region produces, the Wayhouse has it. The quality varies by location. The availability does not. |
| **Medical** | A working medical space. Not a hospital — a competent field-medicine setup with supplies maintained by the Keeper. Players can recover here. NPCs can recover here. The Wayhouse does not ask how you got hurt. |
| **Gear maintenance** | A workshop. Not a Maker's space — the Maker's craft requires relationship and provenance. The Wayhouse workshop is competent field-level repair and cleaning. Enough to keep gear functional. |
| **The common room** | The operational heart. Tables, fire, food, people from every faction sitting in the same room under the same rules. This is where the interesting conversations happen — and the dangerous ones, spoken quietly enough that the Keeper isn't obligated to hear them. |
| **Message drop** | The Keeper holds messages. A player or NPC can leave a message at any Wayhouse addressed to anyone who has standing to receive it. Messages are held, not read. The Keeper does not know the content. They know the sender and the intended recipient and they keep that in the log. |

---

**Wayhouse coin:**

Faction currency doesn't work at Wayhouses. The Compact's scrip, the Hollow's barter credit, the Signal's digital tokens — none of it. The Wayhouse operates on **Warden coin**: small stamped tokens issued at any Wayhouse against labor, goods, or Markers. They carry no value outside the Wayhouse network and full value within it.

Warden coin is faction-neutral by design. A Compact officer and a former Hollow Crew member have the same purchasing power at a Wayhouse table if they have the same number of coins. The coin is the social equalizer the space requires.

Earning coin: completing work for the Keeper or the Wayhouse community, converting Markers at a discount, or being paid in coin by someone who already has it. Coin moves. The Wayhouse network is the most liquid economy in the post-Cascade world that isn't controlled by a faction.

---

**The Keeper:**

Not a warrior. Not primarily. The Keeper's tools are the commission, the rules, and the network's reputation — which is worth more than any weapon in the Wayhouse context.

Every Keeper has a specific skill set that made the Wardens commission them. Some are former hospitality industry — they know how to run a building, manage inventory, read a room. Some are former intelligence — they know how to hold information correctly. Some are former medics. The common thread is competence and the specific kind of trustworthiness that produces: they do what they say they'll do, they don't do what they say they won't, and they don't need anyone watching them to maintain the standard.

The Keeper is one of the most interesting recurring NPC types in the game. They know everyone who has passed through. They've heard conversations they aren't supposed to repeat. They have opinions about every faction that they express obliquely and precisely. They will help a player who has demonstrated they understand what the Wayhouse is and will not help a player who treats it as a convenience rather than a institution.

*"You can sleep here. You can eat. You can have your gear looked at. What you can't do is use this room to conduct business that puts someone at risk on my premises. I don't care what the business is. I care what it requires of my building."*

---

**Burke is the Keeper:**

Not a Keeper at one location. The Keeper — the head of the network, the one who built it, the one who commissioned every individual location keeper and holds them to the standard.

His property is not the archetype of the Wayhouse. It is the original. He was running it as neutral ground before the Wardens formalized the concept, before the network existed, before anyone had a name for what he was doing. Every faction has sat at his fire. That is not a coincidence or a side effect of his neutrality — it is the operational result of a deliberate institution he built and maintains.

The individual Wayhouse locations have their own keepers — people Burke commissioned, trained, and holds accountable. They run the buildings. He holds the standard. If a keeper fails — looks the other way, enforces selectively, takes a side — Burke pulls the commission. The Warden Neutral Ground guarantee goes with it. That combination is the enforcement mechanism, and it has never needed to be tested twice in the same location.

The Wardens co-sign the Neutral Ground guarantee because Burke's network and the Warden institution serve each other's purposes. The relationship is mutual recognition between two institutions that respect each other's function. Burke does not have a Warden commission and has not taken the oath. He doesn't need to. His institution predates the formalization and would function without the co-sign — the co-sign just makes it more expensive for factions to test.

*"The rules aren't mine. They predate me. They predate the Wardens. They predate the Cascade. The rule that you don't conduct violence in a place where someone has extended hospitality — that's older than any code anyone has written down. I'm just enforcing something that was always true."*

He has said this once, to a faction leader who wanted to negotiate an exception. There was no exception. The faction leader left. He came back three weeks later and sat at the fire like nothing had happened. Burke poured him a drink. Neither of them mentioned it again.

---

**GPS anchor:**

Wayhouses appear at real-world locations with prior hospitality or way-station function: hotels, inns, roadhouses, large heritage properties on historically-used routes. OSM tags: `tourism=hotel`, `tourism=hostel`, `amenity=pub` (large, with accommodation), `historic=*` with large footprint on a route. Rural Wayhouses skew toward old coaching inns and farmstead accommodation. Urban Wayhouses are former boutique hotels, heritage inns, or converted civic buildings with guest capacity.

There is always at least one Wayhouse within the player's active territory radius. In dense urban areas there may be several. The GPS anchor algorithm prioritizes buildings on or adjacent to Warden Corridors — the Wayhouse and the Corridor network reinforce each other.

---

**The common room at night:**

The scene that defines what a Wayhouse is:

Fire. Three or four tables. A Compact logistics officer eating alone, not in uniform but not trying to hide what she is. Two Hollow at the bar — not Crew, independent, here because there's nowhere else to sleep tonight that doesn't cost something they don't have. A Signal courier who came in on the Corridor and will leave before dawn. A Warden Sworn who is either here to rest or here to watch, and you can't tell which.

The Keeper behind the bar, cleaning a glass, aware of every person in the room.

Nobody talks to anyone they didn't come in with. Nobody makes a move. The fire runs. The food is on the table.

This is the world working as well as it currently can.

---

The Compact installation: controlled, formalized, the safety is conditional on compliance.
The Covenant site: calm near machine zones, the safety comes from the machines' indifference.
The Reckoner camp: mobile, intentionally impermanent, safety through awareness and capacity.
The Holdout community: communal, variable, the safety is as strong as the community's health.

The Warden House Seat: the safety is in the code, which has been kept here for longer than most post-Cascade structures have existed. The permanence is felt. You are somewhere that was built to last and has been maintained because someone believed it should.

Players who spend time at Warden House Seats will feel the temporal difference. The other faction spaces feel post-Cascade. The House Seats feel like something that survived the Cascade. That distinction is load-bearing for the faction's narrative function.

---

## The Table's Presence in the Spaces

The Table — the collective deliberative body of the Old Houses — does not have a dedicated permanent location. It convenes at the nearest available Moot Hall for the Moot. Between sessions, the Table exists as the relationship between the Houses, maintained through the Corridor network, the Marker system, and the Record.

What the Table *has* that other governance structures don't: every decision it makes is somewhere. The Record holds it. An Arbiter witnessed it. The physical documentation of the Table's decisions is distributed across every Moot Hall and House archive in the region. You cannot suppress a Moot decision because there are a dozen copies in a dozen buildings and the Arbiters who recorded it are not all in one place.

This is intentional. The Table was designed to survive factional pressure. The design has been tested.

---

*v0.1 — Warden spaces. Five space types: Moot Hall, House Seat, Maker Spaces, Sworn Chapters, Warden Corridors. GPS anchor logic for each. Spatial register for each. The Table's physical presence established.*
