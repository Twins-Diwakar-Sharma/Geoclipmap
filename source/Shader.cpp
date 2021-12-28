#include "Shader.h"

Shader::Shader(std::string name): Shader(name, NOTHING)
{
}

Shader::Shader(std::string name, int eliteLevel)
{

	std::string vertexFile(path + name + ".vs");
	std::string fragmentFile(path + name + ".fs");
	programID = glCreateProgram();
	createShader(vertexID, GL_VERTEX_SHADER, vertexFile);
	createShader(fragmentID, GL_FRAGMENT_SHADER, fragmentFile);

	unsigned int geometryID=-1, tessComputeID=-1, tessEvalID=-1;

	if (eliteLevel == GEO)
	{
		std::string geometryFile(path + name + ".gs");
		createShader(geometryID, GL_GEOMETRY_SHADER, geometryFile);
		glAttachShader(programID, geometryID);
	}
	else if (eliteLevel == TESS)
	{
		std::string tessComputeFile(path + name + ".tcs");
		std::string tessEvalFile(path + name + ".tes");
		createShader(tessComputeID, GL_TESS_CONTROL_SHADER, tessComputeFile);
		createShader(tessEvalID, GL_TESS_EVALUATION_SHADER, tessEvalFile);
		glAttachShader(programID, tessComputeID);
		glAttachShader(programID, tessEvalID);
	}
	else if (eliteLevel == GEOTESS)
	{
		std::string tessComputeFile(path + name + ".tcs");
		std::string tessEvalFile(path + name + ".tes");
		std::string geometryFile(path + name + ".gs");
		createShader(geometryID, GL_GEOMETRY_SHADER, geometryFile);
		createShader(tessComputeID, GL_TESS_CONTROL_SHADER, tessComputeFile);
		createShader(tessEvalID, GL_TESS_EVALUATION_SHADER, tessEvalFile);
		glAttachShader(programID, tessComputeID);
		glAttachShader(programID, tessEvalID);
		glAttachShader(programID, geometryID);
	}
		

	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	glLinkProgram(programID);

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	
	if (eliteLevel == GEO)
	{
		glDeleteShader(geometryID);
	}
	else if (eliteLevel == TESS)
	{
		glDeleteShader(tessComputeID);
		glDeleteShader(tessEvalID);
	}
	else if (eliteLevel == GEOTESS)
	{
		glDeleteShader(tessComputeID);
		glDeleteShader(tessEvalID);
		glDeleteShader(geometryID);
	}


}

void Shader::createShader(unsigned int& shaderID, int shaderType, std::string filename)
{
	std::ifstream file(filename);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string shaderCode = buffer.str();
	buffer.clear();
	file.close();

	shaderID = glCreateShader(shaderType);
	const char* cshaderCode = shaderCode.c_str();
	glShaderSource(shaderID, 1, &cshaderCode, NULL);
	glCompileShader(shaderID);

	int success;
	char infoLog[256];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 256, NULL, infoLog);
		std::cerr << "Failed to compile " << filename << " \n " << infoLog << std::endl;
	}

}

void Shader::use()
{
	glUseProgram(programID);
}

void Shader::unuse()
{
	glUseProgram(0);
}

void Shader::mapUniform(std::string uniformName)
{
	const char* cUniform = uniformName.c_str();
	int uniformLoc = glGetUniformLocation(programID, cUniform);
	uniformMap.insert(std::pair <std::string,int> (uniformName, uniformLoc));
}

void Shader::mapUniformOLOD(std::string uniform)
{
	std::string uniformL = uniform + "_N";
	const char* cUniformLoc = uniformL.c_str();
	int loc = glGetUniformLocation(programID, cUniformLoc);
	uniformMap.insert(std::pair <std::string, int>(uniformL, loc));

	uniformL = uniform + "_E";
	cUniformLoc = uniformL.c_str();
	loc = glGetUniformLocation(programID, cUniformLoc);
	uniformMap.insert(std::pair <std::string, int>(uniformL, loc));

	uniformL = uniform + "_S";
	cUniformLoc = uniformL.c_str();
	loc = glGetUniformLocation(programID, cUniformLoc);
	uniformMap.insert(std::pair <std::string, int>(uniformL, loc));

	uniformL = uniform + "_W";
	cUniformLoc = uniformL.c_str();
	loc = glGetUniformLocation(programID, cUniformLoc);
	uniformMap.insert(std::pair <std::string, int>(uniformL, loc));
}

void Shader::setUniform(std::string uniformName, float x, float y)
{
	int loc = uniformMap[uniformName];
	glUniform2f(loc, x, y);
}

void Shader::setUniform(std::string uniformName, int value)
{
	int uniformLoc = uniformMap[uniformName];
	glUniform1i(uniformLoc, value);
}

void Shader::setUniform(std::string name, Vec3& v)
{
	int loc = uniformMap[name];
	glUniform3f(loc, v[0], v[1], v[2]);
}

void Shader::setUniform(std::string name, Vec3&& v)
{
	int loc = uniformMap[name];
	glUniform3f(loc, v[0], v[1], v[2]);

}

void Shader::setUniform(std::string name, Mat4& m)
{
	int loc = uniformMap[name];
	float mArray[] = { m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3],
					 m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2], m[3][3] };
	glUniformMatrix4fv(loc, 1, false, mArray);
}

void Shader::setUniform(std::string name, Camera& cam)
{
	std::string ori = name + ".ori";
	std::string pos = name + ".pos";

	int locOri = uniformMap[ori];
	int locPos = uniformMap[pos];

	glUniform4f(locOri, cam.spin[0], cam.spin[1], cam.spin[2], cam.spin[3]);
	glUniform3f(locPos, cam.position[0], cam.position[1], cam.position[2]);
	
}

void Shader::setUniformOLOD(std::string uniformName, int n, int e, int s, int w)
{
	int uniformLoc = uniformMap[uniformName+"_N"];
	glUniform1i(uniformLoc, n);

	uniformLoc = uniformMap[uniformName + "_E"];
	glUniform1i(uniformLoc, e);

	uniformLoc = uniformMap[uniformName + "_S"];
	glUniform1i(uniformLoc, s);

	uniformLoc = uniformMap[uniformName + "_W"];
	glUniform1i(uniformLoc, w);
}
