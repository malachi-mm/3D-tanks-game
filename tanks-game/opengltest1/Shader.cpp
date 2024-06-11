//#include "shader.h"
//#include <iostream>
//#include <fstream>
//
//Shader::Shader(const std::string& fileName)
//{
//	m_program = glCreateProgram();
//	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
//	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
//
//	for (unsigned int i = 0; i < NUM_SHADERS; i++)
//		glAttachShader(m_program, m_shaders[i]);
//
//	glBindAttribLocation(m_program, 0, "position");
//	glBindAttribLocation(m_program, 1, "texCoord");
//	glBindAttribLocation(m_program, 2, "normal");
//
//	glLinkProgram(m_program);
//	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");
//
//	glValidateProgram(m_program);
//	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
//
//	m_uniforms[0] = glGetUniformLocation(m_program, "MVP");
//	m_uniforms[1] = glGetUniformLocation(m_program, "Normal");
//	m_uniforms[2] = glGetUniformLocation(m_program, "lightDirection");
//}
//
//Shader::~Shader()
//{
//	for (unsigned int i = 0; i < NUM_SHADERS; i++)
//	{
//		glDetachShader(m_program, m_shaders[i]);
//		glDeleteShader(m_shaders[i]);
//	}
//
//	glDeleteProgram(m_program);
//}
//
//void Shader::Bind()
//{
//	glUseProgram(m_program);
//}
//
//void Shader::Update(const Transform& transform, const Camera& camera)
//{
//	glm::mat4 MVP = transform.GetMVP(camera);
//	glm::mat4 Normal = transform.GetModel();
//
//	glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
//	glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
//	glUniform3f(m_uniforms[2], 10.0f, 100.0f, 1.0f);
//}
//
//void Shader::Update(const float g)
//{
//	int vertexColorLocation = glGetUniformLocation(m_program, "ourColor");
//	glUniform4f(vertexColorLocation, 0.0f, g, 0.0f, 1.0f);
//}
//
//std::string Shader::LoadShader(const std::string& fileName)
//{
//	std::ifstream file;
//	file.open((fileName).c_str());
//
//	std::string output;
//	std::string line;
//
//	if (file.is_open())
//	{
//		while (file.good())
//		{
//			getline(file, line);
//			output.append(line + "\n");
//		}
//	}
//	else
//	{
//		std::cerr << "Unable to load shader: " << fileName << std::endl;
//	}
//
//	return output;
//}
//
//void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
//{
//	GLint success = 0;
//	GLchar error[1024] = { 0 };
//
//	if (isProgram)
//		glGetProgramiv(shader, flag, &success);
//	else
//		glGetShaderiv(shader, flag, &success);
//
//	if (success == GL_FALSE)
//	{
//		if (isProgram)
//			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
//		else
//			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
//
//		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
//	}
//}
//
//GLuint Shader::CreateShader(const std::string& text, unsigned int type)
//{
//	GLuint shader = glCreateShader(type);
//
//	if (shader == 0)
//		std::cerr << "Error compiling shader type " << type << std::endl;
//
//	const GLchar* p[1];
//	p[0] = text.c_str();
//	GLint lengths[1];
//	lengths[0] = text.length();
//
//	glShaderSource(shader, 1, p, lengths);
//	glCompileShader(shader);
//
//	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");
//
//	return shader;
//}
//
//
//
//
//
//
//Shader2::Shader2(const std::string& fileName)
//{
//	m_program = glCreateProgram();
//	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
//	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
//
//	for (unsigned int i = 0; i < NUM_SHADERS; i++)
//		glAttachShader(m_program, m_shaders[i]);
//
//	glLinkProgram(m_program);
//	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");
//
//	glValidateProgram(m_program);
//	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");
//
//	//m_uniforms[0] = glGetUniformLocation(m_program, "aPos");
//	//m_uniforms[1] = glGetUniformLocation(m_program, "aColor");
//}
//
//Shader2::~Shader2()
//{
//	for (unsigned int i = 0; i < NUM_SHADERS; i++)
//	{
//		glDetachShader(m_program, m_shaders[i]);
//		glDeleteShader(m_shaders[i]);
//	}
//
//	glDeleteProgram(m_program);
//}
//
//void Shader2::Bind()
//{
//	glUseProgram(m_program);
//}
//
//std::string Shader2::LoadShader(const std::string& fileName)
//{
//	std::ifstream file;
//	file.open((fileName).c_str());
//
//	std::string output;
//	std::string line;
//
//	if (file.is_open())
//	{
//		while (file.good())
//		{
//			getline(file, line);
//			output.append(line + "\n");
//		}
//	}
//	else
//	{
//		std::cerr << "Unable to load shader: " << fileName << std::endl;
//	}
//
//	return output;
//}
//
//void Shader2::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
//{
//	GLint success = 0;
//	GLchar error[1024] = { 0 };
//
//	if (isProgram)
//		glGetProgramiv(shader, flag, &success);
//	else
//		glGetShaderiv(shader, flag, &success);
//
//	if (success == GL_FALSE)
//	{
//		if (isProgram)
//			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
//		else
//			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
//
//		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
//	}
//}
//
//GLuint Shader2::CreateShader(const std::string& text, unsigned int type)
//{
//	GLuint shader = glCreateShader(type);
//
//	if (shader == 0)
//		std::cerr << "Error compiling shader type " << type << std::endl;
//
//	const GLchar* p[1];
//	p[0] = text.c_str();
//	GLint lengths[1];
//	lengths[0] = text.length();
//
//	glShaderSource(shader, 1, p, lengths);
//	glCompileShader(shader);
//
//	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");
//
//	return shader;
//}
//
//
////###########################################################################################
//
//
//
//Shader3::Shader3(const char* vertexPath, const char* fragmentPath)
//{
//	// 1. retrieve the vertex/fragment source code from filePath
//	std::string vertexCode;
//	std::string fragmentCode;
//	std::ifstream vShaderFile;
//	std::ifstream fShaderFile;
//	// ensure ifstream objects can throw exceptions:
//	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	try
//	{
//		// open files
//		vShaderFile.open(vertexPath);
//		fShaderFile.open(fragmentPath);
//		std::stringstream vShaderStream, fShaderStream;
//		// read file's buffer contents into streams
//		vShaderStream << vShaderFile.rdbuf();
//		fShaderStream << fShaderFile.rdbuf();
//		// close file handlers
//		vShaderFile.close();
//		fShaderFile.close();
//		// convert stream into string
//		vertexCode = vShaderStream.str();
//		fragmentCode = fShaderStream.str();
//	}
//	catch (std::ifstream::failure& e)
//	{
//		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
//	}
//	const char* vShaderCode = vertexCode.c_str();
//	const char* fShaderCode = fragmentCode.c_str();
//	// 2. compile shaders
//	unsigned int vertex, fragment;
//	// vertex shader
//	vertex = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex, 1, &vShaderCode, NULL);
//	glCompileShader(vertex);
//	checkCompileErrors(vertex, "VERTEX");
//	// fragment Shader
//	fragment = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment, 1, &fShaderCode, NULL);
//	glCompileShader(fragment);
//	checkCompileErrors(fragment, "FRAGMENT");
//	// shader Program
//	ID = glCreateProgram();
//	glAttachShader(ID, vertex);
//	glAttachShader(ID, fragment);
//	glLinkProgram(ID);
//	checkCompileErrors(ID, "PROGRAM");
//	// delete the shaders as they're linked into our program now and no longer necessary
//	glDeleteShader(vertex);
//	glDeleteShader(fragment);
//
//}
//
//void Shader3::Bind()
//{
//	glUseProgram(ID);
//}
//
//
//void Shader3::setBool(const std::string& name, bool value) const
//{
//	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
//}
//
//void Shader3::setInt(const std::string& name, int value) const
//{
//	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
//}
//
//void Shader3::setFloat(const std::string& name, float value) const
//{
//	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
//}
//
//
//void Shader3::setVec3(const std::string& name, glm::vec3 value) const
//{
//	glUniform3f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2]);
//}
//
//void Shader3::setMat4(const std::string& name, glm::mat4 mat) const
//{
//	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
//}
