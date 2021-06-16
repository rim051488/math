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
	SetWindowText("atan2練習サンプル");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//SetDrawMode(DX_DRAWMODE_NEAREST);
	auto arrowcatH = LoadGraph("img/arrowcat.png");

	//猫ちゃんの中心座標
	Position2 nekoPos = { 320.0f,240.0f };
	Position2 bulletPos = {  };		// 弾座標
	Position2 bulletVel = {  };		// 弾速度

	int lastMoustinput = 0;
	while (!ProcessMessage()) {
		ClearDrawScreen();

		auto mpos = GetCurrentMousePosition();
		auto mouseInput = GetMouseInput();
		auto dir = mpos - nekoPos;
		if ((mouseInput & MOUSE_INPUT_LEFT) &&
			!(lastMoustinput & MOUSE_INPUT_LEFT)) {
			//弾の座標及び
			//弾の速度ベクトルの初期化
			//を書いてください
			//①(弾の座標は猫ちゃん座標、引き算で速度ベクトルを作る)
			// ２つの座標からベクトルを作る座標
			// 始点→終点
			//※ただそ、速度ベクトルは正規化したほうがいい
			bulletPos = nekoPos;
			bulletVel = dir;
			bulletVel.Normalize();
			bulletVel *= 5.0f;
		}
		lastMoustinput = mouseInput;
		//弾の速度をもとに、弾の現在座標を更新してください
		//②
		bulletPos += bulletVel;
		//弾の描画
		DrawCircleAA(bulletPos.x, bulletPos.y, 10, 16, 0xffffff);

		auto angle = atan2(dir.y, dir.x);		// atan2(y,x)でyが先に来るようにする
		DrawRotaGraph(nekoPos.x, nekoPos.y, 1.0f, angle, arrowcatH,true);

		ScreenFlip();
	}
	DxLib_End();
}