namespace Vars
{
	__declspec(selectany) uintptr_t gBase = NULL;
	//__declspec(selectany) uintptr_t closestPlayer = NULL;
	__declspec(selectany) uintptr_t PlayerxD = NULL;

	namespace Global
	{
		Vector2 MenuPos = { 150, 150 };
		bool Panic = false;
		int ScreenHigh = 0;
		int ScreenWidth = 0;
		HWND hWindow = nullptr;
		bool MenuVisible = false;
		WNDPROC oWindproc = nullptr;

		Vector3 LastPos = { 0.f, 0.f, 0.f };
		ULONGLONG LastTime = 0;
		DWORD64 LastSteamID = 0;
		const wchar_t* LastName;
		Vector3 PlayerVeloc = { 0.f, 0.f, 0.f };

		int BoneToAim = 1;
		float BulletSpeed = 250.f;
		DWORD64 PresentDelta = 0;
		float CorX = 0.f;
		float CorY = 0.f;

		int CurPage = 0;
	}

	namespace AimBot
	{
		bool pSilent = false;
		bool TargetLine = false;
		bool silentAim = false;
		bool pSilentHead = false;
		bool SilentxD = false;
		bool pSilentBody = false;
		bool Activate = false;
		bool FakeDist = false;
		bool MagicBullet = false;
		bool HeliSilent = false;
		bool ShootWall = false;
		bool AimHead = false;
		bool AimBody = false;
		bool FatBullet = false;
		bool TraceALL = false;
		bool BodyAim = false;
		bool VisibleCheck = false;
		bool DrawFov = false;
		bool DrawFov1 = false;
		bool DrawFov2 = false;
		bool DrawFov3 = false;
		bool IgnoreNpc = false;
		bool IgnoreTeam = true;
		float FatBulletScore = 1.5f;
		float Range = 400.f;
		float Fov = 120.f;
		float RecoilControl = 0.f;
		float pSilentFov = 120.f;
		float SilentFov = 120.f;
		float MagicFov = 120.f;
		float Velocity = 1.480f;
		float Velocity1 = 1.0f;
	}

	namespace Visuals
	{
		bool ShowPlayerActiveSlot = false;
		bool ShowPlayerBoxCorner = false;
		bool ShowPlayerBox = false;
		bool ShowPlayerName = false;
		bool ShowPlayerHealth = false;
		bool ShowPlayerWeapon = false;
		bool ShowPlayerDist = false;
		bool IgnoreSleepers = false;
		bool TargetLine = false;
		bool Skeleton = false;
		bool Crosshair = false;
		bool Chams = false;
		bool IgnoreNpc = false;
		bool NightMode = false;


		float Fov = 90.f;

		bool Shkaf = false;
		bool Door = false;
		bool Sulphur = false;
		bool Copter = false;
		bool Stone = false;
		bool Stash = false;
		bool BackPack = false;
		bool Hemp = false;
		bool Metal = false;
		bool PlayerDead = false;
		bool Chest = false;
		bool Landmine = false;
		bool Guntrap = false;
		bool Airdrop = false;
		bool Barrel = false;
		bool Helicopter = false;
	}

	namespace Misc
	{
		float pred = 0.f;
		bool SpiderMan = false;
		bool LEGITCFGBYMAVRODI = false;
		bool FastHeal = false;
		bool RAGECFGBYMAVRODI = false;
		bool HVHCFGBYMAVRODI = false;
		bool AntiAim = false;
		bool Spin = false;
		bool Spinz = false;
		bool ForcePosition = false;
		bool DoorKnocker = false;
		bool SlideWalk = false;
		bool HitSound = false;
		bool HitSound1 = false;
		bool HitSound2 = false;
		bool HitSound3 = false;
		bool JumpShoot = false;
		bool Automatic = false;
		bool AutoFarm = false;
		bool AutoFarmTree = false;
		bool AutoFarmOre = false;
		bool MeleeAttack = false;
		bool SwimOnGround = false;
		bool AutoPickup = false;
		bool SuperEoka = false;
		bool FakeDist = false;
		bool InfinityJump = false;
		bool FlyHack = false;
		bool NoSway = false;
		bool FakeAdmin = false;
		bool NoGreenZone = false;
		bool AntiRecoil = false;
		bool FakeLag = false;
		bool RunOnHit = false;
		bool HighJump = false;
		float JumpValue = 1.2f;
		float FakeLagFloat = 0.4f;
		bool NoFall = false;
		bool NoHeavyReduct = false;
		bool Crosshair = false;
	}
}