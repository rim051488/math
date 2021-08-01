#include<DxLib.h>
#include<cmath>
#include"Geometry.h"


using namespace std;

void DrawWood(const Capsule& cap, int handle) {


	auto v = cap.posB - cap.posA;
	auto angle = atan2(v.y, v.x) + DX_PI_F / 2.0f;
	auto w = cos(angle)*cap.radius;
	auto h = sin(angle)*cap.radius;

	DrawModiGraph(
		cap.posA.x - w, cap.posA.y - h,
		cap.posB.x - w, cap.posB.y - h,
		cap.posB.x + w, cap.posB.y + h,
		cap.posA.x + w, cap.posA.y + h, handle,true);
}


///�Ƃ���_�𒆐S�ɉ�]���āA���̉�]��̍��W��Ԃ��܂��B
///@param center ��]���S�_
///@param angle ��]�p�x
///@param pos ���̍��W
///@return �ϊ���̍��W
Matrix RotatePosition(const Position2& center, float angle) {
	//�@���S�����_�ɕ��s�ړ�����
	//�A���_���S�ɉ�]����
	//�B���S�����̍��W�֖߂�

	Matrix mat = TranslateMat(center.x, center.y) *
		RotateMat(angle) *
		TranslateMat(-center.x, -center.y);
	return mat;
	//��������������āA����̓_�𒆐S�ɉ�]���s���悤�ɂ��Ă��������B
}

float Clamp(float value, float minVal = 0.0f, float maxVal = 1.0f)
{
	return max(min(value, maxVal), minVal);
}

//�J�v�Z���Ɖ~�������������H
bool IsHit(const Capsule& cap, const Circle& cc) {
	auto T = cc.pos - cap.posA;
	auto L = cap.posB - cap.posA;
	auto dot = Dot(T, L);
	auto s = dot / L.SQMagnitude();
	s = Clamp(s);
	auto V = T - L * s;
	return V.Magnitude() <= cap.radius + cc.radius;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(512, 800, 32);
	DxLib_Init();
	SetMainWindowText("2016025_�i�y�S");
	SetDrawScreen(DX_SCREEN_BACK);

	int sw, sh;//�X�N���[�����A����
	GetDrawScreenSize(&sw, &sh);

	auto woodH = LoadGraph("img/wood.png");
	int wdW, wdH;
	GetGraphSize(woodH, &wdW, &wdH);
	wdW = 200;

	auto cascadeH = LoadGraph("img/cascade_chip.png");
	auto chipH = LoadGraph("img/atlas0.png");
	auto rockH = LoadGraph("img/rock.png");

	Capsule cap(20,Position2((sw-wdW)/2,sh-100),Position2((sw - wdW) / 2+wdW,sh-100));
	srand((unsigned int)GetTickCount());
	auto circle = Circle(20, Position2(rand() % 512, 24));

	char keystate[256];
	
	float angle = 0.0f;

	int frame = 0;
	bool isLeft = false;
	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);

		DrawBox(0, 0, sw, sh, 0xaaffff, true);

		int mx = 0, my = 0;

		if (keystate[KEY_INPUT_LEFT]) {
			isLeft = true;
		}
		else if (keystate[KEY_INPUT_RIGHT]) {
			isLeft = false;
		}

		if (isLeft) {
			mx = cap.posA.x;
			my = cap.posA.y;
		}
		else {
			mx = cap.posB.x;
			my = cap.posB.y;
		}

		if (keystate[KEY_INPUT_Z]) {

			angle = -0.05f;
		}
		else if (keystate[KEY_INPUT_X]) {

			angle = 0.05f;
		}
		else {
			angle = 0.0f;
		}

		//�����蔻������������ē��������Ƃ��̔����������Ă�������
		if(IsHit(cap,circle)){
			//�����������ɏ����Ă��������B

		}

		//�J�v�Z����]
		Matrix rotMat=RotatePosition(Position2(mx, my), angle);
		cap.posA = MultipleVec(rotMat, cap.posA);
		cap.posB = MultipleVec(rotMat, cap.posB);

		//�w�i�̕`��
		//��
		int chipIdx = (frame/4) % 3;
		constexpr int dest_chip_size = 32;
		int destY = 48;
		while (destY < sh) {
			int destX = 0;
			while (destX < sw) {
				DrawRectExtendGraph(
					destX, destY, destX + dest_chip_size, destY+dest_chip_size,
					chipIdx * 16, 0,
					16, 16,
					cascadeH, false);
				destX += dest_chip_size;
			}
			destY += dest_chip_size;
		}

		DrawRectGraph(0, 0, 96, 0, 32, 32, chipH, true);
		DrawRectGraph(sw - 32, 0, 32, 0, 32, 32, chipH, true);
		DrawRectGraph(0, 32, 96, 32, 32, 32, chipH, true);
		DrawRectGraph(sw-32, 32, 32, 32, 32, 32, chipH, true);
		destY = 64;
		while (destY < sh) {
			DrawRectGraph(0, destY, 96, 64, 32, 32, chipH, true);
			DrawRectGraph(sw - 32, destY, 32, 64, 32, 32, chipH, true);
			destY += dest_chip_size;
		}

		DrawRotaGraph(circle.pos.x, circle.pos.y,2.0f,0.0f, rockH, true);
		DrawCircle(circle.pos.x, circle.pos.y, circle.radius, 0xff0000, false, 3);
		circle.pos.y += 1.0f;
		DrawWood(cap, woodH);
		DrawCircle(mx, my, 30, 0xff0000, false, 3);
		++frame;
		
		ScreenFlip();
	}

	DxLib_End();


}