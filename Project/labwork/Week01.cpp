#pragma once
#include "vulkanbase/VulkanBase.h"
//#include "GP2Mesh.h"

void VulkanBase::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        void* pUser = glfwGetWindowUserPointer(window);
        VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
        vBase->keyEvent(key, scancode, action, mods);
        });
    //glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    //    void* pUser = glfwGetWindowUserPointer(window);
    //    VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
    //    vBase->mouseMove(window, xpos, ypos);
    //    });
}

void VulkanBase::keyEvent(int key, int scancode, int action, int mods)
{
    //if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    //{
    //    m_Radius = std::max(3.0f, m_Radius - 0.2f);
    //}
    //if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    //{
    //    m_Radius = std::min(30.0f, m_Radius + 0.2f);
    //}
    m_Camera.KeyEvent(key, scancode, action, mods);
}

//void VulkanBase::drawScene() {
//
//	VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer()};
//	VkBuffer indexBuffers = { indexBuffer.GetBuffer()};
//	VkDeviceSize offsets[] = { 0 };
//	vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);
//
//	vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT16);
//
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetVertices().size()), 1, 0, 0);
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), 6, 1, 0, 0);
//	vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetIndices().size()), 1, 0, 0, 0);
//}

//void VulkanBase::drawScene3D(uint32_t imageIndex)
//{
//	VkBuffer vertexBuffers[] = { vertexBuffer.GetBuffer() };
//	VkBuffer indexBuffers = { indexBuffer.GetBuffer() };
//	VkDeviceSize offsets[] = { 0 };
//	vkCmdBindVertexBuffers(commandBuffer.GetVkCommandBuffer(), 0, 1, vertexBuffers, offsets);
//
//	vkCmdBindIndexBuffer(commandBuffer.GetVkCommandBuffer(), indexBuffers, 0, VK_INDEX_TYPE_UINT16);
//
//	vkCmdBindDescriptorSets(  commandBuffer.GetVkCommandBuffer()
//							, VK_PIPELINE_BIND_POINT_GRAPHICS
//							, pipelineLayout
//							, 0
//							, 1
//							, &descriptorPool.GetDescriptorSets()[imageIndex], 0, nullptr);
//
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetVertices().size()), 1, 0, 0);
//	//vkCmdDraw(commandBuffer.GetVkCommandBuffer(), 6, 1, 0, 0);
//	vkCmdDrawIndexed(commandBuffer.GetVkCommandBuffer(), static_cast<uint32_t>(mesh.GetIndices().size()), 1, 0, 0, 0);
//}