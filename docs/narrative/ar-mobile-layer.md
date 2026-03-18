# AR Mobile Layer — Narrative Design v0.1

The desktop game (UE5) is the shooter. The mobile AR app is the world between sessions.

---

## What the Mobile App Is

A companion app running on the player's phone, using their real-world GPS position to overlay the game world onto their physical location. When you walk down your actual street, you see faction territory, Historical Markers, active Displacement Zones, Snatch Event warnings — and items left by other players, sitting at the real-world coordinates where they were placed.

The mobile layer is not a mini-game. It is the game's connective tissue — the thing that makes the world feel alive when you're away from the desktop, and the thing that makes your real neighborhood genuinely matter.

---

## Items — Leave It or Take It

Any player can leave an item at their current real-world GPS position through the mobile app. Any other player who physically arrives at that location can pick it up.

**First come, first served. No exceptions.**

There is no reservation system. There is no claiming from a distance. If you see an item on the AR overlay and you want it, you go there. If someone else gets there first, it's gone. The item exists at a physical coordinate in the real world, and the real world determines who gets it.

Items left in the AR layer transfer directly into the desktop game inventory when picked up. Items placed from the desktop game inventory appear in the AR layer at the player's last real-world GPS position.

**What can be placed:**
- Consumables and supplies — medicine, food, ammunition, fuel cells
- Crafting components — including Vael Components and machine parts from Fractured hunts
- Weapons and gear (left at significant personal cost — someone needed to choose to leave this)
- **Notes** — text messages attached to a location, visible to any player who arrives. Persistent until manually removed or the location is cleared.
- **Warden Markers** — players with Warden standing can formally place a Marker token at a location, creating a real-world obligation drop that the recipient must physically collect
- **Dead drops** — encrypted item bundles that only a specific player identifier can open. Signal faction questlines use these heavily.

**What cannot be placed:**
- Faction territory claims (territory is earned through desktop gameplay, reflected in the AR layer automatically)
- Fake Historical Markers (only Arbiters can place these — the app verifies Warden standing server-side)

---

## The Competitive Layer

Because items are first-come-first-served, the AR layer creates genuine real-world competition and cooperation:

**Supply racing:** A player on the desktop game kills a Fractured Composite and places rare components at their home GPS coordinates before logging off. Nearby players — physically nearby, in the real world — can see the cache on the AR overlay. If they want it, they go there. If the placing player has allies, those allies know not to touch it. If they have enemies, those enemies know exactly where to go.

**The displacement return:** A player snatched by the Vael and dropped in an unfamiliar city has nothing. But other players in that city — including players who've been through the same displacement and left caches specifically for new arrivals — may have seeded the drop zone. Holdout-aligned players sometimes maintain standing supply caches near known Vael drop coordinates. Hollow-aligned players watch those same coordinates for vulnerable arrivals.

**Warden Historical Markers:** When an Arbiter places a Historical Marker at a real GPS coordinate, it appears in the AR layer at that exact location. Players who physically visit the site see the full Record entry — what happened here, who was involved, what was decided. Markers at significant real-world locations — a player's actual neighborhood park, a local landmark — carry a different weight than markers in abstract game space. This is your street. This is what happened here.

**The Hollow dead drop economy:** Hollow Syndicates use the AR layer extensively for moving goods between players without direct contact. A Syndicate cache appears as an unmarked item on the overlay — no faction identifier, just a coordinate and a description. If you know what you're looking for, you know what it is. If you don't, you might pick it up anyway and find yourself holding something the Syndicate wants back.

**Notes as world-building:** Notes left at real-world coordinates become micro-lore. A note left at a recognizable local landmark — a building, an intersection, a park — from a character in the game world, describing what that place was like after the Cascade, is a different kind of storytelling than anything the desktop game can do. Players find notes. Players leave notes. The AR layer develops a palimpsest of what the fictional world's version of their real neighborhood became.

---

## AR Overlay Features (Mobile Only)

These are visible in the mobile app, not the desktop game:

**Faction Territory:** Color-coded overlay showing which faction controls or contests each area at your current location. Updates based on desktop gameplay events. Walking through a Hollow-controlled zone feels different when your phone is showing you that in the AR layer.

**Historical Markers:** Warden Record entries anchored to real GPS coordinates. Physically visit the location to see the full entry. Some players seek out markers the way others seek out landmarks.

**The Warden Mark:** Excommunicado players appear as marked contacts in the AR overlay for any player with Warden standing. Their real-time approximate position updates as they move through the world. The Mark is visible at their real-world GPS location — if a marked player is physically near you, the AR app shows it.

**Snatch Event Warnings:** When Probe cluster activity spikes in the player's real-world GPS area (mapped from the game world), the AR app pulses a warning. The subsonic hum and AR flicker that precede a Snatch Event in the desktop game are reflected as an AR distortion effect in the mobile app. Players physically near a Displacement Zone get a proximity warning.

**Displacement Zone Markers:** Active Displacement Zones are visible in the AR layer at their real-world coordinates. Players near a Ship's position see the column effect in the AR overlay. High-risk zones are marked — not as a restriction, but as information.

**Machine Activity Signatures:** High Coherent activity in the game world generates a visual signature in the AR layer at the corresponding real-world location. Players physically near a Coherent work site (mapped to the nearest real-world analog) see increased signature density. Fractured territory is overlaid differently — the AR aesthetic for Fractured zones is degraded, glitching, wrong.

---

## The Loop Between Desktop and Mobile

The two layers are not separate games. They are two interfaces to the same world.

- You play the desktop game, you earn items, you place them in the AR layer at your real-world location
- You walk around in the real world with the mobile app, you pick up items others have placed, you bring them back to the desktop game
- Events in the desktop game change what you see in the mobile app (faction shifts, Historical Markers, Displacement Zone timing)
- Things you do in the mobile app (picking up a cache, leaving a note, collecting a dead drop) affect your desktop game inventory and faction standing

A player who only plays the desktop game is missing the AR layer's resources and information. A player who only uses the mobile app is missing the gameplay that makes their AR actions matter. Neither is required. Both make the experience richer.

**The design intention:** your real neighborhood is the game world. Your commute is a scouting run. Your lunch break is a supply run. The items at the corner of your actual street were left there by someone who plays this game and was there before you. Whether they left them for an ally or as bait is information the AR layer doesn't give you. The world doesn't either.

---

*v0.1 — AR mobile layer narrative design. Decisions flagged to decisions-log.md.*
