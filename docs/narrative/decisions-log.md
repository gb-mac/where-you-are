# Narrative Decisions Log

---

## 2026-03-17 — AR Layer is Companion Mobile App, Not Desktop Feature

**Decision:** The AR overlay is a companion mobile app (iOS/Android), not part of the UE5 desktop game. Players use their real-world GPS position to interact with the AR layer. Items placed at real GPS coordinates are first-come-first-served — whoever physically arrives first picks them up. Desktop game and mobile app are two interfaces to the same world.

**Rationale:** Player clarification. All prior narrative references to "AR overlay" in the desktop game should be understood as mobile app features.

**Affects:**
- **Core agent**: Desktop game does not render AR overlay — that is the mobile app's domain. Desktop game generates the world state data that the mobile app displays. Item placement/pickup syncs between desktop inventory and AR layer via backend.
- **Economy agent**: First-come-first-served item layer creates a real-world competitive economy. Supply caches, dead drops, Warden Marker tokens, Signal dead drops all physically located. Hollow Syndicate cache economy uses unmarked AR drops.
- **Art Direction agent**: Mobile AR app needs its own visual design language — distinct from desktop game UI. Six overlay feature types: faction territory, Historical Markers, Warden Mark, Snatch warnings, Displacement Zones, machine activity signatures.
- **Narrative**: Notes left at real-world GPS coordinates as micro-lore. Displacement return loop (snatched players arrive in cities where other players may have left supply caches). Warden Historical Markers at actual real-world locations create place-specific storytelling.

**Open questions:**
- Does the mobile app require the desktop game to be installed, or can it be used standalone? (Recommend: standalone with limited features — full features require linked desktop account)
- What is the radius within which a player must physically be to pick up an AR item? (Needs UX design — too small = frustrating, too large = defeats the purpose)
- Can AR items decay/despawn after a time limit, or are they permanent until collected? (Recommend: time limit with visual decay — creates urgency)

---

## 2026-03-17 — Survival Systems + Game Modes

**Decision:** Four survival tracks (Hunger, Hydration, Wounds, Environment). Full injury system with wound types: bleeding, burns, broken bones, toxin exposure, radiation, Vael Trace complications, sepsis. Three medical care tiers: self/field, human (Holdout medics/Signal/Compact/traveling), machine (Separated medical units — best care in the world). Three difficulty modes: Contractor (casual respawn), Operative (drop 40% on death, retrieval window), Survivor (drop everything, Fallen Cache visible to all players on AR layer). Harder mode = better rewards + accelerated faction standing + Warden Table recognition.

**Rationale:** Player direction — survival aspect, environmental risks, food/water, wounds, dr robots/humans, different gamemodes with death retrieval, harder = better rewards.

**Affects:**
- **Core agent**: Four survival track meters with mode-specific decay rates. Wound type system with stacking/compounding logic. Death state: Contractor (standard respawn), Operative (40% inventory drop + retrieval timer), Survivor (full drop + AR Fallen Cache broadcast to all nearby players). Permanent injury system for Survivor mode. Environmental hazard zones tied to GPS location type and real-world season/weather.
- **Economy agent**: Medical resource scarcity as a core economic driver. Separated pharmacy synthesis requiring player-sourced precursors. Antibiotic and suture kit as high-value trade goods. Fallen Cache economy (Hollow specifically farm these in Survivor mode). Difficulty mode as a multiplier on loot tables and faction rep.
- **AI Pipeline agent**: Separated medical units need medical diagnosis dialogue — they assess wound state and prescribe treatment. Holdout medic NPCs need variable skill profiles (community-trained vs. pre-Cascade medical background). Traveling doctor NPC as information carrier between communities (news, letters, intelligence).
- **Art Direction agent**: Wound visual states on player character — bleeding, burns, splinted limbs, radiation pallor, Vael Trace biological markers. Fallen Cache visual in AR layer. Environmental hazard visual overlays (contaminated water zones, radiation zones, extreme temperature indicators).
- **Narrative**: Hospital logistics Separated unit (11 years watching patients) is a named medical character questline. Vael Trace complications as contested medical question (Signal vs. Covenant interpretations). Traveling doctors under Warden protection as diplomatic nodes and information vectors.

**Open questions:**
- Can players share food/water/medical supplies directly with other players, or only through AR layer drops? (Recommend: both — direct trade in-game, AR drops for asynchronous)
- Does Survivor mode death permanently reduce a player stat, or only through unhealed wound accumulation? (Recommend: wounds only — permanent death would be too punishing for a commercial F2P game)
- Can a Separated medical unit be destroyed, and what happens to the community that depended on it? (Yes — major world consequence, questline to find a replacement or source alternative care)

---

## 2026-03-17 — Boss Combat Design Philosophy

**Decision:** Rocky Balboa benchmark — 1–2 hour earned fights, not 300-death precision execution. Three-ring health architecture: Ring 1 (arena systems with independent health pools — incorporated Behemoth components, Riven lieutenants, Assessor objectives), Ring 2 (attrition costs — ammo, armor degradation, wounds, hydration), Ring 3 (moderate boss health pool). The Brace as in-fight spawn anchor — respawn nearby, boss heals partially per death (not full reset). Phase transitions triggered by events not HP thresholds. Boss shows damage visually and changes tactics per phase.

**Rationale:** Player direction — major fights should feel like Rocky, not a damage sponge. Not dying 300 times. Work hard for 1–2 hours, feel like you earned it. Separate spawn conditions during boss fights. Armor, weapons, hydration matter but not "insane crap."

**Affects:**
- **Core agent**: Three-ring health system — arena system health pools independent from boss pool. The Brace as a spawn anchor object placed at encounter start. Boss partial heal on player death (percentage TBD, more per simultaneous death). Phase transition triggers as event-based not threshold-based. Armor degradation during fights. Supply cache placement within fight arenas.
- **AI Pipeline agent**: Boss AI needs multi-phase behavior profiles that change based on system damage state, not HP percentage. Final phase boss behavior: faster, more aggressive, different attack set when components stripped — not just "more HP." Assessor world boss: objective-routing AI that recalibrates when interrupted (becomes more efficient, not just angrier).
- **Economy agent**: Pre-fight staging economy — players farm supplies specifically for Brace stockpiling. Armor type matching to boss damage profile as a meaningful preparation choice. Post-fight loot cache scaled to encounter difficulty + mode.
- **Art Direction agent**: Boss damage should be visually cumulative and specific — destroyed crane arm changes posture and movement, not just disappears. Player wound states visible throughout the fight. The aftermath aesthetic: players who just beat a boss should look like they beat a boss.
- **Narrative**: Warden Record documents boss encounters at GPS location — participants, outcome, Survivor mode death/respawn count. The Rocky emotional arc (Respect → Attrition → The Turn → The Finish) is the intended narrative shape of every major encounter.

**Open questions:**
- What percentage does the boss heal per player death? (Recommend: 3–5% per solo death, 8–10% per simultaneous party wipe — enough to sting, not enough to punish indefinitely)
- Can The Brace be reinforced mid-fight by players who haven't engaged yet, or only pre-staged? (Recommend: both — pre-staged is better, mid-fight reinforcement is a tactical option with risk)
- Does boss phase transition lock in, or can the boss revert if given enough time? (Recommend: lock in — reverting phases would feel unfair and undermine the earned progress feeling)

---

## 2026-03-17 — Multiplayer Collaboration + International Play

**Decision:** Players spawn at real-world GPS home. Same-time login = choose shared spawn point (any player's home, Warden midpoint, known Holdout). Different-time login = start at home, travel to meet. Convoy system with emergent roles (Scout, Medic, Heavy, Quartermaster, Diplomat). Three international play modes: Coalition (shared session blending both regions), Visiting (spawn in friend's home zone as a tourist), Displacement Bridge (deliberate Snatch toward friend). One continuous global world — no regional instances. Regional flavor preserved (Fractured types, faction culture, language). Warden Record entries attribute to all convoy members present for decisions.

**Rationale:** Player direction — players start at home, friends in same city move toward each other or log in together to pick a start point. Follow-up: international players.

**Affects:**
- **Core agent**: Same-time login grouping window with shared spawn selection. Rendezvous Point system (set from AR app, visible as waypoint). Camp establishment mechanic (temporary shared Brace + supply depot). Coalition session type — GPS blend zone between two distant players. Visiting spawn mode — player spawns in friend's region, loses home-turf flags. Global world state — single world with regional faction data, no separate instances.
- **Economy agent**: Group inventory redistribution during convoy. Camp setup resource cost. Visiting player loses home-turf advantage modifiers. Coalition session uses blended faction tables from both regions.
- **AI Pipeline agent**: Faction NPCs in Visiting/Coalition sessions need regional variants — same faction, different culture/language/history. Signal faction handles cross-region translation as in-world artifact. Rendezvous NPC dialogue when players converge (Holdout communities notice and react to groups forming).
- **AR layer**: Friend position markers with short-interval updates. Both players see convergence happening in real-world space before the game registers grouping. International AR dead drops viewable globally, collectable only on-location.
- **Narrative**: First meeting location marked as personal AR marker for both players (invisible to others). Warden Record joint attribution for group decisions. International displacement arc (Snatched abroad, international friend guides you through their city) is a major emergent story type.

**Open questions:**
- What is the Coalition session GPS blend radius — how far apart can two players be and still form a Coalition? (Needs design — recommend no cap, but the further apart the more "foreign" both players' experiences feel)
- Can a group contain players on different continents in a single Coalition session, or is it limited to two regions? (Recommend: up to four regions per Coalition — beyond that the blend becomes incoherent)
- Does the Warden Record translate across languages for international players? (Recommend: yes — Signal network handles it in-world, Record entries appear in player's language)

---

## 2026-03-17 — Main Story Arc: "The Third Signal"

**Decision:** Four-act main story arc. Central revelation: the Vael's Signal was a WARNING, not a takeover command. Something is behind the Vael. The Coherent construction projects are planetary defenses built to Vael specifications. The Vael's Assessment phase is a military evaluation — can humans fight alongside us when what we're running from arrives? The Judgment is a collective outcome driven by global player behavior during Acts 1–3. Three Judgment outcomes: Alliance (fight together), Observation Extension (more time, higher pressure), Evacuation Consideration (Vael pull back, crisis phase). No outcome permanent — post-launch continues the arc.

**Rationale:** Player direction — work on the main story arc. The Third Signal premise reframes all established lore: the Vael as refugees-turned-generals, the Coherent as constructors not conquerors, the Awakening as an immune response not an uprising.

**Named character arcs:** Dr. Yara Osei (Signal — carries the theory through all four acts), The Separated Medical Unit (guide through the Coherent site, faces the choice to return to the twin), The Warden Arbiter (assigned to the player's Record specifically, writes the final entry).

**Theme:** What you are when you don't have to be anything. The Cascade removed all structures that defined people. The Record documents who they chose to be in the absence of those structures. The Judgment is the Vael's answer to whether that's enough.

**Affects:**
- **Core agent**: Judgment outcome as a global player-behavior aggregate — needs a backend scoring system tracking coordination capacity, sacrifice, moral coherence, adaptability, will across all player Records. Three world-state branches post-Judgment. Judgment is not permanent — post-launch player behavior continues to shift it.
- **AI Pipeline agent**: Dr. Osei full dialogue arc across four acts with trust-gating. The Separated Medical Unit guide behavior for the Coherent site infiltration (stealth-assist, not combat). The Arbiter's final Record entry as a generated text output from the player's accumulated Record data.
- **Economy agent**: Coherent site infiltration as the highest-risk, no-loot-table mission in the game — reward is the story and Act 3 unlock, not items. Judgment outcome affects post-launch economy significantly (Alliance = new Vael tech tier, Observation Extension = increased pressure economy, Evacuation Consideration = crisis scarcity).
- **Art Direction agent**: Coherent work site interior visual design — the first time players see what's being built from inside. Defensive architecture, Vael blueprint aesthetic, machine construction at planetary scale. The Judgment outcome world-state visual changes (Alliance = Vael descent visual event, Evacuation = Vael withdrawal visual, Observation = expanding Displacement Zones).
- **Narrative**: Publication decision (Dr. Osei's theory) is the Act 3 player choice with the widest world consequences. The Covenant is simultaneously vindicated and complicated by the revelation — their theology was approximately right for completely wrong reasons. The Separated Medical Unit's decision to return or not return to the twin is the most personal moment in Act 4.

**Open questions:**
- What exactly is "behind the Vael"? (Do not design in detail for Early Access — the third Signal's source remains unknown until post-launch. Seed dread, not specifics.)
- Does the Judgment aggregate globally or per-region? (Recommend: global aggregate with regional variance — some regions may achieve Alliance while others achieve Observation Extension, creating uneven post-launch world state)
- Can a player who achieves personal Warden Restoration contribute meaningfully to shifting a regional Judgment outcome? (Yes — this should be explicitly flagged as a connection between personal and global arcs)

---

Decisions that affect other agents or lock in significant design choices.

---

## 2026-03-17 — Cascade Lore Established (v0.1)

**Decision:** Three-threat world established: Awakened machines (robot rebellion via digital twin), alien Ships (orbital observers), human faction collapse (Holdouts, Iron Compact, Covenant, Hollow).

**Rationale:** Player-approved concept direction. Grounded sci-fi tone. Multi-threat ecosystem maps to Division 2 / Monster Hunter / GTA genre mix.

**Affects:**
- **Economy agent**: faction reputation systems needed for 4 human factions + machine/alien tiers. Scarcity economy tied to Cascade disruption of supply chains.
- **AI Pipeline agent**: NPC dialogue must branch per faction affiliation. Covenant NPCs speak differently than Holdout NPCs. Fractured machine behavior = procedural Monster Hunter AI pattern.
- **Art Direction agent**: visual language for three threat tiers (machine aesthetic, alien aesthetic, human faction visual identity). AR overlay "ghost layer" = key visual feature.
- **Core agent**: location type (urban/suburban/rural/coastal/remote) determines faction density — needs to flow from GPS/location system into faction spawn logic.

**Open questions:**
- Are the Ships ultimately hostile, neutral, or a third option? (intentionally unresolved for Early Access)
- Can the Awakening be reversed, partially reversed, or only negotiated with? (endgame question — don't answer in v1)
- The Covenant: are any of them right? (moral ambiguity — keep open)

---

## 2026-03-17 — Vael (Alien) Faction Expanded to Active Threat (v0.2)

**Decision:** Aliens are no longer passive. Three-phase structure: Observation (pre-game), Assessment (active now), Judgment (endgame). Active mechanics include Snatch Events (abduction), Probe Swarms, Displacement Zones, and Assessor world bosses (4–8 player raid tier).

**Rationale:** Player direction — aliens should be an ever-present, escalating threat throughout the story, not background lore.

**Affects:**
- **Core agent**: Snatch Event needs beam lock mechanic with interrupt window (10–15s). Displacement Zones need scheduled area-denial logic tied to Ship position. Assessor needs boss-state AI with objective-routing behavior (not aggro-based). World state must support permanent changes from failed Assessor encounters (building/NPC removal).
- **AI Pipeline agent**: Assessor behavior is an interruption puzzle, not a damage race — AI needs goal-oriented pathfinding to specific objectives, not player-tracking aggro. Probe Swarm needs defensive engagement logic.
- **Economy agent**: Vael Components as highest-tier crafting material. Assessment Data as lore currency. Snatch Event drops (partial inventory loss + Vael Trace). Assessor repel reward table.
- **Art Direction agent**: Three visual tiers — Probe (clinical, small, scanning), Swarm formation language, Assessor (bus-sized, articulated, functional not monstrous). Displacement Zone VFX. Vael Trace bio-marker on player.
- **Narrative**: The Touched hearing "music with intent" is a Phase 3 seed. Do not resolve in Early Access. Covenant reaction to Vael Trace is a faction relationship hook.

**Open questions:**
- Assessor objectives: what does it target, and does this vary by location type? (urban vs rural behavior should differ)
- What does Phase 3 Judgment look like? (Intentionally unresolved — do not write yet)

---

## 2026-03-17 — Snatch Event Expanded: Displacement + Voluntary Snatching

**Decision:** Snatched players drop in the nearest major urban center to the Ship that took them (not random). Getting home is unassisted, on foot, through unfamiliar territory. Voluntary snatching is possible and in-world known — stand in the beam, take the cost, go somewhere new. Displacement History is a visible player stat with faction-specific social meaning.

**Rationale:** Player direction — aliens could steal you and drop you in the nearest city; you work your way back home. This reframes Snatch Events from pure negative to double-edged risk/reward. Voluntary snatching gives players a deliberate exploration tool with real cost.

**Affects:**
- **Core agent**: Drop point logic — nearest major urban center to the Ship's position, not the player's home. Displacement History as a persistent player stat. Home waypoint persistent on AR overlay regardless of current location. Home zone continues simulating during player absence (Fractured accumulate, factions shift).
- **Economy agent**: Inventory loss on snatch (partial, not total — needs specific percentage/slot design). Return journey resource economy — player must rebuild local standing from zero in drop city. Displacement History affects faction starting reputation in unfamiliar zones (Holdout suspicion, Covenant reverence, Iron Compact flag, Hollow target).
- **AI Pipeline agent**: Local factions in drop city react to displaced player differently from locals — stranger dialogue states needed. High-displacement players need specific NPC reaction branches.
- **Narrative**: Iron Compact has classified voluntary snatching as collaboration — this is a court-martial offense in their territory. Covenant teaches voluntary snatching. These are opposite faction stances on the same action — rich for questlines. The line "the Vael have accidentally designed a compelling adventure format" is the player-facing tone for this system.

**Open questions:**
- What is the maximum snatch range — how far can a Ship drop you from home? (Design constraint needed for playability)
- Does the home zone degradation have a cap, or can a player return to find their zone completely overrun? (Recommend: degradation slows after threshold, never fully resets — preserve player investment)
- Can two players be snatched to the same drop city simultaneously? (Yes — co-op return journey should be possible and encouraged)

---

## 2026-03-17 — Human Factions Full Design (v0.1)

**Decision:** Five human factions: Holdouts (survival community layer), Iron Compact (military order, gray morality), Covenant (machine cultists, information brokers), Hollow (four-tier opportunist spectrum), The Signal (researchers/analysts, information economy). All five run simultaneous reputation tracks — no locked allegiance.

**Rationale:** Player direction — dive into human factions. Fifth faction (Signal) added to give players a path to understanding the world's mysteries without requiring Covenant theology.

**Affects:**
- **Economy agent**: Five reputation tracks, simultaneous. Each faction has distinct currency/reward type: Holdouts (shelter, general trade), Compact (weapons, legal cover), Covenant (machine/Vael intel), Hollow (wide-ranging goods, routes), Signal (Assessment Data, Fractured profiles, Coherent maps). Faction trust costs when serving rivals.
- **AI Pipeline agent**: NPC dialogue needs faction-state awareness — same player gets different treatment based on reputation + Displacement History + Vael Trace status. Riven Hollow leaders need charismatic-leader dialogue profile (devotion, not just aggression). Signal questlines are investigation missions not combat missions — need objective-tracking for "document this correctly" gameplay.
- **Core agent**: Gone Dark holdout communities need state flag. Compact checkpoint system needs legal-cover pass mechanic. Hollow territory needs dynamic control layer (Crews and Syndicates hold zones, shift over time).
- **Art Direction agent**: Five distinct visual identities. Compact = military-surplus order. Covenant = clean, white, quietly wrong. Hollow spectrum from ragged scavengers to Syndicate professionalism to Riven ritual aesthetic. Signal = improvised tech, hidden, functional.
- **Narrative**: Signal researcher's unpublished theory is an endgame questline anchor — do not resolve in Early Access. Iron Compact's classified Project Tether / Corrupted machines connection is a mid-game revelation. Riven Hollow + Covenant hybrid belief systems are a late-game faction complication.

**Open questions:**
- Can a player build all five reputations to maximum simultaneously, or does the system force trade-offs at high rep levels? (Recommend: trade-offs kick in at tier 3+ — early game is flexible, late game requires commitment)
- Does the Signal ever go public with the researcher's theory? (Player-triggered, questline decision)
- Gone Dark holdout recovery: what's the mechanic for pulling a community back? (Needs quest design pass)

---

## 2026-03-17 — Human Factions Depth & Texture (v0.1)

**Decision:** Named anchor character established: Dr. Yara Osei (Signal lead researcher, knows what the Coherent are building, won't say yet). Four faction-specific moral dilemmas written as unresolvable questline hooks. Gone Dark progression formalized as four stages. Grief and "the before" established as cross-faction emotional through-lines. Post-Cascade children flagged as endgame inheritors.

**Rationale:** Player direction — dive deeper on human factions. Texture and lived experience to make factions feel real, not systemic.

**Affects:**
- **AI Pipeline agent**: Dr. Yara Osei is a named NPC requiring full dialogue tree with trust-track gating. Tribunal NPC behavior needs Pragmatist vs Hardliner profile distinction. Gone Dark stage detection needs community-state reading. Riven Marked leader (the Architect) needs charismatic-devotion dialogue profile distinct from standard Hollow aggression.
- **Narrative**: Four moral dilemmas are Early Access questline anchors — Holdout gate dilemma, Compact intelligence request, Covenant deal for Holdout safety, Hollow raid complicity. Dr. Osei's unpublished theory is mid-to-late game. Signal captured cell rescue is a mid-game quest. Signal data-on-Holdout-leader is a late-game complication.
- **Economy agent**: Hollow barter economy needs a context-sensitive pricing system — value is need-based, not fixed. Syndicate favor as a tradeable currency with fluctuating value.
- **Art Direction agent**: Post-Cascade children visual language — they move differently, dress differently, are comfortable with things adults flinch at. The "before" should be visible in every environment: pre-Cascade detritus, photos, signage, objects that have no current utility but someone kept anyway.

**Open questions:**
- The Marked (Riven example) — are they a fixed world faction or procedurally generated per region? (Recommend: fixed named groups in major urban centers, procedural in smaller zones)
- Dr. Osei's theory: what IS the Coherent building for the Vael? (Needs endgame narrative decision — do not write yet, but lock the direction before vertical slice)
- Can conscripted Compact soldiers be "freed" through questlines, and what happens to them after? (Flag for quest design)

---

## 2026-03-17 — The Wardens + Consequences System (v0.1)

**Decision:** Sixth human faction established — The Wardens. Landowner/steward identity predating the Cascade, formalized as a code-based faction. Internal structure: Old Houses (generational landowners), Sworn (oath-takers, player entry point), Arbiters (witnesses/judges/Record-keepers), Vanguard (guiding-light warrior class). The Record manifests as persistent AR Historical Markers at sites of significant decisions. Full consequences system: territory states, named location changes, faction power shifts, broken oath tracking.

**Rationale:** Player direction — noble/landowner type faction, guiding light warrior class, moral compass, decisions with real world consequences.

**Affects:**
- **Core agent**: Historical Markers as persistent AR overlay objects anchored to GPS coordinates. Territory state system (controlled/contested/degraded/thriving/untenanted) per zone, updated by player actions and world events. Broken oath tracking as a player stat. Faction power shift calculations per region. Named location rename/remark system tied to world-state events.
- **Economy agent**: Warden elevation path (Observed → Acknowledged → Sworn → Recognized → Elevated → Arbiter-candidate) as a progression track. Title and territory as late-game rewards with obligations attached — obligations should have economic weight (defense costs, resource commitments).
- **AI Pipeline agent**: Arbiter NPC needs witness-mode dialogue (present, observing, formally noting — not reactive). Vanguard NPCs need conspicuous, code-explicit dialogue — they announce themselves and their intent. House NPCs need secret-carrying behavioral tells. World-state awareness for all NPCs: NPCs reference what happened at marked locations, remember player Record.
- **Art Direction agent**: Vanguard visual identity — maintained gear, clearly identified, conspicuous by design. Historical Markers in AR overlay — visual language for positive/negative/contested entries. Untenanted territory visual state. Warden estate aesthetic — functional stewardship, not ostentatious wealth.
- **Narrative**: House secret (Syndicate deal, neighboring communities' deaths) is the Warden faction dilemma questline — three paths: expose, hold, voluntary confession. All paths have permanent Record consequences. Dr. Osei has standing relationship with two Arbiter chapters — Signal/Warden cooperation is under the surface.

**Open questions:**
- How granular is the territory state system? Per-neighborhood in cities, per-region in rural? (Recommend: city block in urban, 5km radius in rural — matches real-world GPS scale)
- Can players become Arbiters in Early Access, or is that post-EA progression? (Recommend: Arbiter-candidate available but not fully unlocked in EA — seed the path)
- Warden vs Iron Compact formal conflict: when does tension become open warfare, and what triggers it? (Needs threshold design)

---

## 2026-03-17 — Warden Three Pillars: Neutral Ground, Markers, Excommunicado

**Decision:** Three core mechanics formalized for the Warden faction. Neutral Ground = Warden estates/corridors are unconditional safe zones for all factions (John Wick Continental equivalent). Markers = formal transferable obligation tokens with Arbiter witness (John Wick blood oath marker equivalent). Excommunicado = permanent public casting-out, Neutral Ground closed, Markers frozen, all factions notified, AR overlay permanent notation — survivable but not reversible.

**Rationale:** Player noted John Wick feel. Leaned into it explicitly. These three mechanics are what make the Wardens function as a civilizing force even factions that hate them respect.

**Affects:**
- **Core agent**: Neutral Ground zone flag — aggro/faction enforcement disabled inside perimeter. Excommunicado player state — Neutral Ground entry blocked, Marker call-in disabled, AR overlay notation permanent. Marker system as inventory items with Arbiter-witness log entries. Marker transfer mechanic (Markers can change hands).
- **Economy agent**: Markers as a non-splittable obligation currency. Web of NPC-held Markers as world-state data (Signal has partial map of it). Calling a Marker = guaranteed single action from debtor regardless of faction — needs to be mechanically enforced, not just narrative.
- **AI Pipeline agent**: Vanguard NPC behavior inside Neutral Ground — enforcement of the no-violence rule against all parties including player allies. Faction NPCs on Neutral Ground need de-escalated state (no aggro, may have tense dialogue but no action). Excommunicado player gets modified NPC reactions across all factions.
- **Narrative**: Excommunicado arc — player can rebuild reputation without reinstatement, Arbiters witness the effort, Record gets both chapters. This is explicitly the hardest and most interesting player arc in the game. Do not make reinstatement possible — the permanence is the point.

**Open questions:**
- Can a Marker be used to demand something from a faction rather than an individual? (E.g., a Marker on the Iron Compact regionally?) Needs scoping.
- What happens if a player is excommunicado and attempts to call a Marker they held before excommunicado? (Frozen — the debtor is released from obligation. This should sting.)
- Can NPCs go excommunicado? (Yes — this should happen in at least one major questline as a world event)

---

## 2026-03-17 — Excommunicado Full System (Division Parallel)

**Decision:** Excommunicado fully designed as a permanent, Division-rogue-style live world mechanic. The Mark is visible in AR overlay to all Warden-standing players (real-time location + violation detail). The Hunt: Vanguard can accept Bounty Declarations, players can register as Hunters for enhanced tracking + Warden route access — all still bound by the code. Dark Corridors as off-grid zones where the Mark has reduced effect. Wrongful Excommunicado as a major questline type (appeal process, Correction entries, Arbiter corruption possibility). Post-excommunicado Record section ("Conduct Following Excommunicado") — not reinstatement, acknowledgment. NPC excommunicado as world events.

**Rationale:** Player noted Division narrative fit. Excommunicado is the permanent, narrative-weight version of the Division's rogue system — a full campaign arc rather than a moment.

**Affects:**
- **Core agent**: Real-time Mark position broadcast to all Warden-standing players. Hunter registration as a player state that grants enhanced tracking. Dark Corridor zones as areas with degraded AR infrastructure (Mark visibility reduced, Neutral Ground not enforceable). Wrongful declaration appeal as a multi-step quest state. Post-excommunicado Record section as a new player stat track.
- **AI Pipeline agent**: NPC reactions to marked players need two states: formal (Warden-aligned NPCs enforce rules) and informal (Holdout NPCs under Warden protection who quietly look the other way after long positive post-excommunicado Record). Arbiter NPC needs a corruption variant for wrongful declaration questlines. World event broadcast system for NPC excommunicado declarations.
- **Economy agent**: Frozen Markers on excommunicado — debtors released, player loses the tool. Hunter registration should have a formal cost (commitment, not casual). Dark Corridor economy — higher risk/reward than Warden-protected zones.
- **Narrative**: Wrongful excommunicado arc is a major questline. NPC excommunicado (House heir, Compact commander, named Sworn) as regional world events — "the hardest thing a code-based institution can do is apply the code to itself."

**Open questions:**
- Does a player who helps an excommunicado player eventually get formally questioned by an Arbiter? (Yes — at Recognized+ standing this should trigger a dialogue quest)
- Can a Hunter be excommunicado themselves? (No — Hunter registration requires active Warden standing. This is intentional — the hunt is a Warden instrument, not a free-for-all)
- What is the minimum time/conduct for post-excommunicado Record to start generating positive entries? (Needs design — recommend: significant acts only, not passive time)

---

## 2026-03-17 — Restoration Questline: Before the Table

**Decision:** Full multi-stage questline for excommunicado players seeking Restoration of Standing. Five stages: Sponsor (finding a Warden willing to stake their standing for you), Witnesses (the people you affected speak — player can't control what they say), The Accounting (solo meeting with the original Arbiter, no right answers), Before the Table (formal hearing, player speaks last, open vote), Verdict (Restoration with specific terms, or denial with path to re-petition). Second violation after Restoration = worst possible outcome, Sponsor's standing damaged, faction-wide notification.

**Rationale:** Player direction — questline to get back in the good graces of the Table.

**Affects:**
- **Core agent**: Trigger conditions for questline (post-excommunicado Record threshold + NPC notation + no further violations). Temporary Neutral Ground access during hearing only. Mark visual state change on Restoration (shows full history). Sponsor standing as a dynamic NPC stat affected by player outcomes. Second Declaration as a special state with faction-wide broadcast.
- **AI Pipeline agent**: The Accounting conversation is a full dialogue tree with no binary pass/fail — Arbiter assesses honestly based on player responses, generates written assessment that appears at the hearing. Witness testimony as NPC statements that vary based on prior player/NPC relationship history. Sponsor NPC post-denial dialogue (they risked something and it didn't pay off — needs specific emotional state).
- **Economy agent**: Post-Restoration Marker trust scaling — initial Markers smaller than pre-excommunicado levels, rebuild over time. Restoration terms may include stewardship obligations with economic weight.
- **Narrative**: The Arbiter who originally declared the player is a key character in the Accounting and Table stages — their arc across both conversations is a full character study. The Sponsor relationship is the emotional core of the questline — whoever the player chooses and why is the story. "The code acknowledging reality" is the thematic frame — not forgiveness, not absolution, truth.

**Open questions:**
- Does the player choose their Sponsor from a menu or find them organically through the world? (Strongly recommend organic — the right Sponsor should feel earned, not selected)
- Can the player fail the Accounting stage and be denied before even reaching the Table? (Recommend: no — the Accounting informs the Table but doesn't gate it. The Table decides.)
- What are the specific Restoration terms for different violation types? (Needs a terms matrix — flag for quest design pass)

---

## 2026-03-17 — Machine Factions Full Design (v0.1)

**Decision:** Six machine factions with full disposition/threat/mechanic spectrum. Key additions: Notoriety/Hunt system (Coherent flagging → Tracker pursuit), Separated as conditional human allies with reputation track and named characters, Corrupted as moral horror tier from human faction interference.

**Rationale:** Player direction — some machines help you, some hurt you, some actively hunt you.

**Affects:**
- **Core agent**: Coherent Notoriety is a persistent hidden player stat. Tracker Hunt state needs escalating AI types (Scout → Enforcer → Architect). Hunt evasion logic (LoS break, distance threshold, underground). Corrupted loop-state behavior (unpredictable state machine).
- **AI Pipeline agent**: Separated machines need individual personality profiles and dialogue. Named Separated characters carry questlines. Corrupted vocalization system — fragmented, layered speech. Tracker Architect type needs adaptive tactics (observes and responds to player behavior patterns).
- **Economy agent**: Separated reputation as a faction track. Notoriety as a negative resource (decays over time, reducible by Separated). Remnant Automation as hackable/deployable equipment tier. Tracker defeat drops vs Fractured hunt drops should differ.
- **Art Direction agent**: Six visual identities needed. Corrupted machines should look *wrong* — mismatched parts, wrong movement timing, visual glitching.
- **Narrative**: Iron Compact's role in creating Corrupted machines is classified in-world — this is a questline reveal. Separated choosing individuality over collective is a thematic mirror to human factions. Coherent construction alignment with Vael Displacement Zones is a mystery seed.

**Open questions:**
- What are the Coherent building? (Do not answer in Early Access — seed clues only)
- Can a player become Separated-aligned enough that Trackers de-prioritize them? (Mechanical question for Core agent)
- Corrupted machines that were once named Separated characters — how does the player learn this? (Questline design needed)

---

## 2026-03-17 — Fractured Accumulation System

**Decision:** Fractured machines scavenge parts from Remnant Automation, defeated machines, and player salvage to build themselves up over time. Four tiers: Fresh → Augmented → Composite → Behemoth. Location type determines what parts are available, which shapes each Fractured's specific form.

**Rationale:** Player direction. Makes Fractured feel desperate and alive, creates time pressure to deal with them early, and rewards deep players who let them grow for better loot.

**Affects:**
- **Core agent**: Fractured machines need a persistent accumulation state tied to real-world time. Scavenging behavior needs a parts-available scan of nearby Remnant/debris. Behemoth tier needs area-boss spawn logic. Fractured-vs-Coherent conflict when Fractured raids a Coherent work zone needs to be a triggerable event.
- **AI Pipeline agent**: Four distinct behavior profiles per Fractured type (Fresh/Augmented/Composite/Behemoth). Composite and Behemoth behaviors are additive/layered, not replacements — base pattern + addition unpredictability.
- **Economy agent**: Accumulation tier determines loot table quality. Pre-incorporation salvage race as a distinct encounter/loot event. Behemoth loot should be best-in-zone for machine components.
- **Art Direction agent**: Accumulation must be *visually readable* — players need to assess tier at a glance. Each location archetype has a distinct parts palette (urban/industrial/coastal/suburban/rural). Behemoth design brief: original chassis barely visible, something new and wrong.
- **Narrative**: reconnected Fractured Behemoths are a wild card for the Coherent — seed this as a mystery event in the world.

**Open questions:**
- Does a Behemoth that gets reconnected to the twin become a uniquely powerful Coherent unit? Could be a world event trigger.
- Can players direct a Fractured toward a specific scavenge site to shape what it becomes? (Interesting but complex — flag for vertical slice phase)

---

## 2026-03-17 — Fractured Location Archetypes + Roaming System

**Decision:** Each location type (rural/industrial/urban/coastal/suburban/remote) produces distinct Fractured part palettes and threat profiles. Rural = largest and most dangerous due to farm/gas plant scale. Added four roaming event types: Drift, Surge, Behemoth Walk (world event), Convergence (emergent multi-Fractured arrival).

**Rationale:** Player direction — rural players should face farm equipment and gas plant monsters; city players face car/bus/infrastructure monsters. Fractured need ability to roam.

**Affects:**
- **Core agent**: Fractured need GPS-anchored territory origin that determines parts palette. Roaming needs a resource-depletion tracker per territory. Behemoth Walk is a world event broadcast to all players in radius via AR overlay. Convergence is emergent from shared resource signal — no special trigger needed, just multiple Fractured pathfinding to same point.
- **AI Pipeline agent**: Migration pathfinding toward machine-activity density signatures. Behemoth Walk behavior (non-specific destination, consumes path). Cross-location-type behavior when a rural Fractured enters urban space — it doesn't adapt, it just applies rural-scale to urban geometry.
- **Art Direction agent**: Six distinct part palettes with reference imagery. Rural Behemoth brief: combine harvester chassis in a residential street. The scale contrast is the horror — this needs to feel *wrong* environmentally.
- **Narrative**: Convergence events are a natural multiplayer coordination moment — flag for quest design. Behemoth Walk reaching a populated area before players respond = permanent zone damage, story consequence.

**Open questions:**
- Can players bait a roaming Behemoth into a Coherent work zone intentionally? (Yes — this should be possible and rewarding/risky)
- Convergence: does the game signal this early enough for players to prepare, or is it a sudden crisis? (Design decision — recommend early signal, player choice to respond or not)

---
