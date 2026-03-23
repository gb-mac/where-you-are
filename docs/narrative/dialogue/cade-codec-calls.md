# Cade Codec Calls — DataTable Spec
*Implementation-ready content for `DT_CodecCalls`, `DT_CodecLines`. Zero LLM. All Cade dialogue is authored.*
*Uses same struct definitions as `burke-codec-calls.md`.*

---

## Call Roster

| CallID | Trigger Event | MinAct | Background | Meshtastic | OneShot | Priority |
|---|---|---|---|---|---|---|
| `mac_first_contact` | `WYA.Event.Reckoners.FirstContact` | Act2 | All | false | true | 80 |
| `mac_veteran_first` | `WYA.Event.Reckoners.FirstContact` | Act2 | Veteran | false | true | 100 |
| `mac_compact_encroach` | `WYA.Event.Compact.AnnexationAttempted` | Act2 | All | true | false | 70 |
| `mac_covenant_observed` | `WYA.Event.Covenant.AscentWitnessed` | Act2 | All | true | true | 65 |
| `mac_burke_disagreement` | `WYA.Event.Burke.MacMentioned` | Act2 | All | false | true | 85 |
| `mac_burke_codec_argument` | `WYA.Event.Reckoners.BurkeContactEstablished` | Act3 | All | true | true | 100 |
| `mac_cleared_fracture` | `WYA.Event.Reckoners.FractureTriggered` | Act3 | All | true | true | 95 |
| `mac_act4_gap` | `WYA.Event.Story.TheoryDelivered` | Act4 | All | true | true | 90 |
| `mac_record_costly` | `WYA.Event.Record.CostedSomething` | Act2 | All | true | false | 35 |

**Notes:**
- `mac_veteran_first` overrides `mac_first_contact` for Veteran background — same trigger, different script
- `mac_burke_codec_argument` requires: `burke_first_vael` + `mac_first_contact` + Meshtastic node connected
- `mac_record_costly` fires once per qualifying Record event, max once per session — lower priority than Burke's version (35 vs 40); if both queue, Burke fires first
- `mac_compact_encroach` is repeatable — fires each time a new annexation attempt is logged, once per session

---

## Call Scripts

---

### `mac_first_contact` — Act 2 — ONE-SHOT
*Trigger: player reaches Reckoner network for the first time.*
*All backgrounds except Veteran.*

**mac_first_L1:**
*"You came recommended. I'll give you fifteen minutes."*

*(Beat)*

*"I'm Cade. You probably know that already. Sit down. I want to hear how you've been operating."*

*(Player describes their background — branch)*

**Branches:**
- *"I've been working with the Compact"* → `mac_first_L2_compact`
- *"I've been keeping independent"* → `mac_first_L2_independent`
- *"I've been working with the Wardens"* → `mac_first_L2_wardens`

---

**mac_first_L2_compact:**
*"Compact. All right. Which wing?"*

*(Beat — player doesn't need to answer)*

*"Doesn't matter. I can read it. You're here, which means you've seen the edges of what they are. Good. That's the right time to be having this conversation."*

→ `mac_first_L3`

---

**mac_first_L2_independent:**
*"Independent. That's either very smart or very stubborn and usually both."*

*(Beat)*

*"I respect it. Means you've been reading the ground yourself. What are you seeing?"*

→ `mac_first_L3`

---

**mac_first_L2_wardens:**
*"Wardens. Old Houses?"*

*(Beat)*

*"Good people. I have disagreements with the process — the Moot, the deliberation, the way they've built in friction. In a crisis you don't have time for friction. But the underlying thing — the craft, the accountability, the idea that you hold to something — that's right. I can work with people who come out of that."*

→ `mac_first_L3`

---

**mac_first_L3:**
*"Here's what I need to know about anyone who comes into this network. Not what you believe. What you've done. What it cost you. What you'd do again."*

*(Beat)*

*"We can get to the rest later."*

---

### `mac_veteran_first` — Act 2 — ONE-SHOT
*Trigger: player reaches Reckoner network for the first time. Veteran background only.*

**mac_vet_L1:**
*"ODA?"*

*(Beat — doesn't wait for confirmation)*

*"Yeah. You move like it. Sit down."*

*(Long beat — he assesses)*

*"VSO. Am I close?"*

*(Beat)*

*"You came home and found it here. The thing you were doing in other people's countries. I know what that looks like on a person. I had it too."*

*(Beat)*

*"Difference is I had about eight years to get angry before the Cascade gave me something to do about it. You look like you're still in the early part of that."*

*(Beat)*

*"I'm not. Come back when you're ready to hear what comes after the anger. I'll be here."*

---

### `mac_burke_disagreement` — Act 2 — ONE-SHOT
*Trigger: player mentions Burke to Cade or Cade's name appears in a Burke codec call.*
*Fires at Cade's location on next visit.*

**mac_burke_L1:**
*"Burke."*

*(Beat)*

*"Yeah. I know Burke. Sat at his fire. He's the smartest man I've met in the post-Cascade world. Possibly before it."*

*(Beat)*

*"He's also wrong about the timeline. He thinks we have time to position people, build networks, wait for the framework to hold. I think the framework is what's infected. You don't save a structure by working inside it. You save what the structure was supposed to protect by tearing it down and rebuilding from the load-bearing parts."*

*(Beat)*

*"He'd tell you I'm wrong. He's not wrong that I'm wrong. I just think being right on the slow timeline doesn't help anyone alive right now."*

**Branches:**
- *"What does he say about you?"* → `mac_burke_L2_hissay`
- *"Who's actually right?"* → `mac_burke_L2_right`
- *"[Say nothing]"* → `mac_burke_L2_nothing`

---

**mac_burke_L2_hissay:**
*"He says I stopped at the diagnosis."*

*(Long beat)*

*"He's not wrong. The difference between us is I think the diagnosis is the action. If you name the thing accurately, clearly, without apology — you change what's possible. Burke thinks naming it is just the first step. Maybe he's right. I've watched him work for a long time and his method produces results."*

*(Beat)*

*"I just don't think we have the time his method requires."*

→ END

---

**mac_burke_L2_right:**
*"About the diagnosis? Both of us. About the response? That's what the next five years will decide."*

*(Beat)*

*"Come back and tell me who was right when you know."*

→ END

---

**mac_burke_L2_nothing:**
*"Good answer."*

*(Beat)*

*"Most people feel like they have to pick a side. You don't. Not yet. Watch what both methods produce. Then decide."*

→ END

---

### `mac_burke_codec_argument` — Act 3 — ONE-SHOT
*The argument. Triggers when both Burke and Cade are in the player's active network and Meshtastic is connected.*
*Three-way codec. Burke opens. Cade responds. Player holds the channel.*

---

**bma_L1** *(Burke)*
*"Cade."*

*(Beat)*

*"I hear you cleared the Fenwick Hollow corridor last week."*

---

**bma_L2** *(Cade)*
*"Burke."*

*(Beat)*

*"We did. Compact was going to absorb it inside sixty days. Three Holdout communities in that corridor are still there because we moved first."*

---

**bma_L3** *(Burke)*
*"I know. I had two contacts in that corridor. Both of them are going to have questions to answer now because the way you moved drew attention. There are people in the Compact's counterintelligence function who were watching that corridor for different reasons. You've put them in a harder position."*

---

**bma_L4** *(Cade)*
*"Your contacts can answer the questions. They're good people, they can handle scrutiny. Meanwhile the communities they live in aren't getting annexed. I'll take that trade."*

---

**bma_L5** *(Burke)*
*"You made that trade for them."*

*(Beat)*

*"That's my point, Cade. Not the outcome — the outcome was fine. The method. You moved without knowing what else was in that space. You were right about the corridor. You didn't know what else you were touching."*

---

**bma_L6** *(Cade)*
*"If I'd waited to know everything I was touching I'd still be waiting. The communities would be registered. That's the difference between your method and mine — mine has a cost and produces a result. Yours produces a result eventually and the cost is everything that happened while you were building the conditions."*

*(Beat)*

*"I've watched your method work. I've watched it take two years to do what we did in a week. I don't have two years. Neither do those communities."*

---

**bma_L7** *(Burke)*
*"You're not wrong about the timeline."*

*(Long beat)*

*"I'm going to tell you something I don't say often. I think you're going to be very useful in Act 4 of this situation. What you're building — the network, the comms, the operational capacity — there's a version of this that matters more than either of us currently understands."*

*(Beat)*

*"I need you to still be intact when that becomes clear. That means not burning the contacts I've spent four years positioning."*

---

**bma_L8** *(Cade)*
*"What's Act 4?"*

---

**bma_L9** *(Burke)*
*"Ask [PLAYER_NAME]."*

*(Beat)*

*"They're the one holding this channel."*

*(Beat — both men go quiet. The codec holds.)*

**Branches:**
- *"There's something behind the Vael"* → `bma_L10_tell`
- *"I need more time to be sure"* → `bma_L10_wait`
- *"[Say nothing]"* → `bma_L10_nothing`

---

**bma_L10_tell:**
*(Burke)* *"There it is."*

*(Cade — long beat)*
*"Say that again."*

*(Player repeats or the assistant relays the Vael warning summary)*

*(Cade — very long beat)*

*"...How long have you known this?"*

*(Burke)* *"Long enough. This is why I said what I said about your timeline."*

*(Cade — quiet)*
*"All right."*

*(Beat)*

*"All right. What do you need?"*

→ END *(triggers `WYA.Event.Reckoners.MacInformed`)*

---

**bma_L10_wait:**
*(Burke)* *"Take your time."*

*(Cade)* *"I'll be here."*

*(Beat)*

*"Burke — this conversation isn't finished."*

*(Burke)* *"It never is."*

→ END *(`WYA.Event.Reckoners.MacArgumentUnresolved` — revisit available)*

---

**bma_L10_nothing:**
*(Long beat — both men wait)*

*(Cade)* *"Burke."*

*(Burke)* *"I know."*

*(Cade)* *"We'll talk later."*

*(Burke)* *"Yes."*

→ END *(`WYA.Event.Reckoners.MacArgumentUnresolved`)*

---

### `mac_cleared_fracture` — Act 3 — ONE-SHOT
*Trigger: Cleared/Builders fracture event.*

**mac_frac_L1:**
*"I need to tell you what just happened before you hear it from someone else."*

*(Beat)*

*"The Cleared wing moved without authorization. I knew they were going to — I didn't know when. Three of our people are dead. Two of theirs. The target was a Compact logistics hub and they hit it."*

*(Beat)*

*"The operation worked. That's the part that makes this hard."*

*(Beat)*

*"What comes next is going to require you to make a decision about where you stand in this network. I'm not going to make that decision for you. I'm going to tell you what I'm doing: I'm holding the Builders together and I'm cutting the Cleared loose. Not because they were wrong about the target. Because I can't run a network that I don't control. That's not a network. That's a liability."*

*(Beat)*

*"You should know that before you decide anything."*

---

### `mac_act4_gap` — Act 4 — ONE-SHOT
*Trigger: Theory delivered. Cade has NOT yet been told via `bma_L10_tell`.*
*If `WYA.Event.Reckoners.MacInformed` already set — skip this call.*

**mac_gap_L1:**
*"Something changed. I can read it in the field — the Coherent movement patterns, the Compact's response time, the Vael event frequency. Something changed and nobody in my network has the picture."*

*(Beat)*

*"You have the picture. I can tell. You've had it for a while."*

*(Beat)*

*"I'm not asking you to explain the whole thing right now. I'm asking you to tell me one thing: is the threat I've been organizing against — the internal drift, the institutional failure, the Estrogenic Drift — is that still the primary problem?"*

**Branches:**
- *"No. There's something bigger."* → `mac_gap_L2_bigger`
- *"It's still part of it."* → `mac_gap_L2_part`

---

**mac_gap_L2_bigger:**
*"Yeah."*

*(Very long beat)*

*"Okay. Tell me."*

→ END *(triggers full theory delivery scene — separate quest flag)*

---

**mac_gap_L2_part:**
*"Part of it."*

*(Beat)*

*"All right. So I've been fighting a real enemy with the wrong frame. That's — I can work with that. What's the frame?"*

→ END *(triggers partial theory delivery — Cade adapts faster than expected)*

---

### `mac_record_costly` — Act 2+ — REPEATABLE
*Fires when player does something that cost them for someone in or adjacent to the Reckoner network.*
*Lower priority than Burke's version — Burke fires first if both queue.*

**mac_costly_L1:**
*"[NPC name] told me what you did."*

*(Beat)*

*"I don't say this often: that was the right call."*

*(No elaboration. That's the whole call.)*

*(Implementation note: substitute NPC name at runtime. Same `WYA.Event.Record.CostedSomething` trigger as `burke_record_costly`. If Burke fires this session, Cade does not.)*

---

## Implementation Notes

- `mac_burke_codec_argument` is a three-way codec — requires a new UI state where two NPC portraits appear simultaneously. Flag to AI Pipeline agent for implementation planning.
- `[PLAYER_NAME]` in `bma_L9` — pull from player name set in prologue.
- Burke's lines in `mac_burke_codec_argument` reference the `burke_codec` audio profile, not Cade's. Two separate voice/audio channels.
- Branch timeout: 8 seconds (same as Burke calls). No selection = `bma_L10_nothing` auto-selected.
- `WYA.Event.Reckoners.MacInformed` flag prevents `mac_act4_gap` from firing if the player already told Cade via the three-way codec.

---

*v0.1 — Cade codec calls. Full call roster, first contact (all backgrounds + Veteran variant), Burke disagreement, three-way Burke/Cade argument with player branch, fracture event, Act 4 gap, record-costly. Implementation notes flagged.*
