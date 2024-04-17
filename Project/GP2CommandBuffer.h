#pragma once
#include "vulkan/vulkan_core.h"
#include <vector>

class GP2CommandBuffer
{
public:
	GP2CommandBuffer() = default;
	~GP2CommandBuffer() = default;

	void SetVkCommandBuffer(VkCommandBuffer buffer)
	{
		m_CommandBuffer = buffer;
	}

	VkCommandBuffer GetVkCommandBuffer()
	{
		return m_CommandBuffer;
	}

	void reset()const;
	void beginRecording(const VkRenderPass& renderPass, const std::vector<VkFramebuffer>& swapChainFramebuffers, int imageIdx, const VkExtent2D& swapChainExtent)const;
	void endRecording()const;
	
	void submit(VkSubmitInfo & info)const;

private:
	VkCommandBuffer m_CommandBuffer;
};