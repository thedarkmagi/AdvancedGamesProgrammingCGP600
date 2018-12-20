#include "ParticleGenerator.h"
#include <sstream>

struct Particle_Constant_Buffer
{
	XMMATRIX WorldViewProjection; // 64bytes
	XMFLOAT4 color;
};
const int constantBufferByteWidth = 80;
ParticleGenerator::ParticleGenerator(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
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

	for (int i = 0; i < 100; i++)
	{
		Particle* temp = new Particle;
		m_free.push_back(temp);
	}
}



HRESULT ParticleGenerator::setupShader()
{
	HRESULT hr = S_OK;
	
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
	//std::stringstream ss;
	//ss << "model_shaders" << std::to_string(shaderFileNumber).c_str() << ".hlsl";
	//string input = ss.str();
	hr = D3DX11CompileFromFile("particle_shader.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);
	if (error != 0)//Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}
	hr = D3DX11CompileFromFile("particle_shader.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);
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
	};
	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	//CalculateModelCentrePoint();
	//CalculaterBoundingSphereRadius();

	return hr;
}

void ParticleGenerator::Draw(XMMATRIX * view, XMMATRIX * projection, XMVECTOR * cameraPosition)
{
	/*Particle test;
	test.color = XMFLOAT4(1.0f, 0.0f, 0.3f, 1.0f);
	test.gravity - 1;
	test.position = XMFLOAT3(0.0f, 3.0f, 14);
	test.velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	DrawOne(&test, view, projection, cameraPosition);*/
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	XMMATRIX world;
	m_untilParticle -= GameTimer::getInstance()->DeltaTime();
	
	if (m_untilParticle <= 0.0f)
	{
		if (m_isActive)//a bool to check if the particle engine is on or off. Make a getter/setter and use it in main
		{
			it = m_free.begin();//point to the beggining of the free list
								//add a new particle to the back of m_active from the front of m_free
			if (m_free.size() != NULL)//safety check
			{
				switch (pType)//the name of my enum
				{
				case RAINBOW_FOUNTAIN:
				{
					m_age = 2.0f;
					m_untilParticle = 0.008f;
					////////////////////////initialise the particle NOTE: all of this is adjustable for different effects////////////////////////
					(*it)->color = XMFLOAT4(RandomZeroToOne(), RandomZeroToOne(), RandomZeroToOne(), 1.0f);
					(*it)->gravity = 4.5f;
					(*it)->position = XMFLOAT3(0.0f, 1.0f, 3.0f);
					(*it)->velocity = XMFLOAT3(RandomNegOneToPosOne(), 2.50f, RandomNegOneToPosOne());
						////////////////////////////////////////////////////////////////////////////////////////////////
						break;
				}
				default:
				{
					break;
				}
				}
				(*it)->age = 0.0f;//set age to 0. this is used for knowing when to delete the particle

								  //////add the particle from the front of the available list to the back of the active list and remove it
				m_active.push_back(*it);
				m_free.pop_front();
			}
		}
		else m_untilParticle = 0.001f;
	}

	if (m_active.size() != NULL)//safety check
	{
		it = m_active.begin();//point the iterator to the front of the active list ready for processing
		while (it != m_active.end())//move all of the particles
		{

			switch (pType)
			{
			case RAINBOW_FOUNTAIN:
			{
				/////////////////////////ALL of this is adjustable for different effects///////////////////////////////////////////////////////////
				(*it)->age += GameTimer::getInstance()->DeltaTime();
				(*it)->velocity.y -= (*it)->gravity*(GameTimer::getInstance()->DeltaTime());
				(*it)->position.x += (*it)->velocity.x*(GameTimer::getInstance()->DeltaTime());
				(*it)->position.y += (*it)->velocity.y*(GameTimer::getInstance()->DeltaTime());
				(*it)->position.z += (*it)->velocity.z*(GameTimer::getInstance()->DeltaTime());
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				break;
			}
			default:
			{
				break;
			}
			}
			world = XMMatrixIdentity();
			switch (pType)
			{
			case RAINBOW_FOUNTAIN:
			{
				/*set scale and world transforms here*/
				break;
			}
			default:
			{
				break;
			}
			world *= XMMatrixTranslation((*it)->position.x, (*it)->position.y, (*it)->position.z);

			//constant buffer stuff for shader
			Particle_Constant_Buffer pcb;
			pcb.WorldViewProjection = (world) * (*view)*(*projection);
			pcb.color = (*it)->color;

			m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
			m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &pcb, 0, 0);

			//set the shader objects as active
			m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
			m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
			m_pImmediateContext->IASetInputLayout(m_pInputLayout);

			m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

			m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_pImmediateContext->RSSetState(m_pRasterParticle);//set backface culling to on
			m_pImmediateContext->Draw(6, 0);//draw the particle
			m_pImmediateContext->RSSetState(m_pRasterSolid);//set backface culling to off
			if ((*it)->age >= m_age)//check the age of the current particle
			{
				it++;
				m_active.front()->age = m_age;
				m_active.front()->position = { (RandomNegOneToPosOne() + m_x * 10)*(RandomZeroToOne() * 10),m_y + 5.0f, /*position.z*/m_z /*camera_pos->z*/ + 7.0f };
				m_active.front()->velocity = { /*RandomNegOneToPosOne()*/0.0f, 4.50f, RandomNegOneToPosOne() };
				m_free.push_back(m_active.front());//move the (now previously) current active particle to the back of the pool			
				m_active.pop_front();//remove the particle			
			}
			else it++;
			}//end of while
		}//end of if(m_active.size()!=NULL)

	}
}

void ParticleGenerator::DrawOne(Particle * one, XMMATRIX * view, XMMATRIX * projection, XMVECTOR * cameraPosition)
{
	UINT stride = sizeof(XMFLOAT3);
	XMMATRIX world;
	XMMATRIX transpose;
	world = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	world *= XMMatrixRotationY(XMConvertToRadians(XM_PI));
	world *= XMMatrixTranslation(one->position.x, one->position.y, one->position.z);

	Particle_Constant_Buffer particleCB;
	particleCB.WorldViewProjection = world * (*view)*(*projection);
	particleCB.color = one->color;


	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &particleCB, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler0);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture0);


	m_pImmediateContext->Draw(6, 0);


}

HRESULT ParticleGenerator::AddTexture(char* filename)
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

HRESULT ParticleGenerator::AddTexture(char * filename, char * filename2)
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

void ParticleGenerator::lookAt_XZ(float x, float z)
{
	float dx = x- m_x;
	float dz = z- m_z;

	m_yAngle = atan2(dx, dz) *(180.0 / XM_PI);
}

void ParticleGenerator::moveForward(float amount)
{
	m_x += sin(m_yAngle * (XM_PI / 180.0)) * amount;
	m_z += cos(m_yAngle * (XM_PI / 180.0)) * amount;

}

XMVECTOR ParticleGenerator::GetBoundingSphereWorldSpacePosition()
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

float ParticleGenerator::GetBoundingSphereRadius()
{
	return m_bounding_sphere_radius*m_scale;
}

bool ParticleGenerator::CheckCollision(ParticleGenerator * input)
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

float ParticleGenerator::RandomZeroToOne()
{
	return rand()%1;
}

float ParticleGenerator::RandomNegOneToPosOne()
{
	return rand()%2-1;
}

int ParticleGenerator::ParticleFactory()
{
	HRESULT hr = S_OK;
	XMFLOAT3 vertices[6] =//verts for the quad NOTE: could be changed to make different shapes??
	{
		XMFLOAT3(-1.0f,-1.0f,0.0f),
		XMFLOAT3(1.0f,1.0f,0.0f),
		XMFLOAT3(-1.0f,1.0f,0.0f),
		XMFLOAT3(-1.0f,-1.0f,0.0f),
		XMFLOAT3(1.0f,-1.0f,0.0f),
		XMFLOAT3(1.0f,1.0f,0.0f),
	};
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_NONE;//sets no culling
	hr = m_pD3DDevice->CreateRasterizerState(&rasterizer_desc, &m_pRasterSolid);
	rasterizer_desc.CullMode = D3D11_CULL_BACK;//sets backface culling
	hr = m_pD3DDevice->CreateRasterizerState(&rasterizer_desc, &m_pRasterParticle);
	//create the vert buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;//both cpu and gpu
	bufferDesc.ByteWidth = sizeof(XMFLOAT3) * 6/*VERTCOUNT*/;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//edit on the cpu

	hr = m_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);

	if (FAILED(hr)) return 0;

	//copy verts to buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//mapping = locking the buffer which allows writing
	m_pImmediateContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));//copy data
												 //unlock the buffer
	m_pImmediateContext->Unmap(m_pVertexBuffer, NULL);

	/*CalcModelCentrePoint();
	CalcBoundingSphereRadius();*/
	setupShader();

	return 0;

}



#pragma region Gets & Sets

void ParticleGenerator::SetXPos(float num)
{
	m_x = num;
}

void ParticleGenerator::SetYPos(float num)
{
	m_y = num;
}

void ParticleGenerator::SetZPos(float num)
{
	m_z = num;
}

float ParticleGenerator::GetXPos()
{
	return m_x;
}

float ParticleGenerator::GetYPos()
{
	return m_y;
}

float ParticleGenerator::GetZPos()
{
	return m_z;
}

void ParticleGenerator::IncXPos(float num)
{
	m_x += num;
}

void ParticleGenerator::IncYPos(float num)
{
	m_y += num;
}

void ParticleGenerator::IncZPos(float num)
{
	m_z += num;
}

void ParticleGenerator::SetXRot(float num)
{
	m_xAngle = num;
}

void ParticleGenerator::SetYRot(float num)
{
	m_yAngle = num;
}

void ParticleGenerator::SetZRot(float num)
{
	m_zAngle = num;
}

float ParticleGenerator::GetXRot()
{
	return m_xAngle;
}

float ParticleGenerator::GetYRot()
{
	return m_yAngle;
}

float ParticleGenerator::GetZRot()
{
	return m_zAngle;
}

void ParticleGenerator::IncXRot(float num)
{
	m_xAngle += num;
}

void ParticleGenerator::IncYRot(float num)
{
	m_yAngle += num;
}

void ParticleGenerator::IncZRot(float num)
{
	m_zAngle += num;
}

void ParticleGenerator::SetScale(float num)
{
	m_scale = num;
}

float ParticleGenerator::GetScale()
{
	return m_scale;
}

void ParticleGenerator::IncScale(float num)
{
	m_scale += num;
}

void ParticleGenerator::setDirectionLightVector(XMVECTOR * directionLightVector)
{
	direction_light_vector = directionLightVector;
}

void ParticleGenerator::setdirectionalLightColour(XMVECTOR * lightColour)
{
	directional_light_colour = lightColour;
}

void ParticleGenerator::setAmbientLightColour(XMVECTOR * ambientLightColour)
{
	ambient_light_colour = ambientLightColour;
}

void ParticleGenerator::setIsActive(bool input)
{
	m_isActive = input;
}

bool ParticleGenerator::getIsActive()
{
	return m_isActive;
}

#pragma endregion

ParticleGenerator::~ParticleGenerator()
{
	for  (it = m_free.end(); it != m_free.begin(); it--)
	{
		m_free.remove(*it);
	}
	for (it = m_active.end(); it != m_active.begin(); it--)
	{
		m_active.remove(*it);
	}
	if (m_pTexture1) m_pTexture1->Release();
	if (m_pTexture0) m_pTexture0->Release();
	if (m_pSampler0) m_pSampler0->Release();
	if (m_pObject) delete m_pObject;
}
