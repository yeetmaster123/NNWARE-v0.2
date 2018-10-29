#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/NetChannel.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/Fakewalk.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/FakeLag.h"

int CFakeLag::lag_comp_break()
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player || local_player->GetHealth() <= 0)
		return 1;

	auto velocity = local_player->GetVelocity();
	velocity.z = 0.f;
	auto speed = velocity.Length();
	auto distance_per_tick = speed * INTERFACES::Globals->interval_per_tick;
	int choked_ticks = std::ceilf(64.f / distance_per_tick);
	return std::min<int>(choked_ticks, 14.f);
}

void CFakeLag::do_fakelag(SDK::CUserCmd* cmd)
{
	GLOBAL::should_send_packet = true;
	int choke_amount;

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	auto net_channel = INTERFACES::Engine->GetNetChannel();
	if (!net_channel)
		return;


	if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame())
		return;

	if ((cmd->buttons & IN_USE) || local_player->GetMoveType() == SDK::MOVETYPE_LADDER)
		return;

	if (weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_C4)
		return;

	if ((weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_HEGRENADE) ||
		(weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_DECOY) ||
		(weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_FLASHBANG) ||
		(weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_SMOKEGRENADE) ||
		(weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_MOLOTOV) ||
		(weapon->GetItemDefenitionIndex() == SDK::ItemDefinitionIndex::WEAPON_INCGRENADE))
		return;

	if (SETTINGS::settings.lag_bool)
	{
		if (fake_walk)
			choke_amount = 14;
		else
		{
			choke_amount = 1;
			if (!(local_player->GetFlags() & FL_ONGROUND))
			{
				switch (SETTINGS::settings.lag_type % 2)
				{
					case 0: choke_amount = SETTINGS::settings.jump_lag; break;
					case 1: choke_amount = lag_comp_break(); break;
				}
			}
			else if (local_player->GetVelocity().Length2D() > 0.1)
			{
				switch (SETTINGS::settings.lag_type % 2)
				{
					case 0: choke_amount = SETTINGS::settings.move_lag; break;
					case 1: choke_amount = lag_comp_break(); break;
				}
			}
			else if (local_player->GetVelocity().Length2D() < 0.1)
			{
				switch (SETTINGS::settings.lag_type % 2)
				{
					case 0: choke_amount = SETTINGS::settings.stand_lag; break;
					case 1: choke_amount = lag_comp_break(); break;
				}
			}
		}
	}
	else
		choke_amount = 1;

	if (net_channel->m_nChokedPackets >= min(14, choke_amount))
		GLOBAL::should_send_packet = true;
	else
		GLOBAL::should_send_packet = false;
}

CFakeLag* fakelag = new CFakeLag();