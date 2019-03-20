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
 
#ifndef _INCLUDE_SOURCEMOD_EXTENSION_UTILS_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_UTILS_H_
 
/**
 * @file utils.h
 * @brief Utils module code header.
 */


/**
 * @brief Initialize utils.
 */
void UtilInit();

/**
 * @brief Close offsets config.
 */
void UtilPurge();

/**
 * @brief Destroy utils during inferface dropping.
 */
void UtilInterfaceDrop();

/**
 * @brief Removes all items from the entity.
 *
 * @param pEntity	The entity index.
 * @param pSuit 	If true, will remove suit.
 **/
void UTIL_RemoveAllItems(CBaseEntity *pEntity, bool pSuit);

/**
 * @brief Sets model to the entity.
 *
 * @param pEntity	The entity index.
 * @param sModel	The path to the model.
 **/
void UTIL_SetModel(CBaseEntity *pEntity, char *sModel);

/**
 * @brief Sets team to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetTeam(CBaseEntity *pEntity, int iValue);

/**
 * @brief Respawn the entity into the world.
 *
 * @param pEntity	The entity index.
 **/
void UTIL_Respawn(CBaseEntity *pEntity);

/**
 * @brief Sets health to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetHealth(CBaseEntity *pEntity, int iValue);

/**
 * @brief Sets armor to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetArmor(CBaseEntity *pEntity, int iValue);

/**
 * @brief Sets speed to the entity.
 *
 * @param pEntity	The entity index.
 * @param flValue	The value to apply.
 **/
void UTIL_SetSpeed(CBaseEntity *pEntity, float flValue);

/**
 * @brief Sets frags to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetFrags(CBaseEntity *pEntity, int iValue);
/**
 * @brief Sets deaths to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetDeaths(CBaseEntity *pEntity, int iValue);

/**
 * @brief Sets night vision to the entity.
 *
 * @param pEntity	The entity index.
 * @param bValue	The value to apply.
 **/
void UTIL_SetNightVision(CBaseEntity *pEntity, bool bValue);

/**
 * @brief Sets fov to the entity.
 *
 * @param pEntity	The entity index.
 * @param iValue	The value to apply.
 **/
void UTIL_SetFov(CBaseEntity *pEntity, int iValue);

/**
 * @brief Sets sensor time to the entity.
 *
 * @param pEntity	The entity index.
 * @param flValue	The value to apply.
 **/
void UTIL_SetSensorTime(CBaseEntity *pEntity, float flValue);

/**
 * @brief Sets gravity to the entity.
 *
 * @param pEntity	The entity index.
 * @param flValue	The value to apply.
 **/
void UTIL_SetGravity(CBaseEntity *pEntity, float flValue);

/**
 * @brief Turn the client into the zombie.
 *
 * @param clientIndex	The client index.
 * @return				True on success, false otherwise.
 **/
bool UTIL_InfectPlayer(int clientIndex);

/**
 * @brief Turn the client into the human.
 *
 * @param clientIndex	The client index.
 * @return				True on success, false otherwise.
 **/
bool UTIL_HumanizePlayer(int clientIndex);

/**
 * @brief Move all clients to random teams.
 **/
void UTIL_BalanceTeams();

/**
 * @brief Gets amount of total humans.
 * 
 * @return	 		The amount of total humans.
 **/
unsigned int UTIL_GetHumans();

/**
 * @brief Gets amount of total zombies.
 *
 * @return	 		The amount of total zombies.
 **/
unsigned int UTIL_GetZombies();

/**
 * @brief Get amount of total alive players.
 *
 * @return	 		The amount of total alive players.
 **/
unsigned int UTIL_GetAlive();

/**
 * @brief Gets index of the random player.
 *
 * @param nRandom	The random number.
 * 
 * @return			The index of random player.
 **/
unsigned int UTIL_GetRandomAlive(unsigned int nRandom);

/**
 * @brief Gets amount of total playing players.
 *
 * @return		The amount of total playing players.
 **/
unsigned int UTIL_GetPlaying();

/**
 * @brief Wrapper macroses.
 */
#define DECLARE_WRAPPER(name) \
	ICallWrapper *g_p##name = NULL;
 
#define INITIALIZE_WRAPPER_ADDR(name, request, code) \
	offset = 0; \
	if (!g_pZombieConfig->GetOffset(#name, &offset) || !offset) {\
		g_SMAPI->ConPrintf("Failed to get memory offset for \"%s\"\n", #name); \
		return; \
	}\
	code; \
	g_p##name = request; \
	g_CallWrappers.push_back(g_p##name); \

#define INITIALIZE_WRAPPER_SIG(name, request, code) \
	addr = NULL; \
	if (!g_pZombieConfig->GetMemSig(#name, &addr) || !addr) { \
		g_SMAPI->ConPrintf("Failed to get memory signature for \"%s\"\n", #name); \
		return; \
	} \
	code; \
	g_p##name = request; \
	g_CallWrappers.push_back(g_p##name); \
	
#define SET_WRAPPER(entity, name, lenght, value, pointer) \
	unsigned char vstk[sizeof(CBaseEntity *) + sizeof(lenght)]; \
	unsigned char *vptr = vstk; \
	*(CBaseEntity **)vptr = entity; \
	vptr += sizeof(CBaseEntity *); \
	pointer vptr = value; \
	g_p##name->Execute(vstk, NULL); 

#define SET_WRAPPER_NULL(entity, name) \
	g_p##name->Execute(&entity, NULL); 

/**
 * @brief Offsets macroses.
 */	
#define DECLARE_OFFSET(name) \
	unsigned int name = 0;
	
#define INITIALIZE_OFFSET(type, name) \
	if(!gamehelpers->FindSendPropInfo(#type, #name, &prop)) {\
		g_SMAPI->ConPrintf("Failed to get prop info for \"%s\"\n", #name); \
		return; \
	}\
	name = prop.actual_offset;

#define SET_OFFSET(entity, name, type, value) \
	unsigned char *ptr = (unsigned char *)(entity) + name; \
	(*(type *)ptr = value); \
	edict_t *pEdict = gameents->BaseEntityToEdict(entity); \
	gamehelpers->SetEdictStateChanged(pEdict, name);

#define SET_OFFSET_DMAP(entity, name, type, value) \
	if (!name) \
	{\
		GetDataMapOffset(entity, #name, name); \
	}\
	*(type *)((char *)entity + name) = value;

#endif // _INCLUDE_SOURCEMOD_EXTENSION_UTILS_H_