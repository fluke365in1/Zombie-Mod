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
 * @file natives.cpp
 * @brief Natives module code file.
 */
 
#include "natives.h"


/* Initialize zombie classes data array */
ZombieClassData g_ZombieClassData[MAX_CLASSES];
unsigned int g_ZombieClass;

/* Initialize human classes data array */
HumanClassData g_HumanClassData[MAX_CLASSES];
unsigned int g_HumanClass;

/**
 * @brief Initialize natives and library.
 */
void NativeLoad()
{
	// Register library
	sharesys->RegisterLibrary(myself, "zombiemod");
	
	// Register natives
	sharesys->AddNatives(myself, g_ZombieModNatives);
}

/**
 * @brief Reset natives.
 */
void NaviteShutDown()
{
	// Reset all classes
	g_ZombieClass = 0;
	g_HumanClass = 0;
}

//*********************************************************************
//*                  MAIN NATIVE CELLS FUNCTIONS           	  	  	  *
//*********************************************************************

/**
 * @brief Gets the zombie round started.
 */
static cell_t API_IsRoundStarted(IPluginContext *pContext, const cell_t *params)
{
	// Return the zombie round status
	return !(g_IsNewRound && g_IsEndRound) && g_IsOnRound;
}

/**
 * @brief Gets the zombie status of the client.
 */
static cell_t API_IsZombie(IPluginContext *pContext, const cell_t *params)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Return the zombie status
	return CPlayer->m_bZombie;
}

/**
 * @brief Turn the client into the zombie.
 */
static cell_t API_MakeZombie(IPluginContext *pContext, const cell_t *params)
{
	// Validate round
	if(g_IsNewRound || g_IsEndRound)
	{
		return pContext->ThrowNativeError("[ZM] Round didn't started or already finished");
	}
	
	
	// Validate client
	CBaseClient *CPlayer = GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Infect the client
	return UTIL_InfectPlayer(params[1]);
}

/**
 * @brief Turn the client into the human.
 */
static cell_t API_MakeHuman(IPluginContext *pContext, const cell_t *params)
{
	// Validate round
	if(g_IsNewRound || g_IsEndRound)
	{
		return pContext->ThrowNativeError("[ZM] Round didn't started or already finished");
	}
	
	// Validate client
	CBaseClient *CPlayer = GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Infect the client
	return UTIL_HumanizePlayer(params[1]);
}

//*********************************************************************
//*                  		ZOMBIE CLASS NATIVES           	  	  	  *
//*********************************************************************

/**
 * @brief Gets the amount of all zombie classes.
 */
static cell_t API_GetNumberZombieClass(IPluginContext *pContext, const cell_t *params)
{
	// Return the zombie classes number
	return g_ZombieClass;
}

/**
 * @brief Gets the zombie class index of the client.
 */
static cell_t API_GetClientZombieClass(IPluginContext *pContext, const cell_t *params)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Return the zombie class index
	return CPlayer->m_nZombieClass;
}

/**
 * @brief Sets the zombie class index to the client.
 */
static cell_t API_SetClientZombieClass(IPluginContext *pContext, const cell_t *params)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Set the zombie class index
	CPlayer->m_nZombieClassNext = params[2];
	
	// Return on success
	return 1;
}

/**
 * @brief Gets the name of the zombie class.
 */
static cell_t API_GetZombieClassName(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Initialize string
	char *sDest;
	
	// Send string to adress
	pContext->LocalToString(params[2], &sDest);
	strncopy(sDest, g_ZombieClassData[iD].ZombieClass_Name, params[3]);
		
	// Return on success
	return 1;
}

/**
 * @brief Gets the model path of the zombie class.
 */
static cell_t API_GetZombieClassModel(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}

	// Initialize string
	char *sDest;
	
	// Send string to adress
	pContext->LocalToString(params[2], &sDest);
	strncopy(sDest, g_ZombieClassData[iD].ZombieClass_Model, params[3]);

	// Return on success
	return 1;
}

/**
 * @brief Gets the claw model path of the zombie class.
 */
static cell_t API_GetZombieClassClaw(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Initialize string
	char *sDest;
	
	// Send string to adress
	pContext->LocalToString(params[2], &sDest);
	strncopy(sDest, g_ZombieClassData[iD].ZombieClass_Claw, params[3]);

	// Return on success
	return 1;
}

/**
 * @brief Gets the health of the zombie class.
 */
static cell_t API_GetZombieClassHealth(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return ZombieGetHealth(iD);
}

/**
 * @brief Gets the speed of the zombie class.
 */
static cell_t API_GetZombieClassSpeed(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return sp_ftoc(ZombieGetSpeed(iD));
}

/**
 * @brief Gets the gravity of the zombie class.
 */
static cell_t API_GetZombieClassGravity(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return sp_ftoc(ZombieGetGravity(iD));
}

/**
 * @brief Gets the knockback of the zombie class.
 */
static cell_t API_GetZombieClassKnockBack(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return sp_ftoc(ZombieGetKnockBack(iD));
}

/**
 * @brief Check the gender of the zombie class.
 */
static cell_t API_GetZombieClassGender(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return ZombieIsFemale(iD);
}

/**
 * @brief Gets the index of the zombie class player model.
 */
static cell_t API_GetZombieClassBodyID(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}

	// Return default restriction status
	return ZombieGetBodyIndex(iD);
}

/**
 * @brief Gets the index of the zombie class claw model.
 */
static cell_t API_GetZombieClassClawID(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_ZombieClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}

	// Return default restriction status
	return ZombieGetClawIndex(iD);
}

/**
 * @brief Load the zombie class from other sm plugin.
 */
static cell_t API_RegisterZombieClass(IPluginContext *pContext, const cell_t *params)
{
	// Maximum amout of zombie classes
	if(g_ZombieClass >= MAX_CLASSES)
	{
		return pContext->ThrowNativeError("[ZM] Maximum number of zombie classes reached (%d). Skipping other classes.", g_ZombieClass);
	}

	// Get main values from the native
	pContext->LocalToString(params[1], &g_ZombieClassData[g_ZombieClass].ZombieClass_Name);
	pContext->LocalToString(params[2], &g_ZombieClassData[g_ZombieClass].ZombieClass_Model);
	pContext->LocalToString(params[3], &g_ZombieClassData[g_ZombieClass].ZombieClass_Claw);
	
	// Strings are empty ?
	if (!(strlen(g_ZombieClassData[g_ZombieClass].ZombieClass_Name) || strlen(g_ZombieClassData[g_ZombieClass].ZombieClass_Model) || strlen(g_ZombieClassData[g_ZombieClass].ZombieClass_Claw)))
	{
		return pContext->ThrowNativeError("[ZM] Can't register zombie class (%d) with an empty name, model pathes", g_ZombieClass);
	}
	
	// Precache player models
	g_ZombieClassData[g_ZombieClass].ZombieClass_BodyID = engine->PrecacheModel(g_ZombieClassData[g_ZombieClass].ZombieClass_Model, true);
	if(!engine->IsModelPrecached(g_ZombieClassData[g_ZombieClass].ZombieClass_Model))
	{
		return pContext->ThrowNativeError("[ZM] Can't precache zombie class player model (%d)", g_ZombieClass);
	}
	
	// Precache claw models
	g_ZombieClassData[g_ZombieClass].ZombieClass_ClawID = engine->PrecacheModel(g_ZombieClassData[g_ZombieClass].ZombieClass_Claw, true);
	if(!engine->IsModelPrecached(g_ZombieClassData[g_ZombieClass].ZombieClass_Model))
	{
		return pContext->ThrowNativeError("[ZM] Can't register zombie class claw model (%d)", g_ZombieClass);
	}
	
	// Get other values from the native
	g_ZombieClassData[g_ZombieClass].ZombieClass_Health   	= params[4];
	g_ZombieClassData[g_ZombieClass].ZombieClass_Speed 		= sp_ctof(params[5]);
	g_ZombieClassData[g_ZombieClass].ZombieClass_Gravity  	= sp_ctof(params[6]);
	g_ZombieClassData[g_ZombieClass].ZombieClass_KnockBack 	= sp_ctof(params[7]);
	g_ZombieClassData[g_ZombieClass].ZombieClass_Female   	= params[8] ? true : false;
	
	// Increment zombie class index
	g_ZombieClass++;

	// Return id under which we registered the class
	return g_ZombieClass-1;
}


//*********************************************************************
//*                  		HUMAN CLASS NATIVES           	  	  	  *
//*********************************************************************

/**
 * @brief Gets the amount of all human classes.
 */
static cell_t API_GetNumberHumanClass(IPluginContext *pContext, const cell_t *params)
{
	// Return the human classes number
	return g_HumanClass;
}

/**
 * @brief Gets the human class index of the client.
 */
static cell_t API_GetClientHumanClass(IPluginContext *pContext, const cell_t *params)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Return the human class index
	return CPlayer->m_nHumanClass;
}

/**
 * @brief Sets the human class index to the client.
 */
static cell_t API_SetClientHumanClass(IPluginContext *pContext, const cell_t *params)
{
	// Validate client
	CBaseClient *CPlayer = GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Set the human class index
	CPlayer->m_nHumanClassNext = params[2];
	
	// Return on success
	return 1;
}

/**
 * @brief Gets the name of the human class.
 */
static cell_t API_GetHumanClassName(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Initialize string
	char *sDest;
	
	// Send string to adress
	pContext->LocalToString(params[2], &sDest);
	strncopy(sDest, g_HumanClassData[iD].HumanClass_Name, params[3]);
		
	// Return on success
	return 1;
}

/**
 * @brief Gets the model path of the human class.
 */
static cell_t API_GetHumanClassModel(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}

	// Initialize string
	char *sDest;
	
	// Send string to adress
	pContext->LocalToString(params[2], &sDest);
	strncopy(sDest, g_HumanClassData[iD].HumanClass_Model, params[3]);

	// Return on success
	return 1;
}

/**
 * @brief Gets the arm model path of the human class.
 */
static cell_t API_GetHumanClassArm(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Initialize string
	char *sDest;
	
	// Send string to adress
	pContext->LocalToString(params[2], &sDest);
	strncopy(sDest, g_HumanClassData[iD].HumanClass_Arm, params[3]);

	// Return on success
	return 1;
}

/**
 * @brief Gets the health of the human class.
 */
static cell_t API_GetHumanClassHealth(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return HumanGetHealth(iD);
}

/**
 * @brief Gets the speed of the human class.
 */
static cell_t API_GetHumanClassSpeed(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return sp_ftoc(HumanGetSpeed(iD));
}

/**
 * @brief Gets the gravity of the human class.
 */
static cell_t API_GetHumanClassGravity(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return sp_ftoc(HumanGetGravity(iD));
}

/**
 * @brief Gets the armor of the human class.
 */
static cell_t API_GetHumanClassArmor(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return HumanGetArmor(iD);
}

/**
 * @brief Check the gender of the human class.
 */
static cell_t API_GetHumanClassGender(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}
	
	// Return default restriction status
	return HumanIsFemale(iD);
}

/**
 * @brief Gets the index of the human class player model.
 */
static cell_t API_GetHumanClassBodyID(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}

	// Return default restriction status
	return HumanGetBodyIndex(iD);
}

/**
 * @brief Gets the index of the human class arm model.
 */
static cell_t API_GetHumanClassArmID(IPluginContext *pContext, const cell_t *params)
{
	// Get the class index.
	unsigned int iD = params[1];
	
	// Validate index
	if(iD >= g_HumanClass)
	{
		return pContext->ThrowNativeError("[ZM] Invalid the class index (%d)", iD);
	}

	// Return default restriction status
	return HumanGetArmIndex(iD);
}

/**
 * @brief Load the human class from other sm plugin.
 */
static cell_t API_RegisterHumanClass(IPluginContext *pContext, const cell_t *params)
{
	// Maximum amout of human classes
	if(g_HumanClass >= MAX_CLASSES)
	{
		return pContext->ThrowNativeError("[ZM] Maximum number of human classes reached (%d). Skipping other classes.", g_HumanClass);
	}

	// Get main values from the native
	pContext->LocalToString(params[1], &g_HumanClassData[g_HumanClass].HumanClass_Name);
	pContext->LocalToString(params[2], &g_HumanClassData[g_HumanClass].HumanClass_Model);
	pContext->LocalToString(params[3], &g_HumanClassData[g_HumanClass].HumanClass_Arm);
	
	// Strings are empty ?
	if (!(strlen(g_HumanClassData[g_HumanClass].HumanClass_Name) || strlen(g_HumanClassData[g_HumanClass].HumanClass_Model) || strlen(g_HumanClassData[g_HumanClass].HumanClass_Arm)))
	{
		return pContext->ThrowNativeError("[ZM] Can't register human class (%d) with an empty name, model pathes", g_HumanClass);
	}
	
	// Precache player models
	g_HumanClassData[g_HumanClass].HumanClass_BodyID = engine->PrecacheModel(g_HumanClassData[g_HumanClass].HumanClass_Model, true);
	if(!engine->IsModelPrecached(g_HumanClassData[g_HumanClass].HumanClass_Model))
	{
		return pContext->ThrowNativeError("[ZM] Can't precache human class player model (%d)", g_HumanClass);
	}
	
	// Precache arm models
	g_HumanClassData[g_HumanClass].HumanClass_ArmID = engine->PrecacheModel(g_HumanClassData[g_HumanClass].HumanClass_Arm, true);
	if(!engine->IsModelPrecached(g_HumanClassData[g_HumanClass].HumanClass_Model))
	{
		return pContext->ThrowNativeError("[ZM] Can't register human class arm model (%d)", g_HumanClass);
	}
	
	// Get other values from the native
	g_HumanClassData[g_HumanClass].HumanClass_Health   	= params[4];
	g_HumanClassData[g_HumanClass].HumanClass_Speed 	= sp_ctof(params[5]);
	g_HumanClassData[g_HumanClass].HumanClass_Gravity  	= sp_ctof(params[6]);
	g_HumanClassData[g_HumanClass].HumanClass_Armor 	= params[7];
	g_HumanClassData[g_HumanClass].HumanClass_Female   	= params[8] ? true : false;
	
	// Increment human class index
	g_HumanClass++;

	// Return id under which we registered the class
	return g_HumanClass-1;
}


//*********************************************************************
//*                  			OTHER NATIVES           	  	  	  *
//*********************************************************************

/**
 * @brief Gets amount of total humans.
 */
static cell_t API_GetHumanAmount(IPluginContext *pContext, const cell_t *params)
{
	return UTIL_GetHumans();
}

/**
 * @brief Gets amount of total zombies.
 */
static cell_t API_GetZombieAmount(IPluginContext *pContext, const cell_t *params)
{
	return UTIL_GetZombies();
}

/**
 * @brief  Gets amount of total alive players.
 */
static cell_t API_GetAliveAmount(IPluginContext *pContext, const cell_t *params)
{
	return UTIL_GetAlive();
}

/**
 * @brief Gets amount of total playing players.
 */
static cell_t API_GetPlayingAmount(IPluginContext *pContext, const cell_t *params)
{
	return UTIL_GetPlaying();
}


//*********************************************************************
//*                  		NATIVE CELLS LIST           	  	  	  *
//*********************************************************************
 
/* Natives list */
sp_nativeinfo_t g_ZombieModNatives[] = 
{
	//* Server natives */
	{"ZM_IsRoundStarted",			API_IsRoundStarted},
	
	//* Player natives */
	{"ZM_IsPlayerZombie",			API_IsZombie},
	{"ZM_MakePlayerZombie", 		API_MakeZombie},
	{"ZM_MakePlayerHuman", 			API_MakeHuman},
	
	//* Class natives */
	{"ZM_GetNumberZombieClass", 	API_GetNumberZombieClass},
	{"ZM_GetNumberHumanClass", 		API_GetNumberHumanClass},
	
	{"ZM_GetClientZombieClass", 	API_GetClientZombieClass},
	{"ZM_SetClientZombieClass",		API_SetClientZombieClass},
	{"ZM_GetClientHumanClass", 		API_GetClientHumanClass},
	{"ZM_SetClientHumanClass",		API_SetClientHumanClass},
	
	{"ZM_GetZombieClassName", 		API_GetZombieClassName},
	{"ZM_GetZombieClassModel", 		API_GetZombieClassModel},
	{"ZM_GetZombieClassClaw", 		API_GetZombieClassClaw},
	{"ZM_GetZombieClassHealth", 	API_GetZombieClassHealth},
	{"ZM_GetZombieClassSpeed", 		API_GetZombieClassSpeed},
	{"ZM_GetZombieClassGravity", 	API_GetZombieClassGravity},
	{"ZM_GetZombieClassKnockBack", 	API_GetZombieClassKnockBack},
	{"ZM_GetZombieClassGender", 	API_GetZombieClassGender},
	{"ZM_GetZombieClassBodyID", 	API_GetZombieClassBodyID},
	{"ZM_GetZombieClassClawID", 	API_GetZombieClassClawID},
	{"ZM_RegisterZombieClass", 		API_RegisterZombieClass},
	
	{"ZM_GetHumanClassName", 		API_GetHumanClassName},
	{"ZM_GetHumanClassModel", 		API_GetHumanClassModel},
	{"ZM_GetHumanClassArm", 		API_GetHumanClassArm},
	{"ZM_GetHumanClassHealth", 		API_GetHumanClassHealth},
	{"ZM_GetHumanClassSpeed", 		API_GetHumanClassSpeed},
	{"ZM_GetHumanClassGravity", 	API_GetHumanClassGravity},
	{"ZM_GetHumanClassArmor", 		API_GetHumanClassArmor},
	{"ZM_GetHumanClassGender", 		API_GetHumanClassGender},
	{"ZM_GetHumanClassBodyID", 		API_GetHumanClassBodyID},
	{"ZM_GetHumanClassArmID", 		API_GetHumanClassArmID},
	{"ZM_RegisterHumanClass", 		API_RegisterHumanClass},
	
	/* Useful utils */
	{"ZM_GetHumanAmount", 			API_GetHumanAmount},
	{"ZM_GetZombieAmount", 			API_GetZombieAmount},
	{"ZM_GetAliveAmount", 			API_GetAliveAmount},
	{"ZM_GetPlayingAmount", 		API_GetPlayingAmount},
	
	{NULL,							NULL},
};

//*********************************************************************
//*                  	HELPER NATIVE FUNCTIONS           	  	  	  *
//*********************************************************************

/**
 * @brief Copy string to another.
 *
 * @param dest  The destenation string. 
 * @param src   The string, which need to be copied.
 * @param count The destenation string lenght. 
 */
unsigned int strncopy(char *dest, const char *src, size_t count)
{
	// If the string is empty, then stop
	if (!count)
	{
		return 0;
	}

	// Copied into the destanation
	char *start = dest;
	while ((*src) && (--count))
	{
		*dest++ = *src++;
	}
	
	// Cut the empty end of the string
	*dest = '\0';

	// Return size
	return (dest - start);
}

/**
 * @brief Gets the name of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel     		The string to return name in.
 **/
char *ZombieGetName(int iD, char *sModel)
{
    // Get class name
	strcpy(sModel, g_ZombieClassData[iD].ZombieClass_Name);
	
	// Return string
	return sModel;
}

/**
 * @brief Gets the player model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *ZombieGetModel(int iD, char *sModel)
{
    // Get model name
	strcpy(sModel, g_ZombieClassData[iD].ZombieClass_Model);
	
	// Return string
	return sModel;
}

/**
 * @brief Gets the knife model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *ZombieGetClawModel(int iD, char *sModel)
{
    // Get claw model name
	strcpy(sModel, g_ZombieClassData[iD].ZombieClass_Claw);
	
	// Return string
	return sModel;
}

/**
 * @brief Gets the health of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The health amount.	
 **/
int ZombieGetHealth(int iD)
{
    // Return default restriction status
    return g_ZombieClassData[iD].ZombieClass_Health;
}

/**
 * @brief Gets the speed of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The speed amount.	
 **/
float ZombieGetSpeed(int iD)
{
    // Return default restriction status
    return g_ZombieClassData[iD].ZombieClass_Speed;
}

/**
 * @brief Gets the gravity of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The gravity amount.	
 **/
float ZombieGetGravity(int iD)
{
    // Return default restriction status
    return g_ZombieClassData[iD].ZombieClass_Gravity;
}

/**
 * @brief Gets the knockback of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The knockback amount.	
 **/
float ZombieGetKnockBack(int iD)
{
    // Return default restriction status
    return g_ZombieClassData[iD].ZombieClass_KnockBack;
}

/**
 * @brief Check the gender of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		True or false.
 **/
bool ZombieIsFemale(int iD)
{
    // Return default restriction status
    return g_ZombieClassData[iD].ZombieClass_Female;
}

/**
 * @brief Gets the index of the zombie class player model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int ZombieGetBodyIndex(int iD)
{
    // Return default restriction status
    return g_ZombieClassData[iD].ZombieClass_BodyID;
}

/**
 * @brief Gets the index of the zombie class claw model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int ZombieGetClawIndex(int iD)
{
    // Return default restriction status
    return g_ZombieClassData[iD].ZombieClass_ClawID;
}

/**
 * @brief Gets the name of a human class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel     		The string to return name in.
 **/
char *HumanGetName(int iD, char *sModel)
{
    // Get class name
	strcpy(sModel, g_HumanClassData[iD].HumanClass_Name);
	
	// Return string
	return sModel;
}

/**
 * @brief Gets the client model of a human class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *HumanGetModel(int iD, char *sModel)
{
    // Get model name
	strcpy(sModel, g_HumanClassData[iD].HumanClass_Model);
	
	// Return string
	return sModel;
}

/**
 * @brief Gets the arm model of a human class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *HumanGetArmModel(int iD, char *sModel)
{
    // Get claw model name
	strcpy(sModel, g_HumanClassData[iD].HumanClass_Arm);
	
	// Return string
	return sModel;
}

/**
 * @brief Gets the health of the human class.
 *
 * @param iD        		The class index.
 * @return          		The health amount.	
 **/
int HumanGetHealth(int iD)
{
    // Return default restriction status
    return g_HumanClassData[iD].HumanClass_Health;
}

/**
 * @brief Gets the speed of the human class.
 *
 * @param iD        		The class index.
 * @return          		The speed amount.	
 **/
float HumanGetSpeed(int iD)
{
    // Return default restriction status
    return g_HumanClassData[iD].HumanClass_Speed;
}

/**
 * @brief Gets the gravity of the human class.
 *
 * @param iD        		The class index.
 * @return          		The gravity amount.	
 **/
float HumanGetGravity(int iD)
{
    // Return default restriction status
    return g_HumanClassData[iD].HumanClass_Gravity;
}

/**
 * @brief Gets the armor of the human class.
 *
 * @param iD        		The class index.
 * @return          		The knockback amount.	
 **/
int HumanGetArmor(int iD)
{
    // Return default restriction status
    return g_HumanClassData[iD].HumanClass_Armor;
}

/**
 * @brief Check the gender of the human class.
 *
 * @param iD        		The class index.
 * @return          		True or false.
 **/
bool HumanIsFemale(int iD)
{
    // Return default restriction status
    return g_HumanClassData[iD].HumanClass_Female;
}

/**
 * @brief Gets the index of the human class player model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int HumanGetBodyIndex(int iD)
{
    // Return default restriction status
    return g_HumanClassData[iD].HumanClass_BodyID;
}

/**
 * @brief Gets the index of the human class arm model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int HumanGetArmIndex(int iD)
{
    // Return default restriction status
    return g_HumanClassData[iD].HumanClass_ArmID;
}