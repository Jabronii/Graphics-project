#include "Scene.h"
SceneMenu::SceneMenu(Scene *& currentTestPointer)
	: m_CurrentTest(currentTestPointer)
{
}

void SceneMenu::RenderImGUI()
{
	for (auto& scene : m_Scenes)
	{
		if (ImGui::Button(scene.first.c_str()))
		{
			m_CurrentTest = scene.second();
		}
	}
}

void SceneMenu::Render()
{
	glClearColor(0.f, 0.0f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Scene::Scene()
{
}

void Scene::UpdateData(float deltaTime)
{
}

void Scene::Render()
{
}

void Scene::RenderImGUI()
{
}
