# CLAUDE.md — ASHFALL (Unreal Engine 5.5)

> **Leeme al empezar cualquier sesión en este proyecto.**

## Qué es esto
**ASHFALL** — horde shooter sci-fi decadente, estilo **Serious Sam** (hordas masivas + mapas grandes + low poly) en **Unreal Engine 5.5**. Migración desde ASHFALL Unity (jun 2026) por mejor stack visual y motor más robusto. **El diseño del juego no cambia** — sigue los specs de `docs/VISION.md`, `docs/ROADMAP.md`, `docs/MAPA_FUNDICION.md`.

## Estado actual (actualizado jun 2026)
Vertical slice en marcha. Hecho: setup + Git/LFS, **runreal MCP** (Python en editor), audio migrado, **gore sangre voxel enganchado al impacto**, **horda de enemigos Mannequin** (3 tipos melee/kamikaze/tank con velocidad+color, IA persecución + NavMesh), **sprint del jugador** (C++), **arena greybox** naranja de pruebas, y **migrado un mega sample** (armas Military Weapons + animaciones Lyra/ALS/GASP) para Fases 6-7.
Pendiente cercano: **perspectiva toggle 1ra/3ra (Serious Sam)** + **sistema de armas** (Fase 7) + **loop de combate** (enemigos atacan/mueren).

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
- **Perspectiva (DECISIÓN jun 2026): toggle 1ra/3ra persona** estilo Serious Sam (una tecla alterna). Implica **cuerpo completo visible** (no solo brazos `Mesh1P`): cámara en spring arm que va de la cabeza (1ra) a detrás (3ra), y el cuerpo `SKM_Manny` se ve en 3ra. Las armas se attachean al cuerpo. Esto revaloriza el mega sample (los montages Lyra animan el **cuerpo entero** disparando/recargando → perfecto para 3ra).
- **Animaciones (ACTUALIZADO jun 2026)**: **Cero Blender**. Todo gratuito y nativo UE:
  - Esqueleto base = **UE5 Mannequin** (Manny/Quinn), **cuerpo completo** (por el toggle 1ra/3ra).
  - **Locomoción = GASP** (Game Animation Sample, motion-matching) — migrado del mega sample. Reemplaza/mejora `ABP_Manny` para que el cuerpo se vea AAA en 3ra persona.
  - **Anims de arma = montages Lyra** (`AM_MM_Pistol/Rifle/Shotgun_Fire/Reload/Equip`) — migrados, sobre esqueleto Manny.
  - Fuentes extra: **Mixamo** (vía **IK Retargeter** + `RTG_Mannequin` del pack) para anims puntuales (ataques enemigos, etc.).
  - **NO se importa** el rig LVA4 ni FBX de Blender del Unity. Daniel se desligó de Blender.
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

## Assets externos migrados (mega sample, jun 2026)
Un proyecto UE sample (`ProjectMegaSample/`, **gitignored** — solo fuente, no se commitea) del que se extrajo a `Content/`:
- ✅ **Armas** (`_WeaponsPacks/`): Military Weapons Dark+Silver → **Pistols**, **Shotgun**, **Assault Rifle** (= pistola/escopeta/rifle), + sniper/lanzacohetes/lanzagranadas/cuchillo. 2 skins.
- ✅ **Animaciones** (`_AnimationLibrary/`): montages **Lyra** (fire/reload/equip por arma) + poses **ALS** + locomoción **GASP** (motion-matching).
- ✅ Soporte de anim: `Blueprints/` (AnimNotifies foley + AnimModifiers) + `Audio/Foley` + `Audio/Mix`.
- Todo en ecosistema Epic/UE (licencia libre) → sin el problema de los assets atados a Unity Asset Store.
- ⚠️ Migrado por filesystem (runreal estaba caído); verificar refs al esqueleto Manny al abrir; re-Migrar puntual si algo sale roto.

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
2. ✅ **MCP de Unreal**: `chongdashu` (`unreal`) + **`runreal`** (Python arbitrario, el que usamos). Ver memorias.
3. ✅ **Audio migrado** del Unity + balance extraído (`docs/BALANCE.md`).
4. ✅ **Gore — sangre voxel**: `NS_BloodVoxel` (color/escala/burst/colisión) **enganchado al impacto en enemigos** (tag `Enemy` en `BP_FirstPersonProjectile`).
5. 🟡 **Gore — desmembramiento**: prototipo con Chaos (`BP_GoreChunk`), pendiente integrar.
6. 🟡 **Personaje + perspectiva**: cuerpo completo Mannequin + **cámara toggle 1ra/3ra (Serious Sam)** + **GASP** (locomoción). Sprint (Shift) ✅ ya hecho.
7. ⬜ **Sistema de armas**: `BP_Weapon_Base` + `WeaponData` (UDataAsset), usando armas migradas (pistola/escopeta/rifle) + montages Lyra. Inventario 1/2/3.
8. 🟡 **Enemigos**: 3 tipos (melee/kamikaze/tank) con velocidad+color e IA persecución (`MoveTo` simple + NavMesh) ✅. Falta **Behavior Tree + ataque + vida/muerte** (loop de combate).
9. ⬜ Hordas + WaveSystem.
10. ⬜ HUD UMG + audio adaptativo.
11. ⬜ Mapa La Fundición (la arena actual es greybox de pruebas, prefijo `AF_Arena_`).

## MCP / Automatización (actualizado jun 2026)
Dos MCP de Unreal en `.mcp.json` (gitignored, **por-PC**):
- **`runreal`** (el que usamos para todo): expone `editor_run_python` (Python arbitrario en el editor). Instalado **global** (`npm i -g @runreal/unreal-mcp`) y el `.mcp.json` lo llama con `node <ruta>/dist/bin.js` (NO `npx` — el npx no spawneaba). Si reiniciás el editor, **cerrá+abrí Claude Code** para que reconecte (el Node no se auto-respawnea; ver [[mcp-unreal-runreal]]).
- **`unreal`** (chongdashu): plugin C++, limitado (sin Python). Backup.
- **Niagara interno** = punto ciego de ambos → se edita a mano en el editor.
- Unreal tiene Python nativo (`Window → Python Console`) para snippets.

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
