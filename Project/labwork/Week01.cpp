#include "vulkanbase/VulkanBase.h"
#include "GP2Mesh.h"

void VulkanBase::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void VulkanBase::drawScene() {

	VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer()};
	VkBuffer indexBuffers = { indexBuffer.GetBuffer()};
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT16);

	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetVertices().size()), 1, 0, 0);
	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), 6, 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetIndices().size()), 1, 0, 0, 0);
}

void VulkanBase::drawScene3D(uint32_t imageIndex)
{

	VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer() };
	VkBuffer indexBuffers = { indexBuffer.GetBuffer() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT16);

	vkCmdBindDescriptorSets(  commandBuffer.GetVkCommandBuffer()
							, VK_PIPELINE_BIND_POINT_GRAPHICS
							, pipelineLayout
							, 0
							, 1
							, &descriptorPool.GetDescriptorSets()[imageIndex], 0, nullptr);

	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetVertices().size()), 1, 0, 0);
	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), 6, 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetIndices().size()), 1, 0, 0, 0);
}