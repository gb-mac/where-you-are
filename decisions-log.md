# Decisions Log

Record of major architectural and design decisions. Add an entry whenever a significant choice is made.

---

## 2026-03-17

### Engine: Unreal Engine 5.5
**Decision:** UE5.5 as primary engine.
**Reasoning:** Native Linux support, Vulkan renderer, Nanite/Lumen for world fidelity, C++ module system.
**Alternatives considered:** Unity, Godot.

### Renderer: Vulkan + FSR
**Decision:** Vulkan on Linux, DX12 on Windows. FSR for upscaling (not DLSS).
**Reasoning:** AMD hardware (RX 5700) — no DLSS support.

### Network: Hybrid P2P
**Decision:** Centralized servers for matchmaking/anti-cheat, P2P for gameplay with host migration.
**Reasoning:** Reduces server costs for F2P while maintaining anti-cheat integrity.

### Location System: ENU Projection
**Decision:** WGS84 → ECEF → ENU local tangent plane for GPS-to-world conversion.
**Reasoning:** True metric distances in all directions; accurate to ~2m within 5km radius. Better than Mercator for world-space placement.

### Location Fallback Chain
**Decision:** Cache → IP geolocation (ip-api.com) → CLI args → DefaultGame.ini → hardcoded default.
**Reasoning:** Supports offline play; no API key required; testable without GPS hardware.

### AI Pipeline: Local First → Bittensor
**Decision:** Local LLM inference first, migrate to Bittensor subnet later.
**Reasoning:** Validate pipeline on CPU before committing to decentralized infra costs.

### World Axis Mapping
**Decision:** UE5 X=North, Y=East, Z=Up (1 UU = 1cm).
**Reasoning:** Standard ENU convention; consistent with navigation/AR frameworks.

### Revenue Model: Free-to-Play
**Decision:** F2P with in-game economy.
**Reasoning:** Maximum player acquisition for an AR game requiring real-world location.
