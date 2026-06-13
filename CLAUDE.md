# CLAUDE.md — ASHFALL (Unreal Engine 5.5)

> **Leeme al empezar cualquier sesión en este proyecto.**

## Qué es esto
**ASHFALL** — horde shooter sci-fi decadente, estilo **Serious Sam** (hordas masivas + mapas grandes + low poly) en **Unreal Engine 5.5**. Migración desde ASHFALL Unity (jun 2026) por mejor stack visual y motor más robusto. **El diseño del juego no cambia** — sigue los specs de `docs/VISION.md`, `docs/ROADMAP.md`, `docs/MAPA_FUNDICION.md`.

## Estado actual
**Recién inicializado.** Proyecto creado desde el FPS template de UE 5.5 (C++, Scalable preset, sin Lumen). Setup mínimo: `.gitignore`, `.gitattributes` con LFS, docs migrados desde Unity. Fase 1 del plan.

## Método de trabajo (IMPORTANTE)
Daniel está **aprendiendo Unreal Engine** (viene de Unity). Por cada paso, el agente debe dar:
1. **Qué** vamos a hacer y **por qué**.
2. **Cómo** hacerlo en Unreal (los clics/menús concretos).
3. **Qué concepto** se aprende.

Daniel hace los conceptos nuevos en el editor (Actors, Blueprints, Animation Blueprints, Behavior Trees). El agente automatiza tareas voluminosas, mide datos, guía paso a paso. **Daniel decide cuándo automatizar y cuándo pedir guía**. Responde y documenta **en español**, conciso, paso a paso.

## Decisiones técnicas tomadas (no relitigar sin pedir)
- **Engine**: Unreal Engine **5.5** (no 5.7 — preferimos estabilidad y compatibilidad de MCPs).
- **Iluminación**: **HYBRID LIGHTING** — baked (Lightmass) para ambiente + movable (Cast Shadows OFF) para acciones (fogonazos, explosiones, linterna).
  - **NO Lumen** (mataría FPS con hordas, no encaja con low poly).
  - **NO Hardware Ray Tracing** (igual razón + requiere RTX).
  - **Path Tracing solo para el trailer**, nunca gameplay.
  - Quality Preset del proyecto = **Scalable** (no Maximum).
- **Estética**: low poly, voxel gore (sangre cubo), referencias = Ultrakill, DUSK, Selaco, Cultic.
- **Animaciones (ACTUALIZADO jun 2026)**: **Cero Blender**. Todo gratuito y fácil de cablear:
  - Esqueleto base = **UE5 Mannequin** (Manny/Quinn).
  - Fuentes de anim: **Mixamo** (vía **IK Retargeter**, UE 5.4+) + **Fab/Marketplace** (anim packs nativos UE) + lo que ya trae el FPS template.
  - **NO se importa** el rig LVA4 ni los FBX de Blender del Unity (`SK_FPSArms_LVA4.fbx`, `SK_Pistol_LVA4.fbx`). Daniel se desliga de Blender.
  - Cuando se modelen los enemigos finales, retargetear las mismas anims al modelo nuevo (cero re-trabajo).
- **HUD**: UMG (sistema nativo Unreal).
- **IA enemigos**: Behavior Trees + Blackboard + NavMesh + AI Perception. Estilo Serious Sam (simple, agresivo, oleadas).
- **Sombras enemigos**: híbrido por distancia — dynamic shadow cerca, distance field shadow medio, blob shadow / sin sombra lejos.
- **Sistema de armas**: BP_Weapon_Base + UDataAsset (WeaponData) — data-driven. Inventario 1/2/3 + rueda.

## Gore (decisión Daniel — actualizado jun 2026)
- **Sangre**: Niagara System `NS_BloodVoxel` (ya prototipado). Cubos voxel con física que rebotan, forman charcos, fade. Replica el look del proyecto Unity.
- **Desmembramiento — CON CHAOS (física de Unreal)**:
  - Se usa el sistema **Chaos Destruction / Chaos Flesh** de UE — más simple y nativo, sin pre-modelar piezas.
  - Al morir: ragdoll + fractura por física. Variantes de muerte (bala / explosión / melee) según fuerza/punto de impacto.
  - **Descartado**: modelo segmentado pre-cortado (Opción B antigua) — Daniel se desligó de Blender, así que pre-modelar piezas no encaja con el flujo nuevo.
- **NO usar**: cortes procedurales geométricos (mesh slicing) — overkill para low poly.

## Las 3 armas iniciales (planificadas)
| | Pistola | Escopeta | Rifle |
|---|---|---|---|
| Tipo | Semi | Semi (8 pellets) | Automático |
| Cargador | 15 | 6 | 30 |
| RPM | 400 | 90 | 600 |
| Daño/tiro | 25 | 12 × 8 | 18 |
| Spread | Bajo | Alto cónico | Medio |

## Enemigos planificados (Behavior Tree por cada uno)
- **Melee** (rojo): detecta → persigue → swing → cooldown
- **Kamikaze** (naranja): detecta → corre → explota
- **Ranged** (futuro): detecta → cobertura → dispara → reposiciona
- **Tanque** (futuro): detecta → camina lento → telegrafiar ataque

## Qué se reusa del proyecto Unity (actualizado jun 2026)
- ✅ **Audio completo** (90 archivos WAV/OGG/MP3 en `Content/Audio/`).
- ✅ **Balance/valores afinados** — extraídos a `docs/BALANCE.md` (vida, daños, hordas, IA). Solo los números, el C# se rehace.
- ✅ Spec del juego (`docs/VISION`, `ROADMAP`, `MAPA_FUNDICION`) + `docs/ANIMACIONES.md` como referencia de lista.
- ❌ **NO se reusan los FBX/rig LVA4 de Blender**. Daniel se desligó de Blender — anims = UE5 Mannequin + Mixamo + Fab.
- ❌ **NO se reusa ThirdParty del Unity** (LowPolyShooterPack, GabrielAguiar VFX, Vefects, Low Poly Weapon Series): licencias atadas a Unity Asset Store. Equivalentes UE = Niagara + Fab.

## Qué se rehace (no se migra)
- Scripts C# → **C++ / Blueprints**
- Animator Controllers → **Animation Blueprints**
- ScriptableObjects → **UDataAsset / DataTables**
- HUD (UI Toolkit) → **UMG**
- Sistema de armas/enemigos/hordas → reimplementado en Unreal

## Plan de fases (orden — actualizado jun 2026)
1. ✅ Setup proyecto + Git + LFS + GitHub
2. ✅ **MCP de Unreal instalado** (`chongdashu/unreal-mcp` en `Plugins/UnrealMCP/`).
3. ✅ **Audio migrado** del Unity (`Content/Audio/`) + balance extraído (`docs/BALANCE.md`).
4. 🟡 **Gore — sangre voxel**: `NS_BloodVoxel` prototipado (Niagara). Pendiente terminarlo y enganchar al daño.
5. 🟡 **Gore — desmembramiento**: prototipo con Chaos en curso.
6. ⬜ Visual del personaje FPS (Mannequin + Mixamo, sin Blender).
7. ⬜ Primera arma (Pistola) con `BP_Weapon_Base` + `WeaponData` (UDataAsset).
8. ⬜ Enemigos (melee + kamikaze) con Behavior Tree.
9. ⬜ Hordas + WaveSystem.
10. ⬜ HUD UMG + audio adaptativo.
11. ⬜ Mapa La Fundición.

## MCP / Automatización (actualizado jun 2026)
**MCP instalado y funcionando**: `chongdashu/unreal-mcp` en `Plugins/UnrealMCP/`. Server Python aparte; la ruta al server vive en `.mcp.json` (ignorado por gitignore, **es por-PC** — re-configurar en cada máquina, ver `docs/MCP.md`). Unreal también tiene Python nativo (`Window → Python Console`) para snippets puntuales.

## Notas técnicas críticas
- **Git LFS obligatorio** para `.uasset`, `.umap`, FBX, WAV, PNG grandes. Sin LFS el repo crece descontroladamente.
- **NO commitear**: `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/`, `*.sln`. Ya están en `.gitignore`.
- **Build first time**: 5-15 min (compila C++). Después es rápido.
- Daniel usa `git lfs install` ya configurado (3.7.1) en su PC.

## Referencias (proyecto Unity archivado)
Path local del proyecto Unity original (referencia, NO modificar): `C:\Users\Daniel\Documents\ASHFALL`. Repo: https://github.com/DanielOjeda25/ASHFALL.

## Idioma técnico Unreal (vs Unity, de donde vengo)
- **Actor** = lo que en Unity era GameObject.
- **Component** = igual concepto que Unity, se le pega a un Actor.
- **Blueprint** = visual scripting (no existe en Unity nativo, lo más parecido es Bolt).
- **Level** = lo que en Unity era Scene.
- **PlayerController** + **Pawn** + **GameMode** = arquitectura "Gameplay Framework" (más estructurada que Unity).
- **UMG** = el HUD/UI (lo que era UI Toolkit en Unity).
- Ventanas clave: **Content Browser** (assets), **Outliner** (jerarquía), **Details** (Inspector), **Viewport** (Scene), **Blueprint Editor** (visual scripting).
