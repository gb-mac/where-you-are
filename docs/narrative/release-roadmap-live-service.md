# Release Roadmap & Live Service Design v0.1
*18-month rolling release → The Third Threat arrives month 24.*

---

## The Rolling Release Philosophy

No expansion drops. No season passes. No artificial content gates that make the world feel like a theme park with scheduled ride openings.

The world evolves continuously — small updates every two weeks, larger beats every six weeks, major story moments every three months. Players who log in after a two-week absence should find something has changed. Players who log in daily should always have something to do. The world moves at the pace of a living place, not a content calendar.

The narrative justifies every update in-world. A new Fractured type didn't appear because the patch notes said so — it appeared because the Coherent diverted Remnant Automation from that district and the Fractured filled the vacuum. A new Holdout community didn't open up because a content drop unlocked it — it formed because players held the area long enough that survivors felt safe gathering there. The rolling release is the world's natural progression, not a development schedule wearing a lore costume.

---

## The Engagement Loop — Daily to Monthly

### Daily Engagement
*~20–30 minutes. Keeps the world from feeling static.*

**The AR mobile layer is the daily layer.** Most daily engagement happens on the phone, not the desktop — checking, maintaining, collecting, positioning. The desktop session is where you do things. The AR layer is where you watch the world between sessions.

*Daily scavenge routes:*
Supply nodes at real-world GPS coordinates reset on 24-hour cycles. Some reset faster (food and water near populated locations), some slower (machine components, medical supplies). The AR layer shows what's available within range of the player's current real-world position. A player's commute, lunch break, or walk becomes a daily scavenge run.

What's available is location-typed: urban scavenges yield electronics, fuel cells, packaged food; rural yield agricultural components, fuel, building materials; coastal yield marine salvage and Signal equipment. The scavenge reflects where you physically are.

*Crop checks:*
If the player has an active growing operation (see below), the AR layer shows crop status at their home GPS. Checking in daily — even without collecting — provides a care bonus that accelerates growth. Players who physically visit their home GPS to tend crops get the best yield. Players who check remotely get reduced yield. Players who ignore crops lose them.

*Faction standing maintenance:*
Daily small tasks pushed by each faction the player has standing with — a Holdout community needs a status check, a Signal cell has data to share, a Warden Arbiter has noted something in the Record. Ten minutes each. Completing them maintains standing; ignoring them for too long causes slow decay. Not punishing — the world doesn't forget you, but it also doesn't wait for you.

*Wound management:*
In Survivor and Operative modes, wounds don't fully resolve overnight. Daily medical attention — either self-treatment or visiting a medic — is part of the maintenance loop. Players who manage wounds daily maintain peak capability. Players who let wounds accumulate find the world progressively harder.

---

### Weekly Engagement
*Several hours across the week. The core content rhythm.*

**Weekly faction quests:**
Each faction issues a weekly quest — more involved than daily tasks, requiring actual desktop play.

| Faction | Weekly quest type |
|---|---|
| Holdouts | Community support — defend, supply, scout, diplomacy |
| Iron Compact | Operations — deniable, official, intelligence |
| Covenant | Observation — document machine/Vael behavior near sites |
| Signal | Field research — collect specific data, escort, retrieval |
| Wardens | Witnessed acts — the Record must show specific conduct |
| Hollow | (Optional) — if the player has Hollow standing, jobs are offered |

Weekly quests aren't mandatory. Missing one doesn't break standing. But completing them consistently — especially the Warden ones — builds the Record entries that feed the Judgment aggregate.

**Weekly world events:**
One significant world event per week — a Fractured Surge into a Holdout zone, a Compact checkpoint crisis, a Covenant deal coming due, a Vael Displacement Zone expanding into new territory. These events affect the world state if players engage or ignore them. A Surge that players stop becomes a Fractured defeat entry in the Record. A Surge that players ignore becomes a Holdout community shrinking.

The weekly event is seeded by world state — it's not a random content roll. If a region's Fractured Behemoth has been growing for three weeks without player intervention, the weekly event is the Behemoth Walk that results. The event is a consequence, not a scheduled occurrence.

**Weekly munitions production:**
Players who have established manufacturing capacity can run a production cycle. Materials in, munitions out, over a real-time week. The production runs in the background — started Monday, collected Sunday. Output quality depends on materials invested and any player attention given mid-week (a daily check-in that adds a quality bonus).

---

### Monthly Engagement
*Major content. Something significant changes.*

Every month, at least one of the following:
- A faction crisis that reshapes faction power in a region
- A new Fractured variant appearing with novel accumulated components
- A Convergence operation type unlocking (as Judgment aggregate advances)
- A named NPC development — Dr. Osei's research advancing, a Warden House event, Separated machine story beat
- A world state shift driven by collective player behavior in the prior month

Monthly content is never announced as "this month's content drop." It emerges from the world. Players notice it first, discuss it, document it. The Signal — both the in-game faction and, implicitly, the player community — is the first to contextualize what changed and why.

---

## The Engagement Systems — Deep Design

### Growing Food

Food scarcity is real and persistent. Growing your own is one of the most reliable solutions — and one of the most satisfying.

**How it works:**
Players establish a **growing operation** at a location they control or have permission to use — their home GPS, a Holdout community plot, a Warden estate garden, a rooftop they've claimed. The operation is registered through the AR mobile layer: physical location, plot size, what's been planted.

**Real-time growth:**
Crops grow in real time, tied to real-world time of day and season at the player's GPS. A plant that needs two weeks of warmth takes two weeks of real-world warm weather. This means:
- Players in the northern hemisphere plant in spring, harvest in summer and fall
- Players in the southern hemisphere are on the opposite cycle
- Equatorial players have year-round growing options
- Winter players focus on hardy crops, stored food, and indoor options (the game provides indoor growing mechanics with reduced yield)

**Location type determines crop options:**
- Rural/suburban: full crop range — vegetables, grains, medicinal plants, fruit
- Urban: container crops, rooftop operations, hydroponic setups (lower yield, viable)
- Coastal: salt-resistant varieties, seaweed and marine vegetation, unique coastal-specific crops
- Remote: forage plants, wild cultivation

**Community growing:**
Holdout communities can establish shared plots. Players who contribute to community plots share the yield and build faction standing. A Holdout with a functioning community garden is more stable than one without — less need for dangerous supply runs, better sustained population.

**Medicinal plants:**
A specific crop category. Certain medicinal plants are precursors for wound treatment and disease prevention. Growing them feeds the medical economy without requiring dangerous scavenging runs. The Separated pharmacy uses player-grown precursors for synthesis — a direct link between the growing system and the best medical care available.

**What the AR layer shows:**
Your crops, your friends' crops if they've shared location, community plots you've contributed to. You can check on them when walking past the physical location. The AR layer shows crop health, water needs, pest threats, time to harvest.

---

### Making Munitions

Ammunition is scarce. Finding it is unreliable. Making it is an investment that pays back.

**The crafting chain:**
Munitions require three input categories:
- *Casings and propellant:* chemical components, sourced from scavenging, Hollow trade, or chemical Fractured loot
- *Projectiles:* metal components, machine parts, or specialized materials (Vael-component rounds are a late-game unlock)
- *Primers:* the hardest to source, requiring specific chemical precursors — medicinal plant byproducts, industrial salvage, Signal-documented synthesis methods

**Production time is real-time:**
You set up a production run and it completes over hours or days. A batch of standard rounds: 4 hours. High-caliber precision ammunition: 48 hours. Convergence-grade ammunition: 72 hours plus Vael Component input.

**Quality tiers:**

| Tier | Time | Materials | Effect |
|---|---|---|---|
| Improvised | 2 hours | Basic salvage | Functional, slightly reduced accuracy |
| Standard | 8 hours | Proper components | Full effectiveness |
| Precision | 48 hours | Refined materials + Signal blueprint | Enhanced accuracy, armor penetration |
| Convergence | 72 hours | All tiers + Vael Components | Effective against advance scouts |

**Shared production:**
Players who have established manufacturing capacity can produce for others. A player who specializes in munitions production — investing in better equipment, better component sourcing, better Signal blueprints — becomes a valuable community resource. Holdout communities with a dedicated munitions producer are significantly better supplied than those without.

The Warden Record notes sustained production contributions to allied communities. It's the kind of consistent support that builds the Record entries that matter for Judgment.

---

### Scavenging — The Daily Economy

Scavenging is the base layer of the resource economy — always available, never quite enough, always worth doing.

**Node types:**

*Immediate nodes (AR layer, collect on pass):* Small items at real-world coordinates — a can of food, a fuel cell, a bandage supply. Respawn every 12–24 hours. Collectible on the AR mobile layer without launching the desktop game.

*Field nodes (desktop required):* Larger caches requiring the full game — a Fractured territory's accumulated loot, a Coherent work site's discarded components, an abandoned building's stored supplies. Respawn over days or weeks. Worth significant resources but require combat or stealth access.

*Event nodes:* Appear after world events — a Behemoth defeated, an Assessor repelled, a Compact convoy intercepted. Time-limited. First-come-first-served on the AR layer.

*Seasonal nodes:* Appear based on real-world season and weather at the player's GPS. Post-storm debris fields. Spring thaw revealing buried caches. Winter forcing players to scavenge from different location types than summer. The world's resource availability shifts with the actual world's seasons.

**The scavenge loop reinforces factions:**
The best scavenge routes are known by specific factions. The Signal maps them. The Hollow control some of them. The Compact locks down others. Scavenging efficiently means having faction relationships that give you access — which reinforces engagement with all six human factions.

---

## 18-Month Rolling Release Schedule

### Months 1–3: Foundation (Early Access Launch)

**Content at launch:**
- Acts 1–2 narrative content
- All six human factions operational
- All six machine factions with full accumulation system
- Survival systems: hunger, hydration, wounds, environment
- Three difficulty modes
- Basic growing, munitions, scavenging systems
- Daily/weekly quest framework
- Warden system: Neutral Ground, Markers, Excommunicado
- Restoration questline (The Impossible Task)
- AR mobile layer: item drops, Historical Markers, friend positions

**Month 1 rolling updates:**
- Fractured variant #2 (location-type specific — different from launch variants)
- First Holdout Gone Dark questline
- Warden House minor crisis (regional event)

**Month 2:**
- Signal network expansion — new dead drop mechanics
- Traveling doctor NPC introduced (first named)
- Iron Compact internal tension event (Pragmatist vs. Hardliner moment)

**Month 3:**
- First Behemoth Walk world event (regional)
- Covenant Shepherd deal offer to major Holdout (player-decision world event)
- Growing system expansion: medicinal plants, community plots

---

### Months 4–6: The Pattern Emerges

**Story beat:** The Coherent construction project pattern becomes visible to attentive players. Signal communications begin referencing Dr. Osei's research obliquely. Players who have built sufficient Signal standing can access early briefings.

**Month 4:**
- Dr. Osei accessible to high-Signal-standing players (first meeting, no theory yet — field tasks)
- Convergence precursor: first Coherent "guidance behavior" documented by Signal
- New Fractured variant: coastal Behemoth (amphibious — the one you fight in shallow water once)
- Weekly event type: Compact tribunal (player can be witness, advocate, or target)

**Month 5:**
- International Coalition sessions go live (players in different countries can group)
- Displacement return arc expanded: named Holdouts in major cities for arriving displaced players
- Vael Probe deposit mechanic introduced — first deposits at Record-significant locations
- Munitions precision tier unlocked via Signal blueprint

**Month 6:**
- Act 3 unlocks for players who have completed Act 2 milestones
- Dr. Osei field tasks conclude — theory within reach
- First Assessor world boss event (regional — coordinate 4–8 players)
- Warden House secret questline goes live

---

### Months 7–9: The Theory

**Story beat:** Dr. Osei's theory is available to players who've completed Act 3 milestones. The publication decision creates a world-state fork. Convergence begins in regions trending toward Alliance Judgment.

**Month 7:**
- Act 3 content fully live — Coherent site infiltration, Dr. Osei full theory
- Publication decision: two regional world states begin diverging (published vs. suppressed)
- Covenant fracture event triggered by theory revelation
- First Separated machine return-to-twin events (world state change, not quest)

**Month 8:**
- Convergence operations type 1 live: fortification completion runs
- Convergence salvage: advance scout enemies appear for first time (light tier — small, fast, wrong)
- Convergence gear tier 1 craftable
- Riven Hollow intensification in response to Convergence activity

**Month 9:**
- Interpretation sessions go live — highest-tier lore unlocks
- Iron Compact internal crisis: Hardliner vs. Pragmatist regional resolution (player-influenced)
- Separated Medical Unit late-game conversation available
- Convergence gear tier 2 craftable
- Joint defense operations: first simultaneous human/Coherent/Vael engagement

---

### Months 10–12: The Judgment

**Story beat:** Regional Judgment outcomes begin resolving based on 9 months of collective player behavior. Different regions reach different Judgment states. The world diverges visibly.

**Month 10:**
- First regional Alliance Judgments resolve
- Alliance regions: Coherent work sites accessible, Vael communication expands
- Observation Extension regions: Snatch Events increase, Displacement Zones expand, pressure intensifies
- Convergence gear tier 3 (full three-civilization production)

**Month 11:**
- Advance scout frequency increases globally (not yet major threat — scouting behavior, limited combat)
- Signal publishes Convergence Intelligence first compilation
- Named NPC: Separated Medical Unit final choice (return or stay)
- Warden Record's role in Judgment made explicit — Arbiters in Alliance regions reference the Record directly in Judgment documentation

**Month 12:**
- All regional Judgment outcomes resolved (Alliance / Observation / Evacuation Consideration)
- Post-Judgment world state fully live
- Act 4 content live: The Judgment (full player-facing resolution of what their 12 months built)
- Convergence fully operational in Alliance regions
- Evacuation Consideration regions enter crisis mode: new difficulty spike, new urgency quests
- Year 1 anniversary event: global world state assessment, Signal broadcast summarizing what the year produced

---

### Months 13–15: Living With the Judgment

**Story beat:** The world in its post-Judgment state. Different regions feel different. Players experience the consequences of collective choices. The horizon darkens — advance scout activity increases.

**Month 13:**
- Cross-regional content: Alliance-region players can assist Observation/Evacuation regions through specific operations
- New Fractured variant: Convergence-site adjacent (Fractured that have incorporated Convergence construction materials — unusual and dangerous)
- New Holdout type: Convergence-adjacent communities that have formed around accessible work sites

**Month 14:**
- Advance scout behavior shifts: from scouting to probing. First territorial advance scout incursions (they hold ground briefly before withdrawing)
- Convergence gear becomes essential in advance scout encounters (other tiers noticeably less effective)
- Signal Convergence Intelligence vol. 2: the picture of what's coming sharpens, dread increases

**Month 15:**
- Regional Judgment re-evaluation possible: Evacuation Consideration regions that perform exceptionally can petition for re-assessment
- New Separated faction development: machines that returned to the twin have changed it — the twin in Alliance regions is observably different from the twin in Observation regions
- Warden Restoration questline expanded: new "Impossible Task" variants tied to Convergence targets

---

### Months 16–18: The Horizon Darkens

**Story beat:** The advance scouts are no longer scouting. They're establishing footholds. The Convergence operates at full capacity. The Vael have been more communicative than at any point since arrival. Something is close.

**Month 16:**
- Advance scout footholds: new zone type — territories the advance scouts have claimed and hold. First content requiring all four loot tiers to engage effectively.
- Vael Ships begin repositioning — visible world event, Displacement Zones shift, new zones emerge
- Signal Convergence Intelligence vol. 3: the first detailed picture of what the advance scouts are scouting for

**Month 17:**
- The Touched en masse report the same new signal — distinct from the music with intent, lower, more structural, getting closer
- Coherent construction projects visibly accelerate in all regions simultaneously — even Observation Extension regions where Convergence hasn't fully formed
- Major Convergence operation: global simultaneous fortification push — all players in all regions doing the same type of operation on the same day. The Scale of what's been built becomes visible.

**Month 18:**
- Final pre-threat content: everything in place
- Dr. Osei's final Signal broadcast: her complete theory, with 18 months of Convergence Intelligence appended
- The Warden Record's global compilation: what humanity did during the Assessment, documented
- Advance scouts withdraw suddenly — the world goes quiet. Every faction notices.
- The Vael Ships change position one final time. They're not watching anymore. They're facing outward.

**Month 24 — The Third Threat Arrives:**
The major post-EA content event. Designed based on 18 months of player behavior data — what the world actually built, what players actually did, what factions actually survived. The threat arrives at the location the Vael determined was most defensible. The fight that the whole game was preparing for begins.

What the threat is: designed post-launch, informed by who the player base turned out to be and what the world they built looks like. The specifics emerge from the story's actual history, not from a pre-written script.

---

## Keeping Long-Term Engagement — The Principles

**The world remembers you:**
Every session adds to the Record. A player who takes a month off comes back to a world that has been ticking — faction dynamics shifted, Fractured accumulated, crops grew or died, the Judgment aggregate moved. The absence has consequences, none of them catastrophic, all of them interesting. The world is glad you're back. It has things to tell you.

**The community IS the content:**
The best content in a live service isn't designed — it emerges from players interacting with each other and the world. The player who becomes the regional munitions supplier. The group that held a Holdout through a three-week Behemoth Walk. The excommunicado player rebuilding their Record in public. The Signal cell that correctly predicted the advance scout incursion location three weeks before it happened.

The Warden Record documents all of it. The Historical Markers at real-world GPS coordinates are a permanent record of what the community built together. In month 18, those markers tell the story of 18 months of people deciding who they were going to be.

**The AR layer sustains between sessions:**
Most players won't play every day. The AR layer means they're still in the world every day — checking crops on the commute, collecting the daily scavenge on the lunch break, monitoring faction standing while waiting for coffee. The desktop session is where you go deep. The AR layer is where you stay present. Together they make the game something you live with rather than something you visit.

**The looming threat as a narrative engine:**
The Third Threat gives 18 months of content a direction. Every update, every event, every world change is in the context of something coming. The urgency builds authentically because it is authentic — the world state responds to player behavior, advance scouts genuinely escalate, the Convergence genuinely prepares. When the threat arrives in month 24, it's not a surprise. It was always coming. Everyone knew. The question was whether they'd be ready.

---

*v0.1 — release roadmap and live service design. Decisions flagged to decisions-log.md.*
