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

/* Main header */
#include "extension.h"

/* Modules includes */
#include "metamod.cpp"
#include "cvars.cpp"
#include "playertools.cpp"
#include "natives.cpp"
#include "forwards.cpp"
#include "utils.cpp"
#include "events.cpp"

/**
 * @file extension.cpp
 * @brief Implement main extension code here.
 */

/* Global singleton for extension's main interface */
ZombieMod g_ZombieMod;		
SMEXT_LINK(&g_ZombieMod);

/**
 * @brief This is called once all known extensions have been loaded.
 * Note: It is is a good idea to add natives here, if any are provided.
 */
void ZombieMod::SDK_OnAllLoaded()
{
	// Forward function to modules
	MetamodInit();
	EventInit();
	ToolInit();
	UtilInit();

	// Return on success
	return;
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
	// Forward function to modules
	MetamodLoad();
	NativeLoad();
	ForwardLoad();
	CvarLoad();
	
	// Return on success
	return true;
}

/**
 * @brief This is called right before the extension is unloaded.
 */
void ZombieMod::SDK_OnUnload()
{
	// Forward function to modules
	MetamodPurge();
	EventPurge();
	ToolPurge();
	ForwardPurge();
	UtilPurge();
	
	// Return on success
	return;
}

/**
 * @brief Called when Metamod is attached, before the extension version is called.
 *
 * @param error			Error buffer.
 * @param maxlen		Maximum size of error buffer.
 * @param late			Whether or not Metamod considers this a late load.
 * @return				True to succeed, false to fail.
 */
bool ZombieMod::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	// Load interfaces
	GET_V_IFACE_ANY(GetServerFactory, gameents, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS);
	GET_V_IFACE_ANY(GetServerFactory, playerinfomngr, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);
	GET_V_IFACE_ANY(GetServerFactory, botmanager, IBotManager, INTERFACEVERSION_PLAYERBOTMANAGER);
	GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_CURRENT(GetEngineFactory, gameevents, IGameEventManager2, INTERFACEVERSION_GAMEEVENTSMANAGER2);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
	
	// Return on success
	return true;
}

/**
 * @brief this is called when Core wants to know if your extension is working.
 *
 * @param error		Error message buffer.
 * @param maxlength	Size of error message buffer.
 * @return			True if working, false otherwise.
 */
bool ZombieMod::QueryRunning(char *error, size_t maxlength)
{
	// Validate bintools
	SM_CHECK_IFACE(BINTOOLS, bintools);
	
	//Return on success
	return true;
}

/**
 * @brief Asks the extension whether it's safe to remove an external 
 * interface it's using.  If it's not safe, return false, and the 
 * extension will be unloaded afterwards.
 *
 * NOTE: It is important to also hook NotifyInterfaceDrop() in order to clean 
 * up resources.
 *
 * @param pInterface		Pointer to interface being dropped.  This 
 * 							pointer may be opaque, and it should not 
 *							be queried using SMInterface functions unless 
 *							it can be verified to match an existing 
 *							pointer of known type.
 * @return					True to continue, false to unload this 
 * 							extension afterwards.
 */
bool ZombieMod::QueryInterfaceDrop(SMInterface *pInterface)
{
	// Validate interface dropping
	return IsMetamodBintools(pInterface) ? false : IExtensionInterface::QueryInterfaceDrop(pInterface);
}

/**
 * @brief Notifies the extension that an external interface it uses is being removed.
 *
 * @param pInterface		Pointer to interface being dropped.  This
 * 							pointer may be opaque, and it should not 
 *							be queried using SMInterface functions unless 
 *							it can be verified to match an existing 
 */
void ZombieMod::NotifyInterfaceDrop(SMInterface *pInterface)
{
	// We have to take care of dependencies modules early
	if (IsMetamodBintools(pInterface))
	{
		UtilInterfaceDrop();
		bintools = NULL;
	}
}

/**
 * @brief Called when server's map was loaded.
 *
 * @param pEdictList		Edict list buffer.
 * @param edictCount		Maximum amount of entities.
 * @param clientMax			Maximum amount of players.
 */
void SDK_OnMapload(edict_t *pEdictList, int edictCount, int clientMax)
{
	// Forward function to modules
	EventActivated();
	CvarActivated();
	
	// Return on success
	RETURN_META(MRES_IGNORED);
}

/**
 * @brief Called when server's map was unloaded.
 */
void SDK_OnMapUnload()
{
	// Forward function to modules
	NaviteShutDown();
	
	// Return on success
	RETURN_META(MRES_IGNORED);
}