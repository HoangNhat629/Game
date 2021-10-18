#pragma once
#include "utilities.h" 


class Shader;
class Model;
class Texture;
class Camera;
class BaseObject
{
public:
    BaseObject()
        : m_id(-1), m_name("Empty"), m_pModel(nullptr), m_pShader(nullptr), m_pTexture(nullptr), m_pCamera(nullptr), m_color(Vector4(0.5f, 0.5f, 0.5f, 1.0f)),
        m_position(Vector3(0.0f, 0.0f, 0.0f)), m_rotation(Vector3(0.0f, 0.0f, 0.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f)), m_isActive(true)
    {
    }
    BaseObject(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
        : m_id(id), m_name("TextureObj"), m_pModel(model), m_pShader(shader), m_pTexture(texture),
        m_position(Vector3(0.0f, 0.0f, 0.0f)), m_rotation(Vector3(0.0f, 0.0f, 0.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f)), m_isActive(true)
    {
    }
    BaseObject(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, Vector4 color)
        : m_id(id), m_name("ColorObj"), m_pModel(model), m_pShader(shader), m_color(color),
        m_position(Vector3(0.0f, 0.0f, 0.0f)), m_rotation(Vector3(0.0f, 0.0f, 0.0f)), m_scale(Vector3(1.0f, 1.0f, 1.0f)), m_isActive(true)
    {
    }
    virtual ~BaseObject() {}

    virtual void Init() = 0;
    virtual void Draw() = 0;
    virtual void Update(GLfloat deltaTime) {};

    void SetObjectID(GLuint id) { m_id = id; }
    GLint GetObjectID(GLuint id) { return	m_id; }

    void SetName(std::string name) { m_name = name; }
    std::string GetName() { return	m_name; }

    void SetColor(Vector4 color) { m_color = color; }

    void SetCamera(std::shared_ptr<Camera> cam) { m_pCamera = cam; }
    const std::shared_ptr<Camera>& GetCamera() const { return m_pCamera; }

    void SetModels(std::shared_ptr<Model> model) { m_pModel = model; }
    const std::shared_ptr<Model>& GetModels() const { return m_pModel; }

    void SetShaders(std::shared_ptr<Shader> shader) { m_pShader = shader; }
    const std::shared_ptr<Shader>& GetShaders() const { return m_pShader; }

    void SetTexture(std::shared_ptr<Texture> texture) { m_pTexture = texture; }
    const std::shared_ptr<Texture>& GetTexture() const { return m_pTexture; }

    void SetPosition(Vector3 position) { m_position = position; }
    Vector3 GetPosition() { return m_position; }

    void SetRotation(Vector3 rotation) { m_rotation = rotation; }
    Vector3 GetRotation() { return m_rotation; }

    void SetScale(Vector3 scale) { m_scale = scale; }
    Vector3 GetScale() { return m_scale; }

    bool IsActive() const { return m_isActive; }
    void SetActive(bool active) { m_isActive = active; }

    void CalculateWorldMatrix()
    {
        Matrix Rx, Ry, Rz, R, S, T;
        Rx.SetRotationX(m_rotation.x);
        Ry.SetRotationY(m_rotation.y);
        Rz.SetRotationZ(m_rotation.z);
        R = Rz * Rx * Ry;
        S.SetScale(m_scale);
        T.SetTranslation(m_position);
        m_worldMatrix = S * R * T;
    }

protected:
    Vector3			m_position;
    Vector3			m_scale;
    Vector3			m_rotation;
    Vector4			m_color;
    Matrix			m_worldMatrix;

    std::shared_ptr<Model>		m_pModel;
    std::shared_ptr<Shader>		m_pShader;
    std::shared_ptr<Texture>	m_pTexture;
    std::shared_ptr<Camera>		m_pCamera;

    bool m_isActive;

private:
    GLint			m_id;
    std::string		m_name;
};

