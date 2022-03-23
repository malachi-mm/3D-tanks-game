#include <iostream>
#include <SDL2/SDL.h>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "inputDetection.h"
#include <vector>
#include <queue>
#include <ctime>
#include <chrono>
#include<glm/gtx/vector_angle.hpp>

static const int DISPLAY_WIDTH = 1000;
static const int DISPLAY_HEIGHT = 800;

#undef main

struct Cube
{
	Transform *T;
	Texture *TE;
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

Vertex vertices1[] =
{
	Vertex(glm::vec3(-100, -3, -100), glm::vec2(1, 0), glm::vec3(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3))),
	Vertex(glm::vec3(-100, -1, -100), glm::vec2(0, 0), glm::vec3(-1 / sqrt(3), 1 / sqrt(3), -1 / sqrt(3))),
	Vertex(glm::vec3(100, -1, -100), glm::vec2(1, 0), glm::vec3(1 / sqrt(3), 1 / sqrt(3), -1 / sqrt(3))),
	Vertex(glm::vec3(100, -3, -100), glm::vec2(1, 1), glm::vec3(1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3))),

	Vertex(glm::vec3(-100, -3, 100), glm::vec2(0, 0), glm::vec3(-1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3))),
	Vertex(glm::vec3(-100, -1, 100), glm::vec2(0, 1), glm::vec3(-1 / sqrt(3), 1 / sqrt(3), 1 / sqrt(3))),
	Vertex(glm::vec3(100, -1, 100), glm::vec2(1, 1), glm::vec3(1 / sqrt(3), 1 / sqrt(3), 1 / sqrt(3))),
	Vertex(glm::vec3(100, -3, 100), glm::vec2(0, 1), glm::vec3(1 / sqrt(3), -1 / sqrt(3), 1 / sqrt(3))),

	//////////////////////////////Vertex(glm::vec3(-100, -3, -100), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
	/////////////////////////Vertex(glm::vec3(-100, -3, 100), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
	///////////////////////////////////Vertex(glm::vec3(100, -3, 100), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
	//////////////////////////////////Vertex(glm::vec3(100, -3, -100), glm::vec2(0, 0), glm::vec3(0, -1, 0)),

	/////////////////////////////Vertex(glm::vec3(-100, -1, -100), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
	//////////////////////Vertex(glm::vec3(-100, -1, 100), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
	//////////////////////////////////Vertex(glm::vec3(100, -1, 100), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
	////////////////////////////Vertex(glm::vec3(100, -1, -100), glm::vec2(0, 0), glm::vec3(0, 1, 0)),

	/////////////////////Vertex(glm::vec3(-100, -3, -100), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
	/////////////////////////Vertex(glm::vec3(-100, -3, 100), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
	////////////////////////////Vertex(glm::vec3(-100, -1, 100), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
	//////////////////////////////////////////Vertex(glm::vec3(-100, -1, -100), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),

	////////////////////////////////////////Vertex(glm::vec3(100, -3, -100), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
	/////////////////////////////////////////Vertex(glm::vec3(100, -3, 100), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
	/////////////////////////////////////////Vertex(glm::vec3(100, -1, 100), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
	///////////////////////////////////////////Vertex(glm::vec3(100, -1, -100), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
};

unsigned int indices[] = {
	0, 1, 2,
	0, 2, 3,

	3, 2, 6,
	3, 6, 7,

	7, 6, 5,
	7, 5, 4,

	4, 5, 1,
	4, 1, 0,

	6, 2, 1,
	6, 1, 5,

	3, 7, 4,
	3, 4, 0
};




struct yeriya
{
	glm::vec3 kivun;
	glm::vec3 pos;
};

#define MyTank 0
#define sizetri 3
#define widthT 64
#define heightT 64
Vertex fl[widthT*heightT + 8];
unsigned int indices1[(widthT - 1) * (heightT - 1) * 2 * 3 + 3 * 8];
typedef std::chrono::high_resolution_clock Clock;


glm::vec3 hetel(glm::vec3 P1, glm::vec3 P2, glm::vec3 V);
float heightMin(float x, float y);

//glm::vec3 De(glm::vec3 a, glm::vec3 kivun, glm::vec3 up)
//{
//	if (a.x < 0 || a.x >= 30 * sizetri || a.z < 0 || a.z >= 30 * sizetri)
//		return glm::vec3(0, 0, 0);
//
//	glm::vec3 right = glm::cross(kivun, up);
//
//	glm::vec3 D;
//
//
//	float X = a.x - (float)((int)a.x / sizetri) * sizetri;
//	float Y = a.z - (float)((int)a.z / sizetri) * sizetri;
//
//
//	int xi = a.x / sizetri;
//	int yi = a.z / sizetri;
//
//	glm::vec3 v1;
//	glm::vec3 v2;
//
//	if (sizetri - X <= Y)
//	{
//		v1 = *fl[(yi + 1) * 30 + xi + 1].GetPos() - *fl[(yi + 1) * 30 + xi].GetPos();
//		v2 = *fl[(yi + 1) * 30 + xi + 1].GetPos() - *fl[yi * 30 + xi + 1].GetPos();
//	}
//	else
//	{
//		v1 = *fl[yi * 30 + xi].GetPos() - *fl[(yi + 1) * 30 + xi].GetPos();
//		v2 = *fl[yi * 30 + xi].GetPos() - *fl[yi * 30 + xi + 1].GetPos();
//	}
//
//	glm::vec3 ux = hetel(v1, v2, glm::vec3(1, 0, 0));
//	glm::vec3 uy = hetel(v1, v2, /*glm::vec3(0, 1, 0)*/up);
//	glm::vec3 uz = hetel(v1, v2, glm::vec3(0, 0, 1));
//
//	glm::vec3 vx = hetel(right, kivun, glm::vec3(1, 0, 0));
//	glm::vec3 vy = hetel(right, kivun, /*glm::vec3(0, 1, 0)*/up);
//	glm::vec3 vz = hetel(right, kivun, glm::vec3(0, 0, 1));
//
//	D.x = acosf(glm::dot(ux, vx) / (glm::length(ux) *glm::length(vx)));
//	D.y = 0;// acosf(glm::dot(uy, vy) / (glm::length(uy) *glm::length(vy)));
//	D.z = acosf(glm::dot(uz, vz) / (glm::length(uz) *glm::length(vz)));
//
//
//	return D;
//
//}


struct Tank
{
	Transform Tr;
	Texture *Te;
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 forward;
	int life = 1000;

	glm::vec3 PoS = glm::vec3(0, 0, 0);
	glm::vec3 PoSF = glm::vec3(0, 0, 0);
	int yeriya = 0;
	const int timeY = 250;

	Tank(Texture& te, glm::vec3 POS, glm::vec3 UP, glm::vec3 FORW)
	{
		Te = &te;
		pos = POS;
		up = UP;
		forward = FORW;
	}

	void Tank::operator=(const Tank& t)
	{
		Te = t.Te;
		pos = t.pos;
		up = t.up;
		forward = t.forward;
		life = t.life;
		PoS = t.PoS;
		PoSF = t.PoSF;
		yeriya = t.yeriya;
	}

	void MoveForward(float amt)
	{
		pos += forward * amt;
	}

	void MoveForwardN(float amt)
	{
		pos += glm::vec3(forward.x, 0, forward.z) * amt;
	}

	void MoveRight(float amt)
	{
		pos += glm::cross(up, forward) * amt;
	}

	glm::mat4 rotRight(float angle)
	{
		return glm::rotate(angle, up);
	}
	glm::mat4 rotZ(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		return glm::rotate(angle, right);
	}

	void Pitch(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));

		//glm::mat4 aa = glm::rotate(angle, right);

		up = glm::normalize(glm::cross(forward, right));

	}

	void RotateY(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	}

	void RotateUP(float angle)
	{
		static const glm::vec3 UP = this->up;

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
	}

	glm::vec3 right()
	{
		up = glm::normalize(up);
		forward = glm::normalize(forward);

		return glm::normalize(glm::cross(up, forward));
	}

	glm::vec3 rot(float angle)
	{

		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		glm::vec3 r;
		r.x = glm::rotate(angle, right)[0][0];
		r.y = glm::rotate(angle, right)[1][1];
		r.z = glm::rotate(angle, right)[2][2];

		return r;
	}
};


float height_min_with_tanks(float x, float z, std::vector<Tank> tt, int my = MyTank);






float f(int x, int y,  int oct, float B, float randY[])
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




int main(int argc, char** argv)
{
	srand((unsigned)NULL(time));


	float Yrandom[widthT * heightT + 8];
	for (int i = 0; i < widthT * heightT + 8; i++)
	{
		Yrandom[i] = 2*(float)rand() / (float)RAND_MAX;
	}
	//Yrandom[0] = 0;

	//system("pause");


	for (int i = 0; i < widthT; i++)
	{
		for (int j = 0; j < heightT; j++)
		{
			fl[i * heightT + j].GetPos()->x = j * sizetri;
			fl[i * heightT + j].GetPos()->z = i * sizetri;
			fl[i * heightT + j].GetPos()->y = 30*f(j, i, 6, 2, Yrandom);

			fl[i * heightT + j].normal = glm::vec3(0, 1, 0);
			fl[i * heightT + j].texCoord = glm::vec2((i + 1) % 2, (j + 1) % 2);
		}
		//std::cout << std::endl;
	}
	//system("pause");
	/**
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
	}*/

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

	/*fl[900].pos =
	fl[901].pos =
	fl[902].pos =
	fl[903].pos =
	fl[904].pos =
	fl[905].pos =
	fl[906].pos =
	fl[907].pos =
	*/





	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	input::InputDetector inp(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	SDL_ShowCursor(SDL_DISABLE);
	std::cout << '#';

	//ver(100,1,100,vertices1,0,-2,0);
	//ver(2, 5, 100, vertices2, 100, 4, 0);
	//ver(2, 5, 100, vertices3,-100,4,0);
	//ver(100, 5, 2, vertices4, 2, 4, 100);

	int iii = 0;


	Mesh grand(fl, sizeof(fl) / sizeof(fl[0]), indices1, sizeof(indices1) / sizeof(indices1[0]));
	std::cout << '#';

	//Mesh kir1(vertices2, sizeof(vertices1) / sizeof(vertices1[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh kir2(vertices3, sizeof(vertices1) / sizeof(vertices1[0]), indices, sizeof(indices) / sizeof(indices[0]));
	//Mesh kir3(vertices4, sizeof(vertices1) / sizeof(vertices1[0]), indices, sizeof(indices) / sizeof(indices[0]));
	Mesh floor(vertices1, sizeof(vertices1) / sizeof(vertices1[0]), indices, sizeof(indices) / sizeof(indices[0]));
	std::cout << '#';

	Mesh monkey("./res/monkey3.obj");
	std::cout << '#';

	Mesh tank("./res/tiger2.obj");
	std::cout << '#';

	Mesh road("./res/setforbridge.txt");
	std::cout << '#';

	Mesh candy("./res/candyCane1.txt");
	std::cout << '#';



	//Mesh Flag("./res/flag4.txt");
	Shader shader("./res/basicShader");
	std::cout << '#';

	Shader shaderM("./res/monkeyShader");
	std::cout << '#';

	Texture textureR("./res/ConcreteBare0314_2_S.jpg");
	std::cout << '#';

	Texture red("./res/red.png");
	std::cout << '#';

	Texture yellow("./res/yellow.png");
	std::cout << '#';


	Texture texture3("./res/bricks.jpg");
	std::cout << '#';

	Texture texture2("./res/gray.png");
	std::cout << '#';

	Texture texture("./res/gra.png");
	std::cout << '#';

	Texture textureT("./res/Tank_Body_Forest.png");
	std::cout << '#';

	Texture textureC("./res/candy_cane_diffuse.jpg");
	std::cout << '#';




	Transform transformc;
	transformc.GetRot()->z = 1.5;
	transformc.GetPos()->y = 7;

	Transform transform2;
	Transform transform3;

	Transform transformY;


	transform3.SetPos(glm::vec3(101, -5, 101));
	transform3.SetScale(glm::vec3(30, 30, 30));
	std::cout << '#';


	Camera camera(glm::vec3(5.0f, 10.0f, 5.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 1000.0f);
	std::cout << '#';

	Tank t1(textureT, camera.pos, camera.up, camera.forward);
	std::cout << '#';

	std::vector<Tank> tanks;
	tanks.push_back(t1);
	for (int i = 0; i < 10; i++)
		tanks.push_back(Tank(textureT, glm::vec3(15*i, heightMin(15*i, 15*i), 15*i), camera.up, camera.forward));

	std::cout << '#';

	glm::mat3 R = rotatez(1.5708);
	glm::vec3 N;



	SDL_Event e;
	bool isRunning = true;
	float counter = 0.0f;

	float upx = 0;
	float upy = 0;
	int yr = 0;
	float goup = 0.5;
	float godown = -goup;
	float C = 0;
	float D = 10;
	float camU = 4;
	float camF = 10;
	float rotM = 0;
	float rotP = 0;
	float counter2 = 0;
	int lorr = 1;

	tanks[MyTank].forward = camera.forward;
	tanks[MyTank].up = camera.up;

	glm::mat4 H;
	glLineWidth(2);

	clock_t time1 = std::clock();
	clock_t time2;
	std::cout << '#';


	while (isRunning)
	{
		time2 = std::clock();
		float timeDiff1 = time2 - time1;
		float timeDiff2 = timeDiff1 / 16;
		timeDiff1 /= 32;


		tanks[MyTank].up = glm::normalize(tanks[MyTank].up);
		tanks[MyTank].forward = glm::normalize(tanks[MyTank].forward);

		//camera.forward = tanks[MyTank].forward;
		//camera.up = tanks[MyTank].up;

		{
			SDL_Delay(C);

			inp.update();

			glm::mat4 Z;
			glm::mat4 Y;



			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
					isRunning = false;
			}

			display.Clear(0.02f, 0.95f, 0.92f, 1.0f);




			//camera.projection *= transform.GetModel();

			if (inp.KeyDown(SDL_SCANCODE_Q))
				camU += 0.1;
			if (inp.KeyDown(SDL_SCANCODE_E))
				camU -= 0.1;

			if (inp.KeyDown(SDL_SCANCODE_A))
				camF += 0.1;
			if (inp.KeyDown(SDL_SCANCODE_D))
				camF -= 0.1;

			if (inp.KeyDown(SDL_SCANCODE_U))
				camera.pos += glm::vec3(camera.up.x * goup, camera.up.y * goup, camera.up.z * goup);
			if (inp.KeyDown(SDL_SCANCODE_D))
				camera.pos += glm::vec3(camera.up.x * godown, camera.up.y * godown, camera.up.z * godown);

			if (inp.KeyDown(SDL_SCANCODE_UP))
			{
				camera.MoveForward((goup *timeDiff1));
				tanks[MyTank].MoveForward((goup *timeDiff1));

			}
			if (inp.KeyDown(SDL_SCANCODE_LEFT) && inp.KeyDown(SDL_SCANCODE_LSHIFT))
			{
				camera.RotateY((0.01 * timeDiff2)*timerotsome);
				rotM += (0.01 * timeDiff2)*timerotsome;
			}
			else if (inp.KeyDown(SDL_SCANCODE_LEFT))
			{
				//	timeDiff /= 64;
				//tanks[MyTank].Tr.mat *= tanks[MyTank].rotRight(0.01);
				Y = tanks[MyTank].rotRight((0.01 * timeDiff2)*timerotsome);
				tanks[MyTank].Tr.mat *= Y;

			//	camera.RotateY((0.01 * timeDiff2)*timerotsome);
			//	rotM += (0.01 * timeDiff2)*timerotsome;
				tanks[MyTank].forward = glm::vec3(glm::normalize(tanks[MyTank].rotRight((0.01 * timeDiff2)*timerotsome) * glm::vec4(tanks[MyTank].forward, 0.0)));
				//timeDiff *= 64;
			}

			if (inp.KeyDown(SDL_SCANCODE_DOWN))
			{
				camera.MoveForward(godown * timeDiff1);
				tanks[MyTank].MoveForward(godown * timeDiff1);
			}
			if (inp.KeyDown(SDL_SCANCODE_RIGHT) && inp.KeyDown(SDL_SCANCODE_LSHIFT))
			{
				camera.RotateY((-0.01 * timeDiff2)*timerotsome);
				rotM -= (0.01 * timeDiff2)*timerotsome;
			}
			else if (inp.KeyDown(SDL_SCANCODE_RIGHT))
			{
				//timeDiff /= 64;

				//tanks[MyTank].Tr.mat *= tanks[MyTank].rotRight(- 0.01);
				Y = tanks[MyTank].rotRight((-0.01 * timeDiff2)*timerotsome);
				tanks[MyTank].Tr.mat *= Y;


				//camera.RotateY((-0.01 * timeDiff2)*timerotsome);
				//rotM -= (0.01 * timeDiff2)*timerotsome;

				tanks[MyTank].forward = glm::vec3(glm::normalize(tanks[MyTank].rotRight((-0.01 * timeDiff2)*timerotsome) * glm::vec4(tanks[MyTank].forward, 0.0)));
				//tanks[MyTank].up = camera.up;
				//	timeDiff *= 64;

			}

			camera.forward = glm::normalize(glm::vec3(tanks[MyTank].forward.x, /*camera.forward.y*/0, tanks[MyTank].forward.z));			
			
			/*if (camera.pos.y > 2)
			camera.pos.y -= 0.05;*/
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

			if (inp.KeyDown(SDL_SCANCODE_SPACE) && camera.pos.y && counter >= 0.5 * timeDiff1)
			{
				tanks[MyTank].yeriya = tanks[MyTank].timeY + 2;
				counter = 0;
			//	tanks[MyTank].PoS = tanks[MyTank].pos - (tanks[MyTank].forward * 1.f);
				//PoS.y += C;
			//	tanks[MyTank].PoSF = tanks[MyTank].PoS;

				for (int j = 0; j < tanks.size(); j++)
				{
					tanks[j].yeriya = tanks[j].timeY + 2;

				}
				//std::cout << std::endl;

			}


			if (inp.KeyDown(SDL_SCANCODE_W))
			{
				//tanks[MyTank].Tr.mat *= tanks[MyTank].rotZ(0.01);
				Z = tanks[MyTank].rotZ(0.01 * timeDiff2);
				tanks[MyTank].Tr.mat *= Z;

			/*	tanks[MyTank].forward =  tanks[MyTank].forward * Z;
				tanks[MyTank].up *= Z;*/


				tanks[MyTank].forward = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].forward, 0.0)));

				//glm::mat4 aa = glm::rotate(angle, right);

				tanks[MyTank].up = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].up, 0.0)));


				//tanks[MyTank].Pitch(0.01 * timeDiff2);
				/*	H = tanks[MyTank].rotZ(0.01);
				glm::vec3 right = glm::normalize(glm::cross(tanks[MyTank].up, tanks[MyTank].forward));

				tanks[MyTank].forward = glm::vec3(glm::normalize(H * glm::vec4(tanks[MyTank].forward, 0.0)));
				tanks[MyTank].up = glm::normalize(glm::cross(tanks[MyTank].forward, right));*/
				rotP += 0.01 * timeDiff2;
			}

			if (inp.KeyDown(SDL_SCANCODE_S))
			{
				//tanks[MyTank].Tr.mat *= tanks[MyTank].rotZ(- 0.01);
				Z = tanks[MyTank].rotZ(-0.01 * timeDiff2);
				tanks[MyTank].Tr.mat *= Z;


		/*		tanks[MyTank].forward *= Z;
				tanks[MyTank].up *= Z;*/

				tanks[MyTank].forward = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].forward, 0.0)));

				//glm::mat4 aa = glm::rotate(angle, right);

				tanks[MyTank].up = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].up, 0.0)));

				//tanks[MyTank].Pitch(-0.01 * timeDiff2);

				/*H = tanks[MyTank].rotZ(-0.01);
				glm::vec3 right = glm::normalize(glm::cross(tanks[MyTank].up, tanks[MyTank].forward));

				tanks[MyTank].forward = glm::vec3(glm::normalize(H * glm::vec4(tanks[MyTank].forward, 0.0)));
				tanks[MyTank].up = glm::normalize(glm::cross(tanks[MyTank].forward, right));*/
				rotP -= 0.01 * timeDiff2;
			}




			//	std::cout << heightMin(camera.pos.x, camera.pos.z) << std::endl;

			tanks[MyTank].Tr.SetPos(camera.pos);
			tanks[MyTank].pos = camera.pos;
		//	camera.pos = tanks[MyTank].pos;
			if (inp.KeyDown(SDL_SCANCODE_P))
			{
				tanks[MyTank].forward = camera.forward;

				tanks[MyTank].up = camera.up;
				rotP = 0;
				tanks[MyTank].Tr.mat = tanks[MyTank].rotRight(rotM);

				camU = 4;
				camF = 9;
			}

			//camera.forward = tanks[MyTank].forward;
			//camera.up = tanks[MyTank].up;

			//tanks[MyTank].Pitch(1);

			//std::cout << inp.getMouseX() << "    " << inp.getMouseY() << std::endl;

			float xplus = inp.getMouseXNormalized();
			float yplus = inp.getDeltaMouseY();
			upy = yplus;

			if (inp.KeyDown(SDL_SCANCODE_N))
			{
				C += 0.001;
				//std::cout << C << std::endl;
			}
			if (inp.KeyDown(SDL_SCANCODE_B))
			{
				C -= 0.001;
				//std::cout << C << std::endl;
			}

			if (inp.KeyDown(SDL_SCANCODE_G))
			{
				tanks[MyTank].life -= 50;

				D += 0.02;
				//std::cout << D << std::endl;
			}
			if (inp.KeyDown(SDL_SCANCODE_H))
			{
				tanks[MyTank].life += 50;

				D -= 0.02;
				//std::cout << D << std::endl;
			}

			camera.pos += glm::vec3(camera.up.x * camU, camera.up.y * camU, camera.up.z * camU) - glm::vec3(camera.forward.x * camF, camera.forward.y * camF, camera.forward.z * camF);
			camera.Pitch(C);

			shader.Bind();
			tanks[MyTank].Te->Bind();
	//		shader.Update(tanks[MyTank].Tr, camera);///////////////////////

	//		tank.Draw();

			shader.Update(transformY, camera);
			tank.Draw();


			shader.Update(transform3, camera);

			//Flag.Draw();


			for (int i = 0; i < tanks.size(); i++)
			{
				if (tanks[i].life <= 0)
				{
					tanks[i] = tanks[tanks.size() - 1];
					tanks.pop_back();
					i--;
				}
				else
				{
					tanks[i].Tr.SetPos(tanks[i].pos); //////////////////////////////////////////////////////should change
					//trT.mat = transform1.mat;
					shader.Update(tanks[i].Tr, camera);

					tank.Draw();

					if (i != MyTank)
					{

						tanks[i].pos.x += cos(counter2) / 20;
						tanks[i].pos.z += 0;// +sin(counter2);
						tanks[i].pos.y = height_min_with_tanks(tanks[i].pos.x , tanks[i].pos.z, tanks,i);


						tanks[i].Tr.mat *= tanks[i].rotRight((-0.01 * timeDiff2) * timerotsome);
						tanks[i].forward = glm::vec3(glm::normalize(tanks[i].rotRight((-0.01 * timeDiff2) * timerotsome) * glm::vec4(tanks[i].forward, 0.0)));


					}////will delete


					//std::cout << tanks[i].pos.x << " " << tanks[i].pos.y << " " << tanks[i].pos.z << " " << std::endl;


					if (tanks[i].yeriya > tanks[i].timeY)
					{
						tanks[i].PoS = tanks[i].pos - (tanks[i].forward * 1.f) + glm::vec3(tanks[i].up.x, 0.89 * tanks[i].up.y, tanks[i].up.z);
						tanks[i].PoSF = tanks[i].PoS;


						for (int j = 0; j < tanks.size(); j++)
						{
							tanks[i].PoS = tanks[i].pos - (tanks[i].forward * 1.f);

							int w = 0;
							for (; w < 70; w++)
							{
								float yy = glm::length(tanks[i].PoS - tanks[j].pos);
								if (yy <= 2.1 && i != j)
									tanks[j].life -= (3.2 - yy) * 100;
								if (tanks[i].PoS.y <= heightMin(tanks[i].PoS.x, tanks[i].PoS.z) - 1)
								{
									//std::cout << "boom!!!!!!!      the bombe at: " << w << std::endl;
									//transformY.SetPos(PoS - tanks[MyTank].forward * 3.f);
									w = 100;
								}
								else
								{
									tanks[i].PoS += tanks[i].forward * 3.f;
								}

							}
							/*if (i < 100)
							std::cout << "loooooooser" << std::endl;*/
							//std::cout << "                             " << tanks[j].life << "     ";

						}
					}
					
				}
			}

			shader.Update(transform2, camera);


			texture3.Bind();
			grand.Draw();



			glm::vec3 aq = tanks[MyTank].right()*0.95f;
			float life = ((float)(tanks[MyTank].life - 500) / 1000) * 2;
			//std::cout << life << std::endl;
			float li = (counter - 0.25) * 4;
			if (li >= 1)
				li = 1;

			glLineWidth(5);

			glColor3f(0.0, 5, 0.0);
			glBegin(GL_LINES);
			glVertex3f(tanks[MyTank].pos.x - aq.x*li, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z - aq.z*li);
			glVertex3f(tanks[MyTank].pos.x - aq.x, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z - aq.z);
			glEnd();

			glColor3f(0.0, 5, 0.0);
			glBegin(GL_LINES);
			glVertex3f(tanks[MyTank].pos.x - aq.x*life, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z - aq.z*life);
			glVertex3f(tanks[MyTank].pos.x - aq.x, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z - aq.z);
			glEnd();

			yellow.Bind();

			glBegin(GL_LINES);
			glVertex3f(tanks[MyTank].pos.x + aq.x, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z + aq.z);
			glVertex3f(tanks[MyTank].pos.x - aq.x*li, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z - aq.z*li);
			glEnd();

			red.Bind();

			glBegin(GL_LINES);
			glVertex3f(tanks[MyTank].pos.x + aq.x, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z + aq.z);
			glVertex3f(tanks[MyTank].pos.x - aq.x*life, tanks[MyTank].pos.y + 1.6, tanks[MyTank].pos.z - aq.z*life);
			glEnd();

		
			//if (tanks[MyTank].yeriya-- > 0)
			//{
			//	std::cout << "PoS: " << tanks[MyTank].PoS.x << " " << tanks[MyTank].PoS.y << " " << tanks[MyTank].PoS.z << std::endl;
			//	std::cout << "PoSF: " << tanks[MyTank].PoSF.x << " " << tanks[MyTank].PoSF.y << " " << tanks[MyTank].PoSF.z << std::endl;
			//
			//	glLineWidth(2);
			//	red.Bind();
			//
			//	glBegin(GL_LINES);
			//	glVertex3f(tanks[MyTank].PoS.x, tanks[MyTank].PoS.y + 0.89, tanks[MyTank].PoS.z);
			//	glVertex3f(tanks[MyTank].PoSF.x, tanks[MyTank].PoSF.y + 0.89, tanks[MyTank].PoSF.z);
			//	glEnd();
			//}
			for (int i = 0; i < tanks.size(); i++)
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
				}
			}

			aq *= 0.8;

			glm::vec3 AAA1 = aq + 1.5f*tanks[MyTank].forward;
			glm::vec3 AAA2 = aq - tanks[MyTank].forward;
			glm::vec3 AAA3 = aq + tanks[MyTank].forward;
			glm::vec3 AAA4 = aq - 1.5f*tanks[MyTank].forward;


			float r1 =/*tanks[MyTank].pos.y - AAA1.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x - AAA1.x, tanks[MyTank].pos.z - AAA1.z, tanks);
			float r2 =/*tanks[MyTank].pos.y - AAA2.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x - AAA2.x, tanks[MyTank].pos.z - AAA2.z, tanks);
			float r3 =/*tanks[MyTank].pos.y + AAA3.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x + AAA3.x, tanks[MyTank].pos.z + AAA3.z, tanks);
			float r4 =/*tanks[MyTank].pos.y + AAA4.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x + AAA4.x, tanks[MyTank].pos.z + AAA4.z, tanks);
		/*	float help12 = 0.001* timeDiff1;
			r1 = r1 < (tanks[MyTank].pos.y - AAA1.y - help12) ? tanks[MyTank].pos.y - AAA1.y - help12 : r1;
			r2 = r2 < (tanks[MyTank].pos.y - AAA2.y - help12) ? tanks[MyTank].pos.y - AAA2.y - help12 : r2;
			r3 = r3 < (tanks[MyTank].pos.y + AAA3.y - help12) ? tanks[MyTank].pos.y + AAA3.y - help12 : r3;
			r4 = r4 < (tanks[MyTank].pos.y + AAA4.y - help12) ? tanks[MyTank].pos.y + AAA4.y - help12 : r4;*/

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


			glBegin(GL_LINE_LOOP);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p4.x, p4.y, p4.z);
			glEnd();

			

			tanks[MyTank].forward = glm::normalize(p2 - p1);
			glm::vec3 left2 = glm::normalize(p4 - p1);
			tanks[MyTank].up = glm::normalize(glm::cross(p2 - p1, left2));

			tanks[MyTank].pos = glm::vec3((p1 + p3).x / 2, (p1 + p3).y / 2, (p1 + p3).z / 2);
			tanks[MyTank].Tr.mat[0] = glm::vec4(tanks[MyTank].right(), 0);
			tanks[MyTank].Tr.mat[1] = glm::vec4(tanks[MyTank].up, 0);
			tanks[MyTank].Tr.mat[2] = glm::vec4(tanks[MyTank].forward, 0);

		

			//send socket:!!
			//
			//transform1.
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//
			//





			shader.Update(transformc, camera);
			textureC.Bind();
			candy.Draw();


			camera.Pitch(-C);
			tanks[MyTank].forward = glm::normalize(tanks[MyTank].forward);


		//	camera.forward = tanks[MyTank].forward;

			//hetel(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1));

		}

		counter2 += 0.005;

		display.SwapBuffers();
		counter += 0.01f;
		camera.pos -= glm::vec3(camera.up.x * camU, camera.up.y * camU, camera.up.z * camU) - glm::vec3(camera.forward.x * camF, camera.forward.y * camF, camera.forward.z * camF);

		//	camera.Pitch(-rotP);

		//x += v(time2 - tim1)
		/*tanks[0].pos.z += cosf(counter2) / 2;
		tanks[0].pos.y  = heightMin(tanks[0].pos.x, tanks[0].pos.z);
		*/
		
		//std::cout << tanks[MyTank].pos.x << " "  << tanks[MyTank].pos.y << " " << tanks[MyTank].pos.z << std::endl;
		//std::cout << camera.pos.x << " "  << camera.pos.y << " " << camera.pos.z << std::endl;

		if (tanks[MyTank].pos.x < 0 || tanks[MyTank].pos.x >= heightT * sizetri || tanks[MyTank].pos.z < 0 || tanks[MyTank].pos.z >= widthT * sizetri || tanks[MyTank].life <= 0)
			isRunning = false;

		//std::cout << timeDiff1 << std::endl;
		//camera.pos = tanks[MyTank].pos;
		//	SDL_Delay(1000000 - (int)((time1 - time2)/100000));
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
	glm::vec3 v1 = P1*d1;

	float a2 = glm::dot(V, P2);
	float h2 = glm::length(P2);
	float d2 = a2 / h2;
	glm::vec3 v2 = P2*d2;

	return v1 + v2;
}

float heightMin(float x, float y)
{
	if (x < 0 || x > heightT* sizetri || y < 0 || y > widthT* sizetri)
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

		glm::vec3 a = glm::vec3(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y);

		float equal = a.x*point.x + a.y*point.y + a.z*point.z;
		return (a.y ? (equal - a.x*x - a.z*y) / a.y : 0);
	}
	else
	{
		glm::vec3 v1 = *fl[yi * heightT + xi].GetPos() - *fl[(yi + 1) * heightT + xi].GetPos();
		glm::vec3 v2 = *fl[yi * heightT + xi].GetPos() - *fl[yi * heightT + xi + 1].GetPos();
		glm::vec3 point = *fl[yi * heightT + xi + 1].GetPos();

		glm::vec3 a = glm::vec3(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y);

		float equal = a.x*point.x + a.y*point.y + a.z*point.z;
		return (a.y ? (equal - a.x*x - a.z*y) / a.y : 0);
	}


	return 0;
}