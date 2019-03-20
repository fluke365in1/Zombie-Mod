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
 * @file events.cpp
 * @brief Events module code file.
 */

#include "events.h"
 

/* Global hooks */
DECLARE_EVENT(round_prestart);
DECLARE_EVENT(round_end);
DECLARE_EVENT(player_spawn);
DECLARE_EVENT(player_hurt);
DECLARE_EVENT(player_death);

/* Global round variables */
bool g_IsNewRound;
bool g_IsEndRound;
bool g_IsOnRound;
unsigned int g_nCounter;

/**
 * @brief Hook events.
 */
void EventInit()
{
	// Initialize hooks
	HOOK_EVENT2(round_prestart);
	HOOK_EVENT2(round_end);
	HOOK_EVENT2(player_spawn);
	HOOK_EVENT2(player_hurt);
	HOOK_EVENT2(player_death);
}

/**
 * @brief Unhook events.
 */
void EventPurge()
{
	// Destroy hooks
	UNHOOK_EVENT2(round_prestart);
	UNHOOK_EVENT2(round_end);
	UNHOOK_EVENT2(player_spawn);
	UNHOOK_EVENT2(player_hurt);
	UNHOOK_EVENT2(player_death);
}

/**
 * @brief Map start hook events.
 */
void EventActivated()
{
	// Create timer for starting game mode
	timersys->CreateTimer(&s_EventsCounter, 1.0, 0, TIMER_FLAG_REPEAT | TIMER_FLAG_NO_MAPCHANGE);
}

/**
 * @brief Called each timer execution. (every second)
 */
void EventTimer()
{
	// If round didn't start yet
	if(g_IsNewRound)
	{
		// Get amount of total alive players
		unsigned int nAlive = UTIL_GetAlive();

		// Switch amount of alive players
		switch(nAlive)
		{
			// No players
			case 0 : break;	
				
			// Wait other players
			case 1 : break;
			
			// If players exists
			default : 							
			{
				// If counter stop counting ?
				if(!g_nCounter)
				{
					// Start!
					EventStartMode();
					
					// Go!
					//UTIL_PrintHintTextAll("Round started");
				}

				// Substitute second
				g_nCounter--;
				break;
			}
		}
	}
}

/**
 * @brief Called right before mode is started.
 **/
void EventStartMode()
{
	// Get amount of total alive players
	unsigned int nAlive = UTIL_GetAlive();
	
	// Initialize max amount of zombies 
	unsigned int nMaxZombies = ceil(nAlive * GetInfectRatio());

	// Reset server grobal bools
	g_IsNewRound = false;
	g_IsEndRound = false;
	g_IsOnRound = true;

	// Randomly turn players into zombies
	unsigned int nZombies = 0;
	while (nZombies < nMaxZombies)
	{
		// Get random client index
		int clientIndex = UTIL_GetRandomAlive(::RandomInt(1, nAlive));
		
		// Validate random client
		CBaseClient *CPlayer = GetPlayer(clientIndex);
		if (!CPlayer || !CPlayer->pEntity || CPlayer->m_bZombie)
		{
			continue;
		}
		
		// If player is dead, then skip
		if(!IsPlayerAlive(CPlayer))
		{
			continue;
		}
		
		// Make a zombie
		UTIL_InfectPlayer(clientIndex);
		
		// Increment zombie count
		nZombies++;
	}
	
	// Remaining players should be humans
	// i = client index
	for (int i = 1; i <= globals->maxClients; i++)
	{
		// Validate client
		CBaseClient *CPlayer = GetPlayer(i);
		if (!CPlayer || !CPlayer->pEntity || CPlayer->m_bZombie)
		{
			continue;
		}
		
		// If player is dead, then skip
		if(!IsPlayerAlive(CPlayer))
		{
			continue;
		}
		
		// Switch team
		UTIL_SetTeam(CPlayer->pEntity, ZM_TEAM_CT);
		
		// Create sensor target
		if(GetZombieXray()) UTIL_SetSensorTime(CPlayer->pEntity, globals->curtime + 9999.0);
	}
	
	// Call forward
	ForwardOnZombieModStarted();
}

/**
 * @brief Event callback (round_prestart).
 */
IMPLEMENT_EVENT(round_prestart)
{
    // Reset server grobal bools
    g_IsNewRound = true;
    g_IsEndRound = false;
    g_IsOnRound  = false;
	
	// Restore default time for main event timer
	g_nCounter = GetGameDelay();
}

/**
 * @brief Event callback (round_end).
 */
IMPLEMENT_EVENT(round_end)
{
    // Reset server grobal bools
	g_IsNewRound = false;
	g_IsEndRound = true;
    g_IsOnRound  = false;
	
	// Move all clients to random teams.
	UTIL_BalanceTeams();
}

/**
 * @brief Event callback (player_spawn).
 */
IMPLEMENT_EVENT(player_spawn)
{			
	// Get the index from the key
	int clientIndex = playerhelpers->GetClientOfUserId(event->GetInt("userid"));
	
	// Validate client
	CBaseClient *CPlayer = GetPlayer(clientIndex);
	if (!CPlayer || !CPlayer->pEntity)
	{
		return;
	}

	// If player is dead, then stop
	if(!IsPlayerAlive(CPlayer))
	{
		return;
	}
	
	// Update variables
	CPlayer->m_bZombie = false;
	CPlayer->m_nZombieClass = CPlayer->m_nZombieClassNext;
	CPlayer->m_nHumanClass = CPlayer->m_nHumanClassNext;
	
	// Create timer for starting spawn initialization
	timersys->CreateTimer(&s_EventsInitialize, 0.1, (void *)clientIndex, TIMER_FLAG_NO_MAPCHANGE);
}

/**
 * @brief Event callback (player_hurt).
 */
IMPLEMENT_EVENT(player_hurt)
{
	// Get the index from the key
	int attackerIndex = playerhelpers->GetClientOfUserId(event->GetInt("attacker"));
	
	// Validate client
	CBaseClient *CPlayer = GetPlayer(attackerIndex);
	if (!CPlayer || !CPlayer->pEntity)
	{
		return;
	}

	// If player is dead, then stop
	if(!IsPlayerAlive(CPlayer))
	{
		return;
	}
	
	// Verify that the attacker is zombie 
	if(CPlayer->m_bZombie) 
	{
		// Infect victim
		if(UTIL_InfectPlayer(playerhelpers->GetClientOfUserId(event->GetInt("userid"))))
		{
			// Increment kills
			UTIL_SetFrags(CPlayer->pEntity, GetPlayerFragCount(CPlayer) + 1);
		}
	}
}

/**
 * @brief Event callback (player_death).
 */
IMPLEMENT_EVENT(player_death)
{			
	// Get the index from the key
	int victimIndex = playerhelpers->GetClientOfUserId(event->GetInt("userid"));
	
	// Validate client
	CBaseClient *CPlayer = GetPlayer(victimIndex);
	if (!CPlayer || !CPlayer->pEntity)
	{
		return;
	}

	// If player is alive, then stop
	if(IsPlayerAlive(CPlayer))
	{
		return;
	}
	
	// If player was killed by world, respawn on suicide?
	if (!AllowRespawnOnSuicide() && victimIndex == playerhelpers->GetClientOfUserId(event->GetInt("attacker")))
	{
		return;
	}
	
	// Respawn if human/zombie
	if ((CPlayer->m_bZombie && !AllowRespawnZombies()) || (!CPlayer->m_bZombie && !AllowRespawnHuman()))
	{
		return;
	}

	// Create timer for starting respawn
	//timersys->CreateTimer(&s_EventsRespawn, GetRespawnTime(), (void *)victimIndex, TIMER_FLAG_NO_MAPCHANGE);
}