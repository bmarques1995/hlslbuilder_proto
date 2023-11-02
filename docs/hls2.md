# High Level Shading Language Solution (.hls2)

HLS2 is the pattern of solution that incorporates a set of HLSL shaders, is inspired on visual studio strategy of C/C++ solutions. It is a json text, storing a set of properties:

* `Sources` : stores all shaders to be built, all pipeline stages used by the shader must be present, all will be compiled by once, following some rules:
  * If is a compute pipeline, only the compute shader main must be present
  * Otherwise:
    * Vertex and Pixel shader are required
    * Geometry shader is optional
    * If Domain shader is present, hull shader is required
  * Mesh pipeline will not be considered this time
* `VulkanVersion` : stores the vulkan target version. Example: Vulkan 1.3 specs.
* `HLSLVersion` : stores the HLSL standard that will be compiled
* `VertexEntry` : stores the vertex shader entryponint
* `PixelEntry` : stores the pixel shader entryponint
* `GeometryEntry` : stores the geometry shader entryponint
* `DomainEntry` : stores the domain shader entryponint
* `HullEntry` : stores the hull shader entryponint
* `ComputeEntry` : stores the compute shader entryponint
