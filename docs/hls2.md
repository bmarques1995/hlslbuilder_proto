# High Level Shading Language Solution (.hls2)

HLS2 is the pattern of solution that incorporates a set of HLSL shaders, is inspired on visual studio strategy of C/C++ solutions. It is a json text, storing a set of properties:

* `GraphicsPipelineSources` : stores all graphics pipeline shaders, the default shaders, to be built, all pipeline stages used by the shader must be present, all will be compiled at once, following some rules:
    * Vertex and Pixel shader are required
    * Geometry shader is optional
    * If Domain shader is present, hull shader is required
* `VulkanVersion` : stores the vulkan target version. Example: Vulkan 1.3 specs.
* `HLSLVersion` : stores the HLSL standard that will be compiled.
* `GraphicsPipeline->VertexEntry` : stores the vertex shader entryponint
* `GraphicsPipeline->PixelEntry` : stores the pixel shader entryponint
* `GraphicsPipeline->GeometryEntry` : stores the geometry shader entryponint
* `GraphicsPipeline->DomainEntry` : stores the domain shader entryponint
* `GraphicsPipeline->HullEntry` : stores the hull shader entryponint

Obs: The arrow `->` indicates a subproperty.

## Parameters Rules:

* `Version`: follows the pattern `<unsigned>.<unsigned>`
* `Name`: starts with a letter or `_` and follows a letter, a digit or a `_`
* `Sources`: must be valid paths

## FutureChanges(it will be removed from this section by implementation)

- Add include options
- Process GraphicsPipeline tokens:
    - InputLayout
    - ConstantBuffers(for all others pipelines)
- Add Compute Pipeline
- Add RayTracing Pipeline
- Add Mesh Pipeline
