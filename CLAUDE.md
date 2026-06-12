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
- **Personajes placeholder**: UE5 Mannequin + anims gratis de **Mixamo** vía **IK Retargeter** (UE 5.4+). Cuando Daniel modele los enemigos finales, retargetear las mismas anims al modelo nuevo (cero re-trabajo).
- **HUD**: UMG (sistema nativo Unreal).
- **IA enemigos**: Behavior Trees + Blackboard + NavMesh + AI Perception. Estilo Serious Sam (simple, agresivo, oleadas).
- **Sombras enemigos**: híbrido por distancia — dynamic shadow cerca, distance field shadow medio, blob shadow / sin sombra lejos.
- **Sistema de armas**: BP_Weapon_Base + UDataAsset (WeaponData) — data-driven. Inventario 1/2/3 + rueda.

## Gore (decisión Daniel)
- **Sangre**: Niagara System. Cubos voxel con física que rebotan, forman charcos, fade. Replicar look del proyecto Unity (no la lógica — la lógica se rehace).
- **Desmembramiento — OPCIÓN B (modelo segmentado pre-cortado)**:
  - Daniel modela 2 versiones por enemigo en Blender: vivo (SkeletalMesh) + muerto (6-8 StaticMesh con cortes en articulaciones).
  - Al morir: BP "Enemigo_Desmembrado" reemplaza al vivo. Cada pedazo con rigidbody + collider + Niagara de sangre desde el muñón.
  - Variantes de muerte: bala (cabeza vuela), explosión (8 pedazos volando), melee (corte vertical), sangrado (entero + charco).
- **NO usar**: cortes procedurales (overkill para low poly).

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

## Qué se reusa del proyecto Unity (ASHFALL anterior)
- ✅ Animaciones FPS arms (Blender, rig LVA4) — FBX directo
- ✅ Modelos 3D, audio (WAV/OGG), texturas
- ✅ Spec del juego (los `docs/*` están copiados acá)
- ✅ Lista de animaciones (docs/ANIMACIONES.md)

## Qué se rehace (no se migra)
- Scripts C# → **C++ / Blueprints**
- Animator Controllers → **Animation Blueprints**
- ScriptableObjects → **UDataAsset / DataTables**
- HUD (UI Toolkit) → **UMG**
- Sistema de armas/enemigos/hordas → reimplementado en Unreal

## Plan de fases (orden)
1. ✅ Setup proyecto + Git + LFS + GitHub
2. 🟡 **Familiarización con el editor** (Daniel explora UE 5.5 + FPS template)
3. ⬜ Migrar visual del personaje (FPS arms LVA4 desde Blender)
4. ⬜ Re-implementar disparo sobre el FPS template (primera arma: pistola)
5. ⬜ Enemigos (melee + kamikaze)
6. ⬜ Hordas + WaveSystem
7. ⬜ HUD + audio adaptativo
8. ⬜ Mapa La Fundición

## MCP / Automatización
**Arrancamos SIN MCP**. Unreal tiene Python nativo en el editor (`Window → Python Console`), suficiente para snippets puntuales. Cuando haga falta más (revisar estado de la escena, hordas masivas), instalar **[chongdashu/unreal-mcp](https://github.com/chongdashu/unreal-mcp)** (UE 5.5 oficial, 2k stars).

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
