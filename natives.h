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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_
 
/**
 * @file natives.h
 * @brief Natives code header.
 */
 
/**
 * Number of max valid addons.
 **/
#define ZombieClassMax 	32

/**
 * List of operation types for zombie classes data.
 **/
struct ZombieClassData
{
	char *ZombieClass_Name;
	char *ZombieClass_Model;
	char *ZombieClass_Claw;
	unsigned int ZombieClass_Health;
	float ZombieClass_Speed;
	float ZombieClass_Gravity;
	float ZombieClass_KnockBack;
	bool ZombieClass_Female;
	int ZombieClass_ClawID;
};

/**
 * @brief Gets the name of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel     		The string to return name in.
 **/
void ZombieGetName(int iD, char *sModel);

/**
 * @brief Gets the player model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
void ZombieGetModel(int iD, char *sModel);
/**
 * @brief Gets the knife model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
void ZombieGetClawModel(int iD, char *sModel);

/**
 * @brief Gets the health of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The health amount.	
 **/
int ZombieGetHealth(int iD);

/**
 * @brief Gets the speed of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The speed amount.	
 **/
float ZombieGetSpeed(int iD);

/**
 * @brief Gets the gravity of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The gravity amount.	
 **/
float ZombieGetGravity(int iD);

/**
 * @brief Gets the knockback of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		The knockback amount.	
 **/
float ZombieGetKnockBack(int iD);
/**
 * @brief Check the gender of the zombie class.
 *
 * @param iD        		The class index.
 * @return          		True or false.
 **/
bool ZombieIsFemale(int iD);

/**
 * @brief Gets the index of the zombie class claw model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int ZombieGetClawIndex(int iD);

/**
 * @brief Sets the index of the zombie class claw model.
 *
 * @param iD         		The class index.
 * @param modelIndex  		The model index.	
 **/
void ZombieSetClawIndex(int iD, int modelIndex);

#endif // _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_