# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**"Where You Are"** — Commercial free-to-play desktop 3rd-person shooter. Players start at their real-world GPS location and explore a persistent online world. Combines Division 2 / Elder Scrolls Online / GTA / Monster Hunter dynamics.

**This repo is the desktop UE5 game only.** The AR companion mobile app lives at `~/Projects/game-mobile`. Do not add AR overlay features here.

### Platform Split
| Platform | Repo | Role |
|----------|------|------|
| Desktop (UE5) | `~/Projects/game` (this repo) | 3rd-person shooter, main game |
| Mobile (AR) | `~/Projects/game-mobile` | AR overlay, GPS item drops, dead drops, faction territory, Warden Markers |

The desktop game uses the player's GPS location **only** to set the world spawn origin. All real-world AR overlay features (Snatch warnings, Displacement Zones, Historical Markers, item placement at GPS coordinates) are mobile-only.

**Shared item state (critical):** Items dropped at GPS coordinates via the mobile app appear in the desktop game at the equivalent `FWYAGeoMath::GeoToWorld` position, and are claimable by desktop players. Desktop players can also place items retrievable by mobile players. Both platforms share a single authoritative item state backend. This cross-platform loop is core gameplay.

- Solo developer (gb_mac)
- Revenue target: 1 year runway → Early Access
- Engine: Unreal Engine 5 (Linux-native on CachyOS)

## Hardware Context

| System | Specs | Status |
|--------|-------|--------|
| Backup (current) | Beelink SER5850U, Vega 8 iGPU | Active dev machine |
| Main (pending) | Ryzen 5700G + RX 5700 (8GB VRAM) | Arriving in weeks |

- Renderer: **Vulkan** (Linux), DX12 (Windows) — use **FSR** not DLSS (AMD hardware)
- UE5 installed and Vulkan confirmed working on CachyOS

## Architecture

### Tech Stack

| Component | Choice | Notes |
|-----------|--------|-------|
| Engine | Unreal Engine 5 | C++ + Blueprints |
| Rendering | Vulkan (Linux) / DX12 (Windows) | FSR upscaling |
| Network | Hybrid P2P | Centralized matchmaking/anti-cheat, P2P gameplay |
| Storage | Local saves | Offline capability required |
| AI | OpenClaw + Claude + local LLMs | NPC behavior, dynamic quest generation |
| Digital Twin | NVIDIA Earth-2 / Google partnership | Real-world location overlay |
| Decentralized AI | Bittensor ecosystem | Subnet migration after local inference validated |

### AI Pipeline

- Local LLM inference first (CPU-viable during backup hardware phase)
- Migrate to Bittensor subnet once main system arrives and pipeline is validated
- Claude API for quest generation prototyping and NPC dialogue

### Networking Model

- Centralized servers: matchmaking, anti-cheat, world state authority
- P2P: gameplay sync between nearby players
- Host migration model for session continuity

## Build Commands

```bash
# UE5 is installed at:
export UE5_ROOT=/home/gb_mac/.cache/yay/unreal-engine/src/unreal-engine

# Generate project files / compile_commands.json
./scripts/generate-project.sh

# Build (Development)
./scripts/build.sh

# Build (Shipping)
./scripts/build.sh -Configuration=Shipping

# Open in editor
$UE5_ROOT/Engine/Binaries/Linux/UnrealEditor WhereYouAre.uproject
```

## Project Structure

```
game/
├── Source/          # UE5 C++ source (modules added as project grows)
├── docs/
│   ├── design/      # GDD, feature specs, economy design
│   └── architecture/ # Technical architecture decisions
├── scripts/         # Build, deploy, tooling scripts
├── ai-pipeline/     # AI/LLM integration code and prompts
└── CLAUDE.md
```

## Development Phases

- **Months 1–3**: Foundation — UE5 project setup, core AR loop, GPS location integration
- **Months 4–6**: Vertical Slice — AI quest generation, basic economy
- **Months 7–9**: Multiplayer — host migration, P2P sync
- **Months 10–12**: Polish & Early Access launch

## Multi-Agent Workflow

This project uses multiple Claude Code instances running in parallel terminals, each owning a domain. All agents read this file first.

| Agent | Terminal | Context file | Output dir |
|-------|----------|-------------|------------|
| **Core** (you) | Main | `CLAUDE.md` | `Source/`, `Config/` |
| **Narrative** | Terminal 2 | `docs/narrative/AGENT.md` | `docs/narrative/` |
| **Art Direction** | Terminal 3 | `docs/art-direction/AGENT.md` | `docs/art-direction/` |
| **AI Pipeline** | Terminal 4 | `docs/ai-pipeline/AGENT.md` | `ai-pipeline/`, `docs/ai-pipeline/` |
| **Economy** | Terminal 5 | `docs/economy/AGENT.md` | `docs/economy/` |

- Major decisions → `decisions-log.md`
- Weekly summaries → `sync-meetings/YYYY-MM-DD-<agent>.md`

## Current Status

- [x] Vulkan confirmed on CachyOS
- [x] UE5 5.5 installed
- [x] Git repo initialized + pushed to github.com/gb-mac/where-you-are
- [x] UE5 project scaffold (.uproject, Config/, Source/)
- [x] Core location system (GPS → UE5 world space, fallback chain) — Calgary default (51.0447, -114.0719)
- [x] 3rd person character + GameMode/GameState
- [x] First playable loop compiled — WYAApiClient, WYAItemSubsystem, AWYAWorldItem, WYAPlayerController, WYAInteractionWidget
- [x] Backend API deployed — https://where-you-are-api.fly.dev
- [x] Cesium for Unreal plugin built (Binaries/Linux/ has .so files)
- [x] CesiumForUnreal added to .uproject + CesiumRuntime added to Build.cs
- [x] CesiumGeoreference wired to WYALocationSubsystem — sets origin on location resolve
- [x] 6 test items seeded near Calgary on backend
- [ ] Build WhereYouAreEditor with Cesium dependency (in progress)
- [ ] Drop CesiumWorldTerrain + Google Photorealistic 3D Tiles in level
- [ ] Debug item spawning in Play (verify 6 seeded items appear)
- [x] AI pipeline CPU test — phi3.5 @ 11.6 t/s CPU-only, async pre-gen viable (2026-03-18)
