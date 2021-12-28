#include "Mesh.h"



void Mesh::generate()
{
	std::cout << "mesh generate " << std::endl;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);

}

Mesh::Mesh()
{
}

Mesh::Mesh(std::string name)
{
	std::string path = "Inventory/Models/" + name + ".stc";
	std::ifstream ifs(path);
	std::string line;
	
	std::getline(ifs, line);
	std::istringstream iss(line);
	float f;
	while (iss >> f)
		vertexData.push_back(f);

	std::getline(ifs, line);
	std::istringstream iss2(line);
	unsigned int ui;
	while (iss2 >> ui)
		indices.push_back(ui);

	generate();
}

Mesh::~Mesh()
{
}

void Mesh::createPlane()
{
	vertexData = {
		// positions          // texture	  // normals
		 0.5f,  0.5f, 0.0f,	  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,// top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,// bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,// bottom left
		-0.5f, 0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,// top left
	};

	indices = { 0, 1, 3, 0, 3, 2 };

}

void Mesh::bind()
{
	glBindVertexArray(vao);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}

int Mesh::indicesSize()
{
	return indices.size();
}


void ClipMesh::generate()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	components = 4;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, components * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, components * sizeof(float), (void*)(2*sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 
	glBindVertexArray(0);
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	std::cout << "generate done" << std::endl;
}

void ClipMesh::createClipMesh(int noOfRings)
{
	float square[] = {
		1.0f,  1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f, 1.0f,
	};


	float size = startSize;
	float pos[4][2];

	pos[0][0] = -1; pos[0][1] =  1; 
	pos[1][0] =  1; pos[1][1] =  1; 
	pos[2][0] = -1; pos[2][1] = -1; 
	pos[3][0] =  1; pos[3][1] = -1; 

	

	float ringPos[12][2];

	ringPos[0][0] = -1.5f; ringPos[0][1] = -1.5f; 		
	ringPos[1][0] = -0.5f; ringPos[1][1] = -1.5f; 		
	ringPos[2][0] = 0.5f; ringPos[2][1] = -1.5f;  		
	ringPos[3][0] = 1.5f; ringPos[3][1] = -1.5f;  		

	ringPos[4][0] = 1.5f; ringPos[4][1] = -0.5f;  		
	ringPos[5][0] = 1.5f; ringPos[5][1] = 0.5f;	  		

	ringPos[6][0] = 1.5f; ringPos[6][1] = 1.5f;   		
	ringPos[7][0] = 0.5f; ringPos[7][1] = 1.5f;	  			
	ringPos[8][0] = -0.5f; ringPos[8][1] = 1.5f;  		
	ringPos[9][0] = -1.5f; ringPos[9][1] = 1.5f;   		 

	ringPos[10][0] = -1.5f; ringPos[10][1] = 0.5f;  	 
	ringPos[11][0] = -1.5f; ringPos[11][1] = -0.5f;  	
	
	// base 4 quads
	for(int j=0; j<4; j++)
	{
		for(int k=0; k<4; k++) // for every vertex of square
		{
			float x=0,y=0;
			// scale
			x = square[k*2] * (size/2);
			y = square[k*2 + 1] * (size/2);
			// translate
			x += pos[j][0] * (size/2);
			y += pos[j][1] * (size/2);

			vertexData.push_back(x);
			vertexData.push_back(y); 

			vertexData.push_back(pos[j][0]);
			vertexData.push_back(pos[j][1]);
		}

	}

	// rest of the rings
	for(int i=0; i<noOfRings; i++)
	{
		for(int j=0; j<12; j++)
		{
			for(int k=0; k<4; k++) // for every vertex of square
			{
				float x=0,y=0;
				// scale
				x = square[k*2] * (size/2);
				y = square[k*2 + 1] * (size/2);
				// translate
				x += ringPos[j][0] * size;
				y += ringPos[j][1] * size;

				vertexData.push_back(x);
				vertexData.push_back(y); 

				vertexData.push_back((int)ringPos[j][0]);
				vertexData.push_back((int)ringPos[j][1]);
			}

		}
		size = size * 2;

	}

}

ClipMesh::ClipMesh(int rings)
{
	createClipMesh(rings);
	generate();
}

ClipMesh::ClipMesh()
{

}

ClipMesh::~ClipMesh()
{
}

int ClipMesh::getStartSize()
{
	return startSize;
}

int ClipMesh::getSize()
{
	vertexData.size()/components;
}

std::vector<float>& ClipMesh::getVertexData()
{
	return vertexData;
}
