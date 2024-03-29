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


///とある点を中心に回転して、その回転後の座標を返します。
///@param center 回転中心点
///@param angle 回転角度
///@param pos 元の座標
///@return 変換後の座標
Matrix RotatePosition(const Position2& center, float angle) {
	//�@中心を原点に平行移動して
	//�A原点中心に回転して
	//�B中心を元の座標へ戻す

	Matrix mat = TranslateMat(center.x, center.y) *
		RotateMat(angle) *
		TranslateMat(-center.x, -center.y);
	return mat;
	//これを書き換えて、特定の点を中心に回転を行うようにしてください。
}

float Clamp(float value, float minVal = 0.0f, float maxVal = 1.0f)
{
	return max(min(value, maxVal), minVal);
}

//カプセルと円が当たったか？
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
	SetMainWindowText("2016025_永冨心");
	SetDrawScreen(DX_SCREEN_BACK);

	int sw, sh;//スクリーン幅、高さ
	GetDrawScreenSize(&sw, &sh);

	auto woodH = LoadGraph("img/wood.png");
	int wdW, wdH;
	GetGraphSize(woodH, &wdW, &wdH);
	wdW = 200;
	bool WoodHflag = true;

	Vector2 posA = { 0.0f,0.0f };
	Vector2 posB = { 0.0f,0.0f };

	auto cascadeH = LoadGraph("img/cascade_chip.png");
	auto chipH = LoadGraph("img/atlas0.png");
	auto rockH = LoadGraph("img/rock.png");
	auto marioH = LoadGraph("img/mario.png");
	auto luigiH = LoadGraph("img/luigi.png");
	auto explosionH = LoadGraph("img/explosion.png");

	Capsule cap(20,Position2((sw-wdW)/2,sh-100),Position2((sw - wdW) / 2+wdW,sh-100));
	srand((unsigned int)GetTickCount());
	auto circle = Circle(20, Position2(rand() % 512, 24));

	char keystate[256];
	
	float angle = 0.0f;

	int frame = 0;
	int explosionframe = 0;
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
		//当たり判定を完成させて当たったときの反応を書いてください
		if(IsHit(cap,circle)){
			//反応をここに書いてください。
			posA = cap.posA;
			posB = cap.posB;
			WoodHflag = false;

			cap.posA = Position2((sw - wdW) / 2, sh - 100);
			cap.posB = Position2((sw - wdW) / 2 + wdW, sh - 100);
			circle.pos = Position2(rand() % 512, 24);
		}

		if (cap.posA.x >= posB.x - posA.x)
		{
			posA = cap.posA;
			posB = cap.posB;
			WoodHflag = false;

			cap.posA = Position2((sw - wdW) / 2, sh - 100);
			cap.posB = Position2((sw - wdW) / 2 + wdW, sh - 100);
		}
		if (cap.posB.x <= posB.x - posA.x)
		{
			posA = cap.posA;
			posB = cap.posB;
			WoodHflag = false;

			cap.posA = Position2((sw - wdW) / 2, sh - 100);
			cap.posB = Position2((sw - wdW) / 2 + wdW, sh - 100);
		}
		//カプセル回転
		Matrix rotMat=RotatePosition(Position2(mx, my), angle);
		cap.posA = MultipleVec(rotMat, cap.posA);
		cap.posB = MultipleVec(rotMat, cap.posB);

		//背景の描画
		//滝
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

		DrawRotaGraph(circle.pos.x, circle.pos.y, 2.0f, 0.0f, rockH, true);
		DrawCircle(circle.pos.x, circle.pos.y, circle.radius, 0xff0000, false, 3);
		circle.pos.y += 1.0f;
		if (circle.pos.y >= 850)
		{
			circle.pos.x = rand() % 512;
			circle.pos.y = 24;
		}

		if (WoodHflag)
		{
			DrawWood(cap, woodH);
			DrawCircle(mx, my, 30, 0xff0000, false, 3);
		}
		else
		{
			DrawRotaGraph(posA.x, posA.y, 2.0f, 0.0f, marioH, true);
			DrawRotaGraph(posB.x, posB.y, 2.0f, 0.0f, luigiH, true);
		}
		if (posA.y >= 850 || posB.y >= 850)
		{
			WoodHflag = true;
		}
		else
		{
			posA.y += 5.0f;
			posB.y += 5.0f;
		}
		++frame;
		
		ScreenFlip();
	}

	DxLib_End();


}