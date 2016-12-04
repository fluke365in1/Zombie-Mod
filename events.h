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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_EVENTS_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_EVENTS_H_
 
/**
 * @file events.cpp
 * @brief Events code header.
 */
 
#include <igameevents.h>

/* Extern round variables */
extern bool g_IsNewRound;
extern bool g_IsEndRound;
extern bool g_IsOnRound;
extern unsigned int g_nCounter;

/**
 * @brief Event callbacks for when a main game timer is executed.
 */
class EventsCounter : public ITimedEvent
{
public:

	/**
	 * @brief Called when a timer is executed.
	 *
	 * @param pTimer		Pointer to the timer instance.
	 * @param pData			Private pointer passed from host.
	 * @return				Pl_Stop to stop timer, Pl_Continue to continue.
	 */
	ResultType OnTimer(ITimer *pTimer, void *pData)
	{
		// If round didn't start yet
		if(g_IsNewRound)
		{
			// If counter is counting ?
			if(g_nCounter)
			{
				// Debug
				g_SMAPI->ConPrintf("$$$$$$$$$$$$======%d\n", g_nCounter);
			}
				
			// Substitute second
			g_nCounter--;
		}
		
		// Allow counter
		return Pl_Continue;
	}

	/**
	 * @brief Called when the timer has been killed.
	 *
	 * @param pTimer		Pointer to the timer instance.
	 * @param pData			Private data pointer passed from host.
	 */
	void OnTimerEnd(ITimer *pTimer, void *pData)
	{
		/* empty statement */
	}
	
} s_EventsCounter;

#endif // _INCLUDE_SOURCEMOD_EXTENSION_EVENTS_H_