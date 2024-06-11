//
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "shader.h"
//#include "camera.h"
//#include "Model.h"
//#include "Display.h"
//#include "Transform.h"
//
//#include <iostream>
//
//
//
//void processInput(GLFWwindow* window);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// camera
////glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//Camera camera(glm::vec3(0.0f, 0.0f, 15.0f), glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//int main1()
//{
//    Display display(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
//    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//
//    stbi_s(true);
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//
//
//    // build and compile shaders
//    // -------------------------
//    Shader ourShader("./res/backpack/model_loading.vs", "./res/backpack/model_loading.fs");
//
//    // load models
//    // -----------
//    Model ourModel("./res/backpack/backpack.obj");
//
//    Transform trans;
//
//    // draw in wireframe
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(display.GetWindow()))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        // -----
//        processInput(display.GetWindow());
//
//        // render
//        // ------
//        display.Clear(0.05f, 0.05f, 0.05f, 1.0f);
//        
//
//        // don't forget to enable shader before setting uniforms
//        ourShader.use();
//
//        // view/projection transformations
//        ourShader.setMat4("projection", camera.GetProjection());
//        ourShader.setMat4("view", camera.GetView());
//
//        // render the loaded model
//        trans.SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
//        trans.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
//        ourShader.setMat4("model", trans.GetModel());
//        ourModel.Draw(ourShader);
//
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        display.SwapBuffers();
//    }
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//
//





#define GLFW_INCLUDE_NONE
#include "online.h"


/*
#define GLT_IMPLEMENTATION
#include "gltext.h"  //https://github.com/vallentin/glText
*/

//#include "text.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <codecvt>

#include <ft2build.h>
#include FT_FREETYPE_H  

static int DISPLAY_WIDTH = 1000;
static int DISPLAY_HEIGHT = 800;


struct Cube
{
	Transform* T;
	Texture* TE;
};


glm::mat3 rotatey(float alpha)
{
	glm::mat3 rotate =
	{
		glm::vec3(cosf(alpha),0.f,-sinf(alpha)),
		glm::vec3(0.f,1.f,0.f),
		glm::vec3(sinf(alpha),0.f,cosf(alpha))
	};
	return rotate;
}

glm::mat3 rotatez(float alpha)
{
	glm::mat3 rotate =
	{
		glm::vec3(cosf(alpha),-sinf(alpha),0.f),
		glm::vec3(sinf(alpha),cosf(alpha),0.f),
		glm::vec3(0.f        ,   0.f,1.f),
	};
	return rotate;
}






struct yeriya
{
	glm::vec3 kivun;
	glm::vec3 pos;
};

vector<Vertex> fl;
vector<unsigned int> indices1((widthT - 1) * (heightT - 1) * 2 * 3 + 3 * 8);
typedef std::chrono::high_resolution_clock Clock;

glm::vec3 hetel(glm::vec3 P1, glm::vec3 P2, glm::vec3 V);
float heightMin(float x, float y);


float height_min_with_tanks(float x, float z, std::vector<Tank> tt, int my = MyTank);





float f(int x, int y, int oct, float B, float randY[])
{
	float noise = 0;
	float scaleAcc = 0;
	float scale = 1;


	for (int i = 0; i < oct; i++)
	{
		int P = widthT >> i;
		int Sx1 = (x / P) * P;
		int Sy1 = (y / P) * P;

		int Sx2 = (Sx1 + P) % widthT;
		int Sy2 = (Sy1 + P) % widthT;

		float Bx = (float)(x - Sx1) / (float)P;
		float By = (float)(y - Sy1) / (float)P;

		float St = (1 - Bx) * randY[Sy1 * widthT + Sx1] + Bx * randY[Sy1 * widthT + Sx2];
		float Sb = (1 - Bx) * randY[Sy2 * widthT + Sx1] + Bx * randY[Sy2 * widthT + Sx2];

		scaleAcc += scale;
		noise += (By * (Sb - St) + St) * scale;
		scale /= B;
	}

	//std::cout << noise / scaleAcc <<" ";

	return noise / scaleAcc;
}


#define timerotsome 1.49



/*std::string keyboardPressed(input::InputDetector inp) {
	std::string str = "";
		
	std::string AB = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string Ns = "1234567890";
	
		//4 - 29    a-z
		//30 - 39   1 - 0
	for (int i = 4; i < 30; i++)
		if (inp.KeyDownS(SDL_Scancode(i)))
			str += AB[i - 4];

	for (int i = 30; i < 40; i++)
		if (inp.KeyDownS(SDL_Scancode(i)))
			str += Ns[i - 30];

	return str;
}*/


void startWin(Display* display, float p = 0)
{
	//Display display1 = *display;
	//input::InputDetector inp1(DISPLAY_WIDTH, DISPLAY_HEIGHT);

	if (p > 1)
		p /= 100;

	//while (numWindow == START_WINDOW) {
		//inp.update();

		display->Clear(0.5, 0.2, 0.3, 1.0);


		glColor3d(9.0, 9, 9.0);
		glBegin(GL_QUADS);
		//2  1
		//3  4
		glVertex3d(0.5, 0.8, 0.);
		glVertex3d(-0.5 + p, 0.8, 0.);
		glVertex3d(-0.5 + p, 0.75, 0.);
		glVertex3d(0.5, 0.75, 0.);
		glEnd();


		glColor3d(0.0, 9, 0.0);
		glBegin(GL_QUADS);
		//2  1
		//3  4
		glVertex3d(-0.5 + p, 0.8, 0.);
		glVertex3d(-0.5, 0.8, 0.);
		glVertex3d(-0.5, 0.75, 0.);
		glVertex3d(-0.5 + p, 0.75, 0.);
		glEnd();

		display->SwapBuffers();

		//std::cout << "%";
	//}

	//display = display1;
	numWindow = GAME_WINDOW;
	//display1.dontKill(true);
}


#if defined(_MSC_VER) && (_MSC_VER >= 1310)
#	pragma warning(disable: 4996) // Disable the fopen, strcpy, sprintf being unsafe warning
#endif


unsigned int VAO, VBO;
// render line of text
// -------------------
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.use();
	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(DISPLAY_WIDTH), 0.0f, static_cast<float>(DISPLAY_HEIGHT));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionText));

	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void loadChars(int f, int l, FT_Face face)
{
	for (unsigned int c = f; c < l; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		cout << c << endl;
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};

		//cout << face->charmap->encoding << endl;
		Characters.insert(std::pair<char, Character>(c, character));
	}


}


std::string startWinEntername(Display* display)
{
	//Shader shader("./res/shaders/shader2.vs", "./res/shaders/shader2.fs");
	//Shader3 lightingShader("./res/shaders/shaderLight.vs", "./res/shaders/shaderLight.fs");
	//Shader3 lightCubeShader("./res/shaders/shaderLightCube.vs", "./res/shaders/shaderLightCube.fs");
	stbi_s(true);
	Shader ourShader("./res/shaders/model_loading.vs", "./res/shaders/model_loading.fs");
	Shader shaderBtn("./res/shaders/btn.vs", "./res/shaders/model_loading.fs");
	Shader shaderSingleColor("./res/shaders/shaderSingleColor.vs", "./res/shaders/shaderSingleColor.fs");
	//Texture tex("./res/bricks.jpg");
	Shader shaderText("./res/shaders/text.vs", "./res/shaders/text.fs");
	Camera camera(glm::vec3(25.0f, 0.0f, 25.0f), glm::radians(45.0f), (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);
	camera.forward = glm::vec3(-1.0, 0.0, -1);
	/*Vertex* vertices = new Vertex[]{
		//Vertex2(glm::vec3(0.5f, -0.5f, 0.0f), /* glm::vec3(1.0f, 0.0f, 0.0f), / glm::vec2(1.0, 1.0)),   // bottom right
		//Vertex2(glm::vec3(-0.5f, -0.5f, 0.0f),/*  glm::vec3(0.0f, 1.0f, 0.0f),/  glm::vec2(1.0, 0.0)),   // bottom left
		//Vertex2(glm::vec3(-0.5f,  0.5f, 0.0f),/*  glm::vec3(0.0f, 0.0f, 1.0f),/  glm::vec2(0.0, 0.0)),    // top 
		//Vertex2(glm::vec3(0.5f,  0.5f, 0.0f), /* glm::vec3(1.0f, 1.0f, 1.0f), / glm::vec2(0.0, 1.0))    // top 
		
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1.0), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1.0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0,0,-1.0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0,0,-1.0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,0,-1.0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,-1.0), glm::vec2(0.0f, 0.0f)),

	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,0,1.0), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0,0,1.0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0,0,1.0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0,0,1.0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,0,1.0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,0,1.0), glm::vec2(0.0f, 0.0f)),

	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0,0,0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0,0,0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0,0,0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0,0,0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0,0,0), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0,0,0), glm::vec2(1.0f, 0.0f)),

	Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0,0,0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0,0,0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0,0,0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0,0,0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0,0,0), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0,0,0), glm::vec2(1.0f, 0.0f)),

	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,-1.0,0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0,-1.0,0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0,-1.0,0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0,-1.0,0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,-1.0,0), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,-1.0,0), glm::vec2(0.0f, 1.0f)),

	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,1.0,0), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0,1.0,0), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0,1.0,0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0,1.0,0), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,1.0,0), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,1.0,0), glm::vec2(0.0f, 1.0f))
	
	
	};
	unsigned int k[] = {
		0, 1, 2,
		3, 4, 5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,	28,29,
		30,31,32,
		33,34,35,
	};
	Mesh mesh(vertices, 36, k, 36);
	Mesh lcube(vertices, 36, k, 36);
	*/
		
	//Model ourModel("./res/backpack/backpack.obj");
	//Model ourModel("./res/Tiger-t/Tiger_I.obj");
	Model ourModel("./res/bullet.obj");
	Model cords("./res/cords.obj");
	Model cube("./res/cube.obj");


	//Model ourModel("./res/tank2.txt");
		//("./res/shaders/2d.txt");
	

	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(DISPLAY_WIDTH), 0.0f, static_cast<float>(DISPLAY_HEIGHT));
	shaderText.use();
	shaderText.setMat4("projection", projectionText);




	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		throw -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		throw -1;
	}
//	face->charmap->encoding = FT_ENCODING_UNICODE;
	FT_Set_Pixel_Sizes(face, 0, 48);
	/*if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		throw -1;
	}*/

	FT_Set_Pixel_Sizes(face, 0, 48);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load first 128 characters of ASCII set
	
	loadChars(0, 255, face);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	
	int viewportWidth, viewportHeight;
	double time;
	std::string str = "name";
	char strT[30];
	//string strT;
	//	wchar_t *str = L"ààà";

	string mess = "searcing server";
	//mess = str;
	
	string dots = "";
	glfwSetWindowShouldClose(display->GetWindow(), GLFW_FALSE);

	Transform trans;
	//glEnable(GL_DEPTH_TEST);


	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor
	//normalShader.use();
	//DrawFloor();

	//	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//glStencilMask(0xFF);
	//DrawTwoContainers();
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);
	//shaderSingleColor.use();
	//DrawTwoScaledUpContainers();
	//glStencilMask(0xFF);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//glEnable(GL_DEPTH_TEST);


	while (!glfwWindowShouldClose(display->GetWindow()))
	{
		time = glfwGetTime();
		display->setSize();

		viewportWidth = display->GetWidth();
		viewportHeight = display->GetHeight();
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(display->GetWindow(), GLFW_TRUE);
			return "";
		}

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_ENTER) && numWindow == FIND_SERVER_WINDOW)
			glfwSetWindowShouldClose(display->GetWindow(), GLFW_TRUE);
		
		//glClear(GL_COLOR_BUFFER_BIT);
		display->Clear(0.1f, 0.1f, 0.1f, 1.f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//tex.Bind();
		/*/
		shader.Bind();
		//float fS = sin(time);
		//float cS = cos(time);
		trans.SetScale(glm::vec3(0.5, 1.5, 0.5));
		trans.SetRot(glm::vec3((float)time, (float)time, (float)time));
		trans.SetPos(glm::vec3(0, 0, 0));
		//trans.SetPos(glm::vec3(cS, fS, -5 - fS * 5));
		// create transformations
		//shader.setMat4("model", trans.GetModel());
		shader.setMat4("projection", camera.GetProjection());
		shader.setMat4("view", camera.GetView());//glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f)));
		shader.setMat4("model", trans.GetModel());

		mesh.Draw();
		trans.SetScale(glm::vec3(1.5, 0.5, 0.5));

		//trans.SetPos(glm::vec3(2, 2, -10));
		shader.setMat4("projection", camera.GetProjection());
		shader.setMat4("view", camera.GetView());//glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f)));
		shader.setMat4("model", trans.GetModel());
		lcube.Draw();

		trans.SetScale(glm::vec3(0.5, 0.5, 1.5));

		//trans.SetPos(glm::vec3(2, 2, -10));
		shader.setMat4("projection", camera.GetProjection());
		shader.setMat4("view", camera.GetView());//glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -3.0f)));
		shader.setMat4("model", trans.GetModel());
		lcube.Draw();
		*/



		/*
		glm::vec3 lightPos(sin(time),  0, cos(time));

		lightingShader.Bind();
		lightingShader.setVec3("objectColor", glm::vec3(1.0f));
		lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", camera.pos);

		// view/projection transformations
		glm::mat4 projection = camera.GetProjection();
		glm::mat4 view = camera.GetView();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		trans.SetPos(glm::vec3(0,0,0));
		trans.SetScale(glm::vec3(1.0f)); // a smaller cube
		trans.SetRot(glm::vec3(sin(time), sin(time), sin(time)));
		lightingShader.setMat4("model", trans.GetModel());
		*/
		

		ourShader.use();
		glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor



		// view/projection transformations
		ourShader.setMat4("projection", camera.GetProjection());
		ourShader.setMat4("view", camera.GetView());

		// render the loaded model
		trans.SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
		float scale = 1.f;
		trans.SetScale(glm::vec3(scale));	// it's a bit too big for our scene, so scale it down
		trans.SetRot(glm::vec3(0, 0, 0));
		ourShader.setMat4("model", trans.GetModel());
		// render the cube

		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);
		cords.Draw(ourShader);

		for (int i = 0; i < 20; i++)
		{
			float f = 8*cos(time + i);
			float r = 8*sin(time + i);
			float w = 8 * sin(time + i);
			trans.SetPos(glm::vec3(f, r, w));
			trans.SetScale(glm::vec3((float)i/20));	// it's a bit too big for our scene, so scale it down
			trans.SetRot(glm::vec3(time * i, time*i, time * i));
			ourShader.setMat4("model", trans.GetModel());
			ourModel.Draw(ourShader);
		}
	
		shaderBtn.use();
		trans.SetPos(glm::vec3(sin(time), 0.0f, 17.0f));
		scale = 2.f;
		trans.SetScale(glm::vec3(2,2,1));	// it's a bit too big for our scene, so scale it down
		trans.SetRot(glm::vec3(0, 0, 0)); 
		shaderBtn.setMat4("model", trans.GetModel());
		projectionText = glm::ortho(0.0f, static_cast<float>(DISPLAY_WIDTH), 0.0f, static_cast<float>(DISPLAY_HEIGHT));
		glUniformMatrix4fv(glGetUniformLocation(shaderBtn.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionText));

		shaderBtn.setMat4("projection", camera.GetProjection());
		shaderBtn.setMat4("view", camera.GetView());

		cube.Draw(shaderBtn);


		if (numWindow == FIND_SERVER_WINDOW)
		{
			mess = "the server find in " + serverIP;
			RenderText(shaderText, mess, 25.0f, 50.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
			RenderText(shaderText, "press ENTER to continue", 25.0f, 20.0f, 0.50f, glm::vec3(0.5, 0.8f, 0.2f));

		}
		else
		{
			int j = (int)time;

			dots = j%4 == 0? "." : (j % 4) - 1 == 0? ".." : (j%4) - 2 == 0 ? "..." : "";
			RenderText(shaderText, mess + dots, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		}
		sprintf(strT, "Time: %.3f", time);
		//wstring s = strT;
		RenderText(shaderText, strT, DISPLAY_WIDTH - 110.f, 25.0f, 0.40f, glm::vec3(0.5, 0.8f, 0.2f));

		
		display->SwapBuffers();
	}

	/*gltDeleteText(text1);
	gltDeleteText(text2);

	gltTerminate();*/

	return str;
}



int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)connectToServer, NULL, NULL, NULL);
	Display* display = new Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	//input::InputDetector inp(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	//SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));

	/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//{
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}
	//main2(display);

	username = startWinEntername(display);
	//std::thread(connectToServer).join();
	//startWin(display);


	srand((unsigned)NULL(time));

	
	


	//numWindow = GAME_WINDOW;

	//numWindow = NEXT_WINDOW;
	//while (numWindow != GAME_WINDOW){}



	float Yrandom[widthT * heightT + 8*5];
	for (int i = 0; i < widthT * heightT + 8*5; i++)
	{
		Yrandom[i] = 2 * (float)rand() / (float)RAND_MAX;
	}
	//Yrandom[0] = 0;

	//system("pause");


	for (int i = 0; i < widthT; i++)
	{
		for (int j = 0; j < heightT; j++)
		{
			Vertex ver;
			ver.Position = glm::vec3(j * sizetri, 30 * f(j, i, 8, 2, Yrandom), i * sizetri);
			ver.TexCoords = glm::vec2((i + 1) % 2, (j + 1) % 2);
			fl.push_back(ver);

			//fl[i * heightT + j].GetPos()->y = ;

			//fl[i * heightT + j].color = glm::vec3();
			//fl[i * heightT + j].normal = glm::vec3(0, 1, 0);
			//fl[i * heightT + j].TexCoords = glm::vec2((i + 1) % 2, (j + 1) % 2);
		}
		//std::cout << std::endl;
	}
	/*
	for (int i = 0; i < widthT; i++)
	{
		for (int j = 0; j < heightT; j++)
		{
			fl[i * heightT + j].GetPos()->x = j * sizetri;
			fl[i * heightT + j].GetPos()->z = i * sizetri;
			fl[i * heightT + j].GetPos()->y = 0.5;
			if (j < 25 && j > 5)
				fl[i * heightT + j].GetPos()->y += 3;
			if (i < 25 && i > 5)
				fl[i * heightT + j].GetPos()->y += 5;

			if (j < 16 && j >= 14 && i < 16 && i >= 14)
				fl[i * heightT + j].GetPos()->y += 10;

			if ((i == 10 || i == 20) && j != 14 && j != 15)
				fl[i * heightT + j].GetPos()->y += 13;

			fl[i * heightT + j].normal = glm::vec3(0, 1, 0);
			fl[i * heightT + j].texCoord = glm::vec2((i + 1) % 2, (j + 1) % 2);


			if (j <= 0 + 1 || j >= heightT - 1 - 1 || i <= 0 + 1 || i >= heightT - 1 - 1)
			{
				fl[i * heightT + j].GetPos()->y = 13;
			}


		}
	}
	*/
	std::cout << '#';
	
	
	int h = 0;
	for (int i = 0; i < widthT - 1; i++)
	{
		for (int j = 0; j < heightT - 1; j++)
		{
			indices1[h++] = j + (i + 1) * heightT;
			indices1[h++] = j + 1 + (i + 1) * heightT;
			indices1[h++] = j + 1 + i * heightT;


			indices1[h++] = j + (i + 1) * heightT;
			indices1[h++] = j + 1 + i * heightT;
			indices1[h++] = j + i * heightT;

		}
	}
	std::cout << '#';


	//Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	std::cout << '#';

	//ver(100,1,100,vertices1,0,-2,0);
	//ver(2, 5, 100, vertices2, 100, 4, 0);
	//ver(2, 5, 100, vertices3,-100,4,0);
	//ver(100, 5, 2, vertices4, 2, 4, 100);

	int iii = 0;

	Texture texture3 = getT("./res/bricks.jpg");
	std::cout << '#';
	vector<Texture> tvs = { texture3 };
	//Mesh grand(fl, sizeof(fl) / sizeof(fl[0]), indices1, sizeof(indices1) / sizeof(indices1[0]));
	Mesh grand(fl, indices1, tvs);
	std::cout << '#';

	//Mesh kir1(vertices2, sizeof(vertices1) / sizeof(vertices1[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh kir2(vertices3, sizeof(vertices1) / sizeof(vertices1[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh kir3(vertices4, sizeof(vertices1) / sizeof(vertices1[0]), indices, sizeof(indices) / sizeof(indices[0]));

	Shader shader("./res/shaders/model_loading.vs", "./res/shaders/model_loading.fs");
	Shader shaderSingleColor("./res/shaders/shaderSingleColor.vs", "./res/shaders/shaderSingleColor.fs");
	Shader shaderText("./res/shaders/text.vs", "./res/shaders/text.fs");

	Model tankModel("./res/Tiger-t/Tiger_I.obj");
	Model bulletModel("./res/bullet.obj");

	
	std::cout << '#';
	/*
	Mesh monkey("./res/monkey3.obj");
	std::cout << '#';

	Mesh tank("./res/tiger2.obj");
	std::cout << '#';

	Mesh road("./res/setforbridge.txt");
	std::cout << '#';

	Mesh candy("./res/candyCane1.txt");
	std::cout << '#';


	Shader3 shader("./res/shaders/shader2.vs", "./res/shaders/shader2.fs");
	//Mesh Flag("./res/flag4.txt");
	//Shader shader("./res/basicShader");
	std::cout << '#';

	//Shader shaderM("./res/monkeyShader");
	std::cout << '#';

	Texture textureR("./res/ConcreteBare0314_2_S.jpg");
	std::cout << '#';

	Texture red("./res/red.png");
	std::cout << '#';

	Texture yellow("./res/yellow.png");
	std::cout << '#';




	Texture texture2("./res/gray.png");
	std::cout << '#';

	Texture texture("./res/gra.png");
	std::cout << '#';

	Texture textureT("./res/Tank_Body_Forest.png");
	std::cout << '#';

	Texture textureC("./res/candy_cane_diffuse.jpg");
	std::cout << '#';

	*/



	Transform transformc;
	transformc.GetRot()->z = 1.5;
	transformc.GetPos()->y = 7;

	Transform transform2;
	Transform transform3;

	Transform transformY;


	transform3.SetPos(glm::vec3(101, -5, 101));
	transform3.SetScale(glm::vec3(30, 30, 30));
	std::cout << '#';


	Camera camera(glm::vec3(5.0f, 10.0f, 5.0f), glm::radians(45.0f), (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 200.0f);
	camera.forward = glm::vec3(0, 0, 1);

	std::cout << '#';

	Tank t1(tankModel, camera.pos, camera.up, camera.forward);
	std::cout << '#';


	std::cout << '#';

	glm::mat3 R = rotatez(1.5708);
	glm::vec3 N;



	//SDL_Event e;
	float counter = 0.0f;

	float upx = 0;
	float upy = 0;
	int yr = 0;
	float goup = 0.5;
	float godown = -goup;
	float C = 0.22;
	float D = 10;
	float camU = 6.5;
	float camF = 13.8;
	float rotM = 0;
	float rotP = 0;
	float counter2 = 0;
	int lorr = 1;



	tanks[MyTank].forward = camera.forward;
	tanks[MyTank].up = camera.up;

	for (int i = 0; i < Max; i++)
		tanks[i] = Tank(tankModel, glm::vec3(15 * i, heightMin(15 * i, 15 * i), 15 * i), camera.up, camera.forward);

	glm::mat4 H;
	glLineWidth(2);

	clock_t time1 = std::clock();
	clock_t time2;
	std::cout << '#';
	double allthetime = 0;



	

	//Camera camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::radians(70.0f), (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 200.0f);
	//camera.forward = glm::vec3(0, 0, 1);
	Transform trans;

	/*
	if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
		glfwTerminate();
		throw EXIT_FAILURE;
	}
	GLTtext* text2 = gltCreateText();
	*/
	//int viewportWidth, viewportHeight;
	//double time;
	std::string str = "name";
	char strT[30];

	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<float>(DISPLAY_WIDTH), 0.0f, static_cast<float>(DISPLAY_HEIGHT));
	shaderText.use();
	glUniformMatrix4fv(glGetUniformLocation(shaderText.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionText));


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	glfwSetWindowShouldClose(display->GetWindow(), GLFW_FALSE);
	double time = 0, prevT = 0;
	
	while (!glfwWindowShouldClose(display->GetWindow()))
	{
		//time = glfwGetTime();
		time = glfwGetTime()*2.f;
		display->setSize();

		int viewportWidth = display->GetWidth();
		int viewportHeight = display->GetHeight();

		//viewportWidth = display->GetWidth();
		//viewportHeight = display->GetHeight();
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(display->GetWindow(), GLFW_TRUE);
		
		display->Clear(0.02f, 0.95f, 0.92f, 1.0f);


		shader.use();

		// view/projection transformations
		shader.setMat4("projection", camera.GetProjection());
		shader.setMat4("view", camera.GetView());

		// render the loaded model
		trans.SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
		trans.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		shader.setMat4("model", trans.GetModel());


		//grand.Draw(shader);
		
		time2 = std::clock();
		float timeDiff1 = (time2 - time1)/2;
		float timeDiff2 = timeDiff1 / 16;
		timeDiff1 /= 32;
		allthetime += timeDiff1 / 128;
		tanks[MyTank].up = glm::normalize(tanks[MyTank].up);
		tanks[MyTank].forward = glm::normalize(tanks[MyTank].forward);
		//timeDiff1 = (time-prevT)*7;
		//timeDiff2 = timeDiff2/2;
		

		//SDL_Delay(C);

		//inp.update();

		glm::mat4 Z;
		glm::mat4 Y;

		


		if (glfwGetKey(display->GetWindow(), GLFW_KEY_Q))
			camU += 0.1;
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_E))
			camU -= 0.1;

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_A))
			camF += 0.1;
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_D))
			camF -= 0.1;

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_U))
			camera.pos += glm::vec3(camera.up.x * goup, camera.up.y * goup, camera.up.z * goup);
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_D))
			camera.pos += glm::vec3(camera.up.x * godown, camera.up.y * godown, camera.up.z * godown);

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_UP))
		{
			//	if (tanks[MyTank].a.x + tanks[MyTank].a.y + tanks[MyTank].a.z < timeDiff1/6)
			//		tanks[MyTank].a += glm::vec3(tanks[MyTank].forward);
			//
			//
			//	camera.pos += tanks[MyTank].a;
			//	tanks[MyTank].pos += tanks[MyTank].a;

			camera.MoveForward((goup * timeDiff1));
			tanks[MyTank].MoveForward((goup * timeDiff1));
		}


		if (glfwGetKey(display->GetWindow(), GLFW_KEY_LEFT) && glfwGetKey(display->GetWindow(), GLFW_KEY_LEFT_SHIFT))
		{
			camera.RotateY((0.01 * timeDiff2) * timerotsome);
			rotM += (0.01 * timeDiff2) * timerotsome;
		}
		else if (glfwGetKey(display->GetWindow(), GLFW_KEY_LEFT))
		{
			Y = tanks[MyTank].rotRight((0.01 * timeDiff2) * timerotsome);
			tanks[MyTank].Tr.mat *= Y;
			tanks[MyTank].forward = glm::vec3(glm::normalize(tanks[MyTank].rotRight((0.01 * timeDiff2) * timerotsome) * glm::vec4(tanks[MyTank].forward, 0.0)));
		}

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_DOWN))
		{
			camera.MoveForward(godown * timeDiff1);
			tanks[MyTank].MoveForward(godown * timeDiff1);
		}
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_RIGHT) && glfwGetKey(display->GetWindow(), GLFW_KEY_LEFT_SHIFT))
		{
			camera.RotateY((-0.01 * timeDiff2) * timerotsome);
			rotM -= (0.01 * timeDiff2) * timerotsome;
		}
		else if (glfwGetKey(display->GetWindow(), GLFW_KEY_RIGHT))
		{
			Y = tanks[MyTank].rotRight((-0.01 * timeDiff2) * timerotsome);
			tanks[MyTank].Tr.mat *= Y;

			tanks[MyTank].forward = glm::vec3(glm::normalize(tanks[MyTank].rotRight((-0.01 * timeDiff2) * timerotsome) * glm::vec4(tanks[MyTank].forward, 0.0)));
		}


		camera.forward = glm::normalize(glm::vec3(tanks[MyTank].forward.x, 0, tanks[MyTank].forward.z));

		if (height_min_with_tanks(camera.pos.x, camera.pos.z, tanks))
		{
			float help13 = height_min_with_tanks(camera.pos.x, camera.pos.z, tanks);

			//std::cout << "timediff1:   " << timeDiff1 << "    ######" << std::endl;
			//std::cout << "calc:   " << camera.pos.y - 0.2 * timeDiff1 << "    ######" << std::endl;
			//std::cout << "help13:   " << help13 << "    ######" << std::endl;
			if (camera.pos.y - 0.2 * timeDiff1 > help13)
				camera.pos.y -= 0.2 * timeDiff1;
			else
				camera.pos.y = help13;

			tanks[MyTank].pos.y = camera.pos.y;
		}

		myPos[0] = tanks[MyTank].pos.x;
		myPos[1] = tanks[MyTank].pos.y;
		myPos[2] = tanks[MyTank].pos.z;


		if (glfwGetKey(display->GetWindow(), GLFW_KEY_SPACE) && counter >= TimeD)
		{
			//std::cout << "c: " << counter << " | td1: " << timeDiff1 << std::endl;
			tanks[MyTank].yeriya = tanks[MyTank].timeY + 2;
			counter = -TimeD;

			//	for (int j = 0; j < tanks.size(); j++)
			//	{
			//		tanks[j].yeriya = tanks[j].timeY + 2;
			//
			//	}
			//	std::cout << std::endl;

		}

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_W))
		{
			Z = tanks[MyTank].rotZ(0.01 * timeDiff2);
			tanks[MyTank].Tr.mat *= Z;
			tanks[MyTank].forward = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].forward, 0.0)));
			tanks[MyTank].up = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].up, 0.0)));

			rotP += 0.01 * timeDiff2;
		}

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_S))
		{
			Z = tanks[MyTank].rotZ(-0.01 * timeDiff2);
			tanks[MyTank].Tr.mat *= Z;
			tanks[MyTank].forward = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].forward, 0.0)));
			tanks[MyTank].up = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].up, 0.0)));

			rotP -= 0.01 * timeDiff2;
		}


		tanks[MyTank].Tr.SetPos(camera.pos);
		tanks[MyTank].pos = camera.pos;
		//	camera.pos = tanks[MyTank].pos;
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_P))
		{
			tanks[MyTank].forward = camera.forward;

			tanks[MyTank].up = camera.up;
			rotP = 0;
			tanks[MyTank].Tr.mat = tanks[MyTank].rotRight(rotM);

			camU = 4;
			camF = 9;

			C = 0.22;
			camU = 6.5;
			camF = 13.8;
		}

		//float xplus = inp.getMouseXNormalized();
		//float yplus = inp.getDeltaMouseY();
		//upy = yplus;

		//cout << "U: " << camU << " F: " << camF << " C: " << C << endl;

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_N))
		{
			C += 0.001 * timeDiff2;
			//std::cout << C << std::endl;
		}
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_B))
		{
			C -= 0.001*timeDiff2;
			//std::cout << C << std::endl;
		}

		if (glfwGetKey(display->GetWindow(), GLFW_KEY_G))
		{
			tanks[MyTank].life -= 50 * timeDiff2;

			D += 0.02 * timeDiff2;
			//std::cout << D << std::endl;
		}
		if (glfwGetKey(display->GetWindow(), GLFW_KEY_H))
		{
			tanks[MyTank].life += 50 * timeDiff2;

			D -= 0.02 * timeDiff2;
			//std::cout << D << std::endl;
		}

		camera.pos += camera.up * camU - camera.forward * camF;
		camera.Pitch(C);

		
		shader.use();
		//tanks[MyTank].Te->Bind();
		//		shader.Update(tanks[MyTank].Tr, camera);///////////////////////

		//		tank.Draw();
		/*/shader.setMat4("projection", camera.GetProjection());
		shader.setMat4("view", camera.GetView());
		shader.setMat4("model", tanks[MyTank].Tr.GetModel());
		//shader.setMat4("model", transformY.GetModel());

		//shader.Update(transformY, camera);
		//tankModel.Draw(shader);


		//shader.Update(transform3, camera);
		shader.setMat4("projection", camera.GetProjection());
		shader.setMat4("view", camera.GetView());
		shader.setMat4("model", transform3.GetModel());
		*/
		//Flag.Draw();

		//shader.Update(transform2, camera);
		shader.setMat4("projection", camera.GetProjection());
		shader.setMat4("view", camera.GetView());
		shader.setMat4("model", transform2.GetModel());


		//texture3.Bind();
		grand.Draw(shader);

		tanks[MyTank].PoS = tanks[MyTank].pos - (tanks[MyTank].forward * 1.f) + glm::vec3(tanks[MyTank].up.x, 0.89 * tanks[MyTank].up.y, tanks[MyTank].up.z);
		tanks[MyTank].PoSF = tanks[MyTank].PoS;

		for (int w = 0; w < 70; w++)
		{
			if (tanks[MyTank].PoS.y <= heightMin(tanks[MyTank].PoS.x, tanks[MyTank].PoS.z) + 2)
				w = 100;
			else
				tanks[MyTank].PoS += tanks[MyTank].forward * 3.f;
		}


		for (int i = 0; i < TotalConnections; i++)
		{
			//if (tanks[i].life <= 0) //ZMANI
			//{
			//	tanks[i] = tanks[tanks.size() - 1];
			//	tanks.pop_back();
			//	i--;
			//}
			//else
			{

				shader.use();
				glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor

				// view/projection transformations
				tanks[i].Tr.SetPos(tanks[i].pos); //////////////////////////////////////////////////////should change
				//shader.Update(tanks[i].Tr, camera);
				shader.setMat4("projection", camera.GetProjection());
				shader.setMat4("view", camera.GetView());
				shader.setMat4("model", tanks[i].Tr.GetModel());


				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				tankModel.Draw(shader);

				// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
				// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
				// the objects' size differences, making it look like borders.
				// -----------------------------------------------------------------------------------------------------------------------------
				if (i != MyTank)
				{
					shaderSingleColor.use();

					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
					glStencilMask(0x00);
					glDisable(GL_DEPTH_TEST);
					shaderSingleColor.setMat4("projection", camera.GetProjection());
					shaderSingleColor.setMat4("view", camera.GetView());
					shaderSingleColor.setMat4("model", tanks[i].Tr.GetModel());
					tankModel.Draw(shaderSingleColor);
				}
				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glEnable(GL_DEPTH_TEST);


				

				if (tanks[i].yeriya > 10 && i != MyTank)
				{

					glm::vec3 vP1 = tanks[i].PoS - tanks[MyTank].pos;
					glm::vec3 vP2 = tanks[i].PoSF - tanks[MyTank].pos;

					float l1 = glm::length(vP1);
					float l2 = glm::length(vP2);

					float lT = glm::length(vP2 - vP1);

					if (lT + 6 >= l1 + l2)
					{
						tanks[MyTank].life -= ((lT + 6 - (l1 + l2)) / 2.4f)* timeDiff1;

					}

				}

			}
		}

		
		

		glm::vec3 aq = tanks[MyTank].right() * 0.95f;
		float life = ((float)(tanks[MyTank].life - 500) / 1000) * 2;
		float li = counter/ TimeD;
		if (li >= 1)
			li = 1;
		
		glLineWidth(5);

		glColor3f(0.0, 5, 0.0);
		glBegin(GL_LINES);
		glVertex3f(tanks[MyTank].pos.x - aq.x * li, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z - aq.z * li);
		glVertex3f(tanks[MyTank].pos.x - aq.x, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z - aq.z);
		glEnd();

		glColor3f(0.0, 5, 0.0);
		glBegin(GL_LINES);
		glVertex3f(tanks[MyTank].pos.x - aq.x * life, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z - aq.z * life);
		glVertex3f(tanks[MyTank].pos.x - aq.x, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z - aq.z);
		glEnd();

		//yellow.Bind();

		glBegin(GL_LINES);
		glVertex3f(tanks[MyTank].pos.x + aq.x, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z + aq.z);
		glVertex3f(tanks[MyTank].pos.x - aq.x * li, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z - aq.z * li);
		glEnd();

		//red.Bind();

		glBegin(GL_LINES);
		glVertex3f(tanks[MyTank].pos.x + aq.x, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z + aq.z);
		glVertex3f(tanks[MyTank].pos.x - aq.x * life, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z - aq.z * life);
		glEnd();



		for (int i = 0; i < TotalConnections; i++)
		{
			tanks[i].yeriya -= timeDiff1 * 15;
			if (tanks[i].yeriya > 0)
			{
				glLineWidth(2);

				tanks[i].up = glm::normalize(tanks[i].up);
				glBegin(GL_LINES);
				glVertex3f(tanks[i].PoS.x, tanks[i].PoS.y, tanks[i].PoS.z);
				glVertex3f(tanks[i].PoSF.x, tanks[i].PoSF.y, tanks[i].PoSF.z);
				glEnd();

				float y = 1-(tanks[i].yeriya/(tanks[i].timeY+2.f));

				shader.setMat4("projection", camera.GetProjection());
				shader.setMat4("view", camera.GetView());
				trans.SetPos(glm::vec3(tanks[i].pos + 150*y*tanks[i].forward));//.x, tanks[i].PoS.y, tanks[i].PoS.z));
				shader.setMat4("model", trans.GetModel());

				bulletModel.Draw(shader);
			}
		}
		
		aq *= 0.8;
		
		glm::vec3 AAA1 = aq + 1.5f * tanks[MyTank].forward;
		glm::vec3 AAA2 = aq - tanks[MyTank].forward;
		glm::vec3 AAA3 = aq + tanks[MyTank].forward;
		glm::vec3 AAA4 = aq - 1.5f * tanks[MyTank].forward;

		
		float r1 =  height_min_with_tanks(tanks[MyTank].pos.x - AAA1.x, tanks[MyTank].pos.z - AAA1.z, tanks);
		float r2 =  height_min_with_tanks(tanks[MyTank].pos.x - AAA2.x, tanks[MyTank].pos.z - AAA2.z, tanks);
		float r3 =  height_min_with_tanks(tanks[MyTank].pos.x + AAA3.x, tanks[MyTank].pos.z + AAA3.z, tanks);
		float r4 =  height_min_with_tanks(tanks[MyTank].pos.x + AAA4.x, tanks[MyTank].pos.z + AAA4.z, tanks);

		glm::vec3 p1(tanks[MyTank].pos.x - AAA1.x, r1, tanks[MyTank].pos.z - AAA1.z);
		glm::vec3 p2(tanks[MyTank].pos.x - AAA2.x, r2, tanks[MyTank].pos.z - AAA2.z);
		glm::vec3 p3(tanks[MyTank].pos.x + AAA3.x, r3, tanks[MyTank].pos.z + AAA3.z);
		glm::vec3 p4(tanks[MyTank].pos.x + AAA4.x, r4, tanks[MyTank].pos.z + AAA4.z);

		r1 = height_min_with_tanks(tanks[MyTank].pos.x - AAA1.x, tanks[MyTank].pos.z - AAA1.z, tanks);
		r2 = height_min_with_tanks(tanks[MyTank].pos.x - AAA2.x, tanks[MyTank].pos.z - AAA2.z, tanks);
		r3 = height_min_with_tanks(tanks[MyTank].pos.x + AAA3.x, tanks[MyTank].pos.z + AAA3.z, tanks);
		r4 = height_min_with_tanks(tanks[MyTank].pos.x + AAA4.x, tanks[MyTank].pos.z + AAA4.z, tanks);


		// 1   3   (2)
		if ((p3 + (p1 - p2)).y > p4.y - 0.01)
			p4 = p3 + (p1 - p2);
		// 2   4   (1)
		if ((p2 + (p4 - p1)).y > p3.y - 0.01)
			p3 = p2 + (p4 - p1);
		// 1   3   (4)
		if ((p3 + (p1 - p4)).y > p2.y - 0.01)
			p2 = p3 + (p1 - p4);
		// 2   4   (3)
		if ((p2 + (p4 - p3)).y > p1.y - 0.01)
			p1 = p2 + (p4 - p3);

		/*
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p4.x, p4.y, p4.z);
		glEnd();

		*/

		tanks[MyTank].forward = glm::normalize(p2 - p1);
		glm::vec3 left2 = glm::normalize(p4 - p1);
		tanks[MyTank].up = glm::normalize(glm::cross(p2 - p1, left2));

		tanks[MyTank].pos = glm::vec3((p1 + p3).x / 2, (p1 + p3).y / 2, (p1 + p3).z / 2);
		tanks[MyTank].Tr.mat[0] = glm::vec4(tanks[MyTank].right(), 0);
		tanks[MyTank].Tr.mat[1] = glm::vec4(tanks[MyTank].up, 0);
		tanks[MyTank].Tr.mat[2] = glm::vec4(tanks[MyTank].forward, 0);

		

		//shader.Update(transformc, camera);

		//shader.setMat4("projection", camera.GetProjection());
		//shader.setMat4("view", camera.GetView());
		//shader.setMat4("model", transformc.GetModel());

		
		//textureC.Bind();
		//candy.Draw();


		camera.Pitch(-C);
		tanks[MyTank].forward = glm::normalize(tanks[MyTank].forward);


		counter2 += 0.005;

		
		sprintf(strT, "Time: %.4f", time);
		RenderText(shaderText, strT, 2.0f, 2.0f, 0.4f, glm::vec3(0.0, 0.0f, 0.5f));

		sprintf(strT, "fps: %.4f", 1 / (time - prevT));
		RenderText(shaderText, strT, 2.0f, 20.f, 0.4f, glm::vec3(0.0, 0.0f, 0.5f));


		prevT = time;
		



		display->SwapBuffers();
		counter += timeDiff1;
		camera.pos -= glm::vec3(camera.up.x * camU, camera.up.y * camU, camera.up.z * camU) - glm::vec3(camera.forward.x * camF, camera.forward.y * camF, camera.forward.z * camF);



		//std::cout << tanks[MyTank].pos.x << " " << tanks[MyTank].pos.y << " " << tanks[MyTank].pos.z << std::endl;
		//std::cout << camera.pos.x << " " << camera.pos.y << " " << camera.pos.z << std::endl;
		if (tanks[MyTank].pos.x < 0 || tanks[MyTank].pos.x >= heightT * sizetri || tanks[MyTank].pos.z < 0 || tanks[MyTank].pos.z >= widthT * sizetri || tanks[MyTank].life <= 0)
		{
			tanks[MyTank].life = 1000;
			tanks[MyTank].pos = glm::vec3(rand() % (sizetri * widthT) / 2 + 1, 25, rand() % (sizetri * widthT) / 2 + 1);
			camera.pos = tanks[MyTank].pos;
			float randN = rand() / 10.f;
			Y = tanks[MyTank].rotRight(randN);
			tanks[MyTank].Tr.mat *= Y;
			tanks[MyTank].forward = glm::vec3(glm::normalize(tanks[MyTank].rotRight(randN) * glm::vec4(tanks[MyTank].forward, 0.0)));
			Sleep(500);
		}
		//std::cout << timeDiff1 << std::endl;

		time1 = time2;
	}

	return 0;
}

float height_min_with_tanks(float x, float z, std::vector<Tank> tt, int my)
{
	float y = heightMin(x, z);

	for (int i = 0; i < tt.size(); i++)
	{
		if (my != i)
		{
			float r = glm::length(glm::vec3(x, y, z) - tt[i].pos);
			if (r <= 2 && y >= tt[i].pos.y)
				y += (3 - r) / 3;
		}
	}

	return y;
}

glm::vec3 hetel(glm::vec3 P1, glm::vec3 P2, glm::vec3 V)
{
	float a1 = glm::dot(V, P1);
	float h1 = glm::length(P1);
	float d1 = a1 / h1;
	glm::vec3 v1 = P1 * d1;

	float a2 = glm::dot(V, P2);
	float h2 = glm::length(P2);
	float d2 = a2 / h2;
	glm::vec3 v2 = P2 * d2;

	return v1 + v2;
}

float heightMin(float x, float y)
{
	if (x < 0 || x > heightT * sizetri || y < 0 || y > widthT * sizetri)
		return -1;


	float X = x - (float)((int)x / sizetri) * sizetri;
	float Y = y - (float)((int)y / sizetri) * sizetri;

	int xi = x / sizetri;
	int yi = y / sizetri;

	if (sizetri - X <= Y)
	{
		glm::vec3 v1 = *fl[yi * heightT + xi + 1].GetPos() - *fl[(yi + 1) * heightT + xi].GetPos();
		glm::vec3 v2 = *fl[yi * heightT + xi + 1].GetPos() - *fl[(yi + 1) * heightT + xi + 1].GetPos();
		glm::vec3 point = *fl[yi * heightT + xi + 1].GetPos();

		glm::vec3 a = glm::vec3(v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);

		float equal = a.x * point.x + a.y * point.y + a.z * point.z;
		return (a.y ? (equal - a.x * x - a.z * y) / a.y : 0);
	}
	else
	{
		glm::vec3 v1 = *fl[yi * heightT + xi].GetPos() - *fl[(yi + 1) * heightT + xi].GetPos();
		glm::vec3 v2 = *fl[yi * heightT + xi].GetPos() - *fl[yi * heightT + xi + 1].GetPos();
		glm::vec3 point = *fl[yi * heightT + xi + 1].GetPos();

		glm::vec3 a = glm::vec3(v1.y * v2.z - v2.y * v1.z, v1.z * v2.x - v2.z * v1.x, v1.x * v2.y - v2.x * v1.y);

		float equal = a.x * point.x + a.y * point.y + a.z * point.z;
		return (a.y ? (equal - a.x * x - a.z * y) / a.y : 0);
	}


	return 0;
}