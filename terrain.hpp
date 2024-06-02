#ifndef SKYBOX_H
#define SKYBOX_H
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"

class SkyBox {
private:
	glm::vec3 scale_factor;
	float* vertices;
	unsigned int* indices;
	GLuint VAO, VBO, EBO;
	GLuint skyTex0, skyTex1, skyTex2, skyTex3, skyTex4;
	Shader skyShader;
public:
	SkyBox(glm::vec3 scale_factor, Shader& skyShader) 
		: scale_factor(scale_factor), skyShader(skyShader) {
		vertices = new float[]{
			-1.0f,  1.0f, 1.0f, 0.0f, 1.0f, // Top Right
			 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, // Bottom Right
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
			 1.0f,  1.0f, 1.0f, 1.0f, 1.0f  // Top Left
		};

		indices = new unsigned int[]{
			0, 1, 2,
			0, 3, 1
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		skyTex0 = SkyBoxFromFile("SkyBox0.bmp", "data/skybox");
		skyTex1 = SkyBoxFromFile("SkyBox1.bmp", "data/skybox");
		skyTex2 = SkyBoxFromFile("SkyBox2.bmp", "data/skybox");
		skyTex3 = SkyBoxFromFile("SkyBox3.bmp", "data/skybox");
		skyTex4 = SkyBoxFromFile("SkyBox4.bmp", "data/skybox");
	}
	~SkyBox() {
		delete[] vertices;
		delete[] indices;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteTextures(1, &skyTex0);
		glDeleteTextures(1, &skyTex1);
		glDeleteTextures(1, &skyTex2);
		glDeleteTextures(1, &skyTex3);
		glDeleteTextures(1, &skyTex4);
	}

	void Draw(glm::mat4 &view, glm::mat4 & proj) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		skyShader.use();

		skyShader.setMat4("view", view);
		skyShader.setMat4("proj", proj);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, scale_factor);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyTex0);
		skyShader.setInt("sprite", 0);
		skyShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale_factor);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyTex1);
		skyShader.setInt("sprite", 0);
		skyShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale_factor);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyTex2);
		skyShader.setInt("sprite", 0);
		skyShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale_factor);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyTex3);
		skyShader.setInt("sprite", 0);
		skyShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale_factor.x, scale_factor.z, scale_factor.y));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, skyTex4);
		skyShader.setInt("sprite", 0);
		skyShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class WaterGround {
private:
	glm::vec3 scale_factor;
	float* vertices;
	unsigned int* indices;
	GLuint VAO, VBO, EBO;
	GLuint waterTex;
	Shader waterShader;
public:
	WaterGround(glm::vec3 scale_factor)
		: scale_factor(scale_factor), waterShader("water.vert", "water.frag") {
		vertices = new float[] {
			-1.0f,  1.0f, 1.0f, 0.0f, 100.0f, // Top Right
			 1.0f, -1.0f, 1.0f, 100.0f, 0.0f, // Bottom Right
			-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
			 1.0f,  1.0f, 1.0f, 100.0f, 100.0f  // Top Left
		};

		indices = new unsigned int[] {
			0, 1, 2,
			0, 3, 1
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		waterTex = WaterFromFile("SkyBox5.bmp", "data/skybox");
	}
	~WaterGround() {
		delete[] vertices;
		delete[] indices;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteTextures(1, &waterTex);
	}

	void Draw(glm::mat4& view, glm::mat4& proj) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		waterShader.use();

		waterShader.setMat4("view", view);
		waterShader.setMat4("proj", proj);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale_factor.x * 1.1f, scale_factor.z * 1.1f, scale_factor.y));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterTex);
		waterShader.setInt("sprite", 0);
		waterShader.setMat4("model", model);
		waterShader.setVec2("offset", (float)glfwGetTime() / 10.0f, (float)glfwGetTime() / 10.0f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class TerrainGround {
private:
	glm::vec3 scale_factor;
	vector<float> vertices;
	vector<unsigned int> indices;
	GLuint VAO, VBO, EBO;
	GLuint groundTex, detailTex;
	Shader groundShader;
public:
	TerrainGround(glm::vec3 scale_factor, Shader& groundShader)
		: scale_factor(scale_factor), groundShader(groundShader) {

		stbi_set_flip_vertically_on_load(true);
		int width, height, nrComponents;
		unsigned char* data = stbi_load("data/heightmap.bmp", &width, &height, &nrComponents, 0);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				vertices.push_back((float)j / height);
				vertices.push_back((float)(*(data + 3 * (i * width + j))) / 1500.0f);
				vertices.push_back((float)i / width);
				vertices.push_back((float)j / height);
				vertices.push_back((float)i / width);
			}
		}
		for (int i = 0; i < height - 1; i++) {
			for (int j = 0; j < width - 1; j++) {
				indices.push_back(i * width + j);
				indices.push_back(i * width + j + 1);
				indices.push_back((i + 1) * width + j);
				indices.push_back(i * width + j + 1);
				indices.push_back((i + 1) * width + j + 1);
				indices.push_back((i + 1) * width + j);
			}
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		groundTex = TextureFromFile("terrain-texture3.bmp", "data");
		detailTex = TextureFromFile("detail.bmp", "data");
	}
	~TerrainGround() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteTextures(1, &groundTex);
		glDeleteTextures(1, &detailTex);
	}

	void Draw(glm::mat4& view, glm::mat4& proj) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		groundShader.use();

		groundShader.setMat4("view", view);
		groundShader.setMat4("proj", proj);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -31.51f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, groundTex);
		groundShader.setInt("tex0", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, detailTex);
		groundShader.setInt("tex1", 1);
		groundShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

#endif // SKYBOX_H
