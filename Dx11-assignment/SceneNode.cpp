#include "SceneNode.h"



SceneNode::SceneNode()
{
	m_pModel = NULL;
	m_x = 0.0f;
	m_y = 0.1f;
	m_z = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_zAngle = 0.0f;
	m_scale = 1.0f;
}


SceneNode::~SceneNode()
{
}

void SceneNode::addChildNode(SceneNode * n)
{
	m_children.push_back(n);
}

bool SceneNode::detatchNode(SceneNode * n)
{
	// traverse tree to find node to detatch
	for (int i = 0; i < m_children.size(); i++)
	{
		if (n == m_children[i])
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}
		if (m_children[i]->detatchNode(n) == true) return true;
	}
	return false; // node not in this tree

}

void SceneNode::execute(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection, XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour)
{
	// the local_world matrix will be used to calc the local transformations for this node
	XMMATRIX local_world = XMMatrixIdentity();

	local_world = XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	local_world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));

	local_world *= XMMatrixScaling(m_scale, m_scale, m_scale);

	local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	// the local matrix is multiplied by the passed in world matrix that contains the concatenated
	// transformations of all parent nodes so that this nodes transformations are relative to those
	local_world *= *world;

	// only draw if there is a model attached
	if (m_pModel) m_pModel->Draw(&local_world, view, projection);
	if (m_pGameObject)
	{
		m_pGameObject->setLightingValues(directionLightVector, lightColour, ambientLightColour);
		m_pGameObject->update(&local_world, view, projection);
	}
	// traverse all child nodes, passing in the concatenated world matrix
	for (int i = 0; i< m_children.size(); i++)
	{
		m_children[i]->execute(&local_world, view, projection, directionLightVector, lightColour, ambientLightColour);
	}

}

#pragma region Gets & Sets
void SceneNode::SetXPos(float num)
{
	m_x = num;
}

void SceneNode::SetYPos(float num)
{
	m_y = num;
}

void SceneNode::SetZPos(float num)
{
	m_z = num;
}

float SceneNode::GetXPos()
{
	return m_x;
}

float SceneNode::GetYPos()
{
	return m_y;
}

float SceneNode::GetZPos()
{
	return m_z;
}

void SceneNode::SetXRot(float num)
{
	m_xAngle = num;
}

void SceneNode::SetYRot(float num)
{
	m_yAngle = num;
}

void SceneNode::SetZRot(float num)
{
	m_zAngle = num;
}

float SceneNode::GetXRot()
{
	return m_xAngle;
}

float SceneNode::GetYRot()
{
	return m_yAngle;
}

float SceneNode::GetZRot()
{
	return m_zAngle;
}

void SceneNode::SetScale(float num)
{
	m_scale = num;
}

float SceneNode::GetScale()
{
	return m_scale;
}
void SceneNode::SetModel(Model * m)
{ 
	m_pModel = m;
}
void SceneNode::SetGameObject(GameObject * m)
{
	m_pGameObject = m;
}
#pragma endregion