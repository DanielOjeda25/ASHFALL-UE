# MCP — Puente Claude ↔ Unreal Engine

> Permite que el agente (Claude) lea la escena, spawnee actors, cree Blueprints, etc.
> directamente en el editor. Usa [chongdashu/unreal-mcp](https://github.com/chongdashu/unreal-mcp).

## Arquitectura (3 piezas, todas tienen que estar vivas)
```
Claude Code  →  Servidor MCP (Python/uv)  →  Plugin UnrealMCP (dentro del editor)
                                          TCP 127.0.0.1:55557
```
**El editor de Unreal TIENE que estar abierto** para que el MCP funcione. Si lo cerrás,
Claude pierde la conexión.

## Cómo se conecta (uso normal)
1. Abrir `AshfallUE.uproject` (el editor levanta el plugin → escucha en el puerto 55557).
2. Tener Claude Code abierto en esta carpeta (lee `.mcp.json` y lanza el server Python).
3. Listo: las herramientas `unreal` quedan disponibles para el agente.

> Si Claude no ve el MCP, reiniciar Claude Code para que relea `.mcp.json`.

## Estado de instalación (esta PC)
- ✅ Plugin copiado a `Plugins/UnrealMCP/` (módulo Editor, compilado).
- ✅ Repo del server clonado en `C:/Users/dani_/unreal-mcp/`.
- ✅ Entorno Python creado con `uv sync` en `C:/Users/dani_/unreal-mcp/Python`.
- ✅ `.mcp.json` en la raíz del proyecto apuntando a esa carpeta Python.
- ✅ `VisualStudioTools` desactivado en el `.uproject` (bloqueaba la compilación; no se usa).

## Requisitos
- Unreal Engine **5.5+**, Python **3.12+**, `uv` instalado.
- Visual Studio 2022 con toolchain C++ (para compilar el plugin).

## ⚠️ Montar en OTRA PC (lo que NO se versiona)
El plugin compilado y el `.mcp.json` son específicos de cada máquina. En otra PC:
1. Clonar el server: `git clone https://github.com/chongdashu/unreal-mcp.git`
2. `cd unreal-mcp/Python && uv sync`
3. Copiar `unreal-mcp/MCPGameProject/Plugins/UnrealMCP` → `ASHFALL-UE/Plugins/UnrealMCP`
   (o dejar el que ya está versionado en `Plugins/` si se decide commitearlo).
4. Crear `.mcp.json` en la raíz del proyecto con la **ruta local** a `unreal-mcp/Python`:
   ```json
   {
     "mcpServers": {
       "unreal": {
         "command": "uv",
         "args": ["--directory", "<RUTA_LOCAL>/unreal-mcp/Python", "run", "unreal_mcp_server.py"]
       }
     }
   }
   ```
5. Recompilar: doble clic al `.uproject` → "rebuild missing modules?" → Yes.

## Herramientas que expone el MCP
editor (spawn/inspect actors, transforms) · blueprint (crear/editar BPs) · nodes
(grafo de Blueprints) · project · umg (widgets de UI). Log del server: `unreal-mcp/Python/unreal_mcp.log`.

## Notas
- El plugin compilado vive en `Plugins/UnrealMCP/Binaries|Intermediate/` → **ignorado por git**.
  El `Source/` del plugin sí está en disco; decidir si se commitea (hace el repo autocontenido).
- Si el build falla por `VisualStudioTools`, ya está desactivado — no reactivar.
