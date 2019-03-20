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

#include "metamod.h"

/**
 * @file metamod.cpp
 * @brief Metamod module code here.
 */
 
 
/* Metamod interfaces */
ICvar *icvar = NULL;
IBinTools *bintools = NULL;
CGlobalVars *globals = NULL;
IServerGameEnts *gameents = NULL;
IGameEventManager2 *gameevents = NULL;
IPlayerInfoManager *playerinfomngr = NULL;
IBotManager *botmanager = NULL;
IServerGameDLL *server = NULL;

/* Metamod server hooks */
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int);
SH_DECL_HOOK0_void(IServerGameDLL, LevelShutdown, SH_NOATTRIB, 0);

/**
 * @brief Initialize metamod hooks.
 */
void MetamodInit()
{
	// Hook metamod events
	SH_ADD_HOOK_STATICFUNC(IServerGameDLL, ServerActivate, gamedll, SDK_OnMapload, true);
	SH_ADD_HOOK_STATICFUNC(IServerGameDLL, LevelShutdown,  gamedll, SDK_OnMapUnload, false);
	
	// Load tools module
	SM_GET_LATE_IFACE(BINTOOLS, bintools);
}

/**
 * @brief Initializing metamod dependencies.
 */
void MetamodLoad()
{
	// Added dependency to module
	sharesys->AddDependency(myself, "bintools.ext", true, true);
	
	// Get globals variables
	globals = g_SMAPI->GetCGlobals();
}

/**
 * @brief Destroy metamod hooks.
 */
void MetamodPurge()
{
	// Hook metamod events
	SH_REMOVE_HOOK_STATICFUNC(IServerGameDLL, ServerActivate, gamedll, SDK_OnMapload, true);
	SH_REMOVE_HOOK_STATICFUNC(IServerGameDLL, LevelShutdown,  gamedll, SDK_OnMapUnload, false);
}

/**
 * @brief Validate metamod bintools dependency.
 *
 * @param pInterface		Pointer to interface being dropped.  This
 * 							pointer may be opaque, and it should not 
 *							be queried using SMInterface functions unless 
 *							it can be verified to match an existing 
 */
bool IsMetamodBintools(SMInterface *pInterface)
{
	// Validate interface
	if (pInterface == bintools)
	{
		return true;
	}
	
	//Return on failure
	return false;
}