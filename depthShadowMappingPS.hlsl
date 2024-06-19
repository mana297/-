#include "common.hlsl"

Texture2D g_Texture : register(t0); // �ʏ�e�N�X�`��
Texture2D g_TextureDepthShadow : register(t1); // �V���h�E�}�b�v
SamplerState g_SamplerState : register(s0);

// PCF�̃T���v�����i�傫���قǊ��炩�ȉe�ɂȂ�j
#define PCF_SAMPLE_COUNT 4

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // �����ł̐F�����ʂ̃e�N�X�`�������_�F���쐬���Ă���
    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    outDiffuse *= In.Diffuse;

    In.ShadowPosition.xyz /= In.ShadowPosition.w; // ���K���f�o�C�X���W�ɕϊ�
    In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5; // �e�N�X�`�����W�ɕϊ�
    In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;

    float2 shadowMapSize = float2(1024.0, 1024.0); // �V���h�E�}�b�v�̃T�C�Y���蓮�Ŏw�肷��

    float totalDarkness = 0.0;

    // PCF�T���v�����O���[�v
    for (int i = -PCF_SAMPLE_COUNT / 2; i <= PCF_SAMPLE_COUNT / 2; ++i)
    {
        for (int j = -PCF_SAMPLE_COUNT / 2; j <= PCF_SAMPLE_COUNT / 2; ++j)
        {
            // �T���v���ʒu���v�Z
            float2 samplePos = In.ShadowPosition.xy + float2(i, j) / shadowMapSize;

            // �V���h�E�}�b�v�e�N�X�`�����A���C�g�J��������s�N�Z���܂ł̋����i�[�x�l�j���擾
            float depth = g_TextureDepthShadow.Sample(g_SamplerState, samplePos).r;

            // �擾�l���ʏ�J��������s�N�Z���ւ̋�����菬����
            if (depth < In.ShadowPosition.z - 0.001) // 0.001��Z�t�@�C�e�B���O�␳�l
            {
                totalDarkness += 1.0; // �e�̃s�N�Z����������΃J�E���g�A�b�v
            }
        }
    }



    // �e�̈Â��𒲐�
    float darknessRatio = totalDarkness / (PCF_SAMPLE_COUNT * PCF_SAMPLE_COUNT);
    outDiffuse.rgb *= lerp(1.0, 0.5, darknessRatio); // �e�̈Â��𒲐�
}
