#include <cmath>
#include<Dxlib.h>
#include"Geometry.h"

// ‹éŒ`‚Æ‹éŒ`‚Ì“–‚½‚è”»’è

bool IsHit(const Rect& rcA, const Rect& rcB) {
	//‡@’†S“_“¯m‚Ì·•ª‚ğŒvZ‚·‚é
	//‡A‚Ü‚¸X•ûŒü‚Ì·•ª‚ÆA,B‚Ì•‚Ì‡Œv‚ğ”äŠr
	//‡B•‚Ì‡Œv<·•ª‚È‚ç‚Î“–‚½‚Á‚Ä‚È‚¢‚Ì‚Åfalse‚ğ•Ô‚·>
	//‡C¡“x‚ÍY•ûŒü‚Ì·•ª‚ÆA,B‚Ì•‚Ì‡Œv‚ğ”äŠr
	//‡D‚‚Ì‡Œv<·•ª‚È‚ç‚Î“–‚½‚Á‚Ä‚È‚¢‚Ì‚Åfalse‚ğ•Ô‚·>
	//©•ª‚Ì‚â‚è•û
	if (abs(rcB.center.x - rcA.center.x) > rcA.w + rcB.w)
	{
		return false;
	}
	if (abs(rcB.center.y - rcA.center.y) > rcA.h + rcB.h)
	{
		return false;
	}
	// æ¶‚Ì‚â‚è•û
	//auto diffX = abs(rcB.center.x - rcA.center.x);
	//if (diffX > rcA.w + rcB.w) {
	//	return false;
	//}
	//auto diffY = abs(rcB.center.y - rcA.center.y);
	//if (diffY > rcA.h + rcB.h) {
	//	return false;
	//}
	return true;
}

//‰~‚Æ‰~‚Ì“–‚½‚è”»’è‚ğs‚¤

bool IsHit(const Circle& cA, const Circle& cB)
{
	//‡@‰~A‚Æ‰~B‚Ì’†S“_“¯m‚Ì‹——£‚ğ‘ª‚é
	// iƒsƒ^ƒSƒ‰ƒX‚Ì’è—‚Å‘ª‚éj
	//‡A‡@‚Å‹‚ß‚½‹——£‚Æ‰~A‚Ì”¼Œa+‰~B‚Ì”¼Œa‚ğ”äŠr‚·‚é
	//‡B‡@‚Å‹‚ß‚½‹——£<=‡A‚Ì‡Œv‚È‚çtrue‚ğ•Ô‚·
	//	‚»‚¤‚Å‚È‚¢‚È‚çfalse‚ğ•Ô‚·
	//‚à‚µAsqrt‚©hypotŠÖ”‚ğg—p‚µ‚½‚¢ê‡‚Í
	//<cmath>‚ğƒCƒ“ƒNƒ‹[ƒh‚·‚é
	// 
	//auto diffX = abs(cB.center.x - cA.center.x);
	//auto diffY = abs(cB.center.y - cA.center.y);
	//return cA.radius+cB.radius>=hypot(diffX,diffY);

	auto diff = cB.center - cA.center;
	auto totalRadius = cA.radius + cB.radius;
	return totalRadius * totalRadius >= diff.SQMagnitude();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetWindowText(L"‰~‚Ì“–‚½‚è”»’è");

	ChangeWindowMode(true);
	if (DxLib_Init()) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	Circle cA = { {100,100},50 };
	Circle cB = { {400,300 }, 100};

	char keystate[256];
	while (ProcessMessage() == 0) {
		GetHitKeyStateAll(keystate);
		ClearDrawScreen();
		constexpr float speed = 4.0f;
		if (keystate[KEY_INPUT_LEFT]) {
			cA.center.x -= speed;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			cA.center.x += speed;
		}
		if (keystate[KEY_INPUT_UP]) {
			cA.center.y -= speed;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			cA.center.y += speed;
		}

		unsigned int color = 0xffffff;
		if (IsHit(cA, cB))
		{
			color = 0xffaaaa;
			auto diffV = cA.center - cB.center;
			auto distance = diffV.Magnitude();
			auto overlap = cA.radius + cB.radius - distance;
			diffV.Normalize();// ‘å‚«‚³‚ğ‚P‚É‚·‚é
			cA.center += diffV * overlap /2;
			cB.center -= diffV * overlap / 2;
		}

		DrawCircleAA(cA.center.x, cA.center.y, cA.radius, 32, color);
		DrawCircleAA(cB.center.x, cB.center.y, cB.radius, 32, color);

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}