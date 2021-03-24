bool mfound = false;
void FindMatrix() {
	UINT64 ObjMgr = safe_read(GetModBase(StrW(cry(L"UnityPlayer.dll"))) + oGameObjectManager, UINT64);
	UINT64 end = safe_read(ObjMgr, UINT64);
	for (UINT64 Obj = safe_read(ObjMgr + 0x8, UINT64); (Obj && (Obj != end)); Obj = safe_read(Obj + 0x8, UINT64))
	{
		UINT64 GameObject = safe_read(Obj + 0x10, UINT64);
		WORD Tag = safe_read(GameObject + 0x54, WORD);
		if (Tag == 5) {
			UINT64 ObjClass = safe_read(GameObject + 0x30, UINT64);
			UINT64	Entity = safe_read(ObjClass + 0x18, UINT64);
			LocalPlayer.pViewMatrix = (Matrix4x4*)(Entity + 0xDC);
			printf(StrA("Found matrix!\n"));
			mfound = true;
			return;
		}
	}
}
void EntityZaLoop()
{
	bool LP_isValid = false;
	float FOV = 1000000.f;
	float FOV1 = Vars::AimBot::pSilentFov;
	BasePlayer* AimEntity = 0;
	if (!LocalPlayer.pViewMatrix || !mfound) {
		FindMatrix();
	}

	DWORD64 BaseNetworkable;
    BaseNetworkable = safe_read(GetModBase(StrW(cry(L"GameAssembly.dll"))) + g_BN, DWORD64);
	DWORD64 EntityRealm = safe_read(BaseNetworkable + 0xB8, DWORD64);
	DWORD64 ClientEntities = safe_read(EntityRealm, DWORD64);
	DWORD64 ClientEntities_list = safe_read(ClientEntities + 0x10, DWORD64);
	DWORD64 ClientEntities_values = safe_read(ClientEntities_list + 0x28, DWORD64);
	if (!ClientEntities_values) return;
	int EntityCount = safe_read(ClientEntities_values + 0x10, int);
	DWORD64 EntityBuffer = safe_read(ClientEntities_values + 0x18, DWORD64);
	
	for (int i = 0; i <= EntityCount; i++)
	{
		DWORD64 Entity = safe_read(EntityBuffer + 0x20 + (i * 0x8), DWORD64);
		if (Entity <= 100000) continue;
		DWORD64 Object = safe_read(Entity + 0x10, DWORD64);
		if (Object <= 100000) continue;
		DWORD64 ObjectClass = safe_read(Object + 0x30, DWORD64);
		if (ObjectClass <= 100000) continue;
		pUncStr name = safe_read(ObjectClass + 0x60, pUncStr);
		if (!name) continue;
		char* buff = name->stub;
		DWORD64 ent = safe_read(Object + 0x28, UINT64);
		if (strstr(buff, StrA(cry("Local")))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x50, DWORD64) || safe_read(Player + 0x18, bool) || !safe_read(Player + O::BasePlayer::playerModel, DWORD64)) continue;
			if (Player != LocalPlayer.BasePlayer /*lp changed*/) {
				mfound = false;
			}
			Vector3 pos = Player->GetPosition();
			LocalPlayer.BasePlayer = Player;
			LP_isValid = true;

		}
		else if ((strstr(buff, StrA(cry("scientist"))) && (!strstr(buff, StrA("prop")) && !strstr(buff, StrA("corpse")) && !strstr(buff, StrA("turret")) && !strstr(buff, StrA("cargo"))))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x50, DWORD64) || safe_read(Player + 0x18, bool) || !safe_read(Player + O::BasePlayer::playerModel, DWORD64)) continue;
			if (!Player->IsValid()) continue;
			if(!Vars::Visuals::IgnoreNpc) ESP(Player, LocalPlayer.BasePlayer, true);
			if (Vars::AimBot::IgnoreNpc) continue;
			if (Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), Player->GetBoneByID(head)) > Vars::AimBot::Range)
				continue;

			float CurFOV;
			if (Player->IsVisible() && (FOV > (CurFOV = GetFov(Player, BoneList(head))))) {
				if (CurFOV <= Vars::AimBot::Fov) {
					FOV = CurFOV; AimEntity = Player;
				}
			}
		}
		else if (strstr(buff, StrA(cry("player.prefab"))) && (!strstr(buff, StrA("prop")) && !strstr(buff, StrA("corpse")))) {
			BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
			if (!safe_read(Player + 0x50, DWORD64) || safe_read(Player + 0x18, bool) || !safe_read(Player + O::BasePlayer::playerModel, DWORD64)) continue;
			if (!Player->IsValid()) continue;
			bool ex = false;
			ESP(Player, LocalPlayer.BasePlayer);
			if (Vars::Visuals::IgnoreSleepers && Player->HasFlags(16)) continue;
			if (Vars::AimBot::IgnoreTeam) {
				ex = Player->IsTeamMate(LocalPlayer.BasePlayer);
			}
			if (ex) continue;
			if (Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), Player->GetBoneByID(head)) > Vars::AimBot::Range)
				continue;

			float CurFOV;
			if (Player->IsVisible() && (FOV > (CurFOV = GetFov(Player, BoneList(head))))) {
				if (CurFOV <= Vars::AimBot::Fov) {
					FOV = CurFOV; AimEntity = Player;
				}
			}
			if (ex) continue;
			if (Player->HasFlags(16)) continue;
			if (Player->IsVisible() && (FOV1 > (CurFOV = GetFov(Player, BoneList(head))))) {
				if (CurFOV <= Vars::AimBot::pSilentFov) {
					FOV1 = CurFOV; AimEntity = Player;
					float mindist = 400.f;
					float Dayn = GetFov(Player, head);
					if (Dayn < mindist) {
						mindist = Dayn;
						Vars::PlayerxD = (uintptr_t)Player;
					}
				}
			}
			if (Vars::PlayerxD)
			{
				if (FOV < (CurFOV = GetFov((BasePlayer*)Vars::PlayerxD, BoneList(head))))
				{
					Vars::PlayerxD = 0;
				}
			}
		}
		else if (Vars::Visuals::Shkaf && strstr(buff, cry("cupboard.tool.deployed.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Шкаф] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::Black, 2.6f);
				GUI::Render.String(Pos, res, D2D1::ColorF::DarkOrange, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("door.hinged.wood.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Деревянная дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("door.hinged.wood.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Деревянная дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("door.double.hinged.wood.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Деревянная дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("door.hinged.metal.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Металлическая дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("door.double.hinged.metal.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Металлическая дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("door.hinged.toptier.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[МВК дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("door.double.hinged.toptier.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[МВК дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Door && strstr(buff, cry("wall.frame.garagedoor.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Гаражная дверь] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);
			}
		}
		else if (Vars::Visuals::Sulphur && strstr(buff, cry("sulfur-ore.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Сера] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::Goldenrod, 2.5f);
			}
		}
		else if (Vars::Visuals::Copter && strstr(buff, cry("minicopter.entity.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Коптер] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::DarkOliveGreen, 2.5f);
			}
		}
		else if (Vars::Visuals::Stone && strstr(buff, cry("stone-ore.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Камень] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::LightGray, 2.5f);
			}
		}
		else if (Vars::Visuals::Stash && strstr(buff, cry("small_stash_deployed.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Стэш] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::LightPink, 2.5f);
			}
		}
		else if (Vars::Visuals::Hemp && strstr(buff, cry("hemp-collectable.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Ткань] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::CadetBlue, 2.5f);
			}
		}
		else if (Vars::Visuals::Metal && strstr(buff, cry("metal-ore.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Металл] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::Firebrick, 2.5f);
			}
		}
		else if (Vars::Visuals::PlayerDead && strstr(buff, cry("player_corpse.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Труп] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::RosyBrown, 2.5f);
			}
		}

		else if (Vars::Visuals::Chest && strstr(buff, cry("box.wooden.large.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Сундук] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::WhiteSmoke, 2.5f);

			}
		}
		else if (Vars::Visuals::Landmine && strstr(buff, cry("landmine.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Мина] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::OliveDrab, 2.5f);

			}
		}
		else if (Vars::Visuals::Guntrap && strstr(buff, cry("guntrap.deployed.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Гантрап] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::NavajoWhite, 2.5f);

			}
		}

		else if (Vars::Visuals::Airdrop && strstr(buff, cry("supply_drop.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Аирдроп] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::Firebrick, 2.5f);
			}
		}

		else if (Vars::Visuals::Barrel && strstr(buff, cry("loot-barrel-1.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Бочка] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::SandyBrown, 2.5f);
			}
		}
		else if (Vars::Visuals::Barrel && strstr(buff, cry("loot-barrel-2.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Бочка] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::SandyBrown, 2.5f);
			}
		}
		else if (Vars::Visuals::Barrel && strstr(buff, cry("loot_barrel_1.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Бочка] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::SandyBrown, 2.5f);
			}
		}
		else if (Vars::Visuals::Barrel && strstr(buff, cry("loot_barrel_2.prefab"))) {
			wchar_t res[64];
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				_swprintf(res, L"[Бочка] %d m", (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetPosition(), pos));
				GUI::Render.String(Pos, res, D2D1::ColorF::SandyBrown, 2.5f);
			}
		}
		else if (Vars::Visuals::Helicopter && strstr(buff, cry("assets/prefabs/npc/patrol helicopter/patrolhelicopter.prefab"))) {
			DWORD64 Res = safe_read(Object + 0x28, DWORD64);
			DWORD64 gameObject = safe_read(ObjectClass + 0x30, DWORD64); //Tag 449
			DWORD64 Trans = safe_read(gameObject + 0x8, DWORD64);
			DWORD64 Vec = safe_read(Trans + 0x38, DWORD64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			Vector2 Pos;
			if (LocalPlayer.WorldToScreen(pos, Pos)) {
				int distation = (int)Math::Calc3D_Dist(LocalPlayer.BasePlayer->GetBoneByID(head), pos);
				wchar_t text[0x100];
				g_Helicopter = Res;
				_swprintf(text, L"[Вертолет] %d m", distation);
				GUI::Render.String(Pos, text, D2D1::ColorF::LightGreen, 2.5f);
			}
		}
	}
	if (LP_isValid)
	{
		WeaponPatch();
		Fov();
		Aim(AimEntity);
		Misc();
	}
	else LocalPlayer.BasePlayer = nullptr;
}