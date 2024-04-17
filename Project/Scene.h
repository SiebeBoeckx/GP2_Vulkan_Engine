#pragma once
#include "GP2Pipeline.h"

class Scene
{
public:
	Scene(const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
		:m_CommandPool{commandPool}
		,m_GraphicsQueue{graphicsQueue}
	{}
	~Scene() = default;

	template<VertexConcept V>
	void Create3DScene(GP2GraphicsPipeline<V>& pipeline)
	{
		std::vector<Vertex3D> vertices{ Vertex3D{glm::vec3{-0.5f, -0.5f, 0.f}, glm::vec3{1.f, 0.f, 0.f}},
									Vertex3D{glm::vec3{0.5f, -0.5f, 0.f}, glm::vec3{0.f, 1.f, 0.f}},
									Vertex3D{glm::vec3{0.5f, 0.5f, 0.f}, glm::vec3{0.f, 0.f, 1.f}},
									Vertex3D{glm::vec3{-0.5f, 0.5f, 0.f}, glm::vec3{1.f, 1.f, 1.f}} };

		std::vector<uint16_t> indices{ 0, 1, 2, 2, 3, 0 };

		GP2Mesh<Vertex3D> mesh{ vertices, indices };

		pipeline.AddMesh(mesh, m_CommandPool, m_GraphicsQueue);

		std::vector<Vertex3D> vertices2{ Vertex3D{glm::vec3{-0.6f, -0.6f, 0.f}, glm::vec3{1.f, 1.f, 1.f}},
									Vertex3D{glm::vec3{0.6f, -0.6f, 0.f}, glm::vec3{1.f, 1.f, 1.f}},
									Vertex3D{glm::vec3{0.6f, 0.6f, 0.f}, glm::vec3{1.f, 1.f, 1.f}},
									Vertex3D{glm::vec3{-0.6f, 0.6f, 0.f}, glm::vec3{1.f, 1.f, 1.f}} };

		std::vector<uint16_t> indices2{ 0, 1, 2, 2, 3, 0 };

		GP2Mesh<Vertex3D> mesh2{ vertices, indices };

		pipeline.AddMesh(mesh2, m_CommandPool, m_GraphicsQueue);
	}
private:
	GP2CommandPool m_CommandPool{};
	VkQueue m_GraphicsQueue{};
};
