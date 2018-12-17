#pragma once
#include "objfilemodel.h"
class Model
{
private:
	ID3D11Device*			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;

	ObjFileModel*		m_pObject;
	ID3D11VertexShader*	m_pVShader;
	ID3D11PixelShader*	m_pPShader;
	ID3D11InputLayout*	m_pInputLayout;
	ID3D11Buffer*		m_pConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11ShaderResourceView* m_pTexture1;
	ID3D11SamplerState* m_pSampler0;

	float			m_x, m_y, m_z;
	float			m_xAngle, m_yAngle, m_zAngle;
	float			m_scale;
	float			m_bounding_sphere_centre_x,
					m_bounding_sphere_centre_y,
					m_bounding_sphere_centre_z, 
					m_bounding_sphere_radius;
	bool			m_twoTextures;
	XMVECTOR* direction_light_vector; // 16 bytes;
	XMVECTOR* directional_light_colour; // 16 bytes;
	XMVECTOR* ambient_light_colour; // 16 bytes;

	void CalculateModelCentrePoint();
	void CalculaterBoundingSphereRadius();
public:
	Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Model();
	HRESULT LoadObjModel(char* Filename, int shaderFileNumber);
	HRESULT setupShader();
	void Draw(XMMATRIX* view, XMMATRIX* projection);
	HRESULT AddTexture(char* filename);
	HRESULT AddTexture(char* filename, char* filename2);
	void lookAt_XZ(float x, float z);
	void moveForward(float distance);

	XMVECTOR GetBoundingSphereWorldSpacePosition();
	float GetBoundingSphereRadius();
	bool CheckCollision(Model* input);
#pragma region Gets & Sets
	void SetXPos(float num);
	void SetYPos(float num);
	void SetZPos(float num);

	float GetXPos();
	float GetYPos();
	float GetZPos();

	void IncXPos(float num);
	void IncYPos(float num);
	void IncZPos(float num);

	void SetXRot(float num);
	void SetYRot(float num);
	void SetZRot(float num);

	float GetXRot();
	float GetYRot();
	float GetZRot();

	void IncXRot(float num);
	void IncYRot(float num);
	void IncZRot(float num);

	void SetScale(float num);
	float GetScale();
	void IncScale(float num);

	void setDirectionLightVector(XMVECTOR* directionLightVector);
	void setdirectionalLightColour(XMVECTOR* lightColour);
	void setAmbientLightColour(XMVECTOR* ambientLightColour);
#pragma endregion
};

