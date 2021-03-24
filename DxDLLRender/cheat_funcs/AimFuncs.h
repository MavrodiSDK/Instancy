float w_last_syringe = 0.f;
bool w_healing = false;
void WeaponPatch()
{
	WeaponData* weapon1 = LocalPlayer.BasePlayer->GetActiveWeapon();
	if (Vars::Misc::FastHeal && (weapon1->GetID() == 1079279582 || weapon1->GetID() == -2072273936) && LocalPlayer.BasePlayer->GetHealth() < 95.f) {
		typedef void(__stdcall* TestS)(DWORD64, Str);
		DWORD64 Held = safe_read(weapon1 + O::Item::heldEntity, DWORD64);
		safe_write(Held + O::Item::heldEntity, 1.f, float); //disable animation
		float curtime = LocalPlayer.BasePlayer->GetTickTime();
		bool deployed = safe_read(Held + 0x188, bool); // protected bool isDeployed;
		//0x800 secondary
		if (deployed && curtime > w_last_syringe + 0.5f) {
			if (w_healing) {
				TestS ss = (TestS)((DWORD64)GetModBase((L"GameAssembly.dll")) + 0x3B5C90); //public void ServerRPC(string funcName) { }
				ss(Held, Str(L"UseSelf"));
			}
			else curtime += 0.1f;
			w_last_syringe = curtime;
			w_healing = true;
		}
		else if (!deployed) {
			w_healing = false;
		}
	}
	else {
		w_healing = false;
	}
	WeaponData* weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
	if (weapon == nullptr) return;
	Weapon target = weapon->Info();
	if (!target.id) return;
	if (!target.category) return;
	if (target.category == 1) {
		weapon->RunOnHit();
		return;
	}
	if (target.category == -2) {
		return;
	}
	weapon->SetRA();
	if (target.category == 2) {
		weapon->SetAutomatic();
	}
	else if (target.category == -1) {
		weapon->SuperEoka();
	}
}

float GetFov(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!LocalPlayer.WorldToScreen(Entity->GetBoneByID(Bone), ScreenPos)) return 1000.f;
	return Math::Calc2D_Dist(Vector2(Vars::Global::ScreenWidth / 2, Vars::Global::ScreenHigh / 2), ScreenPos);
}

float GetGravity(int ammoid) {
	switch (ammoid) {
	case 14241751:
		return 1.f;
	case -1234735557:
		return 0.75f;
	case 215754713:
		return 0.75f;
	case -1023065463:
		return 0.5f;
	case -2097376851:
		return 0.75f;
	case -1321651331:
		return 1.25f;
	default:
		return 1.f;
	}
}

float GetBulletSpeed(Weapon tar, int ammo)
{
	if (ammo == 0) return tar.ammo[0].speed;
	for (Ammo am : tar.ammo) {
		for (int id : am.id) {
			if (id == ammo) {
				return am.speed;
			}
		}
		if (am.id[0] == 0) return am.speed;
	}
	return 250.f;
}

Vector3 Prediction(Vector3 LP_Pos, BasePlayer* Player, BoneList Bone)
{
	WeaponData* active = LocalPlayer.BasePlayer->GetActiveWeapon();
	Weapon tar = active->Info();
	int ammo = active->LoadedAmmo();
	Vector3 BonePos = Player->GetBoneByID(neck);
	float Dist = Math::Calc3D_Dist(LP_Pos, BonePos);
	if (Dist > 0.001f) {
		float speed = GetBulletSpeed(tar, ammo);
		if (!speed) speed = 250.f;
		float BulletTime = Dist / speed;
		Vector3 vel = Player->GetVelocity();
		Vector3 PredictVel = vel * BulletTime * 0.75f;
		BonePos += PredictVel;
		float gravity = GetGravity(ammo);
		BonePos.y += (4.905f * BulletTime * BulletTime) * gravity;
	}
	return BonePos;
}

void Normalize(float& Yaw, float& Pitch) {
	if (Pitch < -89) Pitch = -89;
	else if (Pitch > 89) Pitch = 89;
	if (Yaw < -360) Yaw += 360;
	else if (Yaw > 360) Yaw -= 360;
}

void GoToTarget(BasePlayer* player)
{
	Vector3 Local = LocalPlayer.BasePlayer->GetBoneByID(neck);
	Vector3 PlayerPos = Prediction(Local, player, BoneList(Vars::Global::BoneToAim));
	Vector2 RecoilAng = LocalPlayer.BasePlayer->GetRA();
	Vector2 Offset = Math::CalcAngle(Local, PlayerPos) - LocalPlayer.BasePlayer->GetVA();
	Normalize(Offset.y, Offset.x);
	Vector2 AngleToAim = LocalPlayer.BasePlayer->GetVA() + Offset;
	AngleToAim = AngleToAim - RecoilAng;
	Normalize(AngleToAim.y, AngleToAim.x);
	LocalPlayer.BasePlayer->SetVA(AngleToAim);
}

void Aim(BasePlayer* AimEntity)
{
	if (Vars::AimBot::Activate)
	{
		if (AimEntity && !LocalPlayer.BasePlayer->IsMenu()) {
			if (FC(user32, GetAsyncKeyState, VK_RBUTTON))
				GoToTarget(AimEntity);
		}
	}
}