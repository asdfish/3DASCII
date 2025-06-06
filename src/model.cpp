#include "model.hpp"

SceneObject::SceneObject(float3 position, float3 rotation, const Transform *parent, Model model)
: m_transform(Transform(position, rotation, parent)), m_model(model)
{

}

SceneObject::~SceneObject()
{
}

const Transform& SceneObject::GetTransform()
{
    return m_transform;
    // TODO: insert return statement here
}

const Model& SceneObject::GetModel()
{
    return m_model;
    // TODO: insert return statement here
}

void SceneObject::SetTransform(Transform transform)
{
    m_transform = transform;
}

void SceneObject::SetTransform(float3 position, float3 rotation, const Transform *parent)
{
    m_transform.SetPos(position);
    m_transform.SetRot(rotation);
    m_transform.SetParent(parent);
}

void SceneObject::SetModel(Model model)
{
    m_model = model;
}

Transform::Transform(float3 pos, float3 rot, const Transform* parent)
: m_pos(pos), m_rot(rot), m_parent(parent)
{
}

Transform::~Transform()
{
}

const float3& Transform::GetPos()
{
    return m_pos;
}

const float3& Transform::GetRot()
{
    return m_rot;
}

const Transform* Transform::GetParent()
{
    return m_parent;
}

void Transform::SetPos(const float3 &newPos)
{
    m_pos = newPos;
}

void Transform::SetRot(const float3 &newRot)
{
    m_rot = newRot;
}

void Transform::SetParent(const Transform *parent)
{
    m_parent = parent;
}

Model::Model(const std::vector<float3> &verts, const std::vector<int> &faceIndices)
: m_verts(verts), m_faceIndices(faceIndices)
{
}

Model::~Model()
{
}

const std::vector<float3> &Model::GetVerts()
{
    return m_verts;
}

const std::vector<int> &Model::GetFaceIndices()
{
    return m_faceIndices;
}

void objImporter(const char *path)
{
}
