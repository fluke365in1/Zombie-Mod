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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_EVENTS_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_EVENTS_H_
 
/**
 * @file events.h
 * @brief Events module code header.
 */
 
/**
 * @brief Hook events.
 */
void EventInit();

/**
 * @brief Unhook events.
 */
void EventPurge();

/**
 * @brief Map start hook events.
 */
void EventActivated();

/**
 * @brief Called each timer execution. (every second)
 */
void EventTimer();

/**
 * @brief Called right before mode is started.
 **/
void EventStartMode();

/**
 * @brief Event callbacks for the main game timer is executed.
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
		// Forward function to modules
		EventTimer();
		
		// Return infinitly
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

/**
 * @brief Event callbacks for the main game timer is executed.
 */
class EventsInitialize : public ITimedEvent
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
		// Return infinitly
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
		// If round started, then infect
		if(g_IsOnRound)
		{
			// Infect respawned players
			UTIL_InfectPlayer((int)pData);
		}
		else
		{
			// Humanize respawned players
			UTIL_HumanizePlayer((int)pData);
		}
	}
	
} s_EventsInitialize;

/**
 * @brief Important macroses.
 */
#define HOOK_EVENT2(name) \
	if (!gameevents->FindListener(&g_cls_event_##name, #name)) \
	{\
		if (!gameevents->AddListener(&g_cls_event_##name, #name, true)) { \
			g_SMAPI->ConPrintf("Could not hook event \"%s\"\n", #name); \
			return; \
		} \
	} 

#define UNHOOK_EVENT2(name) gameevents->RemoveListener(&g_cls_event_##name);

#define DECLARE_EVENT(name) \
    class cls_event_##name : public IGameEventListener2 \
    { \
    public: \
        virtual void FireGameEvent(IGameEvent *event); \
        virtual int GetEventDebugID( void ) { return EVENT_DEBUG_ID_INIT; } \
    }; \
    extern cls_event_##name g_cls_event_##name;
	
#define IMPLEMENT_EVENT(name) \
    cls_event_##name g_cls_event_##name; \
    void cls_event_##name::FireGameEvent(IGameEvent *event)

#endif // _INCLUDE_SOURCEMOD_EXTENSION_EVENTS_H_