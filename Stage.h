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
	int selectX, selectY;//�I�΂ꂽ�ꏊ

	Text* pText;
	XMFLOAT3 MousePos;


	/// <summary>
	/// �h�b�g���W����3D���W�ɕϊ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <returns></returns>
	XMFLOAT3 ConvDrawPos(float x, float y);

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Stage(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};