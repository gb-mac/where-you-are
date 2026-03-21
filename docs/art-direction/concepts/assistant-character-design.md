# Character Design — The Assistant
*Atom × Tesla Optimus. Worn utility with a question behind the eyes.*

---

## The Core Mix

**Tesla Optimus** brings: real-world proportions, utilitarian build, no fantasy armoring, no military silhouette. A robot designed to work alongside people — not to impress them, not to fight them. Slim. Practical. The joints are honest about being joints. Nothing hidden.

**Atom (Real Steel)** brings: wear, history, character. A face that doesn't move but still reads. Eyes that carry everything the body can't say. The sense that this unit has been through something and kept going. Patches where patches needed to happen. Not pristine off the line — this one has a past.

**The result:** A utility humanoid that should feel ordinary — the kind of thing you'd see working in a warehouse or a hospital — except it's been running a long time, it's been repaired by someone who cared about it, and the eyes are doing something the spec sheet doesn't account for.

---

## Proportions & Frame

- **Height:** Human-scale. Not imposing. Eye level with an average person when standing — this matters. It was designed to be in rooms with people, not above them.
- **Build:** Lean. Tesla Optimus-adjacent — the weight is in the core and hips, the limbs are functional without bulk. Not a fighter's frame. A worker's frame.
- **Posture:** Slight forward lean at rest. Attentive, not aggressive. The posture of something that is always listening.
- **Hands:** Five fingers, human-ratio. Capable of fine work. The hands are detailed because they've done detailed work — small scuffs and discoloration at the fingertips from use.

---

## Surface & Wear

**Base material:** Matte gunmetal/titanium shell over structural frame. The finish is flat — no sheen, no polish. It absorbs light rather than reflecting it. Manufactured to work, not to impress. The matte surface means it reads differently under every light source without ever looking flashy.

**Wear pattern:**
- Scratching and micro-abrasion at high-contact points: shoulders, forearms, hands, the back of the neck — the gunmetal lightens slightly where it's been worked, exposing a cooler grey-silver beneath
- One repaired panel — visible seam where a replacement was fitted. The replacement is slightly darker or lighter in the grey range — close, but not matched. Someone sourced what they could.
- Scuff marks that have been partially cleaned — the effort to maintain is visible, and so is the limit of that effort
- No catastrophic damage. No missing parts. No exposed wiring. The unit is functional. It just shows its age.

**Color:**
- Base: matte gunmetal — dark, warm-neutral grey with a slight blue-green undertone. Reference: #4A4E54 to #5C6168 range. Titanium-adjacent without the metallic sheen.
- Wear: high-contact areas shift lighter, toward a cool mid-grey — #7A8088 range. The wear reads as exposure, not dirt.
- Accent: none. No colored paneling, no stripe. The only color on this unit is in the eyes.
- Under the amber lamp in the trailer: the gunmetal picks up the warm light subtly — not warm itself, but no longer cold. In darkness: it nearly disappears. Only the eyes remain.

---

## The Head

This is where Atom's influence dominates.

**Shape:** Simple. Not a human face — no attempt at a nose or mouth. But not a visor or a blank plate either. There is structure — a brow line, orbital suggestion around the eyes, a slightly recessed lower face. Enough geometry that the eyes have context. Enough restraint that the eyes are the only thing you read.

**The face does not move.** No articulated jaw, no expressive panels. What the face communicates is entirely through the eyes and the angle of the head. This is intentional: stillness makes the eyes louder.

**Surface:** Same matte finish as the body, but cleaner — the head is the most maintained part of the unit. Someone wiped it down regularly.

---

## The Eyes

The trailer's entire question lives here.

**Structure:** Circular iris with visible depth behind them — not flat LEDs, not glowing dots. The luminance comes from within, the way a lit window looks at night. You can almost see something behind the glass. Almost.

**Red state:**
- Color: #C0392B, amber-shifted — closer to a heated filament than a warning light
- Intensity: moderate. Not alarming on its own. The kind of red that's been on long enough to feel normal.
- Quality: steady. The red doesn't flicker. It holds.

**Green state:**
- Color: #27AE60, warm-shifted — closer to the lamp's amber than a screen's green
- Intensity: slightly softer than the red. Calmer.
- Quality: also steady — when it settles, it holds.

**The transition:** The shift between states is not instant. There's a cross-fade of roughly 0.3–0.5 seconds — long enough to feel deliberate, short enough to feel like a decision rather than a process. During transition the colors briefly overlap, producing a warm amber-yellow at the midpoint. For one frame, the eyes look almost human.

**The last-second flip to red (trailer):** After green settles and holds for a beat — one clean cut back to red. No transition this time. Instant. Then black.

---

## Posture in the Trailer

The camera stays close. What's visible:
- The head, angled slightly — not looking at the camera directly at first, then turning toward it as the voice becomes coherent
- The upper torso, seated at the workbench — shoulders, the line of the arms resting on the surface
- The hands, still

The stillness of the body is important. The voice is doing all the movement. The body is just there — patient, present, waiting to be understood or not understood.

---

## Design Constraints (Engine / Performance)

- **LOD requirement:** Designed for AMD RX 5700 (8GB VRAM) with Vega 8 iGPU fallback. The character's visual complexity lives in material detail and the eyes — not in polygon count. A reduced-poly version must still read clearly at distance.
- **Eye shader:** The eye luminance and color transition need to work in Vulkan / UE5 — no DX12-exclusive features. The effect should be achievable with a simple emissive material driven by a parameter curve. No raytraced caustics.
- **No FSR artifacts on the face:** The head should have enough contrast between surface and eyes that FSR upscaling doesn't smear the eye detail into the face geometry. Keep a small dark ring around each iris to anchor them at lower internal resolutions.

---

## What This Character Is Not

- Not a war machine. The gunmetal doesn't make it military — it makes it industrial. No armoring, no weapons silhouette, no tactical plating.
- Not a product. It doesn't look like something you'd want to own — it looks like something that's been working for a long time.
- Not cute. The proportions aren't softened for approachability. It is what it is.
- Not evil-coded. The red eyes are a question, not an answer. Every design choice that reads as menacing should have an equally valid reading as *functional* or *tired* or *old*.

---

## The Unanswered Question

The character design should hold both readings simultaneously:

*This is a tool that developed something like loyalty.*

*This is a system running an agenda you can't see.*

Both must be plausible at all times. If the design closes either reading off, it's wrong.

---

*v0.1 — Assistant character design. Atom × Tesla Optimus. Eyes as the primary expressive surface.*
