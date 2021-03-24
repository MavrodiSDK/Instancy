void Crosshair() 
{
	float xs = Vars::Global::ScreenWidth / 2, ys = Vars::Global::ScreenHigh / 2;
	GUI::Render.Line(Vector2{ xs - -10, ys }, Vector2{ xs + 5, ys }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ xs, ys - -10 }, Vector2{ xs , ys + 5 }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ xs - -10, ys }, Vector2{ xs + 5, ys }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ xs, ys - 4 }, Vector2{ xs , ys + -10 }, D2D1::ColorF::Red);
	GUI::Render.Line(Vector2{ xs - 4, ys }, Vector2{ xs + -10, ys }, D2D1::ColorF::Red);
}

void Slots(BasePlayer* BP, float Pos_x, float Pos_Y)
{
	float Height = 160.f;
	float With = 200.f;
	float maxheal = 100.f;
	float Pos = 0;
	int health = BP->GetHealth();
	GUI::Render.FillRoundedRectangle({ Pos_x, Pos_Y }, { With, Height }, { 0.f, 0.f, 0.f, 0.8f }, 8.f);
	GUI::Render.FillRectangle(Vector2{ (With / 2), Pos_Y + 40 + Pos }, Vector2{ 5, 100.f * (health / maxheal) }, D2D1::ColorF::LightGreen);
	GUI::Render.String({ 10.f + (With / 2), Pos_Y + 25 }, BP->GetName(), D2D1::ColorF::White);
	for (int i = 0; i < 6; i++)
	{
		WeaponData* ActWeapon = BP->GetWeaponInfo(i);
		if (ActWeapon)
		{
			const wchar_t* ActiveItem = BP->GetWeaponInfo(i)->GetName();
			if (wcslen(ActiveItem) < 20)
			{
				GUI::Render.String({ 10.f + (With / 2), Pos_Y + 40 + Pos }, ActiveItem, D2D1::ColorF::White);
			}
		}
		else {
			GUI::Render.String({ Pos_x + (With / 2), Pos_Y + 40 + Pos }, L"[-]", D2D1::ColorF::White);
		}
		Pos += 15;
	}
}

float GetFovV2(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!LocalPlayer.WorldToScreen(Entity->GetBoneByID(Bone), ScreenPos)) return 1000.f;
	return Math::Calc2D_Dist(Vector2(Vars::Global::ScreenWidth / 2, Vars::Global::ScreenHigh / 2), ScreenPos);
}

void DrawFOV()
{
	float xs = Vars::Global::ScreenWidth / 2, ys = Vars::Global::ScreenHigh / 2;
	GUI::Render.Сircle(Vector2{ xs, ys }, D2D1::ColorF::Black, Vars::AimBot::Fov, 1.005f);
	GUI::Render.Сircle(Vector2{ xs, ys }, D2D1::ColorF::Black, Vars::AimBot::Fov);
}

void DrawFOV1()
{
	float xs = Vars::Global::ScreenWidth / 2, ys = Vars::Global::ScreenHigh / 2;
	GUI::Render.Сircle(Vector2{ xs, ys }, D2D1::ColorF(0.3f, 0.34f, 1.f), Vars::AimBot::pSilentFov, 1.005f);
	GUI::Render.Сircle(Vector2{ xs, ys }, D2D1::ColorF(0.3f, 0.34f, 1.f), Vars::AimBot::pSilentFov);
}


void __fastcall ChamsESP(void* a1, D2D1::ColorF)
{
	if (Vars::Visuals::Chams) {
		typedef void(__fastcall* Chams_fn)(void* a1, D2D1::ColorF a2);
		return ((Chams_fn)ChamsOkey)(a1, D2D1::ColorF(255, 0, 0, 1));
	}
}
void __fastcall NightModeFunction(void* a1, void* a2) {
	if (Vars::Visuals::NightMode) {
		typedef void(__stdcall* F)(float);
		typedef void(__stdcall* I)(int);
		typedef void(__stdcall* C)(D2D1::ColorF);
		((I)(GetModBase(StrW(L"GameAssembly.dll")) + 0x1866840))(3);// public static void set_ambientMode(AmbientMode value) { }
		((F)(GetModBase(StrW(L"GameAssembly.dll")) + 0x1866780))(1.f);//public static void set_ambientIntensity(float value) { }
		((C)(GetModBase(StrW(L"GameAssembly.dll")) + 0x1866800))(D2D1::ColorF(0.8f, 0.8f, 0.8f, 0.8f));//public static void set_ambientLight(Color value) { }
	}
	typedef void(__stdcall* Inp)(void*, void*);
	return ((Inp)NightMode)(a1, a2);
}
void ESP(BasePlayer* BP, BasePlayer* LP, bool npc = false)
{
	bool PlayerSleeping = BP->HasFlags(16);
	if (Vars::Visuals::IgnoreSleepers && PlayerSleeping)
		return;

	Vector2 tempFeetR, tempFeetL;
	Vector3 ro = BP->GetBoneByID(r_foot);
	if (ro.x == 0 && ro.y == 0 && ro.z == 0) return;
	if (LocalPlayer.WorldToScreen(ro, tempFeetR) && LocalPlayer.WorldToScreen(BP->GetBoneByID(l_foot), tempFeetL))
	{
		Vector2 tempHead;
		if (LocalPlayer.WorldToScreen(BP->GetBoneByID(jaw) + Vector3(0.f, 0.16f, 0.f), tempHead))
		{
			Vector2 tempFeet = (tempFeetR + tempFeetL) / 2.f;
			float Entity_h = tempHead.y - tempFeet.y;
			float w = Entity_h / 4;
			float Entity_x = tempFeet.x - w;
			float Entity_y = tempFeet.y;
			float Entity_w = Entity_h / 2;

			bool PLOS(Vector3 a, Vector3 b);

			if (Vars::Visuals::ShowPlayerBoxCorner) {
				if (BP->IsTeamMate(LocalPlayer.BasePlayer)) {
					GUI::Render.Line({ Entity_x, Entity_y }, { Entity_x + Entity_w / 3.5f,Entity_y }, D2D1::ColorF::Cyan);
					GUI::Render.Line({ Entity_x, Entity_y }, { Entity_x,Entity_y + Entity_h / 3.5f }, D2D1::ColorF::Cyan);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y }, { (Entity_x + Entity_w) - Entity_w / 3.5f,Entity_y }, D2D1::ColorF::Cyan);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y }, { Entity_x + Entity_w,Entity_y + Entity_h / 3.5f }, D2D1::ColorF::Black, 3.f);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y }, { Entity_x + Entity_w,Entity_y + Entity_h / 3.5f }, D2D1::ColorF::Cyan);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y + Entity_h }, { (Entity_x + Entity_w) - Entity_w / 3.5f,Entity_y + Entity_h }, D2D1::ColorF::Cyan);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y + Entity_h }, { Entity_x + Entity_w,(Entity_y + Entity_h) - Entity_h / 3.5f }, D2D1::ColorF::Cyan);
					GUI::Render.Line({ Entity_x, Entity_y + Entity_h }, { Entity_x + Entity_w / 3.5f,Entity_y + Entity_h }, D2D1::ColorF::Black, 3.f);
					GUI::Render.Line({ Entity_x, Entity_y + Entity_h }, { Entity_x + Entity_w / 3.5f,Entity_y + Entity_h }, D2D1::ColorF::Cyan);
					GUI::Render.Line({ Entity_x, Entity_y + Entity_h }, { Entity_x,(Entity_y + Entity_h) - Entity_h / 3.5f }, D2D1::ColorF::Cyan);
					//GUI::Render.FillRectangle(Vector2{ Entity_x, Entity_y }, Vector2{ Entity_w, Entity_h }, D2D1::ColorF::Cyan);
				}
				else {
					GUI::Render.Line({ Entity_x, Entity_y }, { Entity_x + Entity_w / 3.5f,Entity_y }, D2D1::ColorF::Red);
					GUI::Render.Line({ Entity_x, Entity_y }, { Entity_x,Entity_y + Entity_h / 3.5f }, D2D1::ColorF::Red);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y }, { (Entity_x + Entity_w) - Entity_w / 3.5f,Entity_y }, D2D1::ColorF::Red);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y }, { Entity_x + Entity_w,Entity_y + Entity_h / 3.5f }, D2D1::ColorF::Red);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y + Entity_h }, { (Entity_x + Entity_w) - Entity_w / 3.5f,Entity_y + Entity_h }, D2D1::ColorF::Red);
					GUI::Render.Line({ Entity_x + Entity_w, Entity_y + Entity_h }, { Entity_x + Entity_w,(Entity_y + Entity_h) - Entity_h / 3.5f }, D2D1::ColorF::Red);
					GUI::Render.Line({ Entity_x, Entity_y + Entity_h }, { Entity_x + Entity_w / 3.5f,Entity_y + Entity_h }, D2D1::ColorF::Red);
					GUI::Render.Line({ Entity_x, Entity_y + Entity_h }, { Entity_x,(Entity_y + Entity_h) - Entity_h / 3.5f }, D2D1::ColorF::Red);
					//GUI::Render.FillRectangle(Vector2{ Entity_x, Entity_y }, Vector2{ Entity_w, Entity_h }, D2D1::ColorF::Red);
				}
			}

			bool PlayerWounded = BP->HasFlags(64);
			auto PlayerClr = !npc ? (PlayerSleeping ? D2D1::ColorF::Orange : (PlayerWounded ? D2D1::ColorF:: IndianRed : (D2D1::ColorF::Red))) : D2D1::ColorF({ 0, 0.85, 1, 1 });


			int CurPos = 0;

			float ActiveSlotPos_X = 70.f;
			float ActiveSlotPos_Y = 300.f;

			if (Vars::Visuals::ShowPlayerActiveSlot && GetFovV2(BP, head) <= 150)
				Slots(BP, ActiveSlotPos_X, ActiveSlotPos_Y);


			if (Vars::Visuals::ShowPlayerName || Vars::Visuals::ShowPlayerDist) {
				wchar_t res[64];
				if (wcslen(BP->GetName()) > 0) {
					if (Vars::Visuals::ShowPlayerName && Vars::Visuals::ShowPlayerDist) {
						_swprintf(res, L"%s [%d M]", BP->GetName(), (int)Math::Calc3D_Dist(LP->GetBoneByID(head), ro));
					}
					else if (Vars::Visuals::ShowPlayerDist) {
						_swprintf(res, L"[%d M]", (int)Math::Calc3D_Dist(LP->GetBoneByID(head), ro));
					}
					else {
						_swprintf(res, L"%s", BP->GetName());
					}
					GUI::Render.String(Vector2{ Entity_x + Entity_w, Entity_y + Entity_h - 20.f }, res, D2D1::ColorF::Black, 1.2f);
					GUI::Render.String(Vector2{ Entity_x + Entity_w, Entity_y + Entity_h - 20.f }, res, D2D1::ColorF::White, 1.2f);
				}
			}
			if (BP != NULL)
			{
				auto* TargetPlayer = reinterpret_cast<BasePlayer*>(Vars::PlayerxD);
				static float screenX = GetSystemMetrics(SM_CXSCREEN);
				static float screenY = GetSystemMetrics(SM_CYSCREEN);
				static Vector2 startPos = Vector2(screenX / 2.f, screenY - 600.f);	
				Vector2 ScreenPos;
				if (Vars::Visuals::TargetLine)
				{
					if (!(TargetPlayer->GetBoneByID(spine1).x == 0 && TargetPlayer->GetBoneByID(spine1).y == 0 && TargetPlayer->GetBoneByID(spine1).z == 0)) {
						if (LocalPlayer.WorldToScreen(TargetPlayer->GetBoneByID(head), ScreenPos))
							GUI::Render.Line(startPos, ScreenPos, D2D1::ColorF::Red, 1.f);
					}
				}
			}
			if (Vars::Visuals::ShowPlayerHealth) {
				Entity_y += 5;
				//constexpr int chunk_size = 1;
				int health = (int)BP->GetHealth();
				float maxheal = (npc ? 300.f : 100.f);
				float minheal = (npc ? 300.f : 100.f);
				//float anal = abs(Entity_h) / 7;
				//int chunks = (abs(Entity_w) / chunk_size) * min(1.f, health / maxheal);
				//for (int idx = 0; idx < chunks; idx++) {
				//	float r = min((2.0f * (chunks - idx)) / chunks, 1.0f);
				//	float g = min((2.0f * idx) / chunks, 1.0f);
				//	D2D1::ColorF clr = D2D1::ColorF(r, g, 0.f, 1.f);
				//	//GUI::Render.FillRectangle(Vector2(Entity_x - idx * chunk_size, Entity_y), Vector2(-chunk_size, anal), clr);
				//}
				wchar_t res1[64];
				_swprintf(res1, L"%d [HP]", health);
				GUI::Render.String(Vector2{ Entity_x + Entity_w, Entity_y + Entity_h - 38.f }, res1, D2D1::ColorF::Black, 1.2f);
				GUI::Render.String(Vector2{ Entity_x + Entity_w, Entity_y + Entity_h - 38.f }, res1, PlayerClr);
			}

			if (Vars::Visuals::ShowPlayerWeapon)
			{
				WeaponData* ActWeapon = BP->GetActiveWeapon();
				if (ActWeapon)
				{
					const wchar_t* ActiveWeaponName;
					ActiveWeaponName = ActWeapon->GetName();
					GUI::Render.String(Vector2{ Entity_x - 0.f, Entity_y - 0.f }, ActiveWeaponName, D2D1::ColorF::Black, 1.2f);
					GUI::Render.String(Vector2{ Entity_x - 0.f, Entity_y - 0.f }, ActiveWeaponName, PlayerClr);
				}
			}
		}
	}
}