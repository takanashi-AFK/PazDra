#pragma once
#include "Engine/GameObject.h"

class Stage : public GameObject {
public:
	Stage(GameObject* scene);
	~Stage();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
	enum COLOR {
		NOCOLOR = -1,
		RED = 0,
		BLUE,
		/*YELLOW,
		GREEN,
		PURPLE,
		HEART,*/
		NUM
	};


	struct BALLINFO {
		COLOR color;
		float x, y; // �ʂ̕\�����W
#if 0
		int counter; // �ړ�����t���[����
#else
		float bx, by;
		float rate;
#endif
		int doErase; // �����Ώ�
	};


	static const int WIDTH = 6;
	static const int HEIGHT = 5;
	BALLINFO field[HEIGHT][WIDTH];
	int hImage[COLOR::NUM];
	XMFLOAT3 mousePos;
	int selectX, selectY; // �I�΂ꂽ�ꏊ
	COLOR selectColor;

	int eraseTime;//��������

	// dottozahyou kara 3dzahyou ni hennkann
	XMFLOAT3 ConvDrawPos(float x, float y);

	// ��ԁiState�j�J��
	//�@�ʂ�߂܂���̂�҂�
	// 	�ʂ�߂܂�����AMOVE��
	//�A�ړ��E����ւ�(MOVE)
	//	��������B��
	//�B������̂��Ȃ�������D��
	//	�R����ł�̂�������\��������
	//	�����I�������C��
	//�C�󂢂��Ƃ���ɋʂ������鉉�o
	//	��������A�B��
	//�D�U���i�v���C���[����A�G����j���o
	//	�U���I�������@��
	enum STATE {
		S_IDLE,
		S_MOVE,
		S_ERASE,
		S_FALL,
		S_ATTACK,
	};
	STATE state;
	void UpdateIdle();
	void UpdateMove();
	void UpdateErase();
	void UpdateFall();
	void UpdateAttack();

	void CalcMouseSelect();

	/// <summary>
	/// �n�_�ƏI�_�̊Ԃ̒l�����߂�
	/// </summary>
	/// <param name="begin">�n�_</param>
	/// <param name="end">�I�_</param>
	/// <param name="rate">����</param>
	/// <returns>���߂�l</returns>
	float GetRateValue(float begin, float end, float rate);

	/// <summary>
	/// �R���񂾂��̂�T���āAdoErase��1�ɂ���
	/// </summary>
	/// <returns>������̂���������true</returns>
	bool CheckErase();

	/// <summary>
	/// �����鏀��
	/// </summary>
	void PrepareFall();
};
