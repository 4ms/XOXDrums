# XOX Drums
by 4ms Company

These are x0x-inspired drum modules for the MetaModule and VCV Rack.

Drums:
- Clap
- Clave
- Conga/bongo
- Cowbell
- Hi hat
- Kick
- Maraca
- Rimshot
- Snare
- Tom

Processors:
- Accent
- Duck

See full documentations here: [4ms XOX Drums User Manual](doc/XOX%20Drums%20User%20Manual.md)

## Compiling

For full documentation about compiling and customizing this project, see [CoreModule Template README](doc/CoreModule-template.md).

In short, after installing the prerequisites (cmake, ninja, arm-none-eabi-gcc v12.3, python3, and optionally the Rack SDK if you wish to compile for VCV Rack),
build a MetaModule plugin with:

```
make mm
ls -l metamodule-plugins/
```

Or build and install a Rack plugin with:

```
make all
make install
```


