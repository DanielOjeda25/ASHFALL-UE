# ASHFALL (Unreal Engine 5.5)

> Horde shooter sci-fi decadente estilo **Serious Sam**, en **Unreal Engine 5.5**.

Migración desde [ASHFALL Unity](https://github.com/DanielOjeda25/ASHFALL) (jun 2026).
El **diseño del juego no cambia** — sigue los specs de `docs/`.

## Estado
**Recién inicializado.** Setup completo, exploración del FPS template en curso.
Ver `docs/MIGRACION.md` para el estado vivo de la migración.

## Stack
- **Unreal Engine 5.5** (C++ + Blueprints)
- Iluminación **baked + dinámica acotada** (sin Lumen, sin RT)
- HUD con **UMG**, IA con **Behavior Trees**
- Armas data-driven con **UDataAsset**
- Personajes low poly, gore voxel (Niagara)

## Estructura
```
AshfallUE/
├── CLAUDE.md            # Contexto para el agente IA (leeme primero)
├── docs/                # Diseño del juego (visión, roadmap, mapa, animaciones)
│   ├── VISION.md
│   ├── ROADMAP.md
│   ├── MAPA_FUNDICION.md
│   ├── ANIMACIONES.md
│   ├── AUDIO_TODO.md
│   └── MIGRACION.md     # Estado de la migración Unity → Unreal
├── Source/              # Código C++
├── Content/             # Assets (LFS)
└── Config/              # Configuración del proyecto
```

## Setup para clonar en otra PC
```bash
git lfs install                                          # solo la primera vez
git clone https://github.com/DanielOjeda25/ASHFALL-UE.git
cd ASHFALL-UE
# Doble click en AshfallUE.uproject → Unreal pregunta si compilar → Yes
# Primera compilación: 5-15 min. Después rápido.
```

## Referencias visuales
Ultrakill · DUSK · Selaco · Cultic · Serious Sam HD · Doom Eternal
