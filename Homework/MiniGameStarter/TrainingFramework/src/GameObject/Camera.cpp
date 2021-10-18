#include "Camera.h"


Camera::Camera() : m_id(0), m_is2D(false), m_position(Vector3(0.0f, 0.0f, 1.0f)), m_target(Vector3(0.0f, 0.0f, 0.0f)), m_up(Vector3(0.0f, 1.0f, 0.0f)), m_fNear(0.1f), m_fFar(100.0f), m_fSpeed(10.0f)
{
	m_viewMatrix.SetIdentity();
	m_projectionMatrix.SetIdentity();
	m_rotationMatrix.SetIdentity();

	CalculateWorldMatrix();
	CalculateLookAtMatrix();
}

Camera::Camera(GLint id, Vector3 position, Vector3 target, Vector3 up, GLfloat fovY, GLfloat fNear, GLfloat fFar, GLfloat fSpeed)
	: m_id(id), m_is2D(false), m_position(position), m_target(target), m_up(up), m_fNear(fNear), m_fFar(fFar), m_fSpeed(fSpeed)
{
	CalculateWorldMatrix();
	CalculateViewMatrix();
	m_rotationMatrix.SetIdentity();
	m_projectionMatrix.SetPerspective(1.0f, (GLfloat)Globals::Globals::screenWidth / Globals::Globals::screenHeight, fNear, fFar);

	CalculateLookAtMatrix();
}

Camera::Camera(GLint id, GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat fNear, GLfloat fFar, GLfloat fSpeed)
	: m_id(id), m_is2D(true), m_position(0.0f, 0.0f, 0.0f), m_target(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f), m_fSpeed(fSpeed)
{
	CalculateWorldMatrix();
	CalculateViewMatrix();
	m_rotationMatrix.SetIdentity();
	m_projectionMatrix.SetOrtho(left, right, bottom, top, fNear, fFar);

	CalculateLookAtMatrix();

	glDepthMask(false);
	glDisable(GL_DEPTH_TEST);
}

Camera::~Camera()
{
}

void Camera::SetTarget(Vector3 target)
{
	m_target = target;
}

void Camera::SetPosition(Vector3 pos)
{
	m_position = pos;
}

void Camera::MoveUp(GLfloat deltaTime)
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = m_up.Cross(zaxis).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis).Normalize());
	Vector3 deltaMove = yaxis * deltaTime * m_fSpeed;
	m_position += deltaMove;
	m_target += deltaMove;
}

void Camera::MoveLeft(GLfloat deltaTime)
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = m_up.Cross(zaxis).Normalize();
	Vector3 deltaMove = -xaxis * deltaTime * m_fSpeed;
	m_position += deltaMove;
	m_target += deltaMove;
}

void Camera::SetRotationZ(GLfloat angle)
{
	m_rotationMatrix.SetRotationZ(angle);
}


void Camera::CalculateWorldMatrix()
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = m_up.Cross(zaxis).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis).Normalize());

	m_worldMatrix.m[0][0] = xaxis.x;		m_worldMatrix.m[0][1] = xaxis.y;		m_worldMatrix.m[0][2] = xaxis.z;		m_worldMatrix.m[0][3] = 0.0f;
	m_worldMatrix.m[1][0] = yaxis.x;		m_worldMatrix.m[1][1] = yaxis.y;		m_worldMatrix.m[1][2] = yaxis.z;		m_worldMatrix.m[1][3] = 0.0f;
	m_worldMatrix.m[2][0] = zaxis.x;		m_worldMatrix.m[2][1] = zaxis.y;		m_worldMatrix.m[2][2] = zaxis.z;		m_worldMatrix.m[2][3] = 0.0f;
	m_worldMatrix.m[3][0] = m_position.x;	m_worldMatrix.m[3][1] = m_position.y;	m_worldMatrix.m[3][2] = m_position.z;	m_worldMatrix.m[3][3] = 1.0f;
}

void Camera::CalculateViewMatrix()
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = m_up.Cross(zaxis).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis).Normalize());

	m_viewMatrix.m[0][0] = xaxis.x;					m_viewMatrix.m[0][1] = yaxis.x;					m_viewMatrix.m[0][2] = zaxis.x;					m_viewMatrix.m[0][3] = 0.0f;
	m_viewMatrix.m[1][0] = xaxis.y;					m_viewMatrix.m[1][1] = yaxis.y;					m_viewMatrix.m[1][2] = zaxis.y;					m_viewMatrix.m[1][3] = 0.0f;
	m_viewMatrix.m[2][0] = xaxis.z;					m_viewMatrix.m[2][1] = yaxis.z;					m_viewMatrix.m[2][2] = zaxis.z;					m_viewMatrix.m[2][3] = 0.0f;
	m_viewMatrix.m[3][0] = -m_position.Dot(xaxis);	m_viewMatrix.m[3][1] = -m_position.Dot(yaxis);	m_viewMatrix.m[3][2] = -m_position.Dot(zaxis);	m_viewMatrix.m[3][3] = 1.0f;
}

void Camera::CalculateLookAtMatrix()
{
	m_lookAtMatrix = m_viewMatrix * m_projectionMatrix * m_rotationMatrix;
}


Camera::Updater& Camera::Updater::SetTarget(Vector3 target)
{
	m_camera.SetTarget(target);

	m_modifedMatricesBitset |= (c_worldMatrixMask | c_viewMatrixMask | c_lookAtMatrixMask);

	return *this;
}

Camera::Updater& Camera::Updater::SetPosition(Vector3 pos)
{
	m_camera.SetPosition(pos);

	m_modifedMatricesBitset |= (c_worldMatrixMask | c_viewMatrixMask | c_lookAtMatrixMask);

	return *this;
}

Camera::Updater& Camera::Updater::MoveUp(GLfloat deltaTime)
{
	m_camera.MoveUp(deltaTime);

	m_modifedMatricesBitset |= (c_worldMatrixMask | c_viewMatrixMask | c_lookAtMatrixMask);

	return *this;
}

Camera::Updater& Camera::Updater::MoveLeft(GLfloat deltaTime)
{
	m_camera.MoveLeft(deltaTime);

	m_modifedMatricesBitset |= (c_worldMatrixMask | c_viewMatrixMask | c_lookAtMatrixMask);

	return *this;
}

Camera::Updater& Camera::Updater::SetRotationZ(GLfloat angle)
{
	m_camera.SetRotationZ(angle);

	m_modifedMatricesBitset |= c_lookAtMatrixMask;

	return *this;
}

void Camera::Updater::ApplyChanges()
{
	if (m_modifedMatricesBitset & c_worldMatrixMask)
	{
		m_camera.CalculateWorldMatrix();
	}

	if (m_modifedMatricesBitset & c_viewMatrixMask)
	{
		m_camera.CalculateViewMatrix();
	}

	if (m_modifedMatricesBitset & c_lookAtMatrixMask)
	{
		m_camera.CalculateLookAtMatrix();
	}
}
