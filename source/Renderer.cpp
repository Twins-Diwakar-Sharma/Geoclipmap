#include "Renderer.h"

Mat4 projection;

void setProjection(float fov, float near, float far, float as)
{
	float radFov = fov * PI / (180.0f);

	float d = 1.00f / (std::tan(radFov / 2.0f));
	float A = (near + far) / (near - far);
	float B = (2 * near * far) / (near - far);

	projection[0][0] = d / as;
	projection[1][1] = d;
	projection[2][2] = A;		projection[3][2] = B;
	projection[2][3] = -1;
}

Renderer::Renderer()
{
	genericShader = new Shader("objects");

	genericShader->mapUniform("transform");
	genericShader->mapUniform("cam.ori");
	genericShader->mapUniform("cam.pos");
	genericShader->mapUniform("projection");

	genericShader->mapUniform("albedo");
	genericShader->mapUniform("lightDir");
	genericShader->mapUniform("lightColor");
}

Renderer::~Renderer()
{
	delete genericShader;
}

void Renderer::render(Object& o, Camera& cam, Vec3& lightColor, Vec3& lightDir)
{
	genericShader->use();
	genericShader->setUniform("albedo", 0);
	genericShader->setUniform("projection", projection);
	genericShader->setUniform("cam", cam);
	genericShader->setUniform("lightDir", lightDir);
	genericShader->setUniform("lightColor", lightColor);
	o.bind();
	genericShader->setUniform("transform", o.getTransform());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, o.getTexID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glDrawElements(GL_TRIANGLES, o.meshIndSize(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	o.unbind();
	genericShader->unuse();
}

ClipmapRenderer::ClipmapRenderer()
{
	shader = new Shader("clipmap",TESS);

	shader->mapUniform("center");
	// in future, we also need position of heightmap on worldspace, subtract it from center to get relative position on world space

	shader->mapUniform("cam.ori");
	shader->mapUniform("cam.pos");
	shader->mapUniform("projection");
	
	shader->mapUniform("lightDir");
	shader->mapUniform("lightColor");

	shader->mapUniform("heightmap");
	shader->mapUniform("normalmap");

	shader->mapUniform("terrainSize");


}

ClipmapRenderer::~ClipmapRenderer()
{
	delete shader;
}

void ClipmapRenderer::render(Clipmap& clipmap, Texture& heightMap, Texture& normalMap, Camera& camera, Vec3& lightColor, Vec3& lightDir)
{
	shader->use();

	glPatchParameteri(GL_PATCH_VERTICES, 4);
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	shader->setUniform("projection", projection);
	shader->setUniform("cam", camera);
	shader->setUniform("lightDir", lightDir);
	shader->setUniform("lightColor", lightColor);
	shader->setUniform("heightmap", 0);
	shader->setUniform("normalmap", 1);
	shader->setUniform("terrainSize", 512); // do this other way plz
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightMap.getID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap.getID());

	clipmap.bindMesh();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	shader->setUniform("center", clipmap.getPosition());
	
	glDrawArrays(GL_PATCHES, 0, clipmap.getMeshSize());

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	clipmap.unbindMesh();
	shader->unuse();

//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}


void ClipmapRenderer::render(GeoClipmap& geo, Texture& heightMap, Texture& normalMap, Camera& camera, Vec3& lightColor, Vec3& lightDir)
{
	shader->use();

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	shader->setUniform("projection", projection);
	shader->setUniform("cam", camera);
	shader->setUniform("lightDir", lightDir);
	shader->setUniform("lightColor", lightColor);
	shader->setUniform("heightmap", 0);
	shader->setUniform("normalmap", 1);
	shader->setUniform("terrainSize", 512); // do this other way plz
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightMap.getID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap.getID());

	for(int i=0; i<geo.getNoOfRings()+1; i++)
	{
		geo.mipmaps[i]->bindMesh();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		shader->setUniform("center", geo.mipmaps[i]->getPosition());
		
		glDrawArrays(GL_PATCHES, 0, geo.mipmaps[i]->getMeshSize());
 
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		geo.mipmaps[i]->unbindMesh();
	}

	shader->unuse();
}

