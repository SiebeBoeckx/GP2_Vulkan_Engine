#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <stdexcept>
#include <memory>
#include <iostream>
//#include "vulkanbase/VulkanUtil.h"
#include "GP2DescriptorPool.h"

//if changing, change where it's set for CMAKE!!!


template <VertexConcept V>
class GP2Mesh
{
public:
	GP2Mesh(std::vector<V> verticeVector, std::vector<uint32_t> indiceVector) : m_Vertices(verticeVector), m_Indices(indiceVector)
	{

	}

	void Initialize(const VkDevice& device, const VkPhysicalDevice& physDevice, const VkCommandPool commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
	void Cleanup();

	const std::vector<V>& GetVertices() const { return m_Vertices; }
	std::vector<V> GetVerticesCopy() { return m_Vertices; }
	const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
	std::vector<uint32_t> GetIndicesCopy() { return m_Indices; }
	const GP2VertexBuffer<V>& GetVertexBuffer() const { return m_VertexBuffer; }
	const GP2IndexBuffer<V> GetIndexBuffer() const { return m_IndexBuffer; }
	const GP2UniformBuffer<V>& GetUniformBuffer() const { return m_UniformBuffer; }
	GP2UniformBuffer<V>& GetWriteAbleUniformBuffer() { return m_UniformBuffer; }
	const GP2DescriptorPool& GetDescriptorPool() const { return m_DescriptorPool; }

	void SetTextureImage(const VkDevice& device, const VkPhysicalDevice& physDevice, const std::pair<VkImage, VkDeviceMemory>& imagePair);
	void SetPBRTextures(const VkDevice& device, const VkPhysicalDevice& physDevice, const std::vector<std::pair<VkImage, VkDeviceMemory>>& pbrPairs);

private:
	VkDevice m_Device{ VK_NULL_HANDLE };

	const std::vector<V> m_Vertices{};

	const std::vector<uint32_t> m_Indices{};

	GP2VertexBuffer<V> m_VertexBuffer;
	GP2IndexBuffer<V> m_IndexBuffer;
	GP2UniformBuffer<V> m_UniformBuffer;
	GP2DescriptorPool m_DescriptorPool{};

	VkImage m_TextureImage{ nullptr };
	VkDeviceMemory m_TextureImageMemory{ nullptr };
	VkImageView m_TextureImageView{ nullptr };
	VkSampler m_TextureSampler{ nullptr };
	bool m_UsingTexture{ false };

	std::vector<VkImage> m_PBRImages{};
	std::vector<VkDeviceMemory> m_PBRImageMemories{};
	std::vector<VkImageView> m_PBRImageViews{};
	std::vector<VkSampler> m_PBRSamplers{};
	bool m_UsingPBR{ false };
	
	void CreateTextureSampler(const VkDevice& device, const VkPhysicalDevice& physDevice);
	void CreatePBRSampler(const VkDevice& device, const VkPhysicalDevice& physDevice);
};

template<VertexConcept V>
inline void GP2Mesh<V>::Initialize(const VkDevice& device, const VkPhysicalDevice& physDevice, const VkCommandPool commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout)
{
	m_Device = device;

	m_VertexBuffer.Initialize(device, physDevice, commandPool, graphicsQueue);
	m_VertexBuffer.CreateBuffer(*this);
	m_IndexBuffer.Initialize(device, physDevice, commandPool, graphicsQueue);
	m_IndexBuffer.CreateBuffer(*this);
	m_UniformBuffer.Initialize(device, physDevice, commandPool, graphicsQueue);
	m_UniformBuffer.CreateBuffer(*this);
	if (!m_UsingPBR)
	{
		m_DescriptorPool.InitializeTexture(device);
		m_DescriptorPool.CreateDescriptorSetsTextures(descriptorSetLayout, m_UniformBuffer, m_TextureImageView, m_TextureSampler);
	}
	else
	{
		m_DescriptorPool.InitializePBR(device);
		m_DescriptorPool.CreateDescriptorSetsPBR(descriptorSetLayout, m_UniformBuffer, m_PBRImageViews, m_PBRSamplers);
	}
}

template<VertexConcept V>
inline void GP2Mesh<V>::Cleanup()
{
	m_VertexBuffer.Cleanup();
	m_IndexBuffer.Cleanup();
	m_UniformBuffer.Cleanup();
	m_DescriptorPool.Destroy();

	for (auto& sampler : m_PBRSamplers)
	{
		vkDestroySampler(m_Device, sampler, nullptr);
	}
	for (auto& imageView : m_PBRImageViews)
	{
		vkDestroyImageView(m_Device, imageView, nullptr);
	}

	for (int i{}; i < m_PBRImageMemories.size(); ++i)
	{
		if (i != 0)
		{
			vkFreeMemory(m_Device, m_PBRImageMemories[i], nullptr);
		}
	}
	for (int i{}; i < m_PBRImages.size(); ++i)
	{
		if (i != 0)
		{
			vkDestroyImage(m_Device, m_PBRImages[i], nullptr);
		}
	}

	vkDestroySampler(m_Device, m_TextureSampler, nullptr);
	vkDestroyImageView(m_Device, m_TextureImageView, nullptr);
	
	vkFreeMemory(m_Device,m_TextureImageMemory, nullptr);
	vkDestroyImage(m_Device, m_TextureImage, nullptr);

}

template<VertexConcept V>
inline void GP2Mesh<V>::SetTextureImage(const VkDevice& device, const VkPhysicalDevice& physDevice, const std::pair<VkImage, VkDeviceMemory>& imagePair)
{
	m_TextureImage = imagePair.first;
	m_TextureImageMemory = imagePair.second;

	m_TextureImageView = CreateImageView(device, m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	CreateTextureSampler(device, physDevice);

	m_UsingTexture = true;

	m_PBRImages.push_back(imagePair.first);
	m_PBRImageMemories.push_back(imagePair.second);

	m_PBRImageViews.push_back(CreateImageView(device, m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT));
	CreatePBRSampler(device, physDevice);
}

template<VertexConcept V>
inline void GP2Mesh<V>::SetPBRTextures(const VkDevice& device, const VkPhysicalDevice& physDevice, const std::vector<std::pair<VkImage, VkDeviceMemory>>& pbrPairs)
{
	if (pbrPairs.size() != 3)
	{
		std::cout << "Wrong number pbr textures, current amount: " << pbrPairs.size() << '\n';
		return;
	}

	for (auto texPair : pbrPairs)
	{
		m_PBRImages.push_back(texPair.first);
		m_PBRImageMemories.push_back(texPair.second);

		m_PBRImageViews.push_back(CreateImageView(device, texPair.first, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT));
		CreatePBRSampler(device, physDevice);
	}

	m_UsingPBR = true;
}

template<VertexConcept V>
inline void GP2Mesh<V>::CreateTextureSampler(const VkDevice& device, const VkPhysicalDevice& physDevice)
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physDevice, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	/*samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;*/

	if (vkCreateSampler(device, &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

template<VertexConcept V>
inline void GP2Mesh<V>::CreatePBRSampler(const VkDevice& device, const VkPhysicalDevice& physDevice)
{
	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physDevice, &properties);

	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	/*samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;*/

	VkSampler* newSampler = new VkSampler{};

	if (vkCreateSampler(device, &samplerInfo, nullptr, newSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}

	m_PBRSamplers.push_back(*newSampler);
}
