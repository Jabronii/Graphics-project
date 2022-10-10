#include "SphereCollisionScene.h"

extern Camera camera;
extern GLFWwindow* window;

const float PI = 3.14159;

const float boxXrange = 2.;
const float boxYrange = 2.;
const float boxZrange = 2.;

glm::vec3 GetCoordsFromIndeces(unsigned int i, unsigned int j, unsigned int horizontalRows, unsigned int verticalRows);
int mod(int number, int modulus);

SphereCollisionScene::SphereCollisionScene() :
	spheresShader("shaders/SphereCollisionScene/spheres.vert", "shaders/SphereCollisionScene/spheres.frag"),
	lightShader("shaders/SphereCollisionScene/light.vert", "shaders/SphereCollisionScene/light.frag"),
	hdrShader("shaders/SphereCollisionScene/hdr.vert", "shaders/SphereCollisionScene/hdr.frag"),
	quadVertices{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	    // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
},
	enclosureVertices{
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f
	},
	enclosureIndices{
		0, 3, 1,
		3, 2, 1,
		3, 7, 6,
		3, 6, 2,
		5, 6, 7,
		5, 7, 4,
		0, 5, 4,
		0, 1, 5,
		1, 2, 6,
		1, 6, 5,
		0, 7, 3,
		0, 4, 7
	}
{
	//seed the rand function, setup the sphere shader, VAO, IBO etc
	srand(time(0));
	GenerateSpheres(30, 4);
	//std::cout << sphereArray.size();
	spheresShader.use();
	//set dir light uniforms
	spheresShader.setVec3("dirLight.direction", glm::vec3(0.5));
	spheresShader.setVec3("dirLight.ambient", glm::vec3(0.015));
	spheresShader.setVec3("dirLight.diffuse", glm::vec3(0.05));
	spheresShader.setVec3("dirLight.specular", glm::vec3(0.05));
	/*
	spheresShader.setVec3("dirLight.direction", glm::vec3(0.));
	spheresShader.setVec3("dirLight.ambient", glm::vec3(0.));
	spheresShader.setVec3("dirLight.diffuse", glm::vec3(0.));
	spheresShader.setVec3("dirLight.specular", glm::vec3(0.));
	*/
	GenerateSphereMesh(24, 24, sphereVertices, sphereIndices);
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereIBO);

	glBindVertexArray(sphereVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), &sphereIndices[0], GL_STATIC_DRAW);

	//setup framebuffer 
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	// generate texture
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size()*sizeof(float), &quadVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	hdrShader.use();
	hdrShader.setInt("hdrBuffer", 0);

	//setup enclosure objects
	glGenVertexArrays(1, &enclosureVAO);
	glGenBuffers(1, &enclosureVBO);
	glGenBuffers(1, &enclosureIBO);

	glBindVertexArray(enclosureVAO);

	glBindBuffer(GL_ARRAY_BUFFER, enclosureVBO);
	glBufferData(GL_ARRAY_BUFFER, enclosureVertices.size() * sizeof(float), &enclosureVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enclosureIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, enclosureIndices.size() * sizeof(unsigned int), &enclosureIndices[0], GL_STATIC_DRAW);
	
}

const float physicsTimeStep = 0.02;
void SphereCollisionScene::UpdateData(float deltatime)
{
	//std::cout << std::to_string(1./deltatime) +"fps"<<std::endl;
	timeBank += deltatime;
	while (timeBank > physicsTimeStep)
	{
		PerformPhysicsCycle(physicsTimeStep);
		timeBank -= physicsTimeStep;
	}
	for (auto& sphere : sphereArray)
	{
		sphere.m_excitement -= deltatime;
		if (sphere.m_excitement < 0.)
		{
			sphere.m_excitement = 0.;
		}
	}
}

void SphereCollisionScene::Render()
{
	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	// make sure we clear the framebuffer's content
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spheresShader.use();
	int lightSphereCount = 0;
	//set point light uniforms
	for (int i = 0; i < sphereArray.size(); i++)
	{
		if (sphereArray[i].m_isLight)
		{
			float lightIntensity = sphereArray[i].m_mass * 250.;
			spheresShader.setVec3(("pointLights[" + std::to_string(lightSphereCount) + "].position").c_str(), sphereArray[i].m_pos);

			spheresShader.setFloat(("pointLights[" + std::to_string(lightSphereCount) + "].constant").c_str(), 1.);
			spheresShader.setFloat(("pointLights[" + std::to_string(lightSphereCount) + "].linear").c_str(), 0.35);
			spheresShader.setFloat(("pointLights[" + std::to_string(lightSphereCount) + "].quadratic").c_str(), 0.44);

			spheresShader.setVec3(("pointLights[" + std::to_string(lightSphereCount) + "].ambient").c_str(), glm::vec3(0.0) * lightIntensity);
			spheresShader.setVec3(("pointLights[" + std::to_string(lightSphereCount) + "].diffuse").c_str(), sphereArray[i].m_color * lightIntensity);
			spheresShader.setVec3(("pointLights[" + std::to_string(lightSphereCount) + "].specular").c_str(), sphereArray[i].m_color * lightIntensity);
			lightSphereCount++;
		}
	}

	for (int i = 0; i < sphereArray.size(); i++)
	{
		if (sphereArray[i].m_isLight)
		{
			RenderLightSphere(sphereArray[i]);
		}
		else
		{
			RenderSphere(sphereArray[i]);
		}
	}
	RenderEnclosure();

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	hdrShader.use();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//graphics
void SphereCollisionScene::RenderSphere(Sphere sphere)
{
	spheresShader.use();

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::translate(glm::mat4(1.), sphere.m_pos);
	model = glm::scale(model, glm::vec3(sphere.m_radius));
	glm::mat4 MVP = projection * view * model;

	spheresShader.setMat4("MVP", MVP);
	spheresShader.setMat4("model", model);
	spheresShader.setVec3("objectColor", sphere.m_color);
	spheresShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	spheresShader.setVec3("viewPos", camera.Position);
	spheresShader.setFloat("excitement", sphere.m_excitement);
	spheresShader.setFloat("time", (float)clock() / (float)CLOCKS_PER_SEC);

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, (void*)0);
}

void SphereCollisionScene::RenderLightSphere(Sphere sphere)
{
	lightShader.use();

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::translate(glm::mat4(1.), sphere.m_pos);
	model = glm::scale(model, glm::vec3(sphere.m_radius));
	glm::mat4 MVP = projection * view * model;

	lightShader.setMat4("MVP", MVP);
	lightShader.setFloat("lightIntensity", 2000*sphere.m_radius * sphere.m_radius * sphere.m_radius);

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, (void*)0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SphereCollisionScene::RenderEnclosure()
{
	/*
	lightShader.use();

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::scale(glm::mat4(1.), glm::vec3(boxXrange, boxYrange, boxZrange));
	glm::mat4 MVP = projection * view * model;

	lightShader.setMat4("MVP", MVP);
	lightShader.setFloat("lightIntensity", 2000.);
	*/
	spheresShader.use();

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::scale(glm::mat4(1.), glm::vec3(boxXrange, boxYrange, boxZrange));
	glm::mat4 MVP = projection * view * model;

	spheresShader.setMat4("MVP", MVP);
	spheresShader.setMat4("model", model);
	spheresShader.setVec3("objectColor", glm::vec3(0., 0., 1.));
	spheresShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	spheresShader.setVec3("viewPos", camera.Position);
	spheresShader.setFloat("excitement", 0.);
	spheresShader.setFloat("time", (float)clock() / (float)CLOCKS_PER_SEC);

	glEnable(GL_CULL_FACE);
	glBindVertexArray(enclosureVAO);
	glDrawElements(GL_TRIANGLES, enclosureIndices.size(), GL_UNSIGNED_INT, (void*)0);
	glDisable(GL_CULL_FACE);
}

void SphereCollisionScene::GenerateSphereMesh(unsigned int horizontalRows, unsigned int verticalRows, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	std::vector<glm::vec3> vertexArray;
	unsigned int totalVertCount = horizontalRows * verticalRows;

	//generate vertex coords into vec3 array
	for (int i = 0; i < horizontalRows; i++)
	{
		for (int j = 0; j < verticalRows; j++)
		{
			vertexArray.push_back(GetCoordsFromIndeces(i, j, horizontalRows, verticalRows));
		}
	}

	//generate triangles
	for (int i = 0; i < horizontalRows; i++)
	{
		for (int j = 1; j < verticalRows; j++)
		{
			indices.push_back(mod(i * verticalRows + j       , totalVertCount));
			indices.push_back(mod((i-1) * verticalRows + j   , totalVertCount));
			indices.push_back(mod(i * verticalRows + j -1    , totalVertCount));
			indices.push_back(mod((i-1) * verticalRows + j   , totalVertCount));
			indices.push_back(mod((i-1) * verticalRows + j-1 , totalVertCount));
			indices.push_back(mod(i * verticalRows + j-1     , totalVertCount));
		}
	}
	//generate pole triangles
	for (int i = 0; i < horizontalRows; i++)
	{
		indices.push_back(totalVertCount);
		indices.push_back(mod((i+1) * verticalRows, totalVertCount));
		indices.push_back(mod(i*verticalRows, totalVertCount));
		indices.push_back(totalVertCount +1);
		indices.push_back(mod((i+1)*verticalRows-1, totalVertCount));
		indices.push_back(mod((i + 2) *verticalRows-1, totalVertCount));
	}

	//add the poles
	vertexArray.push_back(glm::vec3(0.0, 0.0, 1.0));
	vertexArray.push_back(glm::vec3(0.0, 0.0, -1.0));
	//convert vec3 into individual coords
	for (int i = 0; i < vertexArray.size(); i++)
	{
		vertices.push_back(vertexArray[i].x);
		vertices.push_back(vertexArray[i].y);
		vertices.push_back(vertexArray[i].z);
	}
}

void SphereCollisionScene::GenerateSpheres(unsigned int sphereAmountToGenerate, unsigned int lightCount)
{
	int sphereCount = 0;
	while (sphereCount < sphereAmountToGenerate)
	{
		//create a position, radius, color and velocity
		float radius = glm::linearRand(0.1, 0.3);
		glm::vec3 pos = glm::vec3(
			glm::linearRand(-boxXrange + radius, boxXrange - radius),
			glm::linearRand(-boxYrange + radius, boxYrange - radius),
			glm::linearRand(-boxZrange + radius, boxZrange - radius)
		);
		glm::vec3 color = glm::vec3(glm::linearRand(0., 1.), glm::linearRand(0., 1.), glm::linearRand(0., 1.));

		float speed = glm::linearRand(3., 7.);
		glm::vec3 velocity = speed*glm::sphericalRand(radius);

		bool isLight = sphereCount < lightCount ? true : false;

		//create circle from these params
		Sphere newSphere(radius, pos, color, velocity, isLight);

		//check whether it intersects any existing circles
		bool intersectsFlag = false;
		for (int i = 0; i < sphereArray.size(); i++)
		{
			if (SpheresIntersect(newSphere, sphereArray[i]))
			{
				intersectsFlag = true;
				break;
			}
		}
		if (!intersectsFlag)
		{
			sphereArray.push_back(newSphere);
			sphereCount++;
		}
	}
}

bool SphereCollisionScene::SpheresIntersect(Sphere sphere1, Sphere sphere2)
{
	glm::vec3 pos1 = sphere1.m_pos + sphere1.m_velocity * physicsTimeStep;
	glm::vec3 pos2 = sphere2.m_pos + sphere2.m_velocity * physicsTimeStep;

	if (glm::length(pos1 - pos2) < sphere1.m_radius + sphere2.m_radius)
	{
		return true;
	}
	return false;
}

void SphereCollisionScene::PerformPhysicsCycle(float deltatime)
{
	//collide spheres with walls
	for (auto& sphere : sphereArray)
	{

		if (sphere.m_pos.x + sphere.m_radius > boxXrange)
		{
			sphere.m_velocity.x *= -1;
			sphere.m_excitement += 0.5;
		}
		if (sphere.m_pos.x - sphere.m_radius < -boxXrange)
		{
			sphere.m_velocity.x *= -1;
			sphere.m_excitement += 0.5;
		}
		if (sphere.m_pos.y + sphere.m_radius > boxYrange)
		{
			sphere.m_velocity.y *= -1;
			sphere.m_excitement += 0.5;
		}
		if (sphere.m_pos.y - sphere.m_radius < -boxYrange)
		{
			sphere.m_velocity.y *= -1;
			sphere.m_excitement += 0.5;
		}
		if (sphere.m_pos.z + sphere.m_radius > boxZrange)
		{
			sphere.m_velocity.z *= -1;
			sphere.m_excitement += 0.5;
		}
		if (sphere.m_pos.z - sphere.m_radius < -boxZrange)
		{
			sphere.m_velocity.z *= -1;
			sphere.m_excitement += 0.5;
		}
		if (std::abs(sphere.m_pos.x)>boxXrange || std::abs(sphere.m_pos.y) > boxYrange || std::abs(sphere.m_pos.z) > boxZrange)
		{
			std::cout << "OUT OF BOUNDS";
		}
		//move sphere by velocity times deltatime
		sphere.m_pos += sphere.m_velocity * deltatime;
	}

	//collide spheres with other spheres
	for (int i = 0; i < sphereArray.size(); i++)
	{
		for (int j = i + 1; j < sphereArray.size(); j++)
		{
			if (SpheresIntersect(sphereArray[i], sphereArray[j]))
			{
				CollideSpheres(sphereArray[i], sphereArray[j]);
			}
		}
	}
}

void SphereCollisionScene::CollideSpheres(Sphere& sphere1, Sphere& sphere2)
{
	float m1 = sphere1.m_mass;
	float m2 = sphere2.m_mass;
	glm::vec3 v1 = sphere1.m_velocity;
	glm::vec3 v2 = sphere2.m_velocity;

	sphere1.m_velocity = (2 * m2) / (m1 + m2) * v2 + (m1 - m2) / (m1 + m2) * v1;
	sphere2.m_velocity = (2 * m1) / (m1 + m2) * v1 - (m1 - m2) / (m1 + m2) * v2;

	sphere1.m_excitement += 0.5;
}

int mod(int number, int modulus)
{
	if (number>=0)
	{
		return number%modulus;
	}
	else
	{
		int newnumber = number % modulus;
		return modulus + newnumber;
	}
}

glm::vec3 GetCoordsFromIndeces(unsigned int i, unsigned int j, unsigned int horizontalRows, unsigned int verticalRows)
{
	//longitude
	float phi = 2 * PI / ((float) horizontalRows) * i;
	//latitude
	float psi = PI / ((float)verticalRows + 1.0) * (j+1);

	float x = glm::sin(psi) * glm::cos(phi);
	float y = glm::sin(psi) * glm::sin(phi);
	float z = glm::cos(psi);

	return glm::vec3(x, y, z);
}
