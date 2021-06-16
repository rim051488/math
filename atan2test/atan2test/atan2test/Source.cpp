#include<DxLib.h>
#include<math.h>
#include "Geometry.h"

Position2 GetCurrentMousePosition() {
	int mx, my;
	GetMousePoint(&mx, &my);
	return Position2(mx, my);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);
	SetWindowText("atan2���K�T���v��");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//SetDrawMode(DX_DRAWMODE_NEAREST);
	auto arrowcatH = LoadGraph("img/arrowcat.png");

	//�L�����̒��S���W
	Position2 nekoPos = { 320.0f,240.0f };
	Position2 bulletPos = {  };		// �e���W
	Position2 bulletVel = {  };		// �e���x

	int lastMoustinput = 0;
	while (!ProcessMessage()) {
		ClearDrawScreen();

		auto mpos = GetCurrentMousePosition();
		auto mouseInput = GetMouseInput();
		auto dir = mpos - nekoPos;
		if ((mouseInput & MOUSE_INPUT_LEFT) &&
			!(lastMoustinput & MOUSE_INPUT_LEFT)) {
			//�e�̍��W�y��
			//�e�̑��x�x�N�g���̏�����
			//�������Ă�������
			//�@(�e�̍��W�͔L�������W�A�����Z�ő��x�x�N�g�������)
			// �Q�̍��W����x�N�g���������W
			// �n�_���I�_
			//���������A���x�x�N�g���͐��K�������ق�������
			bulletPos = nekoPos;
			bulletVel = dir;
			bulletVel.Normalize();
			bulletVel *= 5.0f;
		}
		lastMoustinput = mouseInput;
		//�e�̑��x�����ƂɁA�e�̌��ݍ��W���X�V���Ă�������
		//�A
		bulletPos += bulletVel;
		//�e�̕`��
		DrawCircleAA(bulletPos.x, bulletPos.y, 10, 16, 0xffffff);

		auto angle = atan2(dir.y, dir.x);		// atan2(y,x)��y����ɗ���悤�ɂ���
		DrawRotaGraph(nekoPos.x, nekoPos.y, 1.0f, angle, arrowcatH,true);

		ScreenFlip();
	}
	DxLib_End();
}