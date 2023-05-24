#include "stage.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"

namespace {
	static const int SIZE = 40; // ボールの直径
	static const int LEFT = 60; // 盤面の左の位置
	static const int TOP = 80; // 盤面の上の位置
};
namespace STAGE {
	static const int SIZE = 40;
};

Stage::Stage(GameObject* parent) : GameObject(parent, "Stage")
{
	// fieldの配列を全部REDにする
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			field[h][w].color = (COLOR)(rand() % COLOR::NUM);
			field[h][w].x = w * 40;
			field[h][w].y = h * 40;
			field[h][w].bx = w * 40;
			field[h][w].by = h * 40;
			field[h][w].rate = 1.0f;
			field[h][w].doErase = 0;
		}
	}
	selectX = -1; // 選ばれてない
	selectY = -1;
	selectColor = COLOR::NOCOLOR;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hImage[COLOR::RED] = Image::Load("ball0.png");
	hImage[COLOR::BLUE] = Image::Load("ball1.png");
	hImage[COLOR::YELLOW] = Image::Load("ball2.png");
	hImage[COLOR::GREEN] = Image::Load("ball3.png");
	hImage[COLOR::PURPLE] = Image::Load("ball4.png");
	hImage[COLOR::HEART] = Image::Load("ball5.png");
	state = STATE::S_IDLE;
}

void Stage::Update()
{

	switch (state) {
	case STATE::S_IDLE:
		UpdateIdle();
		break;
	case STATE::S_MOVE:
		UpdateMove();
		break;
	case STATE::S_ERASE:
		UpdateErase();
		break;
	case STATE::S_FALL:
		UpdateFall();
		break;
	case STATE::S_ATTACK:
		UpdateAttack();
		break;
	}
}

void Stage::Draw()
{
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			int color = field[h][w].color;
			Transform t;
			t.position_ = ConvDrawPos(field[h][w].x, field[h][w].y);
			if (w == selectX && h == selectY) {
				t.scale_ = XMFLOAT3(1.2f, 1.2f, 0);
			}
			else if (field[h][w].doErase > 0) {
				t.scale_ = XMFLOAT3(0.5f, 0.5f, 0);
			}
			else {
				t.scale_ = XMFLOAT3(1.0f, 1.0f, 0);
			}
			Image::SetTransform(hImage[color], t);
			Image::Draw(hImage[color]);
		}
	}
	if (selectColor != COLOR::NOCOLOR) {
		Transform t;
		t.position_ = ConvDrawPos(mousePos.x - 20, mousePos.y - 20);
		Image::SetTransform(hImage[selectColor], t);
		Image::Draw(hImage[selectColor]);
	}
}

void Stage::Release()
{
}

XMFLOAT3 Stage::ConvDrawPos(float x, float y)
{
	XMFLOAT3 p;
	p.x = x / 40 * 0.062f;
	p.y = -y / 40 * 0.110f;
	p.z = 0;
	return p;
}

void Stage::UpdateIdle()
{
	if (Input::IsMouseButtonDown(0)) { // 左クリック
		CalcMouseSelect();
		if (selectX >= 0 && selectX < WIDTH &&
			selectY >= 0 && selectY < HEIGHT) {
			state = STATE::S_MOVE;
		}
	}
}

void Stage::UpdateMove()
{
	int lastX = selectX;
	int lastY = selectY;
	CalcMouseSelect();
	if (selectY < 0) selectY = 0;
	if (selectX < 0) selectX = 0;
	if (selectY >= HEIGHT) selectY = HEIGHT - 1;
	if (selectX >= WIDTH) selectX = WIDTH - 1;

	if (selectX != lastX || selectY != lastY) {
		auto tmp = field[selectY][selectX];
		field[selectY][selectX] = field[lastY][lastX];
		field[lastY][lastX] = tmp;
#if 0
		field[selectY][selectX].counter = 10;
		field[lastY][lastX].counter = 10;
#else
		auto& s = field[selectY][selectX];
		s.bx = s.x;
		s.by = s.y;
		s.rate = 0.0f;
		auto& la = field[lastY][lastX];
		la.bx = la.x;
		la.by = la.y;
		la.rate = 0.0f;
#endif
	}
	if (Input::IsMouseButtonUp(0)) { // 左クリック
		if (CheckErase()) {
			eraseTime = 30;
			state = STATE::S_ERASE;
		}
		else {
			state = STATE::S_IDLE;
		}
	}
	// 滑らかに移動する演出
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			// 今の位置field[h][w].x, field[h][w].y
			// 本来の位置 w*40, h*40
			auto& b = field[h][w];
			if (b.rate < 1.0f) {
				b.rate += 0.1f;
				if (b.rate > 1.0f)
					b.rate = 1.0f;
				b.x = GetRateValue(b.bx, w * 40, b.rate);
				b.y = GetRateValue(b.by, h * 40, b.rate);
			}
		}
	}
}

void Stage::UpdateErase()
{
	eraseTime--;//これの初期化はEraseにstateが変わる直前でしてる
	if (eraseTime <= 0) {
		PrepareFall();
		state = STATE::S_FALL;
	}
}

void Stage::UpdateFall()
{
	// 滑らかに移動する演出
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			// 今の位置field[h][w].x, field[h][w].y
			// 本来の位置 w*40, h*40
			auto& b = field[h][w];
			if (b.rate < 1.0f) {
				b.rate += 0.1f;
				if (b.rate > 1.0f)
					b.rate = 1.0f;
				b.x = GetRateValue(b.bx, w * 40, b.rate);
				b.y = GetRateValue(b.by, h * 40, b.rate);
			}
		}
	}
}

void Stage::UpdateAttack()
{
}

void Stage::CalcMouseSelect()
{
	mousePos = Input::GetMousePosition();
	mousePos.x -= 608;
	mousePos.y -= 330;
	if (mousePos.x >= 0)
		selectX = (int)(mousePos.x / 40);
	else
		selectX = -1;
	if (mousePos.y >= 0)
		selectY = (int)((mousePos.y) / 40);
	else
		selectY = -1;
	if (selectX >= 0 && selectX < WIDTH &&
		selectY >= 0 && selectY < HEIGHT) {
		selectColor = field[selectY][selectX].color;
	}

}

float Stage::GetRateValue(float begin, float end, float rate)
{
	return (end - begin) * rate + begin;
}

bool Stage::CheckErase()
{
	bool ret = false;
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			if (w < WIDTH - 2 && field[h][w].color == field[h][w + 1].color &&
				field[h][w].color == field[h][w + 2].color) {
				field[h][w].doErase = 1;
				field[h][w + 1].doErase = 1;
				field[h][w + 2].doErase = 1;
				ret = true;
			}
			if (h < HEIGHT - 2 && field[h][w].color == field[h + 1][w].color &&
				field[h][w].color == field[h + 2][w].color) {
				field[h][w].doErase = 1;
				field[h + 1][w].doErase = 1;
				field[h + 2][w].doErase = 1;
				ret = true;
			}
		}
	}
	return ret;
}

void Stage::PrepareFall()
{

	for (int w = 0; w < WIDTH; w++) {
		int hole = 0;
		for (int h = HEIGHT - 1; h >= 0; h--) {//下から見る逆for文
			if (field[h][w].doErase > 0) {
				hole += 1;
			}
			else {
				//field[h][w]をholeの数落下させる
				field[h + hole][w] = field[h][w];//その玉を穴(その列で何個消えたか)の数下に配置する
				field[h + hole][w].by = field[h][w].y;
				field[h + hole][w].bx = field[h][w].x;
				field[h + hole][w].rate = 0.0f;
			}

		}
	}
}