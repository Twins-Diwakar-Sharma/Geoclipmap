#include "Object.h"

void Object::syncTrans()
{
	float radx = rotation[0] * PI / 180.0f, rady = rotation[1] * PI / 180.0f, radz = rotation[2] * PI / 180.0f;
	float cx = std::cos(radx), sx = std::sin(radx);
	float cy = std::cos(rady), sy = std::sin(rady);
	float cz = std::cos(radz), sz = std::sin(radz);

	transform[0][0] = (cy*cz)*scale[0];					transform[1][0] = (-sz*cy)*scale[1];					transform[2][0] = (sy)*scale[2];			transform[3][0] = position[0];
	transform[0][1] = (cx*sz + cz*sy*sx)*scale[0];		transform[1][1] = (cz*cx - sx*sz*sy)*scale[1];			transform[2][1] = (-cy*sx)*scale[2];		transform[3][1] = position[1];
	transform[0][2] = (sz*sx - cx*cz*sy)*scale[0];		transform[1][2] = (cz*sx + sz*sy*cx)*scale[1];			transform[2][2] = (cx*cy)*scale[2];			transform[3][2] = position[2];
	transform[0][3] = 0;								transform[1][3] = 0;									transform[2][3] = 0;						transform[3][3] = 1;

}

Object::Object(Mesh* m, Texture* t)
{
	mesh = m;
	tex = t;
	setScale(1, 1, 1);
}

void Object::bind()
{
	mesh->bind();
}

void Object::unbind()
{
	mesh->unbind();
}

unsigned int Object::getTexID()
{
	return tex->getID();
}

int Object::meshIndSize()
{
	return mesh->indicesSize();
}

Mat4& Object::getTransform()
{
	return transform;
}

void Object::setPosition(float x, float y, float z)
{
	transform[3][0] = x;
	transform[3][1] = y;
	transform[3][2] = z;
	position[0] = x; position[1] = y; position[2] = z;
}

void Object::setPosition(Vec3& p)
{
	transform[3][0] = p[0];
	transform[3][1] = p[1];
	transform[3][2] = p[2];
	position = p;
}

void Object::setPosition(Vec3&& p)
{
	transform[3][0] = p[0];
	transform[3][1] = p[1];
	transform[3][2] = p[2];
	position = p;

}

void Object::setRotation(float x, float y, float z)
{
	rotation[0] = x; rotation[1] = y; rotation[2] = z;
	syncTrans();
}

void Object::translate(float dx, float dy, float dz)
{
	setPosition(position[0] + dx, position[1] + dy, position[2] + dz);
	//setPosition(position + Vec3(dx, dy, dz));
}

void Object::rotate(float rx, float ry, float rz)
{
	rotation[0] += rx;
	rotation[1] += ry;
	rotation[2] += rz;
	syncTrans();
}

void Object::setScale(float x, float y, float z)
{
	scale[0] = x;	scale[1] = y; scale[2] = z;
	syncTrans();
}


Clipmap::Clipmap()
{
	mesh = new ClipMesh(rings);
}

Clipmap::~Clipmap()
{
	delete mesh;
}

void Clipmap::bindMesh()
{
	mesh->bind();
} 

void Clipmap::unbindMesh()
{
	mesh->unbind();
}

int Clipmap::getNoOfRings()
{
	return rings;
}

Vec3& Clipmap::getPosition()
{
	return position;
}

void Clipmap::update(Vec3& campos)
{
	Vec3 delta = campos - position;
	
	float step = mesh->getStartSize();
	if (delta[0] > step)
	{
		position[0] += step;
	}
	else if (delta[0] < -step)
	{
		position[0] -= step;
	}

	if (delta[2] < -step)
	{
		position[2] -= step;
	}
	else if (delta[2] > step)
	{
		position[2] += step;
	}
}

int Clipmap::getMeshSize()
{
	return mesh->getSize();
}


Mipmap::Mipmap()
{
	mesh = new ClipMesh();
}

Mipmap::~Mipmap()
{
	delete mesh;
}

void Mipmap::bindMesh()
{
	mesh->bind();
} 

void Mipmap::unbindMesh()
{
	mesh->unbind();
}

Vec3& Mipmap::getPosition()
{
	return position;
}

void Mipmap::update(int dirX, int dirZ)
{
	
	//std::cout << stepX << ", " << stepZ << std::endl;
	position[0] += dirX * stepSize;
	position[2] += dirZ * stepSize;

	stepX += dirX;
	stepZ += dirZ;
	//std::cout << stepX << "," << stepZ << std::endl;
	if(stepX >= 2 || stepX <= -2)
	{
		stepX = 0;
		if(parent != nullptr)
		{
			parent->update(dirX,dirZ);
		//	std::cout << "parent" << std::endl;
		}

	}
	if(stepZ >= 2 || stepZ <= -2)
	{
		stepZ = 0;
		if(parent != nullptr)
		{
			parent->update(dirX,dirZ);
		//	std::cout << "parent" << std::endl;
		}
	}
	/*
	if(stepX >= 2 || stepX <= -2)
	{
		int sign = stepX > 0 ? 1 : -1;
		parent->stepX = parent->stepX + sign;
		parent->update();
		stepX = 0;
	}
	if(stepZ >= 2 || stepZ <= -2)
	{
		int sign = stepX > 0 ? 1 : -1;
		parent->stepZ = parent->stepZ + sign;
		parent->update();
		stepZ = 0;
	}
	*/
}

int Mipmap::getMeshSize()
{
	return mesh->getSize();
}

ClipMesh* Mipmap::getMesh()
{
	return mesh;
}

GeoClipmap::GeoClipmap(int rings)
{
	this->rings = rings;
	mipmaps = new Mipmap*[rings+1]; 

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

	int level = 0;
	mipmaps[level] = new Mipmap();
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

			mipmaps[level]->getMesh()->getVertexData().push_back(x);
			mipmaps[level]->getMesh()->getVertexData().push_back(y); 

			mipmaps[level]->getMesh()->getVertexData().push_back(pos[j][0]);
			mipmaps[level]->getMesh()->getVertexData().push_back(pos[j][1]);
		}

	}

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

			mipmaps[level]->getMesh()->getVertexData().push_back(x);
			mipmaps[level]->getMesh()->getVertexData().push_back(y); 

			mipmaps[level]->getMesh()->getVertexData().push_back((int)ringPos[j][0]);
			mipmaps[level]->getMesh()->getVertexData().push_back((int)ringPos[j][1]);
		}

	}
	mipmaps[level]->getMesh()->generate();

	size = size * 2;
	// rest of the rings
	for(int i=0; i<rings; i++)
	{
		level++;
		mipmaps[level] = new Mipmap();
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

				mipmaps[level]->getMesh()->getVertexData().push_back(x);
				mipmaps[level]->getMesh()->getVertexData().push_back(y); 

				mipmaps[level]->getMesh()->getVertexData().push_back((int)ringPos[j][0]);
				mipmaps[level]->getMesh()->getVertexData().push_back((int)ringPos[j][1]);
			}

		}
		mipmaps[level]->parent = mipmaps[level-1];
		mipmaps[level]->getMesh()->generate();
		mipmaps[level]->stepSize = mipmaps[level-1]->stepSize * 2;
		size = size * 2;

	}

	for(int sh = 0; sh<rings; sh++)
	{
		mipmaps[sh]->parent = mipmaps[sh+1];
	}

	mipmaps[rings]->parent = nullptr;
	for(int sh = 0; sh<rings+1; sh++)
	{
		std::cout << mipmaps[sh]->parent << std::endl;
	}


}

GeoClipmap::~GeoClipmap()
{
	for(int i=0; i<rings+1; i++)
		delete mipmaps[i];
	delete [] mipmaps;
}

void GeoClipmap::update(Vec3& campos)
{
	
	Vec3 delta = campos - mipmaps[0]->getPosition();
	float step = 1;
	if (delta[0] > step)
	{
		mipmaps[0]->update(1,0);
	}
	else if (delta[0] < -step)
	{
		mipmaps[0]->update(-1,0);
	}

	if (delta[2] < -step)
	{
		mipmaps[0]->update(0,-1);
	}
	else if (delta[2] > step)
	{
		mipmaps[0]->update(0,1);
	}
}

int GeoClipmap::getNoOfRings()
{
	return rings;
}