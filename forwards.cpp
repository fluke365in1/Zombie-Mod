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
 
/*
 * Application Programming Interface (API)
 * 
 * To allow other plugins or extensions to interact directly with Zombie Mod Mod we need to implement
 * an API.  SourceMod allows us to do this by creating global "natives" or "forwards."
 * 
 * To better understand how natives and forwards are created, go here:
 * http://wiki.alliedmods.net/Creating_Natives_(SourceMod_Scripting)
 * http://wiki.alliedmods.net/Function_Calling_API_(SourceMod_Scripting) 
 */
 
/**
 * @file forwards.cpp
 * @brief Forwards module code file.
 */
 
#include "forwards.h"


/* Forwards list */
IForward *g_pOnClientInfect = NULL;
IForward *g_pOnClientHuminize = NULL;
IForward *g_pOnZombieModStarted = NULL;

/**
 * @brief Initialize forwards.
 */
void ForwardLoad()
{
	// Register forwards
	g_pOnClientInfect = forwards->CreateForward("ZM_OnClientInfected", ET_Ignore, 1, NULL, Param_Cell);
	g_pOnClientHuminize = forwards->CreateForward("ZM_OnClientHuminized", ET_Ignore, 2, NULL, Param_Cell, Param_Cell);
	g_pOnZombieModStarted = forwards->CreateForward("ZM_OnZombieModStarted", ET_Ignore, 0, NULL);
}

/**
 * @brief Reset forwards.
 */
void ForwardPurge()
{
	// Destroy forwards
	forwards->ReleaseForward(g_pOnClientInfect);
	forwards->ReleaseForward(g_pOnClientHuminize);
	forwards->ReleaseForward(g_pOnZombieModStarted);
}

/**
 * @brief Call infect forward.
 *
 * @param clientIndex 	The client index.
 */
void ForwardOnClientInfect(int clientIndex)
{
	g_pOnClientInfect->PushCell(clientIndex);
	g_pOnClientInfect->Execute(NULL);
}

/**
 * @brief Call humanize forward.
 *
 * @param clientIndex 	The client index.
 * @param antidotUse	False on the spawn, true otherelse.
 */
void ForwardOnClientHuminize(int clientIndex, bool antidotUse)
{
	g_pOnClientHuminize->PushCell(clientIndex);
	g_pOnClientHuminize->PushCell(antidotUse);
	g_pOnClientHuminize->Execute(NULL);
}

/**
 * @brief Call round start forward.
 */
void ForwardOnZombieModStarted()
{
	g_pOnZombieModStarted->Execute(NULL);
}