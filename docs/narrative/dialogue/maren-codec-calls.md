# Maren Codec Calls — DataTable Spec
*Implementation-ready content for `DT_CodecCalls`, `DT_CodecLines`.*
*Maren starts with the conclusion. Evidence available on request.*

---

## Call Roster

| CallID | Trigger Event | MinAct | Background | Meshtastic | OneShot | Priority |
|---|---|---|---|---|---|---|
| `morgan_first_assessment` | `WYA.Event.Reckoners.EntryComplete` | Act2 | All | false | true | 70 |
| `morgan_mutual_aid` | `WYA.Event.MutualAid.OperationAvailable` | Act2 | All | false | false | 55 |
| `morgan_narrative_warning` | `WYA.Event.Faction.PlayerFramed` | Act2 | All | true | false | 65 |
| `morgan_community_read` | `WYA.Event.Reckoners.MorganThresholdMet` | Act2 | All | false | true | 80 |
| `morgan_broadcast_fragment` | `WYA.Event.Maren.BroadcastActive` | Act1 | All | true | false | 30 |
| `morgan_act4` | `WYA.Event.Story.TheoryDelivered` | Act4 | All | false | true | 75 |

**Notes:**
- `morgan_first_assessment` fires after entry complete — Cade and Aldric have already called. Maren is the third contact, not the first. She's been watching since before the hazing.
- `morgan_mutual_aid` is repeatable — fires when a community operation needs a player with the player's specific operational profile. Not a general task board — Maren calls when she specifically needs you.
- `morgan_narrative_warning` fires when any faction produces an analysis or report that misframes the player or the Reckoner network. Repeatable, once per framing event. Maren has been tracking this already.
- `morgan_community_read` fires when Maren decides the player passes her threshold — they're fighting for something, not extracting from it. Unlocks mutual aid network access.
- `morgan_broadcast_fragment` is a passive ambient call — the player picks up Maren's channel mid-content. Low priority (30), fires in quiet moments. Repeatable, draw from pool. No codec UI — plays through the assistant's audio system in the Quiet register, same delivery as Aldric's broadcast. Fires from Act 1 onward, before the player formally joins the Reckoners. The broadcast doesn't require membership.
- `morgan_act4` fires after theory delivery. Maren is mid-operation. She will talk while she works.

---

## Call Scripts

---

### `morgan_first_assessment` — Act 2 — ONE-SHOT
*Fires after Reckoner entry complete. Maren has been watching since before the hazing.*
*She starts with the conclusion.*

**mfa_L1:**
*"Maren. I run the mutual aid network and the community operations. You've probably heard about that already — Aldric covers everything eventually."*

*(Beat — no warmup, straight to it)*

*"I've been watching how you move since before Briggs ran the standard on you. I do that with everyone. I want to know what someone is actually after before they're officially in."*

*(Beat)*

*"Here's what I've got on you so far: you're not extracting. You make decisions that cost you and don't advertise it. That's the tell I look for. The people who are here to take something from the network announce themselves without knowing it — they optimize for visibility on the wins and disappear on the costs. You don't do that."*

*(Beat)*

*"That's why I'm calling. Not because you passed Briggs's standard. Because of what I saw before you got there."*

**Branches:**
- *"What do you need from me?"* → `mfa_L2_need`
- *"What happens next?"* → `mfa_L2_next`
- *"[Say nothing]"* → `mfa_L2_nothing`

---

**mfa_L2_need:**
*"Right now? Nothing specific. I want you to know the mutual aid network exists and that you have access to it. It doesn't run through faction systems. It runs through trust — which means it has resources that faction economies can't touch."*

*(Beat)*

*"I'll call when I need something specific. And I will — I call everyone who passes the read eventually. The network has gaps and I fill them with people."*

→ END

---

**mfa_L2_next:**
*"You'll hear from me when a community operation needs your specific profile. I don't run a task board — I call people directly when I need them. That's how the network stays clean."*

*(Beat)*

*"In the meantime: if you're in a Holdout community and something is wrong — supply situation, pressure from outside, someone acting in bad faith inside — you can reach me through the network. You don't have to wait for me to call."*

→ END

---

**mfa_L2_nothing:**
*(She takes it as an answer — the right one)*

*"Good. I'll be in touch."*

→ END

---

### `morgan_mutual_aid` — Act 2+ — REPEATABLE
*Trigger: community operation available that matches player's operational profile.*
*Maren calls directly. Specific, practical, no preamble.*

**mma_L1:**
*"[COMMUNITY_NAME]. East side, [LOCATION_DESCRIPTOR]. They've got a supply situation — [SUPPLY_CONTEXT]. There's a route that works but it needs someone who can move through [FACTION] territory without triggering a response."*

*(Beat)*

*"That's you. I've looked at the alternatives. You're the right person for this one."*

*(Beat)*

*"If you do this, the community gets what it needs. The network gets a route that stays warm for future use. You get — actually, the supply situation, you'll see what's available when you get there. I don't overpromise."*

**Branches:**
- *"I'm in."* → `mma_L2_yes`
- *"Tell me more about the route."* → `mma_L2_route`
- *"I can't right now."* → `mma_L2_no`

---

**mma_L2_yes:**
*"Good. Maren out."*

*(She's already moved on)*

→ END *(Quest flag: `WYA.Quest.MutualAid.Active`)*

---

**mma_L2_route:**
*"[ROUTE_DETAILS — generated from operation context]. The problem section is [PROBLEM_SECTION]. That's why I called you specifically."*

*(Beat)*

*"Still in?"*

→ `mma_L2_yes` or `mma_L2_no`

---

**mma_L2_no:**
*"Understood. I'll find another option. It'll take longer."*

*(Beat — not a guilt trip, just factual)*

*"If your situation changes, let me know before the window closes."*

→ END

---

*Implementation note: `[COMMUNITY_NAME]`, `[LOCATION_DESCRIPTOR]`, `[SUPPLY_CONTEXT]`, `[FACTION]`, `[ROUTE_DETAILS]`, `[PROBLEM_SECTION]` are runtime substitutions from the operation data struct. Maren's repeatable calls are operation-specific — she is never generic.*

---

### `morgan_narrative_warning` — Act 2+ — REPEATABLE
*Trigger: any faction produces a report or analysis that misframes the player or the network.*
*Maren has the original. She's already run the comparison.*

**mnw_L1:**
*"[FACTION] filed an analysis on you."*

*(Beat — no alarm in her voice, this is routine to her)*

*"I've read it. The accurate parts: [ACCURATE_SUMMARY]. The inaccurate parts: [INACCURATE_SUMMARY]."*

*(Beat)*

*"The inaccurate parts are where the tell is. What they got wrong tells you what they're trying to build a case for — which tells you how they're planning to move against you. They're not there yet. But that's the direction."*

*(Beat)*

*"I'm not telling you to change anything you're doing. I'm telling you what they see. Act accordingly."*

**Branches:**
- *"How do you get these?"* → `mnw_L2_how`
- *"What should I adjust?"* → `mnw_L2_adjust`
- *"[Say nothing]"* → `mnw_L2_nothing`

---

**mnw_L2_how:**
*"Channels. Some of them overlap with Aldric's. Some of them are mine. I've been tracking faction narratives since before the Cascade — I ran for office, I know how institutions construct their public positions and I know what the internal documents look like before the public position gets finalized."*

*(Beat — dry)*

*"It turns out the skills transfer. Who knew."*

→ END

---

**mnw_L2_adjust:**
*"Nothing structural. What you're doing is working — they're misreading it, which means the pressure they're building is aimed at the wrong shape. If you change to correct the misread you give them the real picture."*

*(Beat)*

*"Stay as you are. Let them be wrong."*

→ END

---

**mnw_L2_nothing:**
*"Good."*

→ END

---

*Implementation note: `[FACTION]`, `[ACCURATE_SUMMARY]`, `[INACCURATE_SUMMARY]` are runtime substitutions from the triggering faction event. The call structure is constant — the content is operation-specific.*

---

### `morgan_community_read` — Act 2 — ONE-SHOT
*Trigger: player takes an action that costs them for someone in a mutual aid community — not the Reckoner network directly, a community in the network's orbit.*
*Maren has been watching. This is her telling the player what she sees.*

**mcr_L1:**
*"What you did for [COMMUNITY_NAME]."*

*(Beat)*

*"You didn't have to do that. The community isn't in the formal network — they're adjacent, they trust us, we move resources their way when we can. There was nothing obligating you to spend what you spent on that."*

*(Beat)*

*"That's the threshold I look for. Not whether someone will do the right thing when it's required — most people will. Whether they do it when it isn't. That's the tell."*

*(Beat)*

*"You're in the mutual aid network. Not the Reckoner faction — the network. That's a different thing and it's worth more."*

*(Beat — the warmth is present but she doesn't announce it)*

*"Cade's right about the operational picture. I'm right about the social one. The social one is what holds everything else up. You've just demonstrated you understand that, whether you know it or not."*

→ END

---

### `morgan_act4` — Act 4 — ONE-SHOT
*Trigger: Theory delivered. Maren is mid-operation when this fires.*
*She does not stop what she's doing. She talks while she works.*

**ma4_L1:**
*"I know. Cade told me. Aldric's been on air for three hours about it."*

*(Beat — she's clearly moving, doing something)*

*"I don't have time for the cosmological picture right now. I'll get there. Right now the Reckoner-adjacent communities in this network have heard fragments of what's happening and they're scared and they need to know what it means for them specifically. Not the theory — what do they do next week. That's what I'm working on."*

*(Beat)*

*"You want to help — actually help, not in the abstract — tell me what you know about the eastern sector's supply situation. The route through [LOCATION] that we've been using is going to come under pressure. I need to know if you've seen anything on the ground that changes the assessment."*

**Branches:**
- *"The route is still viable."* → `ma4_L2_viable`
- *"There's a problem at [LOCATION]."* → `ma4_L2_problem`
- *"I haven't been through there."* → `ma4_L2_unknown`

---

**ma4_L2_viable:**
*"Good. That's useful. I'll keep it open."*

*(Beat — still moving)*

*"When you have time for the larger picture — Cade will have processed it by tonight. Talk to him. Talk to Aldric if you want the full argument. I'll be here making sure people are eating."*

*(Dry, not bitter)*

*"Someone has to."*

→ END

---

**ma4_L2_problem:**
*"Tell me exactly."*

*(She stops what she's doing)*

*(Player details — she takes it)*

*"All right. I've got two backup routes. One of them is longer. I'll need to move some resources before the window closes."*

*(Beat)*

*"This is useful. Thank you."*

*(Back to work)*

*"Talk to Cade when you can. He needs people who have ground-level information right now, not just the theory."*

→ END *(flags `WYA.Event.MutualAid.RouteCompromised` — triggers backup route operation)*

---

**ma4_L2_unknown:**
*"Understood. If you go through there — anything that looks like Compact presence or Hollow crew moving into the corridor, flag it immediately."*

*(Beat)*

*"The communities in the eastern sector are the most exposed right now. The Act 4 situation is going to create pressure on the margins first. That's where it always starts."*

→ END

---

### `morgan_broadcast_fragment` — Act 1+ — REPEATABLE POOL
*Passive ambient. Plays through the assistant's audio system — not a full codec call.*
*The player picks up Maren's channel mid-content. Draw from the pool contextually.*
*Priority 30 — fires in quiet moments, yields to anything more urgent.*

---

**Pool — Recipes / Preservation:**

*"—and the thing people get wrong about rendered fat is the temperature. You want it low and slow, not hot. Hot fat burns and you lose half the yield. Render it low, strain it twice, seal it while it's still warm. Keeps for months without refrigeration if the seal holds."*

*"If you're salting meat, use more than you think. People underestimate because they're thinking about flavour. You're not salting for flavour — you're salting for chemistry. The salt has to reach the centre. Give it time."*

*"Rose hips are still on a lot of the old hedgerows. High vitamin C, easy to process — dry them, make a tea, add them to anything you're already cooking. The seeds and hairs inside are an irritant, strain them out. But the flesh is useful and most people are walking past it."*

*"Fermentation doesn't require anything you don't already have. Salt, water, a container that seals. The bacteria are already on the vegetables — you're just giving them conditions to work. Brine, weight it down, keep it cool. Three days for a fast ferment, two weeks for something stable. You'll know it's working when it smells sour and not wrong. There's a difference. You'll learn it."*

*"Bone broth is not complicated. Cover the bones with cold water, bring it up slowly, skim what comes off the top in the first hour. Then low heat for as long as you can — twelve hours minimum, twenty-four is better. Everything that was in those bones comes out. It doesn't taste like much until you reduce it. Reduce it."*

---

**Pool — Gardening / Growing:**

*"Garlic goes in in the fall, not the spring. If you haven't put it in yet and you still have ground that isn't frozen, do it this week. Six inches deep, flat side down, six inches apart. It overwinters underground and comes up early. It's one of the easiest things you can grow and one of the most useful things you can store."*

*"If you're starting seeds indoors, the mistake people make is not enough light. A south-facing window is not enough light. They get leggy and weak and they don't transplant well. You need a grow light or you need to wait until you can put them directly outside. Weak seedlings are more trouble than starting late."*

*"Companion planting is not folklore — it's chemistry. Tomatoes and basil, beans and corn, nasturtiums around anything aphids go after. The plants communicate through the soil. The combinations that have been passed down for centuries work because they were tested for centuries. Use them."*

*"If your soil is compacted, don't till it — you'll destroy the fungal network and it takes years to rebuild. Sheet mulch it instead. Cardboard down, compost on top, leave it over winter. By spring you have soil you can work with. Tilling is a short-term fix with long-term costs."*

*"Dandelions are not weeds. The whole plant is edible — leaves, flowers, root. The greens are bitter but that's the medicine in them. Blanch them if you don't like the bitterness. The root roasted and ground makes a decent coffee substitute. The flowers make wine if you have sugar to spare. Don't pull them."*

---

**Pool — Seasonal / Regional (GPS-contextual):**

*These lines are flagged `bRegional = true` — implementation draws from the pool that matches the player's current season and climate zone. The assistant's audio system passes `PlayerClimateZone` and `CurrentSeason` to the draw function.*

*"[SEASON] is when you want to be [SEASONAL_TASK]. In [CLIMATE_ZONE] you've got roughly [WINDOW] weeks before [CONDITION_CHANGE]. Don't wait for perfect conditions — good enough conditions and the work done beats perfect conditions and the work not started."*

*(Runtime substitution: `[SEASON]`, `[SEASONAL_TASK]`, `[CLIMATE_ZONE]`, `[WINDOW]`, `[CONDITION_CHANGE]` from seasonal data table. Minimum 12 authored entries per season per zone for the Calgary region; procedural variants generated for other regions via LLM pre-gen pipeline at location resolve.)*

---

**Broadcast closing line — appended to any fragment at random (20% chance):**

*"Maren out."*

*(Dry. She's already moved on.)*

---

## Implementation Notes

- Maren's codec portrait: she is frequently mid-task. Depending on the call, she may be looking at a map, moving supplies, writing. She is not staged for the call.
- Voice tempo: faster than Cade, more targeted than Aldric. She does not build to conclusions — she leads with them.
- Humor is dry, arrives without warning, never at length. One line, then she's already past it.
- `morgan_mutual_aid` and `morgan_narrative_warning` runtime substitutions: pull from operation/event context structs. Maren's calls are never generic — if the dynamic content isn't available, delay the call rather than fire a generic version.
- `morgan_act4` route location substitution: `[LOCATION]` pulls from `WYA.Data.Territory.EasternSectorRouteKey` — the specific Reckoner supply route relevant to the player's current region.

---

*v0.1 — Maren codec calls. First assessment, mutual aid operations, narrative warning, community read threshold, Act 4 mid-operation. Runtime substitution model throughout — Maren is always specific, never generic.*
