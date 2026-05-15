#include "framework.h"
#include "Zombies treasure hunt.h"
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include "treasure.h"
#include "fcheck.h"
#include "ErrH.h"
#include "D2BMPLOADER.h"
#include "gifresizer.h"
#include <chrono>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "treasure.lib")
#pragma comment(lib, "fcheck.lib")
#pragma comment(lib, "errh.lib")
#pragma comment(lib, "d2bmploader.lib")
#pragma comment(lib, "gifresizer.lib")

constexpr wchar_t bWinClassName[]{ L"GhostTr" };
constexpr char tmp_file[]{ ".\\res\\data\\temp.dat" };
constexpr wchar_t Ltmp_file[]{ L".\\res\\data\\temp.dat" };
constexpr wchar_t sound_file[]{ L".\\res\\snd\\main.wav" };
constexpr wchar_t help_file[]{ L".\\res\\data\\help.dat" };
constexpr wchar_t save_file[]{ L".\\res\\data\\save.dat" };
constexpr wchar_t record_file[]{ L".\\res\\data\\record.dat" };

constexpr int mNew{ 1001 };
constexpr int mSpeed{ 1002 };
constexpr int mExit{ 1003 };
constexpr int mSave{ 1004 };
constexpr int mLoad{ 1005 };
constexpr int mHoF{ 1006 };

constexpr int first_record{ 2001 };
constexpr int no_record{ 2002 };
constexpr int record{ 2003 };

WNDCLASS bWinClass{};
HINSTANCE bIns{ nullptr };
HMENU bBar{ nullptr };
HMENU bMain{ nullptr };
HMENU bStore{ nullptr };
HICON mainIco{ nullptr };
HCURSOR mainCur{ nullptr };
HCURSOR outCur{ nullptr };
HWND bHwnd{ nullptr };
HDC PaintDC{ nullptr };
PAINTSTRUCT bPaint{};
MSG bMsg{};
BOOL bRet{ 0 };

POINT cur_pos{};
float x_scale{ 0 };
float y_scale{ 0 };

D2D1_RECT_F b1Rect{ 50.0f, 10.0f, scr_width / 3.0f - 50.0f, 40.0f };
D2D1_RECT_F b2Rect{ scr_width / 3.0f + 50.0f, 10.0f, scr_width * 2.0f / 3.0f - 50.0f, 40.0f };
D2D1_RECT_F b3Rect{ scr_width * 2.0f / 3.0f + 50.0f, 10.0f, scr_width - 50.0f, 40.0f };

D2D1_RECT_F b1TxtRect{ 90.0f, 15.0f, scr_width / 3.0f - 50.0f, 40.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3.0f + 90.0f, 15.0f, scr_width * 2.0f / 3.0f - 50.0f, 40.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 2.0f / 3.0f + 70.0f, 15.0f, scr_width - 50.0f, 40.0f };

D2D1_RECT_F FullScreenRect{ 0, 0, scr_width, scr_height };

wchar_t current_player[16]{ L"TARLYO" };

float speed{ 1.0f };
int score = 0;
int map_pieces = 0;

bool pause = false;
bool sound = true;
bool in_client = true;
bool name_set = false;
bool show_help = false;
bool b1Hglt = false;
bool b2Hglt = false;
bool b3Hglt = false;

bool treasure_found = false;

ID2D1Factory* iFactory{ nullptr };
ID2D1HwndRenderTarget* Draw{ nullptr };

ID2D1RadialGradientBrush* b1Bckg{ nullptr };
ID2D1RadialGradientBrush* b2Bckg{ nullptr };
ID2D1RadialGradientBrush* b3Bckg{ nullptr };

ID2D1SolidColorBrush* statBrush{ nullptr };
ID2D1SolidColorBrush* txtBrush{ nullptr };
ID2D1SolidColorBrush* inactBrush{ nullptr };
ID2D1SolidColorBrush* hgltBrush{ nullptr };

ID2D1SolidColorBrush* grayBrush{ nullptr };

IDWriteFactory* iWriteFactory{ nullptr };
IDWriteTextFormat* nrmText{ nullptr };
IDWriteTextFormat* midText{ nullptr };
IDWriteTextFormat* bigText{ nullptr };

ID2D1Bitmap* bmpIconArmor{ nullptr };
ID2D1Bitmap* bmpIconGold{ nullptr };
ID2D1Bitmap* bmpIconGun{ nullptr };

ID2D1Bitmap* bmpBullet{ nullptr };
ID2D1Bitmap* bmpChest{ nullptr };
ID2D1Bitmap* bmpMap{ nullptr };
ID2D1Bitmap* bmpPotion{ nullptr };

ID2D1Bitmap* bmpDirt{ nullptr };
ID2D1Bitmap* bmpLogo{ nullptr };
ID2D1Bitmap* bmpLoose{ nullptr };
ID2D1Bitmap* bmpMark{ nullptr };
ID2D1Bitmap* bmpMountain1{ nullptr };
ID2D1Bitmap* bmpMountain2{ nullptr };
ID2D1Bitmap* bmpRecord{ nullptr };
ID2D1Bitmap* bmpRIP{ nullptr };
ID2D1Bitmap* bmpTomb{ nullptr };
ID2D1Bitmap* bmpTree1{ nullptr };
ID2D1Bitmap* bmpTree2{ nullptr };
ID2D1Bitmap* bmpTree3{ nullptr };
ID2D1Bitmap* bmpWater{ nullptr };
ID2D1Bitmap* bmpWin{ nullptr };
ID2D1Bitmap* bmpLevelUp{ nullptr };

ID2D1Bitmap* bmpIntro[16]{ nullptr };

ID2D1Bitmap* bmpFlyerL[11]{ nullptr };
ID2D1Bitmap* bmpFlyerR[11]{ nullptr };

ID2D1Bitmap* bmpGirlL[26]{ nullptr };
ID2D1Bitmap* bmpGirlR[26]{ nullptr };

ID2D1Bitmap* bmpSoul[120]{ nullptr };

ID2D1Bitmap* bmpZombieL[4]{ nullptr };
ID2D1Bitmap* bmpZombieR[4]{ nullptr };

ID2D1Bitmap* bmpHeroShootL[47]{ nullptr };
ID2D1Bitmap* bmpHeroShootR[47]{ nullptr };
ID2D1Bitmap* bmpHeroStandL[32]{ nullptr };
ID2D1Bitmap* bmpHeroStandR[32]{ nullptr };
ID2D1Bitmap* bmpHeroWalkL[16]{ nullptr };
ID2D1Bitmap* bmpHeroWalkR[16]{ nullptr };

/////////////////////////////////////////////////////

dll::RANDIT RandIt{};

dll::FIELD* Field{ nullptr };

D2D1_RECT_F TreasureMark{};

dll::HERO* Hero{ nullptr };

dll::NATURE* Mountain{ nullptr };

std::vector<dll::NATURE*> vTrees;

std::vector<D2D1_RECT_F> vTombs;

std::vector<D2D1_RECT_F> vChests;

std::vector<dll::SHOT*>vHeroShots;

std::vector<dll::SHOT*>vEvilShots;

std::vector<dll::EVIL*>vEvils;

dll::BAG<D2D1_RECT_F>ObstBag;

std::vector<FADE>vAssetIcons;

std::vector<D2D1_RECT_F> vMaps;

std::vector<D2D1_RECT_F> vPotions;

/////////////////////////////////////////////////////

template<typename T>concept HasRelease = requires (T check)
{
	check.Release();
};
template<HasRelease T>bool FreeMem(T** var)
{
	if (*var)
	{
		(*var)->Release();
		(*var) = nullptr;
		return true;
	}

	return false;
}
void LogErr(const wchar_t* what)
{
	std::wofstream err(L".\\res\\data\\error.log", std::ios::app);

	err << what << L" Time stamp: " << std::chrono::system_clock::now() << std::endl;

	err.close();
}
void ReleaseResources()
{
	if (!FreeMem(&iFactory))LogErr(L"Error unloading D2D1 iFactory !");
	if (!FreeMem(&Draw))LogErr(L"Error unloading D2D1 Draw !");
	if (!FreeMem(&b1Bckg))LogErr(L"Error unloading D2D1 b1Bckg !");
	if (!FreeMem(&b2Bckg))LogErr(L"Error unloading D2D1 b2Bckg !");
	if (!FreeMem(&b3Bckg))LogErr(L"Error unloading D2D1 b3Bckg !");
	if (!FreeMem(&inactBrush))LogErr(L"Error unloading D2D1 inactBrush !");
	if (!FreeMem(&statBrush))LogErr(L"Error unloading D2D1 statBrush !");
	if (!FreeMem(&txtBrush))LogErr(L"Error unloading D2D1 txtBrush !");
	if (!FreeMem(&hgltBrush))LogErr(L"Error unloading D2D1 hgltBrush !");

	if (!FreeMem(&grayBrush))LogErr(L"Error unloading D2D1 grayBrush !");

	if (!FreeMem(&iWriteFactory))LogErr(L"Error unloading D2D1 iWriteFactory !");
	if (!FreeMem(&nrmText))LogErr(L"Error unloading D2D1 nrmText !");
	if (!FreeMem(&midText))LogErr(L"Error unloading D2D1 midText !");
	if (!FreeMem(&bigText))LogErr(L"Error unloading D2D1 bigText !");

	if (!FreeMem(&bmpIconArmor))LogErr(L"Error unloading D2D1 bmpIconArmor !");
	if (!FreeMem(&bmpIconGold))LogErr(L"Error unloading D2D1 bmpIconGold !");
	if (!FreeMem(&bmpIconGun))LogErr(L"Error unloading D2D1 bmpIconGun !");

	if (!FreeMem(&bmpBullet))LogErr(L"Error unloading D2D1 bmpBullet !");
	if (!FreeMem(&bmpChest))LogErr(L"Error unloading D2D1 bmpChest !");
	if (!FreeMem(&bmpMap))LogErr(L"Error unloading D2D1 bmpMap !");
	if (!FreeMem(&bmpPotion))LogErr(L"Error unloading D2D1 bmpPotion !");

	if (!FreeMem(&bmpDirt))LogErr(L"Error unloading D2D1 bmpDirt !");
	if (!FreeMem(&bmpLogo))LogErr(L"Error unloading D2D1 bmpLogo !");
	if (!FreeMem(&bmpLoose))LogErr(L"Error unloading D2D1 bmpLoose !");
	if (!FreeMem(&bmpMark))LogErr(L"Error unloading D2D1 bmpMark !");
	if (!FreeMem(&bmpMountain1))LogErr(L"Error unloading D2D1 bmpMountain1 !");
	if (!FreeMem(&bmpMountain2))LogErr(L"Error unloading D2D1 bmpMountain2 !");
	if (!FreeMem(&bmpRecord))LogErr(L"Error unloading D2D1 bmpRecord !");
	if (!FreeMem(&bmpRIP))LogErr(L"Error unloading D2D1 bmpRIP !");
	if (!FreeMem(&bmpTomb))LogErr(L"Error unloading D2D1 bmpTomb !");
	if (!FreeMem(&bmpTree1))LogErr(L"Error unloading D2D1 bmpTree1 !");
	if (!FreeMem(&bmpTree2))LogErr(L"Error unloading D2D1 bmpTree2 !");
	if (!FreeMem(&bmpTree3))LogErr(L"Error unloading D2D1 bmpTree3 !");
	if (!FreeMem(&bmpWater))LogErr(L"Error unloading D2D1 bmpWater !");
	if (!FreeMem(&bmpWin))LogErr(L"Error unloading D2D1 bmpWin !");
	if (!FreeMem(&bmpLevelUp))LogErr(L"Error unloading D2D1 bmpLevelUp !");

	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpIntro[i]))LogErr(L"Error unloading D2D1 bmpIntro !");

	for (int i = 0; i < 11; ++i)if (!FreeMem(&bmpFlyerL[i]))LogErr(L"Error unloading D2D1 bmpFlayerL !");
	for (int i = 0; i < 11; ++i)if (!FreeMem(&bmpFlyerR[i]))LogErr(L"Error unloading D2D1 bmpFlayerR !");

	for (int i = 0; i < 26; ++i)if (!FreeMem(&bmpGirlL[i]))LogErr(L"Error unloading D2D1 bmpGirlL !");
	for (int i = 0; i < 26; ++i)if (!FreeMem(&bmpGirlR[i]))LogErr(L"Error unloading D2D1 bmpGirlR !");

	for (int i = 0; i < 120; ++i)if (!FreeMem(&bmpSoul[i]))LogErr(L"Error unloading D2D1 bmpSoul !");

	for (int i = 0; i < 4; ++i)if (!FreeMem(&bmpZombieL[i]))LogErr(L"Error unloading D2D1 bmpZombieL !");
	for (int i = 0; i < 4; ++i)if (!FreeMem(&bmpZombieR[i]))LogErr(L"Error unloading D2D1 bmpZombieR !");

	for (int i = 0; i < 47; ++i)if (!FreeMem(&bmpHeroShootL[i]))LogErr(L"Error unloading D2D1 bmpHeroShootL !");
	for (int i = 0; i < 47; ++i)if (!FreeMem(&bmpHeroShootR[i]))LogErr(L"Error unloading D2D1 bmpHeroShootR !");
	for (int i = 0; i < 32; ++i)if (!FreeMem(&bmpHeroStandL[i]))LogErr(L"Error unloading D2D1 bmpHeroStandL !");
	for (int i = 0; i < 32; ++i)if (!FreeMem(&bmpHeroStandR[i]))LogErr(L"Error unloading D2D1 bmpHeroStandR !");
	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpHeroWalkL[i]))LogErr(L"Error unloading D2D1 bmpHeroWalkL !");
	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpHeroWalkR[i]))LogErr(L"Error unloading D2D1 bmpHeroWalkR !");
}
int IntroFrame()
{
	static int frame_delay{ 4 };
	static int frame{ 0 };

	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = 4;
		++frame;
		if (frame > 15)frame = 0;
	}

	return frame;
}
void ErrExit(int what)
{
	MessageBeep(MB_ICONERROR);
	MessageBox(NULL, ErrHandle(what), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);

	std::remove(tmp_file);
	ReleaseResources();
	
	exit(1);
}

void GameOver()
{
	PlaySound(NULL, NULL, NULL);


	bMsg.message = WM_QUIT;
	bMsg.wParam = 0;
}
void InitGame()
{
	speed = 1.0f;
	score = 0;
	
	wcscpy_s(current_player, L"TARLYO");
	
	treasure_found = false;

	map_pieces = 0;

	ObstBag.clear();

	if (Field)delete Field;
	Field = new dll::FIELD;

	vAssetIcons.clear();

	vMaps.clear();
	vPotions.clear();

	if (Mountain)Mountain->Release();
	
	bool ok{ false };

	while (!ok)
	{
		dll::NATURE* dummy{ dll::NATURE::create(static_cast<nature>(RandIt(3,4)),RandIt(0.0f,scr_width - 100.0f),
			RandIt(sky,ground - 100.0f)) };
		
		ok = true;
		
		for (int row = 0; row < FIELD_ROWS; ++row)
		{
			for (int col = 0; col < FIELD_COLS; ++col)
			{
				if (Field->is_water_tile(row, col))
				{
					if (dll::Intersect(dummy->get_rect(), Field->get_tile_rect(row, col)))
					{
						ok = false;
						break;
					}
				}
			}

			if (!ok)break;
		}

		if (ok)
		{
			Mountain = dummy;
			dummy = nullptr;
		}
		else dummy->Release();
	}

	ok = false;
	
	if (!vTrees.empty())for (int i = 0; i < vTrees.size(); ++i)FreeMem(&vTrees[i]);
	vTrees.clear();
	for (int i = 0; i < 7; ++i)
	{
		ok = false;

		while (!ok)
		{
			dll::NATURE* dummy{ dll::NATURE::create(static_cast<nature>(RandIt(0,2)),RandIt(0.0f,scr_width - 50.0f),
				RandIt(sky,ground - 50.0f)) };

			ok = true;

			for (int row = 0; row < FIELD_ROWS; ++row)
			{
				for (int col = 0; col < FIELD_COLS; ++col)
				{
					if (Field->is_water_tile(row, col))
					{
						if (dll::Intersect(dummy->get_rect(), Field->get_tile_rect(row, col)))
						{
							ok = false;
							break;
						}
					}
				}

				if (!ok)break;
			}

			if (Mountain)
			{
				if (dll::Intersect(Mountain->get_rect(), dummy->get_rect()))ok = false;
			}

			if (ok)vTrees.push_back(dummy);
			else dummy->Release();
		}
	}
	
	ok = false;

	if (Hero)Hero->Release();
	while (!ok)
	{
		dll::HERO* dummy{ dll::HERO::create(RandIt(0.0f, scr_width - 100.f), ground - 100.0f) };

		ok = true;

		for (int row = 0; row < FIELD_ROWS; ++row)
		{
			for (int col = 0; col < FIELD_COLS; ++col)
			{
				if (Field->is_water_tile(row, col))
				{
					if (dll::Intersect(dummy->get_rect(), Field->get_tile_rect(row, col)))
					{
						ok = false;
						break;
					}
				}
			}

			if (!ok)break;
		}

		if (Mountain)
		{
			if (dll::Intersect(dummy->get_rect(), Mountain->get_rect()))ok = false;

		}

		if (!vTrees.empty())
		{
			for (int i = 0; i < vTrees.size(); ++i)
			{
				if (dll::Intersect(dummy->get_rect(), vTrees[i]->get_rect()))
				{
					ok = false;
					break;
				}
			}
		}

		if (ok)
		{
			Hero = dummy;
			dummy = nullptr;
		}
		else dummy->Release();
	}

	ok = false;

	vTombs.clear();
	for (int i = 0; i < 5; ++i)
	{
		ok = false;

		while (!ok)
		{
			float sx = RandIt(0.0f, scr_width - 50.0f);
			float sy = RandIt(sky, ground - 50.0f);

			float ex = sx + 60.0f;
			float ey = sy + 59.0f;

			D2D1_RECT_F dummy{ sx, sy, ex, ey };

			ok = true;

			for (int row = 0; row < FIELD_ROWS; ++row)
			{
				for (int col = 0; col < FIELD_COLS; ++col)
				{
					if (Field->is_water_tile(row, col))
					{
						if (dll::Intersect(dummy, Field->get_tile_rect(row, col)))
						{
							ok = false;
							break;
						}
					}
				}

				if (!ok)break;
			}

			if (Mountain)
			{
				if (dll::Intersect(Mountain->get_rect(), dummy))ok = false;
			}

			if (!vTrees.empty())
			{
				for (int i = 0; i < vTrees.size(); ++i)
				{
					if (dll::Intersect(dummy, vTrees[i]->get_rect()))
					{
						ok = false;
						break;
					}
				}
			}

			if (!vTombs.empty())
			{
				for (int i = 0; i < vTombs.size(); ++i)
				{
					if (dll::Intersect(dummy, vTombs[i]))
					{
						ok = false;
						break;
					}
				}
			}

			if (ok)vTombs.push_back(dummy);
		}
	}

	ok = false;

	vChests.clear();
	for (int i = 0; i < 5; ++i)
	{
		ok = false;

		while (!ok)
		{
			float sx = RandIt(0.0f, scr_width - 50.0f);
			float sy = RandIt(sky, ground - 50.0f);

			float ex = sx + 32.0f;
			float ey = sy + 32.0f;

			D2D1_RECT_F dummy{ sx, sy, ex, ey };

			ok = true;

			for (int row = 0; row < FIELD_ROWS; ++row)
			{
				for (int col = 0; col < FIELD_COLS; ++col)
				{
					if (Field->is_water_tile(row, col))
					{
						if (dll::Intersect(dummy, Field->get_tile_rect(row, col)))
						{
							ok = false;
							break;
						}
					}
				}

				if (!ok)break;
			}

			if (Mountain)
			{
				if (dll::Intersect(Mountain->get_rect(), dummy))ok = false;
			}

			if (!vTrees.empty())
			{
				for (int i = 0; i < vTrees.size(); ++i)
				{
					if (dll::Intersect(dummy, vTrees[i]->get_rect()))
					{
						ok = false;
						break;
					}
				}
			}

			if (!vTombs.empty())
			{
				for (int i = 0; i < vTombs.size(); ++i)
				{
					if (dll::Intersect(dummy, vTombs[i]))
					{
						ok = false;
						break;
					}
				}
			}

			if (!vChests.empty())
			{
				for (int i = 0; i < vChests.size(); ++i)
				{
					if (dll::Intersect(dummy, vChests[i]))
					{
						ok = false;
						break;
					}
				}
			}

			if (ok)vChests.push_back(dummy);
		}
	}

	ok = false;

	while (!ok)
	{
		ok = true;

		float sx = RandIt(0.0f, scr_width - 50.0f);
		float sy = RandIt(sky, ground - 50.0f);

		float ex = sx + 100.0f;
		float ey = sy + 95.0f;

		D2D1_RECT_F dummy{ sx, sy, ex, ey };

		for (int row = 0; row < FIELD_ROWS; ++row)
		{
			for (int col = 0; col < FIELD_COLS; ++col)
			{
				if (Field->is_water_tile(row, col))
				{
					if (dll::Intersect(dummy, Field->get_tile_rect(row, col)))
					{
						ok = false;
						break;
					}
				}
			}

			if (!ok)break;
		}

		if (Mountain)
		{
			if (dll::Intersect(Mountain->get_rect(), dummy))ok = false;
		}

		if (!vTrees.empty())
		{
			for (int i = 0; i < vTrees.size(); ++i)
			{
				if (dll::Intersect(dummy, vTrees[i]->get_rect()))
				{
					ok = false;
					break;
				}
			}
		}

		if (!vTombs.empty())
		{
			for (int i = 0; i < vTombs.size(); ++i)
			{
				if (dll::Intersect(dummy, vTombs[i]))
				{
					ok = false;
					break;
				}
			}
		}

		if (ok)TreasureMark = dummy;
	}

	if (!vHeroShots.empty())for (int i = 0; i < vHeroShots.size(); ++i)FreeMem(&vHeroShots[i]);
	vHeroShots.clear();

	if (!vEvilShots.empty())for (int i = 0; i < vEvilShots.size(); ++i)FreeMem(&vEvilShots[i]);
	vEvilShots.clear();

	if (!vEvils.empty())for (int i = 0; i < vEvils.size(); ++i)FreeMem(&vEvils[i]);
	vEvils.clear();

	// Obstacles BAG /////////////////////////////////////////////////

	for (int row = 0; row < FIELD_ROWS; ++row)
	{
		for (int col = 0; col < FIELD_COLS; ++col)
		{
			if (Field->is_water_tile(row, col))ObstBag.push_back(Field->get_tile_rect(row, col));
		}
	}

	ObstBag.push_back(Mountain->get_rect());

	for (int i = 0; i < vTrees.size(); ++i)ObstBag.push_back(vTrees[i]->get_rect());
}
void LevelUp()
{
	Draw->BeginDraw();
	Draw->DrawBitmap(bmpLevelUp, FullScreenRect);
	Draw->EndDraw();
	if (sound)
	{
		PlaySound(NULL, NULL, NULL);
		PlaySound(L".\\res\\snd\\levelup.wav", NULL, SND_SYNC);
		PlaySound(sound_file, NULL, SND_ASYNC | SND_LOOP);
	}
	else Sleep(4000);

	score += 100 * (int)(speed);
	++speed;
	
	treasure_found = false;

	map_pieces = 0;

	ObstBag.clear();

	if (Field)delete Field;
	Field = new dll::FIELD;

	vAssetIcons.clear();

	vMaps.clear();
	vPotions.clear();

	if (Mountain)Mountain->Release();

	bool ok{ false };

	while (!ok)
	{
		dll::NATURE* dummy{ dll::NATURE::create(static_cast<nature>(RandIt(3,4)),RandIt(0.0f,scr_width - 100.0f),
			RandIt(sky,ground - 100.0f)) };

		ok = true;

		for (int row = 0; row < FIELD_ROWS; ++row)
		{
			for (int col = 0; col < FIELD_COLS; ++col)
			{
				if (Field->is_water_tile(row, col))
				{
					if (dll::Intersect(dummy->get_rect(), Field->get_tile_rect(row, col)))
					{
						ok = false;
						break;
					}
				}
			}

			if (!ok)break;
		}

		if (ok)
		{
			Mountain = dummy;
			dummy = nullptr;
		}
		else dummy->Release();
	}

	ok = false;

	if (!vTrees.empty())for (int i = 0; i < vTrees.size(); ++i)FreeMem(&vTrees[i]);
	vTrees.clear();
	for (int i = 0; i < 7; ++i)
	{
		ok = false;

		while (!ok)
		{
			dll::NATURE* dummy{ dll::NATURE::create(static_cast<nature>(RandIt(0,2)),RandIt(0.0f,scr_width - 50.0f),
				RandIt(sky,ground - 50.0f)) };

			ok = true;

			for (int row = 0; row < FIELD_ROWS; ++row)
			{
				for (int col = 0; col < FIELD_COLS; ++col)
				{
					if (Field->is_water_tile(row, col))
					{
						if (dll::Intersect(dummy->get_rect(), Field->get_tile_rect(row, col)))
						{
							ok = false;
							break;
						}
					}
				}

				if (!ok)break;
			}

			if (Mountain)
			{
				if (dll::Intersect(Mountain->get_rect(), dummy->get_rect()))ok = false;
			}

			if (ok)vTrees.push_back(dummy);
			else dummy->Release();
		}
	}

	ok = false;

	if (Hero)Hero->Release();
	while (!ok)
	{
		dll::HERO* dummy{ dll::HERO::create(RandIt(0.0f, scr_width - 100.f), ground - 100.0f) };

		ok = true;

		for (int row = 0; row < FIELD_ROWS; ++row)
		{
			for (int col = 0; col < FIELD_COLS; ++col)
			{
				if (Field->is_water_tile(row, col))
				{
					if (dll::Intersect(dummy->get_rect(), Field->get_tile_rect(row, col)))
					{
						ok = false;
						break;
					}
				}
			}

			if (!ok)break;
		}

		if (Mountain)
		{
			if (dll::Intersect(dummy->get_rect(), Mountain->get_rect()))ok = false;

		}

		if (!vTrees.empty())
		{
			for (int i = 0; i < vTrees.size(); ++i)
			{
				if (dll::Intersect(dummy->get_rect(), vTrees[i]->get_rect()))
				{
					ok = false;
					break;
				}
			}
		}

		if (ok)
		{
			Hero = dummy;
			dummy = nullptr;
		}
		else dummy->Release();
	}

	ok = false;

	vTombs.clear();
	for (int i = 0; i < 5; ++i)
	{
		ok = false;

		while (!ok)
		{
			float sx = RandIt(0.0f, scr_width - 50.0f);
			float sy = RandIt(sky, ground - 50.0f);

			float ex = sx + 60.0f;
			float ey = sy + 59.0f;

			D2D1_RECT_F dummy{ sx, sy, ex, ey };

			ok = true;

			for (int row = 0; row < FIELD_ROWS; ++row)
			{
				for (int col = 0; col < FIELD_COLS; ++col)
				{
					if (Field->is_water_tile(row, col))
					{
						if (dll::Intersect(dummy, Field->get_tile_rect(row, col)))
						{
							ok = false;
							break;
						}
					}
				}

				if (!ok)break;
			}

			if (Mountain)
			{
				if (dll::Intersect(Mountain->get_rect(), dummy))ok = false;
			}

			if (!vTrees.empty())
			{
				for (int i = 0; i < vTrees.size(); ++i)
				{
					if (dll::Intersect(dummy, vTrees[i]->get_rect()))
					{
						ok = false;
						break;
					}
				}
			}

			if (!vTombs.empty())
			{
				for (int i = 0; i < vTombs.size(); ++i)
				{
					if (dll::Intersect(dummy, vTombs[i]))
					{
						ok = false;
						break;
					}
				}
			}

			if (ok)vTombs.push_back(dummy);
		}
	}

	ok = false;

	vChests.clear();
	for (int i = 0; i < 5; ++i)
	{
		ok = false;

		while (!ok)
		{
			float sx = RandIt(0.0f, scr_width - 50.0f);
			float sy = RandIt(sky, ground - 50.0f);

			float ex = sx + 32.0f;
			float ey = sy + 32.0f;

			D2D1_RECT_F dummy{ sx, sy, ex, ey };

			ok = true;

			for (int row = 0; row < FIELD_ROWS; ++row)
			{
				for (int col = 0; col < FIELD_COLS; ++col)
				{
					if (Field->is_water_tile(row, col))
					{
						if (dll::Intersect(dummy, Field->get_tile_rect(row, col)))
						{
							ok = false;
							break;
						}
					}
				}

				if (!ok)break;
			}

			if (Mountain)
			{
				if (dll::Intersect(Mountain->get_rect(), dummy))ok = false;
			}

			if (!vTrees.empty())
			{
				for (int i = 0; i < vTrees.size(); ++i)
				{
					if (dll::Intersect(dummy, vTrees[i]->get_rect()))
					{
						ok = false;
						break;
					}
				}
			}

			if (!vTombs.empty())
			{
				for (int i = 0; i < vTombs.size(); ++i)
				{
					if (dll::Intersect(dummy, vTombs[i]))
					{
						ok = false;
						break;
					}
				}
			}

			if (!vChests.empty())
			{
				for (int i = 0; i < vChests.size(); ++i)
				{
					if (dll::Intersect(dummy, vChests[i]))
					{
						ok = false;
						break;
					}
				}
			}

			if (ok)vChests.push_back(dummy);
		}
	}

	ok = false;

	while (!ok)
	{
		ok = true;

		float sx = RandIt(0.0f, scr_width - 50.0f);
		float sy = RandIt(sky, ground - 50.0f);

		float ex = sx + 100.0f;
		float ey = sy + 95.0f;

		D2D1_RECT_F dummy{ sx, sy, ex, ey };

		for (int row = 0; row < FIELD_ROWS; ++row)
		{
			for (int col = 0; col < FIELD_COLS; ++col)
			{
				if (Field->is_water_tile(row, col))
				{
					if (dll::Intersect(dummy, Field->get_tile_rect(row, col)))
					{
						ok = false;
						break;
					}
				}
			}

			if (!ok)break;
		}

		if (Mountain)
		{
			if (dll::Intersect(Mountain->get_rect(), dummy))ok = false;
		}

		if (!vTrees.empty())
		{
			for (int i = 0; i < vTrees.size(); ++i)
			{
				if (dll::Intersect(dummy, vTrees[i]->get_rect()))
				{
					ok = false;
					break;
				}
			}
		}

		if (!vTombs.empty())
		{
			for (int i = 0; i < vTombs.size(); ++i)
			{
				if (dll::Intersect(dummy, vTombs[i]))
				{
					ok = false;
					break;
				}
			}
		}

		if (ok)TreasureMark = dummy;
	}

	if (!vHeroShots.empty())for (int i = 0; i < vHeroShots.size(); ++i)FreeMem(&vHeroShots[i]);
	vHeroShots.clear();

	if (!vEvilShots.empty())for (int i = 0; i < vEvilShots.size(); ++i)FreeMem(&vEvilShots[i]);
	vEvilShots.clear();

	if (!vEvils.empty())for (int i = 0; i < vEvils.size(); ++i)FreeMem(&vEvils[i]);
	vEvils.clear();

	// Obstacles BAG /////////////////////////////////////////////////

	for (int row = 0; row < FIELD_ROWS; ++row)
	{
		for (int col = 0; col < FIELD_COLS; ++col)
		{
			if (Field->is_water_tile(row, col))ObstBag.push_back(Field->get_tile_rect(row, col));
		}
	}

	ObstBag.push_back(Mountain->get_rect());

	for (int i = 0; i < vTrees.size(); ++i)ObstBag.push_back(vTrees[i]->get_rect());
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_INITDIALOG:
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(mainIco));
		return true;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;

	case WM_COMMAND:
		if (GetDlgItemText(hwnd, IDC_NAME, current_player, 16) < 1)
		{
			wcscpy_s(current_player, L"TARLYO");
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името !", L"Забраватор !", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		EndDialog(hwnd, IDOK);
		break;
	}

	return (INT_PTR)(FALSE);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_CREATE:
		if (bIns)
		{
			bBar = CreateMenu();
			bMain = CreateMenu();
			bStore = CreateMenu();

			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bMain), L"Основно меню");
			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bStore), L"Меню за данни");

			AppendMenu(bMain, MF_STRING, mNew, L"Нова игра");
			AppendMenu(bMain, MF_STRING, mSpeed, L"Турбо режим");
			AppendMenu(bMain, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bMain, MF_STRING, mExit, L"Изход");

			AppendMenu(bStore, MF_STRING, mSave, L"Запази игра");
			AppendMenu(bStore, MF_STRING, mLoad, L"Зареди игра");
			AppendMenu(bStore, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bStore, MF_STRING, mHoF, L"Зала на славата");

			SetMenu(hwnd, bBar);

			InitGame();
		}
		break;

	case WM_CLOSE:
		pause = true;
		if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
		if (MessageBox(hwnd, L"Ако излезеш, губиш играта !\n\n Наистина ли излизаш ?", L"Изход !",
			MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION) == IDNO)
		{
			pause = false;
			break;
		}
		GameOver();
		break;

	case WM_PAINT:
		PaintDC = BeginPaint(hwnd, &bPaint);
		FillRect(PaintDC, &bPaint.rcPaint, CreateSolidBrush(RGB(10, 10, 10)));
		EndPaint(hwnd, &bPaint);
		break;

	case WM_SETCURSOR:
		GetCursorPos(&cur_pos);
		ScreenToClient(hwnd, &cur_pos);
		if (LOWORD(lParam) == HTCLIENT)
		{
			if (!in_client)
			{
				in_client = true;
				pause = false;
			}

			if (cur_pos.y * y_scale <= 50)
			{
				if (cur_pos.x * x_scale >= b1Rect.left && cur_pos.x * x_scale <= b1Rect.right)
				{
					if (!b1Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = true;
						b2Hglt = false;
						b3Hglt = false;
					}
				}
				else if (cur_pos.x * x_scale >= b2Rect.left && cur_pos.x * x_scale <= b2Rect.right)
				{
					if (!b2Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = true;
						b3Hglt = false;
					}
				}
				else if (cur_pos.x * x_scale >= b3Rect.left && cur_pos.x * x_scale <= b3Rect.right)
				{
					if (!b3Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = false;
						b3Hglt = true;
					}
				}
				else if (b1Hglt || b2Hglt || b3Hglt)
				{
					if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
					b1Hglt = false;
					b2Hglt = false;
					b3Hglt = false;
				}

				SetCursor(outCur);

				return true;
			}
			else if (b1Hglt || b2Hglt || b3Hglt)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
				b1Hglt = false;
				b2Hglt = false;
				b3Hglt = false;
			}

			SetCursor(mainCur);

			return true;
		}
		else
		{
			if (in_client)
			{
				in_client = false;
				pause = true;
			}

			if (b1Hglt || b2Hglt || b3Hglt)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
				b1Hglt = false;
				b2Hglt = false;
				b3Hglt = false;
			}

			SetCursor(LoadCursor(NULL, IDC_ARROW));

			return true;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case mNew:
			pause = true;
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			if (MessageBox(hwnd, L"Ако рестартираш, губиш играта !\n\nНаистина ли рестартираш ?", L"Рестарт !",
				MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION) == IDNO)
			{
				pause = false;
				break;
			}
			InitGame();
			break;

		case mSpeed:
			pause = true;
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			if (MessageBox(hwnd, L"Готов ли си за турбо скорост ?", L"Турбо режим !",
				MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION) == IDNO)
			{
				pause = false;
				break;
			}
			speed++;
			break;

		case mExit:
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			break;

		}
		break;

	case WM_LBUTTONDOWN:
		if (Hero)
		{
			vHeroShots.push_back(dll::SHOT::create(Hero->center.x, Hero->center.y, (float)(LOWORD(lParam)* x_scale),
				(float)(HIWORD(lParam)* x_scale), Hero->damage));
			if (sound)mciSendString(L"play .\\res\\snd\\shoot.wav", NULL, NULL, NULL);
		}
		break;

	case WM_RBUTTONDOWN:
		if (Hero)
		{
			Hero->set_path((float)(LOWORD(lParam)* x_scale), (float)(HIWORD(lParam)* y_scale));
			Hero->set_action(action::walk);
		}
		break;

	default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
	}

	return (LRESULT)(FALSE);
}

void CreateResources()
{
	int result = 0;
	CheckFile(Ltmp_file, &result);
	if (result == FILE_EXIST)ErrExit(eStarted);
	else
	{
		std::wofstream start(Ltmp_file);
		start << L"Game started at: " << std::chrono::system_clock::now();
		start.close();
	}

	int win_x = (int)(GetSystemMetrics(SM_CXSCREEN) / 2 - (int)(scr_width / 2.0f));
	int win_y = 10;

	if (GetSystemMetrics(SM_CXSCREEN) < win_x + (int)(scr_width) || GetSystemMetrics(SM_CYSCREEN) < win_y + (int)(scr_height))
		ErrExit(eScreen);

	mainIco = (HICON)(LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 255, 255, LR_LOADFROMFILE));
	if (!mainIco)ErrExit(eIcon);
	mainCur = LoadCursorFromFileW(L".\\res\\main.ani");
	outCur = LoadCursorFromFileW(L".\\res\\out.ani");
	if (!mainCur || !outCur)ErrExit(eCursor);

	bWinClass.lpszClassName = bWinClassName;
	bWinClass.hInstance = bIns;
	bWinClass.lpfnWndProc = &WinProc;
	bWinClass.hbrBackground = CreateSolidBrush(RGB(10, 10, 10));
	bWinClass.hIcon = mainIco;
	bWinClass.hCursor = mainCur;
	bWinClass.style = CS_DROPSHADOW;

	if (!RegisterClass(&bWinClass))ErrExit(eClass);

	bHwnd = CreateWindow(bWinClassName, L"TREASURE OF THE ZOMBIE KING", WS_CAPTION | WS_SYSMENU, win_x, win_y, (int)(scr_width),
		(int)(scr_height), NULL, NULL, bIns, NULL);
	
	if (!bHwnd)ErrExit(eWindow);
	else
	{
		ShowWindow(bHwnd, SW_SHOWDEFAULT);

		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 main Factory !");
			ErrExit(eD2D);
		}

		if (iFactory)hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(bHwnd,
			D2D1::SizeU((UINT32)(scr_width), (UINT32)(scr_height))), &Draw);
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 HwndRenderTarget !");
			ErrExit(eD2D);
		}
		
		if (Draw)
		{
			RECT ClR{};
			GetClientRect(bHwnd, &ClR);

			D2D1_SIZE_F DIPRect{ Draw->GetSize() };

			x_scale = DIPRect.width / (ClR.right - ClR.left);
			y_scale = DIPRect.height / (ClR.bottom - ClR.top);

			D2D1_GRADIENT_STOP gStops[2]{};
			ID2D1GradientStopCollection* gColl{ nullptr };

			gStops[0].position = 0;
			gStops[0].color = D2D1::ColorF(D2D1::ColorF::MediumOrchid);
			gStops[1].position = 1.0f;
			gStops[1].color = D2D1::ColorF(D2D1::ColorF::Maroon);

			hr = Draw->CreateGradientStopCollection(gStops, 2, &gColl);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 RadialGradientStopCollection for butBckg !");
				ErrExit(eD2D);
			}
			if (gColl)
			{
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b1Rect.left +
					(b1Rect.right - b1Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b1Rect.right - b1Rect.left) / 2.0f, 25.0f),
					gColl, &b1Bckg);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b2Rect.left +
					(b2Rect.right - b2Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b2Rect.right - b2Rect.left) / 2.0f, 25.0f),
					gColl, &b2Bckg);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b3Rect.left +
					(b3Rect.right - b3Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b3Rect.right - b3Rect.left) / 2.0f, 25.0f),
					gColl, &b3Bckg);
				if (hr != S_OK)
				{
					LogErr(L"Error creating D2D1 RadialGradientBrushes for butBckg !");
					ErrExit(eD2D);
				}
				FreeMem(&gColl);
			}

			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CadetBlue), &statBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Lime), &txtBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gold), &hgltBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::PowderBlue), &inactBrush);

			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &grayBrush); 

			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 Text Bushes !");
				ErrExit(eD2D);
			}

			bmpIconArmor = Load(L".\\res\\img\\assets\\icons\\armor.png", Draw);
			if (!bmpIconArmor)
			{
				LogErr(L"Error loading bmpIconArmor !");
				ErrExit(eD2D);
			}
			bmpIconGold = Load(L".\\res\\img\\assets\\icons\\gold.png", Draw);
			if (!bmpIconGold)
			{
				LogErr(L"Error loading bmpIconGold !");
				ErrExit(eD2D);
			}
			bmpIconGun = Load(L".\\res\\img\\assets\\icons\\gun.png", Draw);
			if (!bmpIconGun)
			{
				LogErr(L"Error loading bmpIconGun !");
				ErrExit(eD2D);
			}

			bmpBullet = Load(L".\\res\\img\\assets\\bullet.png", Draw);
			if (!bmpBullet)
			{
				LogErr(L"Error loading bmpBullet !");
				ErrExit(eD2D);
			}
			bmpChest = Load(L".\\res\\img\\assets\\chest.png", Draw);
			if (!bmpChest)
			{
				LogErr(L"Error loading bmpChest !");
				ErrExit(eD2D);
			}
			bmpMap = Load(L".\\res\\img\\assets\\map.png", Draw);
			if (!bmpMap)
			{
				LogErr(L"Error loading bmpMap !");
				ErrExit(eD2D);
			}
			bmpPotion = Load(L".\\res\\img\\assets\\potion.png", Draw);
			if (!bmpPotion)
			{
				LogErr(L"Error loading bmpPotion !");
				ErrExit(eD2D);
			}

			bmpDirt = Load(L".\\res\\img\\field\\dirt.png", Draw);
			if (!bmpDirt)
			{
				LogErr(L"Error loading bmpDirt !");
				ErrExit(eD2D);
			}
			bmpLogo = Load(L".\\res\\img\\field\\logo.png", Draw);
			if (!bmpLogo)
			{
				LogErr(L"Error loading bmpLogo !");
				ErrExit(eD2D);
			}
			bmpLoose = Load(L".\\res\\img\\field\\loose.png", Draw);
			if (!bmpLoose)
			{
				LogErr(L"Error loading bmpLoose !");
				ErrExit(eD2D);
			}
			bmpMark = Load(L".\\res\\img\\field\\mark.png", Draw);
			if (!bmpMark)
			{
				LogErr(L"Error loading bmpMark !");
				ErrExit(eD2D);
			}
			bmpMountain1 = Load(L".\\res\\img\\field\\Mountain1.png", Draw);
			if (!bmpMountain1)
			{
				LogErr(L"Error loading bmpMountain1 !");
				ErrExit(eD2D);
			}
			bmpMountain2 = Load(L".\\res\\img\\field\\Mountain2.png", Draw);
			if (!bmpMountain2)
			{
				LogErr(L"Error loading bmpMountain2 !");
				ErrExit(eD2D);
			}
			bmpRecord = Load(L".\\res\\img\\field\\Record.png", Draw);
			if (!bmpRecord)
			{
				LogErr(L"Error loading bmpRecord !");
				ErrExit(eD2D);
			}
			bmpRIP = Load(L".\\res\\img\\field\\RIP.png", Draw);
			if (!bmpRIP)
			{
				LogErr(L"Error loading bmpRIP !");
				ErrExit(eD2D);
			}
			bmpTomb = Load(L".\\res\\img\\field\\Tomb.png", Draw);
			if (!bmpTomb)
			{
				LogErr(L"Error loading bmpTomb !");
				ErrExit(eD2D);
			}
			bmpTree1 = Load(L".\\res\\img\\field\\Tree1.png", Draw);
			if (!bmpTree1)
			{
				LogErr(L"Error loading bmpTree1 !");
				ErrExit(eD2D);
			}
			bmpTree2 = Load(L".\\res\\img\\field\\Tree2.png", Draw);
			if (!bmpTree2)
			{
				LogErr(L"Error loading bmpTree2 !");
				ErrExit(eD2D);
			}
			bmpTree3 = Load(L".\\res\\img\\field\\Tree3.png", Draw);
			if (!bmpTree3)
			{
				LogErr(L"Error loading bmpTree3 !");
				ErrExit(eD2D);
			}
			bmpWater = Load(L".\\res\\img\\field\\water.png", Draw);
			if (!bmpWater)
			{
				LogErr(L"Error loading bmpWater !");
				ErrExit(eD2D);
			}
			bmpWin = Load(L".\\res\\img\\field\\Win.png", Draw);
			if (!bmpWin)
			{
				LogErr(L"Error loading bmpWin !");
				ErrExit(eD2D);
			}
			bmpLevelUp = Load(L".\\res\\img\\field\\levelup.png", Draw);
			if (!bmpLevelUp)
			{
				LogErr(L"Error loading bmplevelup !");
				ErrExit(eD2D);
			}

			for (int i = 0; i < 16; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\intro\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpIntro[i] = Load(name, Draw);
				if (!bmpIntro[i])
				{
					LogErr(L"Error loading bmpIntro !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 11; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\evils\\flyer\\l\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpFlyerL[i] = Load(name, Draw);
				if (!bmpFlyerL[i])
				{
					LogErr(L"Error loading bmpFlyerL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 11; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\evils\\flyer\\r\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpFlyerR[i] = Load(name, Draw);
				if (!bmpFlyerR[i])
				{
					LogErr(L"Error loading bmpFlyerR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 26; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\evils\\girl\\l\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpGirlL[i] = Load(name, Draw);
				if (!bmpGirlL[i])
				{
					LogErr(L"Error loading bmpGirlL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 26; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\evils\\girl\\r\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpGirlR[i] = Load(name, Draw);
				if (!bmpGirlR[i])
				{
					LogErr(L"Error loading bmpGirlR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 120; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\evils\\soul\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"00");
				else if (i < 100)wcscat_s(name, L"0");

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpSoul[i] = Load(name, Draw);
				if (!bmpSoul[i])
				{
					LogErr(L"Error loading bmpSoul !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 4; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\evils\\zombie\\l\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpZombieL[i] = Load(name, Draw);
				if (!bmpZombieL[i])
				{
					LogErr(L"Error loading bmpZombieL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 4; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\evils\\zombie\\r\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpZombieR[i] = Load(name, Draw);
				if (!bmpZombieR[i])
				{
					LogErr(L"Error loading bmpZombieR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 47; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\hero\\shoot\\l\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");
				
				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroShootL[i] = Load(name, Draw);
				if (!bmpHeroShootL[i])
				{
					LogErr(L"Error loading bmpHeroShootL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 47; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\hero\\shoot\\r\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroShootR[i] = Load(name, Draw);
				if (!bmpHeroShootR[i])
				{
					LogErr(L"Error loading bmpHeroShootR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 32; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\hero\\stand\\l\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroStandL[i] = Load(name, Draw);
				if (!bmpHeroStandL[i])
				{
					LogErr(L"Error loading bmpHeroStandL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 32; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\hero\\stand\\r\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroStandR[i] = Load(name, Draw);
				if (!bmpHeroStandR[i])
				{
					LogErr(L"Error loading bmpHeroStandR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 16; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\hero\\walk\\l\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroWalkL[i] = Load(name, Draw);
				if (!bmpHeroWalkL[i])
				{
					LogErr(L"Error loading bmpHeroWalkL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\hero\\walk\\r\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");

				wsprintf(add, L"%d", i);

				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroWalkR[i] = Load(name, Draw);
				if (!bmpHeroWalkR[i])
				{
					LogErr(L"Error loading bmpHeroWalkR !");
					ErrExit(eD2D);
				}
			}
		}

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), 
			reinterpret_cast<IUnknown**>(&iWriteFactory));
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 Write Factory !");
			ErrExit(eD2D);
		}

		if (iWriteFactory)
		{
			hr = iWriteFactory->CreateTextFormat(L"Copperplate Gothic", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
				DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"", &nrmText);
			hr = iWriteFactory->CreateTextFormat(L"Copperplate Gothic", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
				DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 24.0f, L"", &midText);
			hr = iWriteFactory->CreateTextFormat(L"Copperplate Gothic", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
				DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"", &bigText);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 Write text formats !");
				ErrExit(eD2D);
			}
		}
	}

	PlaySound(L".\\res\\snd\\intro.wav", NULL, SND_ASYNC);
	for (int i = 0; i < 250; ++i)
	{
		Draw->BeginDraw();
		Draw->DrawBitmap(bmpIntro[IntroFrame()], FullScreenRect);
		Draw->DrawBitmap(bmpLogo, FullScreenRect);
		Draw->EndDraw();
	}
	Draw->BeginDraw();
	Draw->DrawBitmap(bmpIntro[IntroFrame()], FullScreenRect);
	Draw->DrawBitmap(bmpLogo, FullScreenRect);
	Draw->EndDraw();
	PlaySound(L".\\res\\snd\\boom.wav", NULL, SND_SYNC);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	bIns = hInstance;
	if (!bIns)
	{
		LogErr(L"Error obtaining hInstance from Windows !");
		ErrExit(eClass);
	}

	CreateResources();

	while (bMsg.message != WM_QUIT)
	{
		if ((bRet = PeekMessage(&bMsg, NULL, NULL, NULL, PM_REMOVE)) != 0)
		{
			if (bRet == -1)ErrExit(eMsg);
			TranslateMessage(&bMsg);
			DispatchMessage(&bMsg);
		}

		if (pause)
		{
			if (show_help)continue;
			Draw->BeginDraw();
			Draw->DrawBitmap(bmpIntro[IntroFrame()], FullScreenRect);
			if (txtBrush && bigText)Draw->DrawTextW(L"ПАУЗА", 6, bigText, D2D1::RectF(scr_width / 2.0f - 100.0f,
				scr_height / 2.0f - 50.0f, scr_width, scr_height), txtBrush);
			Draw->EndDraw();
			continue;
		}

	/////////////////////////////////////////////////////////////////

		if (Hero)
		{
			if (Hero->get_action() == action::walk)
			{
				float tx = Hero->start.x;
				float ty = Hero->start.y;

				if (!Hero->move(speed))Hero->set_action(action::stand);
				else
				{
					if (Mountain)
					{
						if (dll::Intersect(Mountain->get_rect(), Hero->get_rect()))
						{
							Hero->start.x = tx;
							Hero->start.y = ty;
							Hero->set_edges();
							Hero->set_action(action::stand);
						}
					}

					for (int row = 0; row < FIELD_ROWS; ++row)
					{
						for (int col = 0; col < FIELD_COLS; ++col)
						{
							if (Field->is_water_tile(row, col))
							{
								if (dll::Intersect(Field->get_tile_rect(row, col), Hero->get_rect()))
								{
									Hero->start.x = tx;
									Hero->start.y = ty;
									Hero->set_edges();
									Hero->set_action(action::stand);
									break;
								}
							}
						}

						if (Hero->get_action() == action::stand)break;
					}

					if (!vTrees.empty())
					{
						for (int i = 0; i < vTrees.size(); ++i)
						{
							if (dll::Intersect(Hero->get_rect(), vTrees[i]->get_rect()))
							{
								Hero->start.x = tx;
								Hero->start.y = ty;
								Hero->set_edges();
								Hero->set_action(action::stand);
								break;
							}
						}
					}
				}
			}
		}

		if (!vHeroShots.empty())
		{
			for (int i = 0; i < vHeroShots.size(); ++i)
			{
				if (!vHeroShots[i]->move(speed))
				{
					vHeroShots[i]->Release();
					vHeroShots.erase(vHeroShots.begin() + i);
					break;
				}
			}
		}

		if (!vTombs.empty() && RandIt(0, 200) == 66)
		{
			int rand_tomb{ RandIt(0, (int)(vTombs.size() - 1)) };

			float tex{ 0 };
			float tey{ ground - 100.0f };

			if (vTombs[rand_tomb].left < scr_width / 2.0f)tex = scr_width - 100.0f;
			if (vTombs[rand_tomb].top > scr_height / 2.0f)tey = sky;
			
			vEvils.push_back(dll::EVIL::create(static_cast<moveables>(RandIt(0, 3)), vTombs[rand_tomb].left + 30.0f,
				vTombs[rand_tomb].top + 30.0f, tex, tey));

			if (sound)(mciSendString(L"play .\\res\\snd\\evil_born.wav", NULL, NULL, NULL));

		}

		if (!vEvils.empty() && Hero)
		{
			for (std::vector<dll::EVIL*>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
			{
				dll::BAG<FPOINT>ChestBag;

				if (!vChests.empty())
				{
					for (int i = 0; i < vChests.size(); ++i)
						ChestBag.push_back(FPOINT{ vChests[i].left + 16.0f, vChests[i].top + 16.0f });
				}

				action doing = dll::AIMove(*(*evil), ObstBag, ChestBag, Hero->center);

				if (doing != action::shoot && doing != action::stand && doing != action::patrol)
				{
					if (doing == action::bumped)(*evil)->move(speed);
					else if (!(*evil)->move(speed))
					{
						if ((*evil)->start.y <= sky)
						{
							if ((*evil)->start.x <= 0)(*evil)->set_path(scr_width, ground);
							else if ((*evil)->end.x >= scr_width)(*evil)->set_path(0, ground);
							else (*evil)->set_path((*evil)->center.x, ground);
						}
						else if ((*evil)->end.y >= ground)
						{
							if ((*evil)->start.x <= 0)(*evil)->set_path(scr_width, sky);
							else if ((*evil)->end.x >= scr_width)(*evil)->set_path(0, sky);
							else (*evil)->set_path((*evil)->center.x, sky);
						}
						else if ((*evil)->start.x <= 0 || (*evil)->get_target_x() <= (*evil)->center.x)
							(*evil)->set_path(scr_width, (*evil)->center.y);
						else if ((*evil)->end.x >= scr_width || (*evil)->get_target_x() >= (*evil)->center.x)
							(*evil)->set_path(0, (*evil)->center.y);
					}
				}
				else if (doing == action::shoot)
				{
					if ((*evil)->attack() > 0)
					{
						if ((*evil)->type == moveables::soul || (*evil)->type == moveables::flyer)
						{
							vEvilShots.push_back(dll::SHOT::create((*evil)->center.x, (*evil)->center.y,
								Hero->center.x, Hero->center.y, (*evil)->damage));
							if (sound)mciSendString(L"play .\\res\\snd\\shot.wav", NULL, NULL, NULL);
						}
						else
						{
							if (dll::Intersect((*evil)->get_rect(), Hero->get_rect()))
								Hero->lifes -= ((*evil)->damage - Hero->armor);
							if (sound)mciSendString(L"play .\\res\\snd\\hurt.wav", NULL, NULL, NULL);
						}
					}
				}
				else if (doing == action::patrol)(*evil)->move(speed);
				else if (doing == action::stand )
				{
					float tx{ 0 };
					float ty{ sky };

					if ((*evil)->start.x >= scr_width / 2.0f)tx = scr_width;
					if ((*evil)->start.y >= scr_height / 2.0f)ty = ground;
					(*evil)->current_action = action::patrol;
					(*evil)->set_path(tx, ty);
					(*evil)->move(speed);
				}
			}
		}

		if (Hero && !vChests.empty())
		{
			for (std::vector<D2D1_RECT_F>::iterator chest = vChests.begin(); chest < vChests.end(); ++chest)
			{
				D2D1_RECT_F a_chest{ chest->left, chest->top, chest->right, chest->bottom };
				
				if (dll::Intersect(Hero->get_rect(), a_chest))
				{
					assets asset{ static_cast<assets>(RandIt(0,4)) };

					if (asset != assets::life && asset != assets::map)
						vAssetIcons.push_back(FADE{ asset, FPOINT{a_chest.left,a_chest.top} });
					else
					{
						if (a_chest.left - 50.0f >= 0)a_chest.left -= 50.0f;
						else a_chest.left += 50.0f;
						if (a_chest.top - 50.0f >= sky)a_chest.top -= 50.0f;
						else a_chest.top += 50.0f;

						bool is_ok = false;
						
						while (!is_ok)
						{
							is_ok = true;

							for (int row = 0; row < FIELD_ROWS; ++row)
							{
								for (int col = 0; col < FIELD_COLS; ++col)
								{
									if (Field->is_water_tile(row, col))
									{
										if (dll::Intersect(a_chest, Field->get_tile_rect(row, col)))
										{
											is_ok = false;
											if (a_chest.left - 50.0f >= 0)a_chest.left -= 50.0f;
											else a_chest.left += 50.0f;
											if (a_chest.top - 50.0f >= 0)a_chest.top -= 50.0f;
											else a_chest.top += 50.0f;
											break;
										}
									}
								}

								if (!is_ok)break;
							}

							if (Mountain)
							{
								if (dll::Intersect(Mountain->get_rect(), a_chest))
								{
									if (a_chest.left - 50.0f >= 0)a_chest.left -= 50.0f;
									else a_chest.left += 50.0f;
									if (a_chest.top - 50.0f >= sky)a_chest.top -= 50.0f;
									else a_chest.top += 50.0f;
									is_ok = false;
								}
							}

							if (!vTrees.empty())
							{
								for (int i = 0; i < vTrees.size(); ++i)
								{
									if (dll::Intersect(a_chest, vTrees[i]->get_rect()))
									{
										if (a_chest.left - 50.0f >= 0)a_chest.left -= 50.0f;
										else a_chest.left += 50.0f;
										if (a_chest.top - 50.0f >= sky)a_chest.top -= 50.0f;
										else a_chest.top += 50.0f;
										is_ok = false;
										break;
									}
								}
							}

							if (is_ok)
							{
								if (asset == assets::life)
								{
									a_chest.right = a_chest.left + 32.0f;
									a_chest.bottom = a_chest.top + 32.0f;
								}
								else
								{
									a_chest.right = a_chest.left + 50.0f;
									a_chest.bottom = a_chest.top + 39.0f;
								}
							}
						}
					}
					
					vChests.erase(chest);
					
					switch (asset)
					{
					case assets::gold:
						score += 50;
						break;

					case assets::armor:
						Hero->armor++;
						break;

					case assets::gun:
						Hero->damage++;
						break;

					case assets::map:
						vMaps.push_back(a_chest);
						break;

					case assets::life:
						vPotions.push_back(a_chest);
						break;
					}

					break;
				}
			}
		}

		if (Hero && !vMaps.empty())
		{
			for (std::vector<D2D1_RECT_F>::iterator map = vMaps.begin(); map < vMaps.end(); ++map)
			{
				
				if (dll::Intersect(Hero->get_rect(), *map))
				{
					++map_pieces;
					vMaps.erase(map);

					if (map_pieces >= 8)treasure_found = true;

					break;
				}
			}
		}

		if (!vEvils.empty() && !vHeroShots.empty())
		{
			for (std::vector<dll::EVIL*>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
			{
				bool killed = false;

				for (std::vector<dll::SHOT*>::iterator shot = vHeroShots.begin(); shot < vHeroShots.end(); ++shot)
				{
					if (dll::Intersect((*evil)->center, (*shot)->center, (*evil)->x_rad, (*shot)->x_rad,
						(*evil)->y_rad, (*shot)->y_rad))
					{
						(*evil)->lifes -= ((*shot)->damage - (*evil)->armor);
						(*shot)->Release();
						vHeroShots.erase(shot);

						if ((*evil)->lifes <= 0)
						{
							score += (*evil)->damage;
							if (RandIt(0, 8) == 6)vMaps.push_back((*evil)->get_rect());
							(*evil)->Release();
							vEvils.erase(evil);
							killed = true;
							if (sound)(mciSendString(L"play .\\res\\snd\\evil_killed.wav", NULL, NULL, NULL));

						}

						break;
					}
				}

				if (killed)break;
			}
		}

		if (!vEvilShots.empty())
		{
			for (int i = 0; i < vEvilShots.size(); ++i)
			{
				if (!vEvilShots[i]->move(speed))
				{
					vEvilShots[i]->Release();
					vEvilShots.erase(vEvilShots.begin() + i);
					break;
				}
			}
		}
		
		if (!vEvilShots.empty() && Hero)
		{
			for (std::vector<dll::SHOT*>::iterator shot = vEvilShots.begin(); shot < vEvilShots.end(); ++shot)
			{
				if (dll::Intersect((*shot)->center, Hero->center, (*shot)->x_rad, Hero->x_rad, (*shot)->y_rad, 
					Hero->y_rad))
				{
					Hero->lifes -= ((*shot)->damage - Hero->armor);
					(*shot)->Release();
					vEvilShots.erase(shot);
					if (sound)(mciSendString(L"play .\\res\\snd\\hurt.wav", NULL, NULL, NULL));
					break;
				}
			}
		}

		if (!vEvils.empty() && !vChests.empty())
		{
			for (std::vector<dll::EVIL*>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
			{
				bool taken = false;

				for (std::vector<D2D1_RECT_F>::iterator chest = vChests.begin(); chest < vChests.end(); ++chest)
				{
					if (dll::Intersect((*evil)->get_rect(), *chest))
					{
						assets asset{ static_cast<assets>(RandIt(0,3)) };
						vAssetIcons.push_back(FADE{ asset, FPOINT{chest->left,chest->top} });
						switch (asset)
						{
						case assets::gold:
							if (score - 50 >= 0)score -= 50;
							else score = 0;
							if (sound)(mciSendString(L"play .\\res\\snd\\gold.wav", NULL, NULL, NULL));
							break;

						case assets::armor:
							(*evil)->armor++;
							if (sound)(mciSendString(L"play .\\res\\snd\\armor.wav", NULL, NULL, NULL));
							break;

						case assets::gun:
							(*evil)->damage++;
							if (sound)(mciSendString(L"play .\\res\\snd\\weapon.wav", NULL, NULL, NULL));
							break;

						case assets::life:
							(*evil)->lifes = (*evil)->get_max_lifes();
							if (sound)(mciSendString(L"play .\\res\\snd\\heal.wav", NULL, NULL, NULL));
							break;
						}
						vChests.erase(chest);
						taken = true;
						break;
					}
				}
				if (taken) break;
			}
		}

		if (!vPotions.empty() && Hero)
		{
			for (std::vector<D2D1_RECT_F>::iterator pot = vPotions.begin(); pot < vPotions.end(); ++pot)
			{
				if (dll::Intersect(Hero->get_rect(), *pot))
				{
					if (Hero->lifes + 20 <= 200)Hero->lifes += 20;
					else Hero->lifes = 200;
					if (sound)(mciSendString(L"play .\\res\\snd\\heal.wav", NULL, NULL, NULL));
					vPotions.erase(pot);
					break;
				}
			}
		}

		if (treasure_found && dll::Intersect(Hero->center, FPOINT{ TreasureMark.left + 50.0f, TreasureMark.top + 47.0f },
			Hero->x_rad, 50.0f, Hero->y_rad, 47.0f))LevelUp();

	// DRAW THINGS **************************************************

		Draw->BeginDraw();

		if (Field)
		{
			for (int rows = 0; rows < FIELD_ROWS; ++rows)
			{
				for (int cols = 0; cols < FIELD_COLS; ++cols)
				{
					if (Field->is_water_tile(rows, cols))Draw->DrawBitmap(bmpWater, Field->get_tile_rect(rows, cols));
					else Draw->DrawBitmap(bmpDirt, Field->get_tile_rect(rows, cols));
				}
			}
		}

		if (Hero)
		{
			int frame = Hero->get_frame();

			switch (Hero->get_action())
			{
			case action::stand:
				if (Hero->dir == dirs::left)Draw->DrawBitmap(bmpHeroStandL[frame],
					Resizer(bmpHeroStandL[frame], Hero->start.x, Hero->start.y));
				else Draw->DrawBitmap(bmpHeroStandR[frame], Resizer(bmpHeroStandR[frame], Hero->start.x, Hero->start.y));
				break;

			case action::walk:
				if (Hero->dir == dirs::left)Draw->DrawBitmap(bmpHeroWalkL[frame],
					Resizer(bmpHeroWalkL[frame], Hero->start.x, Hero->start.y));
				else Draw->DrawBitmap(bmpHeroWalkR[frame], Resizer(bmpHeroWalkR[frame], Hero->start.x, Hero->start.y));
				break;

			case action::shoot:
				if (Hero->dir == dirs::left)Draw->DrawBitmap(bmpHeroShootL[frame],
					Resizer(bmpHeroShootL[frame], Hero->start.x, Hero->start.y));
				else Draw->DrawBitmap(bmpHeroShootR[frame], Resizer(bmpHeroShootR[frame], Hero->start.x, Hero->start.y));
				break;
			}

			float life_ratio = (float)(Hero->lifes * (80.0f / 200.0f));
			
			Draw->DrawLine(D2D1::Point2F(Hero->start.x, Hero->end.y + 6.0f),
				D2D1::Point2F(Hero->end.x, Hero->end.y + 6.0f), grayBrush, 5.0f);
			Draw->DrawLine(D2D1::Point2F(Hero->start.x, Hero->end.y + 6.1f),
				D2D1::Point2F(Hero->start.x + life_ratio, Hero->end.y + 6.1f), txtBrush, 2.0f);
		}

		if (Mountain)
		{
			switch (Mountain->type)
			{
			case nature::mountain1:
				Draw->DrawBitmap(bmpMountain1, Mountain->get_rect());
				break;

			case nature::mountain2:
				Draw->DrawBitmap(bmpMountain2, Mountain->get_rect());
				break;
			}
		}

		if (!vTrees.empty())
		{
			for (int i = 0; i < vTrees.size(); ++i)
			{
				switch (vTrees[i]->type)
				{
				case nature::tree1:
					Draw->DrawBitmap(bmpTree1, vTrees[i]->get_rect());
					break;

				case nature::tree2:
					Draw->DrawBitmap(bmpTree2, vTrees[i]->get_rect());
					break;

				case nature::tree3:
					Draw->DrawBitmap(bmpTree3, vTrees[i]->get_rect());
					break;
				}
			}
		}

		if (!vTombs.empty())
		{
			for (int i = 0; i < vTombs.size(); ++i)Draw->DrawBitmap(bmpTomb, vTombs[i]);
		}

		if (!vChests.empty())
		{
			for (int i = 0; i < vChests.size(); ++i)Draw->DrawBitmap(bmpChest, vChests[i]);
		}

		if (!vHeroShots.empty())
		{
			for (int i = 0; i < vHeroShots.size(); ++i)
			{
				Draw->DrawBitmap(bmpBullet, vHeroShots[i]->get_rect());
			}
		}

		if (!vEvilShots.empty())
		{
			for (int i = 0; i < vEvilShots.size(); ++i)
			{
				Draw->DrawBitmap(bmpBullet, vEvilShots[i]->get_rect());
			}
		}

		if (!vEvils.empty())
		{
			for (int i = 0; i < vEvils.size(); ++i)
			{
				int frame = vEvils[i]->get_frame();

				switch (vEvils[i]->type)
				{
				case moveables::soul:
					Draw->DrawBitmap(bmpSoul[frame], Resizer(bmpSoul[frame], vEvils[i]->start.x, vEvils[i]->start.y));
					break;

				case moveables::flyer:
					if (vEvils[i]->dir == dirs::left)Draw->DrawBitmap(bmpFlyerL[frame], Resizer(bmpFlyerL[frame],
						vEvils[i]->start.x, vEvils[i]->start.y));
					else Draw->DrawBitmap(bmpFlyerR[frame], Resizer(bmpFlyerR[frame],
						vEvils[i]->start.x, vEvils[i]->start.y));
					break;

				case moveables::zombie:
					if (vEvils[i]->dir == dirs::left)Draw->DrawBitmap(bmpZombieL[frame], Resizer(bmpZombieL[frame],
						vEvils[i]->start.x, vEvils[i]->start.y));
					else Draw->DrawBitmap(bmpZombieR[frame], Resizer(bmpZombieR[frame],
						vEvils[i]->start.x, vEvils[i]->start.y));
					break;

				case moveables::girl:
					if (vEvils[i]->dir == dirs::left)Draw->DrawBitmap(bmpGirlL[frame], Resizer(bmpGirlL[frame],
						vEvils[i]->start.x, vEvils[i]->start.y));
					else Draw->DrawBitmap(bmpGirlR[frame], Resizer(bmpGirlR[frame],
						vEvils[i]->start.x, vEvils[i]->start.y));
					break;
				}

				float life_ratio = (float)(vEvils[i]->lifes * (vEvils[i]->get_width() / vEvils[i]->get_max_lifes()));

				Draw->DrawLine(D2D1::Point2F(vEvils[i]->start.x, vEvils[i]->end.y + 6.0f),
					D2D1::Point2F(vEvils[i]->end.x, vEvils[i]->end.y + 6.0f), grayBrush, 5.0f);
				Draw->DrawLine(D2D1::Point2F(vEvils[i]->start.x, vEvils[i]->end.y + 6.1f),
					D2D1::Point2F(vEvils[i]->start.x + life_ratio, vEvils[i]->end.y + 6.1f), txtBrush, 2.0f);
			}
		}

		if (!vAssetIcons.empty())
		{
			for (std::vector<FADE>::iterator icon = vAssetIcons.begin(); icon < vAssetIcons.end(); ++icon)
			{
				float an_opacity = icon->get_opacity();

				switch (icon->type)
				{
				case assets::gold:
					Draw->DrawBitmap(bmpIconGold, D2D1::RectF(icon->position.x - 16.0f, icon->position.y - 16.0f,
						icon->position.x + 16.0f, icon->position.y + 16.0f), an_opacity);
					break;

				case assets::gun:
					Draw->DrawBitmap(bmpIconGun, D2D1::RectF(icon->position.x - 16.0f, icon->position.y - 16.0f,
						icon->position.x + 16.0f, icon->position.y + 16.0f), an_opacity);
					break;

				case assets::armor:
					Draw->DrawBitmap(bmpIconArmor, D2D1::RectF(icon->position.x - 16.0f, icon->position.y - 16.0f,
						icon->position.x + 16.0f, icon->position.y + 16.0f), an_opacity);
					break;
				}

				if (an_opacity <= 0)
				{
					vAssetIcons.erase(icon);
					break;
				}
			}
		}

		if (!vMaps.empty())for (int i = 0; i < vMaps.size(); ++i)Draw->DrawBitmap(bmpMap, D2D1::RectF(vMaps[i].left, vMaps[i].top,
			vMaps[i].right, vMaps[i].bottom));

		if (!vPotions.empty())for (int i = 0; i < vPotions.size(); ++i)Draw->DrawBitmap(bmpPotion, 
			D2D1::RectF(vPotions[i].left, vPotions[i].top, vPotions[i].right, vPotions[i].bottom));

		if (treasure_found)Draw->DrawBitmap(bmpMark, TreasureMark);

	////////////////////////////////////////////////////////////

		if (nrmText && statBrush && txtBrush && hgltBrush && inactBrush && b1Bckg && b2Bckg && b3Bckg)
		{
			Draw->FillRectangle(D2D1::RectF(0, 0, scr_width, 50.0f), statBrush);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b1Rect, 10.0f, 15.0f), b1Bckg);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b2Rect, 10.0f, 15.0f), b2Bckg);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b3Rect, 10.0f, 15.0f), b3Bckg);
			Draw->FillRectangle(D2D1::RectF(0, ground, scr_width, scr_height), statBrush);

			if (name_set)Draw->DrawTextW(L"ИМЕ НА ГЕРОЙ", 13, nrmText, b1TxtRect, inactBrush);
			else
			{
				if (b1Hglt)Draw->DrawTextW(L"ИМЕ НА ГЕРОЙ", 13, nrmText, b1TxtRect, hgltBrush);
				else Draw->DrawTextW(L"ИМЕ НА ГЕРОЙ", 13, nrmText, b1TxtRect, txtBrush);
			}
			if (b2Hglt)Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmText, b2TxtRect, hgltBrush);
			else Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmText, b2TxtRect, txtBrush);
			if (b3Hglt)Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmText, b3TxtRect, hgltBrush);
			else Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmText, b3TxtRect, txtBrush);
		}
		
		if (Hero)
		{
			if (Hero->lifes <= 0)
			{
				Draw->DrawBitmap(bmpRIP, Hero->get_rect());

				FreeMem(&Hero);
				Draw->EndDraw();
				if (sound)
				{
					PlaySound(NULL, NULL, NULL);
					PlaySound(L".\\res\\snd\\killed.wav", NULL, SND_SYNC);
				}

				GameOver();
			}
		}
		
		Draw->EndDraw();
	}

	std::remove(tmp_file);
	ReleaseResources();

	return (int) bMsg.wParam;
}