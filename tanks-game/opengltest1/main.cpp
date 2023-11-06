#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <string>

SOCKET Connection;


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
#define Max 20


#undef main

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

int MyTank = 0;
float myPos[] = { 10,5,10 };

#define sizetri 5
#define widthT 64
#define heightT 64
Vertex fl[widthT * heightT + 8];
unsigned int indices1[(widthT - 1) * (heightT - 1) * 2 * 3 + 3 * 8];
typedef std::chrono::high_resolution_clock Clock;
bool isRunning = true;
int TotalConnections = 1;

glm::vec3 hetel(glm::vec3 P1, glm::vec3 P2, glm::vec3 V);
float heightMin(float x, float y);


struct Tank
{
	Transform Tr;
	Texture* Te;
	glm::vec3 pos = { 0,0,0 };
	glm::vec3 up;
	glm::vec3 forward;
	int life = 1000;
	//glm::vec3 a = glm::vec3(0, 0, 0);

	glm::vec3 PoS = glm::vec3(0, 0, 0);
	glm::vec3 PoSF = glm::vec3(0, 0, 0);
	int yeriya = 0;
	const int timeY = 250;

	Tank() {	}


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
std::vector<Tank> tanks;


float height_min_with_tanks(float x, float z, std::vector<Tank> tt, int my = MyTank);


int connectToServer();


struct playerT
{
	float pos[3];
	float forward[3];
	float up[3];

	float Pos[3];
	float PosF[3];
	int yeriya;

	float Tpos[3];
	float Trot[3];
	float Tscale[3];
	float Tmat[4][4];
};


void ClientThread()
{
	while (isRunning)
	{
		int len;
		char* msgType;
		recv(Connection, (char*)&len, sizeof(int), NULL);
		if (len != 0)
		{
			msgType = new char[len + 1];
			msgType[len] = '\0';
			recv(Connection, msgType, len, NULL);
			if ((std::string)msgType == "exit")
			{
				isRunning = false;
				delete msgType;
			}
			else if ((std::string)msgType == "get")
			{
				playerT pt;
				pt.pos[0] = myPos[0];
				pt.pos[1] = myPos[1];
				pt.pos[2] = myPos[2];

				pt.forward[0] = tanks[MyTank].forward.x;
				pt.forward[1] = tanks[MyTank].forward.y;
				pt.forward[2] = tanks[MyTank].forward.z;

				pt.up[0] = tanks[MyTank].up.x;
				pt.up[1] = tanks[MyTank].up.y;
				pt.up[2] = tanks[MyTank].up.z;

				pt.Pos[0] = tanks[MyTank].PoS[0];
				pt.Pos[1] = tanks[MyTank].PoS[1];
				pt.Pos[2] = tanks[MyTank].PoS[2];

				pt.PosF[0] = tanks[MyTank].PoSF[0];
				pt.PosF[1] = tanks[MyTank].PoSF[1];
				pt.PosF[2] = tanks[MyTank].PoSF[2];

				pt.yeriya = tanks[MyTank].yeriya;

				pt.Tpos[0] = tanks[MyTank].Tr.pos.x;
				pt.Tpos[1] = tanks[MyTank].Tr.pos.y;
				pt.Tpos[2] = tanks[MyTank].Tr.pos.z;

				pt.Trot[0] = tanks[MyTank].Tr.rot.x;
				pt.Trot[1] = tanks[MyTank].Tr.rot.y;
				pt.Trot[2] = tanks[MyTank].Tr.rot.z;

				pt.Tscale[0] = tanks[MyTank].Tr.scale.x;
				pt.Tscale[1] = tanks[MyTank].Tr.scale.y;
				pt.Tscale[2] = tanks[MyTank].Tr.scale.z;

				for (int k = 0; k < 4; k++)
				{
					pt.Tmat[k][0] = tanks[MyTank].Tr.mat[k].x;
					pt.Tmat[k][1] = tanks[MyTank].Tr.mat[k].y;
					pt.Tmat[k][2] = tanks[MyTank].Tr.mat[k].z;
					pt.Tmat[k][3] = tanks[MyTank].Tr.mat[k].w;
				}

				send(Connection, (char*)&pt, sizeof(pt), NULL);
				//send(Connection, (char*)&tanks[MyTank].forward, sizeof tanks[MyTank].forward, NULL);
				//send(Connection, (char*)&tanks[MyTank].PoS, sizeof tanks[MyTank].PoS, NULL);
				//send(Connection, (char*)&tanks[MyTank].PoSF, sizeof tanks[MyTank].PoSF, NULL);

			}
			else if ((std::string)msgType == "Vtanks")
			{
				//std::cout << "~~~~~~~~~~~~~~" << std::endl;
				recv(Connection, (char*)&TotalConnections, sizeof(TotalConnections), NULL);


				for (int i = 0; i < TotalConnections; i++)
				{
					playerT pt;
					recv(Connection, (char*)&pt, sizeof(pt), NULL);
					//std::cout << pt.pos[0] << "\t" << pt.pos[1] << "\t" << pt.pos[2] << "\t" << std::endl;
					if (i != MyTank)
					{
						tanks[i].pos = glm::vec3(pt.pos[0], pt.pos[1], pt.pos[2]);

						tanks[i].forward = glm::vec3(pt.forward[0], pt.forward[1], pt.forward[2]);
						tanks[i].up = glm::vec3(pt.up[0], pt.up[1], pt.up[2]);

						tanks[i].PoS = glm::vec3(pt.Pos[0], pt.Pos[1], pt.Pos[2]);
						tanks[i].PoSF = glm::vec3(pt.PosF[0], pt.PosF[1], pt.PosF[2]);

						tanks[i].yeriya = pt.yeriya;


						tanks[i].Tr.pos = glm::vec3(pt.Tpos[0], pt.Tpos[1], pt.Tpos[2]);
						tanks[i].Tr.rot = glm::vec3(pt.Trot[0], pt.Trot[1], pt.Trot[2]);
						tanks[i].Tr.scale = glm::vec3(pt.Tscale[0], pt.Tscale[1], pt.Tscale[2]);
						for (int k = 0; k < 4; k++)
						{
							tanks[i].Tr.mat[k].x = pt.Tmat[k][0];
							tanks[i].Tr.mat[k].y = pt.Tmat[k][1];
							tanks[i].Tr.mat[k].z = pt.Tmat[k][2];
							tanks[i].Tr.mat[k].w = pt.Tmat[k][3];
						}
					}
					//recv(Connection, (char*)&tanks[MyTank].forward, sizeof tanks[MyTank].forward, NULL);
					//recv(Connection, (char*)&tanks[MyTank].PoS, sizeof tanks[MyTank].PoS, NULL);
					//recv(Connection, (char*)&tanks[MyTank].PoSF, sizeof tanks[MyTank].PoSF, NULL);
					//recv(Connection, (char*)&tanks[MyTank].Tr, sizeof tanks[MyTank].Tr, NULL);
				}
			}
		}
	}
}

int connectToServer()
{
	std::string hostaddress;
	std::cout << "Enter host address: ";
	std::cin >> hostaddress;
	std::cout << std::endl;
	if (hostaddress == "localhost")
		hostaddress = "127.0.0.1";
	//winsock startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBox(NULL, L"Winsock startup failed", L"error", MB_OK | MB_ICONERROR);
		return 1;
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof addr;
	addr.sin_addr.s_addr = inet_addr(hostaddress.c_str());

	int portN = 25569;
	//std::cout << "enter port number: ";
	//std::cin >> portN;

	addr.sin_port = htons(portN);
	addr.sin_family = AF_INET;

	std::string name;
	std::cout << "enter name: ";
	std::cin >> name;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof addr) != 0)
	{
		MessageBox(NULL, L"Failed connecting", L"error", MB_OK | MB_ICONERROR);
		return 1;
	}

	int namelen = name.length();
	send(Connection, (char*)&namelen, sizeof(int), NULL);
	send(Connection, name.c_str(), namelen, NULL);
	recv(Connection, (char*)&MyTank, sizeof(int), NULL);
	//recv(Connection, (char*)&numT, sizeof(int), NULL);

	std::cout << "connected with name: " << name << " num: " << MyTank << "th" << std::endl;
	for (int i = 0; i < Max; i++)
		tanks.push_back(Tank());

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	return 0;
}





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



int main(int argc, char** argv)
{
	srand((unsigned)NULL(time));

	if (connectToServer() != 0)
	{
		return 1;
		isRunning = false;
		return 0;
	}


	float Yrandom[widthT * heightT + 8];
	for (int i = 0; i < widthT * heightT + 8; i++)
	{
		Yrandom[i] = 2 * (float)rand() / (float)RAND_MAX;
	}
	//Yrandom[0] = 0;

	//system("pause");


	for (int i = 0; i < widthT; i++)
	{
		for (int j = 0; j < heightT; j++)
		{
			fl[i * heightT + j].GetPos()->x = j * sizetri;
			fl[i * heightT + j].GetPos()->z = i * sizetri;
			fl[i * heightT + j].GetPos()->y = 5;//30*f(j, i, 6, 2, Yrandom);

			fl[i * heightT + j].normal = glm::vec3(0, 1, 0);
			fl[i * heightT + j].texCoord = glm::vec2((i + 1) % 2, (j + 1) % 2);
		}
		//std::cout << std::endl;
	}
	//system("pause");
	
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


	std::cout << '#';

	glm::mat3 R = rotatez(1.5708);
	glm::vec3 N;



	SDL_Event e;
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
	glm::vec4 Light = glm::vec4(1.f, 0.f, 0.f, 0.f);



	tanks[MyTank].forward = camera.forward;
	tanks[MyTank].up = camera.up;

	for (int i = 0; i < Max; i++)
		tanks[i] = Tank(textureT, glm::vec3(15 * i, heightMin(15 * i, 15 * i), 15 * i), camera.up, camera.forward);

	glm::mat4 H;
	glLineWidth(2);

	clock_t time1 = std::clock();
	clock_t time2;
	std::cout << '#';
	double allthetime = 0;

	while (isRunning)
	{
		time2 = std::clock();
		float timeDiff1 = time2 - time1;
		float timeDiff2 = timeDiff1 / 16;
		timeDiff1 /= 32;
		allthetime += timeDiff1 / 128;
		tanks[MyTank].up = glm::normalize(tanks[MyTank].up);
		tanks[MyTank].forward = glm::normalize(tanks[MyTank].forward);

		Light.x = cosf(allthetime);
		Light.y = sinf(allthetime);


		//std::cout << Light.x << " " << Light.y << std::endl << timeDiff1 << std::endl << MyTank << std::endl;
		//	tanks[MyTank].a -= glm::vec3(timeDiff1, timeDiff1, timeDiff1)*1000.f;
		//	(tanks[MyTank].a.x > 0 ? tanks[MyTank].a.x -= timeDiff1/10 : 1);
		//	(tanks[MyTank].a.y > 0 ? tanks[MyTank].a.y -= timeDiff1/10 : 1);
		//	(tanks[MyTank].a.z > 0 ? tanks[MyTank].a.z -= timeDiff1/10 : 1);
		//	(tanks[MyTank].a.x < 0 ? tanks[MyTank].a.x = 0 : 1);
		//	(tanks[MyTank].a.y < 0 ? tanks[MyTank].a.y = 0 : 1);
		//	(tanks[MyTank].a.z < 0 ? tanks[MyTank].a.z = 0 : 1);




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
			//	if (tanks[MyTank].a.x + tanks[MyTank].a.y + tanks[MyTank].a.z < timeDiff1/6)
			//		tanks[MyTank].a += glm::vec3(tanks[MyTank].forward);
			//
			//
			//	camera.pos += tanks[MyTank].a;
			//	tanks[MyTank].pos += tanks[MyTank].a;

			camera.MoveForward((goup * timeDiff1));
			tanks[MyTank].MoveForward((goup * timeDiff1));
		}


		if (inp.KeyDown(SDL_SCANCODE_LEFT) && inp.KeyDown(SDL_SCANCODE_LSHIFT))
		{
			camera.RotateY((0.01 * timeDiff2) * timerotsome);
			rotM += (0.01 * timeDiff2) * timerotsome;
		}
		else if (inp.KeyDown(SDL_SCANCODE_LEFT))
		{
			Y = tanks[MyTank].rotRight((0.01 * timeDiff2) * timerotsome);
			tanks[MyTank].Tr.mat *= Y;
			tanks[MyTank].forward = glm::vec3(glm::normalize(tanks[MyTank].rotRight((0.01 * timeDiff2) * timerotsome) * glm::vec4(tanks[MyTank].forward, 0.0)));
		}

		if (inp.KeyDown(SDL_SCANCODE_DOWN))
		{
			camera.MoveForward(godown * timeDiff1);
			tanks[MyTank].MoveForward(godown * timeDiff1);
		}
		if (inp.KeyDown(SDL_SCANCODE_RIGHT) && inp.KeyDown(SDL_SCANCODE_LSHIFT))
		{
			camera.RotateY((-0.01 * timeDiff2) * timerotsome);
			rotM -= (0.01 * timeDiff2) * timerotsome;
		}
		else if (inp.KeyDown(SDL_SCANCODE_RIGHT))
		{
			Y = tanks[MyTank].rotRight((-0.01 * timeDiff2) * timerotsome);
			tanks[MyTank].Tr.mat *= Y;

			tanks[MyTank].forward = glm::vec3(glm::normalize(tanks[MyTank].rotRight((-0.01 * timeDiff2) * timerotsome) * glm::vec4(tanks[MyTank].forward, 0.0)));
		}


		camera.forward = glm::normalize(glm::vec3(tanks[MyTank].forward.x, /*camera.forward.y*/0, tanks[MyTank].forward.z));

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


		if (inp.KeyDown(SDL_SCANCODE_SPACE) && counter >= 0.5 * timeDiff1)
		{
			tanks[MyTank].yeriya = tanks[MyTank].timeY + 2;
			counter = 0;

			/*	for (int j = 0; j < tanks.size(); j++)
				{
					tanks[j].yeriya = tanks[j].timeY + 2;

				}
				std::cout << std::endl;*/

		}

		if (inp.KeyDown(SDL_SCANCODE_W))
		{
			Z = tanks[MyTank].rotZ(0.01 * timeDiff2);
			tanks[MyTank].Tr.mat *= Z;
			tanks[MyTank].forward = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].forward, 0.0)));
			tanks[MyTank].up = glm::vec3(glm::normalize(Z * glm::vec4(tanks[MyTank].up, 0.0)));

			rotP += 0.01 * timeDiff2;
		}

		if (inp.KeyDown(SDL_SCANCODE_S))
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
		if (inp.KeyDown(SDL_SCANCODE_P))
		{
			tanks[MyTank].forward = camera.forward;

			tanks[MyTank].up = camera.up;
			rotP = 0;
			tanks[MyTank].Tr.mat = tanks[MyTank].rotRight(rotM);

			camU = 4;
			camF = 9;
		}

		float xplus = inp.getMouseXNormalized();
		float yplus = inp.getDeltaMouseY();
		upy = yplus;

		if (inp.KeyDown(SDL_SCANCODE_N))
		{
			C += 0.001 * timeDiff2;
			//std::cout << C << std::endl;
		}
		if (inp.KeyDown(SDL_SCANCODE_B))
		{
			C -= 0.001*timeDiff2;
			//std::cout << C << std::endl;
		}

		if (inp.KeyDown(SDL_SCANCODE_G))
		{
			tanks[MyTank].life -= 50 * timeDiff2;

			D += 0.02 * timeDiff2;
			//std::cout << D << std::endl;
		}
		if (inp.KeyDown(SDL_SCANCODE_H))
		{
			tanks[MyTank].life += 50 * timeDiff2;

			D -= 0.02 * timeDiff2;
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


		tanks[MyTank].PoS = tanks[MyTank].pos - (tanks[MyTank].forward * 1.f) + glm::vec3(tanks[MyTank].up.x, 0.89 * tanks[MyTank].up.y, tanks[MyTank].up.z);
		tanks[MyTank].PoSF = tanks[MyTank].PoS;

		for (int w = 0; w < 70; w++)
		{
			if (tanks[MyTank].PoS.y <= heightMin(tanks[MyTank].PoS.x, tanks[MyTank].PoS.z) - 1)
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
				tanks[i].Tr.SetPos(tanks[i].pos); //////////////////////////////////////////////////////should change
				shader.Update(tanks[i].Tr, camera);

				tank.Draw();


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

		shader.Update(transform2, camera);


		texture3.Bind();
		grand.Draw();



		glm::vec3 aq = tanks[MyTank].right() * 0.95f;
		float life = ((float)(tanks[MyTank].life - 500) / 1000) * 2;
		float li = (counter - 0.25) * 4;
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

		yellow.Bind();

		glBegin(GL_LINES);
		glVertex3f(tanks[MyTank].pos.x + aq.x, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z + aq.z);
		glVertex3f(tanks[MyTank].pos.x - aq.x * li, tanks[MyTank].pos.y + 1.5, tanks[MyTank].pos.z - aq.z * li);
		glEnd();

		red.Bind();

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
			}
		}

		aq *= 0.8;

		glm::vec3 AAA1 = aq + 1.5f * tanks[MyTank].forward;
		glm::vec3 AAA2 = aq - tanks[MyTank].forward;
		glm::vec3 AAA3 = aq + tanks[MyTank].forward;
		glm::vec3 AAA4 = aq - 1.5f * tanks[MyTank].forward;


		float r1 =/*tanks[MyTank].pos.y - AAA1.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x - AAA1.x, tanks[MyTank].pos.z - AAA1.z, tanks);
		float r2 =/*tanks[MyTank].pos.y - AAA2.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x - AAA2.x, tanks[MyTank].pos.z - AAA2.z, tanks);
		float r3 =/*tanks[MyTank].pos.y + AAA3.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x + AAA3.x, tanks[MyTank].pos.z + AAA3.z, tanks);
		float r4 =/*tanks[MyTank].pos.y + AAA4.y;/*/ height_min_with_tanks(tanks[MyTank].pos.x + AAA4.x, tanks[MyTank].pos.z + AAA4.z, tanks);

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


		counter2 += 0.005;

		display.SwapBuffers();
		counter += 0.01f;
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