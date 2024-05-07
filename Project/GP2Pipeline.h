#pragma once
#include <memory>
#include "GP2DataBuffer.h"
#include "GP2Shader.h"
#include "GP2CommandPool.h"
#include "GP2Mesh.h"
#include "libraries/stb_image.h"

template<VertexConcept V>
class GP2GraphicsPipeline
{
public:
	GP2GraphicsPipeline(
		const std::string& vertexShaderFile,
		const std::string& fragmentShaderFile
	);
	void Initialize(const VkDevice& device, const VkPhysicalDevice& physDevice, const VkRenderPass& renderPass);
	VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo() { return m_Shader.CreateVertexInputStateInfo(); };
	VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo() { return m_Shader.CreateInputAssemblyStateInfo(); };
	void Cleanup();
	void Record(GP2CommandBuffer& buffer, int imageIdx, const VkExtent2D& swapChainExtent);
	void Update(uint32_t imageIdx, const glm::mat4& cameraToWorld);
	void AddMesh(GP2Mesh<V>& mesh, const GP2CommandPool& commandPool, const VkQueue& queue);
	void SetUBO(UniformBufferObject ubo);

	std::pair<VkImage, VkDeviceMemory> CreateTextureImage(const std::string& fileName, const GP2CommandPool& commandPool, const VkQueue& queue);
private:
	void CreateGraphicsPipeline();
	void CreateDescriptorSetLayout(const VkDevice& vkDevice);
	void DrawScene(GP2CommandBuffer& buffer, int imageIdx);

	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, const GP2CommandPool& commandPool, const VkQueue& queue);
	VkCommandBuffer BeginSingleTimeCommands(const GP2CommandPool& commandPool);
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer, const GP2CommandPool& commandPool, const VkQueue& queue);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, const GP2CommandPool& commandPool, const VkQueue& queue);

	VkDevice m_Device{VK_NULL_HANDLE};
	VkPhysicalDevice m_PhysDevice{VK_NULL_HANDLE};
	VkRenderPass m_RenderPass{};
	VkPipeline m_Pipeline{};
	VkPipelineLayout m_PipelineLayout{};
	VkDescriptorSetLayout m_DescriptorSetLayout;

	GP2Shader m_Shader;
	std::vector<GP2Mesh<V>> m_pMeshes{};
	//UniformBufferObject m_Ubo{};
};

template<VertexConcept V>
inline GP2GraphicsPipeline<V>::GP2GraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	:m_Shader{vertexShaderFile, fragmentShaderFile}
{	
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::Initialize(const VkDevice& device, const VkPhysicalDevice& physDevice, const VkRenderPass& renderPass)
{
	m_Device = device;
	m_PhysDevice = physDevice;
	m_RenderPass = renderPass;
	
	m_Shader.Initialize(device);
	CreateDescriptorSetLayout(device);
	CreateGraphicsPipeline();
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::Record(GP2CommandBuffer& buffer, int imageIdx, const VkExtent2D& swapChainExtent)
{
	vkCmdBindPipeline(buffer.GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

	DrawScene(buffer, imageIdx);
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::Update(uint32_t imageIdx, const glm::mat4& cameraToWorld)
{
	for (int i{}; i < m_pMeshes.size(); ++i)
	{
		m_pMeshes[i].GetWriteAbleUniformBuffer().Update(imageIdx, cameraToWorld);
	}
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::SetUBO(UniformBufferObject ubo)
{
	for (int i{}; i < m_pMeshes.size(); ++i)
	{
		m_pMeshes[i].GetWriteAbleUniformBuffer().SetUBO(ubo);
	}
}

template<VertexConcept V>
inline std::pair<VkImage, VkDeviceMemory> GP2GraphicsPipeline<V>::CreateTextureImage(const std::string& fileName, const GP2CommandPool& commandPool, const VkQueue& queue)
{
	VkImage textureImage{};
	VkDeviceMemory textureImageMemory;
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(fileName.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}


	GP2DataBuffer dataBuffer{ m_Device, m_PhysDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, imageSize };

	void* data;
	vkMapMemory(m_Device, dataBuffer.GetBufferMemory(), 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(m_Device, dataBuffer.GetBufferMemory());

	stbi_image_free(pixels);
	CreateImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandPool, queue);
	CopyBufferToImage(dataBuffer.GetBuffer(), textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), commandPool, queue);
	TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandPool, queue);

	//setup destroy when going out of scope here
	//vkDestroyBuffer(device, dataBuffer.GetBuffer(), nullptr);
	//vkFreeMemory(device, dataBuffer.GetBufferMemory(), nullptr);
	return std::pair<VkImage, VkDeviceMemory>{textureImage, textureImageMemory};
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::AddMesh(GP2Mesh<V>& mesh, const GP2CommandPool& commandPool, const VkQueue& queue)
{
	mesh.Initialize(m_Device, m_PhysDevice, commandPool.GetCommandPool(), queue, m_DescriptorSetLayout);
	m_pMeshes.push_back(mesh);
}

template <VertexConcept V>
inline void GP2GraphicsPipeline<V>::DrawScene(GP2CommandBuffer& buffer, int imageIdx)
{
	for (int i{}; i < m_pMeshes.size(); ++i)
	{
		VkBuffer vertexBuffers[]{ m_pMeshes[i].GetVertexBuffer().GetBuffer() };
		VkBuffer indexBuffers{ m_pMeshes[i].GetIndexBuffer().GetBuffer() };
		VkDeviceSize offsets[]{ 0 };
		vkCmdBindVertexBuffers(buffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(buffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(buffer.GetVkCommandBuffer()
			, VK_PIPELINE_BIND_POINT_GRAPHICS
			, m_PipelineLayout
			, 0
			, 1
			, &m_pMeshes[i].GetDescriptorPool().GetDescriptorSets()[imageIdx], 0, nullptr);

		vkCmdDrawIndexed(buffer.GetVkCommandBuffer(), static_cast<uint32_t>(m_pMeshes[i].GetIndices().size()), 1, 0, 0, 0);
	}
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(m_Device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(m_Device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = GP2DataBuffer::FindMemoryType(m_PhysDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(m_Device, image, imageMemory, 0);
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, const GP2CommandPool& commandPool, const VkQueue& queue)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands(commandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);


	EndSingleTimeCommands(commandBuffer, commandPool, queue);

}

template<VertexConcept V>
inline VkCommandBuffer GP2GraphicsPipeline<V>::BeginSingleTimeCommands(const GP2CommandPool& commandPool)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool.GetCommandPool();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::EndSingleTimeCommands(VkCommandBuffer commandBuffer, const GP2CommandPool& commandPool, const VkQueue& queue)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(m_Device, commandPool.GetCommandPool(), 1, &commandBuffer);
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, const GP2CommandPool& commandPool, const VkQueue& queue)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands(commandPool);

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};
	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	EndSingleTimeCommands(commandBuffer, commandPool, queue);
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::Cleanup()
{
	vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
	vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
	vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);

	for (int i{}; i < m_pMeshes.size(); ++i)
	{
		m_pMeshes[i].Cleanup();
	}
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::CreateGraphicsPipeline()
{
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;

	if (vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};

	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = m_Shader.GetShaderStages().data();
	VkPipelineVertexInputStateCreateInfo is = m_Shader.CreateVertexInputStateInfo();

	auto bindingDescription = V::GetBindingDescription();
	auto attributeDescriptions = V::GetAttributeDescriptions();
	is.vertexBindingDescriptionCount = 1;
	is.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	is.pVertexBindingDescriptions = &bindingDescription;
	is.pVertexAttributeDescriptions = attributeDescriptions.data();
	pipelineInfo.pVertexInputState = &is;
	auto inputAssemblyState = m_Shader.CreateInputAssemblyStateInfo();
	pipelineInfo.pInputAssemblyState = &inputAssemblyState;

#pragma region pipelineInfo
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_PipelineLayout;
	pipelineInfo.renderPass = m_RenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
#pragma endregion
	if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
	m_Shader.DestroyShaderModules(m_Device);
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::CreateDescriptorSetLayout(const VkDevice& vkDevice)
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;

	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	//VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	//pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	//pipelineLayoutInfo.setLayoutCount = 1;
	//pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
}