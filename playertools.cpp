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
 * @file playertools.cpp
 * @brief Player tools module code file.
 */

#include "playertools.h"

/* Initialize player variables */
ZPBaseClient g_PlayersList[MAXPLAYERS+1];

/**
 * @brief Called once a client successfully connects.
 *
 * @param clientIndex 	The client index.
 */
void ClientListener::OnClientConnected(int clientIndex)
{
	// Reset player data array
	memset(&g_PlayersList[clientIndex], 0, sizeof(ZPBaseClient));
}

/**
 * @brief Called when a client is entering the game.
 *
 * @param clientIndex 	The client index.
 */
void ClientListener::OnClientPutInServer(int clientIndex)
{
	// Validate client
	ZPBaseClient *CPlayer = ZP_GetPlayer(clientIndex);
	if (!CPlayer)
	{
		return;
	}
	
	// Validate entity
	CBaseEntity *pEntity = ZP_GetBaseEntity(clientIndex);
	if (!pEntity)
	{
		return;
	}

	// Update main player variables
	CPlayer->pEdict 			= gamehelpers->EdictOfIndex(clientIndex);
	CPlayer->pEntity 			= pEntity;
	CPlayer->pInfo 				= playerinfomngr->GetPlayerInfo(CPlayer->pEdict);
	CPlayer->userid 			= engine->GetPlayerUserId(CPlayer->pEdict);
}

/**
 * @brief Called when a client is disconnected from the server.
 *
 * @param clientIndex 	The client index.
 */
void ClientListener::OnClientDisconnected(int clientIndex)
{
	// Validate client
	ZPBaseClient *CPlayer = ZP_GetPlayer(clientIndex);
	if (!CPlayer)
	{
		return;
	}

	// Kill respawn timer
	if (CPlayer->respawn_timer)
	{
		timersys->KillTimer(CPlayer->respawn_timer);
	}
	
	// Reset client data array
	memset(CPlayer, 0, sizeof(ZPBaseClient));
}

/**
 * @brief Validate data array of the client.
 *
 * @param clientIndex	The client index.
 * @return				The data array.
 **/
ZPBaseClient *ZP_GetPlayer(int clientIndex)
{
	// Validate index
	if (clientIndex < 1 || clientIndex > gpGlobals->maxClients)
	{
		return NULL;
	}

	// Return client data array
	return &g_PlayersList[clientIndex];
}

/**
 * @brief Validate index of the entity.
 *
 * @param index			The index.
 * @return				The entity index.
 **/
CBaseEntity *ZP_GetBaseEntity(int index)
{
	// Validate entity
	edict_t *pEdict = gamehelpers->EdictOfIndex(index);
	if (!pEdict || pEdict->IsFree())
	{
		return NULL;
	}
	
	// Validate entity
	IServerUnknown *pUnknown = pEdict->GetUnknown();
	if (!pUnknown)
	{
		return NULL;
	}
	
	// Return index
	return pUnknown->GetBaseEntity();
}

/**
 * @brief Validate aliveness of the client.
 *
 * @param index			The index.
 * @return				True or false.
 **/
bool ZP_IsPlayerAlive(int clientIndex)
{
	// Validate client
	ZPBaseClient *CPlayer = ZP_GetPlayer(clientIndex);
	if (!CPlayer || !CPlayer->pEntity)
	{
		return false;
	}

	// Return true on success
	return CPlayer->pInfo->IsDead() ? false : true;
}