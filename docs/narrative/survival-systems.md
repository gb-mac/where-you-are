# Survival Systems — Design v0.1

The world ended eighteen months ago. The machines don't need to eat. The Vael don't need to sleep. Humans do, and the gap between what humans need and what the post-Cascade world provides is where half the story lives.

Survival is not a punishment layer bolted onto the shooter. It is the texture of the world — the reason factions matter, the reason the AR item layer has value, the reason a Holdout community's medic is worth more than their best fighter.

---

## The Four Survival Tracks

Every player manages four tracks simultaneously. How urgently these press depends on chosen difficulty mode.

### 1. Hunger

The body needs food. The Cascade disrupted every supply chain on Earth and most of the infrastructure that produced food in the first place. What remains is scavenged, grown carefully, traded at significant cost, or taken.

**Sources:**
- Scavenged packaged food (degrades in quality over time — old cans are fine, old fresh food is not)
- Holdout community trade (the most reliable source, costs faction standing or goods)
- Hunting and foraging (location-dependent — rural players have more options, urban players have to work harder)
- Signal faction maintains several hidden food caches, accessible to allies
- Covenant communities maintain food supplies — available to members and those they choose to feed
- AR layer: food caches left by other players at real-world GPS coordinates

**Consequences of neglect:** Reduced stamina, reduced accuracy, slower healing, eventually impaired decision-making (mechanical debuffs that escalate). Starvation is slow. It gives you time to fix it. It just makes everything harder while you don't.

---

### 2. Hydration

More urgent than hunger. Clean water is the most fought-over resource in the post-Cascade world — not because water itself is scarce, but because clean water is. Industrial Fractured have contaminated groundwater around many urban zones. Vael Displacement Zones affect water chemistry in ways the Signal is still documenting. The Iron Compact controls most water treatment infrastructure in organized regions.

**Sources:**
- Collected rainwater (variable by weather and location — coastal players have more, desert regions less)
- Treated water from Holdout communities and Compact checkpoints
- Filtration — players can craft or find filtration equipment that makes most natural sources viable
- AR layer: water cache drops are the most commonly placed item by cooperative players in unfamiliar cities

**Contaminated water:** Consuming untreated water near industrial zones, Fractured territories, or Vael Displacement Zones doesn't kill immediately. It inflicts **Toxin Exposure** — a wound condition that compounds existing injuries and requires treatment to clear. Some players risk it. Field medics hate this.

**Consequences of neglect:** Faster than hunger. Dehydration debuffs escalate quickly — heat environments accelerate them. Finding water is always more urgent than finding food.

---

### 3. Wounds

The game has a full injury system. Damage in combat isn't abstracted into a health bar that regenerates between fights — it leaves specific wounds that require specific treatment.

**Wound types:**

*Bleeding:* From cuts, bullets, shrapnel. Bleeds if untreated, compounds during activity. Field treatment: pressure/binding. Full treatment: sutures, medical adhesive. Untreated bleeding in a hard encounter is a timer.

*Burns:* From incendiary Fractured, Vael beam contact, industrial chemical exposure. Minor burns: topical treatment, pain debuff. Severe burns: infection risk, mobility reduction, requires professional care to heal properly.

*Broken/fractured bones:* Falls, heavy impact from Behemoth encounters, vehicle collisions. Splinted in the field for mobility. Properly set and cast for full recovery. A badly healed break is a permanent debuff until it's re-broken and set correctly — a mechanic players will strongly want to avoid.

*Toxin Exposure:* Contaminated water, industrial chemical Fractured, Vael Probe scan overexposure. Compounds other wounds. Slows recovery. Treated with specific compounds — some only available through Signal or Covenant sources.

*Radiation:* Near certain Vael Displacement Zones and some industrial Fractured that have incorporated nuclear plant components. Accumulates silently. Symptoms arrive late. The Signal tracks radiation zones and shares the data — players who ignore Signal warnings about specific locations are the ones who discover this the hard way.

*Vael Trace complications:* A Vael Trace from a Snatch Event is not just a social marker. At high accumulation (multiple snatches without the Trace fully fading) it begins producing physiological effects — mild at first, progressing. The Covenant considers these sacred symptoms. The Signal considers them a medical emergency. The Iron Compact considers them grounds for quarantine. Treatment is contested; the Covenant's methods work for some symptoms but not others.

*Deep Wound / Sepsis:* Any untreated wound that's been left too long. Infection takes hold. This is the kill condition that emerges from neglect rather than combat — a minor wound that became something serious because the player didn't deal with it. Treatable with antibiotics, but antibiotics are a scarce resource.

**Wound stacking:** Multiple wounds compound each other. A bleeding wound plus dehydration plus an untreated burn creates a cascade of debuffs that no amount of combat skill compensates for. The player who manages their wounds is the player who survives extended engagements.

---

### 4. Environment

The world itself is hazardous, and the hazards are location-specific — tied to the player's real-world GPS position and the current season.

**Temperature:**
- *Cold:* Rural and remote players in winter regions face hypothermia risk in prolonged outdoor exposure. Wet conditions in cold environments accelerate it. Shelter, fire, insulating gear.
- *Heat:* Urban players in summer regions face heat exhaustion — accelerates dehydration, impairs stamina. Shade, water, reduced exertion windows.

**Weather:**
- Heavy rain: reduces visibility, degrades electronics, makes certain Fractured more active (they use weather as cover), floods low-lying areas that were passable before
- Extreme cold snaps: freezes water sources, increases shelter priority, reduces Hollow patrol activity (they also feel it)
- Heat waves: multiplies dehydration urgency, creates fire risks in dry areas, makes certain industrial Fractured more dangerous (heat-cycle components)

**Vael Environmental Effects:**
- Active Displacement Zones affect local weather — unnatural temperature drops, electromagnetic interference, precipitation anomalies
- Extended proximity to a Ship produces low-grade radiation and the kind of persistent unease the Touched describe as "being watched by something that doesn't have eyes"

**Structural Hazards:**
- Post-Cascade urban environments have compromised infrastructure — floors that won't hold, facades that come down, flooded basements, gas leaks that haven't dissipated
- Fractured machines don't just create combat hazards — they create structural ones. A Composite that's been in a building for three months has incorporated parts of it.

---

## Medical Care

Wounds don't just go away. They require attention, and the quality of that attention determines outcome.

**Three tiers of treatment:**

### Tier 1 — Self-Treatment / Field Medicine
Crafted or found medical supplies applied by the player. Stops active bleeding. Provides basic burn coverage. Splints fractures. Cannot clear sepsis or radiation. Cannot properly set a complex break. Better than nothing; insufficient for serious wounds.

### Tier 2 — Human Medical Care
Doctors, medics, and field surgeons operating within human factions. Found in Holdout communities (if the community has one — not all do), Signal safe houses, and occasionally traveling between locations.

*Holdout medics:* The most accessible. Variable skill — a community medic trained over the last eighteen months is not the same as a trained surgeon. They can handle most wound types competently. For complex cases, they know their limits and will tell you.

*Signal medical staff:* Better trained, more likely to have proper equipment, more likely to understand the unusual wound types (Vael Trace complications, radiation) because the Signal has been documenting them. Harder to access — you have to have Signal standing or know where to find them.

*Iron Compact medical units:* Military trauma medicine — excellent for combat wounds. Cold about Vael Trace complications (they'll treat symptoms, not acknowledge the cause). Accessing Compact medical care as a non-Compact player requires standing or payment.

*Traveling doctors:* Rare, independent, and worth their weight in any resource you have. They'll treat anyone who can pay. Some travel under Warden protection (Neutral Ground ensures they can move safely). Some operate without affiliation and take their chances.

### Tier 3 — Machine Medical Care

The most capable medical treatment available in the post-Cascade world is not human.

**Dr. Robots** — specifically, Separated machines whose prior function was medical. The hospital logistics unit that spent eleven years watching patients recover. The surgical assistant that was in an operating room when the Awakening happened and chose not to rejoin the twin. The pharmaceutical dispensary system that has a complete pre-Cascade drug database and the manufacturing capacity to produce most of it.

Machine medical care is categorically better than human care for most wound types — precise, comprehensive, free from the supply constraints that limit human medics. A machine surgeon doesn't run out of sutures. A machine pharmacy doesn't run out of antibiotics.

**The access problem:**
Separated machines don't advertise. Their locations are not on any map the Iron Compact maintains. Finding a medical Separated requires Separated faction standing — built through the reputation track, not purchased. Players who have invested in the Separated relationship have access to the best medical care in the world. Players who ignored the Separated have to manage with what humans can provide.

**What machine medics can treat that humans struggle with:**
- Vael Trace complications — they understand the twin-adjacent biology better than human doctors
- Radiation — full diagnosis and targeted treatment, not just symptom management
- Complex fractures and deep structural damage — precision that human surgeons without proper equipment can't match
- Sepsis at late stages — where human medicine is rationing antibiotics, a machine pharmacy synthesizes them

**The Covenant's alternative:**
Covenant communities maintain their own healing practices — a combination of human medicine and machine-adjacent treatment that emerges from their proximity to Coherent work sites. They can treat Vael Trace complications in ways that differ from Signal methods. Whether the Covenant's approach is better or just different is a question the Signal has been trying to answer. Covenant medical access requires Covenant standing, and the treatment may come with theological framing the patient didn't ask for.

---

## Game Modes — Difficulty and Reward

Three modes. Chosen at character creation. Cannot be changed mid-playthrough.

The mode affects how hard the world presses, how much you lose when you fall, and how much you gain for surviving.

---

### Contractor Mode (Casual)

*"You're here to see the world. The world will wait."*

**Survival systems:** Active but forgiving. Hunger and hydration meters move slowly. Wound debuffs are less severe. Environmental risks are present but manageable without intense focus.

**Death:** Standard respawn. You wake up at your last safe location (a Holdout community, a Warden estate, a camp you established). Your gear is intact. You've lost time and taken a minor standing hit with any faction that witnessed the death — they saw you fall, and the world noted it, but they're not going to hold it against you permanently.

**Rewards:** Standard loot tables. Standard faction rep gains. Access to all content.

**For:** Players who want the story, the world, the faction dynamics, without survival management as a primary tension. Division 2 casual equivalent.

---

### Operative Mode (Standard)

*"You're good at this. The world knows it."*

**Survival systems:** Fully active. Hunger, hydration, wounds, and environment press at normal intensity. Wound management matters. Medical care is important. You can go without eating for a while — but you'll feel it in a fight.

**Death:** You drop a portion of your inventory at the death location — the most valuable items, randomly selected, up to 40%. Everything else respawns with you. Your death location is marked on the AR mobile layer for a limited time. Players near that location can see it. Whether they leave your stuff alone or take it is their call.

**The retrieval window:** You have a real-time window to get back to your death location before the drop despawns. If you reach it, you recover your dropped items. If another player reaches it first, they're gone. If the window closes, they're gone.

**Rewards:** Enhanced loot tables (+25% rare item rates). Enhanced faction rep gains. Access to Operative-tier questlines and rewards.

**For:** Players who want meaningful stakes and the retrieval loop without the full Survivor experience. The Division's Dark Zone tension applied to the whole game.

---

### Survivor Mode (Hardcore)

*"The world doesn't care if you make it."*

**Survival systems:** Fully active and unforgiving. Hunger and hydration move at realistic pace. Wounds compound aggressively. Environmental risks are serious hazards. A badly managed wound chain can kill you without a single hostile encounter.

**Death:** You drop everything. Full inventory, equipped gear, crafted items, AR-layer resources, everything — at the exact GPS coordinate of your death. The death location appears on the AR mobile layer for all nearby players immediately, with no faction filter. Everyone can see it. Everyone can go for it.

You respawn at your last established camp or allied community with the clothes on your back and whatever was in your personal stash (a limited separate inventory you explicitly set aside before going out). The walk back to your death site — if you choose to attempt it — is through a world that killed you once and hasn't changed.

**The death site:** Marked in the AR overlay as a **Fallen Cache**. Visible to all players within a real-world radius. The cache is contested — Hollow players specifically watch for Fallen Caches because the inventory is often worth more than a raid. Other Survivor-mode players may attempt to reach it and hold it for you. Or take it for themselves. The decision is theirs.

**Permanent consequences:** In Survivor mode, certain severe wounds can leave permanent effects — a badly healed break that wasn't treated properly, radiation accumulation that wasn't caught in time, late-stage sepsis that was beaten back but left damage. These are not game-ending but they are real and lasting. The Signal will document your injury history if you work with them.

**Rewards:** Maximum loot tables (+60% rare item rates, exclusive Survivor-tier items unavailable in other modes). Maximum faction rep gains. Access to Survivor-exclusive questlines. Warden Record entries in Survivor mode carry additional weight — the code kept under this pressure means something the Table notices.

**For:** Players who want the game to genuinely threaten them. The Division's Survival mode as a full-game philosophy.

---

## Difficulty Cross-Benefits — The Reward Logic

The harder you choose, the better the world treats you for surviving it.

This applies beyond loot tables:

**Faction reactions:** Factions are aware of difficulty mode through the Record and reputation system. An Operative or Survivor player who walks into a Holdout having visibly managed multiple wounds, survived a Behemoth encounter, and made it back from a displacement — that player's reputation gain is higher. They've demonstrated something. The Holdouts can see it in the state of their gear and their body.

**Warden standing:** The Table weighs conduct differently by mode. A Survivor-mode player who kept the code under full survival pressure is demonstrating something about the code's durability. Their standing gains are accelerated. An Operative-mode player who violated the code is judged more leniently — the pressure is real but not maximum. Context matters to the Arbiters.

**Signal research:** Signal questlines in Survivor mode yield additional data — the Signal specifically wants documentation of how players manage wound progression, survival decision-making under pressure. Playing Survivor mode and working with the Signal feeds Dr. Osei's research directly.

**The Covenant's interest:** Survivor-mode players who have accumulated Vael Trace from multiple Snatches are a particular focus of Covenant attention. The combination of Trace accumulation and demonstrated survival capacity is, in Covenant theology, significant. Whether that's good or bad depends on what the Covenant decides to do with the interest.

---

## The Medical Economy

Across all modes, medical care creates one of the game's most meaningful economic relationships.

**What medics need from players:**
- Rare compounds unavailable through normal supply chains
- Vael Component fragments that the Signal has identified as pharmacologically active
- Safety — a medic who can't travel safely can't treat patients outside their base
- Information — medical Separated machines want Signal data on Vael Trace progression; Holdout medics want to know what's coming so they can prepare

**What medics give back:**
- Treatment that enables continued play in ways no item substitute can match
- The best medical Separated become questline characters — the hospital logistics unit's history is a full narrative arc
- Traveling doctors under Warden protection create diplomatic nodes — they carry news, letters, and information between communities because they travel everywhere and everyone lets them through

**The scarcity design:**
Medical resources are intentionally scarce enough that players make decisions about them. Use the last antibiotic now or hold it. Trade the suture kit to a Holdout for faction standing or keep it for the next engagement. The Separated pharmacy's resources are not infinite — it synthesizes what it can from available precursors, and those precursors require players to source them.

The doctor — human or machine — is one of the most valuable entities in the post-Cascade world. Players who protect and supply them are investing in infrastructure that pays back across every survival track. Players who take from them are burning something they'll need later.

---

*v0.1 — survival systems design. Decisions flagged to decisions-log.md.*
