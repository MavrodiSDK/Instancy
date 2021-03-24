#pragma warning(disable : 4530)

#define Use_VMProtect 0
#define AllocCons 0

#include <Windows.h>
#include "sdk/VMProtectSDK.h"
#pragma comment(lib, "VMProtectSDK64.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "ws2_32.lib")
#include <psapi.h>
#include <d3d11.h>
#include <emmintrin.h>
#include "winternl.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int isSteam = 0;
DWORD64 ServerRPC = 0;
DWORD64 CreateProjectile = 0;
DWORD64 SENDPRJ = 0;
DWORD64 ClientInput = 0;
DWORD64 DoFixedUpdate = 0;
DWORD64 CanAttack = 0;
DWORD64 ChamsOkey = 0;
DWORD64 sendClientTick = 0;
DWORD64 SendProjectileAttack = 0;
DWORD64 DoHit = 0;
DWORD64 Flying = 0;
DWORD64 HitSND = 0;
DWORD64 NightMode = 0;
DWORD64 AutoFarm = 0;
//DWORD64 Update = 0;
DWORD64 AimDirection = 0;

#include "sdk/NewUtils.h"
#include "sdk/utils.h"
#include "sdk/safe_mem.h"
#include "sdk_new/math.h"
#include "vars/vars.h"
#include "sdk/game_sdk.h"
#include "NewGui.h"
#include <sys\stat.h>

#include "cheat_funcs/EspFuncs.h"
#include "cheat_funcs/AimFuncs.h"
#include "cheat_funcs/MiscFuncs.h"
#include "cheat_funcs/EntityZaLoop.h"
#include "E:\Source [Instancy]\DxDLLRender\detours.h"
#include <time.h>
#include "ReloadLibrary.h"
#include <IPTypes.h>

#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

using namespace GUI;

WSAData wsaData;
UINT vps = 1;
D3D11_VIEWPORT viewport;
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;
ID3D11RenderTargetView* RenderTargetView = NULL;
ID3D11Device* pD11Device = nullptr;
IDXGISwapChain* dxSwapChain = nullptr;
ID3D11DeviceContext* pD11DeviceContext = nullptr;
ID3D11RenderTargetView* pD11RenderTargetView = nullptr;
HWND hWnd = NULL;
WNDPROC OrigWndProc;
SOCKET Connection;


bool panic = false;

typedef HRESULT(__fastcall* DX11PresentFnPresent) (IDXGISwapChain* pSwapChain, UINT syncintreval, UINT flags);
DX11PresentFnPresent oPresent = nullptr;

typedef HRESULT(__stdcall* DX11ResizeFnResize)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
DX11ResizeFnResize oResize = nullptr;

HRESULT __stdcall Resize(IDXGISwapChain* Swap, UINT a2, UINT a3, UINT a4, DXGI_FORMAT a5, UINT a6) { 
	GUI::Render.ResetCanvas(); return oResize(Swap, a2, a3, a4, a5, a6); 
}
LRESULT CALLBACK WndProcHook(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(OrigWndProc, window, message, wParam, lParam);
}
bool InitD3DX11(IDXGISwapChain* dxSwapChain)
{
	if (!pD11Device || !pD11DeviceContext)
	{
		if (SUCCEEDED(dxSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pD11Device)))
		{
			dxSwapChain->GetDevice(__uuidof(pD11Device), (void**)&pD11Device);
			pD11Device->GetImmediateContext(&pD11DeviceContext);
		}
		DXGI_SWAP_CHAIN_DESC desc;
		dxSwapChain->GetDesc(&desc);
		hWnd = desc.OutputWindow;
		GUI::NextWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)GUI::WndProc));
		return true;
	}
	return false;
}
HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	dxSwapChain = pSwapChain;
	if (!InitD3DX11(dxSwapChain))
	{
		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (!pD11RenderTargetView)
		{
			if (SUCCEEDED(dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
			{
				pD11Device->CreateRenderTargetView(renderTargetTexture, nullptr, &pD11RenderTargetView);
				renderTargetTexture->Release();
			}
		}
	}
	if (GUI::Render.NewFrame(pSwapChain))
	{
		if (!Vars::Global::Panic)
		{
			Vector2 kek = GUI::Render.CanvasSize();
			Vars::Global::ScreenWidth = kek.x;
			Vars::Global::ScreenHigh = kek.y;

			if (Vars::AimBot::BodyAim) Vars::Global::BoneToAim = 1;
			else Vars::Global::BoneToAim = BoneList(neck);

			if (!Vars::Global::Panic && !LocalPlayer.BasePlayer->IsMenu())
			{
				if (Vars::Visuals::Crosshair)
					Crosshair();

				if (Vars::AimBot::DrawFov)
					DrawFOV();

				if (Vars::AimBot::DrawFov1)
					DrawFOV1();
			}

			if (GetAsyncKeyState(VK_END)) {
				Vars::Global::Panic = true;
				Vars::AimBot::pSilent = false;
				Vars::AimBot::pSilentHead = false;
				Vars::AimBot::pSilentBody = false;
				Vars::AimBot::DrawFov1 = false;
				Vars::Misc::AntiAim = false;
				Vars::Misc::Spinz = false;
				Vars::Misc::Spin = false;
				Vars::Misc::JumpShoot = false;
				Vars::Visuals::Chams = false;
				Vars::Misc::Automatic = false;
				Vars::Misc::SlideWalk = false;
				Vars::Visuals::NightMode = false;
			}
			if (!Vars::Global::Panic) {
				EntityZaLoop();
			}
			else {
				if (lastState == true) {
					Misc();
				}
			}

			if (!Vars::Global::Panic && GUI::Visible) {
				if (Vars::AimBot::Fov > (kek.y - 3)) Vars::AimBot::Fov = (kek.y - 3);
				GUI::Begin(Vars::Global::MenuPos, Vector2{ 630.f, 630.f }, { 0.f, 0.f, 0.f, 0.8f });
				GUI::Render.StringCenter(GUI::CenterLine({ Vars::Global::MenuPos.x, Vars::Global::MenuPos.y + 23.f }), cry(L"INSTANCY"), D2D1::ColorF::White, true);
				GUI::CurWnd.Pos.y += 25.f;
				GUI::Render.Line({ CurWnd.Pos.x, CurWnd.Pos.y }, { CurWnd.Pos.x + 630.f, CurWnd.Pos.y }, { 255.f, 0.f, 0.f, 0.4f });
				GUI::Render.Line({ CurWnd.Pos.x + 120.f, CurWnd.Pos.y }, { CurWnd.Pos.x + 120.f, CurWnd.Pos.y + 580.f - 0.f }, { 255.f, 0.f, 0.f, 0.4f });

				GUI::CurWnd.Pos.y += 15.f;
				float CurPos = GUI::CurWnd.Pos.y;
				const wchar_t* pos[4] = {
					L"Аимбот", L"ВХ", L"Оружие", L"Прочее"
				};
				for (int i = 0; i < 4; i++) {
					D2D1::ColorF TextClr = { 1, 1, 1, 0.9 };
					D2D1::ColorF FillClr = { 255.f, 0.f, 0.f, 0.4f };
					if (i == Vars::Global::CurPage) {
						Render.FillRectangle({ CurWnd.Pos.x + 10.f, CurPos }, { 90.f, 30.f }, FillClr);
					}
					else {
						Render.Rectangle({ CurWnd.Pos.x + 10.f, CurPos }, { 90.f, 30.f }, FillClr);
					}
					Render.String1({ CurWnd.Pos.x + 20.f, CurPos }, pos[i], TextClr, true);
					if (IO.LKM == Clicked && InRect({ CurWnd.Pos.x + 10.f, CurPos }, { 90.f, 30.f }, IO.MousePos) && InputMgr(cry(L"Tab"), true)) Vars::Global::CurPage = i;
					CurPos += 60.f;
				}
				GUI::CurWnd.Pos.x += 120.f;
				CurWnd.Size.x -= 120.f;
				if (Vars::Global::CurPage == 0) {
					GUI::CheckBox(cry(L"Отпиздить игроков (SilentMelee)"), &Vars::Misc::MeleeAttack);
					GUI::CheckBox(cry(L"пСайлент аим"), &Vars::AimBot::pSilent);
					if (Vars::AimBot::pSilent) {
						GUI::CheckBox(cry(L"На голову"), &Vars::AimBot::pSilentHead);
						GUI::CheckBox(cry(L"На тело"), &Vars::AimBot::pSilentBody);
						GUI::CheckBox(cry(L"Показать на кого работает (линия)"), &Vars::Visuals::TargetLine);
						GUI::CheckBox(cry(L"помощь в миссах xd"), &Vars::AimBot::SilentxD);
						GUI::CheckBox(cry(L"Звук при попадании №1"), &Vars::Misc::HitSound);
						if (Vars::Misc::HitSound) {
							URLDownloadToFileA(nullptr, cry("http://timoxa5651.siteme.org/Instancy/sound.wav"), cry("C:\\sound\\1.wav"), 0, nullptr);
							URLDownloadToFileA(nullptr, cry("http://timoxa5651.siteme.org/Instancy/sound1.wav"), cry("C:\\sound\\2.wav"), 0, nullptr);
							URLDownloadToFileA(nullptr, cry("http://timoxa5651.siteme.org/Instancy/sound2.wav"), cry("C:\\sound\\3.wav"), 0, nullptr);
							GUI::Spacing(cry(L"Выбери нужный звук"));
							GUI::CheckBox(cry(L"Звук колокола"), &Vars::Misc::HitSound1);
							GUI::CheckBox(cry(L"Смэрть (глад валакас)"), &Vars::Misc::HitSound2);
							GUI::CheckBox(cry(L"Еще один колокол (из сампа)"), &Vars::Misc::HitSound3);
							GUI::Spacing(cry(L""));
						}						
						//GUI::CheckBox(cry(L"Хитсаунд (майнкрафт)"), &Vars::Misc::HitSoundMinecraft);
						/*GUI::CheckBox(cry(L"Показать радиус"), &Vars::AimBot::DrawFov1, true);*/
						//GUI::CheckBox(cry(L"Лютое хвх (шанс детекта: 999)"), &Vars::AimBot::FatBullet);
						//GUI::CheckBox(cry(L"Лютое хвх #2 (шанс детекта: 999)"), &Vars::AimBot::MagicBullet);
						//GUI::CheckBox(cry(L"Фейк дистанция"), &Vars::AimBot::FakeDist);
						if (Vars::AimBot::FatBullet) {
							GUI::SliderFloat(cry(L"пизда"), &Vars::AimBot::FatBulletScore, 0.50f, 1.5f);
						}
						GUI::SliderFloat(cry(L"Радиус"), &Vars::AimBot::pSilentFov, 0.f, ((kek.y / 2.f) - 3));
					}
					GUI::CheckBox(cry(L"Обычный аим"), &Vars::AimBot::Activate);
					if (Vars::AimBot::Activate) {
						GUI::CheckBox(cry(L"Проверка на видимость (криво работает)"), &Vars::AimBot::VisibleCheck);
						GUI::CheckBox(cry(L"Игнорировать ботов"), &Vars::AimBot::IgnoreNpc);
						GUI::CheckBox(cry(L"Показать радиус"), &Vars::AimBot::DrawFov, true);
						GUI::SliderFloat(cry(L"Радиус"), &Vars::AimBot::Fov, 0.f, ((kek.y / 2.f) - 3));
						GUI::SliderFloat(cry(L"Дистанция"), &Vars::AimBot::Range, 80.f, 400.f);
					}
					GUI::Spacing(cry(L"Конфиги от Мавроди"));
					GUI::CheckBox(cry(L"Legit cfg"), &Vars::Misc::LEGITCFGBYMAVRODI);
					if (Vars::Misc::LEGITCFGBYMAVRODI) {
						GUI::Render.String({ 10.f, 10.f }, L"Legit config loaded..", D2D1::ColorF::Red);
						Vars::AimBot::pSilent = true;
						Vars::AimBot::pSilentBody = true;
						Vars::Visuals::ShowPlayerBoxCorner = true;
						Vars::Visuals::ShowPlayerName = true;
						Vars::Visuals::ShowPlayerHealth = true;
						Vars::Visuals::ShowPlayerWeapon = true;
						Vars::Visuals::ShowPlayerDist = true;
						Vars::Visuals::IgnoreNpc = true;
						Vars::Visuals::PlayerDead = true;
						Vars::Visuals::Helicopter = true;
						Vars::Misc::JumpShoot = true;
						Vars::Misc::SpiderMan = true;
						Vars::Misc::Automatic = true;
					}
					GUI::CheckBox(cry(L"Rage cfg"), &Vars::Misc::RAGECFGBYMAVRODI);
					if (Vars::Misc::RAGECFGBYMAVRODI) {
						GUI::Render.String({ 10.f, 10.f }, L"Rage config loaded..", D2D1::ColorF::Red);
						Vars::AimBot::pSilent = true;
						Vars::AimBot::pSilentHead = true;
						Vars::Visuals::ShowPlayerBoxCorner = true;
						Vars::Visuals::ShowPlayerName = true;
						Vars::Visuals::ShowPlayerHealth = true;
						Vars::Visuals::ShowPlayerWeapon = true;
						Vars::Visuals::ShowPlayerDist = true;
						Vars::Visuals::IgnoreSleepers = true;
						Vars::Visuals::IgnoreNpc = true;
						Vars::Visuals::PlayerDead = true;
						Vars::Visuals::Helicopter = true;
						Vars::Misc::JumpShoot = true;
						Vars::Misc::SpiderMan = true;
						Vars::Misc::Automatic = true;
						Vars::Misc::SuperEoka = true;
						Vars::Misc::InfinityJump = true;
						Vars::Misc::SlideWalk = true;
						Vars::Misc::FastHeal = true;
					}
				}
				else if (Vars::Global::CurPage == 1) {
					GUI::Spacing(cry(L"Игрок"));
					GUI::CheckBox(cry(L"Обводка игрока"), &Vars::Visuals::ShowPlayerBoxCorner);
					GUI::CheckBox(cry(L"Имя"), &Vars::Visuals::ShowPlayerName);
					GUI::CheckBox(cry(L"Здоровье"), &Vars::Visuals::ShowPlayerHealth);
					GUI::CheckBox(cry(L"Оружие"), &Vars::Visuals::ShowPlayerWeapon);
					GUI::CheckBox(cry(L"Дистанция"), &Vars::Visuals::ShowPlayerDist);
					GUI::CheckBox(cry(L"Инвентарь игрока"), &Vars::Visuals::ShowPlayerActiveSlot);
					GUI::CheckBox(cry(L"Игнорировать спящих"), &Vars::Visuals::IgnoreSleepers);
					GUI::CheckBox(cry(L"Игнорировать ботов"), &Vars::Visuals::IgnoreNpc);
					GUI::CheckBox(cry(L"Показывать умершего игрока"), &Vars::Visuals::PlayerDead);
					GUI::SliderFloat(cry(L"Поле зрение игрока"), &Vars::Visuals::Fov, 70.f, 150.f);
					GUI::Spacing(cry(L"Прочее"));
					GUI::CheckBox(cry(L"Видная ночь"), &Vars::Visuals::NightMode);
					GUI::CheckBox(cry(L"Чамсы на руки"), &Vars::Visuals::Chams);
					GUI::CheckBox(cry(L"Вертолет"), &Vars::Visuals::Helicopter);
					GUI::CheckBox(cry(L"Коптер"), &Vars::Visuals::Copter);
					GUI::CheckBox(cry(L"Прицел"), &Vars::Visuals::Crosshair, true);
					GUI::Spacing(cry(L"Ресурсы"));
					GUI::CheckBox(cry(L"Сера"), &Vars::Visuals::Sulphur);
					GUI::CheckBox(cry(L"Металл"), &Vars::Visuals::Metal);
					GUI::CheckBox(cry(L"Камень"), &Vars::Visuals::Stone);
					GUI::CheckBox(cry(L"Шкаф"), &Vars::Visuals::Shkaf);
					GUI::CheckBox(cry(L"Ткань"), &Vars::Visuals::Hemp);
					GUI::CheckBox(cry(L"Бочки"), &Vars::Visuals::Barrel);
					GUI::CheckBox(cry(L"Гантрапы"), &Vars::Visuals::Guntrap);
					GUI::CheckBox(cry(L"Двери"), &Vars::Visuals::Door);
					GUI::CheckBox(cry(L"Аирдроп"), &Vars::Visuals::Airdrop);
					GUI::CheckBox(cry(L"Сундуки"), &Vars::Visuals::Chest);
					GUI::CheckBox(cry(L"Мины"), &Vars::Visuals::Landmine);
					GUI::CheckBox(cry(L"Стэши (будьте аккуратнее бывают фейк стеши)"), &Vars::Visuals::Stash);
				}
				else if (Vars::Global::CurPage == 2) {
					GUI::CheckBox(cry(L"Анти отдача"), &Vars::Misc::AntiRecoil);
					GUI::CheckBox(cry(L"Автоматические выстрелы"), &Vars::Misc::Automatic);
					GUI::CheckBox(cry(L"Стрелять в прыжке"), &Vars::Misc::JumpShoot);
					GUI::CheckBox(cry(L"Убрать покачивание на оружие"), &Vars::Misc::NoSway);
					GUI::CheckBox(cry(L"Стрельба с еоки с 1 выстрела"), &Vars::Misc::SuperEoka);
				}
				else if (Vars::Global::CurPage == 3) {
				GUI::CheckBox(cry(L"Лечение без клика"), &Vars::Misc::FastHeal);
				GUI::CheckBox(cry(L"Фейк админка (большой риск получить бан)"), &Vars::Misc::FakeAdmin);
				GUI::CheckBox(cry(L"Автоподбор ресурсов"), &Vars::Misc::AutoPickup);
				GUI::CheckBox(cry(L"Автофарм"), &Vars::Misc::AutoFarm);
				if (Vars::Misc::AutoFarm) {
					GUI::CheckBox(cry(L"на дерево"), &Vars::Misc::AutoFarmTree);
					GUI::CheckBox(cry(L"на руду"), &Vars::Misc::AutoFarmOre);
				}
				GUI::CheckBox(cry(L"Бегать в разные стороны"), &Vars::Misc::SlideWalk);
				GUI::CheckBox(cry(L"Тики (лютая хуйня для хвх)"), &Vars::Misc::FakeLag);
				if (Vars::Misc::FakeLag) {
					GUI::SliderFloat(cry(L"Количество тиков"), &Vars::Misc::FakeLagFloat, 0.4f, 10.f);
				}
				//GUI::CheckBox(cry(L"Плавать"), &Vars::Misc::SwimOnGround);
				GUI::CheckBox(cry(L"Анти аим"), &Vars::Misc::AntiAim);
				if (Vars::Misc::AntiAim) {
					GUI::CheckBox(cry(L"Джитер"), &Vars::Misc::Spin);
					GUI::CheckBox(cry(L"Даунский анти аим"), &Vars::Misc::Spinz);
				}
				GUI::CheckBox(cry(L"Ползанье по стенам"), &Vars::Misc::SpiderMan);
				GUI::CheckBox(cry(L"Бесконечный прыжок"), &Vars::Misc::InfinityJump);
				}
				GUI::End();
			}
			GUI::Render.EndFrame();
		}
	end:
		Vars::Global::PresentDelta = 0;
		GUI::Render.EndFrame();
	}
	pD11DeviceContext->OMSetRenderTargets(1, &pD11RenderTargetView, nullptr);
	if (pD11RenderTargetView)
	{
		pD11RenderTargetView->Release();
		pD11RenderTargetView = nullptr;
	}
	return oPresent(pSwapChain, SyncInterval, Flags);
}

void* DetourVTable(void** VTable, int Function, void* Detour)
{
	void* ret = VTable[Function];
	DWORD old;
	VirtualProtect(&(VTable[Function]), sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
	VTable[Function] = Detour;
	VirtualProtect(&(VTable[Function]), sizeof(void*), old, &old);
	return ret;
}

__declspec(noinline) void Start()
{
	Vars::gBase = (DWORD64)GetModuleHandleA("GameAssembly.dll");
	HWND window;
	uintptr_t* pSwapChainVtable = NULL;
	window = FindWindowA(NULL, "Rust");
	HMODULE hDXGIDLL = 0;
	do
	{
		hDXGIDLL = GetModuleHandleA("dxgi.dll");
	} while (!hDXGIDLL);
	IDXGISwapChain* SwapChain;
	D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	ID3D11Device* D3DDevice = nullptr;
	ID3D11DeviceContext* D3DContext = nullptr;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof SwapChainDesc);
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = window;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	UINT createFlags = 0;
	IDXGISwapChain* d3dSwapChain = 0;
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		requestedLevels,
		sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&D3DDevice,
		&obtainedLevel,
		&D3DContext)))
	{
		return;
	}
	pSwapChainVtable = (uintptr_t*)((uintptr_t*)SwapChain)[0];
	oPresent = (DX11PresentFnPresent)DetourVTable((void**)pSwapChainVtable, 8, (void*)hookD3D11Present);
	oResize = (DX11ResizeFnResize)DetourVTable((void**)pSwapChainVtable, 13, (void*)Resize);
	D3DDevice->Release();
	D3DContext->Release();
	SwapChain->Release();
	DWORD dwOld;
	VirtualProtect(hookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &dwOld);
	OrigWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProcHook);
}
BOOL __stdcall DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved)
{
	PROCESS_MITIGATION_EXTENSION_POINT_DISABLE_POLICY policy; policy.DisableExtensionPoints = 1;
	PROCESS_MITIGATION_DEP_POLICY  policyf; policyf.DisableAtlThunkEmulation = 1;
	_PROCESS_MITIGATION_ASLR_POLICY  policyg; policyg.EnableBottomUpRandomization = 1;
	_PROCESS_MITIGATION_ASLR_POLICY  policyj; policyj.EnableForceRelocateImages = 1;
	_PROCESS_MITIGATION_ASLR_POLICY  policyb; policyb.EnableHighEntropy = 1;
	_PROCESS_MITIGATION_ASLR_POLICY  policyc; policyc.DisallowStrippedImages = 1;

	// тут защита

	if (dwCallReason == DLL_PROCESS_ATTACH)
	{
#if AllocCons == 1
		AllocConsole();
		freopen(StrA("CONIN$"), "r", stdin);
		freopen(StrA("CONOUT$"), "w", stderr);
		freopen(StrA("CONOUT$"), "w", stdout);
#endif
		srand((unsigned)time(NULL));
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		il2cpp::init();
		ChamsOkey = GetModBase(cry(L"GameAssembly.dll")) + 0x73DF60; // private void SetSkinColor(Color skinColor) { }
		CanAttack = GetModBase(cry(L"GameAssembly.dll")) + O::BasePlayer::CanAttack;
		NightMode = GetModBase(cry(L"GameAssembly.dll")) + O::TOD_Sky::get_Instance;
		HitSND = GetModBase(cry(L"GameAssembly.dll")) + O::BaseCombatEntity::DoHitNotify;
		CreateProjectile = GetModBase(cry(L"GameAssembly.dll")) + O::BaseProjectile::CreateProjectile;
		//SENDPRJ = GetModBase(cry(L"GameAssembly.dll")) + O::BasePlayer::SendProjectileAttack;
		ClientInput = GetModBase(cry(L"GameAssembly.dll")) + O::BasePlayer::ClientInput;
		DoFixedUpdate = GetModBase(cry(L"GameAssembly.dll")) + O::PlayerWalkMovement::DoFixedUpdate;
		sendClientTick = GetModBase(cry(L"GameAssembly.dll")) + O::BasePlayer::SendClientTick;
		AimDirection = GetModBase(cry(L"GameAssembly.dll")) + 0x984350; //public static Vector3 GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside = True) { }
		ServerRPC = GetModBase(cry(L"GameAssembly.dll")) + 0xF57660; // E8 ? ? ? ? 49 8B 06 48 8D 4D AF
		DetourAttach(&(LPVOID&)CanAttack, &JumpShoot);
		DetourAttach(&(LPVOID&)AimDirection, &pSilent);
		DetourAttach(&(LPVOID&)NightMode, &NightModeFunction);
		DetourAttach(&(LPVOID&)CreateProjectile, &Fix);
		DetourAttach(&(LPVOID&)sendClientTick, &AntiAim);
		DetourAttach(&(LPVOID&)HitSND, &HitSound);
		DetourAttach(&(LPVOID&)ChamsOkey, &ChamsESP);
		//DetourAttach(&(LPVOID&)SENDPRJ, &Silent);
		DetourAttach(&(LPVOID&)ClientInput, &ClientInputxD);
		DetourAttach(&(LPVOID&)DoFixedUpdate, &SlideWalk);
		DetourTransactionCommit();
		CreateThreadSafe(Start);
	}
	return TRUE;
}
