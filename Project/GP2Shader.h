#pragma once
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>


class GP2Shader
{
public:
	GP2Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	:m_VertextShaderFile(vertexShaderFile)
	,m_FragmentShaderFile(fragmentShaderFile)
	{
	}

	virtual ~GP2Shader() = default;
	GP2Shader(const GP2Shader&) = delete;
	GP2Shader& operator=(const GP2Shader&) = delete;
	GP2Shader(const GP2Shader&&) = delete;
	GP2Shader& operator=(const GP2Shader&&) = delete;

	virtual VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
	virtual VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo();

	virtual void Initialize(const VkDevice& vkDevice);
	virtual const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() const { return m_ShaderStages; };
	virtual void DestroyShaderModules(const VkDevice& vkDevice);

protected:
	virtual VkPipelineShaderStageCreateInfo CreateFragmentShaderInfo(const VkDevice& vkDevice);
	virtual VkPipelineShaderStageCreateInfo CreateVertexShaderInfo(const VkDevice& vkDevice);
	virtual VkShaderModule CreateShaderModule(const VkDevice& vkDevice, const std::vector<char>& code);

	std::string m_VertextShaderFile{};
	std::string m_FragmentShaderFile{};

	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages{};
};

//class GP2Shader3D : public GP2Shader
//{
//public:
//	GP2Shader3D(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
//		:GP2Shader(vertexShaderFile, fragmentShaderFile)
//	{
//	}
//
//	//const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }
//
//	//void Cleanup(const VkDevice& vkDevice);
//private:
//	//VkDescriptorSetLayout m_DescriptorSetLayout{nullptr};
//};