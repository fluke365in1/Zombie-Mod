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
 * @file utils.cpp
 * @brief Utils module code file.
 */

#include "utils.h"


/* Use sourcemod extension spacename */
using namespace SourceMod;
using namespace SourceHook;

/* Config interface */
IGameConfig *g_pZombieConfig = NULL;

/* Wrappers variables */
List<ICallWrapper *> g_CallWrappers;
DECLARE_WRAPPER(RemoveAllItems);
DECLARE_WRAPPER(SetEntityModel);
DECLARE_WRAPPER(SwitchTeam);
DECLARE_WRAPPER(RoundRespawn);

/* Offsets variables */
DECLARE_OFFSET(m_iHealth);
DECLARE_OFFSET(m_ArmorValue);
DECLARE_OFFSET(m_flLaggedMovementValue);
DECLARE_OFFSET(m_iFrags);
DECLARE_OFFSET(m_iDeaths);
DECLARE_OFFSET(m_bNightVisionOn);
DECLARE_OFFSET(m_iFOV);
DECLARE_OFFSET(m_iDefaultFOV);
DECLARE_OFFSET(m_flDetectedByEnemySensorTime);
DECLARE_OFFSET(m_flGravity);

/**
 * @brief Initialize utils.
 */
void UtilInit()
{
	// Initialize variables
	void *addr;
	int offset;
	char path[256];
	sm_sendprop_info_t prop;
	SourceMod::PassInfo pass[4];

	// Load offset config
	if (!gameconfs->LoadGameConfigFile("zombiemod.games", &g_pZombieConfig, path, sizeof(path)))
	{
		return;
	}
	
	// Initialize wrapper
	INITIALIZE_WRAPPER_ADDR(RemoveAllItems, 
				bintools->CreateVCall(offset, 0, 0, NULL, pass, 1),
				pass[0].flags = PASSFLAG_BYVAL; \
				pass[0].size = sizeof(bool); \
				pass[0].type = PassType_Basic);
	
	// Initialize wrapper
	INITIALIZE_WRAPPER_ADDR(SetEntityModel, 
				bintools->CreateVCall(offset, 0, 0, NULL, pass, 1),
				pass[0].flags = PASSFLAG_BYVAL; \
				pass[0].size = sizeof(void *); \
				pass[0].type = PassType_Basic);
				
	// Initialize wrapper
	INITIALIZE_WRAPPER_SIG(SwitchTeam, 
				bintools->CreateCall(addr, CallConv_ThisCall, NULL, pass, 1),
				pass[0].flags = PASSFLAG_BYVAL; \
				pass[0].size = sizeof(int); \
				pass[0].type = PassType_Basic);
	
	// Initialize wrapper
	INITIALIZE_WRAPPER_SIG(RoundRespawn,
				bintools->CreateCall(addr, CallConv_ThisCall, NULL, NULL, 0),
				/* empty statement */ );	   
				
	// Initialize offsets
	INITIALIZE_OFFSET(CCSPlayer, m_iHealth);
	INITIALIZE_OFFSET(CCSPlayer, m_ArmorValue);
	INITIALIZE_OFFSET(CCSPlayer, m_flLaggedMovementValue); 
	INITIALIZE_OFFSET(CCSPlayer, m_iFrags);
	INITIALIZE_OFFSET(CCSPlayer, m_iDeaths);
	INITIALIZE_OFFSET(CCSPlayer, m_bNightVisionOn);
	INITIALIZE_OFFSET(CCSPlayer, m_iFOV);
	INITIALIZE_OFFSET(CCSPlayer, m_iDefaultFOV);
	INITIALIZE_OFFSET(CCSPlayer, m_flDetectedByEnemySensorTime);
	
	// Print version on success
	g_SMAPI->ConPrintf("\n%s [%s]\n%s\n%s\n%s\n\n", SMEXT_CONF_NAME, SMEXT_CONF_VERSION, SMEXT_CONF_AUTHOR, SMEXT_CONF_URL, SMEXT_CONF_LICENSE);
}

/**
 * @brief Close offsets config.
 */
void UtilPurge()
{
	// Close offset config
	gameconfs->CloseGameConfigFile(g_pZombieConfig);
}

/**
 * @brief Destroy utils during inferface dropping.
 */
void UtilInterfaceDrop()
{
	// Remove all wrappers
	List<ICallWrapper *>::iterator i;
	for (i = g_CallWrappers.begin(); i != g_CallWrappers.end(); i++)
	{
		(*i)->Destroy();
	}
	
	// Clear array
	g_CallWrappers.clear();
}

/**
 * @brief Removes all items from the entity.
 *
 * @param pEntity	The entity index.
 * @param pSuit 	If true, will remove suit.
 **/
void UTIL_RemoveAllItems(CBaseEntity *pEntity, bool pSuit)
{
	// Set value to signature
	SET_WRAPPER(pEntity, RemoveAllItems, bool, pSuit, *(bool *));
}

/**
 * @brief Sets model to the entity.
 *
 * @param pEntity	The entity index.
 * @param sModel	The path to the model.
 **/
void UTIL_SetModel(CBaseEntity *pEntity, char *sModel)
{
	// Set value to signature
	SET_WRAPPER(pEntity, SetEntityModel, void *, sModel, *(char **));
}

/**
 * @brief Sets team to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetTeam(CBaseEntity *pEntity, int iValue)
{
	// Set value to signature
	SET_WRAPPER(pEntity, SwitchTeam, int, iValue, *(int *));
}

/**
 * @brief Respawn the entity into the world.
 *
 * @param pEntity	The entity index.
 **/
void UTIL_Respawn(CBaseEntity *pEntity)
{
	// Set value to signature
	SET_WRAPPER_NULL(pEntity, RoundRespawn);
}

/**
 * @brief Sets health to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetHealth(CBaseEntity *pEntity, int iValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_iHealth, int, iValue);
}

/**
 * @brief Sets armor to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetArmor(CBaseEntity *pEntity, int iValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_ArmorValue, int, iValue);
}

/**
 * @brief Sets speed to the entity.
 *
 * @param pEntity	The entity index.
 * @param flValue	The value to apply.
 **/
void UTIL_SetSpeed(CBaseEntity *pEntity, float flValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_flLaggedMovementValue, float, flValue);
}

/**
 * @brief Sets frags to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetFrags(CBaseEntity *pEntity, int iValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_iFrags, int, iValue);
}

/**
 * @brief Sets deaths to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetDeaths(CBaseEntity *pEntity, int iValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_iDeaths, int, iValue);
}

/**
 * @brief Sets night vision to the entity.
 *
 * @param pEntity	The entity index.
 * @param bValue	The value to apply.
 **/
void UTIL_SetNightVision(CBaseEntity *pEntity, bool bValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_bNightVisionOn, bool, bValue);
}

/**
 * @brief Sets fov to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetFov(CBaseEntity *pEntity, int iValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_iFOV, int, iValue);
	SET_OFFSET(pEntity, m_iDefaultFOV, int, iValue);
}

/**
 * @brief Sets sensor time to the entity.
 *
 * @param pEntity	The entity index.
 * @param flValue	The value to apply.
 **/
void UTIL_SetSensorTime(CBaseEntity *pEntity, float flValue)
{
	// Set value to memory
	SET_OFFSET(pEntity, m_flDetectedByEnemySensorTime, float, flValue);
}

/**
 * @brief Sets gravity to the entity.
 *
 * @param pEntity	The entity index.
 * @param flValue	The value to apply.
 **/
void UTIL_SetGravity(CBaseEntity *pEntity, float flValue)
{
	// Set value to memory
	SET_OFFSET_DMAP(pEntity, m_flGravity, float, flValue);
}

/**
 * @brief Turn the client into the zombie.
 *
 * @param clientIndex	The client index.
 * @return				True on success, false otherwise.
 **/
bool UTIL_InfectPlayer(int clientIndex)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(clientIndex);
	if (!CPlayer || !CPlayer->pEntity)
	{
		return false;
	}
	
	// If player is dead, then stop
	if(!IsPlayerAlive(CPlayer))
	{
		return false;
	}
	
	// Validate zombie 
	if(CPlayer->m_bZombie)
	{
		return false;
	}
	
	// If player is last human, then stop
	if(UTIL_GetHumans() <= 1)
	{
		return false;
	}
	
	// Remove all weapons
	UTIL_RemoveAllItems(CPlayer->pEntity, true);
	
	// Set class variable
	CPlayer->m_bZombie = true;
	
	// Set client properties
	UTIL_SetHealth(CPlayer->pEntity,  		ZombieGetHealth(CPlayer->m_nZombieClass) + ((UTIL_GetZombies() <= 1) ? GetZombieExtraHealth() : 0);
	UTIL_SetSpeed(CPlayer->pEntity,   	 	ZombieGetSpeed(CPlayer->m_nZombieClass));
	UTIL_SetGravity(CPlayer->pEntity, 		ZombieGetGravity(CPlayer->m_nZombieClass));
	UTIL_SetArmor(CPlayer->pEntity,  	 	0);
	UTIL_SetFov(CPlayer->pEntity,     		GetZombieFov());
	if(GetZombieNvg())  UTIL_SetNightVision(CPlayer->pEntity, 	true);
	if(GetZombieXray()) UTIL_SetSensorTime(CPlayer->pEntity, 	0.0);
	
	// Set model to the client
	char sModel[512];
	ZombieGetModel(CPlayer->m_nZombieClass, sModel);
	UTIL_SetModel(CPlayer->pEntity, sModel);
	
	// Force to swith team
	UTIL_SetTeam(CPlayer->pEntity, ZM_TEAM_T);
	
	// Increment deaths
	UTIL_SetDeaths(CPlayer->pEntity, GetPlayerDeathCount(CPlayer) + 1);
	
	// Call forward
	ForwardOnClientInfect(clientIndex);
	
	// Return on success
	return true;
}

/**
 * @brief Turn the client into the human.
 *
 * @param clientIndex	The client index.
 * @return				True on success, false otherwise.
 **/
bool UTIL_HumanizePlayer(int clientIndex)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(clientIndex);
	if (!CPlayer || !CPlayer->pEntity)
	{
		return false;
	}
	
	// If player is dead, then stop
	if(!IsPlayerAlive(CPlayer))
	{
		return false;
	}
	
	// If player didn't spawn, then humanize him
	if(g_IsOnRound)
	{
		// Validate human 
		if(!CPlayer->m_bZombie)
		{
			return false;
		}
		
		// If player is last zombie, then stop
		if(UTIL_GetZombies() <= 1)
		{
			return false;
		}
		
		// Remove all weapons
		UTIL_RemoveAllItems(CPlayer->pEntity, true);
		
		// Force to swith team
		UTIL_SetTeam(CPlayer->pEntity, ZM_TEAM_CT);
	}
	
	// Set class variable
	CPlayer->m_bZombie = false;
	
	// Set client properties
	UTIL_SetHealth(CPlayer->pEntity,  		HumanGetHealth(CPlayer->m_nHumanClass));
	UTIL_SetSpeed(CPlayer->pEntity,   		HumanGetSpeed(CPlayer->m_nHumanClass));
	UTIL_SetGravity(CPlayer->pEntity, 		HumanGetGravity(CPlayer->m_nHumanClass));
	UTIL_SetArmor(CPlayer->pEntity,   		HumanGetArmor(CPlayer->m_nHumanClass));
	UTIL_SetFov(CPlayer->pEntity,    	 	90);
	if(GetZombieNvg())  UTIL_SetNightVision(CPlayer->pEntity, 	false);
	if(GetZombieXray()) UTIL_SetSensorTime(CPlayer->pEntity, 	g_IsOnRound ? (globals->curtime + 9999.0) : 0.0);
	
	// Set model to the client
	char sModel[512];
	HumanGetModel(CPlayer->m_nHumanClass, sModel);
	UTIL_SetModel(CPlayer->pEntity, sModel);
	
	// Call forward
	ForwardOnClientHumanize(clientIndex, g_IsOnRound ? true : false);
	
	// Return on success
	return true;
}

/**
 * @brief Move all clients to random teams.
 **/
void UTIL_BalanceTeams()
{
	// i = client index
	for (int i = 1; i <= globals->maxClients; i++)
	{
		// Validate client
		CBaseClient *CPlayer = GetPlayer(i);
		if (!CPlayer || !CPlayer->pEntity)
		{
			continue;
		}
		
		// Switch team
		UTIL_SetTeam(CPlayer->pEntity, !(i % 2) ? ZM_TEAM_T : ZM_TEAM_CT);
	}
}

/**
 * @brief Gets amount of total humans.
 * 
 * @return	 		The amount of total humans.
 **/
unsigned int UTIL_GetHumans()
{
	// Initialize variable
	unsigned int nHumans = 0;
	
	// i = client index
	for (int i = 1; i <= globals->maxClients; i++)
	{
		// Validate client
		CBaseClient *CPlayer = GetPlayer(i);
		if (!CPlayer || CPlayer->m_bZombie)
		{
			continue;
		}
		
		// If player is dead, then skip
		if(!IsPlayerAlive(CPlayer))
		{
			continue;
		}
		
		// Increment amount
		nHumans++;
	}
	
	// Return amount
	return nHumans;
}

/**
 * @brief Gets amount of total zombies.
 *
 * @return	 		The amount of total zombies.
 **/
unsigned int UTIL_GetZombies()
{
	// Initialize variable
	unsigned int nZombies = 0;
	
	// i = client index
	for (int i = 1; i <= globals->maxClients; i++)
	{
		// Validate client
		CBaseClient *CPlayer = GetPlayer(i);
		if (!CPlayer || !CPlayer->m_bZombie)
		{
			continue;
		}
		
		// If player is dead, then skip
		if(!IsPlayerAlive(CPlayer))
		{
			continue;
		}
		
		// Increment amount	
		nZombies++;
	}
	
	// Return amount
	return nZombies;
}

/**
 * @brief Gets amount of total alive players.
 *
 * @return	 		The amount of total alive players.
 **/
unsigned int UTIL_GetAlive()
{
	// Initialize variable
	unsigned int nAlive = 0;

	// i = client index
	for (int i = 1; i <= globals->maxClients; i++)
	{
		// Validate client
		CBaseClient *CPlayer = GetPlayer(i);
		if (!CPlayer)
		{
			continue;
		}
		
		// If player is dead, then skip
		if(!IsPlayerAlive(CPlayer))
		{
			continue;
		}
		
		// Increment amount
		nAlive++;
	}
	
	// Return amount
	return nAlive;
}

/**
 * @brief Gets index of the random player.
 *
 * @param nRandom	The random number.
 * 
 * @return			The index of random player.
 **/
unsigned int UTIL_GetRandomAlive(unsigned int nRandom)
{
	// Initialize variable
	unsigned int nAlive = 0;
	
	// i = client index
	for (int i = 1; i <= globals->maxClients; i++)
	{
		// Validate client
		CBaseClient *CPlayer = GetPlayer(i);
		if (!CPlayer)
		{
			continue;
		}
		
		// If player is dead, then skip
		if(!IsPlayerAlive(CPlayer))
		{
			continue;
		}
		
		// Increment amount
		nAlive++;
		
		// If random number is equal, so return index
		if (nAlive == nRandom)
		{
			return i;
		}
	}
	
	// Return error
	return -1;
}

/**
 * @brief Gets amount of total playing players.
 *
 * @return		The amount of total playing players.
 **/
unsigned int UTIL_GetPlaying()
{
	// Initialize variable
	unsigned int nPlaying = 0;

	// i = client index
	for (int i = 1; i <= globals->maxClients; i++)
	{
		// Validate client
		CBaseClient *CPlayer = GetPlayer(i);
		if (!CPlayer)
		{
			continue;
		}
		
		// Increment amount
		nPlaying++;
	}
	
	// Return amount
	return nPlaying;
}