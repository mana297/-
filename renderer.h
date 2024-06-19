#pragma once




// ���_�\����
struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
};


// �}�e���A���\����
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};

// �}�e���A���\����
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture* Texture;
};

// �`��T�u�Z�b�g�\����
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};


struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte���E�p
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;

	D3DXMATRIX ViewMatrix;
	D3DXMATRIX ProjectionMatrix;

};

struct PARAMETER
{
	//D3DXVECTOR4 hp;
	//D3DXCOLOR	Diffuse;
	//D3DXCOLOR	Ambient;

	//���C�v����
	float dissolveThreshold;
	float dissolveRange;
	float dummy[2];
};


class CVertexBuffer;
class CIndexBuffer;
class CTexture;


class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_CameraBuffer;
	static ID3D11Buffer*			m_ParameterBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;


	static ID3D11DepthStencilView* m_DepthShadowDepthStencilView;
	static ID3D11ShaderResourceView* m_DepthShadowShaderResourceView;

public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetParameter(PARAMETER param);


	static void SetDefaltViewport();
	static void SetDepthViewport();

	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


	static ID3D11ShaderResourceView* GetDepthShadowTexture()
	{
		return m_DepthShadowShaderResourceView;
	}

	static void BeginDepth()
	{
		m_DeviceContext->OMSetRenderTargets(0, NULL, m_DepthShadowDepthStencilView);
		m_DeviceContext->ClearDepthStencilView(m_DepthShadowDepthStencilView,
			D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
};
