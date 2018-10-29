#include "../includes.h"
#include "../UTILS/interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/AutoWall.h"
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
bool next_lby_update(const float yaw_to_break, SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = INTERFACES::Globals->curtime;
	local_update = next_lby_update_time;

	auto animstate = local_player->GetAnimState();

	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1f && !fake_walk)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}
float fov_player(Vector ViewOffSet, Vector View, SDK::CBaseEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = aimbot->get_hitbox_pos(entity, hitbox);

	MATH::AngleVectors(Angles, &Forward);
	MATH::VectorSubtract(AimPos, Origin, Delta);
	MATH::NormalizeNum(Delta, Delta);

	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	SDK::CBaseEntity* local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	Vector angles;
	INTERFACES::Engine->GetViewAngles(angles);

	for (int i = 1; i <= INTERFACES::Globals->maxclients; i++)
	{
		SDK::CBaseEntity *entity = INTERFACES::ClientEntityList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeam() == local_player->GetTeam() || entity->GetIsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}

void CAntiAim::RandAa(SDK::CUserCmd * cmd)
{
	int FpSs = rand() % 6;
	switch (FpSs)
	{
	case 0: cmd->viewangles.y -= 180 + ((rand() % 10) - (10 * 0.9f)) + rand() % 180; break;
	case 1: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + rand() % 180; break;
	case 2: cmd->viewangles.y += randnum(-180, 180) + rand() % 180; break;
	case 3: cmd->viewangles.y += 0 + rand() % 180; break;
	case 4: cmd->viewangles.y = GLOBAL::real_angles.y + 180.f + rand() % 180; break;
	case 5: {
		static int y2 = -rand() % 180;
		int speed = rand() % 180;
		y2 += speed; if (y2 >= rand() % 180) y2 = -rand() % 180;
		cmd->viewangles.y = y2 + rand() % 180;

	} break;
	}
}

void CAntiAim::TreeHouseY(SDK::CUserCmd * cmd)
{
	if (GLOBAL::should_send_packet)
	{
		cmd->viewangles.y += SDK::CBaseEntity().GetLowerBodyYaw() + rand() % 90;
	}
	else
	{
		cmd->viewangles.y -= SDK::CBaseEntity().GetLowerBodyYaw() - rand() % 90;
	}
}

void CAntiAim::UltimateY(SDK::CUserCmd * cmd)
{
	static bool swap2 = false;
	if (GLOBAL::should_send_packet)
	{
		swap2 = !swap2;
		cmd->viewangles.x = 169;
		cmd->viewangles.y = swap2 ? -360.0000000f : 360.0000000f;

	}
	else
	{
		cmd->viewangles.x = 169;
		cmd->viewangles.y = 145 + (swap2 ? -360.0000000f : 360.0000000f);
	}


}

void autoDirection(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	auto leyepos = local_player->GetVecOrigin() + local_player->GetViewOffset();
	auto headpos = aimbot->get_hitbox_pos(local_player, 0);
	auto origin = local_player->GetAbsOrigin();

	auto checkWallThickness = [&](SDK::CBaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetVecOrigin() + pPlayer->GetViewOffset();

		SDK::Ray_t ray;
		ray.Init(newhead, eyepos);

		SDK::CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		SDK::trace_t trace1, trace2;
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;

		ray.Init(eyepos, newhead);
		INTERFACES::Trace->TraceRay(ray, MASK_SHOT_BRUSHONLY, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = closest_to_crosshair();
	auto entity = INTERFACES::ClientEntityList->GetClientEntity(index);

	float step = (2 * M_PI) / 18.f;
	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}
	if (no_active)
		cmd->viewangles.y += 180.f;
	else
		cmd->viewangles.y = RAD2DEG(bestrotation);

	last_real = cmd->viewangles.y;
}

void CAntiAim::BetaAA(SDK::CUserCmd* cmd) 
{
	float add = cmd->viewangles.y * 5000.0 + 72;
	cmd->move.y = +36000000.0;
}

void CAntiAim::do_antiaim(SDK::CUserCmd* cmd)
{
	double rate = 360.0 / 1.618033988749895;
	double yaw = fmod(static_cast<double>(3)*rate, 360.0);
	cmd->move.y += static_cast<float>(yaw);
	int random = 160 + rand() % 40;
	static float current_y = cmd->viewangles.y;
	current_y += random;

	if (GetAsyncKeyState('W'))
		cmd->move.x = 450;
	cmd->move.x = 422;
	if (GetAsyncKeyState('S'))
		cmd->move.y = -450;
	cmd->move.y = -560;
	if (GetAsyncKeyState('D'))
		cmd->move.z = 450;
	cmd->move.x = 335;
	if (GetAsyncKeyState('A'))
		cmd->move.x = 335;
	cmd->move.y = -560;
	INTERFACES::Engine->ExecuteClientCmd("bind s +back; bind w +forward; bind d +moveright; bind a +moveleft");

	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return;
	if (local_player->GetHealth() <= 0) return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon) return; auto grenade = (SDK::CBaseCSGrenade*)weapon;
	if (local_player->GetMoveType() == SDK::MOVETYPE_LADDER) return;
	if (cmd->buttons & IN_USE) return;

	if (cmd->buttons & IN_ATTACK && aimbot->can_shoot(cmd)) return;
	if (weapon->get_full_info()->type == 9) return;
	if (!SETTINGS::settings.aa_bool) return;
	if (GLOBAL::DisableAA) return;



	static float last_real; int local_flags = local_player->GetFlags();
	if ((local_player->GetVelocity().Length2D() < 80) && !(cmd->buttons & IN_JUMP))
	{
		if (SETTINGS::settings.Disableaanade)
		{
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
		}
		switch (SETTINGS::settings.aa_pitch_type)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 89.f; break;
		case 2: cmd->viewangles.x = -179.f; break;
		case 3: cmd->viewangles.x = 179.f; break;
		case 4: cmd->viewangles.x = 1080.f; break;
		}
		if (GLOBAL::should_send_packet)
		{
			switch (SETTINGS::settings.aa_fake_type)
			{
			case 0: break;
			case 1: cmd->viewangles.y += 180.000000 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_fakeadditive_val; break;
			case 2: cmd->viewangles.y += randnum(-180, 180) + SETTINGS::settings.aa_fakeadditive_val; break;
			case 3: cmd->viewangles.y += 0 + SETTINGS::settings.aa_fakeadditive_val; break;
			case 4: cmd->viewangles.y = GLOBAL::real_angles.y + 180.f + SETTINGS::settings.aa_fakeadditive_val; break;
			case 5: {
				static int y2 = -SETTINGS::settings.spinanglefake;
				int speed = SETTINGS::settings.spinspeedfake;
				y2 += speed; if (y2 >= SETTINGS::settings.spinanglefake) y2 = -SETTINGS::settings.spinanglefake;
				cmd->viewangles.y = y2 + SETTINGS::settings.aa_fakeadditive_val;
			} break;

			}
		}
		else
		{
			switch (SETTINGS::settings.aa_real_type)
			{
			case 0: break;
			case 1: cmd->viewangles.y += 180.f + SETTINGS::settings.aa_realadditive_val; break;
			case 2: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
			case 3: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_realadditive_val; break;
			case 4: {
				if (SETTINGS::settings.flip_bool)
				{
					if (next_lby_update(cmd->viewangles.y + SETTINGS::settings.delta_val, cmd))
					{
						cmd->viewangles.y = last_real + SETTINGS::settings.delta_val;
					}
					else
					{
						cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val;
						last_real = cmd->viewangles.y;
					}
				}
				else
				{
					if (next_lby_update(cmd->viewangles.y - SETTINGS::settings.delta_val, cmd))
					{
						cmd->viewangles.y = last_real - SETTINGS::settings.delta_val;
					}
					else
					{
						cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val;
						last_real = cmd->viewangles.y;
					}
				} }break;
			case 5: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive_val; break;
			case 6: {
				static int y2 = -SETTINGS::settings.spinangle;
				int speed = SETTINGS::settings.spinspeed;
				y2 += speed; if (y2 >= SETTINGS::settings.spinangle) y2 = -SETTINGS::settings.spinangle;
				SETTINGS::settings.flip_bool ? cmd->viewangles.y -= y2 + 90 + SETTINGS::settings.aa_realadditive_val : cmd->viewangles.y += y2 + 90 + SETTINGS::settings.aa_realadditive_val;
			} break;
			case 7: autoDirection(cmd); break;
			case 8: RandAa(cmd);
			}
		}
	}
	else if ((local_player->GetVelocity().Length2D() > 80) && (!(cmd->buttons & IN_JUMP) && (local_flags & FL_ONGROUND)))
	{
		if (SETTINGS::settings.Disableaanade)
		{
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
		}
		switch (SETTINGS::settings.aa_pitch1_type)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 89.f; break;
		case 2: cmd->viewangles.x = -179.f; break;
		case 3: cmd->viewangles.x = 179.f; break;
		case 4: cmd->viewangles.x = 1080.f; break;
		}
		if (GLOBAL::should_send_packet)
		{
			switch (SETTINGS::settings.aa_fake1_type)
			{
			case 0: break;
			case 1: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_fakeadditive1_val; break;
			case 2: cmd->viewangles.y += randnum(-180, 180) + SETTINGS::settings.aa_fakeadditive1_val; break;
			case 3: cmd->viewangles.y += 0 + SETTINGS::settings.aa_fakeadditive1_val; break;
			case 4: cmd->viewangles.y = GLOBAL::real_angles.y + 180.f + SETTINGS::settings.aa_fakeadditive1_val; break;
			case 5: {
				static int y2 = -SETTINGS::settings.spinanglefake;
				int speed = SETTINGS::settings.spinspeedfake;
				y2 += speed; if (y2 >= SETTINGS::settings.spinanglefake) y2 = -SETTINGS::settings.spinanglefake;
				cmd->viewangles.y = y2 + SETTINGS::settings.aa_fakeadditive1_val;
			} break;
			}
		}
		else
		{
			switch (SETTINGS::settings.aa_real1_type)
			{
			case 0: break;
			case 1: cmd->viewangles.y += 180.f + SETTINGS::settings.aa_realadditive1_val; break;
			case 2: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive1_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive1_val; break;
			case 3: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_realadditive1_val; break;
			case 4: {
				if (SETTINGS::settings.flip_bool)
				{
					if (next_lby_update(cmd->viewangles.y + SETTINGS::settings.delta1_val, cmd))
					{
						cmd->viewangles.y = last_real + SETTINGS::settings.delta1_val;
					}
					else
					{
						cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive1_val;
						last_real = cmd->viewangles.y;
					}
				}
				else
				{
					if (next_lby_update(cmd->viewangles.y - SETTINGS::settings.delta1_val, cmd))
					{
						cmd->viewangles.y = last_real - SETTINGS::settings.delta1_val;
					}
					else
					{
						cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive1_val;
						last_real = cmd->viewangles.y;
					}
				} }break;
			case 5: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive1_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive1_val; break;
			case 6: {
				static int y2 = -SETTINGS::settings.spinangle1;
				int speed = SETTINGS::settings.spinspeed1;
				y2 += speed; if (y2 >= SETTINGS::settings.spinangle1) y2 = -SETTINGS::settings.spinangle1;
				SETTINGS::settings.flip_bool ? cmd->viewangles.y -= y2 + 90 + SETTINGS::settings.aa_realadditive1_val : cmd->viewangles.y += y2 + 90 + SETTINGS::settings.aa_realadditive1_val;
			} break;
			case 7: autoDirection(cmd); break;
			case 8: RandAa(cmd);
			}
		}
	}
	else
	{
		if (SETTINGS::settings.Disableaanade)
		{
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
		} 
		switch (SETTINGS::settings.aa_pitch2_type)
		{
		case 0: break;
		case 1: cmd->viewangles.x = 89.f; break;
		case 2: cmd->viewangles.x = -179.f; break;
		case 3: cmd->viewangles.x = 179.f; break;
		case 4: cmd->viewangles.x = 1080.f; break;
		}
		if (GLOBAL::should_send_packet)
		{
			switch (SETTINGS::settings.aa_fake2_type)
			{
			case 0: break;
			case 1: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_fakeadditive2_val; break;
			case 2: cmd->viewangles.y += randnum(-180, 180) + SETTINGS::settings.aa_fakeadditive2_val; break;
			case 3: cmd->viewangles.y += 0 + SETTINGS::settings.aa_fakeadditive2_val; break;
			case 4: cmd->viewangles.y = GLOBAL::real_angles.y + 180.f + SETTINGS::settings.aa_fakeadditive2_val; break;
			case 5: {
				static int y2 = -SETTINGS::settings.spinanglefake;
				int speed = SETTINGS::settings.spinspeedfake;
				y2 += speed; if (y2 >= SETTINGS::settings.spinanglefake) y2 = -SETTINGS::settings.spinanglefake;
				cmd->viewangles.y = y2 + SETTINGS::settings.aa_fakeadditive2_val;
			} break;
			}
		}
		else
		{
			switch (SETTINGS::settings.aa_real2_type)
			{
			case 0: break;
			case 1: cmd->viewangles.y += 180.f + SETTINGS::settings.aa_realadditive2_val; break;
			case 2: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive2_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive2_val; break;
			case 3: cmd->viewangles.y += 180 + ((rand() % 15) - (15 * 0.5f)) + SETTINGS::settings.aa_realadditive2_val; break;
			case 4: {
				if (SETTINGS::settings.flip_bool)
				{
					if (next_lby_update(cmd->viewangles.y + SETTINGS::settings.delta2_val, cmd))
					{
						cmd->viewangles.y = last_real + SETTINGS::settings.delta2_val;
					}
					else
					{
						cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive2_val;
						last_real = cmd->viewangles.y;
					}
				}
				else
				{
					if (next_lby_update(cmd->viewangles.y - SETTINGS::settings.delta2_val, cmd))
					{
						cmd->viewangles.y = last_real - SETTINGS::settings.delta2_val;
					}
					else
					{
						cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive2_val;
						last_real = cmd->viewangles.y;
					}
				} }break;
			case 5: SETTINGS::settings.flip_bool ? cmd->viewangles.y -= 90 + SETTINGS::settings.aa_realadditive2_val : cmd->viewangles.y += 90 + SETTINGS::settings.aa_realadditive2_val; break;
			case 6: {
				static int y2 = -SETTINGS::settings.spinangle2;
				int speed = SETTINGS::settings.spinspeed2;
				y2 += speed; if (y2 >= SETTINGS::settings.spinangle2) y2 = -SETTINGS::settings.spinangle2;
				SETTINGS::settings.flip_bool ? cmd->viewangles.y -= y2 + 90 + SETTINGS::settings.aa_realadditive2_val : cmd->viewangles.y += y2 + 90 + SETTINGS::settings.aa_realadditive2_val;
			} break;
			case 7: autoDirection(cmd); break;
			case 8: RandAa(cmd);
			}
		}
	}
}

void CAntiAim::fix_movement(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (SETTINGS::settings.aa_pitch > 1 || SETTINGS::settings.aa_pitch > 1 || SETTINGS::settings.aa_pitch > 1)
	{
		cmd->move.x = clamp(cmd->move.x, -450.f, 450.f);
		cmd->move.y = clamp(cmd->move.y, -450.f, 450.f);
		cmd->move.z = clamp(cmd->move.z, -320.f, 320.f);

		cmd->viewangles.x = clamp(cmd->viewangles.x, -89.f, 89.f);
	}

	Vector real_viewangles;
	INTERFACES::Engine->GetViewAngles(real_viewangles);

	Vector vecMove(cmd->move.x, cmd->move.y, cmd->move.z);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	Vector angMove;
	MATH::VectorAngles(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

	cmd->move.x = cos(yaw) * speed;
	cmd->move.y = sin(yaw) * speed;

	cmd->viewangles = MATH::NormalizeAngle(cmd->viewangles);

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->move.x *= -1;
}

Vector CAntiAim::fix_movement(SDK::CUserCmd* cmd, SDK::CUserCmd originalCMD)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
	if (!local_player) return Vector(0, 0, 0);

	Vector wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

	auto viewangles = cmd->viewangles;
	auto movedata = originalCMD.move;
	viewangles.Normalize();

	if (!(local_player->GetFlags() & FL_ONGROUND) && viewangles.z != 0.f)
		movedata.y = 0.f;

	MATH::AngleVectors2(originalCMD.viewangles, &wish_forward, &wish_right, &wish_up);
	MATH::AngleVectors2(viewangles, &cmd_forward, &cmd_right, &cmd_up);

	auto v8 = sqrt(wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y), v10 = sqrt(wish_right.x * wish_right.x + wish_right.y * wish_right.y), v12 = sqrt(wish_up.z * wish_up.z);

	Vector wish_forward_norm(1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f),
		wish_right_norm(1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f),
		wish_up_norm(0.f, 0.f, 1.0f / v12 * wish_up.z);

	auto v14 = sqrt(cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y), v16 = sqrt(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y), v18 = sqrt(cmd_up.z * cmd_up.z);

	Vector cmd_forward_norm(1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f),
		cmd_right_norm(1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f),
		cmd_up_norm(0.f, 0.f, 1.0f / v18 * cmd_up.z);

	auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x, v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y, v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

	Vector correct_movement;
	correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25
		+ (cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28)
		+ (cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
	correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25
		+ (cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28)
		+ (cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
	correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25
		+ (cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28)
		+ (cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

	correct_movement.x = clamp(correct_movement.x, -450.f, 450.f);
	correct_movement.y = clamp(correct_movement.y, -450.f, 450.f);
	correct_movement.z = clamp(correct_movement.z, -320.f, 320.f);

	return correct_movement;
}

CAntiAim* antiaim = new CAntiAim();

