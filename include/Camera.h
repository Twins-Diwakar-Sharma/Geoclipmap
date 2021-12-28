#ifndef CAM_ERA
#define CAM_ERA

#include "Algebra.h"

class Camera
{
private:
	Vec3 rotation;
//	Quat spin;
	void syncView();
	

public:

	Quat spin;
	Vec3 position;
	Mat4 view;
	Camera();
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void translate(float forward, float side);
	void rotate(float x, float y, float z);
	void spinIt(float x, float y, float z);
	void move(float forward, float strafe);
//	void getSpin();

};
#endif