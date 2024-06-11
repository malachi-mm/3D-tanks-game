#define _WINSOCK_DEPRECATED_NO_WARNINGS




#pragma comment(lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <string>

#include <iostream>
//#include <SDL2/SDL.h>
//#undef main

#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "Model.h"
#include <vector>
#include <queue>
#include <ctime>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include<glm/gtx/vector_angle.hpp>

#include <thread>

#include <stdlib.h>
#include <stdio.h>

#include <math.h>



#define sizetri 1
#define widthT (64*5)
#define heightT (64*5)

#define Max 20

#define TimeD 100

static SOCKET Connection;
static bool isRunning = true;
static int TotalConnections = 1;
static std::string username = "";
static std::string serverIP = "";

#define NEXT_WINDOW 0
#define START_WINDOW 1
#define GAME_WINDOW 2
#define FIND_SERVER_WINDOW 3
#define WAIT 10
static int numWindow = 1;
static int MyTank = 0;
static float myPos[] = { 10,5,10 };

struct Bullet
{
	glm::vec3 pos;
	glm::vec3 dir;
	float restTime = 0;


	void Bullet::operator=(float f)
	{
		restTime=f;
	}
};

struct Tank
{
	Transform Tr;
	Model* Mo;
	glm::vec3 pos = { 0,0,0 };
	glm::vec3 up;
	glm::vec3 forward;
	int life = 1000;
	//glm::vec3 a = glm::vec3(0, 0, 0);

	glm::vec3 PoS = glm::vec3(0, 0, 0);
	glm::vec3 PoSF = glm::vec3(0, 0, 0);
	//Bullet yeriya;
	float yeriya = 0;
	const int timeY = 250;

	Tank() {	}


	Tank(Model& mo, glm::vec3 POS, glm::vec3 UP, glm::vec3 FORW)
	{
		Mo = &mo;
		pos = POS;
		up = UP;
		forward = FORW;
	}

	void Tank::operator=(const Tank& t)
	{
		Mo = t.Mo;
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
static std::vector<Tank> tanks;


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


struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

static std::map<char, Character> Characters;