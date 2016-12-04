/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Zombie Mod Extension by Ushakov Nikita
 * Copyright (C) 2015-2017 AlliedModders LLC.  All rights reserved.
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
 *
 * Version: $Id$
 */

/**
 * @file events.cpp
 * @brief Events module code file.
 */

#include "events.h"
 
/* Global hooks */
DECLARE_EVENT(round_prestart);
DECLARE_EVENT(round_start);
DECLARE_EVENT(round_end);
DECLARE_EVENT(player_spawn);
DECLARE_EVENT(player_death);

/* Round variables */
bool g_IsNewRound;
bool g_IsEndRound;
bool g_IsOnRound;
unsigned int g_nCounter;

/**
 * @brief Event callback (round_prestart).
 */
IMPLEMENT_EVENT(round_prestart)
{
    // Reset server grobal bools
    g_IsNewRound = true;
    g_IsEndRound = false;
    g_IsOnRound  = false;
	
	// Restore default time for zombie event timer
	g_nCounter = 30;	//	GetConVarInt(gCvarList[CVAR_GAME_ROUND_START]);
	
	// Debug
	g_SMAPI->ConPrintf("round_prestart");
}

/**
 * @brief Event callback (round_start).
 */
IMPLEMENT_EVENT(round_start)
{
	// Get max amount of entities
	//int nGetMaxEnt = gpGlobals->maxEntities;
	
	// nEntity = entity index
	/*
	for (int i = 0; i <= nGetMaxEnt; i++)
	{
		// If entity isn't valid, then stop
		if(!IsValidEdict(nEntity))
		{
			continue;	
		}
		
		// Get valid edict's classname
		GetEdictClassname(nEntity, sClassname, sizeof(sClassname));
		
		// Check if it matches any objective entities, then stop if it doesn't
		if(StrContains(ROUNDSTART_OBJECTIVE_ENTITIES, sClassname) != -1) 
		{
			// Entity is an objective, kill it
			RemoveEdict(nEntity);
		}
		
		// If entity isn't valid, then skip
		edict_t *current = gamehelpers->EdictOfIndex(i);
		if (current == NULL || current->IsFree())
		{
			continue;	
		}

		// If entity isn't valid, then skip
		IServerNetworkable *network = current->GetNetworkable();
		if (network == NULL)
		{
			continue;	
		}

		// Get valid edict's classname
		ServerClass *sClass = network->GetServerClass();
		const char *name = sClass->GetName();

		// Check if it matches any objective entities, then stop if it doesn't
		if (!strcmp(name, classname))
		{
			
		}
	}*/
	
	
	// Debug
	g_SMAPI->ConPrintf("round_start");
}

/**
 * @brief Event callback (round_end).
 */
IMPLEMENT_EVENT(round_end)
{
    // Reset server grobal bools
    g_IsNewRound = false;
    g_IsEndRound = true;
    g_IsOnRound  = false;
	
	// Debug
	g_SMAPI->ConPrintf("round_end");
}

/**
 * @brief Event callback (player_spawn).
 */
IMPLEMENT_EVENT(player_spawn)
{
	// Debug
	g_SMAPI->ConPrintf("player_spawn");
}

/**
 * @brief Event callback (player_death).
 */
IMPLEMENT_EVENT(player_death)
{
	// Debug
	g_SMAPI->ConPrintf("player_death");
}