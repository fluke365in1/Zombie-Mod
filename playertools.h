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
 
#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PLAYERTOOLS_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PLAYERTOOLS_H_
 
/**
 * @file playertools.h
 * @brief Player tools code header.
 */
 
#include <edict.h>
#include <iplayerinfo.h>
#include <iserverentity.h>

/**
 * List of the player data array.
 **/
struct ZPBaseClient
{
	edict_t *pEdict;
	CBaseEntity *pEntity;
	IPlayerInfo *pInfo;
	ITimer *respawn_timer;
	int userid;
	bool m_bZombie;
	unsigned int m_nZombieClass;
	unsigned int m_nZombieClassNext;
	unsigned int m_nRespawnTimes;
};

/**
 * @brief Initialize the client listeners.
 */
class ClientListener : public IClientListener
{
public:

	/**
	 * @brief Called once a client successfully connects.
	 *
	 * @param clientIndex The client index.
	 */
	void OnClientConnected(int clientIndex);
	
	/**
	 * @brief Called when a client is entering the game.
	 *
	 * @param clientIndex The client index.
	 */
	void OnClientPutInServer(int clientIndex);
	
	/**
	 * @brief Called when a client is disconnected from the server.
	 *
	 * @param clientIndex The client index.
	 */
	void OnClientDisconnected(int clientIndex);

} g_ClientListener;

/* Extern player variables */
extern IPlayerInfoManager *playerinfomngr;
extern CGlobalVars *gpGlobals;

/**
 * @brief Validate data array of the player.
 *
 * @param clientIndex	The client index.
 * @return				The data array.
 **/
ZPBaseClient *ZP_GetPlayer(int clientIndex);

/**
 * @brief Validate index of the entity.
 *
 * @param index			The index.
 * @return				The entity index.
 **/
CBaseEntity *ZP_GetBaseEntity(int clientIndex);

/**
 * @brief Validate aliveness of the client.
 *
 * @param index			The index.
 * @return				True or false.
 **/
bool ZP_IsPlayerAlive(int clientIndex);

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PLAYERTOOLS_H_