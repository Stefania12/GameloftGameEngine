#pragma once
#include "../Utilities/math.h"
#include "../Utilities/esUtil.h"
class Camera
{
public:
	GLint id = 1;
	Vector3 position = {0 , 0 , 3};
	Vector3 forward = {0, 0, -1};
	Vector3 up = { 0, 1, 0 };
	Vector3 right = { 1, 0, 0 };
	GLfloat moveSpeed = 5;
	GLfloat currentSpeed = 5;
	GLfloat rotateSpeed = 0.005f;
	GLfloat rotateSpeedOz = 0.5f;
	GLfloat z_near;
	GLfloat z_far;
	GLfloat fov;
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;
	Matrix viewMatrix;
	Matrix worldMatrix;
	Matrix projectionMatrix;
	Matrix viewProjectionMatrix;
	GLfloat aspect_ratio;
	GLfloat deltatime;
	void moveOx(GLfloat);
	void moveOy(GLfloat);
	void moveOz(GLfloat);
	void rotateOx(GLfloat);
	void rotateOy(GLfloat);
	void rotateOz(GLfloat);
	void updateWorldView();
	Camera();
	Camera(GLint cam_id, GLfloat z_near_, GLfloat z_far_, GLfloat fov_, GLfloat aspect_ratio_, Vector3 pos, Vector3 target, Vector3 camera_up, GLfloat translationSpeed, GLfloat rotationSpeed);
	void Init(GLint cam_id, GLfloat z_near_, GLfloat z_far_, GLfloat fov_, GLfloat aspect_ratio_, Vector3 pos, Vector3 target, Vector3 camera_up, GLfloat translationSpeed, GLfloat rotationSpeed);
};