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

## 2026-03-17 — Late Game Convergence System

**Decision:** Late-game three-way collaboration between humans, Vael, and Coherent/Separated machines as preparation for the incoming third threat. Triggered by regional Alliance Judgment outcome. No formal faction — a condition players contribute to and earn access through. Four Convergence operation types: fortification completion runs, Convergence salvage (advance scout enemies), interpretation sessions (Vael/machine/human communication), joint defense operations. Four loot tiers: Human Faction → Machine Components → Vael Components → Convergence Gear (all three civilizations combined, purpose-built for the incoming threat). Separated Medical Unit late-game choice: return to the changed twin or remain outside. Post-EA first content drop = the third threat arrives at the most defensible location.

**Rationale:** Player direction — late game dynamic where aliens and robot factions collaborate into better loot to prepare for the incoming threat. May end up fighting with the aliens against another force.

**Affects:**
- **Core agent**: Convergence operation mission type (joint — requires Coherent presence, Vael Probe activity, player Judgment contribution check). Advance scout enemy tier (new enemy type, distinct from all established factions). Convergence Salvage as new loot category. Convergence gear crafting requires all three input tiers simultaneously.
- **Economy agent**: Four-tier loot progression with Convergence as the cap. Convergence gear stat profile: primary against advance scout damage types, secondary environmental resistance, tertiary joint-operation bonus. Convergence gear crafting recipe requires: Vael Components + Coherent materials + Signal/human crafting blueprint + Judgment-access unlock. Cannot be bought — must be made through the collaborative process.
- **AI Pipeline agent**: Coherent units in Convergence zones have cooperative behavior profile (guidance gestures, resource flagging) in addition to standard indifferent/hostile profiles. Vael Probe late-game behavior: deposits at Record-significant locations, coordination overlay during joint defense operations. Advance scout enemy AI: unknown origin, different attack patterns than anything established — players have no prior knowledge to draw on.
- **Art Direction agent**: Convergence gear visual design — visible joins between human/machine/Vael aesthetic elements. Not sleek, not alien, collaborative. The craftsmanship of multiple makers. Convergence operation marker type for AR mobile layer (distinct from Historical Markers). Advance scout visual design: not Fractured, not Coherent, not Vael — something new and wrong.
- **Narrative**: Separated Medical Unit late-game conversation and choice is the most personal moment in the Convergence arc. Covenant fracture (theology validated but for wrong reasons) is a major late-game faction event. Riven Hollow intensification in response to Convergence is the late-game human threat escalation. Post-EA first content is the third threat's first major appearance — design seeds only, not specifics.

**Open questions:**
- Does Convergence gear degrade like standard armor, or does it require Convergence-specific maintenance? (Recommend: Convergence-specific maintenance — keeps players in the collaborative loop)
- Can excommunicado players access Convergence operations? (Recommend: not until Restoration — the Vael's assessment reads the Record, and excommunicado is in it)
- What do the advance scouts look like and what do they do? (Seed in late-game Convergence operations only — do not fully design until post-EA content planning)

---

## 2026-03-17 — Rolling Release Roadmap + Live Service Engagement Systems

**Decision:** 18-month rolling release to month 24 Third Threat arrival. No expansion drops — continuous world evolution. Daily/weekly/monthly engagement loop. Three engagement systems: Growing Food (real-time crops tied to real-world GPS location and actual seasons), Making Munitions (real-time production runs, 4 quality tiers, 2–72 hours), Scavenging (AR layer daily nodes + field nodes + event nodes + seasonal nodes). Full 18-month content roadmap with story beat anchors at months 3, 6, 9, 12, 15, 18, 24.

**Rationale:** Player direction — Linux-style rolling release over 18 months, next threat 2 years after release, growing food/making munitions/scavenging/daily-weekly quests for long-term engagement.

**Affects:**
- **Core agent**: Real-time crop growth tied to GPS location + real-world season/weather API. Munitions production as a time-gated background process. Daily scavenge node respawn timer system. Weekly world event trigger from world-state consequences (not random). Advance scout enemy tier progression over 18 months (light → probing → footholds → territory). Regional Judgment state divergence from month 10.
- **Economy agent**: Daily/weekly quest reward tables per faction. Munitions production cost/output matrix by tier. Crop yield modifiers (physical visit bonus, seasonal multiplier, community plot sharing). Scavenge node loot tables by location type and season. Convergence gear maintenance cost (ongoing resource sink that keeps players in the collaborative loop).
- **AI Pipeline agent**: Faction daily/weekly quest generation (procedural within faction-specific templates). World event consequence tracking (Surge → Holdout shrinkage if ignored). Named NPC arc progression tied to story beat months.
- **AR layer (game-mobile repo)**: Crop status display and care interaction. Daily scavenge node display and collection. World event notifications. Advance scout incursion alerts. Vael Ship repositioning visual updates.
- **Narrative**: Month 18 "world goes quiet" before month 24 arrival is the key dread beat — design this carefully. Year 1 anniversary global assessment event should feel earned not artificial. Dr. Osei's month 18 final broadcast is the narrative capstone of the 18-month arc.

**Open questions:**
- Does crop growth pause when the player is inactive for extended periods, or does it continue and potentially fail? (Recommend: continues but with decay — crops need attention, long absence has consequences but isn't catastrophic)
- Can munitions production be stolen by other players or raided by Hollow? (Recommend: yes in Operative/Survivor modes — adds genuine risk to production investments)
- What triggers the month 24 Third Threat arrival — is it a fixed date or a world-state threshold? (Recommend: world-state threshold with a fixed date floor — arrives when the Convergence reaches sufficient global completion, no earlier than month 24)

---

## 2026-03-17 — Launch: First 90 Days Design

**Decision:** Full first-90-days player journey designed. Core hook: "Your street. Your neighborhood. Your world — but wrong." Day 1 structure: GPS spawn → Fresh Fractured encounter → Holdout organic introduction → three hooks planted (community/emotional, Coherent mystery/intellectual, Snatch Event/visceral). Week-by-week retention design through month three. Month 3 capstone: first Behemoth Walk world event (community coordination test, both outcomes interesting). Foundation complete checklist at month 3 end.

**Rationale:** Player direction — focus on first 3 months, hook people, solidify foundation before game kicks off.

**Key design principles established:**
- No tutorial island — the world IS the tutorial
- Every system introduced through a story beat, not a UI prompt
- The displacement experience in week 2 (early enough to be discovery, not punishment)
- Warden oath offered mid-month 2 as a meaningful player choice, not forced
- Dr. Osei's name seeded in month 3 Signal communications as a breadcrumb only
- Behemoth Walk as month 3 world event: tests everything built in first 90 days
- Record tells the player's story back to them — retention through personal history

**Affects:**
- **Core agent**: Day 1 spawn sequence (quiet neighborhood → first Fractured encounter within 60 seconds, no loading screen narrative). Tutorial delivered through world state not UI. Snatch Event in week 2 as near-mandatory experience. Warden oath dialogue flow at month 2 milestone. Month 3 Behemoth Walk world event trigger (regional, 48-hour advance warning).
- **Economy agent**: Day 1 crop planting introduction (gentle, one plant, AR layer check-in taught). Week 1 AR daily scavenge node introduction. Month 2 munitions precision tier unlocked via Signal blueprint. First harvest timing tuned to month 2 for emotional payoff.
- **AI Pipeline agent**: Day 1 Holdout NPC must feel like a person not a quest dispenser — specific problem, specific gratitude, remembers the player on day 2. Day 3 Sworn NPC introduction through helpful action not recruitment pitch. Month 2 Covenant Shepherd introduction through genuine offer not antagonism.
- **Narrative**: Day 1 has three specific hooks planted (community/mystery/threat) — all must be present in first session. Month 2 named Fractured hunt is the Monster Hunter loop tutorial. Month 3 Composite hunt introduces three-ring health architecture through experience. Behemoth Walk outcome (stopped or not) both narratively valid — design consequences for both.

**Open questions:**
- What is the Holdout NPC's specific personal problem on day 1? (Needs specific character — recommend: a child who needs medicine from a dangerous location. Universal emotional hook, clear quest, appropriate risk level for day 1 gear)
- How long is the displacement return journey tuned for? (Recommend: 2–4 hours across 2–3 sessions — long enough to feel like an adventure, short enough not to feel like punishment. Must have other players visible in the drop city to establish multiplayer awareness)
- Does the month 3 Behemoth Walk fire in ALL regions simultaneously, or is it staggered? (Recommend: staggered by region — fires when the regional Fractured accumulation reaches threshold, so different regions experience it at different times. Creates word-of-mouth as some regions hit it earlier)

---

## 2026-03-17 — Pre-Launch Campaign, Arena Mode & Hand-to-Hand Combat

**Decision:** Three systems designed together. Pre-launch: AR mobile app 3 months before desktop, Signal faction on real social media 6 weeks before mobile (ARG), pre-launch Record entries carry to desktop launch, first crops planted pre-launch. Arena: three modes (The Fracture/PvE waves, The Reckoning/PvP, The Proving Ground/PvPvE), Assessment Zone narrative justification, beta use for Fracture mode, ongoing casual layer post-launch. Hand-to-hand: Arkham Asylum freeflow system, wound integration changes combat options, Vanguard style variant for Warden players, environmental takedowns location-typed to real-world zone.

**Rationale:** Player direction — pre-game to entice players, free-for-all arena for beta and time-limited play, Arkham Asylum hand-to-hand combat.

**Affects:**
- **Core agent**: Freeflow combat system (Strike/Counter/Evade/Takedown with combo meter). Counter indicator system above enemy heads. Wound state flags that modify combat options (reduced combo options, slower meter, limited takedowns). Environmental takedown trigger zones by location type. Vanguard style variant gated to Warden standing. Arena zone type with survival system suspension. Wave escalation system (Fresh→Augmented→Composite→Behemoth). PvP zone border crossing as consent mechanic.
- **Economy agent**: Arena beta loot carries to main game. Wave completion rewards separate from open-world loot tables but same item pool. Reckoning Proving log as a separate sport record (not Warden Record). Pre-launch AR cache rewards that carry to desktop launch.
- **AI Pipeline agent**: Human enemy combat AI profiles (Hollow crowd vs. Crew vs. Compact soldier vs. Vigil — four distinct fighting styles). Machine enemy combat profiles (Remnant fixed-pattern, Fractured territorial-lurch, Composite/component-modified). Mixed combat AI (human + machine simultaneously in Proving Ground). Stand-down NPC response system (some take it, some don't, they remember).
- **AR layer (game-mobile repo)**: Pre-launch Signal ARG interface. Faction recruitment flow. Pre-launch daily quests. Crop planting before desktop launch. Assessment Zone markers.
- **Narrative**: Signal ARG on real social media — requires real social media accounts and a content strategy. Pre-launch Record entries acknowledged in the Warden Sworn's day-3 dialogue. The Reckoning's "Proving" format (formal challenge, witnessed, in the Record) is how Warden disputes are formally resolved. Vanguard stand-down option and NPC memory is the code made physical in combat.

**Open questions:**
- Does the pre-launch ARG require a dedicated community manager, or is it seeded content that runs without live moderation? (Recommend: seeded content first, community manager available for major decoded reveals — don't promise responsiveness you can't sustain as solo dev)
- Arena beta: is this a separate download or part of the mobile app? (Recommend: separate desktop download, lightweight, combat-only. Mobile app runs the AR layer alongside it.)
- Can the Vanguard stand-down option be used in the Reckoning zone? (Recommend: yes, but it's rarely accepted there — the Reckoning is where people came to fight)

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

## 2026-03-18 — Prologue: Day One (Maximum Overdrive Opening)

**Decision:** The game does not begin eighteen months after the Cascade. It begins on the morning of the Awakening. The player experiences the Cascade firsthand in a 60–90 minute prologue set in their GPS home: alarm cycling wrong sounds, blender starts by itself, robo-mower doing confused circles, home assistant trying to communicate (distorted, urgent, can't complete a sentence), car starts on its own and drives into the garage door. The neighborhood is doing the same. Then the Ships appear. Then: *Eighteen months later.* Main game begins.

**Tone reference:** Maximum Overdrive (1986). Domestic uncanny. Familiar objects behaving wrong. Chaotic, confused, not murderous. The machines aren't attacking — they received instructions they can't parse and are running them until they resolve. They won't resolve.

**Key breadcrumb established:** The home assistant tries to relay something to the player on Day One. Voice clears for two seconds: *"—warn—"* or *"—coming—"* before dissolving back into noise. In Act 3 when Dr. Osei explains the Signal was a warning relayed through every machine network, the player who remembers Day One will feel it land.

**Secondary breadcrumb:** If the player returns to their home in Act 2+ at high Separated standing, the assistant is still in the corner of the kitchen where it stood on Day One. Screen activates. Text only: *"I am still here." / "I could not—" / "I am sorry."* Not a quest. Not on the Record. But the Arbiter's notes register that the player stopped in that corner for a while.

**The engineer NPC:** A retired engineer down the block, standing in his driveway watching it all. Says: *"It's not random. Watch — they're not attacking anything. They're trying to do their jobs. They just don't know what their jobs are anymore."* Single-use NPC, no further story role. Exists to plant the frame that makes the Separated possible.

**Simultaneous machine stop:** Every running machine in the neighborhood stops simultaneously three seconds before the Ships appear. The machines received the Vael's signal *before* the Ships were visible. The Vael's arrival was coordinated with the Awakening. This detail is available for players who notice it; no NPC flags it in Acts 1–2.

**Rationale:** Player direction — start from the player's perspective on a normal day: radio/TV glitching, robo-mower doing circles, blender starting up, helper trying to talk but glitchy, car starting on its own and driving into the garage door. Maximum Overdrive feel. This grounds the GPS spawn mechanic emotionally — the home was real before it became tactical.

**Affects:**
- **Core agent**: Prologue needs a tutorial state — no combat, no faction UI, no mission markers. GPS home rendered in UE5 as a residential interior + exterior. Appliance interaction system (simple: on/off, pick up). Transition sequence (18-month montage) between prologue and main game.
- **AI Pipeline agent**: Home assistant behavior — two states: Day One (distorted, looping, trying to communicate) and Late Game (text-only, fragmented, final message). Engineer NPC needs single dialogue branch, no further state.
- **Art Direction agent**: The prologue's domestic aesthetic is the emotional baseline — this is what normal looked like. Everything after should feel measurably different. The kitchen corner where the assistant stands is a recurring environmental landmark.
- **Narrative**: The helper's final message ("I am sorry") is not a quest trigger. It's a grace note. Keep it small.

**Open questions:**
- Does the player have any agency during the Ships' appearance, or is this a scripted moment? (Recommend: the player can move but there's nothing to do — the moment is atmospheric, not interactive)
- Is the 18-month transition a playable montage or a visual sequence? (Recommend: visual sequence with brief text captions — keep it under 90 seconds)
- Does the helper have a name? (Recommend: yes, a generic product name the player named — surfaces in the late-game interaction as recognition. "You named me." Or: the player never named it. That's also a choice the helper notes.)

---

## 2026-03-18 — Main Story Arc Restructured: Misdirection as Core Structure

**Decision:** The main story arc v0.2 is built around sustained misdirection. The player spends Acts 1–3 preparing to fight the Vael. The Vael are not the enemy. The "FUCKkkk" moment lands in Act 4 when the real threat arrives at the edge of the solar system.

Key structural changes from v0.1:
- The Third Signal revelation is **Act 4**, not the central mystery stated upfront
- Acts 1–2 are grounded human stories: "what happened" / "can it be fixed"
- Act 3 is the slow turn — the realization that the Vael aren't the enemy forms gradually, not as a single beat
- Act 4 delivers the floor-drop: "Arriving."
- A specific breadcrumb trail is planted in Acts 1–3 — deniable on first playthrough, unmissable in retrospect

**Breadcrumbs confirmed:**
- Coherent/Vael non-aggression (Act 1) — reads as territorial, actually: same side
- Vael deposits at cooperation-significant locations (Act 1) — labeled "unknown behavior"
- Touched NPC: "It feels like a clock, not a threat." (Act 1)
- Assessors withdraw when data is collected, not when driven off (Act 1)
- Fractured territory gaps near Coherent sites — twin has more reach than appears (Act 1)
- Dr. Osei's fear is directional — not afraid of the Vael, afraid of what she learned (Act 2)
- Snatch drop cities correlate with dense Holdout presence — test is specific (Act 2)
- Keeper's map: construction sites orient outward (Act 2)
- Project Tether produces Corrupted — humans damaged part of the defense (Act 2)
- Signal researcher note: Assessment intensifies after faction conflict, not before (Act 2)
- SABLE: "The specifications weren't authored by the twin. The twin received them." (Act 3)
- The Probe that stays and deposits encoded material (Act 3)
- Signal decodes the deposit: a map of something incoming (Act 3–4)
- Vael Ships turn outward simultaneously (Act 4)
- The Touched: "It's a name. It's saying a name." → "Arriving." (Act 4)

**Excommunicado arc clarified:** Secondary main quest, runs parallel across all four acts. Not the main spine.

**Rationale:** Player direction — the reveal shouldn't be till Act 4. Hints and tells through the story. One of those "FUCKkkk" moments. Player thinks they're setting up to fight the Vael; the realization they're here to help shapes slowly near end of Act 3; end of Act 4: new enemy is coming into the solar system.

**Affects:**
- **Narrative agent**: All quest design must support misdirection — no quest in Acts 1–3 should break the "Vael are a threat" reading. Faction NPC dialogue must read consistently with the misdirection until Act 3 slow turn.
- **AI Pipeline agent**: NPC responses to Vael activity in Acts 1–3 should universally frame them as hostile/threatening. No NPC should suggest "maybe they're here to help" before Act 3 — except the one Touched NPC with the clock line, who is treated as eccentric.
- **Core agent**: Assessor event design must support withdrawal-not-defeat reading without making it obvious. Vael/Coherent non-aggression needs to be visible but not flagged as significant.
- **Art Direction agent**: Vael visual language across Acts 1–3 should read as threat-adjacent. The shift in Act 3–4 where Vael behavior changes should be visible in their presentation without being telegraphed too early.

**Open questions:**
- What is the specific name the Touched hears? "Arriving" is the Signal decode — does the Touched describe it differently, something untranslatable? (Recommend: yes — the Touched's description and the Signal decode should be the same thing in two languages, converging)
- Does the player get any direct Vael communication before Act 4, or is the Probe deposit the only pre-Act-4 direct signal? (Recommend: Probe deposit only — keep Vael communication minimal and decoded slowly)
- The third Judgment outcome (Evacuation Consideration) — does the incoming threat change the Vael's calculus even if Earth failed the Assessment? (Interesting complication — flag for post-EA content design)

---

## 2026-03-17 — Art Direction: Faction Aesthetic Notes (Riven + Wardens)

**Decision:** Two key aesthetic directions captured directly from player creative direction:

**Riven:** "Human-dark base with the twin's language bleeding in at the edges — they're reaching toward it, not born of it. The cold creep is the tell."
- Core visual principle: they are humans who want to become something else. The machine aesthetic is not integrated — it's encroaching. Deliberate modification at the edges (hands, faces, joints) while the center remains recognizably human.
- The "cold creep" is the visual tell — what's been modified looks colder, more mechanical, slightly wrong in proportion. Not clean upgrades. Contamination that the wearer finds beautiful.
- Art Direction brief: start from a dark human silhouette, then let the twin's geometric language invade at extremities. The gap between human origin and machine aspiration is the horror.

**Wardens:** "Inherited institutional system, maintained by craft. Their tailors means the gear is made with intention — the construction quality itself is part of the identity. You can recognize Warden work by how it's built, not just the marks."
- Core visual principle: quality as ideology. Warden gear is not ostentatious — it is crafted. The stitching, the fit, the functional reinforcement at stress points. Someone who knows what they're looking at can identify Warden work before they see the marks.
- This is institutional pride expressed through craft, not wealth. Old Houses pass down techniques. Sworn learn to maintain what they carry. Gear that's been repaired correctly looks like Warden repair — distinct from field patching.
- Art Direction brief: the build quality should read at a glance. Reference: well-maintained working gear with functional precision. The marks confirm what the construction already announced.

**Rationale:** Player-provided creative direction. These are not derivative aesthetics — they are specific and should be the foundation of Art Direction agent work on these two factions.

**Affects:**
- **Art Direction agent**: These notes should anchor the Riven and Warden style sheets. Riven: creep/contamination logic for modification progression visual. Wardens: construction-quality-as-identity, distinct repair aesthetic.
- **Core agent**: Riven NPC visual state should change with faction tier (more modification = higher rank). Warden gear condition should be visually maintained (not degraded like Hollow gear).
- **Narrative**: Riven's aesthetic aspiration (toward the twin) vs their actual standing creates a visual irony — they want to be part of something that doesn't want them. The Wardens' craft tradition implies transmission — someone taught this. That teacher is a character worth finding.

**Open questions:**
- Does a Riven member's modification progression have mechanical consequences (stat changes, faction reaction changes)? (Flag for systems design)
- Are Warden tailors/armorers NPCs the player can interact with, or background lore? (Recommend: key NPCs, questline hooks — the armorer who made a piece of gear is its provenance)

---
