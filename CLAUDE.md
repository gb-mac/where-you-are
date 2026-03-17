# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**"Where You Are"** — Commercial free-to-play desktop 3rd-person shooter with AR. Players start at their real-world GPS location and explore a digital twin overlay of the real world. Combines Division 2 / Elder Scrolls Online / GTA / Monster Hunter dynamics.

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
# Set UE5 root (find yours with: find /opt /usr -name "UnrealEditor" 2>/dev/null)
export UE5_ROOT=/opt/unreal-engine

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

## Current Status

- [x] Vulkan confirmed on CachyOS
- [x] UE5 installed
- [x] Git repo initialized
- [ ] UE5 project scaffold (.uproject, Config/, Content/)
- [ ] Core location/AR prototype
- [ ] AI pipeline CPU test
