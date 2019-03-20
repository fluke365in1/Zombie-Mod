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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

/**
 * @file extension.h
 * @brief Sample extension code header.
 */

/* Source mod sdk includes */
#include "smsdk_ext.h"

/* Source mod tools includes */
#include <IBinTools.h>
#include <ISDKTools.h>

/* Game sdk includes */
#include <edict.h>
#include <iplayerinfo.h>
#include <igameevents.h>
#include <eiface.h>
#include <sh_list.h>
#include <itoolentity.h>
#include <vstdlib/random.h>
#include <server_class.h>
#include <datamap.h>

/* Teams */
#define ZM_TEAM_T			2
#define ZM_TEAM_CT			3

/**
 * @brief The main implementation of the Zombie SDK Extension.
 */
class ZombieMod : public SDKExtension
{
public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
	
	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload();

	/**
	 * @brief This is called once all known extensions have been loaded.
	 * Note: It is is a good idea to add natives here, if any are provided.
	 */
	virtual void SDK_OnAllLoaded();

	/**
	 * @brief this is called when Core wants to know if your extension is working.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @return			True if working, false otherwise.
	 */
	virtual bool QueryRunning(char *error, size_t maxlength);
	
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
	bool QueryInterfaceDrop(SMInterface *pInterface);
	
	/**
	 * @brief Notifies the extension that an external interface it uses is being removed.
	 *
	 * @param pInterface		Pointer to interface being dropped.  This
	 * 							pointer may be opaque, and it should not 
	 *							be queried using SMInterface functions unless 
	 *							it can be verified to match an existing 
	 */
	void NotifyInterfaceDrop(SMInterface *pInterface);
public:
#if defined SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late);
#endif
};

/**
 * @brief Called when server's map was loaded.
 *
 * @param pEdictList		Edict list buffer.
 * @param edictCount		Maximum amount of entities.
 * @param clientMax			Maximum amount of players.
 */
void SDK_OnMapload(edict_t *pEdictList, int edictCount, int clientMax);

/**
 * @brief Called when server's map was unloaded.
 */
void SDK_OnMapUnload();

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
