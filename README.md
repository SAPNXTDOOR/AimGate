A lightweight Windows utility that disables movement keys (W/A/S/D) while the fire button (Left Mouse Click) is held, and restores movement automatically when released.

## Features
- Disables movement while firing
- Automatically restores movement if keys are still held
- Minimal and lightweight

## How it works
- Uses low-level keyboard and mouse hooks
- Tracks physical key state
- Gates movement output while fire is held
- Uses SendInput safely (ignores injected input)
