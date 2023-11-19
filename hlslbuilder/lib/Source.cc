#include "Source.hh"
#include "Utils/FileHandler.hh"
#include <filesystem>
#include "SolutionParser.hh"

HLSLBuilder::Source::Source(std::string_view path)
{
	FileHandler::ReadTextFile(path, &m_SourceCode);
	std::filesystem::path pathName(path.data());
	m_BaseName = pathName.stem().string();
	m_ParentPath = pathName.parent_path().string();
	std::replace(m_ParentPath.begin(), m_ParentPath.end(), '\\', '/');
}

void HLSLBuilder::Source::CompileVertexShader(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion)
{
	if (HLSLVersion.Major < 6)
		HLSLOldCompileVertex(buildMode, HLSLVersion);
	else
		HLSLNewCompileVertex(buildMode, outputTarget, HLSLVersion, VulkanVersion);
}

void HLSLBuilder::Source::CompilePixelShader(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion)
{
	if (HLSLVersion.Major < 6)
		HLSLOldCompilePixel(buildMode, HLSLVersion);
	else
		HLSLNewCompilePixel(buildMode, outputTarget, HLSLVersion, VulkanVersion);
}

void HLSLBuilder::Source::HLSLOldCompileVertex(BuildMode buildMode, Version HLSLVersion)
{
	ComPtr<ID3DBlob> errorBlob;
	std::stringstream treated_arg;
	treated_arg << "vs_" << HLSLVersion.Major << "_" << HLSLVersion.Minor;
	std::string target = treated_arg.str();
	treated_arg.str("");
	Json::Value solution = *SolutionParser::GetSolution();
	std::string entry = solution["GraphicsPipeline"]["VertexEntry"].asString();
	uint32_t compileFlags;
	if (buildMode == BuildMode::DEBUG)
		compileFlags = D3DCOMPILE_DEBUG;
	else
		compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;



	D3DCompile(m_SourceCode.c_str(), m_SourceCode.size(), nullptr, nullptr, nullptr,
		entry.c_str(), target.c_str(), compileFlags, 0, reinterpret_cast<ID3DBlob**>(m_VertexBlob.GetAddressOf()), errorBlob.GetAddressOf());
	if (errorBlob != nullptr)
	{
		if (errorBlob->GetBufferSize())
		{
			std::string error = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
			throw CompileErrorException(error);
		}
	}
	else
	{
		std::stringstream fileOut;
		fileOut << m_ParentPath << "/" << m_BaseName << ".gp.vs.cso";
		m_Properties["VertexFile"] = fileOut.str();
		FileHandler::WriteBinFile(fileOut.str().c_str(), reinterpret_cast<std::byte*>(m_VertexBlob->GetBufferPointer()), m_VertexBlob->GetBufferSize());
		Console::Log("{0} successfully compiled", fileOut.str());
	}
}

void HLSLBuilder::Source::HLSLOldCompilePixel(BuildMode buildMode, Version HLSLVersion)
{
	ComPtr<ID3DBlob> errorBlob;
	std::stringstream treated_arg;
	treated_arg << "ps_" << HLSLVersion.Major << "_" << HLSLVersion.Minor;
	std::string target = treated_arg.str();
	treated_arg.str("");
	Json::Value solution = *SolutionParser::GetSolution();
	std::string entry = solution["GraphicsPipeline"]["PixelEntry"].asString();
	uint32_t compileFlags;
	if (buildMode == BuildMode::DEBUG)
		compileFlags = D3DCOMPILE_DEBUG;
	else
		compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;



	D3DCompile(m_SourceCode.c_str(), m_SourceCode.size(), nullptr, nullptr, nullptr,
		entry.c_str(), target.c_str(), compileFlags, 0, reinterpret_cast<ID3DBlob**>(m_PixelBlob.GetAddressOf()), errorBlob.GetAddressOf());
	if (errorBlob != nullptr)
	{
		if (errorBlob->GetBufferSize())
		{
			std::string error = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
			throw CompileErrorException(error);
		}
	}
	else
	{
		std::stringstream fileOut;
		fileOut << m_ParentPath << "/" << m_BaseName << ".gp.ps.cso";
		m_Properties["PixelFile"] = fileOut.str();
		FileHandler::WriteBinFile(fileOut.str().c_str(), reinterpret_cast<std::byte*>(m_PixelBlob->GetBufferPointer()), m_PixelBlob->GetBufferSize());
		Console::Log("{0} successfully compiled", fileOut.str());
	}
		
}

void HLSLBuilder::Source::HLSLNewCompileVertex(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion)
{
#pragma region DxcStarter
	ComPtr<IDxcUtils> dxc_utils = { 0 };
	ComPtr<IDxcCompiler3> dxcompiler = { 0 };

	DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(dxc_utils.ReleaseAndGetAddressOf()));
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(dxcompiler.GetAddressOf()));

	DxcBuffer srcBuffer;
	srcBuffer.Ptr = &(*m_SourceCode.begin());
	srcBuffer.Size = m_SourceCode.size();
	srcBuffer.Encoding = 0;
#pragma endregion

#pragma region ArgPusher
	std::wstringstream treated_arg;
	std::vector<std::wstring> args;

	args.push_back(L"-Zpc");
	args.push_back(L"-HV");
	args.push_back(L"2021");
	args.push_back(L"-T");
	treated_arg << "vs_" << HLSLVersion.Major << "_" << HLSLVersion.Minor;
	args.push_back(treated_arg.str());
	treated_arg.str(L"");
	treated_arg.clear();

	if (outputTarget == OutputTarget::SPV)
	{
		args.push_back(L"-spirv");
		treated_arg << "-fspv-target-env=vulkan" << VulkanVersion.Major << "." << VulkanVersion.Minor;
		args.push_back(treated_arg.str());
		treated_arg.str(L"");
		treated_arg.clear();
	}

	args.push_back(L"-E");
	Json::Value solution = *SolutionParser::GetSolution();
	std::string entry = solution["GraphicsPipeline"]["VertexEntry"].asString();
	args.push_back(std::wstring(entry.begin(), entry.end()));
	if (buildMode == BuildMode::DEBUG)
		args.push_back(L"-Od");
	else
		args.push_back(L"-O3");

#pragma endregion

	std::vector<LPCWSTR> rawArgs;
	for (auto& i : args)
	{
		rawArgs.push_back(i.c_str());
	}

	ComPtr<IDxcResult> result;
	dxcompiler->Compile(&srcBuffer, rawArgs.data(), static_cast<uint32_t>(rawArgs.size()), nullptr, IID_PPV_ARGS(result.GetAddressOf()));

	ComPtr<IDxcBlobEncoding> errorBlob;
	result->GetErrorBuffer(errorBlob.GetAddressOf());

	result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(m_VertexBlob.GetAddressOf()), nullptr);

	if (errorBlob->GetBufferSize() > 0)
	{
		std::string error = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
		throw CompileErrorException(error);
	}
	else
	{
		std::stringstream fileOut;
		if (outputTarget == OutputTarget::SPV)
		{
			fileOut << m_ParentPath << "/" << m_BaseName << ".gp.vs.spv";
		}
		else
		{
			fileOut << m_ParentPath << "/" << m_BaseName << ".gp.vs.cso";
		}
		m_Properties["VertexFile"] = fileOut.str();
		FileHandler::WriteBinFile(fileOut.str().c_str(), reinterpret_cast<std::byte*>(m_VertexBlob->GetBufferPointer()), m_VertexBlob->GetBufferSize());
		Console::Log("{0} successfully compiled", fileOut.str());
	}
}

void HLSLBuilder::Source::HLSLNewCompilePixel(BuildMode buildMode, OutputTarget outputTarget, Version HLSLVersion, Version VulkanVersion)
{
#pragma region DxcStarter
	ComPtr<IDxcUtils> dxc_utils = { 0 };
	ComPtr<IDxcCompiler3> dxcompiler = { 0 };

	DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(dxc_utils.ReleaseAndGetAddressOf()));
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(dxcompiler.GetAddressOf()));

	DxcBuffer srcBuffer;
	srcBuffer.Ptr = &(*m_SourceCode.begin());
	srcBuffer.Size = m_SourceCode.size();
	srcBuffer.Encoding = 0;
#pragma endregion

#pragma region ArgPusher
	std::wstringstream treated_arg;
	std::vector<std::wstring> args;

	args.push_back(L"-Zpc");
	args.push_back(L"-HV");
	args.push_back(L"2021");
	args.push_back(L"-T");
	treated_arg << "ps_" << HLSLVersion.Major << "_" << HLSLVersion.Minor;
	args.push_back(treated_arg.str());
	treated_arg.str(L"");
	treated_arg.clear();

	if (outputTarget == OutputTarget::SPV)
	{
		args.push_back(L"-spirv");
		treated_arg << "-fspv-target-env=vulkan" << VulkanVersion.Major << "." << VulkanVersion.Minor;
		args.push_back(treated_arg.str());
		treated_arg.str(L"");
		treated_arg.clear();
	}

	args.push_back(L"-E");
	Json::Value solution = *SolutionParser::GetSolution();
	std::string entry = solution["GraphicsPipeline"]["PixelEntry"].asString();
	args.push_back(std::wstring(entry.begin(), entry.end()));
	if (buildMode == BuildMode::DEBUG)
		args.push_back(L"-Od");
	else
		args.push_back(L"-O3");

#pragma endregion

	std::vector<LPCWSTR> rawArgs;
	for (auto& i : args)
	{
		rawArgs.push_back(i.c_str());
	}

	ComPtr<IDxcResult> result;
	dxcompiler->Compile(&srcBuffer, rawArgs.data(), static_cast<uint32_t>(rawArgs.size()), nullptr, IID_PPV_ARGS(result.GetAddressOf()));
	
	result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(m_PixelBlob.GetAddressOf()), nullptr);
	
	ComPtr<IDxcBlobEncoding> errorBlob;
	result->GetErrorBuffer(errorBlob.GetAddressOf());

	if (errorBlob->GetBufferSize() > 0)
	{
		std::string error = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());
		throw CompileErrorException(error);
	}
	else
	{
		std::stringstream fileOut;
		if (outputTarget == OutputTarget::SPV)
		{
			fileOut << m_ParentPath << "/" << m_BaseName << ".gp.ps.spv";
		}
		else
		{
			fileOut << m_ParentPath << "/" << m_BaseName << ".gp.ps.cso";
		}
		m_Properties["PixelFile"] = fileOut.str();
		FileHandler::WriteBinFile(fileOut.str().c_str(), reinterpret_cast<std::byte*>(m_PixelBlob->GetBufferPointer()), m_PixelBlob->GetBufferSize());
		Console::Log("{0} successfully compiled", fileOut.str());
	}
}

const Json::Value* HLSLBuilder::Source::GetProperties() const
{
	return &m_Properties;
}
