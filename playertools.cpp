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
 * @file playertools.cpp
 * @brief Player tools module code file.
 */

#include "playertools.h"


/* Initialize player data array */
CBaseClient g_PlayersList[65];

/**
 * @brief Gets the client data array.
 *
 * @param clientIndex	The client index.
 * @return				The client data.
 **/
inline CBaseClient *GetPlayer(int clientIndex)
{
	// Validate client index
	if (clientIndex < 1 || clientIndex > globals->maxClients)
	{
		return NULL;
	}

	// Return client data array
	return &g_PlayersList[clientIndex];
}

/**
 * @brief Gets the cbaseentity index.
 *
 * @param entityIndex	The entity index.
 * @return				The cbaseentity index.
 **/
inline CBaseEntity *GetBaseEntity(int entityIndex)
{
	// Validate edict
	edict_t *pEdict = gamehelpers->EdictOfIndex(entityIndex);
	if (!pEdict || pEdict->IsFree())
	{
		return NULL;
	}
	
	// Validate client
	IGamePlayer *pGamePlayer = playerhelpers->GetGamePlayer(pEdict);
	if (!pGamePlayer || !pGamePlayer->IsConnected())
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
 * @brief Gets the offset index.
 *
 * @param pEntity		The cbaseentity index.
 * @param sName			The name of offset.	 
 * @param offset		The ofset index.
 * @return				True or false.
 **/
inline bool GetDataMapOffset(CBaseEntity *pEntity, const char *sName, unsigned int &offset)
{
	// Validate data map 
    datamap_t *pMap = gamehelpers->GetDataMap(pEntity);
    if (!pMap)
    {
        return false;
    }
    
	// Get offset in the data map
    typedescription_t *typedesc = gamehelpers->FindInDataMap(pMap, sName);
    
	// Validate it
    if (typedesc == NULL)
    {
        return false;
    }

	// Return success and store offset
    offset = typedesc->fieldOffset;
    return true;
}  

/**
 * @brief Initialize clients listeners.
 */
void ToolInit()
{
	// Set listeners
	playerhelpers->AddClientListener(&g_ClientListener);
}

/**
 * @brief Destroy clients listeners.
 */
void ToolPurge()
{
	// Remove listeners
	playerhelpers->RemoveClientListener(&g_ClientListener);
}

/**
 * @brief Called once a client successfully connects.
 *
 * @param clientIndex 	The client index.
 */
void ClientListener::OnClientConnected(int clientIndex)
{
	// Reset player data array
	memset(&g_PlayersList[clientIndex], 0, sizeof(CBaseClient));
}

/**
 * @brief Called when a client is entering the game.
 *
 * @param clientIndex 	The client index.
 */
void ClientListener::OnClientPutInServer(int clientIndex)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(clientIndex);
	if (!CPlayer)
	{
		return;
	}
	
	// Validate entity
	CBaseEntity *pEntity = GetBaseEntity(clientIndex);
	if (!pEntity)
	{
		return;
	}

	// Update main player variables
	CPlayer->pEdict 			= gamehelpers->EdictOfIndex(clientIndex);
	CPlayer->pEntity 			= pEntity;
	CPlayer->pInfo 				= playerinfomngr->GetPlayerInfo(CPlayer->pEdict);
	CPlayer->pBot 				= botmanager->GetBotController(CPlayer->pEdict);
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
	CBaseClient *CPlayer = GetPlayer(clientIndex);
	if (!CPlayer)
	{
		return;
	}

	// Reset client data array
	memset(CPlayer, 0, sizeof(CBaseClient));
}

/**
 * @brief Returns if the client is alive or dead.
 *
 * @param CPlayer		The client data.
 * @return				True if the client is alive, false otherwise.
 **/
bool IsPlayerAlive(CBaseClient *CPlayer)
{
	return (!CPlayer->pInfo || CPlayer->pInfo->IsObserver() || CPlayer->pInfo->IsDead()) ? false : true;
}

/**
 * @brief Retrieves a client's team index.
 *
 * @param CPlayer		The client data.
 * @return				Team index the client is on (mod specific).			
 **/
int GetPlayerTeamIndex(CBaseClient *CPlayer)
{
	return CPlayer->pInfo->GetTeamIndex();
}

/**
 * @brief Returns the client's frag count.
 *
 * @param CPlayer		The client data.
 * @return				The frag count.	
 **/
int GetPlayerFragCount(CBaseClient *CPlayer)
{
	return CPlayer->pInfo->GetFragCount();
}

/**
 * @brief Returns the client's death count.
 *
 * @param CPlayer		The client data.
 * @return				The death count.	
 **/
int GetPlayerDeathCount(CBaseClient *CPlayer)
{
	return CPlayer->pInfo->GetDeathCount();
}

/**
 * @brief Returns the client's health.
 *
 * @param CPlayer		The client data.
 * @return				The health value.
 **/
int GetPlayerHealthValue(CBaseClient *CPlayer)
{
	return CPlayer->pInfo->GetHealth();
}

/**
 * @brief Returns the client's armor.
 *
 * @param CPlayer		The client data.
 * @return				The armor value.
 **/
int GetPlayerArmorValue(CBaseClient *CPlayer)
{
	return CPlayer->pInfo->GetArmorValue();
}