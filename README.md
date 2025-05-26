# XOX Drums
by 4ms Company

These are x0x-inspired drum modules for the [MetaModule](https://metamodule.info) and [VCV Rack](https://vcvrack.com).

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

Briefly:

Clone the submodules:

```
git clone https://github.com/4ms/XOXDrums
cd XOXDrums
git submodule update --init --recursive
```

Install the prerequisites (cmake, ninja, arm-none-eabi-gcc v12.3, python3, and optionally the Rack SDK if you wish to compile for VCV Rack).

Build a MetaModule plugin:

```
make mm
```

The .mmplugin file will be in `metamodule-plugins/`.


Build and install a Rack plugin with:

```
make all
make install
```

