# Burke Responsibles — Specific Asks
*Runtime content for `burke_responsibles_ask` call, `bra_L3` parameter `[SPECIFIC ASK]`.*
*`WYABurkeNetworkSubsystem` selects one based on which cluster is currently under threat.*

---

## How This Works

`burke_responsibles_ask` fires when a player's Record meets the threshold. `bra_L3` contains a `[SPECIFIC ASK]` placeholder populated at runtime from this doc. The subsystem checks which network cluster is currently flagged as at risk and passes the corresponding ask text.

If multiple clusters are at risk simultaneously: highest-priority cluster wins (order below is priority order, 1 = highest).

---

## Ask 1 — Compact Pragmatist Cluster
*Fires when: a Compact Pragmatist officer in the network is flagged for removal or reassignment by Hardliner command action.*

**`[SPECIFIC ASK]` content:**

*"A Compact officer — Pragmatist, regional command level, one of three people in this zone who runs the mission as stated rather than as the Hardliners have reinterpreted it — is being reassigned. Not fired. Reassigned to an administrative position that removes her from operational authority. The order comes from Hardliner command. She has thirty days.*

*What I need: a specific piece of evidence that exists in a Compact facility outside her region — an operational record that documents a Hardliner decision that contradicts Compact founding charter. That record gets her a formal review hearing rather than a quiet reassignment. The review buys six months. Six months changes the outcome.*

*The facility is in contested territory. The record is in a file format the Compact stopped using two years ago. You'll need to know what you're looking for before you go in or you'll walk past it.*

*Cost: the Compact will know someone was in that facility. They'll trace it eventually. You will have operational exposure with the Compact's counterintelligence function for approximately ninety days.*

*What it prevents: the Hardliner replacement runs the annexation protocol in her region at full pace. Three Holdout communities get reclassified as strategic assets within sixty days of the transition. I've been keeping those communities out of the Register for eight months."*

---

## Ask 2 — Signal Analyst Cluster
*Fires when: a Signal analyst in the network has had their location compromised — Compact surveillance, Hollow intelligence sale, or internal Signal security failure.*

**`[SPECIFIC ASK]` content:**

*"One of my Signal contacts has been working for eighteen months on the Vael behavioral pattern data — the subset of the Assessment event analysis that isn't published, the material that runs three steps ahead of what Dr. Osei releases to the network. Her location has been burned. I don't know by whom. I know she has approximately seventy-two hours before whoever burned it acts on the information.*

*What I need: get her to a secondary location she doesn't know about. She'll resist — the data she's working on is physically at her current site and she won't leave without it. The data has to come with her. The current site has Fractured accumulation within two blocks.*

*Cost: you're taking on operational exposure at a Signal cell that will want to know who you are and how you knew to do this. I cannot give you cover for the Signal's internal security questions without revealing I knew. I won't do that. You'll have to be vague in a way that Signal security will notice and flag.*

*What it prevents: the analysis she's currently holding represents eight months of work that doesn't exist anywhere else. If it's lost, the Signal rebuilds from older data. What they'll miss: the pattern shift that occurred six weeks ago in Assessor behavior that suggests the evaluation timeline is accelerating. Someone needs to know that timeline before it becomes obvious."*

---

## Ask 3 — Warden Cross-Faction Cluster
*Fires when: a Warden Sworn in the network with cross-faction credibility is being politically isolated within their House.*

**`[SPECIFIC ASK]` content:**

*"There's a Sworn in the Eastfield House who has been carrying relationships I can't carry — she can walk into an Iron Compact facility and be received as a neutral witness, walk into a Covenant gathering without triggering the Vigil, carry messages between communities that won't speak directly to each other. That function has kept three potential conflicts from escalating in the last four months.*

*She's being isolated. There's a faction inside her House that has decided her cross-faction relationships are a code violation — that keeping relationships with the Compact and the Covenant is a conflict of interest, not a service. The formal challenge is coming at the next Moot. If it succeeds, she loses Recognized standing and the cross-faction function dies with it.*

*What I need: a formal Account. Specific — a documented record of an incident where her cross-faction relationship produced an outcome that a Warden-only position could not have. That Account gets entered in the Record before the Moot. It doesn't win the challenge. It changes the weight of it.*

*The incident is real. You were part of it, or you witnessed it, or you can reach someone who was. The Account has to be true — the Arbiters will verify it.*

*Cost: you're injecting yourself into a House's internal politics. The faction challenging her will know your name after this."*

---

## Ask 4 — Holdout Anchor Cluster
*Fires when: a Holdout community leader in the network is being undermined by internal community politics or external faction pressure.*

**`[SPECIFIC ASK]` content:**

*"A community leader I've been supporting for eight months is about to lose her community. Not to the Compact. To her own people — specifically to a faction inside the community that has decided the current leadership is too accommodating to outsiders and wants a harder gate policy.*

*She's not going down fighting. She's going down because she's exhausted and the faction against her has more energy right now than she does. That's how most of these transitions happen. Not a coup. Attrition.*

*What I need: a material win for her in the next two weeks. Specific, visible, attributable to her leadership — not to outside help. The community needs to see her produce something that the isolationist faction can't replicate or explain away. She has a resource problem that I can't solve from here — a supply route that's been blocked for six weeks by a Hollow Crew that's not ideological, just territorial.*

*Clear the route. Don't attribute it to me or to her. Let it look like the route opened on its own and she was the one who was positioned to use it.*

*Cost: the Hollow Crew will know someone worked against them. Word travels in Hollow networks.*

*What it prevents: the isolationist faction takes control and that community is on the Gone Dark trajectory within sixty days. I've seen the progression before. The leadership transition is the last recoverable point."*

---

## Ask 5 — Independent Operator Cluster
*Fires when: a positioned contact (not formally in the network) is about to make a consequential decision with incomplete information that Burke can correct.*

**`[SPECIFIC ASK]` content:**

*"This one is different. This isn't protecting someone in a network position — this is preventing a decision that can't be undone.*

*There's a player — an operator I've been watching, not formally in my network — who is about to hand a piece of intelligence to the Iron Compact that I know the Compact will use in a way that harms three Holdout communities in the eastern zone. The operator doesn't know this. They think they're doing a clean information trade. The Compact contact they're trading with is a Hardliner running a specific collection operation, and the intelligence fits directly into the annexation case for the eastern zone.*

*The operator won't take the information from me — we've never met and an approach from a stranger with a warning sounds exactly like an attempt to compromise the trade. They need to hear it from someone they already know. You.*

*What I need: reach them before the handoff. You have forty-eight hours. Tell them specifically what the Compact contact is collecting and why this piece fits it. That's all — give them the context. What they decide after that is their call.*

*Cost: you're asking them to burn a trade relationship and take your word for the reason. If they don't believe you, you've done nothing. If they do believe you and later find out it was accurate — which they will — you've built something real. Either way, the cost is the ask itself.*

*What it prevents: the annexation case for the eastern zone advances by approximately six months."*

---

*v0.1 — Burke Responsibles specific asks. Five cluster-specific asks for `burke_responsibles_ask` runtime parameter. Priority order: Compact Pragmatist > Signal Analyst > Warden Cross-Faction > Holdout Anchor > Independent Operator.*
