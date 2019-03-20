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
 
#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PLAYERTOOLS_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PLAYERTOOLS_H_
 
/**
 * @file playertools.h
 * @brief Player tools module code header.
 */

 
/**
 * @brief Initialize clients listeners.
 */
void ToolInit();
 
/**
 * @brief Destroy clients listeners.
 */
void ToolPurge();


 
/**
 * List of the client data array.
 **/
struct CBaseClient
{
	edict_t *pEdict;
	CBaseEntity *pEntity;
	IPlayerInfo *pInfo;
	IBotController *pBot;
	int userid;
	bool m_bZombie;
	unsigned int m_nZombieClass;
	unsigned int m_nZombieClassNext;
	unsigned int m_nHumanClass;
	unsigned int m_nHumanClassNext;
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

/**
 * @brief Returns if the client is alive or dead.
 *
 * @param CPlayer		The client data.
 * @return				True if the client is alive, false otherwise.
 **/
bool IsPlayerAlive(CBaseClient *CPlayer);

/**
 * @brief Retrieves a client's team index.
 *
 * @param CPlayer		The client data.
 * @return				Team index the client is on (mod specific).			
 **/
int GetPlayerTeamIndex(CBaseClient *CPlayer);

/**
 * @brief Returns the client's frag count.
 *
 * @param CPlayer		The client data.
 * @return				The frag count.	
 **/
int GetPlayerFragCount(CBaseClient *CPlayer);

/**
 * @brief Returns the client's death count.
 *
 * @param CPlayer		The client data.
 * @return				The death count.	
 **/
int GetPlayerDeathCount(CBaseClient *CPlayer);

/**
 * @brief Returns the client's health.
 *
 * @param CPlayer		The client data.
 * @return				The health value.
 **/
int GetPlayerHealthValue(CBaseClient *CPlayer);

/**
 * @brief Returns the client's armor.
 *
 * @param CPlayer		The client data.
 * @return				The armor value.
 **/
int GetPlayerArmorValue(CBaseClient *CPlayer);

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PLAYERTOOLS_H_