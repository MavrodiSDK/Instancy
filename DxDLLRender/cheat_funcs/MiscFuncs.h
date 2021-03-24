bool lastState = false;
void Misc()
{
	if (Vars::Misc::FakeAdmin) LocalPlayer.BasePlayer->SetFlags(4);

	if (Vars::Misc::InfinityJump) LocalPlayer.BasePlayer->InfinityJump();

	if (Vars::Misc::NoHeavyReduct)
		LocalPlayer.BasePlayer->NoHeavyReduct();

	if (Vars::Misc::SpiderMan)
		LocalPlayer.BasePlayer->SpiderMan();

	if (Vars::Visuals::NightMode) {
		LocalPlayer.BasePlayer->NightModer();
	}
}
