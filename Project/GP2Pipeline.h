#pragma once
#include <memory>
#include "GP2DataBuffer.h"
#include "GP2Shader.h"
#include "GP2CommandPool.h"
#include "GP2Mesh.h"

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
	void Update(uint32_t imageIdx);
	void AddMesh(GP2Mesh<V>& mesh, const GP2CommandPool& commandPool, const VkQueue queue);
	void SetUBO(UniformBufferObject ubo);
private:
	void CreateGraphicsPipeline();
	void CreateDescriptorSetLayout(const VkDevice& vkDevice);
	void DrawScene(GP2CommandBuffer& buffer, int imageIdx);

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

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(buffer.GetVkCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(buffer.GetVkCommandBuffer(), 0, 1, &scissor);

	DrawScene(buffer, imageIdx);
}

template<VertexConcept V>
inline void GP2GraphicsPipeline<V>::Update(uint32_t imageIdx)
{
	for (int i{}; i < m_pMeshes.size(); ++i)
	{
		//static auto startTime = std::chrono::high_resolution_clock::now();
		//
		//auto currentTime = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		//
		////UniformBufferObject ubo = m_pMeshes[i]->GetWriteAbleUniformBuffer().GetWritableUBO();
		//
		//m_Ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//m_Ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//m_Ubo.proj = glm::perspective(glm::radians(45.0f), WIDTH / static_cast<float>(HEIGHT), 0.1f, 10.0f);
		//m_Ubo.proj[1][1] *= -1;
		//
		//void* mappedLocation = m_pMeshes[i]->GetWriteAbleUniformBuffer().GetWritableMappedUniformBuffers()[imageIdx];
		//
		//memcpy(mappedLocation, &m_Ubo, sizeof(m_Ubo));

		m_pMeshes[i].GetWriteAbleUniformBuffer().Update(imageIdx);
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
inline void GP2GraphicsPipeline<V>::AddMesh(GP2Mesh<V>& mesh, const GP2CommandPool& commandPool, const VkQueue queue)
{
	mesh.Initialize(m_Device, m_PhysDevice, commandPool.GetCommandPool(), queue, m_DescriptorSetLayout);
	m_pMeshes.push_back(mesh);
}

template <VertexConcept V>
inline void GP2GraphicsPipeline<V>::DrawScene(GP2CommandBuffer& buffer, int imageIdx)
{
	for (int i{}; i < m_pMeshes.size(); ++i)
	{
		VkBuffer vertexBuffers[] = { m_pMeshes[i].GetVertexBuffer().GetBuffer() };
		VkBuffer indexBuffers = { m_pMeshes[i].GetIndexBuffer().GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(buffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(buffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT16);

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