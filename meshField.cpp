
#include "main.h"
#include "renderer.h"
#include "meshField.h"
#include <random>

// マップの幅と高さを定義
const int MapWidth = 41;
const int MapHeight = 41;

// マップデータの初期化
void InitializeMapData(float mapData[MapWidth][MapHeight]) {
	std::random_device rd; // 乱数生成デバイス
	std::mt19937 gen(rd()); // メルセンヌ・ツイスタ乱数生成器
	std::uniform_real_distribution<float> heightDistribution(0.0f, 5.0f); // 高さの範囲を設定

	// マップデータをランダムな高さで初期化
	for (int x = 0; x < MapWidth; x++) {
		for (int z = 0; z < MapHeight; z++) {
			mapData[x][z] = heightDistribution(gen);
		}
	}
}


//各頂点の高さを決める
float g_FieldHeight[MapWidth][MapHeight] =
{
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	{2.0f,1.0f,5.0f,3.0f,3.0f,5.0f,3.0f,3.0f,5.0f,0.0f,3.0f,5.0f,2.0f,3.0f,4.0f,2.0f,2.0f,2.0f,0.0f,3.0f,3.0f,0.0f,5.0f,1.0f,2.0f,1.0f,3.0f,4.0f,3.0f,5.0f,5.0f,4.0f,4.0f,2.0f,1.0f,0.0f,3.0f,5.0f,4.0f,1.0f,5.0f},
	
};

void MeshField::Init()
{
	// マップデータの初期化
	InitializeMapData(g_FieldHeight);

	// 頂点バッファ生成
	{
		for (int x = 0; x <= 40; x++)
		{
			for (int z = 0; z <= 40; z++)
			{
				m_Vertex[x][z].Position = D3DXVECTOR3((x - 10) * 5.0f, g_FieldHeight[z][x], (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトル
				m_Vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}

		//法線ベクトル
		for (int x = 1; x < 39; x++)
		{
			for (int z = 1; z < 39; z++)
			{
				D3DXVECTOR3 vx, vz, vn;
				vx = m_Vertex[x + 1][z].Position - m_Vertex[x - 1][z].Position;
				vz = m_Vertex[x][z - 1].Position - m_Vertex[x][z + 1].Position;

				//外積
				D3DXVec3Cross(&vn, &vz, &vx);
				//正規化
				D3DXVec3Normalize(&vn, &vn);
				m_Vertex[x][z].Normal = vn;

			}
		}


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 41 * 41;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}




	// インデックスバッファ生成
	{
		unsigned int index[(42 * 2) * 40 - 2];

		int i = 0;
		for (int x = 0; x < 40; x++)
		{
			for (int z = 0; z < 41; z++)
			{
				index[i] = x * 41 + z;
				i++;

				index[i] = (x + 1) * 41 + z;
				i++;
			}

			if (x == 39)
				break;

			index[i] = (x + 1) * 41 + 40;
			i++;

			index[i] = (x + 1) * 41;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((42 * 2) * 40 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}




	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/green.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\depthShadowMappingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\depthShadowMappingPS.cso");


}


void MeshField::UnInit()
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}


void MeshField::Update()
{

}


void MeshField::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	ID3D11ShaderResourceView* depthShadowTexture = Renderer::GetDepthShadowTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &depthShadowTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed((42 * 2) * 40 - 2, 0, 0);

}

float MeshField::GetHight(D3DXVECTOR3 Position) //const
{
	int x, z;
	//どこのブロックにいるか求める
	x = Position.x / 5.0f + 10.0f;
	z = Position.z / -5.0f + 10.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	//上の三角形と下の三角形の分割する
	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	//外積を計算して垂直な角度を求める
	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	//内積はどこを向いていても９０度だったら０になる
	if (c.y > 0.0f)
	{
		//左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		//右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}
	//高さを取得
	//方程式の式変形
	py = -((Position.x - pos1.x) * n.x
		+ (Position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

void InitializeMap()
{
	InitializeMapData(g_FieldHeight);
}
