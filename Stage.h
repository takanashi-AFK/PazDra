#pragma once
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class Stage : public GameObject
{
private:
	enum COLOR {
		RED = 0,
		BLUE,
		YELLOW,
		GREEN,
		PURPLE,
		HEART,
		NUM,
	};

	static const int WIDTH = 6;
	static const int HEIGHT = 5;
	COLOR field[HEIGHT][WIDTH];
	COLOR selectColor;

	int hPict_[COLOR::NUM];
	int selectX, selectY;//選ばれた場所

	Text* pText;
	XMFLOAT3 MousePos;


	/// <summary>
	/// ドット座標から3D座標に変換
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	XMFLOAT3 ConvDrawPos(float x, float y);

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Stage(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};