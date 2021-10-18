#pragma once

#include "utilities.h" 
#include "GameConfig.h"


class Camera
{
public:
	class Updater
	{
	public:
		Updater& SetTarget(Vector3 target);
		Updater& SetPosition(Vector3 pos);

		Updater& MoveUp(GLfloat deltaTime);
		Updater& MoveLeft(GLfloat deltaTime);

		Updater& SetRotationZ(GLfloat angle);

		void ApplyChanges();
	private:
		Updater(Camera& camera) : m_camera(camera), m_modifedMatricesBitset{}
		{
		}
		friend class Camera;

	private:
		static constexpr uint8_t c_worldMatrixMask		= 1 << 0;
		static constexpr uint8_t c_viewMatrixMask		= 1 << 1;
		static constexpr uint8_t c_lookAtMatrixMask		= 1 << 2;

	private:
		Camera& m_camera;

		uint8_t m_modifedMatricesBitset;
	};

public:
	Camera(void);
	Camera(GLint id, Vector3 position, Vector3 target, Vector3 up, GLfloat fovY, GLfloat fNear, GLfloat fFar, GLfloat fSpeed);
	Camera(GLint id, GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat fNear, GLfloat fFar, GLfloat fSpeed);
	~Camera(void);

	GLfloat		GetNear() const { return m_fNear; }
	GLfloat		GetFar() const { return m_fFar; }
	void		SetId(GLint id) { m_id = id; }
	GLint		GetId() const { return m_id; }

	Vector3		GetPosition() const { return m_position; }
	Matrix		GetWorldMatrix() const { return m_worldMatrix; }
	Matrix		GetViewMatrix() const { return m_viewMatrix; }
	Matrix		GetProjectionMatrix() const { return m_projectionMatrix; }
	Matrix		GetLookAtMatrix() const { return m_lookAtMatrix; }

	Updater		BeginUpdate() { return Updater(*this); }

private:
	void SetTarget(Vector3 target);
	void SetPosition(Vector3 pos);

	void MoveUp(GLfloat deltaTime);
	void MoveLeft(GLfloat deltaTime);

	void SetRotationZ(GLfloat angle);

private:
	GLint		m_id;
	bool		m_is2D;
	Vector3		m_position;
	Vector3		m_target;
	Vector3		m_up;

	Matrix		m_worldMatrix;
	Matrix		m_viewMatrix;
	Matrix		m_projectionMatrix;
	Matrix		m_rotationMatrix;
	Matrix		m_lookAtMatrix;

	GLfloat		m_fSpeed;
	GLfloat		m_fNear;
	GLfloat		m_fFar;

	void		CalculateWorldMatrix();
	void		CalculateViewMatrix();
	void		CalculateLookAtMatrix();
};

