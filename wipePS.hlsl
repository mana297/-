

#include "common.hlsl"


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

	 //�w�i�F��ݒ�
    outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f);
 
 //���C�v�p�e�N�X�`������T���v�����O(rgba����r�������o����)
    float dissolveValue = g_Texture.Sample(g_SamplerState, In.TexCoord);
 
 //step(x,y) �� x <= y �Ȃ�1��Ԃ� �����łȂ��Ȃ�0��Ԃ�
    outDiffuse.a = step(dissolveThreshold, dissolveValue);
}

