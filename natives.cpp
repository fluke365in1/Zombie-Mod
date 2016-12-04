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
 * @file api.cpp
 * @brief Natives module code file.
 */
 
#include "natives.h"

/**
 * Arrays to store the server's zombie classes.
 **/
unsigned int g_ZombieClass;
ZombieClassData *g_ZombieClassData[ZombieClassMax];

/**
 * @brief Gets the zombie status of the player.
 */
static cell_t API_IsZombie(IPluginContext *pContext, const cell_t *params)
{
	// Validate client
	ZPBaseClient *CPlayer = ZP_GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Return the zombie status
	return CPlayer->m_bZombie;
}

/**
 * @brief Gets the amount of all zombie classes.
 */
static cell_t API_GetNumberZombieClass(IPluginContext *pContext, const cell_t *params)
{
	// Return the zombie classes number
	return g_ZombieClass;
}

/**
 * @brief Gets the zombie class index of the player.
 */
static cell_t API_GetClientZombieClass(IPluginContext *pContext, const cell_t *params)
{
	// Validate client
	ZPBaseClient *CPlayer = ZP_GetPlayer(params[1]);
	if(!CPlayer)
	{
		return pContext->ThrowNativeError("[ZM] Invalid client index (%d)", params[1]);
	}
	
	// Return the zombie class index
	return CPlayer->m_nZombieClass;
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
	
	// Send string to adress
	pContext->StringToLocal(params[2], params[3], g_ZombieClassData[iD]->ZombieClass_Name);

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

	// Send string to adress
	pContext->StringToLocal(params[2], params[3], g_ZombieClassData[iD]->ZombieClass_Model);

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
	
	// Send string to adress
	pContext->StringToLocal(params[2], params[3], g_ZombieClassData[iD]->ZombieClass_Claw);

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
	return ZombieGetSpeed(iD);
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
	return ZombieGetGravity(iD);
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
	return ZombieGetKnockBack(iD);
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
	if(g_ZombieClass >= ZombieClassMax)
	{
		return pContext->ThrowNativeError("[ZM] Maximum number of zombie classes reached (%d). Skipping other classes.", g_ZombieClass);
	}
	
	// Get main values from the native
	pContext->LocalToString(params[1], &g_ZombieClassData[g_ZombieClass]->ZombieClass_Name);
	pContext->LocalToString(params[2], &g_ZombieClassData[g_ZombieClass]->ZombieClass_Model);
	pContext->LocalToString(params[3], &g_ZombieClassData[g_ZombieClass]->ZombieClass_Claw);
	
	// Strings are empty ?
	if (!(strlen(g_ZombieClassData[g_ZombieClass]->ZombieClass_Name) || strlen(g_ZombieClassData[g_ZombieClass]->ZombieClass_Model) || strlen(g_ZombieClassData[g_ZombieClass]->ZombieClass_Claw)))
	{
		return pContext->ThrowNativeError("[ZM] Can't register zombie class (%d) with an empty name", g_ZombieClass);
	}
	
	// Get other values from the native
	g_ZombieClassData[g_ZombieClass]->ZombieClass_Health   	= params[4];
	g_ZombieClassData[g_ZombieClass]->ZombieClass_Speed 	= params[5];
	g_ZombieClassData[g_ZombieClass]->ZombieClass_Gravity  	= params[6];
	g_ZombieClassData[g_ZombieClass]->ZombieClass_KnockBack = params[7];
	g_ZombieClassData[g_ZombieClass]->ZombieClass_Female   	= params[8] ? true : false;

	// Increment zombie class index
	g_ZombieClass++;

	// Return id under which we registered the class
	return g_ZombieClass-1;
}
 
/* Natives list */
sp_nativeinfo_t g_ZombieNatives[] = 
{
	{"ZM_IsPlayerZombie",			API_IsZombie},

	{"ZM_GetNumberZombieClass", 	API_GetNumberZombieClass},
	{"ZM_GetClientZombieClass", 	API_GetClientZombieClass},
	{"ZM_GetZombieClassName", 		API_GetZombieClassName},
	{"ZM_GetZombieClassModel", 		API_GetZombieClassModel},
	{"ZM_GetZombieClassClaw", 		API_GetZombieClassClaw},
	{"ZM_GetZombieClassHealth", 	API_GetZombieClassHealth},
	{"ZM_GetZombieClassSpeed", 		API_GetZombieClassSpeed},
	{"ZM_GetZombieClassGravity", 	API_GetZombieClassGravity},
	{"ZM_GetZombieClassKnockBack", 	API_GetZombieClassKnockBack},
	{"ZM_GetZombieClassGender", 	API_GetZombieClassGender},
	{"ZM_GetZombieClassClawID", 	API_GetZombieClassClawID},
	{"ZM_RegisterZombieClass", 		API_RegisterZombieClass},
	{NULL,							NULL},
};

/**
 * @brief Gets the name of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel     		The string to return name in.
 **/
void ZombieGetName(int iD, char *sModel)
{
    // Get class name
	strcpy(sModel, g_ZombieClassData[iD]->ZombieClass_Name);
}

/**
 * @brief Gets the player model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
void ZombieGetModel(int iD, char *sModel)
{
    // Get model name
	strcpy(sModel, g_ZombieClassData[iD]->ZombieClass_Model);
}

/**
 * @brief Gets the knife model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
void ZombieGetClawModel(int iD, char *sModel)
{
    // Get claw model name
	strcpy(sModel, g_ZombieClassData[iD]->ZombieClass_Claw);
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
    return g_ZombieClassData[iD]->ZombieClass_Health;
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
    return g_ZombieClassData[iD]->ZombieClass_Speed;
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
    return g_ZombieClassData[iD]->ZombieClass_Gravity;
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
    return g_ZombieClassData[iD]->ZombieClass_KnockBack;
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
    return g_ZombieClassData[iD]->ZombieClass_Female;
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
    return g_ZombieClassData[iD]->ZombieClass_ClawID;
}

/**
 * @brief Sets the index of the zombie class claw model.
 *
 * @param iD         		The class index.
 * @param modelIndex  		The model index.	
 **/
void ZombieSetClawIndex(int iD, int modelIndex)
{
    // Set the claw index
    g_ZombieClassData[iD]->ZombieClass_ClawID = modelIndex;
}