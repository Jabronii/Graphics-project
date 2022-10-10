#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <vector>
#include <string>
#include <functional>
#include <iostream>
//code taken from the Cherno on youtube

class Scene
{
public:
	Scene();
	//virtual ~Scene();
	virtual void UpdateData(float deltaTime);
	virtual void Render();
	virtual void RenderImGUI();
};
class SceneMenu : public Scene
{
public:
	SceneMenu(Scene *& currentTestPointer);

	void RenderImGUI() override;
	void Render() override;

	template<typename T>
	void RegisterScene(const std::string& name);
private:
	Scene*& m_CurrentTest;
	std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;
};

template<typename T>
inline void SceneMenu::RegisterScene(const std::string& name)
{
	std::cout << "Registering scene " << name << std::endl;

	m_Scenes.push_back(std::make_pair(name, []() { return new T(); }));
}
