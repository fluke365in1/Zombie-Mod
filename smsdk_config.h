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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

/**
 * @file smsdk_config.h
 * @brief Contains macros for configuring basic extension information.
 */

/* Basic information exposed publicly */
#define SMEXT_CONF_NAME			"Zombie Mod"
#define SMEXT_CONF_DESCRIPTION	"Game modification"
#define SMEXT_CONF_VERSION		"1.0.0"
#define SMEXT_CONF_AUTHOR		"Copyright (C) 2015-2016 Nikita Ushakov (Ireland, Dublin)"
#define SMEXT_CONF_URL			"https://forums.alliedmods.net/showthread.php?t=290657"
#define SMEXT_CONF_LOGTAG		"ZOMBIEMOD"
#define SMEXT_CONF_LICENSE		"GNU GPL, Version 3"
#define SMEXT_CONF_DATESTRING	__DATE__

/** 
 * @brief Important macroses.
 */
#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

#define VERIFY_SIGNATURE(name) \
	if (!g_pDmConf->GetMemSig(name, &addr) || !addr) { \
		return; \
	}

#define VERIFY_OFFSET(name) \
	if (!g_pDmConf->GetOffset(name, &offset) || !offset) { \
		return; \
	}

#define HOOK_EVENT2(name) \
    if (!gameevents->AddListener(&g_cls_event_##name, #name, true)) { \
        return; \
    }

#define UNHOOK_EVENT2(name) gameevents->RemoveListener(&g_cls_event_##name);

#define DECLARE_EVENT(name) \
    class cls_event_##name : public IGameEventListener2 \
    { \
    public: \
        virtual void FireGameEvent(IGameEvent *event); \
        virtual int  GetEventDebugID( void ) { return EVENT_DEBUG_ID_INIT; } \
    }; \
    extern cls_event_##name g_cls_event_##name;

#define IMPLEMENT_EVENT(name) \
    cls_event_##name g_cls_event_##name; \
    void cls_event_##name::FireGameEvent(IGameEvent *event)

#define SMALL_LINE_LENGTH 	32
#define NORMAL_LINE_LENGTH 	64
#define BIG_LINE_LENGTH 	128
#define HUGE_LINE_LENGTH	512

#define MAXPLAYERS			64
	
/**
 * @brief Sets whether or not this plugin required Metamod.
 * NOTE: Uncomment to enable, comment to disable.
 */
#define SMEXT_CONF_METAMOD		

/** Enable interfaces you want to use here by uncommenting lines */
#define SMEXT_ENABLE_FORWARDSYS
//#define SMEXT_ENABLE_HANDLESYS
#define SMEXT_ENABLE_PLAYERHELPERS
//#define SMEXT_ENABLE_DBMANAGER
#define SMEXT_ENABLE_GAMECONF
//#define SMEXT_ENABLE_MEMUTILS
#define SMEXT_ENABLE_GAMEHELPERS
#define SMEXT_ENABLE_TIMERSYS
//#define SMEXT_ENABLE_THREADER
//#define SMEXT_ENABLE_LIBSYS
//#define SMEXT_ENABLE_MENUS
//#define SMEXT_ENABLE_ADTFACTORY
//#define SMEXT_ENABLE_PLUGINSYS
//#define SMEXT_ENABLE_ADMINSYS
#define SMEXT_ENABLE_TEXTPARSERS
//#define SMEXT_ENABLE_USERMSGS
//#define SMEXT_ENABLE_TRANSLATOR
//#define SMEXT_ENABLE_ROOTCONSOLEMENU

#endif // _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
