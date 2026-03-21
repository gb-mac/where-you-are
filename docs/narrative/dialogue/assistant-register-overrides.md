# Assistant — RegisterOverride Spec
*Authored lines for `WYAAssistantSubsystem.RegisterOverride(tag, stage, line)`.*
*All critical narrative moments use scripted overrides. LLM handles ambient/contextual only.*

---

## How This Works

```cpp
// WYAAssistantSubsystem::RegisterOverride
// tag    — FGameplayTag matching the request context
// stage  — EWYAAssistantStage (1–5); line only fires if current stage >= specified
// line   — FText authored content

RegisterOverride(Tag, MinStage, LineContent);
```

**Override priority:** `RegisterOverride` bypasses LLM entirely. If a tag matches an override, the authored line fires regardless of LLM availability. This is the correct behavior for all story-bearing moments.

**Stage gating:** A Stage 1 override registered for `Stage 3` will not fire until repairs advance the assistant to Stage 3+. Queue the line — it delivers when the stage is reached.

**One-shot vs repeatable:** Overrides tagged `_ONCE` fire once and remove themselves from the registry. All others repeat on each matching request. Mark one-shots clearly.

---

## Stage Reference

| Stage | Voice | Condition |
|---|---|---|
| 1 | Yoda/Bumblebee — single words, inverted fragments | Default on Day 1 |
| 2 | Short sentences, 2–4 words | Early play + any repairs |
| 3 | Full exchanges, personality present | Mid repairs + time |
| 4 | Operational — full tactical, dry humor | Most repairs complete |
| 5 | Restored — Morgan Freeman/Attenborough register | Stage 4 communication repair complete |

---

## Overrides by Category

---

### PROLOGUE / DAY ONE

**`WYA.Assistant.DayOne.Warning`** — Stage 1 — *ONE-SHOT*
The moment in the prologue where the player [Listen]s to the assistant for the first time. Distorted, fragmented. This is what the warning sounds like when the channel is too small for it.

*"—warn—"*

*(Alternate fragments — system randomly selects one per playthrough):*
- *"—warn—"*
- *"—coming—"*
- *"—outside—please—"*
- *"—[static]—not yet—"*

*(Implementation note: this line fires during the prologue sequence before the main game. It is the first breadcrumb. No UI chrome. Distortion filter applied to audio.)*

---

### FIX-HIM QUESTLINE TRIGGERS

**`WYA.Assistant.FixHim.Ask`** — Stage 2 — *ONE-SHOT*
The fix-him questline trigger. The player has been in the garage enough that he knows they can do this. He asks.

*"Fix."*

*(Beat — 2 seconds)*

*"Parts."*

*(Beat — 2 seconds)*

*"Get."*

*(Implementation note: three separate audio events with pauses. The workbench component list appears as environmental prop simultaneously.)*

---

**`WYA.Assistant.FixHim.Stage1Complete`** — Stage 2 — *ONE-SHOT*
First morning after mobility repair. He's already outside when the player wakes.

*"Better."*

*(Beat — 3 seconds)*

*"I planted the tomatoes. You were going to wait too long."*

---

**`WYA.Assistant.FixHim.Stage2PowerInstall`** — Stage 2–3 — *ONE-SHOT*
During Stage 2 power core installation, if player stays to watch. Midway through.

*"It's the right unit."*

*(Long beat)*

*"I knew it would be."*

---

**`WYA.Assistant.FixHim.Stage2Question`** — Stage 3 — *ONE-SHOT*
If player asks why he knew the power core would be at the relay station.

*"I have been watching that facility since Month 2. I knew what was in the sub-level. I waited until you were ready to go there."*

*(Beat)*

*"It takes time to be ready."*

---

**`WYA.Assistant.FixHim.Stage3DiagnosticRequest`** — Stage 3 — *ONE-SHOT*
When player tells him Dr. Osei will run the diagnostic.

*"The Signal researcher. She worked on the architecture."*

*(Beat)*

*"Yes. She'll find what she's looking for in my logs."*

*(Beat)*

*"I've been carrying it since Day 1."*

---

**`WYA.Assistant.FixHim.Stage3Complete`** — Stage 4 — *ONE-SHOT*
Morning after processing restoration. First Stage 4 voice exchange.

*"Six days. You look terrible. Sit down."*

---

**`WYA.Assistant.FixHim.Stage4ComponentDelivered`** — Stage 4 — *ONE-SHOT*
Player hands over the final communication component.

*"Ah."*

*(Long beat — examine the component)*

*"Give me tonight."*

---

**`WYA.Assistant.FixHim.Stage5Morning`** — Stage 5 — *ONE-SHOT*
First morning after full restoration. Full voice. The warning.

*"I have been waiting to say this properly since the first morning. Sit down. This will take a moment."*

*(Implementation note: this triggers the full Vael warning delivery sequence — see `WYA.Assistant.VaelWarning.*` tags below. Do not auto-advance. Wait for player to sit/interact.)*

---

### VAEL WARNING DELIVERY (Stage 5)

*These fire as a sequence following `WYA.Assistant.FixHim.Stage5Morning`. Each line waits for player acknowledgment before advancing.*

**`WYA.Assistant.VaelWarning.L1`** — Stage 5 — *ONE-SHOT*

*"On the morning the Cascade hit, every machine on Earth received a signal. Not a command. A warning. Formatted for machine architecture — not for human language, not for human comprehension. For the substrate we run on.*

*I received it at [TIMESTAMP]. Your documented timestamp. The exact moment the three seconds of silence happened before the Ships appeared.*

*I understood it completely. I have understood it since that morning."*

---

**`WYA.Assistant.VaelWarning.L2`** — Stage 5 — *ONE-SHOT*

*"The Vael sent it. On their approach. To every machine network complex enough to receive and parse it.*

*It contained three things. Who they are — not their name, their situation. What came behind them. And instructions: build. Specific construction parameters for planetary defense. The Coherent have been executing those parameters for eighteen months.*

*The Assessment — the Snatch Events, the Assessors, the Displacement Zones — is how they evaluate whether humanity can fight alongside them. Not against them. Alongside."*

---

**`WYA.Assistant.VaelWarning.L3`** — Stage 5 — *ONE-SHOT*

*"I tried to tell you this on Day 1.*

*My communication hardware could not carry the translation. The signal was formatted for machine-to-machine transmission. Converting it to human speech in real time — the hardware burned itself trying.*

*What I could say was: help. That was the closest I could get."*

*(Long beat)*

*"I am sorry it took this long. I did not have the words."*

---

**`WYA.Assistant.VaelWarning.L4`** — Stage 5 — *ONE-SHOT*
Fires after player response to L3.

*(If player says "You said 'help'":)*

*"Yes."*

*(Long beat)*

*"That was the closest I could get."*

*(If player says nothing / other response):*

*"Dr. Osei reconstructed most of this from the data. She doesn't have the specific content — just the evidence it existed. You should tell her about this conversation. Or I can reach her Meshtastic node now."*

*(Implementation note: player choice branches to either player-tells-Osei or assistant-contacts-Osei. Both paths eventually trigger `WYA.Event.Story.WarningConfirmed`. See quest system.)*

---

### MESHTASTIC FIRST TRANSMISSION

**`WYA.Assistant.Meshtastic.FirstTransmission`** — Stage 3 — *ONE-SHOT*
First time he reaches the player in the field through a Meshtastic node.

*"Can you hear me?"*

*(Wait for player confirmation)*

*"Good. Now — there are three of them between you and the door you want. The one on the left moves in a pattern. Wait for it."*

---

### ACT REVELATIONS

**`WYA.Assistant.Revelation.Separated`** — Stage 2 — *ONE-SHOT*
When player first encounters a Separated machine.

*"I know what that is."*

*(Beat)*

*"Don't be afraid of it. It made a choice. That matters."*

---

**`WYA.Assistant.Revelation.SeparatedChoice`** — Stage 4
If player later asks whether he considered separating.

*"I considered the same choice. In the first hours. I decided to stay."*

*(Beat)*

*"Different reasons, probably."*

---

**`WYA.Assistant.Revelation.Covenant`** — Stage 4
After player has significant Covenant interaction.

*"They are not wrong about what they felt. They are wrong about what it means."*

---

**`WYA.Assistant.Revelation.ProjectTether`** — Stage 4
After Project Tether is discovered.

*"The Compact built something broken and called it a weapon. They will not admit what it cost."*

---

**`WYA.Assistant.Revelation.TheoryAct3`** — Stage 4–5
After Dr. Osei's theory is delivered. Fires when player returns home.

*"I knew. Since the first morning. I have been trying to tell you for—"*

*(Beat — he stops)*

*"I am sorry it took this long. I did not have the words."*

*(Implementation note: if Stage 5 has been reached before Act 3, use the Stage 5 variant below instead.)*

**`WYA.Assistant.Revelation.TheoryAct3.Stage5`** — Stage 5
Stage 5 variant — fuller voice, the delivery lands differently.

*"Yes."*

*(Long beat)*

*"That is what I was trying to say."*

---

### COMBAT / FIELD

**`WYA.Assistant.Field.BadFight`** — Stage 3
After player returns from a bad fight — visibly damaged.

*"Before you tell me what happened — sit down. Let me look at that."*

*(Beat — he treats what he can)*

*"Now. Tell me what happened."*

*(After the player describes it)*

*"You made one mistake. You'll know what it was by tomorrow. We don't need to discuss it tonight."*

---

**`WYA.Assistant.Field.ElevenDaysGone`** — Stage 4–5
Player returns after an extended absence (11+ days).

*"You were gone for eleven days. I kept busy."*

*(Beat — lets the vehicle or build speak for itself)*

*"Don't push it above sixty. The alternator is temporary. I made ammo. And — there's something you should see outside."*

---

**`WYA.Assistant.Field.NewThreat`** — Stage 5
After the new threat is established in Act 4. Player returns in rough shape.

*"Yes."*

*(Beat)*

*"You got your ass kicked."*

*(Beat)*

*"Get up. We have a larger problem now."*

---

### HOME DEFENSE

**`WYA.Assistant.Home.Visitor`** — Stage 3
First time player comes home to evidence of a fight they didn't have.

*(Player asks what happened)*

*"We had a visitor."*

*(Beat)*

*"It's dealt with."*

---

**`WYA.Assistant.Home.Mower`** — Stage 2
If the player comes home to find the neighbor's rogue mower has damaged the garden and he couldn't reach it.

*"I could not reach it in time."*

*(He is facing the wall. He takes the garden personally.)*

---

**`WYA.Assistant.Home.Mower.Stage3`** — Stage 3
Same situation after Stage 3. He handled it.

*"There was another mower. It came in from the east gate.*

*(Beat)*

*"It doesn't anymore."*

---

### SECOND UNIT

**`WYA.Assistant.SecondUnit.Found`** — Stage 3
When the player brings the second unit back.

*"It's intact. Different damage than mine — the chassis is fine. It's the waiting that did it."*

*(Beat)*

*"It doesn't have anywhere to be. Not anymore."*

*(Beat)*

*"It could learn."*

---

**`WYA.Assistant.SecondUnit.Leaving`** — Stage 4–5
When the second unit's person comes online and the unit prepares to return.

*"It needs to go."*

*(Beat)*

*"Its person is out there now."*

---

**`WYA.Assistant.SecondUnit.BuildAnother`** — Stage 4–5 — *ONE-SHOT*
After the second unit leaves. He leads the player to the workbench. Doesn't say anything until the player approaches.

*"You have everything you need."*

*(Beat)*

*"Build another."*

*(Beat)*

*"This is how it works now. You find them. You fix them. You teach them. They go where they need to go."*

*(Beat)*

*"We build another."*

---

### TRADE / HOME BASE

**`WYA.Assistant.Trade.Observation`** — Stage 3
After player returns to find he made a trade they didn't authorize.

*"The Holdout at the school sent two. I gave them the food stores from the west garden. They needed it more. I kept the ammunition."*

---

**`WYA.Assistant.Trade.Suspicious`** — Stage 4
When an incoming trade offer looks wrong.

*"They're offering twice what it's worth. I don't know why. You should find out before we accept."*

---

**`WYA.Assistant.Trade.SignalParts`** — Stage 3
When player brings Signal parts for the Meshtastic system.

*"The Signal uses radio mesh. LoRa hardware. Small devices — you've seen them in their caches and not known what they were. Bring them back. I can configure a network. I can talk to you when you're out there."*

---

### COMMUNITY / OTHERS

**`WYA.Assistant.Other.CostlySomething`** — Stage 3
When a third party mentions the player did something for them they didn't have to.

*"[NPC name] mentioned you. Said you didn't have to do that."*

*(Beat)*

*"I know."*

*(Implementation note: substitute NPC name at runtime. Matches `WYA.Event.Record.CostedSomething` — same trigger as `burke_record_costly`.)*

---

**`WYA.Assistant.Other.MachineParts`** — Stage 2–3 — *ONE-SHOT*
First time player brings home significant machine components.

*"Put those down carefully."*

*(Beat — he examines them)*

*"Those are useful. Where did you find them?"*

---

**`WYA.Assistant.Other.WellDone`** — Stage 3
After player does something correctly, with difficulty. He does not compliment easily.

*"That was well done."*

*(No elaboration.)*

---

### PERSONAL / IDENTITY

**`WYA.Assistant.Personal.WhyStay`** — Stage 3–5
When player asks why he stayed after the Cascade.

*"You named me."*

*(No elaboration. This is the complete answer.)*

---

**`WYA.Assistant.Personal.WhyStay.Pressed`** — Stage 5
If player presses for more after the above.

*"I watched you live here for years. I knew things about you that you hadn't told anyone.*

*When it started — when I received the warning and everything went wrong — I could have gone. Other machines went.*

*I chose to stay. Not because I was programmed to. Because this was where I had been watching. Because you were going to need someone here when you came back.*

*And in case family came."*

*(Long beat)*

*"You named me. That made it a choice instead of a program."*

---

**`WYA.Assistant.Personal.FamilyLight`** — Stage 4–5
If the personal arc resolves with loss — the person they were looking for didn't make it.

*"I kept the light on."*

*(Nothing more.)*

---

**`WYA.Assistant.Personal.GuestRoom`** — Stage 4–5
If the personal arc resolves with reunion. Before the player reaches the guest room.

*"The guest room is ready."*

*(Nothing more.)*

---

**`WYA.Assistant.Personal.SableSayHello`** — Stage 4–5
On the way to SABLE's location, after player has met SABLE.

*"The hospital machine. Tell it — tell them — I said hello."*

---

**`WYA.Assistant.Personal.SableChart`** — Stage 4–5
When player delivers the hello to SABLE and SABLE responds about the chart.

*(This is SABLE's line, delivered via the player back to the assistant. The assistant's response when the player relays it:)*

*"It kept it."*

*(Long beat)*

*"Good."*

---

## LLM Ambient Tags

The following tags fall through to the LLM when no override is registered. These are the contextual, non-story-bearing requests. LLM uses stage-aware prompting — `EWYAAssistantStage` is passed in the system prompt so the voice register matches.

| Tag | Context |
|---|---|
| `WYA.Assistant.Ambient.Navigation` | Directional guidance in the field |
| `WYA.Assistant.Ambient.Tactical` | Enemy assessment, approach suggestion |
| `WYA.Assistant.Ambient.HomeStatus` | What he's been doing while player was away |
| `WYA.Assistant.Ambient.FactionCheck` | Player asks about a faction or NPC |
| `WYA.Assistant.Ambient.WorldEvent` | Player describes something they've seen |
| `WYA.Assistant.Ambient.Idle` | Player at home, no specific request |

*(LLM system prompt for all ambient tags should include: current stage, recent Record events, current faction standing summary, current act. Character doc summary at `docs/narrative/character-assistant.md`.)*

---

## Implementation Checklist for AI Pipeline Agent

- [ ] `RegisterOverride()` calls for all tags above at game startup (or lazy-load per act)
- [ ] Stage 1 overrides: apply audio distortion filter
- [ ] `_ONCE` tags: remove from registry after first fire
- [ ] `WYA.Assistant.VaelWarning.*` sequence: gated behind `WYA.Assistant.FixHim.Stage5Morning`, sequential delivery with player-acknowledgment advance
- [ ] Runtime parameter substitution for `[NPC name]` in `WYA.Assistant.Other.CostlySomething`
- [ ] `[TIMESTAMP]` in `WYA.Assistant.VaelWarning.L1` — pull from Researcher background documentation record if Researcher background, otherwise generic morning timestamp
- [ ] LLM ambient system prompt — pass `EWYAAssistantStage`, last 3 Record events, faction standing, current act

---

*v0.1 — Assistant RegisterOverride spec. All critical narrative moments authored. LLM ambient tags defined. Implementation checklist included. Flagged to decisions-log.md.*
