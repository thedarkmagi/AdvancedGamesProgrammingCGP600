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

void SceneNode::updateCollisionTree(XMMATRIX * world, float scale)
{
	// the local_world matrix will be used to calculate the local transformations for this node
	 m_local_world_matrix = XMMatrixIdentity();

	 m_local_world_matrix = XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	 m_local_world_matrix *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	 m_local_world_matrix *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));

	 m_local_world_matrix *= XMMatrixScaling(m_scale, m_scale, m_scale);

	 m_local_world_matrix *= XMMatrixTranslation(m_x, m_y, m_z);

	// the local matrix is multiplied by the passed in world matrix that contains the concatenated
	// transformations of all parent nodes so that this nodes transformations are relative to those
	 m_local_world_matrix *= *world;

	// calc the world space scale of this object, is needed to calculate the  
	// correct bounding sphere radius of an object in a scaled hierarchy
	m_world_scale = scale * m_scale;

	XMVECTOR v;
	if (m_pGameObject)
	{
		Model* temp = m_pGameObject->getModel();
		v = temp->getBoundingSpherePos();
	}
	else v = XMVectorSet(0, 0, 0, 0); // no model, default to 0

									  // find and store world space bounding sphere centre
	v = XMVector3Transform(v, m_local_world_matrix);
	m_world_centre_x = XMVectorGetX(v);
	m_world_centre_y = XMVectorGetY(v);
	m_world_centre_z = XMVectorGetZ(v);

	// traverse all child nodes, passing in the concatenated world matrix and scale
	for (int i = 0; i< m_children.size(); i++)
	{
		m_children[i]->updateCollisionTree(&m_local_world_matrix, m_world_scale);
	}

}

bool SceneNode::checkCollision(SceneNode * compare_tree)
{
	return checkCollision(compare_tree, this);
}

bool SceneNode::checkCollision(SceneNode * compare_tree, SceneNode * object_tree_root)
{
	// check to see if root of tree being compared is same as root node of object tree being checked
	// i.e. stop object node and children being checked against each other
	if (object_tree_root == compare_tree) return false;

	// only check for collisions if both nodes contain a model
	if (m_pGameObject && compare_tree->m_pGameObject)
	{
		XMVECTOR v1 = getWorldCentrePosition();
		XMVECTOR v2 = compare_tree->getWorldCentrePosition();
		XMVECTOR vdiff = v1 - v2;

		//XMVECTOR a = XMVector3Length(vdiff);
		float x1 = XMVectorGetX(v1);
		float x2 = XMVectorGetX(v2);
		float y1 = XMVectorGetY(v1);
		float y2 = XMVectorGetY(v2);
		float z1 = XMVectorGetZ(v1);
		float z2 = XMVectorGetZ(v2);

		float dx = x1 - x2;
		float dy = y1 - y2;
		float dz = z1 - z2;

		// check bounding sphere collision
		if (dx*dx + dy * dy + dz * dz <
			(compare_tree->m_pGameObject->getModel()->GetBoundingSphereRadius() * compare_tree->m_world_scale) +
			(this->m_pGameObject->getModel()->GetBoundingSphereRadius() * m_world_scale))
		{
			return true;
		}
	}

	// iterate through compared tree child nodes
	for (int i = 0; i< compare_tree->m_children.size(); i++)
	{
		// check for collsion against all compared tree child nodes 
		if (checkCollision(compare_tree->m_children[i], object_tree_root) == true) return true;
	}

	// iterate through composite object child nodes
	for (int i = 0; i< m_children.size(); i++)
	{
		// check all the child nodes of the composite object against compared tree
		if (m_children[i]->checkCollision(compare_tree, object_tree_root) == true) return true;
	}

	return false;

}

bool SceneNode::checkCollisionRay(ObjFileModel::xyz * ray, ObjFileModel::xyz * rayDirection, SceneNode * compare_tree, SceneNode * object_tree_root)
{
	if (m_pGameObject)
	{
		if (m_pGameObject->getModel())
		{
			ObjFileModel::xyz dist;
			dist.x = ray->x - XMVectorGetX(getWorldCentrePosition());
			dist.y = ray->y - XMVectorGetY(getWorldCentrePosition());
			dist.z = ray->z - XMVectorGetZ(getWorldCentrePosition());

			if (dist.x*dist.x + dist.y * dist.y + dist.z*dist.z > (compare_tree->m_pGameObject->getModel()->GetBoundingSphereRadius() * compare_tree->m_world_scale) +
				(this->m_pGameObject->getModel()->GetBoundingSphereRadius() * m_world_scale))
			{
				for (int i = 0; i < m_pGameObject->getModel()->getObject()->numverts; i += 3)
				{

				}
			}
		}
	}

	return false;
}

bool SceneNode::lookAt_XZ(float x, float z)
{
	float dx = x - m_x;
	float dz = z - m_z;

	m_yAngle = atan2(dx, dz) *(180.0 / XM_PI);
	return false;
}

bool SceneNode::lookAt_XZ(float x, float z, SceneNode * root_node)
{
	return false;
}

bool SceneNode::moveForward(float amount)
{
	m_x += sin(m_yAngle * (XM_PI / 180.0)) * amount;
	m_z += cos(m_yAngle * (XM_PI / 180.0)) * amount;
	return false;
}

bool SceneNode::moveForward(float amount, SceneNode * root_node)
{
	float old_x = m_x;	// save current state 
	float old_z = m_z;	// save current state 
	m_x += sin(m_yAngle * (XM_PI / 180.0)) * amount;
	m_z += cos(m_yAngle * (XM_PI / 180.0)) * amount;		// update state

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	root_node->updateCollisionTree(&identity, 1.0);

	// check for collision of this node (and children) against all other nodes
	if (checkCollision(root_node) == true)
	{
		// if collision restore state
		m_x = old_x;
		m_z = old_z;
		return true;
	}

	return false;
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

bool SceneNode::incX(float in, SceneNode * root_node)
{
	float old_x = m_x;	// save current state 
	m_x += in;		// update state

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	root_node->updateCollisionTree(&identity, 1.0);

	// check for collision of this node (and children) against all other nodes
	if (checkCollision(root_node) == true)
	{
		// if collision restore state
		m_x = old_x;

		return true;
	}

	return false;

}

bool SceneNode::incY(float in, SceneNode * root_node)
{
	float old_y = m_y;	// save current state 
	m_y += in;		// update state

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	root_node->updateCollisionTree(&identity, 1.0);

	// check for collision of this node (and children) against all other nodes
	if (checkCollision(root_node) == true)
	{
		// if collision restore state
		m_y = old_y;

		return true;
	}

	return false;
}

bool SceneNode::incZ(float in, SceneNode * root_node)
{
	float old_z = m_z;	// save current state 
	m_z += in;		// update state

	XMMATRIX identity = XMMatrixIdentity();

	// since state has changed, need to update collision tree
	// this basic system requires entire hirearchy to be updated
	// so start at root node passing in identity matrix
	root_node->updateCollisionTree(&identity, 1.0);

	// check for collision of this node (and children) against all other nodes
	if (checkCollision(root_node) == true)
	{
		// if collision restore state
		m_z = old_z;

		return true;
	}

	return false;
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
XMVECTOR SceneNode::getWorldCentrePosition()
{
	return XMVectorSet( m_world_centre_x,
						m_world_centre_y,
						m_world_centre_z, 0.0);
}
#pragma endregion