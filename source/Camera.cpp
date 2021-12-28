#include "Camera.h"

void Camera::syncView()
{
	float radx = -rotation[0] * PI / 180.0f, rady = -rotation[1] * PI / 180.0f, radz = -rotation[2] * PI / 180.0f;
	float cx = std::cos(radx), sx = std::sin(radx);
	float cy = std::cos(rady), sy = std::sin(rady);
	float cz = std::cos(radz), sz = std::sin(radz);

	view[0][0] = (cy*cz);			  view[1][0] = (-sz*cy);			view[2][0] = (sy);		view[3][0] = 0;
	view[0][1] = (cx*sz + cz*sy*sx);  view[1][1] = (cz*cx - sx*sz*sy);	view[2][1] = (-cy*sx);	view[3][1] = 0;
	view[0][2] = (sz*sx - cx*cz*sy);  view[1][2] = (cz*sx + sz*sy*cx);	view[2][2] = (cx*cy);	view[3][2] = 0;

	view[3] = (-position[0] * view[0]) + (-position[1] * view[1]) + (-position[2] * view[2]) ;
	view[3][3] = 1;
}

Camera::Camera()
{
	spin[0] = 1;
	spin[1] = 0;
	spin[2] = 0;
	spin[3] = 0;
}

void Camera::setPosition(float x, float y, float z)
{
	position[0] = x; position[1] = y; position[2] = z;
	syncView();
}

void Camera::setRotation(float x, float y, float z)
{
	rotation[0] = x; rotation[1] = y; rotation[2] = z;
	for (int i = 0; i < 3; i++) {
		if (rotation[0] >= 360.0f)
			rotation[0] -= 360.0f;
		if (rotation[0] <= 0.0f)
			rotation[0] += 360.0f;
	}
	syncView();
}

void Camera::translate(float forward, float side)
{
	forward *= 0.5f;	side *= 0.5f;
	float radY = -PI / 180.0f * rotation[1];
	position[0] += forward * std::sin(radY) + side * std::cos(radY);
	position[2] += -forward * std::cos(radY) + side * std::sin(radY);
	syncView();
}

void Camera::rotate(float x, float y, float z)
{
	setRotation(rotation[0] + x,	rotation[1] + y,	rotation[2] + z);
}

void Camera::spinIt(float x, float y, float z)
{
	rotation[0] += x;	rotation[1] += y;	rotation[2] += z;
	float rad = PI / 180.0f;
	float radx = x / 2 * rad, rady = y / 2 * rad, radz = z / 2 * rad;
	float cx = std::cos(radx), sx = std::sin(radx);
	float cy = std::cos(rady), sy = std::sin(rady);
	float cz = std::cos(radz), sz = std::sin(radz);

	spin = Quat(cy, sy * Vec3(0, 1, 0))*spin * Quat(cx, sx * Vec3(1, 0, 0)) * Quat(cz, sz * Vec3(0, 0, 1));
}

void Camera::move(float forward, float strafe)
{
	Quat mv(0.0f, strafe, 0.0f, -forward);
	mv = spin * mv * spin.conjugate();
	position = position + Vec3(mv[1], mv[2], mv[3]);
}
