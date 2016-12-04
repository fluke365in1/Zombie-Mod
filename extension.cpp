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
 * @file extension.cpp
 * @brief Implement extension code here.
 */

#include "extension.h"



/* Extension modules */
//#include "cvars.cpp"
#include "playertools.cpp"
#include "events.cpp"
#include "natives.cpp"
//#include "zombie.cpp"

/* Global variables for extension's main interface */
ZombieMod g_ZombieMod;	
CGlobalVars *gpGlobals = NULL;
IGameEventManager2 *gameevents = NULL;
IPlayerInfoManager *playerinfomngr = NULL;
SMEXT_LINK(&g_ZombieMod);

// Metamod hooks
SH_DECL_HOOK3_void(IServerGameDLL, ServerActivate, SH_NOATTRIB, 0, edict_t *, int, int);

/**
 * @brief This is called once all known extensions have been loaded.
 */
void ZombieMod::SDK_OnAllLoaded()
{
	// Add listeners
	playerhelpers->AddClientListener(&g_ClientListener);
	
	// Hook server events
    HOOK_EVENT2(round_prestart);
	HOOK_EVENT2(round_start);
    HOOK_EVENT2(round_end);
	
	// Hook player events
	HOOK_EVENT2(player_spawn);
	HOOK_EVENT2(player_death);

	// Hook map load
	SH_ADD_HOOK_STATICFUNC(IServerGameDLL, ServerActivate, gamedll, ServerActivate, true);
	
	// Version info 
	g_SMAPI->ConPrintf("\n%s [%s]\n%s\n%s\n%s\n\n", SMEXT_CONF_NAME, SMEXT_CONF_VERSION, SMEXT_CONF_AUTHOR, SMEXT_CONF_URL, SMEXT_CONF_LICENSE);
}

/**
 * @brief This is called after the initial loading sequence has been processed.
 *
 * @param error		Error message buffer.
 * @param maxlength	Size of error message buffer.
 * @param late		Whether or not the module was loaded after map load.
 * @return			True to succeed loading, false to fail.
 */
bool ZombieMod::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	// Register library
	sharesys->AddNatives(myself, g_ZombieNatives);
	sharesys->RegisterLibrary(myself, "zombiemod");
	
	// Initialize globals
	gpGlobals = g_SMAPI->GetCGlobals();
	
	// Return on success
	return true;
}

/**
 * @brief This is called right before the extension is unloaded.
 */
void ZombieMod::SDK_OnUnload()
{
	// Remove listeners
	playerhelpers->RemoveClientListener(&g_ClientListener);
	
	// Unhook server events
    UNHOOK_EVENT2(round_prestart);
	UNHOOK_EVENT2(round_start);
    UNHOOK_EVENT2(round_end);
	
	// Unhook player events
	UNHOOK_EVENT2(player_spawn);
	UNHOOK_EVENT2(player_death);
	
	// Unhook map load
	SH_REMOVE_HOOK_STATICFUNC(IServerGameDLL, ServerActivate, gamedll, ServerActivate, true);
}

/**
 * @brief Called when Metamod is attached, before the extension version is called.
 *
 * @param error			Error buffer.
 * @param maxlength		Maximum size of error buffer.
 * @param late			Whether or not Metamod considers this a late load.
 * @return				True to succeed, false to fail.
 */
bool ZombieMod::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	// Load interfaces
	GET_V_IFACE_CURRENT(GetEngineFactory, gameevents, IGameEventManager2, INTERFACEVERSION_GAMEEVENTSMANAGER2);
	GET_V_IFACE_ANY(GetServerFactory, playerinfomngr, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);
	
	// Return on success
	return true;
}

/**
 * @brief Called when map is loaded.
 *
 * @param pEdictList		Edict list buffer.
 * @param edictCount		Maximum amount of entities.
 * @param clientMax			Maximum amount of players.
 */
void ServerActivate(edict_t *pEdictList, int edictCount, int clientMax)
{
	// Create timer for starting game mode
	timersys->CreateTimer(&s_EventsCounter, 1.0, 0, TIMER_FLAG_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
	
	//char *model;
	//ZombieGetModel(0, model);
	//engine->PrecacheModel("models/player/custom_player/cso2_zombi/police.mdl", true);
	
	/*
	// Execute main config file
	engine->ServerCommand("exec sourcemod/zombiemod.cfg\n");
	
	// Execute map config file
	char sMapConfig[BIG_LINE_LENGTH];
	snprintf(sMapConfig, sizeof(sMapConfig), "exec sourcemod/zombiemod/%s.cfg\n", STRING(gpGlobals->mapname));
	engine->ServerCommand(sMapConfig);
	*/
	
	// Return on success
	RETURN_META(MRES_IGNORED);
}