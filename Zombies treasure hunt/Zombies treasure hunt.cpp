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

D2D1_RECT_F b1TxtRect{ 70.0f, 10.0f, scr_width / 3.0f - 50.0f, 40.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3.0f + 70.0f, 10.0f, scr_width * 2.0f / 3.0f - 50.0f, 40.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 2.0f / 3.0f + 60.0f, 10.0f, scr_width - 50.0f, 40.0f };

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

bool hero_killed = false;
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

ID2D1Bitmap* bmpIntro[16]{ nullptr };

ID2D1Bitmap* bmpFlyerL[11]{ nullptr };
ID2D1Bitmap* bmpFlyerR[11]{ nullptr };

ID2D1Bitmap* bmpGirlL[26]{ nullptr };
ID2D1Bitmap* bmpGirlR[26]{ nullptr };

ID2D1Bitmap* bmpSoul[120]{ nullptr };

ID2D1Bitmap* bmpZombieL[4]{ nullptr };
ID2D1Bitmap* bmpZombieR[4]{ nullptr };

ID2D1Bitmap* bmpHeroShootL[50]{ nullptr };
ID2D1Bitmap* bmpHeroShootR[50]{ nullptr };
ID2D1Bitmap* bmpHeroStandL[32]{ nullptr };
ID2D1Bitmap* bmpHeroStandR[32]{ nullptr };
ID2D1Bitmap* bmpHeroWalkL[16]{ nullptr };
ID2D1Bitmap* bmpHeroWalkR[16]{ nullptr };

/////////////////////////////////////////////////////

dll::RANDIT RandIt{};

dll::FIELD* Field{ nullptr };

dll::HERO* Hero{ nullptr };




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

	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpIntro[i]))LogErr(L"Error unloading D2D1 bmpIntro !");

	for (int i = 0; i < 11; ++i)if (!FreeMem(&bmpFlyerL[i]))LogErr(L"Error unloading D2D1 bmpFlayerL !");
	for (int i = 0; i < 11; ++i)if (!FreeMem(&bmpFlyerR[i]))LogErr(L"Error unloading D2D1 bmpFlayerR !");

	for (int i = 0; i < 26; ++i)if (!FreeMem(&bmpGirlL[i]))LogErr(L"Error unloading D2D1 bmpGirlL !");
	for (int i = 0; i < 26; ++i)if (!FreeMem(&bmpGirlR[i]))LogErr(L"Error unloading D2D1 bmpGirlR !");

	for (int i = 0; i < 120; ++i)if (!FreeMem(&bmpSoul[i]))LogErr(L"Error unloading D2D1 bmpSoul !");

	for (int i = 0; i < 4; ++i)if (!FreeMem(&bmpZombieL[i]))LogErr(L"Error unloading D2D1 bmpZombieL !");
	for (int i = 0; i < 4; ++i)if (!FreeMem(&bmpZombieR[i]))LogErr(L"Error unloading D2D1 bmpZombieR !");

	for (int i = 0; i < 50; ++i)if (!FreeMem(&bmpHeroShootL[i]))LogErr(L"Error unloading D2D1 bmpHeroShootL !");
	for (int i = 0; i < 50; ++i)if (!FreeMem(&bmpHeroShootR[i]))LogErr(L"Error unloading D2D1 bmpHeroShootR !");
	for (int i = 0; i < 32; ++i)if (!FreeMem(&bmpHeroStandL[i]))LogErr(L"Error unloading D2D1 bmpHeroStandL !");
	for (int i = 0; i < 32; ++i)if (!FreeMem(&bmpHeroStandR[i]))LogErr(L"Error unloading D2D1 bmpHeroStandR !");
	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpHeroWalkL[i]))LogErr(L"Error unloading D2D1 bmpHeroWalkL !");
	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpHeroWalkR[i]))LogErr(L"Error unloading D2D1 bmpHeroWalkR !");
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
	
	hero_killed = false;
	treasure_found = false;

	map_pieces = 0;

	if (Field)
	{
		delete Field;
		Field = new dll::FIELD;
	}
	if (Hero)
	{
		Hero->Release();
		Hero = dll::HERO::create(RandIt(0.0f, scr_width - 100.f), ground - 100.0f);
	}





}




int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	bIns = hInstance;
	if (!bIns)
	{
		LogErr(L"Error obtaining hInstance from Windows !");
		ErrExit(eClass);
	}












	std::remove(tmp_file);
	ReleaseResources();

	return (int) bMsg.wParam;
}