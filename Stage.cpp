#include "Stage.h"
#include "Engine/Image.h"
#include "Engine/text.h"
#include "Engine/GameObject.h"
#include "Engine/Input.h"
#include "Engine/Debug.h"
namespace {
	static const int SIZE = 40;//ボールの直系	
	static const int LEFT = 60;//盤面の左の位置
	static const int TOP  = 80;//盤面の上の位置
}


//コンストラクタ
Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"),pText(nullptr),selectX(-1),selectY(-1)
{
	/*const char* filename[] = { "ball0.png", "ball1.png", "ball3.png", "ball2.png", "ball4.png", "ball5.png" };

	for (int i = 0; i < COLOR::NUM; i++)
	{
		hPict_[i] = Image::Load(filename[i]);
	}*/

	hPict_[COLOR::RED] = Image::Load("ball0.png");
	hPict_[COLOR::BLUE] = Image::Load("ball1.png");
	hPict_[COLOR::YELLOW] = Image::Load("ball3.png");
	hPict_[COLOR::GREEN] = Image::Load("ball2.png");
	hPict_[COLOR::PURPLE] = Image::Load("ball4.png");
	hPict_[COLOR::HEART] = Image::Load("ball5.png");



	for (int H = 0; H < HEIGHT; H++) {
		for (int W = 0; W < WIDTH; W++){
			field[H][W] = (COLOR) (rand() % COLOR::NUM);
		}
	}
	field[2][3] = COLOR::BLUE;
}

//初期化
void Stage::Initialize()
{
}

//更新
void Stage::Update()
{
	MousePos = Input::GetMousePosition();
	MousePos.x -= 608; MousePos.y -= 330;
	if (MousePos.x >= 0)
		selectX = (int)(MousePos.x / 40);
	else
		selectX = -1;

	if (MousePos.y >= 0)
		selectY = (int)(MousePos.y / 40);
	else
		selectY = -1;

	if (selectX >= 0 && selectX < WIDTH && selectY >= 0 && selectY < HEIGHT)
		selectColor = field[selectY][selectX];
}
//描画
void Stage::Draw()
{
	for (int H = 0; H < HEIGHT; H++){

		for (int W = 0; W < WIDTH; W++){
			int color = field[H][W];
			Transform t;

			t.position_ = ConvDrawPos(W * 40, H * 40);

			if (W == selectX && H == selectY) {
				t.scale_ = (XMFLOAT3(1.2f, 1.2f, 0));
			}
			else {
				t.scale_ = (XMFLOAT3(1.0f, 1.0f, 0));
			}
			Image::SetTransform(hPict_[color], t);
			Image::Draw(hPict_[color]);
		}
	}	
	//{
	//pText = new Text;
	//pText->Initialize();
	//std::string str = "MouseX " + std::to_string(MousePos.x) + "   MouseY " + std::to_string(MousePos.y);
	//pText->Draw(20, 20, str.c_str());
	//}

	/*float x = (MousePos.x - 608 - 20) / 40 * 0.062f;
	float y = -(MousePos.y - 330 - 20) / 40 * 0.110f;*/
	Transform t;
	//t.position_ = XMFLOAT3(x, y, 0);
	t.position_ = ConvDrawPos(MousePos.x - 20, MousePos.y - 20);

	Image::SetTransform(hPict_[selectColor], t);
	Image::Draw(hPict_[selectColor]);

}

//開放
void Stage::Release()
{
}

XMFLOAT3 Stage::ConvDrawPos(float x, float y)
{
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y = -y / 40 * 0.110;
	p.z = 0;
	return p;
}
