#pragma once


class PolygonWipe
{
private:

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	//�����ɃV�F�[�_�[�֘A�̕ϐ���ǉ�
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;


	float m_Threshold; //�\���̂��߂̂������l

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};