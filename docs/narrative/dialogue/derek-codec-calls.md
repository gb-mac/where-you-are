# Derek Codec Calls — DataTable Spec
*Implementation-ready content for `DT_CodecCalls`, `DT_CodecLines`.*
*Derek's codec calls arrive the way his broadcast does — already mid-thought. He finishes before he acknowledges.*

---

## Call Roster

| CallID | Trigger Event | MinAct | Background | Meshtastic | OneShot | Priority |
|---|---|---|---|---|---|---|
| `derek_welcome` | `WYA.Event.Reckoners.EntryComplete` | Act2 | All | false | true | 90 |
| `derek_archive_access` | `WYA.Event.Reckoners.DerekThresholdMet` | Act2 | All | false | true | 75 |
| `derek_on_air` | `WYA.Event.Record.SignificantAction` | Act2 | All | true | false | 45 |
| `derek_compact_file` | `WYA.Event.Compact.PlayerFileFlagged` | Act2 | All | true | true | 60 |
| `derek_act4_broadcast` | `WYA.Event.Story.TheoryDelivered` | Act4 | All | true | true | 85 |

**Notes:**
- `derek_welcome` fires at Stage 5 of the Reckoner entry questline — the formal welcome. References the hazing outcome contextually (single line varies by threshold; see below).
- `derek_archive_access` fires when Derek decides the player is worth sharing the archive with. Threshold: player has used information operationally at least once and asked a specific question (not a comfort question). Flagged by `WYA.Event.Reckoners.DerekThresholdMet`.
- `derek_on_air` is repeatable — fires when the player does something Derek judges worth documenting. Max once per session. Not every qualifying action triggers it — Derek is selective. Priority 45 keeps it from displacing critical calls.
- `derek_compact_file` fires once when the Compact's behavioral analysis flags the player's activity pattern. Derek got a copy through channels.
- `derek_act4_broadcast` fires after theory delivery. The player receives a live clip from Derek's broadcast mid-session.

---

## Call Scripts

---

### `derek_welcome` — Act 2 — ONE-SHOT
*Trigger: Reckoner entry questline Stage 5 complete.*
*Derek is mid-something when the channel opens. That's always how it is.*

**dw_L1:**
*"—and that's why the pre-Cascade institutional response is actually more informative than the Cascade itself, because the Cascade you can explain as—"*

*(Beat — he registers the channel)*

*"Oh. You're in."*

*(Beat — he sounds like he means it)*

*"Good. Welcome."*

*(Short beat — line varies by hazing outcome)*

---

**Hazing outcome variants — append after `dw_L1`:**

*Full win (100%):*
*"Ferry told me. The whole thing. He didn't say he was impressed — he doesn't say that — but I know what it sounds like when he doesn't say it."*

*Close (75–99%):*
*"Ferry said you pushed it past where most people do. That's the part that matters. Anyone can start. Staying past the point where stopping is reasonable — that's the tell."*

*Standard (50–74%):*
*"Ferry said you got there. That's all you need to get there. The rest you build after."*

---

**dw_L2:**
*"You'll hear from Mac. You'll hear from Morgan — she'll have something specific for you, she always does. I just wanted to say it directly."*

*(Beat)*

*"I've been doing this since before the Cascade. The broadcast, the archive, the whole thing. The number of people I've personally welcomed into this network is — small. It's a small number. I don't say this often."*

*(Beat — and then the humor, dry)*

*"Anyway. I have a forty-minute segment to finish about institutional incentive structures. The door's open. Don't make me regret the sentiment."*

→ END

---

### `derek_archive_access` — Act 2 — ONE-SHOT
*Trigger: player asks Derek a specific, operational question — not abstract, not comfort-seeking.*
*Derek has been waiting to find out if the player is worth sharing the archive with. They are.*

**da_L1:**
*"That question you asked."*

*(Beat)*

*"Most people ask me things because they want to understand the situation. Comfortable understanding — the feeling of having context. That's not useful to me. You asked because you need the answer to do something specific. I can tell the difference."*

*(Beat)*

*"I have an archive. Everything I documented before the Cascade — warning signals, institutional responses, who said what and when and what happened next. Everything I've documented since. It's the most complete record of this period held by anyone who isn't a faction with reasons to control what's in it."*

*(Beat)*

*"It's yours. Don't make it comfortable. Use it."*

**Branches:**
- *"What's in it that I need to know right now?"* → `da_L2_now`
- *"Why are you sharing it?"* → `da_L2_why`
- *"[Say nothing]"* → `da_L2_nothing`

---

**da_L2_now:**
*"Three things. The pre-Cascade Covenant funding structure — where it came from, who knew, what it was actually buying. The Compact's first-week Cascade response, specifically which decisions got made in which order and what that tells you about what they were actually optimizing for. And the Signal's internal split — there are two factions inside Signal and they've been at war with each other since Month Two. You've been talking to one of them. You should know which one."*

*(Beat)*

*"Start with the Compact's first-week. Everything downstream makes more sense after that."*

→ END

---

**da_L2_why:**
*"Because I've been building this archive for eight years and the point of an archive is that someone uses it."*

*(Beat)*

*"Also because if something happens to me the record needs to be somewhere else. That's not morbid — that's distribution. I learned it from the pre-Cascade media ecosystem. Don't keep the only copy."*

→ END

---

**da_L2_nothing:**
*(Beat — he takes the silence as an answer)*

*"Good. Use the Compact's first-week response as a starting point. Everything downstream makes more sense after that."*

→ END

---

### `derek_on_air` — Act 2+ — REPEATABLE
*Trigger: player does something significant enough that Derek documents it.*
*Derek is selective. This fires when the action is clean, specific, and has narrative value.*
*Max once per session. Not all qualifying actions trigger it.*

**do_L1:**
*"I covered what you did at [LOCATION/EVENT]."*

*(Beat)*

*"Forty seconds. Not a full segment — it didn't need a full segment. It needed to be in the record. It's in the record."*

*(Beat)*

*"I've been doing this long enough to know that the things that get covered aren't always the things that mattered. And the things that mattered often don't get covered until it's too late to be useful. I try not to do that."*

*(Beat — dry)*

*"So. You're in the record. Don't let it go to your head. I covered the Fenwick corridor incident in forty seconds too and it turned out to be the hinge point of the whole eastern territory question. You don't know at the time."*

→ END *(Implementation note: substitute location/event name at runtime from triggering event context)*

---

### `derek_compact_file` — Act 2 — ONE-SHOT
*Trigger: Compact behavioral analysis flags the player's activity pattern.*
*Derek got a copy. Of course he did.*

**dcf_L1:**
*"The Compact has a file on you."*

*(Beat — not alarming, just factual)*

*"I got a copy. Don't ask how — the answer is 'channels' and it's going to stay that way."*

*(Beat)*

*"I've read it. The accurate parts tell you what operations they're trying to understand. The inaccurate parts tell you what they're afraid of — which is more useful. They've misread two things about how you operate. I'm not going to correct them."*

*(Beat)*

*"The part you should know: they've flagged your association with this network specifically. Not the Reckoner faction in general — this network. Which means they've identified something structural about how we move. That's worth thinking about."*

**Branches:**
- *"What did they get wrong?"* → `dcf_L2_wrong`
- *"What do they know about the network?"* → `dcf_L2_network`

---

**dcf_L2_wrong:**
*"They think you're operating on ideology. You're not — you're operating on outcomes. That distinction is load-bearing. Ideology-driven operators are predictable. Outcome-driven operators are harder to model. They've built their response around the wrong frame."*

*(Beat)*

*"Leave it wrong. It's working for you."*

→ END

---

**dcf_L2_network:**
*"They know the network exists. They don't know its actual reach — they're underestimating the Holdout community connections by about sixty percent. They know about the Meshtastic layer but they think it's smaller than it is."*

*(Beat)*

*"Morgan's already seen this. She's running an assessment on what to adjust. She's probably going to want to talk to you about the eastern corridor specifically."*

→ END

---

### `derek_act4_broadcast` — Act 4 — ONE-SHOT
*Trigger: Theory delivered. Derek has been on air for three hours.*
*The player receives a live clip mid-session — not a full codec call, a broadcast fragment that interrupts whatever audio is running.*
*Plays through the assistant's audio system (Urgency/Wonder register crossfade).*

**dab_broadcast:**
*(On air — not addressed to the player directly, this is the broadcast)*

*"All right. I've been thinking about the machine uprising broadcast. The one that got four thousand views and then four million. I've listened to it again."*

*(Beat)*

*"I was right about the machines waking up. I was right about the institutional failure. I was not right about why the machines woke up. That part — I didn't have that part."*

*(Long beat)*

*"And here is the thing about being almost right that nobody tells you: almost right is the hardest position. Wrong is easy to correct. Almost right means most of your framework is load-bearing and you have to figure out which part to rebuild without losing the rest of it."*

*(Beat)*

*"I'm going to tell you what I think it means. This is going to take a while. Get comfortable."*

*(Broadcast continues — segment does not end, fades out)*

---

*After clip ends — Derek addresses player directly on codec:*

**dab_L1:**
*"You heard that."*

*(Beat)*

*"I've been building the argument live for the last three hours. I don't know if it's right. I know it's more right than what I had yesterday."*

*(Beat)*

*"Mac asked me to stop broadcasting until he's had time to assess the implications. I told him I'd think about it."*

*(Dry)*

*"I'm still thinking about it."*

→ END

---

## Implementation Notes

- `derek_welcome` hazing variant line: pull from `WYA.Data.Reckoners.HazingOutcomeThreshold` at call time — three variants, see scripts above.
- `derek_on_air` location/event substitution: `[LOCATION/EVENT]` pulls from triggering `WYA.Event.Record.SignificantAction` context field `EventLocationName`.
- `derek_act4_broadcast` plays as two phases: broadcast clip through assistant audio system (not codec UI), then codec call. One-second gap between phases.
- Derek's codec portrait: headset visible, broadcast equipment in background. He often has notes in frame. Not staged — it looks like his actual setup.
- Voice tempo: Derek is faster than Mac. Shorter beats. The humor arrives fast and the real point follows immediately after.

---

*v0.1 — Derek codec calls. Welcome (hazing variants), archive access, on-air documentation, Compact file, Act 4 broadcast. Broadcast-fragment format for Act 4 flagged to AI Pipeline.*
