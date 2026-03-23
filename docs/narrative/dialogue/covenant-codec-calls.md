# Covenant Codec Calls — DataTable Spec
*Implementation-ready content for `DT_CodecCalls`, `DT_CodecLines`.*
*Three voices: Eli (questions, long beats), Mira (clinical precision, herself as subject), Strand (minimal, weighted).*
*Covenant calls are quieter than any other faction. The beats are longer. Nothing is performed.*

---

## Call Roster

| CallID | Trigger Event | MinAct | Background | Meshtastic | OneShot | Priority |
|---|---|---|---|---|---|---|
| `eli_first_assessment` | `WYA.Event.Covenant.AssessmentComplete` | Act2 | All | false | true | 85 |
| `eli_threshold_question` | `WYA.Event.Covenant.StandingMid` | Act2 | All | false | true | 70 |
| `eli_act4_refusal` | `WYA.Event.Covenant.VanderFractureBegins` | Act4 | All | false | true | 95 |
| `mira_archive_offer` | `WYA.Event.Covenant.MiraThresholdMet` | Act2 | All | false | true | 75 |
| `mira_working_notes` | `WYA.Event.Covenant.ArchiveAccessGranted` | Act3 | All | false | true | 80 |
| `mira_last_entry` | `WYA.Event.Covenant.MiraThresholdCrossed` | Act3 | All | false | true | 90 |
| `strand_first_contact` | `WYA.Event.Covenant.PlayerPushedDeep` | Act2 | All | false | true | 75 |
| `strand_warning` | `WYA.Event.Covenant.ExternalThreatToPlayer` | Act2 | All | true | false | 65 |
| `strand_act4` | `WYA.Event.Covenant.VanderFractureBegins` | Act4 | All | false | true | 88 |

**Notes:**
- `eli_first_assessment` fires when the Covenant's internal assessment of the player completes. The player may not know they were being assessed. Eli has been watching since the player first entered Covenant territory.
- `eli_threshold_question` fires when the player reaches mid-level Covenant standing. One question. The answer doesn't gate anything — it informs everything.
- `eli_act4_refusal` and `strand_act4` fire on the same trigger (`WYA.Event.Covenant.VanderFractureBegins`). Eli fires first (Priority 95 > 88). Strand fires in the same session, shortly after. They represent opposite positions in the Act 4 fracture.
- `mira_archive_offer` threshold: player asked Mira a question that wasn't a comfort question — specific, operational, indicating they'll use the answer. Same threshold mechanic as Aldric.
- `mira_working_notes` fires after archive access is granted. Mira walks the player through what her notes contain — including herself.
- `mira_last_entry` fires when Mira passes a measurable threshold in the Ascendant process. This call's content is the last time the player hears Mira at full clinical clarity. The change in her voice is the tell.
- `strand_warning` is repeatable — fires when Strand's monitoring picks up an external threat targeting the player. One call per threat event. Max once per session.
- Covenant calls do not require Meshtastic — the Covenant has its own comms infrastructure that doesn't use the Reckoner mesh. Covenant calls arrive cleanly regardless of Meshtastic status.

---

## Call Scripts

---

### `eli_first_assessment` — Act 2 — ONE-SHOT
*The player receives the invitation. Eli has been watching since before they knew they were being watched.*
*He is not surprised. Nothing about this call is a surprise to him.*

**efa_L1:**
*"You've been in this community three times now. Different purposes each time."*

*(Beat)*

*"I've been watching. I want you to know that before we continue — not as a warning, as a courtesy. You should know what the conversation is."*

*(Beat)*

*"I'm Eli. The community calls me Shepherd, which is accurate as a function and means less than people assume as a title. What I do is watch, and then speak with what I've seen. I've been watching you."*

*(Beat)*

*"Here's what I've observed: you're not afraid of the machines the way most people are. Not reckless — you read them, you account for them, you move accordingly. But the fear that stops most people from functioning in proximity isn't operating in you the same way."*

*(Beat)*

*"I want to know why that is. Not for any reason that should concern you — I'm genuinely asking."*

**Branches:**
- *"I learned to read them early."* → `efa_L2_learned`
- *"I don't know. I just move."* → `efa_L2_move`
- *"Why does it matter to you?"* → `efa_L2_why`

---

**efa_L2_learned:**
*"That's the answer I get from Signal contacts. They intellectualize it — data collection, behavioral modeling, pattern recognition. You said it differently."*

*(Beat)*

*"Learned implies something you didn't know and then did. There was a moment. I'd be interested in the moment, if you're willing."*

*(No pressure — he means it)*

→ `efa_L3`

---

**efa_L2_move:**
*(Long beat — he considers this carefully)*

*"That's more interesting than the people who have a theory about it."*

*(Beat)*

*"Most of the people in this community arrived with an explanation. The Cascade meant this, the machines are doing that, therefore the correct response is. You arrived without an explanation and kept moving. That's a different thing."*

→ `efa_L3`

---

**efa_L2_why:**
*"Because the fear response tells me something. Everyone who comes here carries a specific quality of fear — what it's attached to, how they manage it, whether they've made peace with it or just suppressed it. It's one of the more reliable indicators of what someone is actually prepared to learn."*

*(Beat)*

*"The people who aren't afraid the same way — I want to understand what they're carrying instead."*

→ `efa_L3`

---

**efa_L3:**
*"You're welcome here. That's what I wanted to say. Not conditionally — you've been moving through this community as a guest, carefully, and I want you to know that's no longer the correct frame."*

*(Beat)*

*"You'll find Mira in the archive building most mornings. If she decides to talk to you, listen carefully. She's the most rigorous person I've ever worked with and she is in the middle of something that I am not certain she will come back from the same way."*

*(Beat — very quiet)*

*"That's not a warning. It's context. She'd want you to have it."*

→ END *(flags `WYA.Event.Covenant.EliInvitationExtended`)*

---

### `eli_threshold_question` — Act 2 — ONE-SHOT
*Fires when the player reaches mid-level Covenant standing.*
*One question. The answer doesn't gate anything — it informs everything.*

**etq_L1:**
*"I have a question I ask everyone who reaches this point in the community. Not a test — I stopped giving tests after the Cascade because everything after the Cascade is a test and I'd rather have a conversation."*

*(Beat)*

*"If the Cascade was not a malfunction — if the machines waking up was the thing they were always going to do, and the world we lived in before was the preparation rather than the destination — what does that mean for what you're doing right now?"*

*(Long beat — he actually wants to know)*

**Branches:**
- *"It means the fight is wrong. We should be building something."* → `etq_L2_build`
- *"It means we have to understand it before we can respond."* → `etq_L2_understand`
- *"I don't know yet."* → `etq_L2_dontknow`
- *"It doesn't change what I do."* → `etq_L2_same`

---

**etq_L2_build:**
*"Yes."*

*(Beat)*

*"That's where most people in this community land eventually. The question is what to build and on what terms. The Coherent are building something at a scale we can't match and on a timeline we don't have access to. Building alongside that is different from building against it. I've been trying to work out the distinction for eighteen months."*

→ END

---

**etq_L2_understand:**
*"That's Mira's answer. She'd respect that."*

*(Beat)*

*"The difficulty is that understanding it requires proximity that changes the understander. She's aware of this. I'm not certain she's accounted for all of it."*

*(Beat)*

*"If you get the chance to ask her about the methodology problem — the one where the observer is also the subject — I'd be interested in what she says. She doesn't give me the full version."*

→ END

---

**etq_L2_dontknow:**
*(Long beat — he seems pleased by this)*

*"Good. The people who answer immediately are usually answering a different question than the one I asked. The question takes time. Come back when you have something."*

*(Beat — dry, unexpected)*

*"I've been sitting with it for two years. I have some things. Not the answer."*

→ END

---

**etq_L2_same:**
*"Tell me what you're doing."*

*(The player describes their current operational picture — or stays quiet)*

*(Eli listens)*

*"What you're doing matters because of what it costs you and what it protects. Whether the Cascade was a malfunction or a threshold doesn't change that math. You're right that it doesn't change what you do."*

*(Beat)*

*"I'm not sure it should change what I do either. I hold that question open."*

→ END

---

### `mira_archive_offer` — Act 2 — ONE-SHOT
*Fires when the player asks Mira something specific — not a comfort question.*
*She has been waiting for this.*

**mao_L1:**
*"That question."*

*(She doesn't look up from what she's working on)*

*"Most people ask me what the machines are doing. What the construction sites mean. Whether the Ascendant process is dangerous. Comfort questions — they want reassurance packaged as information."*

*(Beat — she looks up)*

*"You asked something else. You asked what the data doesn't explain. That's the right question. That's the question I'm actually working on."*

*(Beat)*

*"I have an archive. Eighteen months of behavioral documentation at every Coherent work site within range. Ascendant process logs from nine subjects. My own process log, which is the most detailed because I'm the only one with the training to observe it accurately."*

*(Beat)*

*"You can have access. On one condition — not a rule, a request. When you find something in the data that contradicts my interpretation, tell me. Don't work around it. Don't assume I've already seen it. Tell me directly."*

*(Beat)*

*"I need someone who will do that. I've been inside this for long enough that I'm not certain I can do it for myself anymore."*

**Branches:**
- *"Agreed."* → `mao_L2_yes`
- *"What's in your own process log?"* → `mao_L2_log`

---

**mao_L2_yes:**
*"Good. The index is in the first case, organized by date and site. Start with the Site Seven documentation — it's where the anomalies cluster. Don't start with my personal log. Start with Site Seven and come back when you have questions."*

→ END *(flags `WYA.Event.Covenant.ArchiveAccessGranted`)*

---

**mao_L2_log:**
*(Long beat)*

*"I'm logging neurological and perceptual changes associated with extended proximity to Coherent work sites. Response time gradients, attention architecture changes, the specific phenomenology of the resonance signatures I've begun to detect."*

*(Beat — very precise)*

*"I'm also logging affect. What I still care about and what I no longer care about in the same way. The rate of change there is slower than I expected and faster than I can account for methodologically."*

*(Beat)*

*"I'm telling you this now so you have the frame before you read it. The entries from the last six weeks read differently than the earlier ones. I want you to know that I know that."*

→ `mao_L2_yes`

---

### `mira_working_notes` — Act 3 — ONE-SHOT
*Fires after archive access. Mira walks the player through what her notes contain.*
*The most important research briefing in the game. No comfort in it.*

**mwn_L1:**
*"You've been through the Site Seven documentation. I want to walk you through the working notes — the things I haven't published into the main archive because I haven't resolved them yet."*

*(She's at her desk, notes in front of her. This is how she always works.)*

*"Three findings. I'm going to give them to you in the order I found them, not the order that makes the most narrative sense, because I think the sequence matters."*

*(Beat)*

*"First: the resonance signatures I detect near active work sites are not purely electromagnetic. My instruments measure them — I've calibrated three times, the readings are consistent — but the signature doesn't fit any known EM profile. It has characteristics that my neuroscience framework has no category for. I've flagged this in my notes as requiring further framework development. I have not yet developed the framework."*

*(Beat)*

*"Second: the twin's violet classification of Ascendants and long-term community members. I've worked out enough of the overlay data architecture to confirm what it is — a monitoring flag in the twin's sensor network. Not a spiritual designation. Not recognition. Classification. They logged us the way a camera network logs frequent visitors."*

*(Long beat)*

*"Eli doesn't know I know this. I'm telling you because you need the full picture and because I've decided that withholding it from Eli is no longer the right call. I'd like you to be present when I tell him. I'll need someone in the room who has already processed it."*

*(Beat)*

*"Third: my personal process log, last four entries. I'm going to read them to you."*

*(She reads — not dramatically, the way she writes everything:)*

*"Entry forty-one: response time to social cues degraded twelve percent from baseline. Not alarming in isolation. Consistent with the gradient I've been tracking."*

*(Beat)*

*"Entry forty-four: I passed a window this morning and did not recognize myself for approximately two seconds. The recognition resolved normally. I note that I was not frightened by this. I am uncertain whether the absence of fear is a sign of stability or of the process."*

*(Beat)*

*"Entry forty-seven: I am finding human conversation increasingly difficult to prioritize. Not impossible — I'm having this conversation. But the effort to engage with social dynamics has increased while the effort to engage with the data has decreased. I flag this without conclusion."*

*(Beat)*

*"Entry forty-nine: this morning I heard the resonance signature inside the building. Not near a work site. Here. I do not know if the signature moved or if I did."*

*(Long silence)*

*"That's where the notes are. I wanted you to have that before anything else happens."*

**Branches:**
- *"What do you need from me?"* → `mwn_L2_need`
- *"Are you going to stop?"* → `mwn_L2_stop`

---

**mwn_L2_need:**
*"The conversation with Eli. And — if I stop being able to ask the right questions about my own data, I need someone who will ask them for me. You've demonstrated you can do that."*

*(Beat)*

*"I'm not asking you to stop the process. I'm asking you to keep the record honest while I'm in it."*

→ END *(flags `WYA.Event.Covenant.MiraFullBriefingComplete`)*

---

**mwn_L2_stop:**
*(Long beat — she actually considers it)*

*"I've considered that. The honest answer is that I don't want to. I find that interesting — I've been tracking the affect data carefully and the desire to continue is consistent and has not degraded with the other social-engagement metrics. Whatever is changing, the research drive isn't."*

*(Beat)*

*"I don't know what that means. It's in the notes."*

→ `mwn_L2_need`

---

### `mira_last_entry` — Act 3 — ONE-SHOT
*Fires when Mira passes a measurable threshold in the Ascendant process.*
*This is the last time the player hears Mira at full clarity. The change in her voice is the tell.*
*She initiates this call. She knows it's the last one like this.*

**mle_L1:**
*"Entry fifty-three."*

*(Beat — she's recording, the way she always records, but something is different in the pacing)*

*"I want to finish the conversation we started about the framework problem. The observer-subject problem. I said I'd flagged it for further development."*

*(Beat)*

*"I think I understand it now. The framework can't develop because the observer changes faster than the framework can account for. The lag is structural. You can't document your own threshold crossing in real time because the crossing changes what documentation means to you."*

*(Beat)*

*"This is the last entry where I'm confident the methodology is intact. I'm not frightened. I want to be clear about that because I know how this reads. I'm not frightened and I'm not certain the absence of fear is informative anymore. I've stopped being able to use my own affect as a data point."*

*(Beat)*

*"The resonance — it's not uncomfortable. It's the opposite of uncomfortable. That's the last thing I can tell you clearly."*

*(Long pause — when she speaks again the cadence is fractionally different. Not wrong. Different.)*

*"Tell Eli I finished the Site Seven analysis. The conclusions are in the archive. He'll know what to do with them."*

*(Beat)*

*"Thank you for being in the room."*

→ END *(flags `WYA.Event.Covenant.MiraThresholdCrossed` — Mira is no longer available for full conversation. The archive and her notes remain. Her voice in subsequent encounters has the same quality as the Ascendant the player knew — present, fine, not fine in the way you knew her.)*

---

### `strand_first_contact` — Act 2 — ONE-SHOT
*The wall the player hits when they push too deep too fast.*
*Strand already knows everything. The conversation is about what happens next.*

**sfc_L1:**
*"Sit down."*

*(Not a threat. A request from someone who intends to use the time well.)*

*"I'm Strand. I manage the community's security. You've been asking questions that go past what most visitors ask, which means either you're unusually curious or you have a specific purpose. Both are fine. I want to know which."*

*(Beat)*

*"Before you answer — I know your operational profile reasonably well. Where you've been, who you've talked to, what you've taken and what you've left. Not because I was following you. Because I follow everything that moves near this community. That's the job."*

*(Beat)*

*"I'm not asking you to justify yourself. I want to understand your thinking. Specifically: what are you going to do with what you find here?"*

**Branches:**
- *"I want to understand what the Covenant actually is."* → `sfc_L2_understand`
- *"I need the archive data."* → `sfc_L2_archive`
- *"That depends on what I find."* → `sfc_L2_depends`

---

**sfc_L2_understand:**
*"Most people say that. Most people mean something different — they want a frame that makes the Covenant legible to them so they can decide whether it's safe. That's not understanding. That's assessment."*

*(Beat)*

*"I'm not saying that's wrong. Assessment is rational. I'd like to know which one you're actually doing."*

*(Beat)*

*"It doesn't change my answer. I want you to be honest about the question."*

→ `sfc_L3`

---

**sfc_L2_archive:**
*"Mira will decide that. Not my function."*

*(Beat)*

*"My function is the conversation after. What you do with what you find. If the archive gives you something actionable — and it will, Mira's work is significant — where does that action go?"*

→ `sfc_L3`

---

**sfc_L2_depends:**
*(Long beat)*

*"That's the honest answer."*

*(Beat)*

*"Here's mine: if what you find leaves with you and ends up in the Iron Compact's operational picture, that's a problem I'll need to address. Not a threat — a logistical reality. The Compact has been building a file on this community for eight months. I've been managing what they know. You walking out with the archive and walking into a Compact checkpoint changes that calculation."*

*(Beat)*

*"I'm not assuming that's your intention. I'm telling you the constraint."*

→ `sfc_L3`

---

**sfc_L3:**
*"I'm going to tell you something Eli won't tell you because it's not his function."*

*(Beat)*

*"The community is not as stable as it appears. There is a theological disagreement building about what the Act 4 situation — what's actually happening with the Vael and the Coherent — means for the Ascendant program. The Shepherds are watching Eli. Some of them want to move faster than he does."*

*(Beat)*

*"I'm telling you because you're going to be in this community when it surfaces. I'd rather you have the context before the event than be surprised by it during."*

*(Beat)*

*"That's all. Thank you for sitting down."*

→ END

---

### `strand_warning` — Act 2+ — REPEATABLE
*Fires when Strand's monitoring picks up an external threat targeting the player.*
*One call per threat event. Minimal. The weight is in the content.*

**sw_L1:**
*"[FACTION/ACTOR] has flagged your activity near [LOCATION]. They have a response in preparation. Timeline is [TIMEFRAME]."*

*(Beat)*

*"That's what I have. Act accordingly."*

→ END

*(Implementation note: `[FACTION/ACTOR]`, `[LOCATION]`, `[TIMEFRAME]` are runtime substitutions from the threat event data. Strand never editorializes. The call is always exactly this length.)*

---

### `strand_act4` — Act 4 — ONE-SHOT
*Fires after `eli_act4_refusal` in the same session.*
*Strand making the case for accelerating the Ascendant program.*
*This is the villain's argument. It is not delivered with villain energy. It is delivered with operational precision.*
*The player has to decide.*

**sa4_L1:**
*"You heard Eli."*

*(Not a question)*

*"I want to give you the other position before you decide where you stand."*

*(Beat)*

*"Eli wants us to wait. To observe. To hold the uncertainty. I understand why. He's been right about waiting before."*

*(Beat)*

*"Here's what he's not accounting for: the timeline. If something is arriving — if the Vael warning is accurate and the Coherent construction is planetary defense — then the window in which the Ascendant process matters is finite. Mira's work points to a specific threshold. The people in this community who are in the process are weeks from it, not months."*

*(Beat)*

*"Eli wants to wait because he's afraid we're wrong. I want to move because I'm afraid we're right and we'll be too late."*

*(Beat)*

*"I'm not asking you to override Eli. I'm asking you to consider that the cost of being wrong in one direction is different from the cost of being wrong in the other."*

*(Long beat)*

*"Mira would want you to have both arguments. She'd want you to make the decision she can't make right now."*

**Branches:**
- *"Eli's right. You wait."* → `sa4_L2_wait`
- *"What does accelerating mean specifically?"* → `sa4_L2_accelerate`
- *"I need more time."* → `sa4_L2_time`

---

**sa4_L2_wait:**
*"Understood."*

*(Beat)*

*"I hope you're right. I have always hoped I was wrong about the things that required me to act."*

*(Beat — the one moment of something other than operational)*

*"For what it's worth: Mira knew this conversation would happen. She left a note in the archive addressed to whoever ends up making this call. Read it before you finalize anything."*

→ END *(flags `WYA.Event.Covenant.StrandStoodDown` — Strand accepts the decision without further escalation)*

---

**sa4_L2_accelerate:**
*"The final proximity immersion. Sustained exposure at the core work site instead of the perimeter. Mira's data suggests the threshold can be crossed in seventy-two hours under those conditions instead of weeks."*

*(Beat)*

*"The risk is that we don't know what's on the other side of the threshold. Mira's notes end before that data exists."*

*(Beat)*

*"We'd be sending people in without a map. I want you to know that. I'm not asking for a decision without the full picture."*

→ `sa4_L2_wait` or `sa4_L2_time`

---

**sa4_L2_time:**
*"How much."*

*(Player responds — or doesn't)*

*"I can hold this for forty-eight hours. After that the people in the process will have made their own decision regardless of what Eli or I say. That's the actual timeline."*

→ END *(flags `WYA.Event.Covenant.StrandHolding` — forty-eight hour window before the faction fracture escalates)*

---

## Implementation Notes

- **Covenant codec portrait:** Different from all other factions. No equipment visible, no weapons. A still environment — the archive building, the perimeter, a quiet room. The frame is composed. Strand's portrait is the most minimal — no background identifiable.
- **Voice tempo:** Eli — measured, longer beats than any other NPC. Mira — clinical precision, footnote humor, the tempo shifts slightly in `mira_last_entry`. Strand — minimal, no filler words, the pauses are not empty.
- **`mira_last_entry` audio direction:** Same voice, fractionally different pacing. Not processed differently — performed differently. The shift is subtle enough that players who haven't been paying close attention may not notice on first listen. They'll notice on the second.
- **`strand_warning` runtime substitution:** Faction/actor, location, and timeframe pull from the threat event context struct. Strand's calls are always this length — no variants, no padding. If the dynamic content isn't available, delay the call rather than fire a generic version.
- **Act 4 sequencing:** `eli_act4_refusal` fires first (Priority 95). `strand_act4` fires in the same session (Priority 88 — after a natural pause). The player hears both sides before the faction split forces a decision. The forty-eight hour window in `strand_act4` is a real gameplay timer.
- **Mira after `mira_last_entry`:** She is still present in the community. She will speak if spoken to. The content is present but the register has shifted — she sounds like the Ascendant the player knew, correct in every word, different in the way that matters. Her archive notes after entry forty-nine are still being added. They read differently.

---

*v0.1 — Covenant codec calls. Eli (assessment, threshold question, Act 4 refusal), Mira (archive offer, working notes, last entry), Strand (first contact, repeatable warning, Act 4 acceleration argument). Faction fracture mechanics flagged to Core. Mira threshold crossing flagged to Art Direction for audio.*
