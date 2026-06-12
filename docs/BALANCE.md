# BALANCE — valores afinados (migrados del ASHFALL Unity)

> Números ya balanceados en el proyecto Unity. **No re-inventar** — al crear los
> `UDataAsset` / DataTables en Unreal, partir de acá. El código C# NO se migra;
> estos son solo los valores.

## Combate base
| Valor | Número | Notas |
|---|---|---|
| Vida player | 100 | `Core/Health.maxHealth` |
| Vida enemigo base | 100 | escala +15%/oleada (ver Hordas) |
| Daño bala (genérico) | 25 | `LpspBulletDamage.damage` |
| Holster (guardar arma) | 0.4 s | tiempo de cambio de arma |
| Volumen ambiente | 0.5 | `MapAudio.ambientVolume` |

## Armas iniciales (de CLAUDE.md, sin cambios)
| | Pistola | Escopeta | Rifle |
|---|---|---|---|
| Tipo | Semi | Semi (8 pellets) | Automático |
| Cargador | 15 | 6 | 30 |
| RPM | 400 | 90 | 600 |
| Daño/tiro | 25 | 12 × 8 | 18 |
| Spread | Bajo | Alto cónico | Medio |

## Enemigos
| Enemigo | Daño | Detalle |
|---|---|---|
| Melee | 10 | `MeleeAttack.damage` |
| Kamikaze | 40 | radio 3.5 m · knockback 6 · shake 0.5 · VFX life 2 s |
| Ranged | — | (stats en prefab, pendiente de extraer si se reusa) |
| Tank | — | (idem) |

### IA común (EnemyAI)
| Valor | Número |
|---|---|
| Rango de detección | 25 m |
| Rango de ataque | 2 m |
| Cooldown de ataque | 1 s |
| Repath interval | 0.2 s |
| Knockback duration | 0.08 s |
| Knockback multiplier | 1 |

### Pesos de spawn (probabilidad relativa en oleadas)
| Enemigo | spawnWeight |
|---|---|
| Melee | 1 |
| Kamikaze | 3 |

## Hordas (WaveSystem) — modo survival
| Valor | Número | Notas |
|---|---|---|
| totalWaves | 0 | 0 = infinitas (sin victoria); N = finitas |
| baseEnemies | 5 | enemigos en la oleada 1 |
| enemiesGrowthPerWave | +3 | por cada oleada siguiente |
| maxAliveAtOnce | 20 | tope de vivos a la vez |
| spawnBatchSize | 5 | cuántos intenta meter por tick |
| spawnInterval | 0.5 s | entre tandas de spawn |
| maxSpawnFailTicks | 5 | anti-cuelgue |
| timeBetweenWaves | 5 s | descanso tras oleada 1 |
| restReductionPerWave | -0.5 s | se acorta por oleada |
| minTimeBetweenWaves | 1.5 s | suelo del descanso |
| firstWaveDelay | 2 s | margen antes de la 1ª oleada |
| **healthGrowthPerWave** | **+15%** | vida de enemigos por oleada |
| **speedGrowthPerWave** | **+5%** | velocidad por oleada |
| maxSpeedMultiplier | ×2 | tope de velocidad |

## Audio adaptativo (MapAudio / MusicManager)
- Dos mapas, cada uno con par **peace ↔ fight** (`Content/Audio/Music/`).
- Crossfade entre peace y fight según haya combate. Re-implementar el patrón en UE
  (no el código): un `peace` loop y un `fight` loop por mapa, cruzar volumen.

## Pendiente de extraer (si se decide reusar Ranged/Tank)
Las stats de velocidad (NavMeshAgent), vida específica y daño de Ranged/Tank viven
en los prefabs de Unity, no en los scripts. Extraer cuando lleguemos a esos enemigos.
