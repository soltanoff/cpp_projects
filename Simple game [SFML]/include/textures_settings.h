#ifndef TEXTURE_SETTINGS
#define TEXTURE_SETTINGS



namespace Player_Texture
{
	//const char TEXTURE_NAME[] = "hero.png";
	const short BASE_RECTLEFT_POS = 96;

	// Движение влево MVL
	const short MVL_LEFT_STEP = 6;
	const short MVL_TOP = 136;
	const short MVL_WIDTH = 89;
	const short MVL_HEIGHT = 55;

	// Движение вправо MVR
	const short MVR_LEFT_STEP = 6;
	const short MVR_TOP = 231;
	const short MVR_WIDTH = 89;
	const short MVR_HEIGHT = 55;

	// Движение вверх MVU
	const short MVU_LEFT_STEP = 27;
	const short MVU_TOP = 305;
	const short MVU_WIDTH = 39;
	const short MVU_HEIGHT = 89;

	// Движение вниз MVD
	const short MVD_LEFT_STEP = 27;
	const short MVD_TOP = 6;
	const short MVD_WIDTH = 39;
	const short MVD_HEIGHT = 89;
}

namespace Enemy_Texture
{
	//const char TEXTURE_NAME[] = "enemy.png";

	const int RECT_LEFT = 0;
	const int RECT_TOP = 0;

	const int WIDTH = 32;
	const int HEIGHT = 32;
}

namespace Bullet_Texture
{
	//const char TEXTURE_NAME[] = "enemy.png";

	const int RECT_LEFT = 0;
	const int RECT_TOP = 0;

	const int WIDTH = 16; // 16
	const int HEIGHT = 16; //16
}
#endif /* TEXTURE_SETTINGS */