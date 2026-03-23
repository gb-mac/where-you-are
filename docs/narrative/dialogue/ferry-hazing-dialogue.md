# Ferry — Hazing Dialogue
*All scripted lines for the Ferryman's Toll hazing sequence.*
*Zero LLM. Every threshold, every outcome, every branch authored.*

---

## Pre-Fight

**`ferry_hazing_open`**
*Ferry looks the player over. Unhurried. He's done this enough times that he doesn't need to perform the assessment — he's just doing it.*

*"You want in. Everyone wants in. The question is what you're bringing."*

*(Beat)*

*"We'll find out."*

---

## During the Fight — Phase Lines

*Delivered mid-fight at specific health thresholds. No UI chrome. Ferry says these without stopping.*

**`ferry_phase_25`** *(player at 25% damage dealt to Ferry)*
*"There it is."*

*(He adjusts. The fight gets harder.)*

---

**`ferry_phase_50_incoming`** *(player approaching 50% — Ferry has taken significant damage)*
*"You've got something."*

*(He doesn't stop. But the player can feel the assessment shifting.)*

---

**`ferry_player_struggling`** *(player health drops below 30% before reaching 50% on Ferry)*
*"Don't stop thinking."*

*(Not encouragement. Instruction.)*

---

**`ferry_player_adapts`** *(player changes approach mid-fight after a failed sequence)*
*Nothing. Ferry notices. He doesn't say so. The fight continues.*

*(Implementation note: no line fires — Ferry's behavioral AI shifts slightly, acknowledging the adaptation by becoming fractionally less predictable. The player feels it.)*

---

## The 50% Pause

*Ferry steps back. Not retreating — pausing. He looks at the player.*

**`ferry_toll_paid`**
*"You paid the toll."*

*(Beat — genuine, not performed)*

*"You want to keep going?"*

**Branches:**
- *"I'm done."* → `ferry_standard_admit`
- *"Keep going."* → `ferry_continue`

*Branch timeout: 5 seconds. No selection = `ferry_standard_admit` auto-selected. The toll doesn't require pushing.*

---

**`ferry_standard_admit`**
*(Player takes the standard admission)*

*"All right."*

*(He steps aside. Looks up toward where Mac is watching.)*

*(No further words. Mac comes down.)*

---

**`ferry_continue`**
*(Player chooses to keep going)*

*(Ferry looks at the player for a moment. Something changes in how he's standing.)*

*"Okay."*

*(He steps back in. He was at seventy percent before. He isn't now.)*

---

## Post-50% Fight Lines

*Only fire if player chose to continue.*

**`ferry_continue_25`** *(player deals 25% more damage after resuming — now at 75% total)*
*"Further than most."*

*(Said quietly, mid-fight. Not a compliment. An observation.)*

---

**`ferry_continue_close`** *(player gets Ferry below 15%)*
*Nothing. Ferry goes quiet. The fight is real now.*

*(Implementation note: ambient audio drops. Ferry's behavioral pattern becomes fully reactive — no more assessment mode. He's not holding back.)*

---

## Outcomes

### Under 25% — Too Early

**`ferry_notyetunder25`**
*(Ferry stops the fight before it escalates. Clean.)*

*"Come back."*

*(He walks away. Nothing else.)*

*(Quest flag: retry available after specific story milestone — faction reputation threshold or Act progression.)*

---

### 25–49% — Acknowledged, Not Enough

**`ferry_notyet2549`**
*(Ferry steps back at the natural stopping point.)*

*"You've got something."*

*(Beat)*

*"Not enough yet. Come back."*

*(Beat — he's already moving away)*

*"Sooner than you think."*

*(Quest flag: retry available after shorter milestone. Minor reputation bump registered.)*

---

### 50% — Standard Admission (player took the out)

**`ferry_admit_standard`**
*(After `ferry_standard_admit` — Mac has come down)*

*(Ferry to Mac, not to the player:)*
*"Paid the toll."*

*(Mac nods. The debrief begins. Ferry steps back — present but not the focus anymore.)*

---

### 51–74% — Continued, Didn't Finish

**`ferry_continued_sub75`**
*(Player kept going, got further, fight ended short of full win)*

*(Ferry steps back. Breathes.)*

*"Further than most."*

*(Beat)*

*"You're in."*

*(He looks up toward Mac. Mac is already coming down.)*

---

### 75–99% — Close

**`ferry_continued_close`**
*(Player got Ferry genuinely low — he's feeling it)*

*(Ferry straightens up slowly.)*

*"All right."*

*(Long beat — he's recalibrating)*

*"That's the closest anyone's gotten."*

*(Beat)*

*"You're in. And you're going to hear about this from me for a long time."*

*(The last line lands dry. It's a compliment. He won't say that directly.)*

---

### Full Win — Ferry Goes Down

**`ferry_win`**
*(Ferry is down. He stays there for a moment. Gets up.)*

*"Huh."*

*(Long beat. He looks at the player the way you look at something you weren't expecting.)*

*"You paid the toll and then some."*

*(Beat)*

*"Mac."*

*(He doesn't need to say more. Mac is already moving.)*

---

## Mac's Arrival — Variant Lines

*Mac comes down after every outcome above 50%. Line varies by threshold.*

**`mac_arrival_standard`** *(50% standard)*
*"Ferry doesn't say 'all right' to many people."*

*(Beat)*

*"Come inside."*

---

**`mac_arrival_continued`** *(51–74%)*
*"You kept going."*

*(Beat — he's noting it, not asking about it)*

*"Come inside."*

---

**`mac_arrival_close`** *(75–99%)*
*"You almost had him."*

*(Beat)*

*"That means something here."*

*(Beat)*

*"Come inside. I want to hear what you've been doing out there."*

---

**`mac_arrival_win`** *(full win)*
*(Mac looks at Ferry briefly. Then at the player.)*

*"First time."*

*(Beat)*

*"Come inside. We have things to talk about."*

*(Implementation note: Mac's debrief for full-win players runs longer and ends with a harder first operation assignment. He doesn't explain why. The player will figure it out.)*

---

## Ferry's Debrief Presence

*Ferry stays present during Mac's debrief but doesn't lead it. One line, delivered at the end, after Mac has finished.*

**`ferry_debrief_standard`** *(50% admission)*
*"You'll know what you did wrong by tomorrow."*

*(Beat)*

*"Don't tell me. Fix it."*

---

**`ferry_debrief_continued`** *(continued past 50%)*
*"You made a decision back there."*

*(Beat)*

*"I respect it."*

*(Nothing more.)*

---

**`ferry_debrief_win`** *(full win)*
*(Ferry says nothing during the debrief. On the way out, quietly, not for the room:)*

*"Good fight."*

*(That's all.)*

---

## Background-Specific Lines

**Veteran** — `ferry_veteran_pre`
*Before the fight starts, Ferry clocks the movement pattern.*

*(To Mac, not the player:)*
*"VSO."*

*(Mac, from his position:)*
*"I know."*

*(Ferry, back to the player:)*
*"All right. Let's see it."*

---

**First Responder** — `ferry_firstresponder_phase`
*At 25% — Ferry escalates faster than usual.*

*"Community trust is a skill set. I can see it. Doesn't help you here."*

*(The second phase starts immediately.)*

---

**Tradesperson** — `ferry_trade_post`
*After admission, on the way inside:*

*"You've been fixing things. Mac mentioned it."*

*(Beat)*

*"Useful."*

---

**Researcher** — `ferry_researcher_post`
*After admission:*

*"You think differently than the others who've come through."*

*(Beat)*

*"Derek's going to want to talk to you."*

---

**Homesteader** — `ferry_homesteader_post`
*After admission:*

*"You've got land."*

*(Beat)*

*"Hold it."*

*(That's the welcome.)*

---

## Implementation Notes

- Fight health tracking: `FerryHazingSubsystem` tracks damage dealt as percentage of Ferry's total health pool. Thresholds fire events that trigger dialogue tags above.
- 50% branch timeout: 5 seconds, auto-selects `ferry_standard_admit`. No pressure on the player to decide fast — but the choice should feel real.
- `ferry_player_adapts`: no audio line, behavioral AI shift only. The adaptation is acknowledged through Ferry's response, not through words. Flag to AI/combat team.
- Phase lines (`ferry_phase_25`, `ferry_phase_50_incoming`) fire once each, mid-combat, no UI indication. Ambient only.
- Mac's position: he is not reachable or interactable during the fight. Environmental presence only — silhouette, elevated. Camera does not auto-frame him.
- Ferry's `ferry_debrief_win` line fires on exit from the debrief space, not during it. Quiet. The room doesn't hear it.
- Scale recorded permanently in `WYA.Record.ReckоnerHazingResult` — visible in Reckoner faction standing screen.

---

*v0.1 — Ferry hazing dialogue. Full threshold scale, 50% pause with player choice, all outcome variants, Mac arrival lines, debrief presence, background-specific moments. Implementation notes flagged.*
