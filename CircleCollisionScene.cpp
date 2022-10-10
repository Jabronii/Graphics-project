#include "CircleCollisionScene.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

extern Camera camera;
extern GLFWwindow* window;

CircleCollisionScene::CircleCollisionScene() :
	circleShader("shaders/CircleCollisionScene/main.vert", "shaders/CircleCollisionScene/main.frag"),
	shadowShader("shaders/CircleCollisionScene/shadow.vert", "shaders/CircleCollisionScene/shadow.frag", "shaders/CircleCollisionScene/shadow.geom"),
	quadShader("shaders/CircleCollisionScene/quad.vert", "shaders/CircleCollisionScene/quad.frag"),
	quadVertices{
		-1.0,  1.0, 1.0,
		 1.0,  1.0, 1.0,
		 1.0, -1.0, 1.0,
		-1.0,  1.0, 1.0,
		-1.0, -1.0, 1.0,
		 1.0, -1.0, 1.0
},
	quadIndices{
	0, 1, 2,
	2, 3, 0
},
	shadowStrength(0.1f)
{
	srand(time(0));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//circle objects
	glGenVertexArrays(1, &circleVAO);
	glGenBuffers(1, &circleVBO);
	glGenBuffers(1, &circleIBO);

	glBindVertexArray(circleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size()*sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size()*sizeof(unsigned int), &quadIndices[0], GL_STATIC_DRAW);

	//shadow objects
	glGenVertexArrays(1, &shadowVAO);
	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	GenerateCircles(60, 6);
	glDisable(GL_DEPTH_TEST);
}
const float physicsTimeStep = 0.02;
void CircleCollisionScene::UpdateData(float deltatime)
{
	timeBank += deltatime;
	while (timeBank > physicsTimeStep)
	{
		PerformPhysicsCycle(physicsTimeStep/5.);
		timeBank -= physicsTimeStep;
	}
	for (auto& circle : circleArray)
	{ 
		circle.m_excitement -= deltatime;
		if (circle.m_excitement<0.)
		{
			circle.m_excitement =0.;
		}
	}
}

void CircleCollisionScene::PerformPhysicsCycle(float deltatime)
{
	//collide circles with walls
	for (auto & circle : circleArray)
	{
		
		if (circle.m_pos.x+circle.m_radius>(float)SCR_WIDTH/2.)
		{
			circle.m_velocity.x *= -1;
			circle.m_excitement += 0.5;
		}
		if (circle.m_pos.x - circle.m_radius < -(float)SCR_WIDTH / 2.)
		{
			circle.m_velocity.x *= -1;
			circle.m_excitement += 0.5;
		}
		if (circle.m_pos.y + circle.m_radius > (float)SCR_HEIGHT / 2.)
		{
			circle.m_velocity.y *= -1;
			circle.m_excitement += 0.5;
		}
		if (circle.m_pos.y - circle.m_radius < -(float)SCR_HEIGHT / 2.)
		{
			circle.m_velocity.y *= -1;
			circle.m_excitement += 0.5;
		}
		
		//move circle by velocity times deltatime
		circle.m_pos += circle.m_velocity * deltatime;
	}

	//collide circles with other circles
	for (int i = 0; i < circleArray.size(); i++)
	{
		for (int j = i+1; j < circleArray.size(); j++)
		{
			if (CirclesIntersect(circleArray[i],circleArray[j]))
			{
				CollideCircles(circleArray[i], circleArray[j]);
			}
		}
	}
}

template<typename T>
void CreateAttrib(unsigned int index, std::vector<T>& vector, unsigned int componentPerAttrib, bool isInstanced)
{
	unsigned int instanceVBO;
	unsigned int vectorElementSize = sizeof(T);
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, vectorElementSize * vector.size(), &vector[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(index);
	//std::cout << glGetError() << std::endl;
	glVertexAttribPointer(index, componentPerAttrib, GL_FLOAT, GL_FALSE, vectorElementSize, (void*)0);
	if (isInstanced)
	{
		glVertexAttribDivisor(index, 1);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void CreateAttrib(unsigned int index, std::vector<glm::mat4>& vector,  unsigned int componentPerAttrib, bool isInstanced)
{
	unsigned int instanceVBO;
	unsigned int vectorElementSize = sizeof(glm::mat4);
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * vector.size(), &vector[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(index+0);
	glEnableVertexAttribArray(index+1);
	glEnableVertexAttribArray(index+2);
	glEnableVertexAttribArray(index+3);

	glVertexAttribPointer(index + 0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribPointer(index + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(index + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(index + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 12));

	glVertexAttribDivisor(index + 0, 1);
	glVertexAttribDivisor(index + 1, 1);
	glVertexAttribDivisor(index + 2, 1);
	glVertexAttribDivisor(index + 3, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void CircleCollisionScene::DrawShadows(unsigned int lightIndex, float shadowStrengthParam)
{
	glBindVertexArray(shadowVAO);
	shadowShader.use();

	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	shadowShader.setVec2("lightPos", circleArray[lightIndex].m_pos.x, circleArray[lightIndex].m_pos.y);
	shadowShader.setFloat("lightRadius", circleArray[lightIndex].m_radius);
	glDrawArraysInstanced(GL_POINTS, 0, 1, circleArray.size());

	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glBindVertexArray(quadVAO);
	quadShader.use();
	quadShader.setFloat("shadowStrength", shadowStrengthParam);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CircleCollisionScene::Render()
{
	glClearColor(0.9f, 0.9f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//fill vectors with data
	std::vector<glm::mat4> mvpArray;
	std::vector<glm::mat4> modelArray;
	std::vector<float> radiusArray;
	std::vector<glm::vec3> centrePosArray;
	std::vector<float> excitementArray;
	std::vector<glm::vec3> circleColorArray;
	std::vector<float> isLightArray;
	for (int i = 0; i < circleArray.size(); i++)
	{ 
		Circle2D circle = circleArray[i];
		//model
		glm::mat4 model(1.0);
		model = glm::translate(model, circle.m_pos);
		model = glm::scale(model, glm::vec3(circle.m_radius));
		//view
		glm::mat4 view(1.0);
		//shaderProgram.setMat4("view", view);
		//projection
		glm::mat4 projection = glm::ortho(-(float)SCR_WIDTH / 2., (float)SCR_WIDTH / 2., -(float)SCR_HEIGHT / 2., (float)SCR_HEIGHT / 2., -1.0, 1.0);
		//shaderProgram.setMat4("projection", projection);
		glm::mat4 MVP = projection * view * model;
		mvpArray.push_back(MVP);
		modelArray.push_back(model);
		radiusArray.push_back(circle.m_radius);
		centrePosArray.push_back(circle.m_pos);
		excitementArray.push_back(circle.m_excitement);
		circleColorArray.push_back(circle.m_color);
		isLightArray.push_back(circle.m_isLight);
	}

	shadowShader.use();
	//draw shadows
	glBindVertexArray(shadowVAO);
	shadowShader.setVec2("scrDimensions", SCR_WIDTH, SCR_HEIGHT);
	CreateAttrib(0, centrePosArray, 3, true);
	CreateAttrib(1, radiusArray, 1, true);

	glStencilFunc(GL_EQUAL, 1, 0xFF);
	for (int i = 0; i < circleArray.size(); i++)
	{
		if (circleArray[i].m_isLight==1.f)
		{
			DrawShadows(i, shadowStrength);
		}
	} 

	circleShader.use();
	circleShader.setFloat("time", (float)clock() / (float)CLOCKS_PER_SEC);
	//bind circleVAO and create all of the instance attribs
	{
	glBindVertexArray(circleVAO);
	CreateAttrib(1, mvpArray, 16, true);
	CreateAttrib(5, modelArray,16, true);
	CreateAttrib(9, radiusArray,  1, true);
	CreateAttrib(10, centrePosArray, 3, true);
	CreateAttrib(11, excitementArray, 1, true);
	CreateAttrib(12, circleColorArray, 3, true);
	CreateAttrib(13, isLightArray, 1, true);
	}
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, circleArray.size());



	mvpArray.clear();
	modelArray.clear();
	radiusArray.clear();
	centrePosArray.clear();
	excitementArray.clear();
	circleColorArray.clear();
	isLightArray.clear();
}

void CircleCollisionScene::GenerateCircles(unsigned int circleAmountToGenerate, unsigned int lightCountToGenerate)
{
	int circleCount = 0;
	int lightCount = 0;
	while(circleCount<circleAmountToGenerate)
	{
		float finishedGeneratingLights = 0.;
		if (lightCount == lightCountToGenerate)
		{
			finishedGeneratingLights = 1.;
		}
		else
		{
			lightCount++;
		}
		//create a position, radius, color and velocity
		float radius = glm::linearRand(10., 60.);
		glm::vec3 pos = glm::vec3(
			glm::linearRand(-(float)SCR_WIDTH / 2. + radius, (float)SCR_WIDTH / 2. - radius),
			glm::linearRand(-(float)SCR_HEIGHT / 2. + radius, (float)SCR_HEIGHT / 2. - radius),
			0.0
		);
		glm::vec3 color = glm::vec3(0.,glm::linearRand(0., 1.),0.0);

		float speed = glm::linearRand(100., 200.);
		glm::vec2 velocity2 = glm::circularRand(speed);
		glm::vec3 velocity = glm::vec3(velocity2.x, velocity2.y, 0.);

		//create circle from these params
		Circle2D newCircle(radius, pos, color, velocity, 1.- finishedGeneratingLights);

		//check whether it intersects any existing circles
		bool intersectsFlag = false;
		for (int i = 0; i < circleArray.size(); i++)
		{
			if (CirclesIntersect(newCircle, circleArray[i]))
			{
				intersectsFlag = true;
				break;
			}
		}
		if (!intersectsFlag)
		{
		circleArray.push_back(newCircle);
		circleCount++;
		}
	}
}

bool CircleCollisionScene::CirclesIntersect(Circle2D circle1, Circle2D circle2)
{
	glm::vec3 pos1 = circle1.m_pos + circle1.m_velocity * physicsTimeStep;
	glm::vec3 pos2 = circle2.m_pos + circle2.m_velocity * physicsTimeStep;

	if (glm::length(pos1 - pos2) < circle1.m_radius + circle2.m_radius)
	{
		return true;
	}
	return false;
}

void CircleCollisionScene::CollideCircles(Circle2D &circle1, Circle2D &circle2)
{
	float m1 = circle1.m_mass;
	float m2 = circle2.m_mass;
	glm::vec3 v1 = circle1.m_velocity;
	glm::vec3 v2 = circle2.m_velocity;

	circle1.m_velocity = (2 * m2) / (m1 + m2) * v2 + (m1 - m2) / (m1 + m2) * v1;
	circle2.m_velocity = (2 * m1) / (m1 + m2) * v1 - (m1 - m2) / (m1 + m2) * v2;

	circle1.m_excitement += 0.5;
}

void CircleCollisionScene::RenderImGUI()
{
	ImGui::SliderFloat("shadowStrength", &shadowStrength, 0.0f, 1.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void CircleCollisionScene::RenderCircle(Circle2D circle)
{

	circleShader.use();

	//model
	glm::mat4 model(1.0);
	model = glm::translate(model, circle.m_pos);
	model = glm::scale(model, glm::vec3(circle.m_radius));
	circleShader.setMat4("model", model);
	circleShader.setFloat("radius", circle.m_radius);
	circleShader.setVec3("centrePos", circle.m_pos);
	circleShader.setFloat("excitement", circle.m_excitement);
	circleShader.setFloat("time", (float)clock() / (float)CLOCKS_PER_SEC);

	//view
	glm::mat4 view(1.0);
	//shaderProgram.setMat4("view", view);

	//projection
	glm::mat4 projection = glm::ortho(-(float)SCR_WIDTH / 2., (float)SCR_WIDTH / 2., -(float)SCR_HEIGHT / 2., (float)SCR_HEIGHT / 2., -1.0, 1.0);
	//shaderProgram.setMat4("projection", projection);

	glm::mat4 MVP =  projection * view * model;
	circleShader.setMat4("MVP", MVP);

	circleShader.setVec3("circleColor", circle.m_color);

	glBindVertexArray(circleVAO);
	glDrawElements(GL_TRIANGLES, (unsigned int)(sizeof(quadIndices) / sizeof(unsigned int)), GL_UNSIGNED_INT, (void*)0);
};

