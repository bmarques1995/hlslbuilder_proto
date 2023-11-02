# List of HLSL Builder Commands

## Command Line Args

All arguments must follow this structure `arg:value`

This is the list of accepted arguments

* -b/--build: specifies the projet that will be built, must have a second argument with the extension .hls2, that stores all infos necessary for build.
* -c/--config: specifies if the build is in debug or release mode
* -api/--target-api: specifies the target renderer API: can be spv(SPIRV) or cso(Compiled Shader Object, for Direct3D)
