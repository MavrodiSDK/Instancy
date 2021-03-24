#include <cstdint>
#include "../offsets.h"
#include "E:\Source [Instancy]\DxDLLRender\xorstr.h"
#include "../il2cpp.hpp"
////////////////////////////////////
#define MAX_STRING_LEN 64
#define g_BN 0x2FF92B0 //   "Signature": "BaseNetworkable_c*"
#define oGameObjectManager 0x17C1F18 
////////////////////////////////////
ULONG64 fpTransform = GetModBase(StrW(L"GameAssembly.dll")) + O::UnityEngine_Transform::get_position;

enum BoneList : int
{
	pelvis = 0,
	l_hip = 1,
	l_knee = 2,
	l_foot = 3,
	l_toe = 4,
	l_ankle_scale = 5,
	penis = 6,
	GenitalCensor = 7,
	GenitalCensor_LOD0 = 8,
	Inner_LOD0 = 9,
	GenitalCensor_LOD1 = 11,
	GenitalCensor_LOD2 = 12,
	r_hip = 13,
	r_knee = 14,
	r_foot = 15,
	r_toe = 16,
	r_ankle_scale = 17,
	spine1 = 18,
	spine1_scale = 19,
	spine2 = 20,
	spine3 = 21,
	spine4 = 22,
	l_clavicle = 23,
	l_upperarm = 24,
	l_forearm = 25,
	l_hand = 26,
	l_index1 = 27,
	l_index2 = 28,
	l_index3 = 29,
	l_little1 = 30,
	l_little2 = 31,
	l_little3 = 32,
	l_middle1 = 33,
	l_middle2 = 34,
	l_middle3 = 35,
	l_prop = 36,
	l_ring1 = 37,
	l_ring2 = 38,
	l_ring3 = 39,
	l_thumb1 = 40,
	l_thumb2 = 41,
	l_thumb3 = 42,
	IKtarget_righthand_min = 43,
	IKtarget_righthand_max = 44,
	l_ulna = 45,
	neck = 46,
	head = 47,
	jaw = 48,
	eyeTranform = 49,
	l_eye = 50,
	l_Eyelid = 51,
	r_eye = 52,
	r_Eyelid = 53,
	r_clavicle = 54,
	r_upperarm = 55,
	r_forearm = 56,
	r_hand = 57,
	r_index1 = 58,
	r_index2 = 59,
	r_index3 = 60,
	r_little1 = 61,
	r_little2 = 62,
	r_little3 = 63,
	r_middle2 = 65,
	r_middle3 = 66,
	r_prop = 67,
	r_ring1 = 68,
	r_ring2 = 69,
	r_ring3 = 70,
	r_thumb1 = 71,
	r_thumb2 = 72,
	r_thumb3 = 73,
	IKtarget_lefthand_min = 74,
	IKtarget_lefthand_max = 75,
	r_ulna = 76,
	l_breast = 77,
	r_breast = 78,
	BoobCensor = 79,
	BreastCensor_LOD0 = 80,
	BreastCensor_LOD1 = 83,
	BreastCensor_LOD2 = 84
};
enum class layer : uint32_t {
	Default = 0,
	TransparentFX = 1,
	Ignore_Raycast = 2,
	Reserved1 = 3,
	Water = 4,
	UI = 5,
	Reserved2 = 6,
	Reserved3 = 7,
	Deployed = 8,
	Ragdoll = 9,
	Invisible = 10,
	AI = 11,
	PlayerMovement = 12,
	Vehicle_Detailed = 13,
	Game_Trace = 14,
	Vehicle_World = 15,
	World = 16,
	Player_Server = 17,
	Trigger = 18,
	Player_Model_Rendering = 19,
	Physics_Projectile = 20,
	Construction = 21,
	Construction_Socket = 22,
	Terrain = 23,
	Transparent = 24,
	Clutter = 25,
	Debris = 26,
	Vehicle_Large = 27,
	Prevent_Movement = 28,
	Prevent_Building = 29,
	Tree = 30,
	Unused2 = 31
};
enum class HitTestType
{
	Generic = 0,
	ProjectileEffect = 1,
	Projectile = 2,
	MeleeAttack = 3,
	Use = 4,
};

typedef struct _UncStr
{
	char stub[0x10];
	int len;
	wchar_t str[1];
} *pUncStr;

class WeaponData
{

public:
	int LoadedAmmo() {
		DWORD64 Held = safe_read((DWORD64)this + O::Item::heldEntity, DWORD64);
		if (Held <= 0) return 0;
		DWORD64 Magazine = safe_read(Held + O::BaseProjectile::primaryMagazine, DWORD64);
		if (Magazine <= 0 || Magazine == 0x3F000000) {
			return 0;
		}
		DWORD64 ammoType = safe_read(Magazine + 0x20, DWORD64); //?
		if (ammoType <= 0 || ammoType == 0x3F000000) {
			return 0;
		}
		int ammo = safe_read(ammoType + 0x18, int);
		return ammo;
	}

	char* ClassName() {
		return (char*)safe_read(safe_read(safe_read(this + O::Item::heldEntity, DWORD64), DWORD64) + 0x10, DWORD64);
	}

	int GetID() {
		DWORD64 Info = safe_read((DWORD64)this + 0x20, DWORD64);
		int ID = safe_read(Info + 0x18, int);
		return ID;
	}

	Weapon Info() {
		int ID = GetID();
		for (Weapon k : info) {
			if (k.id == ID) {
				return k;
			}
		}
		return Weapon{ 0 };
	}

	wchar_t* GetShortName(int* len)
	{
		DWORD64 Info = safe_read(this + 0x20, DWORD64);
		pUncStr Str = ((pUncStr)safe_read(Info + 0x20, DWORD64));
		int leng = safe_read(Str + 0x10, int);
		if (!leng) return nullptr;
		if (len)*len = leng;
		return Str->str;
	}

	wchar_t* GetName() {
		DWORD64 Info = safe_read(this + 0x20, DWORD64);
		DWORD64 DispName = safe_read(Info + 0x28, DWORD64);
		pUncStr Str = ((pUncStr)safe_read(DispName + 0x18, DWORD64));
		if (!Str) return nullptr; return Str->str;
	}

	float& gatherDamage() {
		return *reinterpret_cast<float*>((uintptr_t)this + 0x10);
	}

	float& destroyFraction() {
		return *reinterpret_cast<float*>((uintptr_t)this + 0x14);
	}

	float& conditionLost() {
		return *reinterpret_cast<float*>((uintptr_t)this + 0x18);
	}

	float get_repeat_delay() {
		return *reinterpret_cast<float*>((uintptr_t)this + O::AttackEntity::repeatDelay);
	}

	float get_next_attack_time() {
		return *reinterpret_cast<float*>((uintptr_t)this + O::AttackEntity::nextAttackTime);
	}

	float get_time_since_deploy() {
		return *reinterpret_cast<float*>((uintptr_t)this + O::AttackEntity::timeSinceDeploy);
	}

	float get_deploy_delay() {
		return *reinterpret_cast<float*>((uintptr_t)this + O::AttackEntity::deployDelay);
	}

	WeaponData*& Tree() {
		return *reinterpret_cast<WeaponData**>((uintptr_t)this + 0x10);
	}

	WeaponData*& Ore() {
		return *reinterpret_cast<WeaponData**>((uintptr_t)this + 0x18);
	}

	WeaponData* GetBaseProjectile() {
		return (this + O::Item::heldEntity);
	}

	WeaponData* Gathering() {
		return (this + O::BaseMelee::gathering);
	}

	uintptr_t get_damage_properties() {
		return *reinterpret_cast<uintptr_t*>((uintptr_t)this + O::BaseMelee::damageProperties);
	}

	int GetUID() {
		return safe_read(this + 0x28, int);
	}

	void SetAutomatic()
	{
		if (Vars::Misc::Automatic)
		{
			DWORD64 Held = safe_read(this + O::Item::heldEntity, DWORD64);
			safe_write(Held + O::BaseProjectile::automatic, 1, bool);
		}
	}

	void SuperEoka()
	{
		if (Vars::Misc::SuperEoka)
		{
			DWORD64 Held = safe_read(this + O::Item::heldEntity, DWORD64);
			safe_write(Held + O::BowWeapon::attackReady, 1.f, float);
		}
	}

	void RunOnHit()
	{
		if (Vars::Misc::RunOnHit) {
			DWORD64 Held = safe_read(this + O::Item::heldEntity, DWORD64);
			safe_write(Held + O::BaseMelee::blockSprintOnAttack, 0, int);
		}

	}
	void SetRA()
	{
		if (Vars::Misc::AntiRecoil)
		{
			DWORD64 Held = safe_read(this + O::Item::heldEntity, DWORD64);
			DWORD64 recoil = safe_read(Held + O::BaseProjectile::recoil, DWORD64);
			safe_write(recoil + O::RecoilProperties::recoilYawMin, Vector2(0.f, 0.f), Vector2);
			safe_write(recoil + O::RecoilProperties::recoilYawMax, Vector2(0.f, 0.f), Vector2);
			safe_write(recoil + O::RecoilProperties::recoilPitchMin, Vector2(0.f, 0.f), Vector2);
			safe_write(recoil + O::RecoilProperties::recoilPitchMax, Vector2(0.f, 0.f), Vector2);
			safe_write(recoil + O::RecoilProperties::ADSScale, Vector2(0.f, 0.f), Vector2);
			safe_write(recoil + O::RecoilProperties::movementPenalty, Vector2(0.f, 0.f), Vector2);
		}
	}
};
class transform;
class BasePlayer
{
public:
	float GetHealth() {
		return safe_read(this + O::BaseCombatEntity::_health, float);
	}

	float GetTickTime() {
		return safe_read(this + 0x5CC, float); // private float lastSentTickTime;
	}

	float Time() {
		return safe_read(this + O::BasePlayer::lastSentTickTime, float);
	}

	const wchar_t* GetName() {
		pUncStr Str = ((pUncStr)(safe_read(this + O::BasePlayer::_displayName, DWORD64)));
		if (!Str) return StrW(L""); return Str->str;
	}

	void NightModer() {
		if (Vars::Visuals::NightMode) {
			static DWORD64 dwGameObjectManager = 0;
			if (!dwGameObjectManager)
			dwGameObjectManager = RVA(FindPattern((PBYTE)"\x48\x8B\x15\x00\x00\x00\x00\x66\x39", "xxx????xx", L"UnityPlayer.dll"), 7);
			DWORD64 ObjMgr = safe_read(dwGameObjectManager, DWORD64); if (!ObjMgr) return;
			DWORD64 Obj = safe_read(ObjMgr + 0x8, DWORD64); (Obj && (Obj != safe_read(ObjMgr, DWORD64))); Obj = safe_read(Obj + 0x8, DWORD64);
			DWORD64 GameObject = safe_read(Obj + 0x10, DWORD64);
			DWORD64 ObjClass = safe_read(GameObject + 0x30, DWORD64);
			DWORD64    Entity = safe_read(ObjClass + 0x18, DWORD64);
			DWORD64 Dome = safe_read(Entity + 0x28, DWORD64);
			DWORD64 TodCycle = safe_read(Dome + 0x38, DWORD64);
			safe_write(TodCycle + 0x10, 01.00f, float);
		}
	}

	Vector3 GetVelocity() {
		DWORD64 PlayerModel = safe_read(this + O::BasePlayer::playerModel, DWORD64);
		return safe_read(PlayerModel + O::PlayerModel::newVelocity, Vector3);
	}

	void SetModelFlag(int flag) {
		DWORD64 mstate = safe_read(this + O::BasePlayer::modelState, DWORD64);

		int flags = safe_read(mstate + O::ModelState::flags, int);
		safe_write(mstate + O::ModelState::flags, flags |= flag, int);
	}

	bool IsTeamMate(BasePlayer* Player)
	{
		ULONG64 ClientTeam = safe_read(Player + O::BasePlayer::clientTeam, ULONG64);
		ULONG64 members = safe_read(ClientTeam + O::ProtoBuf_PlayerTeam::members, ULONG64);
		int size = safe_read(members + 0x18, int);
		ULONG64 List = safe_read(members + 0x10, ULONG64);

		DWORD64 Search = this->GetSteamID();
		for (int i = 0; i < size; i++) {
			ULONG64 player = safe_read(List + 0x20 + (i * 0x8), ULONG64);
			DWORD64 id = safe_read(player + 0x20, ULONG64);
			if (id == Search) return true;
		}
		return false;
	}

	void SetRemoveFlag(int flag) {
		DWORD64 mstate = safe_read(this + O::BasePlayer::modelState, DWORD64);

		int flags = safe_read(mstate + O::ModelState::flags, int);
		safe_write(mstate + O::ModelState::flags, flags &= flag, int);
	}

	void SetWaterLevel(float val) {
		DWORD64 mstate = safe_read(this + O::BasePlayer::modelState, DWORD64);
		safe_write(mstate + O::ModelState::waterLevel, val, float);
	}

	//bool PLOS(Vector3 a, Vector3 b) {
	//	typedef bool(__stdcall* LOS)(Vector3, Vector3, int, float);
	//	return ((LOS)(Vars::gBase + O::GamePhysics::LineOfSight))(a, b, 2162688, 0.f);
	//}

	//bool IsVisible(BasePlayer* Player) {
	//	typedef bool(__stdcall* LOS)(Vector3, Vector3, int, float);
	//	bool canIgnore = PLOS(Player->GetPosition(), this->GetPosition());
	//	return canIgnore;
	//}

	bool IsVisible()
	{
		DWORD64 PlayerModel = safe_read(this + O::BasePlayer::playerModel, DWORD64);
		return safe_read(PlayerModel + 0x248, bool);
	    return true;
	}

	bool IsNpc() {
		DWORD64 PlayerModel = safe_read(this + O::BasePlayer::playerModel, DWORD64); //	public PlayerModel playerModel;
		return safe_read(PlayerModel + 0x2C0, bool);
	}

	bool IsValid(bool LPlayer = false) {
		return (((LPlayer || Vars::Visuals::IgnoreSleepers) ? !HasFlags(16) : true) && !IsDead() && (GetHealth() >= 0.8f));
	}

	bool HasFlags(int Flg) {
		return (safe_read(this + O::BasePlayer::playerFlags, int) & Flg);
	}

	void SetFlags(int Flg) {
		int Flags = safe_read(this + O::BasePlayer::playerFlags, int);
		safe_write(this + O::BasePlayer::playerFlags, (Flags |= Flg), int);
	}

	void AddFlag(int flag) {
		DWORD64 mstate = safe_read(this + 0x588, DWORD64);
		int flags = safe_read(mstate + 0x24, int);
		safe_write(mstate + 0x24, flags |= flag, int);
	}

	ULONG64 GetTransform(int bone)
	{
		DWORD64 entity_model = safe_read(this + O::BaseEntity::model, DWORD64);
		DWORD64 bone_dict = safe_read(entity_model + O::Model::boneTransforms, DWORD64);
		DWORD64 BoneValue = safe_read(bone_dict + 0x20 + bone * 0x8, DWORD64);
		return BoneValue;
	}

	transform* get_bone_transform(int bone_id) {
		uintptr_t entity_model = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x118);
		uintptr_t bone_dict = *reinterpret_cast<uintptr_t*>(entity_model + 0x48);
		transform* BoneValue = *reinterpret_cast<transform**>(bone_dict + 0x20 + bone_id * 0x8);

		return BoneValue;
	}

	Vector3 GetPosition() {
		return GetBone(GetTransform(Vars::Global::BoneToAim));
	}

	Vector3 GetBoneByID(BoneList BoneID) {
		return GetBone(GetTransform(BoneID));
	}

	int GetTeamSize()
	{
		DWORD64 ClientTeam = safe_read(this + O::BasePlayer::clientTeam, DWORD64);
		DWORD64 members = safe_read(ClientTeam + 0x28, DWORD64);
		return safe_read(members + 0x18, DWORD64);
	}

	bool IsDead() {
		if (!this) return true;
		return safe_read(this + O::BaseCombatEntity::lifestate, bool);
	}

	DWORD64 GetSteamID() {
		return safe_read(this + O::BasePlayer::userID, DWORD64);
	}

	bool IsMenu() {
		if (!this) return true;
		DWORD64 Input = safe_read(this + O::BasePlayer::input, DWORD64);
		return !(safe_read(Input + O::PlayerInput::hasKeyFocus, bool));
	}

	void SetVA(const Vector2& VA) {
		DWORD64 Input = safe_read(this + O::BasePlayer::input, DWORD64);
		safe_write(Input + 0x3C, VA, Vector2);
	}

	Vector2 GetRA() {
		DWORD64 Input = safe_read(this + O::BasePlayer::input, DWORD64);
		return safe_read(Input + 0x64, Vector2); //public Vector3 recoilAngles;
	}

	Vector2 GetVA() {
		DWORD64 Input = safe_read(this + O::BasePlayer::input, DWORD64);
		return safe_read(Input + 0x3C, Vector2);
	}

	WeaponData* GetWeaponInfo(int Id) {
		DWORD64 Inventory = safe_read(this + O::BasePlayer::inventory, DWORD64);
		DWORD64 Belt = safe_read(Inventory + 0x28, DWORD64);
		DWORD64 ItemList = safe_read(Belt + 0x38, DWORD64);
		DWORD64 Items = safe_read(ItemList + 0x10, DWORD64);
		return (WeaponData*)safe_read(Items + 0x20 + (Id * 0x8), DWORD64);
	}

	WeaponData* GetActiveWeapon() {
		int ActUID = safe_read(this + O::BasePlayer::clActiveItem, int);
		if (!ActUID) return nullptr;
		WeaponData* ActiveWeapon;
		for (int i = 0; i < 6; i++)
			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->GetUID()) {
				return ActiveWeapon;
			}
		return 0;
	}

	WeaponData* GetBaseProjectile() {
		return *reinterpret_cast<WeaponData**>((uintptr_t)this + O::Item::heldEntity);
	}

	void SpiderMan() {
		DWORD64 Movement = safe_read(this + O::BasePlayer::movement, DWORD64);
		safe_write(Movement + O::PlayerWalkMovement::groundAngle, 0.f, float);
		safe_write(Movement + O::PlayerWalkMovement::groundAngleNew, 0.f, float);
	}

	void InfinityJump() {
		if (GetAsyncKeyState(VK_SPACE)) {
			DWORD64 Movement = safe_read(this + O::BasePlayer::movement, DWORD64);
			safe_write(Movement + O::PlayerWalkMovement::groundAngleNew, Vector3(0, 100000, 0), Vector3);
			safe_write(Movement + O::PlayerWalkMovement::groundAngle, Vector3(99999, 99999, 99999), Vector3);
			Vars::Misc::JumpShoot = true;
			Vars::Misc::SlideWalk = true;
		}
	}

	void NoHeavyReduct() {
		float Reduct = safe_read(this + O::BasePlayer::clothingMoveSpeedReduction, float);
		if (Reduct == 0.f) { safe_write(this + O::BasePlayer::clothingMoveSpeedReduction, -0.1f, float); }
		else if ((Reduct > 0.15f) && (Reduct < 0.35f)) {
			safe_write(this + O::BasePlayer::clothingMoveSpeedReduction, 0.15f, float);
		}
		else if (Reduct > 0.39f) {
			safe_write(this + O::BasePlayer::clothingMoveSpeedReduction, 0.25f, float);
		}
	}

	void SetGravity(float val) {
		DWORD64 Movement = safe_read(this + O::BasePlayer::movement, DWORD64);
		safe_write(Movement + O::PlayerWalkMovement::gravityMultiplier, val, float);
	}

	typedef Vector3(__stdcall* Transform)(UINT64);
	Vector3 GetBone(ULONG_PTR pTransform)
	{
		if (!pTransform) return Vector3();
		Transform original = (Transform)(Vars::gBase + O::UnityEngine_Transform::get_position);
		Vector3 res = original(pTransform);
		return res;
	}
};

class LPlayerBase
{
public:
	BasePlayer* BasePlayer = 0;
	Matrix4x4* pViewMatrix = nullptr;
	bool WorldToScreen(const Vector3& EntityPos, Vector2& ScreenPos)
	{
		if (!pViewMatrix) return false;
		Vector3 TransVec = Vector3(pViewMatrix->_14, pViewMatrix->_24, pViewMatrix->_34);
		Vector3 RightVec = Vector3(pViewMatrix->_11, pViewMatrix->_21, pViewMatrix->_31);
		Vector3 UpVec = Vector3(pViewMatrix->_12, pViewMatrix->_22, pViewMatrix->_32);
		float w = Math::Dot(TransVec, EntityPos) + pViewMatrix->_44;
		if (w < 0.098f) return false;
		float y = Math::Dot(UpVec, EntityPos) + pViewMatrix->_42;
		float x = Math::Dot(RightVec, EntityPos) + pViewMatrix->_41;
		ScreenPos = Vector2((Vars::Global::ScreenWidth / 2) * (1.f + x / w), (Vars::Global::ScreenHigh / 2) * (1.f - y / w));
		return true;
	}
};

DECLSPEC_NOINLINE void Flex() {
	FC(kernel32, Sleep, 0);
}
LPlayerBase LocalPlayer;
DWORD64 g_Helicopter;
float GetFov2(BasePlayer* Entity, BoneList Bone) {
	Vector2 ScreenPos;
	if (!LocalPlayer.WorldToScreen(Entity->GetBoneByID(head), ScreenPos)) return 1000.f;
	return Math::Calc2D_Dist(Vector2(Vars::Global::ScreenWidth / 2, Vars::Global::ScreenHigh / 2), ScreenPos);
}
unsigned int m_wcslen(wchar_t* str)
{
	int cnt = 0;
	if (!str)
		return 0;
	for (; *str != '\0'; ++str)
		++cnt;
	return cnt;
}

class Str {
	char zpad[0x10];
public:
	int size;
	wchar_t str[MAX_STRING_LEN + 1];
	Str(const wchar_t* st) {
		size = min(m_wcslen((wchar_t*)st), MAX_STRING_LEN);
		for (int i = 0; i < size; i++) {
			str[i] = st[i];
		}
		str[size] = 0;
	}
};

bool m_wcsicmp(wchar_t* a, const wchar_t* b) {

	int ret = 0;
	wchar_t* p1 = (wchar_t*)a;
	wchar_t* p2 = (wchar_t*)b;
	while (!(ret = *p1 - *p2) && *p2)
		++p1, ++p2;

	return ret == 0;
}

void Fov() {
	DWORD64 BaseNetworkable = safe_read(GetModBase(StrW(cry(L"GameAssembly.dll"))) + 0x2FF7FE0, DWORD64); //       "Signature": "ConVar_Graphics_c*"
	DWORD64 Graphics = safe_read(BaseNetworkable + 0xB8, DWORD64);
	safe_write(Graphics + 0x18, Vars::Visuals::Fov, float);
}

float GetBulletSpeedPS(Weapon tar, int ammo)
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

float GetGravityPS(int ammoid) {
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
Vector3 PredictionPS(Vector3 LP_Pos, BasePlayer* Player, BoneList Bone)
{
	WeaponData* active = LocalPlayer.BasePlayer->GetActiveWeapon();
	Weapon tar = active->Info();
	int ammo = active->LoadedAmmo();
	if (Vars::AimBot::pSilentHead) {
		Vector3 BonePos = Player->GetBoneByID(head);
		float Dist = Math::Calc3D_Dist(LP_Pos, BonePos);
		if (Dist > 0.001f) {
			float speed = GetBulletSpeedPS(tar, ammo) * Vars::AimBot::Velocity;
			if (!speed) speed = 250.f;
			float BulletTime = Dist / speed;
			Vector3 vel = Player->GetVelocity();
			Vector3 PredictVel = vel * BulletTime * 0.75f;
			BonePos += PredictVel;
			float gravity = GetGravityPS(ammo);
			BonePos.y += (4.905f * BulletTime * BulletTime) * gravity;
		}
		return BonePos;
	}
	if (Vars::AimBot::pSilentBody) {
		Vector3 BonePos = Player->GetBoneByID(spine1);
		float Dist = Math::Calc3D_Dist(LP_Pos, BonePos);
		if (Dist > 0.001f) {
			float speed = GetBulletSpeedPS(tar, ammo) * Vars::AimBot::Velocity;
			if (!speed) speed = 250.f;
			float BulletTime = Dist / speed;
			Vector3 vel = Player->GetVelocity();
			Vector3 PredictVel = vel * BulletTime * 0.75f;
			BonePos += PredictVel;
			float gravity = GetGravityPS(ammo);
			BonePos.y += (4.905f * BulletTime * BulletTime) * gravity; // 4.905f
		}
		return BonePos;
	}
}
Vector3 __fastcall pSilent(float aimCone, Vector3 inputVec, bool anywhereInside = true) {
	if (Vars::AimBot::pSilent) {
		if (Vars::PlayerxD != NULL) {
			typedef Vector3(__stdcall* Orig)(float, Vector3, bool);
			Vector3 dir = (PredictionPS(LocalPlayer.BasePlayer->GetBoneByID(head), (BasePlayer*)Vars::PlayerxD, head) - LocalPlayer.BasePlayer->GetBoneByID(head)).normalize();
			return ((Orig)AimDirection)(0.f, dir, anywhereInside);
		}
		else {
			typedef Vector3(__stdcall* Orig)(float, Vector3, bool);
			return ((Orig)AimDirection)(aimCone, inputVec, anywhereInside);
		}
	}
	else {
		typedef Vector3(__stdcall* Orig)(float, Vector3, bool);
		return ((Orig)AimDirection)(aimCone, inputVec, anywhereInside);
	}
}

DWORD64 Fix(void* BaseProjectile, void* prefabPath, Vector3 pos, Vector3 forward, Vector3 velocity) {
	if (Vars::AimBot::pSilent) {
		typedef DWORD64(__stdcall* Orig)(void*, void*, Vector3, Vector3, Vector3);
		WeaponData* weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
		Weapon target = weapon->Info();
		if (Vars::AimBot::pSilent) {
			if (target.id == 1796682209) {
				safe_write(BaseProjectile + 0x26C, 1.1f, float);
				Vars::AimBot::Velocity == 1.1f;
			}
			else {
				safe_write(BaseProjectile + 0x26C, 1.48f, float);
				Vars::AimBot::Velocity == 1.48f;
			}
			if (target.id == 1318558775) {
				safe_write(BaseProjectile + 0x26C, 1.1f, float);
				Vars::AimBot::Velocity == 1.1f;
			}
			else {
				safe_write(BaseProjectile + 0x26C, 1.48f, float);
				Vars::AimBot::Velocity == 1.48f;
			}
			if (target.id == 1965232394) {
				safe_write(BaseProjectile + 0x26C, 1.50f, float);
				Vars::AimBot::Velocity == 1.50f;
			}
			else {
				safe_write(BaseProjectile + 0x26C, 1.48f, float);
				Vars::AimBot::Velocity == 1.48f;
			}
			if (target.id == 884424049) {
				safe_write(BaseProjectile + 0x26C, 1.50f, float);
				Vars::AimBot::Velocity == 1.50f;
			}
			else {
				safe_write(BaseProjectile + 0x26C, 1.48f, float);
				Vars::AimBot::Velocity == 1.48f;
			}
		}
	}
	typedef DWORD64(__stdcall* Orig)(void*, void*, Vector3, Vector3, Vector3);
	return ((Orig)CreateProjectile)(BaseProjectile, prefabPath, pos, forward, velocity);
}
class AimResult {
public:
	bool valid;
	BasePlayer* entity;
	Vector3 pos;
	float dist = 10000.f;
	float realdist = 0.f;
	float fov = 10000.f;

	bool operator<(const AimResult& b) {
		if (fov == 10000.f) {
			return this->dist < b.dist;
		}
		else {
			return this->fov < b.fov;
		}
	}

	AimResult() {
		this->valid = false;
		this->fov = 10000.f;
	}
	AimResult(Vector3 target) {
		this->valid = false;
		this->pos = target;
	}
};
struct Ray {
	Vector3 origin;
	Vector3 dir;
	Ray(Vector3 o, Vector3 d) {
		origin = o;
		dir = d;
	}
	Ray() {}
};
DWORD64 hkMelee = 0;
void DoMeleeAttack(AimResult target, DWORD64 active, bool transform = true) {
	if (!target.valid || !active) return;

	Vector3 lp = LocalPlayer.BasePlayer->GetPosition();
	typedef float(__stdcall* A)();

	float time = ((A)(Vars::gBase + O::UnityEngine_Time::get_time))();
	if (safe_read(active + O::AttackEntity::nextAttackTime, float) >= time) { //nextattacktime
		return;
	}

	if (safe_read(active + O::AttackEntity::timeSinceDeploy, float) < safe_read(active + O::AttackEntity::deployDelay, float)) {
		return;
	}

	//HitTest
	auto hit_test_class = il2cpp::init_class(cry("HitTest"));
	if (!hit_test_class) return;

	typedef DWORD64(__stdcall* NNew)(DWORD64);
	auto HitTest = ((NNew)il2cpp::methods::object_new(hit_test_class));

	Ray ray = Ray(lp, (target.pos - lp).normalize());
	safe_write(HitTest + O::HitTest::MaxDistance, 1000.f, float); //MaxDistance
	DWORD64 trans;
	if (transform) {
		trans = target.entity->GetTransform(BoneList::head);
	}
	else {
		typedef DWORD64(__stdcall* GetTr)(DWORD64);
		trans = ((GetTr)(Vars::gBase + O::UnityEngine_Component::get_transform))((DWORD64)target.entity);
	}
	if (!trans) return;

	safe_write(HitTest + O::HitTest::HitTransform, trans, DWORD64); //HitTransform
	safe_write(HitTest + O::HitTest::AttackRay, ray, Ray);
	//safe_write(HitTest + 0xC0, Str(xorstr(L"Flesh")), Str); //HitMaterial
	safe_write(HitTest + O::HitTest::DidHit, true, bool); //DidHit
	safe_write(HitTest + O::HitTest::HitEntity, target.entity, BasePlayer*); //HitEntity
	typedef Vector3(__stdcall* ITP)(DWORD64, Vector3);
	Vector3 hitpoint = ((ITP)(Vars::gBase + O::UnityEngine_Transform::InverseTransformPoint))(trans, target.pos);
	safe_write(HitTest + O::HitTest::HitPoint, hitpoint, Vector3); //HitPoint
	safe_write(HitTest + O::HitTest::HitNormal, Vector3(0, 0, 0), Vector3); //HitNormal
	safe_write(HitTest + O::HitTest::damageProperties, safe_read(active + O::BaseMelee::damageProperties, DWORD64), DWORD64);
	typedef void(__stdcall* Atk)(DWORD64, DWORD64);
	float kd = safe_read(active + O::AttackEntity::repeatDelay, float);
	typedef void(__stdcall* StartKD)(DWORD64, float);
	((StartKD)(Vars::gBase + O::AttackEntity::StartAttackCooldown))(active, kd);

	return ((Atk)(Vars::gBase + O::BaseMelee::ProcessAttack))(active, (DWORD64)HitTest);
}
bool PLOS(Vector3 a, Vector3 b) {
	typedef bool(__stdcall* LOS)(Vector3, Vector3, int, float);
	return ((LOS)(Vars::gBase + O::GamePhysics::LineOfSight))(a, b, 2162688, 0.f);
}

float MaxMeleeDist(DWORD64 melee, bool hren) {
	float pad = 0.1f;
	typedef float(__stdcall* RetF)();
	float time = ((RetF)(Vars::gBase + O::UnityEngine_Time::get_time))();

	float desyncTime = max(time - LocalPlayer.BasePlayer->Time() - 0.0325f, 0.f);
	float res = pad + desyncTime * 5.5f;
	if (hren) {
		res += (1.5f * safe_read(melee + 0x278, float)); //maxDistance
	}
	return max(res, 0.05f);
}
void Silent(void* a1, void* a2) {
	if (Vars::AimBot::SilentxD) {
		if ((BasePlayer*)Vars::PlayerxD != NULL)
		{
			const auto PlayerAttack = safe_read(a2 + 0x18, uintptr_t);
			const auto Attack = safe_read(PlayerAttack + 0x18, uintptr_t);
			safe_write(Attack + 0x30, 698017942, uint32_t);
			safe_write(Attack + 0x64, 16144115, uint32_t);
			safe_write(Attack + 0x2C, safe_read(safe_read((BasePlayer*)Vars::PlayerxD + 0x50, uintptr_t) + 0x10, uint32_t), uint32_t);
		}
	}
	typedef void(__stdcall* Orig)(void*,void*);
	return ((Orig)SENDPRJ)(a1,a2);
}
AimResult TargetMeleeTest(BasePlayer* Player, DWORD64 melee) {
	AimResult res = AimResult();
	if (!Player || !Player->IsValid()) return res;
	//if (!Player->HasFlags(16)) return res;
	//if (!Player->IsTeamMate(Player)) return res;

	typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);
	Vector3 prepos = Player->GetPosition();
	Vector3 closest_entity = ((CPoint)(Vars::gBase + O::BaseEntity::ClosestPoint))(LocalPlayer.BasePlayer, prepos);
	Vector3 closest_local = ((CPoint)(Vars::gBase + O::BaseEntity::ClosestPoint))(Player, closest_entity);

	float disttoentity = MaxMeleeDist(melee, false);
	float distfromlocal = MaxMeleeDist(melee, true);

	float maxdist = distfromlocal + disttoentity;

	float realdist = (closest_local - closest_entity).Length();

	if (realdist > maxdist) return res;

	Vector3 target;
	if (realdist <= disttoentity) {
		target = closest_entity;
	}
	else {
		Vector3 dir = (closest_entity - closest_local).normalize();
		target = closest_local + dir * disttoentity;
	}

	res.valid = PLOS(closest_local, target);
	if (!res.valid) return res;

	Vector3 lppos = LocalPlayer.BasePlayer->GetPosition();
	res.valid = PLOS(lppos, target);
	if (!res.valid) return res;

	res.dist = realdist;
	res.entity = Player;
	res.pos = target;
	return res;
}

void PickupItem(DWORD64 item) {
	typedef void(__stdcall* Pick)(DWORD64, Str);
	return ((Pick)(Vars::gBase + 0x3B5C90))(item, Str(cry(L"Pickup")));
}

void ClientInputxD(DWORD64 Lp, DWORD64 ModelState) {
	typedef void(__stdcall* Input)(DWORD64, DWORD64);
	((Input)ClientInput)(Lp, ModelState);
	if (Vars::Misc::SlideWalk) {
		LocalPlayer.BasePlayer->SetModelFlag(0x10);
	}
	if (Vars::Misc::SwimOnGround) {
		LocalPlayer.BasePlayer->SetWaterLevel(0.65f);
	}
	if (Vars::Misc::Spin) {
		LocalPlayer.BasePlayer->SetModelFlag(32);
	}
	if (Vars::Misc::MeleeAttack) {
		DWORD64 BaseNetworkable;
		BaseNetworkable = safe_read(GetModBase(StrW(cry(L"GameAssembly.dll"))) + g_BN, DWORD64);
		DWORD64 EntityRealm = safe_read(BaseNetworkable + 0xB8, DWORD64);
		DWORD64 ClientEntities = safe_read(EntityRealm, DWORD64);
		DWORD64 ClientEntities_list = safe_read(ClientEntities + 0x10, DWORD64);
		DWORD64 ClientEntities_values = safe_read(ClientEntities_list + 0x28, DWORD64);
		if (!ClientEntities_values) return;
		int EntityCount = safe_read(ClientEntities_values + 0x10, int);
		DWORD64 EntityBuffer = safe_read(ClientEntities_values + 0x18, DWORD64);
		WeaponData* weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
		DWORD64 active = safe_read(weapon + O::Item::heldEntity, DWORD64);
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
			char* classname = weapon->ClassName();
			bool weaponmelee = weapon && classname && (m_strcmp(classname, cry("BaseMelee")) || m_strcmp(classname, cry("Jackhammer")));
			if (weaponmelee) {
				if (Vars::Misc::MeleeAttack && m_strstr(buff, cry("player.prefab"))) {
					BasePlayer* Player = (BasePlayer*)safe_read(Object + 0x28, DWORD64);
					DWORD64 ent = safe_read(Object + 0x28, UINT64);
					UINT64 gameObject = safe_read(ObjectClass + 0x30, UINT64);
					UINT64 Trans = safe_read(gameObject + 0x8, UINT64);
					UINT64 Vec = safe_read(Trans + 0x38, UINT64);
					Vector3 pos = safe_read(Vec + 0x90, Vector3);
					AimResult target = TargetMeleeTest(Player, active);
					DoMeleeAttack(target, active, true);
				}
			}
		}
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
	WeaponData* weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
	DWORD64 active = safe_read(weapon + O::Item::heldEntity, DWORD64);
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
		if (Vars::Misc::AutoPickup && m_strstr(buff, cry("/collectable/"))) {
			DWORD64 ent = safe_read(Object + 0x28, UINT64);
			UINT64 gameObject = safe_read(ObjectClass + 0x30, UINT64);
			UINT64 Trans = safe_read(gameObject + 0x8, UINT64);
			UINT64 Vec = safe_read(Trans + 0x38, UINT64);
			Vector3 pos = safe_read(Vec + 0x90, Vector3);
			typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);
			Vector3 local = ((CPoint)(Vars::gBase + O::BaseEntity::ClosestPoint))(LocalPlayer.BasePlayer, pos);
			if (Math::Calc3D_Dist(local, pos) < 3.f) {
				PickupItem(ent);
			}
		}
	}
	if (Vars::Misc::AutoFarm) {
		DWORD64 BaseNetworkable;
		BaseNetworkable = safe_read(GetModBase(StrW(cry(L"GameAssembly.dll"))) + g_BN, DWORD64);
		DWORD64 EntityRealm = safe_read(BaseNetworkable + 0xB8, DWORD64);
		DWORD64 ClientEntities = safe_read(EntityRealm, DWORD64);
		DWORD64 ClientEntities_list = safe_read(ClientEntities + 0x10, DWORD64);
		DWORD64 ClientEntities_values = safe_read(ClientEntities_list + 0x28, DWORD64);
		if (!ClientEntities_values) return;
		int EntityCount = safe_read(ClientEntities_values + 0x10, int);
		DWORD64 EntityBuffer = safe_read(ClientEntities_values + 0x18, DWORD64);
		WeaponData* weapon = LocalPlayer.BasePlayer->GetActiveWeapon();
		DWORD64 active = safe_read(weapon + O::Item::heldEntity, DWORD64);
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
			if (Vars::Misc::AutoFarm) {
				if (Vars::Misc::AutoFarmTree && (m_strstr((char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64), cry("TreeEntity")))) {
					char* name = (char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64);
					UINT64 gameObject = safe_read(ObjectClass + 0x30, UINT64);
					UINT64 Trans = safe_read(gameObject + 0x8, UINT64);
					UINT64 Vec = safe_read(Trans + 0x38, UINT64);
					Vector3 pos = safe_read(Vec + 0x90, Vector3);
					typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);
					Vector3 local = ((CPoint)(Vars::gBase + O::BaseEntity::ClosestPoint))(LocalPlayer.BasePlayer, pos);

					if (Math::Calc3D_Dist(local, pos) >= 3.f) {
						continue;
					}

					AimResult target = AimResult();
					target.valid = true;
					target.pos = pos + Vector3(0, 1, 0);
					target.entity = (BasePlayer*)ent;
					DoMeleeAttack(target, active, false);
				}
				if (Vars::Misc::AutoFarmOre && (m_strstr((char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64), cry("OreHotSpot")) || m_strstr((char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64), cry("ResourceEntity")))) {
					char* name = (char*)safe_read(safe_read(ent, DWORD64) + 0x10, DWORD64);
					UINT64 gameObject = safe_read(ObjectClass + 0x30, UINT64);
					UINT64 Trans = safe_read(gameObject + 0x8, UINT64);
					UINT64 Vec = safe_read(Trans + 0x38, UINT64);
					Vector3 pos = safe_read(Vec + 0x90, Vector3);
					typedef Vector3(__stdcall* CPoint)(BasePlayer*, Vector3);
					Vector3 local = ((CPoint)(Vars::gBase + O::BaseEntity::ClosestPoint))(LocalPlayer.BasePlayer, pos);

					if (Math::Calc3D_Dist(local, pos) >= 3.f) {
						continue;
					}

					AimResult target = AimResult();
					target.valid = true;
					target.pos = pos + Vector3(0, 1, 0);
					target.entity = (BasePlayer*)ent;
					DoMeleeAttack(target, active, false);
				}
			}
		}
	}
}
bool waslagging = false;
void SlideWalk(DWORD64 mov, DWORD64 modelstate) {
	typedef void(__stdcall* Inp)(DWORD64, DWORD64);
	if (!waslagging && Vars::Misc::FakeLag) {
		safe_write(LocalPlayer.BasePlayer + O::BasePlayer::clientTickInterval, Vars::Misc::FakeLagFloat, float);
		waslagging = true;
	}
	else if (waslagging && !Vars::Misc::FakeLag) {
		safe_write(LocalPlayer.BasePlayer + O::BasePlayer::clientTickInterval, 0.05f, float);
		waslagging = false;
	}
	float speed = (safe_read(mov + O::PlayerWalkMovement::swimming, bool) /*swim*/ || safe_read(mov + O::BaseMovement::Duckingk__BackingField, float) > 0.5f) ? 1.7f : (safe_read(mov + O::PlayerWalkMovement::jumping, bool) ? 8.f : 5.5f);
	Vector3 vel = safe_read(mov + O::BaseMovement::TargetMovementk__BackingField, Vector3);
	speed = max(speed, vel.Length());
	if (Vars::Misc::SlideWalk) {
		float len = vel.Length();
		if (len > 0.f) {
			vel = Vector3(vel.x / len * speed, vel.y, vel.z / len * speed);
			safe_write(mov + O::BaseMovement::TargetMovementk__BackingField, vel, Vector3);
		}
	}
	((Inp)DoFixedUpdate)(mov, modelstate);
}


void HitSound(DWORD64 Info) {
	if (Vars::Misc::HitSound) {
		if (Vars::Misc::HitSound1) {
			PlaySoundA("C:\\sound\\1.wav", NULL, SND_ASYNC | SND_FILENAME);
		}
		if (Vars::Misc::HitSound2) {
			PlaySoundA("C:\\sound\\2.wav", NULL, SND_ASYNC | SND_FILENAME);
		}
		if (Vars::Misc::HitSound3) {
			PlaySoundA("C:\\sound\\3.wav", NULL, SND_ASYNC | SND_FILENAME);
		}
	}
	else {
		typedef void(__stdcall* Orig)(DWORD64);
		return ((Orig)HitSND)(Info);
	}
}

inline void __fastcall AntiAim(void* self)
{
	if (Vars::Misc::AntiAim)
	{
		auto input = safe_read(self + 0x4C8, uintptr_t);
		if (!input)
		{
			typedef void(__fastcall* AntiAim1)(void*);
			return ((AntiAim1)sendClientTick)(self);
		}

		auto state = safe_read(input + 0x20, uintptr_t);
		if (!state)
		{
			typedef void(__fastcall* AntiAim1)(void*);
			return ((AntiAim1)sendClientTick)(self);
		}

		auto current = safe_read(state + 0x10, uintptr_t);
		if (!current)
		{
			typedef void(__fastcall* AntiAim1)(void*);
			return ((AntiAim1)sendClientTick)(self);
		}
		if (Vars::Misc::Spin) {
			safe_write(current + 0x18, Vector3(0, rand() % 180 + 90, rand() % 360 - 180), Vector3);
		}	
		if (Vars::Misc::Spinz) {
			safe_write(current + 0x18, Vector3(100, - 180 + 180, -180), Vector3);
		}
	}
	typedef void(__fastcall* AntiAim1)(void*);
	return ((AntiAim1)sendClientTick)(self);
}
bool __fastcall JumpShoot(void* a1, void* a2)
{
	if (Vars::Misc::JumpShoot)
		return true;
	typedef bool(__fastcall* Jump)(void*, void*);
	return ((Jump)CanAttack)(a1, a2);
}