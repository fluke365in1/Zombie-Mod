/**
 * =============================================================================
 * SourceMod Zombie Escape Extension
 * Copyright (C) 2015-2018 Nikita Ushakov.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 */

/**
 * @file cvars.cpp
 * @brief Cvars module code file.
 */

#include "cvars.h"


/**
 * @brief Initialize cvars.
 */
void CvarLoad()
{
	g_pCVar = icvar;
	ConVar_Register(0, &s_LinkConVars);
}

/**
 * @brief Load cvars config.
 */
void CvarActivated()
{
	engine->ServerCommand("exec zombiemod.cfg\n");
}

// Game Purpose
// ----------
ConVar zm_delay_time("zm_delay_time", "30", 0, "Time before any game mode starts in seconds");
ConVar zm_infect_ratio("zm_infect_ratio", "0.25", 0, "Infect ratio (zombie count = ratio * player count)");

/**
 * @brief Gets the time before any game mode starts in seconds.
 */
unsigned int GetGameDelay()
{
	return zm_delay_time.GetInt();
}

/**
 * @brief Gets the infect ratio.
 */
float GetInfectRatio()
{
	return zm_infect_ratio.GetFloat();
}

// Deathmatch
// ----------
ConVar zm_deathmatch("zm_deathmatch", "0", 0, "Deathmatch mode during normal rounds, respawn as: [0-zombie // 1-human // 2-randomly // 3-balance // 4-disabled]");
ConVar zm_respawn_time("zm_respawn_time", "5.0", 0, "Delay before respawning on deathmatch mode in seconds");
ConVar zm_respawn_on_suicide("zm_respawn_on_suicide", "1", 0, "Respawn players if they commited suicide [0-no // 1-yes]");
ConVar zm_respawn_zombies("zm_respawn_zombies", "1", 0, "Whether to respawn killed zombies [0-no // 1-yes]");
ConVar zm_respawn_humans("zm_respawn_humans", "0", 0, "Whether to respawn killed humans [0-no // 1-yes]");

/**
 * @brief Gets the deathmatch mode.
 */
unsigned int GetDeathMatchMode()
{
	return zm_deathmatch.GetInt();
}

/**
 * @brief Gets the respawn time.
 */
float GetRespawnTime()
{
	return zm_respawn_time.GetFloat();
}

/**
 * @brief Allow the respawn on suicide.
 */
bool AllowRespawnOnSuicide()
{
	return zm_respawn_on_suicide.GetInt() ? true : false;
}

/**
 * @brief Allow the respawn for zombies.
 */
bool AllowRespawnZombies()
{
	return zm_respawn_zombies.GetInt() ? true : false;
}

/**
 * @brief Allow the respawn for humans.
 */
bool AllowRespawnHuman()
{
	return zm_respawn_humans.GetInt() ? true : false;
}

// Zombies
// ----------
ConVar zm_zombie_additional_health("zm_zombie_additional_health", "5000", 0, "Additional health to first zombie");
ConVar zm_zombie_nvg_give("zm_zombie_nvg_give", "1", 0, "Enable custom nightvision [0-no // 1-yes]");
ConVar zm_zombie_xray_give("zm_zombie_xray_give", "1", 0, "Enable custom x-ray for viewing through walls [0-no // 1-yes]");
ConVar zm_zombie_fov("zm_zombie_fov", "120", 0, "Set fov eye distance");
ConVar zm_zombie_silent("zm_zombie_silent", "0", 0, "Enable silent footsteps [0-no // 1-yes]");

/**
 * @brief Gets the zombie extra health.
 */
int GetZombieExtraHealth()
{
	return zm_zombie_additional_health.GetInt();
}

/**
 * @brief Gets the zombie nvg mode.
 */
bool GetZombieNvg()
{
	return zm_zombie_nvg_give.GetInt() ? true : false;
}

/**
 * @brief Gets the zombie xray mode.
 */
bool GetZombieXray()
{
	return zm_zombie_xray_give.GetInt() ? true : false;
}

/**
 * @brief Gets the zombie fov.
 */
int GetZombieFov()
{
	return zm_zombie_fov.GetInt();
}

/**
 * @brief Gets the zombie silent mod.
 */
/*bool GetZombieSilent()
{
	return zm_zombie_silent.GetInt();
}*/