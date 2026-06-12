# Migración Unity → Unreal Engine 5.5

> Estado vivo de la migración. Si volvés a este proyecto después de un tiempo, leé esto primero.

## Por qué migramos
Jun 2026, después de cerrar el sistema de pistola en Unity, Daniel decidió cambiar de motor:
- **Visualmente** Unity URP no estaba dando el nivel que quería (sin pasar a HDRP y re-aprender)
- Quería **probar Unreal** (curiosidad + futuro profesional)
- **El diseño del juego no cambia** — sigue siendo Serious Sam-like sci-fi decadente con hordas + low poly
- Acepta empezar de cero técnicamente

## Estado al inicializar (esta sesión)
✅ Hecho:
- Proyecto Unreal creado: **`AshfallUE`** (FPS template, C++, Scalable preset, Starter Content, sin Lumen)
- `.gitignore` y `.gitattributes` (LFS) configurados para Unreal
- `CLAUDE.md` con todas las decisiones técnicas
- Docs de diseño migrados desde Unity (VISION, ROADMAP, MAPA_FUNDICION, ANIMACIONES, AUDIO_TODO)
- Repo en GitHub: **ASHFALL-UE**

🟡 En curso:
- **Fase 2: Familiarización con el editor** — Daniel explora el FPS template default

⬜ Próximo (orden):
1. **Importar FPS arms del rig LVA4** (Blender → Unreal). Es donde Daniel más laburo tiene puesto.
2. **Migrar mira/crosshair** a UMG.
3. **Primera arma (Pistola) sobre el FPS template** — usar el sistema de armas base de UE y meter `WeaponData` (UDataAsset).
4. **Anims de Mixamo + UE5 Mannequin** para primer enemigo placeholder.
5. **Behavior Tree del enemigo melee**.

## Decisiones técnicas (resumen — detalle en CLAUDE.md)
| Tema | Decisión |
|---|---|
| Engine | UE 5.5 (no 5.7 — estabilidad y compatibilidad MCPs) |
| Iluminación | **Hybrid** — baked (Lightmass) ambiente + movable (sin sombras dinámicas) acciones |
| Lumen | OFF (no encaja con hordas + low poly) |
| Quality Preset | **Scalable** |
| HUD | UMG nativo |
| IA | Behavior Trees + Blackboard |
| Armas | BP_Weapon_Base + WeaponData (UDataAsset) — data-driven |
| Inventario | Slots 1/2/3 + rueda (como WeaponSwitch del Unity) |
| Personajes placeholder | UE5 Mannequin + Mixamo via IK Retargeter |
| Sombras enemigos | Híbrido por distancia |
| Gore — sangre | Niagara voxel (replica look del Unity) |
| Gore — desmembramiento | **Modelo segmentado pre-cortado** (Opción B): 2 versiones por enemigo |
| MCP | Empezamos sin. Si hace falta: `chongdashu/unreal-mcp` |

## Qué se reusa del proyecto Unity
- ✅ Animaciones FPS arms (rig LVA4 en `BlenderWork/LVAA.blend` del Unity)
- ✅ Modelos 3D propios, audio, texturas (FBX/WAV/PNG directo)
- ✅ Diseño del juego (`docs/*` copiados acá)
- ✅ Lista de animaciones en `docs/ANIMACIONES.md` (válida igual para Unreal)

## Qué se rehace
- C# → C++ / Blueprints
- Animator Controllers → Animation Blueprints (AnimBP)
- ScriptableObjects (WeaponData, EnemyData, MapAudio) → UDataAsset / DataTable
- UI Toolkit → UMG
- WeaponManager/EnemyPool/WaveSystem → reimplementados al estilo Unreal
- LpspBulletDamage, PlayerHealth, EnemyHealth → arquitectura Unreal (Gameplay Framework)

## Referencias del proyecto Unity
- Path local: `C:\Users\Daniel\Documents\ASHFALL` (NO modificar — archivo de consulta)
- Repo: https://github.com/DanielOjeda25/ASHFALL
- Animaciones FPS arms: `BlenderWork/LVAA.blend` (la fuente, mismo .blend se usa acá)

## Conceptos clave de Unreal que Daniel va a aprender (orden)
1. **Editor + Viewport** (ya en proceso)
2. **Blueprint** (variables, funciones, eventos, casting)
3. **UMG** para el HUD
4. **Animation Blueprint** (AnimBP + State Machine)
5. **IK Retargeter** (Mixamo → UE Mannequin)
6. **Behavior Tree + Blackboard** (IA)
7. **DataAsset / DataTable** (datos de armas/enemigos)
8. **Niagara** (sangre voxel)
9. **NavMesh** (pathfinding)
10. **C++** (cuando los Blueprints empiecen a quedar cortos)
