# Animaciones — Qué necesitamos (fuente: Mixamo + Fab + UE Mannequin)

> Lista viva de animaciones a conseguir. **Cero Blender** — todo se saca de
> Mixamo (retarget con IK Retargeter), Fab/Marketplace o el FPS template.
> Esqueleto base = **UE5 Mannequin**.

## Brazos FPS (viewmodel) — por ARMA
Cada arma necesita su set. El template ya trae varias para el rifle (reusar/retargetear).

| Animación | Qué es | Prioridad | Nota |
|---|---|---|---|
| **Idle** | Quieta, respiración en loop | — | El template ya trae `FP_Rifle_Idle` |
| **Fire / Shot** | Kick + corredera atrás 1 frame | **Crítica** | Sin esto el arma no dispara visualmente |
| **Reload** | Recarga normal (con cargador) | Alta | |
| **Reload Empty** | Recarga con corredera trabada | Media | |
| **Draw / Equip** (sacar) | El arma sube al equiparla | Alta | Para el cambio de armas (Fase 4+) |
| **Holster** (guardar) | El arma baja al cambiar | Alta | holster 0.4s (ver BALANCE.md) |
| **Run / Sprint** | Pose al correr con el arma | Media | Template trae `FP_Rifle_Run` |
| **Jump / Fall / Land** | Manos en el aire | Media | Template trae las del rifle |
| Inspect | Revisar el arma (flourish) | Baja | |
| Aim / ADS | Apuntar con mira | Opcional | Estilo Serious Sam no suele usar |

## Enemigos (cuerpo completo — Mixamo)
Sacar de Mixamo y retargetear al esqueleto de cada enemigo. Mismo set sirve para
todos los humanoides; retargetear una vez por modelo.

| Animación | Melee | Kamikaze | Ranged | Tank |
|---|---|---|---|---|
| Idle | ✓ | ✓ | ✓ | ✓ |
| Walk / Run (persecución) | ✓ | ✓ (correr) | ✓ | ✓ (lento) |
| Attack | swing | — (explota) | dispara | telegrafiado |
| Hit / Stagger (knockback) | ✓ | ✓ | ✓ | ✓ |
| Death | (ver gore) | (explota) | ✓ | ✓ |

> **Muerte + gore**: el desmembramiento NO es una animación — se resuelve por física
> (ragdoll / chunks con impulso) + Niagara de sangre voxel. Ver decisión en CLAUDE.md.

## Orden sugerido (lo que mueve la aguja)
1. **Fire pistola** — sin esto no hay disparo visual. *Crítica.*
2. **Reload pistola** (normal + empty).
3. **Draw/Holster** — para el cambio de armas.
4. **Set del primer enemigo melee** (idle, run, attack, hit) desde Mixamo.

## Pipeline en Unreal (reemplaza el viejo de Blender)
- **Mixamo → UE**: descargar FBX sin skin → importar → **IK Retargeter** del esqueleto
  Mixamo al UE Mannequin → exportar las anims retargeteadas.
- **Animation Blueprint** (State Machine) maneja las transiciones idle/run/jump.
- **Fire** se dispara como **AnimMontage** (como ya hace el template en `Fire()`).
- Eventos de animación (notifies) para sincronizar sonido de recarga (mag_in/out, slide).
