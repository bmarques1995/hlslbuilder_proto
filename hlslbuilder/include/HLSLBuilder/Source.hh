#pragma once

#include "HLSLBuilderAPI.hh"
#include "ArgParser.hh"
#include "Version.hh"
#include <wrl.h>
#include <dxcapi.h>
#include <d3dcompiler.h>
#include <json/json.h>

using Microsoft::WRL::ComPtr;

namespace HLSLBuilder
{
	class HLSLB_API Source
	{
	public:
		Source(std::string_view path);
		void CompileVertexShader(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion);
		void CompilePixelShader(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion);

		const Json::Value* GetProperties() const;
	private:
		void HLSLOldCompileVertex(BuildMode buildMode, Version HLSLVersion);
		void HLSLOldCompilePixel(BuildMode buildMode, Version HLSLVersion);
		void HLSLNewCompileVertex(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion);
		void HLSLNewCompilePixel(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion);

		ComPtr<IDxcBlob> m_VertexBlob;
		ComPtr<IDxcBlob> m_PixelBlob;

		std::string m_BaseName;
		std::string m_SourceCode;
		std::string m_ParentPath;

		Json::Value m_Properties;
	};
}