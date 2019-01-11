#include "Model.h"
#include <sstream>
#include "ModelManager.h"
struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; // 64bytes

	XMVECTOR direction_light_vector; // 16 bytes;
	XMVECTOR directional_light_colour; // 16 bytes;
	XMVECTOR ambient_light_colour; // 16 bytes;
	bool	twoTextures;
};// total 112;
const int constantBufferByteWidth = 112;

void Model::CalculateModelCentrePoint()
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	minX = D3D11_FLOAT32_MAX;
	minY = D3D11_FLOAT32_MAX; 
	minZ = D3D11_FLOAT32_MAX;
	maxX = -D3D11_FLOAT32_MAX;
	maxY = -D3D11_FLOAT32_MAX; 
	maxZ = -D3D11_FLOAT32_MAX;
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		if (minX > m_pObject->vertices[i].Pos.x)
		{
			minX = m_pObject->vertices[i].Pos.x;
		}
		if (maxX < m_pObject->vertices[i].Pos.x)
		{
			maxX = m_pObject->vertices[i].Pos.x;
		}

		if (minY > m_pObject->vertices[i].Pos.y)
		{
			minY = m_pObject->vertices[i].Pos.y;
		}
		if (maxY < m_pObject->vertices[i].Pos.y)
		{
			maxY = m_pObject->vertices[i].Pos.y;
		}

		if (minZ > m_pObject->vertices[i].Pos.z)
		{
			minZ = m_pObject->vertices[i].Pos.z;
		}
		if (maxZ < m_pObject->vertices[i].Pos.z)
		{
			maxZ = m_pObject->vertices[i].Pos.z;
		}
	}

	// if collision is off centre this is at fault
	m_bounding_sphere_centre_x = minX + ( (abs( maxX) + abs(minX)) /2);
	m_bounding_sphere_centre_y = minY + ((abs(maxY) + abs(minY)) / 2);
	m_bounding_sphere_centre_z = minZ + ((abs(maxZ) + abs(minZ)) / 2);
}
void Model::CalculaterBoundingSphereRadius()
{
	float distance=0;
	XMFLOAT3 centrePoints(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z);
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		float currentDistance = pow(centrePoints.x - m_pObject->vertices[i].Pos.x, 2)
			+ pow(centrePoints.y - m_pObject->vertices[i].Pos.y, 2)
			+ pow(centrePoints.z - m_pObject->vertices[i].Pos.z, 2);
		//XMFLOAT3
		if (distance == 0)
		{
			distance = currentDistance;
		}
		else
		{
			if (distance < currentDistance)
			{
				distance = currentDistance;
			}
		}
	}

	m_bounding_sphere_radius = distance;
	distance = sqrt(distance); //exists only to compare values from tutorial
}
Model::Model(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_pD3DDevice = device;
	m_pImmediateContext = deviceContext;
	m_x = 0.0f;
	m_y = 0.1f;
	m_z = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_zAngle = 0.0f;
	m_scale = 0.50f;
	m_twoTextures = false;
}

HRESULT Model::LoadObjModel(char * Filename, int shaderFileNumber)
{
	HRESULT hr = S_OK;

	//m_pObject = new ObjFileModel(Filename, m_pD3DDevice, m_pImmediateContext);
	m_pObject = ModelManager::getInstance()->getModel(Filename, m_pD3DDevice, m_pImmediateContext);
	if (m_pObject->filename == "FILE NOT LOADED")hr= S_FALSE;
	

	//create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // can use updateSubResource() to update
	constant_buffer_desc.ByteWidth = constantBufferByteWidth; // Must be a multiple fo 16 CB Struct  Could be a constant in
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // use as a constant buffer

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}
	//Load and compile the pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS, *PS, *error;
	std::stringstream ss;
	ss<< "model_shaders"<<std::to_string(shaderFileNumber).c_str() << ".hlsl";
	string input = ss.str();
	hr = D3DX11CompileFromFile((LPCSTR)input.c_str(), 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	if (error != 0)//Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}
	hr = D3DX11CompileFromFile((LPCSTR)input.c_str(), 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);
	if (error != 0)//Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}

	//Create shader objects
	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);
	if (FAILED(hr))
	{
		return hr;
	}

	//Set the shader objects as active
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	//Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//Be very careful setting the correct dxgi format and D3D version
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc),VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	CalculateModelCentrePoint();
	CalculaterBoundingSphereRadius();

	return hr;
}

HRESULT Model::setupShader()
{
	HRESULT hr = S_OK;
	//Load and compile the pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob *VS, *PS, *error;
	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0)//Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}

	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0)//Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}

	//Create shader objects
	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);
	if (FAILED(hr))
	{
		return hr;
	}

	//Set the shader objects as active
	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	return hr;
}

void Model::Draw(XMMATRIX* view, XMMATRIX* projection)
{

	

	XMMATRIX world;
	XMMATRIX transpose;
	//scale
	world = XMMatrixScaling(m_scale, m_scale, m_scale);

	//rotate
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	
	//translate
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	MODEL_CONSTANT_BUFFER model_cb_values;
	model_cb_values.WorldViewProjection = world * (*view)*(*projection);

	transpose = XMMatrixTranspose(world);

	model_cb_values.ambient_light_colour = *ambient_light_colour;
	model_cb_values.directional_light_colour = *directional_light_colour;
	model_cb_values.direction_light_vector = XMVector3Transform(*direction_light_vector, transpose);
	model_cb_values.direction_light_vector = XMVector3Normalize(model_cb_values.direction_light_vector);
	if (m_twoTextures)
	{
		model_cb_values.twoTextures = true;
		m_pImmediateContext->PSSetShaderResources(1, 1, &m_pTexture1);
	}
	else
	{
		model_cb_values.twoTextures = false;
	}
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);
	
	
	m_pObject->Draw();
	
}

void Model::Draw(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection)
{
	XMMATRIX transpose;
	MODEL_CONSTANT_BUFFER model_cb_values;
	model_cb_values.WorldViewProjection = (*world)*(*view)*(*projection);
	transpose = XMMatrixTranspose(*world);

	model_cb_values.ambient_light_colour = *ambient_light_colour;
	model_cb_values.directional_light_colour = *directional_light_colour;
	model_cb_values.direction_light_vector = XMVector3Transform(*direction_light_vector, transpose);
	model_cb_values.direction_light_vector = XMVector3Normalize(model_cb_values.direction_light_vector);

	if (m_twoTextures)
	{
		model_cb_values.twoTextures = true;
		m_pImmediateContext->PSSetShaderResources(1, 1, &m_pTexture1);
	}
	else
	{
		model_cb_values.twoTextures = false;
	}

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);

	m_pObject->Draw();

}

HRESULT Model::AddTexture(char* filename)
{
	HRESULT hr = S_OK;

	// setup texture buffer
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, filename, NULL, NULL, &m_pTexture0, NULL);

	return hr;
}

HRESULT Model::AddTexture(char * filename, char * filename2)
{
	HRESULT hr = S_OK;
	m_twoTextures = true;
	// setup texture buffer
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler0);

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, filename, NULL, NULL, &m_pTexture0, NULL);
	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, filename2, NULL, NULL, &m_pTexture1, NULL);
	return hr;
}

void Model::lookAt_XZ(float x, float z)
{
	float dx = x- m_x;
	float dz = z- m_z;

	m_yAngle = atan2(dx, dz) *(180.0 / XM_PI);
}

void Model::moveForward(float amount)
{
	m_x += sin(m_yAngle * (XM_PI / 180.0)) * amount;
	m_z += cos(m_yAngle * (XM_PI / 180.0)) * amount;

}

XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world;
	XMVECTOR offset;
	//scale
	world = XMMatrixScaling(m_scale, m_scale, m_scale);

	//rotate
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));

	//translate
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	offset = XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, 0.0);
	offset = XMVector3Transform(offset, world);

	return offset;
}

float Model::GetBoundingSphereRadius()
{
	return m_bounding_sphere_radius*m_scale;
}

bool Model::CheckCollision(Model * input)
{
	bool result = false;

	if (input == this)
	{
		result = false;
		return false;
	}
	XMVECTOR currentModelPos = GetBoundingSphereWorldSpacePosition();
	XMVECTOR inputModel = input->GetBoundingSphereWorldSpacePosition();
	
	float x1, y1, z1, x2, y2, z2;
	
	x1 = XMVectorGetX(currentModelPos);
	y1 = XMVectorGetY(currentModelPos);
	z1 = XMVectorGetZ(currentModelPos);

	x2 = XMVectorGetX(inputModel);
	y2 = XMVectorGetY(inputModel);
	z2 = XMVectorGetZ(inputModel);
	float distance = pow(x1 - x2, 2)
		+ pow(y1 - y2, 2)
		+ pow(z1 - z2, 2);
	if ((GetBoundingSphereRadius() + input->GetBoundingSphereRadius()) > distance)
	{
		result = true;
	}

	return result;
}



#pragma region Gets & Sets

void Model::SetXPos(float num)
{
	m_x = num;
}

void Model::SetYPos(float num)
{
	m_y = num;
}

void Model::SetZPos(float num)
{
	m_z = num;
}

float Model::GetXPos()
{
	return m_x;
}

float Model::GetYPos()
{
	return m_y;
}

float Model::GetZPos()
{
	return m_z;
}

void Model::IncXPos(float num)
{
	m_x += num;
}

void Model::IncYPos(float num)
{
	m_y += num;
}

void Model::IncZPos(float num)
{
	m_z += num;
}

void Model::SetXRot(float num)
{
	m_xAngle = num;
}

void Model::SetYRot(float num)
{
	m_yAngle = num;
}

void Model::SetZRot(float num)
{
	m_zAngle = num;
}

float Model::GetXRot()
{
	return m_xAngle;
}

float Model::GetYRot()
{
	return m_yAngle;
}

float Model::GetZRot()
{
	return m_zAngle;
}

void Model::IncXRot(float num)
{
	m_xAngle += num;
}

void Model::IncYRot(float num)
{
	m_yAngle += num;
}

void Model::IncZRot(float num)
{
	m_zAngle += num;
}

void Model::SetScale(float num)
{
	m_scale = num;
}

float Model::GetScale()
{
	return m_scale;
}

void Model::IncScale(float num)
{
	m_scale += num;
}

void Model::setDirectionLightVector(XMVECTOR * directionLightVector)
{
	direction_light_vector = directionLightVector;
}

void Model::setdirectionalLightColour(XMVECTOR * lightColour)
{
	directional_light_colour = lightColour;
}

void Model::setAmbientLightColour(XMVECTOR * ambientLightColour)
{
	ambient_light_colour = ambientLightColour;
}

XMVECTOR Model::getBoundingSpherePos()
{
	return XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z , 0.0);
}

ObjFileModel * Model::getObject()
{
	return m_pObject;
}

#pragma endregion

Model::~Model()
{
	if (m_pTexture1) m_pTexture1->Release();
	if (m_pTexture0) m_pTexture0->Release();
	if (m_pSampler0) m_pSampler0->Release();
	if (m_pObject) delete m_pObject;
}
