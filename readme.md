# HLSL Builder

This is a project that uses a solution to compile a set of HLSL files to cso or spv. Is completely inspired in Visual Studio and its solution schema.

The builder is based in 4 pipelines:

## Graphics Pipeline (.gp) (will be the first pipeline implemented)

This is the most common shader pipeline, either GLSL and HLSL, involving the following stages: vertex, domain, hull, geometry and pixel, following the rules above:

* Vertex and Pixel stages are mandatory
* Geometry stage is optional
* If Domain stage is present, Hull is mandatory

## Compute Pipeline (.cp)

This is the pipeline used to compute data on GPU using the graphics API, it only has the compute stage

## Mesh Pipeline

This pipeline intend to replace the Graphics Pipeline, when more flexibility is necessary to the Geometry Stage, more details can be found [here](https://microsoft.github.io/DirectX-Specs/d3d/MeshShader.html). It has 3 stages, mesh, amplification and pixel, and all 3 are mandatories.
