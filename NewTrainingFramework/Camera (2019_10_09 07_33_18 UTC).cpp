#include "stdafx.h"
#include "Camera.h"
#include <iostream>
Camera::Camera()
{
}

Camera::Camera(GLint cam_id, GLfloat z_near_, GLfloat z_far_, GLfloat fov_, GLfloat aspect_ratio_, Vector3 pos, Vector3 target, Vector3 camera_up, GLfloat translationSpeed, GLfloat rotationSpeed)
{
	Init(cam_id, z_near_, z_far_, fov_, aspect_ratio_, pos, target, camera_up, translationSpeed, rotationSpeed);
}

void Camera::moveOx(GLfloat delta)
{
	Vector3 d = xAxis * currentSpeed * delta * deltatime;
	position += d;
	updateWorldView();
}

void Camera::moveOy(GLfloat delta)
{
	Vector3 d = Vector3(0.0, 1.0, 0.0) * currentSpeed * delta * deltatime;
	position += d;
	updateWorldView();
}

void Camera::moveOz(GLfloat delta)
{
	Vector3 d = -zAxis * currentSpeed* delta* deltatime;
	position += d;
	updateWorldView();
}

void Camera::rotateOx(GLfloat alpha)
{
	Matrix R;
	Vector4 newForward, newUp;

	R.SetRotationAngleAxis(alpha * rotateSpeed, right.x, right.y, right.z);
	newForward = (R * Vector4(forward, 0.0f)).Normalize();
	forward = Vector3(newForward.x, newForward.y, newForward.z);

	up = -forward.Cross(right).Normalize();
	updateWorldView();
}

void Camera::rotateOy(GLfloat alpha)
{
	Matrix R;
	Vector4 newRight, newForward;

	R.SetRotationY(alpha * rotateSpeed);

	newForward = (R * Vector4(forward, 0.0f)).Normalize();
	forward = Vector3(newForward.x, newForward.y, newForward.z);

	newRight = (R * Vector4(right, 0.0f)).Normalize();
	right = Vector3(newRight.x, newRight.y, newRight.z);

	up = -forward.Cross(right).Normalize();
	updateWorldView();
}

void Camera::rotateOz(GLfloat alpha)
{
	Matrix R;
	Vector4 newright;

	R.SetRotationAngleAxis(alpha * rotateSpeed, forward.x, forward.y, forward.z);
	newright = (R * Vector4(right, 0.0)).Normalize();
	right = Vector3(&newright.x);
	up = -forward.Cross(right).Normalize();
	updateWorldView();
}

void Camera::updateWorldView()
{
	zAxis = -forward.Normalize();
	yAxis = up.Normalize();
	xAxis = -zAxis.Cross(yAxis).Normalize();

	Matrix R, T;
	R[0][0] = xAxis.x; R[0][1] = xAxis.y; R[0][2] = xAxis.z; R[0][3] = 0;
	R[1][0] = yAxis.x; R[1][1] = yAxis.y; R[1][2] = yAxis.z; R[1][3] = 0;
	R[2][0] = zAxis.x; R[2][1] = zAxis.y; R[2][2] = zAxis.z; R[2][3] = 0;
	R[3][0] = 0; R[3][1] = 0; R[3][2] = 0; R[3][3] = 1;
	T.SetTranslation(position);

	worldMatrix = R * T;
	viewMatrix = T.SetTranslation(-position) * R.Transpose();
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

void Camera::Init(GLint cam_id, GLfloat z_near_, GLfloat z_far_, GLfloat fov_, GLfloat aspect_ratio_,
	Vector3 pos, Vector3 target, Vector3 camera_up, GLfloat translationSpeed, GLfloat rotationSpeed)
{
	id = cam_id;
	position = pos;
	forward = target;
	up = camera_up;
	moveSpeed = translationSpeed;
	currentSpeed = translationSpeed;
	rotateSpeed = rotationSpeed;
	z_near = z_near_;
	z_far = z_far_;
	fov = fov_;
	aspect_ratio = aspect_ratio_;
	projectionMatrix.SetPerspective(fov, aspect_ratio, z_near, z_far);
	updateWorldView();
}