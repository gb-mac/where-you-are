# Iris Mesh Spec — The Assistant
*Geometry, UV, placement, and naming for M_AssistantEye.*

---

## Mesh

**Shape:** Flat disc. No depth — the perceived depth comes entirely from the material gradient, not geometry.

**Polygon count:**
- LOD0: 32-sided polygon (32 tris from center) — smooth enough at close range, cheap enough to not matter
- LOD1: 16-sided
- LOD2: 8-sided
- LOD3+: 8-sided, RingOpacity reduced to 0.5 via LOD callback

**Thickness:** Zero (single-sided plane). Faces outward from head surface. No backface needed — Two-Sided is false on the material.

---

## UV Mapping

**Critical requirement:** UV must map a unit circle centered at (0.5, 0.5).

- Center of disc = UV (0.5, 0.5)
- Edge of disc = UV distance 0.5 from center (so full radius = 0.5 in UV space)
- The material multiplies dist by 2.0 to normalize — this assumes exactly this UV layout

If UV is not centered at (0.5, 0.5), or the radius isn't 0.5 in UV space, the ring and depth gradient will be off-center or incorrectly scaled. Verify in the material by temporarily setting `EmissiveMultiplier` to 1.0 and checking the depth falloff is symmetric.

---

## Scale & Placement

**Scale:** Each iris disc is approximately 4–5cm diameter in world space (human eye reference: ~12mm iris, but the Assistant's eyes read larger for expressiveness — 3–4× human scale at gameplay distances).

**Placement:** Flush against the head mesh surface, offset 0.5mm outward along the face normal to prevent z-fighting. The head mesh should have shallow recessed sockets — the iris disc sits in the socket so the dark surround ring merges with the socket shadow.

**Orientation:** Face normal must point exactly outward from the head. Any tilt will cause the radial depth gradient to read asymmetrically.

---

## Component Setup in BP_Assistant

```
BP_Assistant
  ├── SkeletalMesh (head + body)
  ├── StaticMeshComponent: SM_AssistantIris_L
  │     Material slot 0: MI_AssistantEye (source for MID)
  │     Attached to: head socket "Eye_L"
  └── StaticMeshComponent: SM_AssistantIris_R
        Material slot 0: MI_AssistantEye
        Attached to: head socket "Eye_R"
```

Both iris components use the same source material instance. In BeginPlay, create one MID and apply it to both:

```
CreateDynamicMaterialInstance(MI_AssistantEye) → MID_Eye
SM_AssistantIris_L → SetMaterial(0, MID_Eye)
SM_AssistantIris_R → SetMaterial(0, MID_Eye)
```

Both eyes always transition together. They are never independent.

---

## Source Asset

| Asset | Path |
|-------|------|
| Mesh | `Content/Characters/Assistant/Meshes/SM_AssistantIris.uasset` |
| Material | `Content/Characters/Assistant/Materials/M_AssistantEye.uasset` |
| Material Instance | `Content/Characters/Assistant/Materials/MI_AssistantEye.uasset` |
| MID (runtime) | Created in BP, not a saved asset |

---

## Naming Conventions

```
SM_AssistantIris        // static mesh source
M_AssistantEye          // master material
MI_AssistantEye         // material instance (source for runtime MID)
MID_Eye                 // runtime dynamic instance (BP variable)
Eye_L / Eye_R           // socket names on head skeleton
```

---

*v0.1 — Iris mesh spec. UV unit-circle requirement, LOD counts, component setup. See M_AssistantEye node graph in assistant-eye-shader.md v0.3.*
