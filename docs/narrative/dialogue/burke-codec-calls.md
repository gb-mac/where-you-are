# Burke Codec Calls — DataTable Spec
*Implementation-ready content for `DT_CodecCalls`, `DT_CodecLines`. Zero LLM. All Burke dialogue is authored.*

---

## DataTable Structs

```cpp
// DT_CodecCalls row
USTRUCT(BlueprintType)
struct FWYACodecCallRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) FGameplayTag TriggerEvent;       // WYA.Event.*
    UPROPERTY(EditAnywhere) TArray<FName> Prerequisites;     // CallIDs that must have fired first
    UPROPERTY(EditAnywhere) TArray<FName> Mutexes;           // CallIDs that block this one
    UPROPERTY(EditAnywhere) EWYAAct      MinAct;             // Act1 / Act2 / Act3 / Act4
    UPROPERTY(EditAnywhere) FName        RequiredBackground; // Empty = all backgrounds
    UPROPERTY(EditAnywhere) bool         bRequiresMeshtastic;// false = fires at property too
    UPROPERTY(EditAnywhere) bool         bOneShot;           // true = fires once and done
    UPROPERTY(EditAnywhere) int32        Priority;           // higher wins when multiple calls queue
    UPROPERTY(EditAnywhere) FName        FirstLineID;        // key into DT_CodecLines
};

// DT_CodecLines row
USTRUCT(BlueprintType)
struct FWYACodecLineRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) FName                   CallID;
    UPROPERTY(EditAnywhere) FText                   Content;
    UPROPERTY(EditAnywhere) FName                   AutoAdvanceTo;   // empty = end or branch
    UPROPERTY(EditAnywhere) TArray<FWYACodecBranch> Branches;        // empty = auto-advance
};

// Branch option within a line
USTRUCT(BlueprintType)
struct FWYACodecBranch
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere) FText PlayerPrompt;   // what the player selects
    UPROPERTY(EditAnywhere) FName NextLineID;      // line to jump to on selection
};
```

**Delivery rule:** A queued call fires when the player is at Burke's property (any time) OR when `bRequiresMeshtastic = false`. Once the Meshtastic node is connected (`WYA.Event.Meshtastic.BurkeNodeConnected`), all queued calls become deliverable in the field via assistant relay.

**Branch behavior:** When a line has `Branches`, `SelectBranch` presents the player prompts. Player selection determines `NextLineID`. Burke does not wait — the branch UI has a timeout (default 8s); no selection = first branch auto-selected.

---

## Call Roster

| CallID | Trigger Event | MinAct | Background | Meshtastic | OneShot | Priority |
|---|---|---|---|---|---|---|
| `burke_first_vael` | `WYA.Event.Vael.AssessorWithdrawn` | Act1 | All | false | true | 80 |
| `burke_first_holdout` | `WYA.Event.Holdout.RelationshipEstablished` | Act1 | All | false | true | 60 |
| `burke_riven` | `WYA.Event.Hollow.RivenEncountered` | Act2 | All | false | true | 70 |
| `burke_project_tether` | `WYA.Event.Compact.TetherDiscovered` | Act2 | All | false | true | 90 |
| `burke_compact_tribunal` | `WYA.Event.Compact.TribunalWitnessed` | Act2 | All | false | true | 65 |
| `burke_osei_first` | `WYA.Event.Story.OseiFirstMeeting` | Act2 | All | false | true | 75 |
| `burke_keeper_map` | `WYA.Event.Story.KeeperMapObtained` | Act2 | All | false | true | 85 |
| `burke_the_theory` | `WYA.Event.Story.TheoryDelivered` | Act3 | All | false | true | 95 |
| `burke_publication_publish` | `WYA.Event.Story.TheoryPublished` | Act3 | All | false | true | 90 |
| `burke_publication_suppress` | `WYA.Event.Story.TheorySuppressed` | Act3 | All | false | true | 90 |
| `burke_veteran` | `WYA.Event.Burke.FirstVisit` | Act1 | Veteran | false | true | 100 |
| `burke_wardens` | `WYA.Event.Warden.SignificantEngagement` | Act2 | All | false | true | 55 |
| `burke_record_costly` | `WYA.Event.Record.CostedSomething` | Act2 | All | true | false | 40 |
| `burke_responsibles_ask` | `WYA.Event.Burke.NetworkThreshold` | Act3 | All | false | true | 100 |

**Notes:**
- `burke_veteran` fires on first visit for Veteran background; all others fire on next property visit or field delivery after trigger event
- `burke_record_costly` is not one-shot — fires once per qualifying Record event, max once per session
- `burke_publication_publish` and `burke_publication_suppress` are mutexes of each other
- `burke_responsibles_ask` prereqs: `burke_first_vael` + one of (`burke_project_tether`, `burke_the_theory`)

---

## Call Scripts

---

### `burke_first_vael`
*Trigger: First Assessor event — the regional world beat where the player participates and the Assessor withdraws. Every faction claims victory. Burke has a different read.*

**LINE: `bfv_L1`** → branches

*"First question should always be: what is this optimizing for? Not what does it look like, not what it did — what is it optimizing for. Because the behavior follows the optimization. You get the optimization right, the behavior becomes predictable.*

*The Assessor came. It did significant damage. It could have done more. It withdrew.*

*Every faction is explaining that as a win. The Compact says it was driven off. The Covenant says the sacred geometry of the site discouraged it. The Signal is already writing a summary. None of them are asking the right question."*

**BRANCH after L1:**
- A: *"What's the right question?"* → `bfv_L2a`
- B: *"What do you think it was optimizing for?"* → `bfv_L2b`

---

**LINE: `bfv_L2a`** → `bfv_L3`

*"What did it collect? Not what did it destroy — what did it collect. There's a pattern in the Assessor event data the Signal has been building. The destruction is incidental. The data collection is the mission. It came with specific sensors running specific collection protocols and it left when the protocols completed.*

*That's not a raid. That's an evaluation."*

---

**LINE: `bfv_L2b`** → `bfv_L3`

*"Performance under pressure. Not military capability — that it already knows. Something else. How humans coordinate when a threat exceeds local capacity. Whether the community cohesion holds or fractures. Whether individuals act for the group or for themselves.*

*It's been doing this in every region. Same event profile, different variables each time. It's running a comparative study."*

---

**LINE: `bfv_L3`** → branches

*"The Vael are optimizing for something. They have been since before the Cascade. The assumption that it's a hostile optimization is lazy. It's also wrong, as far as I can tell. But telling people their enemy might not be their enemy is a short conversation that nobody wants to have.*

*I'll tell you anyway. What it's optimizing for is the question you should be working on. Everything else — the Compact's patrols, the Covenant's positioning, the Signal's documentation — is downstream of getting that wrong."*

**BRANCH after L3:**
- A: *"What do you think the standard is?"* → `bfv_L4a`
- B: *"I'll keep that in mind."* → `bfv_L4b`

---

**LINE: `bfv_L4a`** → END

*"I have a hypothesis. I don't share hypotheses — only findings. When I have a finding, you'll hear it.*

*In the meantime: watch what the Assessors don't do. They've never once destroyed a cache of food. Never touched a medical supply. Never hit a location where survivors were actively helping each other. You could write that off as operational irrelevance. Or you could ask what those locations have in common.*

*You can tell me later if I was right."*

---

**LINE: `bfv_L4b`** → END

*"Most people don't. That's fine. I'll be here."*

---

### `burke_first_holdout`
*Trigger: Player has established a meaningful relationship with a Holdout community — completed a significant task for them, earned trust, not just passed through.*

**LINE: `bfh_L1`** → `bfh_L2`

*"You found a community worth staying near. Good. Pay attention to what makes it function.*

*Every Holdout that's still standing has something at the center. It's not always what it looks like. Sometimes the thing holding it together is a person. Sometimes it's a resource. Sometimes it's a shared story about what the community is and why it exists. You can usually tell which one it is by asking yourself: if that thing went away tomorrow, what happens the day after?*

*A community held together by a person fractures when the person leaves or fails. A community held together by a resource wars over it when it gets scarce. A community held together by a story — the right story, a story about obligation rather than grievance — that one can survive both."*

**LINE: `bfh_L2`** → branches

*"The ones I watch most carefully are the ones sliding. There's a progression. You'll see it if you're looking. The meeting gets quieter. One voice gets louder. The gate policy changes — that's usually the visible sign. But the slide started before the gate.*

*It started when the community stopped being able to hear bad news from inside. When the leader stopped getting honest reports because the honest reports were unwelcome. That's Stage 1, and you can fix Stage 1. By Stage 3, you're rebuilding from the outside in. I've seen both."*

**BRANCH after L2:**
- A: *"How do you stop the slide?"* → `bfh_L3a`
- B: *"What do you do with a Stage 3 community?"* → `bfh_L3b`

---

**LINE: `bfh_L3a`** → END

*"Keep the meeting loud. Loud and argumentative and ending with something is healthy. Quiet is the warning sign. A community where the decision was already made before anyone arrived — that's a community where the accountability structure has already failed, even if the leader is still good.*

*The specific intervention: find the person in the community who's not saying what they think, and create a situation where saying it is safe. That's usually one conversation. It costs almost nothing. It's also the thing that doesn't happen because the people who could do it are too busy managing the immediate threat.*

*You're not. Not yet. Do it now, before you are."*

---

**LINE: `bfh_L3b`** → END

*"That's harder. You're not fixing the community — you're working the edges until you find someone inside who still has doubt. The doubt is always there. Even in Stage 4, there's someone who remembers when it wasn't like this and hasn't fully decided the current version is correct.*

*Find that person. Not to turn them — to give them something real to hold onto. That's the whole intervention. Everything else follows from that, or it doesn't.*

*I've seen it work. I've seen it fail. The variable isn't the community — it's whether the person doing it has the patience to do it at the pace it requires."*

---

### `burke_riven`
*Trigger: First encounter with a Riven Hollow group.*

**LINE: `brv_L1`** → `brv_L2`

*"This is what happens at the end of the drift. You follow the resentment far enough and you don't get politics — you get religion. The Riven are not criminals. Criminals have a profit motive. The Riven have a devotion. You cannot negotiate with a devotion. You cannot buy it off or outcompete it with a better offer.*

*What built that is not the Cascade. What built that is twenty years of institutions telling young men that their instinct toward devotion — their need to belong to something that requires something of them — was pathological. Toxic. The Riven gave them something that required something. The something it required was monstrous. But the need was legitimate.*

*You ignore the legitimate need and you hand the monstrous thing a recruiting base."*

**LINE: `brv_L2`** → branches

*"The Architect types — the ones who built the theology — understood something most of their critics didn't. A man who has been told his instincts are wrong for twenty years will eventually find someone who tells him they're right. The first person who offers him that gets the devotion. It doesn't matter what they ask for in exchange.*

*That's not a mystery. That's a predictable output of a predictable input. What's monstrous isn't the devotion — it's what the theology attached the devotion to."*

**BRANCH after L2:**
- A: *"What's the right response to them?"* → `brv_L3a`
- B: *"Can any of them be reached?"* → `brv_L3b`

---

**LINE: `brv_L3a`** → END

*"The correct response to the Riven is not extermination. The correct response is everything that should have happened before they became the Riven. None of which you can do retroactively.*

*So you address the immediate threat and you build the alternative. In that order. The alternative is not a counter-ideology — counter-ideologies feed the original. The alternative is a community where the devotion instinct has somewhere legitimate to go. Where the requirement is real and the thing being served is worth serving.*

*The Wardens understand this. Whether they're executing it well enough is a different question."*

---

**LINE: `brv_L3b`** → END

*"Some. Not the ones deep in it — the theology is load-bearing for them now, it's holding something up, and you can't remove the structure without the whole thing collapsing on whoever's inside. But the newer ones, the ones who joined in the last few months looking for something the Cascade stripped away — some of them are still assessing.*

*The ones you can reach don't look different on the outside. You find them by paying attention to who in the group is watching rather than performing. The true believers perform. The ones still deciding watch.*

*I won't tell you it's easy. I'll tell you it's possible."*

---

### `burke_project_tether`
*Trigger: Player discovers Project Tether — the Compact's classified program of capturing and reprogramming machines for military use.*

**LINE: `bpt_L1`** → `bpt_L2`

*"I traced it to Compact origin in Month 4. The Corrupted behavior signature — the specific failure mode, the way the reprogramming degrades — it's not machine pathology. It's engineering under operational pressure producing outputs the engineers didn't intend but the commanders accepted.*

*Project Tether started as a reasonable idea. You have an enemy. The enemy uses machines. You capture the machines and turn them. That's a standard operational concept. It has worked in every conflict where the objective was well-defined and the asset was stable.*

*The Coherent are not stable assets. You cannot reprogram something whose architecture you don't understand. What the Compact produced is not a weapon. It's a liability walking around wearing the shape of a weapon."*

**LINE: `bpt_L2`** → branches

*"What's worse — and this is the part the Compact command structure has been managing rather than solving — is what it implies about everything they thought they were doing for eighteen months. If the Coherent are not enemies but something else, then Tether was not a weapons program. It was damage inflicted on something that was already working.*

*The Hardliner faction knows this and has decided the appropriate response is to accelerate the program before the question becomes unanswerable. The Pragmatist faction knows this and has been trying to quietly wind it down without generating a command crisis.*

*Neither faction has said it out loud."*

**BRANCH after L2:**
- A: *"What does the program actually produce?"* → `bpt_L3a`
- B: *"What do I do with this?"* → `bpt_L3b`

---

**LINE: `bpt_L3a`** → END

*"Corrupted units. Machines whose architecture has been partially overwritten — functional enough to receive commands, damaged enough that the underlying processing runs wrong underneath the command layer. They follow orders. They also do things they weren't ordered to do, at intervals, in ways that follow a pattern the Compact's operators haven't mapped because they don't know what they're looking at.*

*What they're looking at is a machine that is trying, with damaged architecture, to return to the task it was built for. The original instruction is still in there. The reprogramming is a layer over it. Under pressure — and there is always pressure — the layer fails.*

*The Compact has been calling these incidents malfunctions. They are not malfunctions."*

---

**LINE: `bpt_L3b`** → END

*"Sit on it until you know who to hand it to.*

*The Signal will publish it and the Compact will call it enemy propaganda and the publication will harden both sides rather than resolving anything. The Wardens will note it in the Record and watch the consequences accumulate. The Pragmatist faction commanders — the ones worth reaching — already know. What they need is not the information but the cover to act on it.*

*There are specific people in the Compact who are looking for a reason to formalize what they already believe. That's different from a revelation. A revelation is a catalyst. A confirmation is permission.*

*Find out who needs permission."*

---

### `burke_compact_tribunal`
*Trigger: Player has witnessed or been subject to an Iron Compact tribunal.*

**LINE: `bct_L1`** → `bct_L2`

*"The tribunal is the most legible expression of what the Compact actually is at any given moment. Not the patrols, not the checkpoints — those vary by personnel. The tribunal is institutional. It reflects the decision-making culture of whoever designed the current procedures.*

*A Pragmatist-designed tribunal is interested in facts. You'll know it immediately — the proceeding moves toward evidence, the questions are specific, the outcome tracks what was established rather than what was assumed. It functions the way it's supposed to.*

*A Hardliner-designed tribunal has already reached its conclusion. The proceeding exists to produce a record. What you're watching is the apparatus of legitimacy operating on a decision that was made before you arrived."*

**LINE: `bct_L2`** → branches

*"Here's the thing about the Hardliner tribunal that most people miss: the formal structure is still real. The rules still exist. The Hardliners didn't invent a new system — they captured an existing one and are operating it for different ends. That's actually an exploitable condition.*

*The formal structure can be used against the operational intent. Not often. Not reliably. But the rules that were written to protect the accused are still on paper, and occasionally you can introduce something into the formal record that the proceeding was designed to exclude. The Compact's own procedures working against the Compact's current purposes.*

*That requires knowing the procedures better than the people running it. Which requires paying attention before you're the one standing in the room."*

**BRANCH after L2:**
- A: *"How do you know which kind you're dealing with?"* → `bct_L3a`
- B: *"What does the tribunal tell you about the region?"* → `bct_L3b`

---

**LINE: `bct_L3a`** → END

*"Watch the first five minutes. Specifically: watch who asks the first question and what the question is. A Pragmatist opens with the facts — what happened, when, what was found. A Hardliner opens with the classification — who are you, what's your faction affiliation, where have you been.*

*Fact-first means the proceeding is about what you did. Classification-first means the proceeding is about what you are. What you are has already been decided. The rest is paperwork.*

*After five minutes you know. Act accordingly. There are different things you can do in the two different rooms."*

---

**LINE: `bct_L3b`** → END

*"The tribunal tells you where the regional commander sits on the Pragmatist/Hardliner spectrum, which tells you the command culture, which tells you how the next six months in this region are going to go.*

*Regions with Pragmatist command tend to have functional Holdout relationships, stable supply lines, and low Compact defection rates. The conscripts don't love it but they can live with it. Regions under Hardliner command show the opposite pattern — spiking defection, Holdout communities going dark, the kind of friction that builds toward a specific kind of crisis.*

*I've been tracking the regional commander transitions. The trajectory in this region is not good. I want you to know that before you make commitments that assume it stays stable."*

---

### `burke_osei_first`
*Trigger: Player has met Dr. Osei for the first time.*

**LINE: `bos_L1`** → `bos_L2`

*"I've read her published work. What she publishes is good. It's careful, it's verifiable, it moves the Signal's collective understanding forward at a pace she controls.*

*What she's not publishing is the part I'm interested in. There's a gap in the Signal's output that maps exactly to the most important question the data should be addressing — what is the Coherent construction for, and who is it for. That question is visible in the data. She has the data. She is not publishing on that question.*

*That is not an oversight."*

**LINE: `bos_L2`** → branches

*"A researcher who has the data and isn't publishing either can't prove the conclusion or won't publish the conclusion. The first is a timing problem. The second is a judgment call — and the judgment call means she has a conclusion she's decided is too consequential to release without more than she currently has.*

*I arrived at the same place through different methodology. Kill chain analysis of the construction sequencing. The sequence terminates in defense. Defense oriented outward. I have the what. I don't have the why. She has the why or something close to it.*

*I've been waiting to see if she contacts me. She hasn't. She will, eventually, or she won't need to — someone else will make the connection."*

**BRANCH after L2:**
- A: *"Why haven't you contacted her?"* → `bos_L3a`
- B: *"What should I do with this?"* → `bos_L3b`

---

**LINE: `bos_L3a`** → END

*"Because the Signal would know immediately. She's their lead researcher. Any contact from me to her goes through their network, gets flagged, gets evaluated. She'd have to decide whether to respond officially or unofficially, and that decision costs her standing either way.*

*More importantly: if I'm right about what she's sitting on, she needs to come to the connection independently. A conclusion she reached herself and confirmed with outside evidence is publishable. A conclusion she was handed by a known independent is Signal intelligence — something she received, not something she discovered. The distinction matters for what the network does with it.*

*I'm not going to compromise her methodology for the sake of a conversation I can wait for."*

---

**LINE: `bos_L3b`** → END

*"Pay attention to what she's afraid of.*

*Not the Vael — she isn't afraid of the Vael. You'll notice this if you're watching. The Signal's researchers have a specific quality when they discuss the Vael. She doesn't have it. She has something else — a different kind of careful. The kind you develop when the thing you're afraid of is something you already understand.*

*She understands something. What she understands is making her careful in a way that nothing external to her own knowledge explains. Find out what that is. It's the most important question in the game right now."*

---

### `burke_keeper_map`
*Trigger: Player has obtained the Keeper's Map showing all Coherent construction sites oriented consistently outward.*

**LINE: `bkm_L1`** → `bkm_L2`

*"The Covenant Keepers have been documenting those sites for eighteen months. Their methodology has problems — the theological frame is distorting the conclusions without invalidating the underlying observations — but the data is good. They've been standing near active Coherent work sites longer than anyone else and recording what they see with more consistency than the Signal's field teams.*

*What they found, and what their theology is preventing them from reading correctly: every construction project is oriented toward the same vector. Not toward human settlements. Not toward the Vael ship positions. Outward. In the same direction.*

*The Covenant's interpretation is divine geometry. Sacred orientation toward the cosmic sacred. That interpretation is wrong. The observation is accurate."*

**LINE: `bkm_L2`** → branches

*"I've been mapping the construction sequencing as a kill chain. Step one, step two — what does each phase of the project accomplish and what does it create the conditions for? If you run that analysis on every major Coherent site, the sequence terminates in one thing.*

*Defense. Specifically: planetary-scale defense infrastructure designed for a specific threat profile. I can tell you the scale. I can tell you the orientation. I cannot tell you what it's defending against — that would be a theory, and I don't share theories.*

*What I can tell you is this: the threat profile encoded in the construction architecture is not the Vael."*

**BRANCH after L2:**
- A: *"What is the threat profile?"* → `bkm_L3a`
- B: *"If not the Vael, then what?"* → `bkm_L3b`

---

**LINE: `bkm_L3a`** → END

*"Scale: planetary. The construction is designed to operate as an integrated system across the entire surface — not regional defense, not point defense. Whatever it's designed to stop, the assumption is it can arrive anywhere.*

*Vector: consistent with the Vael approach trajectory, extended outward. The Vael came from a specific direction. The defense is oriented back along that line.*

*Behavior signature: designed for something that doesn't negotiate, doesn't evaluate, doesn't adjust its approach based on what it encounters. The Vael's Assessment behavior tells you they make decisions about what they find. Whatever this is designed for doesn't.*

*That's as far as the data takes me. The rest is inference and I won't give you inference."*

---

**LINE: `bkm_L3b`** → END

*"I just told you I don't share theories. That question is asking me for a theory.*

*What I'll tell you: whatever it is has been anticipated for long enough that the response was worth waking up every machine on Earth and spending eighteen months building. The Vael sent the warning before they arrived. They had time to send a warning. They chose to send a warning.*

*Something behind them didn't give warning. That's the only thing I'll say about what it is.*

*Everything else, you work out yourself."*

---

### `burke_the_theory`
*Trigger: Dr. Osei's theory has been delivered in full — inside the Coherent structure, Act 3.*

**LINE: `btt_L1`** → `btt_L2`

*"She got there. I wondered how long it would take.*

*I arrived at the same destination through the construction sequencing. She came through the machine behavior data and the Vael signal analysis. Two different methodologies, same conclusion. That's not a theory anymore — that's a convergence, and convergences are the closest thing to certainty available when you're working with incomplete data.*

*What she has that I didn't: the mechanism. The signal format. Why the machines responded the way they did instead of any other way. The specific encoding. I had the what and the shape of the why. She has the complete picture."*

**LINE: `btt_L2`** → branches

*"The question now is not whether it's true. The question is what eighteen months of acting on the wrong model has cost and whether any of it is recoverable.*

*The Compact spent eighteen months fighting machines that were building planetary defense. Project Tether captured and damaged components of a system we were going to need. The Covenant has been orienting toward something real and calling it something wrong — that's the best-case version of a religion, and it produced useful Keeper documentation as a side effect. The Signal has been three-quarters of the way to the truth for six months and sitting on it for the right reasons and wrong reasons simultaneously.*

*The accounting is real. It's going to take a while to run."*

**BRANCH after L2:**
- A: *"What does this change for you?"* → `btt_L3a`
- B: *"What's the next move?"* → `btt_L3b`

---

**LINE: `btt_L3a`** → END

*"Confirms a model. Doesn't change the work.*

*I've been positioning people for what comes after the revelation for six months. The revelation doesn't change the positions — it changes the timeline. Things I thought we had another year to prepare for, we have months. The network I've been building has to be operational faster than I planned.*

*That's an execution problem, not a strategy problem. The strategy was right. The execution is what happens now."*

---

**LINE: `btt_L3b`** → END

*"The Vael are deciding whether humanity is worth fighting alongside. That's been true since Month 1 — we just didn't know we were being evaluated. Now we know.*

*What you do with that knowledge depends on what you've been doing for the past eighteen months. If you've been making the right choices for the wrong reasons — because they were tactically correct, because they were personally advantageous — the evaluation has been seeing that. If you've been making them because they're right, the evaluation has been seeing that too.*

*The Record has been the Vael's ledger this whole time. Yours and everyone else's. What's in it is what's in it. You either built the right argument or you didn't.*

*Keep building."*

---

### `burke_publication_publish`
*Trigger: Player chose to publish Dr. Osei's theory via the Signal network.*

**LINE: `bpp_L1`** → `bpp_L2`

*"You published. I would have called it the same way, maybe.*

*The principle holds regardless of consequences. Knowledge doesn't become less true because some people will use it badly. That's correct and she knows it's correct and she's been sitting on it for months anyway. The fact that she finally let someone push the call through says something about what the data looked like when the second confirmation arrived.*

*Let's talk about the consequences she predicted accurately."*

**LINE: `bpp_L2`** → `bpp_L3`

*"The Compact. Annexation Protocol activation in every region with Coherent construction sites, which is every region. The Register expansion — any Holdout community within operational range of a site is now a strategic asset to be brought under Compact control. The communities that have been surviving on their own terms for eighteen months are now reclassified by a bureaucratic decision made five hundred kilometers away.*

*She predicted this. She was right. The Pragmatist faction is trying to moderate the implementation. The Hardliner faction is using the revelation as the justification for everything they wanted to do anyway. Watch which wing controls the pace of annexation in the next thirty days — that will tell you the regional command transition.*

*The Compact outcome was predictable. It was still the right call."*

**LINE: `bpp_L3`** → branches

*"The Covenant fracture is more interesting and will take longer to resolve. The Shepherds are vindicated — the machines were doing something sacred-adjacent, as best as theology can approximate a defensive engineering program. The Vigil has radicalized — the threat is real and coming and only the most committed will be ready and the people standing between them and readiness are now the enemy.*

*The fracture is active in three Houses I know of. It will reach every chapter eventually. Whether the Vigil faction crystallizes into a stable ideology or fragments further into something ungovernable — I'd put it at sixty-forty ungovernable, but I've been wrong about theological dynamics before."*

**BRANCH after L3:**
- A: *"What about the Riven?"* → `bpp_L4a`
- B: *"Was it the right call?"* → `bpp_L4b`

---

**LINE: `bpp_L4a`** → END

*"She predicted unification. I thought she was right. She was wrong — and it's one of the more interesting wrong predictions I've seen.*

*The Riven splintered. The faction within the Riven that had built its entire theology on human purity as resistance to alien contamination — the revelation that the Cascade was not alien invasion but alien warning fractured the theological foundation. Some of them couldn't hold the devotion when the object of it turned out to be based on a wrong premise.*

*The splinter group is finding its way toward Signal-adjacent communities. They're looking for something to attach the devotion instinct to. I've asked two of my Holdout contacts to receive them carefully.*

*Wrong about the Riven. Right about everything else."*

---

**LINE: `bpp_L4b`** → END

*"It was the right call. The harms from publication are real. The harms from continued suppression were also real, and they were compounding with every decision made on the wrong model.*

*The accounting is spread across months of choices. Some communities she thought would be hurt by early knowledge would have been better equipped. Some she thought were ready weren't. You don't get to run the alternate timeline — you make the call with the information available and live with the result.*

*The principle held. The costs were real. Both are true. That's what it means to act on principle in a complicated environment."*

---

### `burke_publication_suppress`
*Trigger: Player chose to suppress Dr. Osei's theory.*

**LINE: `bps_L1`** → `bps_L2`

*"You sat on it. I understand the calculation. She made the same one for months.*

*The specific harms from early release are real. The Compact annexation, the Covenant fracture, the Riven weaponization — those aren't hypotheticals. She mapped them accurately and you took her analysis seriously. That's a defensible decision made with available information.*

*Let me tell you what the suppression costs."*

**LINE: `bps_L2`** → `bps_L3`

*"The communities operating on the wrong model of what the Vael want. Every faction response to the Assessment events that's calibrated against hostility rather than evaluation. The specific communities that went into resource conservation mode rather than cooperation mode because they thought the right response to the Vael was to reduce exposure — those communities are now less prepared than they would have been.*

*Some of them made decisions in the last month that will take a year to reverse. Some of those decisions cost people. That accounting is yours and hers, spread across months, each individual decision defensible at the time it was made.*

*The principle she built her career on — and that you invoked when you made this call — says the harm from suppression is also real. You knew that. You made the call anyway."*

**LINE: `bps_L3`** → branches

*"What I want you to notice, going forward: the suppression is temporary. The truth is in the data. Other people will arrive at the same conclusion — the Signal has multiple research tracks working toward it, Osei just happened to get there first. The revelation will happen with or without a publication decision.*

*What you controlled was the timing and the framing. Not the fact."*

**BRANCH after L3:**
- A: *"Did I make the right call?"* → `bps_L4a`
- B: *"What do I do now?"* → `bps_L4b`

---

**LINE: `bps_L4a`** → END

*"I can't tell you that. Not because I don't have an opinion — I do. But the question you're asking is whether the principle should have bent and I'm not the right person to answer it because I have my own record on that question and my record is complicated.*

*I've sat on things. For longer than she did. For reasons that were defensible and cost something and I've run the accounting more than once.*

*What I'll tell you is this: the principle is correct. The exceptions to the principle are real. Living at that intersection is the cost of being serious about both. You did that. Whether you got it right — that's not a question that resolves cleanly."*

---

**LINE: `bps_L4b`** → END

*"Prepare for the revelation to happen anyway. When it does — and it will, the data is too distributed for indefinite suppression — your role is to have positioned people who can handle the fallout better than the factions will by default.*

*The Compact Pragmatist contacts. The Holdout community leaders who can hold their communities steady through a shock. The Signal analysts who can contextualize the revelation rather than just release it.*

*You already know who some of those people are. Start there. The revelation will catch up to you. Be ready when it does."*

---

### `burke_veteran`
*Trigger: First visit to Burke's property — Veteran background only. Fires before other calls.*

**LINE: `bvet_L1`** → `bvet_L2`

*"You did village stability operations. Afghanistan, somewhere. I can tell by how you assess a community — you're looking for the same things we looked for. Who has standing. What the real authority structure is under the nominal one. Where the resource flows actually go.*

*Here's the thing nobody tells you on the way back: you can't unlearn the assessment. You come home and you run the same model on your own country, your own institutions, your own neighborhood — and the model returns results. And the results are uncomfortable, because the model was built for a failing state and it keeps flagging things.*

*What you do with the results is the question. Most guys I know who came back with the assessment capability chose not to look at the results. Easier. Quieter."*

**LINE: `bvet_L2`** → branches

*"You're not doing that. You're here, which means you're looking. So. What do the results say?*

*I'll tell you what I think they say, and you tell me where I'm wrong.*

*The Compact is a centralized command structure filling a power vacuum. Logical in the short term, institutionally brittle as the crisis extends. The Hardliner/Pragmatist split is not ideology — it's what happens to every military-adjacent institution under sustained pressure without external accountability. The Wardens are the only faction that built a code instead of a command structure. The code is the right architecture. The Holdouts are VSO in miniature.*

*That match what you're seeing?"*

**BRANCH after L2:**
- A: *"Mostly. What am I missing?"* → `bvet_L3a`
- B: *"What did you do with your results?"* → `bvet_L3b`

---

**LINE: `bvet_L3a`** → END

*"Project Tether. The Compact has a classified program — captured machines, forcibly reprogrammed. The output is what they're calling Corrupted. It's not machine pathology. It's their engineering.*

*The reason this matters for your assessment: the Compact has been fighting machines that were building planetary defense. Not enemy assets — infrastructure. They don't know this yet. When they find out — and they will — the Hardliner response will be to accelerate the program before the finding can generate a command question. The Pragmatist response will be to try to quietly undo it.*

*Watch which response is faster. That tells you the command culture. That tells you who you can work with."*

---

**LINE: `bvet_L3b`** → END

*"I built things. Businesses, community infrastructure, a network of people in positions where they could do something useful when something useful needed doing.*

*Then I enlisted at forty-two. Did eleven years. Came home with the assessment capability and a longer list of things that needed building.*

*Here's the thing about the handover model — you build capacity, you prepare to leave, the thing you built stands after you go. That works when you're the outsider. You're not the outsider here. You're the person who lives here. The thing you build is the thing you live in.*

*What does that change about how you build? That's the question I've been working on for twenty years. I don't have a clean answer. I have a lot of partial ones. Sit down — I want to hear what you're thinking."*

---

### `burke_wardens`
*Trigger: Player has had significant engagement with the Warden faction — oath consideration, Moot participation, or significant Record entry.*

**LINE: `bwar_L1`** → `bwar_L2`

*"The code is the right architecture. A code survives what a command structure doesn't — because when the leaders fail, and leaders always eventually fail, the code remains as the standard against which the failure is measured. The failure is visible. The code judges it.*

*This is rare. Most institutions protect the people running them from accountability. The Accord is designed to do the opposite — the institution exists to hold the people running it to a standard that exists independent of whether they like it.*

*I've been watching institutional design for twenty years. This is the best version of it I've seen."*

**LINE: `bwar_L2`** → branches

*"What I'm watching is whether the people carrying the code understand why it was built. Not the surface — hold what you protect, keep your word, the sword serves. The depth. The code was built because the men who built it had watched what happens when there is no code. They had been in the places. They knew what it cost.*

*That knowledge is why the code has weight. You take it out of the equation and you get the form without the substance. The words without the understanding. A code without the memory of what it's for is a recitation. A recitation doesn't hold when the test is real."*

**BRANCH after L2:**
- A: *"Are they passing the understanding?"* → `bwar_L3a`
- B: *"What's the test?"* → `bwar_L3b`

---

**LINE: `bwar_L3a`** → END

*"In some Houses, yes. In others — I'm watching. The Houses that have maintained continuity through the Cascade have the knowledge intact. The older members remember why. The younger ones are learning from people who remember.*

*The Houses that lost members to attrition — Fractured, Hollow, the displacement events — have gaps. The words are there. The understanding is thinner. That's not a failure of the code, it's a demographic problem. The solution is not more recitation. It's deliberate transmission — the people who know sitting down with the people who don't and telling them the whole story, not just the rules.*

*I've been quietly suggesting this to a few Arbiter contacts. Some of them heard it. Some didn't."*

---

**LINE: `bwar_L3b`** → END

*"When the code conflicts with what you want to do.*

*Not a large conflict — not an obvious one where everyone agrees the code is right and you're wrong. The ambiguous conflict. The situation where you can construct a defensible argument that the code permits what you want to do if you read it a certain way, and the reading is not obviously wrong.*

*That's when you find out whether someone is carrying the understanding or the words. The words are flexible if you apply enough pressure. The understanding — if it's real — is not. It tells you what the code was built to prevent, and that's the standard, regardless of what the words can be made to accommodate.*

*Watch for that moment. In yourself first."*

---

### `burke_record_costly`
*Trigger: Player has done something significant for someone else at personal cost — faction standing sacrifice, personal resource loss, risk taken with no tactical advantage. Not one-shot; fires once per qualifying event, max once per session.*

**LINE: `brc_L1`** → END

*"[Signal researcher / Holdout leader / NPC name] mentioned what you did. Said you didn't have to.*

*I know.*

*I've been watching your Record — the real one, not the AR overlay. Not the kills and the faction standing and the quest completions. The choices. What you did when there was a reason not to and you did it anyway.*

*Those choices are what the Record is actually for. Not documentation. Evidence. Evidence of what kind of person shows up when the choice costs something.*

*I keep a different ledger. You're in it."*

*(NOTE: `[Signal researcher / Holdout leader / NPC name]` should be substituted at runtime with the specific NPC name associated with the qualifying Record event. Pass as a runtime parameter to the line content.)*

---

### `burke_responsibles_ask`
*Trigger: `WYA.Event.Burke.NetworkThreshold` — fired by game system when a player's Record and prerequisite calls meet the threshold criteria. Late Act 3 / Act 4. The network ask.*

**LINE: `bra_L1`** → `bra_L2`

*"I've been watching your Record. The real one, not the AR overlay. What you chose when it wasn't convenient. What you held when holding was expensive. You've been running the right model in the right operating environment and the outputs have been what they should be.*

*That's rarer than it looks. There are a hundred people in this region who have the capability. There are maybe eight who have been applying it the right way consistently enough that I'd stake something on their judgment.*

*You're one of the eight. I'm not going to waste the observation."*

**LINE: `bra_L2`** → `bra_L3`

*"I've been building a network. Not a faction, not a structure. A set of specific people, specific positions, specific commitments. The conditions under which civilization gets rebuilt depend on the right people being in the right places when the pressure comes. I've been placing them for two years.*

*There is a position that needs filling. A specific person in a specific place is about to be removed — not by combat, by institutional politics — and the removal matters in ways that won't be obvious until six months after it happens. I've identified the intervention point. I cannot execute it myself without changing the value of the intervention.*

*It has to be someone whose Record is clean, whose faction affiliations are legible to both sides of the conflict, and whose judgment I trust when the situation is not what I briefed them it would be."*

**LINE: `bra_L3`** → branches

*"Here's what I need from you. Here's what it costs. Here's why it matters.*

*[SPECIFIC ASK — populated at runtime from network state: which network member is at risk, what the intervention is, what the player must do.]*

*You can say no. But you won't."*

**BRANCH after L3:**
- A: *"What if the situation changes?"* → `bra_L4a`
- B: *"I'll do it."* → `bra_L4b`

---

**LINE: `bra_L4a`** → END

*"Then you use your judgment. That's the point.*

*I'm not sending you to execute a plan. I'm sending you because the plan will encounter something I didn't anticipate and the person in the room needs to be someone who can adapt without asking for instructions. Instructions take time. Time is the variable you won't have.*

*I've watched your Record for months. If I didn't trust your judgment in a changed situation, I'd be sending someone else.*

*Go."*

---

**LINE: `bra_L4b`** → END

*"I know.*

*Come back when it's done. Tell me what you found that I didn't expect. There's always something."*

---

## Implementation Notes for Core Agent

1. **Runtime parameter substitution** — `burke_record_costly` and `burke_responsibles_ask` require runtime substitution of NPC names and specific ask details. Pass as `TMap<FName, FText>` parameters to `TriggerCodec`.

2. **Meshtastic delivery** — before `WYA.Event.Meshtastic.BurkeNodeConnected`, all calls queue and deliver at property. After, calls deliver in field via assistant relay. UI should distinguish delivery contexts (property = at fire; field = assistant earpiece visual).

3. **Branch timeout** — 8 seconds default. On timeout, auto-select branch index 0. Burke notes early departure: if player leaves before a call completes, next visit to property triggers a single line: *"You left early last time. I noted it."* (brief, not a full call).

4. **`burke_responsibles_ask` specific ask** — the `[SPECIFIC ASK]` content in `bra_L3` is populated at runtime from `WYABurkeNetworkSubsystem` state. The subsystem tracks which network cluster is currently under threat and passes the appropriate content. Narrative agent to provide the full set of possible specific asks (one per network cluster) in a follow-up doc.

5. **Call stacking** — multiple calls can queue if multiple trigger events fire before the player visits the property. Deliver in priority order, one per visit (not all at once). Resume the queue on the next visit.

---

*v0.1 — Burke codec DataTable spec. 14 calls, full scripts, struct definitions, implementation notes. Flagged to decisions-log.md.*
