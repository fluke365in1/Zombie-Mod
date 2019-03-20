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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_
 
/**
 * @file natives.h
 * @brief Natives module code header.
 */

/* Extern native list */
extern sp_nativeinfo_t g_ZombieModNatives[];

/* Extern round variables */
extern bool g_IsNewRound;
extern bool g_IsEndRound;
extern bool g_IsOnRound;

/* Extern some util functions */
extern bool UTIL_InfectPlayer(int clientIndex);
extern bool UTIL_HumanizePlayer(int clientIndex);
extern unsigned int UTIL_GetHumans();
extern unsigned int UTIL_GetZombies();
extern unsigned int UTIL_GetAlive();
extern unsigned int UTIL_GetPlaying();

/**
 * @brief Initialize natives and library.
 */
void NativeLoad();

/**
 * @brief Reset natives.
 */
void NaviteShutDown();


 
/* Number of max valid classes */
#define MAX_CLASSES 	32

/**
 * List of operation types for zombie classes data.
 **/
struct ZombieClassData
{
	char *ZombieClass_Name;
	char *ZombieClass_Model;
	char *ZombieClass_Claw;
	int ZombieClass_Health;
	float ZombieClass_Speed;
	float ZombieClass_Gravity;
	float ZombieClass_KnockBack;
	bool ZombieClass_Female;
	int ZombieClass_BodyID;
	int ZombieClass_ClawID;
};

/**
 * List of operation types for human classes data.
 **/
struct HumanClassData
{
	char *HumanClass_Name;
	char *HumanClass_Model;
	char *HumanClass_Arm;
	int HumanClass_Health;
	float HumanClass_Speed;
	float HumanClass_Gravity;
	int HumanClass_Armor;
	bool HumanClass_Female;
	int HumanClass_BodyID;
	int HumanClass_ArmID;
};

/**
 * @brief Copy string to another.
 *
 * @param dest  The destenation string. 
 * @param src   The string, which need to be copied.
 * @param count The destenation string lenght. 
 */
unsigned int strncopy(char *dest, const char *src, size_t count);


//*********************************************************************
//*                  		ZOMBIE CLASS NATIVES           	  	  	  *
//*********************************************************************

/**
 * @brief Gets the name of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel     		The string to return name in.
 **/
char *ZombieGetName(int iD, char *sModel);

/**
 * @brief Gets the client model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *ZombieGetModel(int iD, char *sModel);

/**
 * @brief Gets the knife model of a zombie class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *ZombieGetClawModel(int iD, char *sModel);

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
 * @brief Gets the index of the zombie class player model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int ZombieGetBodyIndex(int iD);

/**
 * @brief Gets the index of the zombie class claw model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int ZombieGetClawIndex(int iD);


//*********************************************************************
//*                  		HUMAN CLASS NATIVES           	  	  	  *
//*********************************************************************

/**
 * @brief Gets the name of a human class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel     		The string to return name in.
 **/
char *HumanGetName(int iD, char *sModel);

/**
 * @brief Gets the client model of a human class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *HumanGetModel(int iD, char *sModel);

/**
 * @brief Gets the arm model of a human class at a given index.
 *
 * @param iD     	 		The class index.
 * @param sModel      		The string to return name in.
 **/
char *HumanGetArmModel(int iD, char *sModel);

/**
 * @brief Gets the health of the human class.
 *
 * @param iD        		The class index.
 * @return          		The health amount.	
 **/
int HumanGetHealth(int iD);

/**
 * @brief Gets the speed of the human class.
 *
 * @param iD        		The class index.
 * @return          		The speed amount.	
 **/
float HumanGetSpeed(int iD);

/**
 * @brief Gets the gravity of the human class.
 *
 * @param iD        		The class index.
 * @return          		The gravity amount.	
 **/
float HumanGetGravity(int iD);

/**
 * @brief Gets the armor of the human class.
 *
 * @param iD        		The class index.
 * @return          		The knockback amount.	
 **/
int HumanGetArmor(int iD);

/**
 * @brief Check the gender of the human class.
 *
 * @param iD        		The class index.
 * @return          		True or false.
 **/
bool HumanIsFemale(int iD);

/**
 * @brief Gets the index of the human class player model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int HumanGetBodyIndex(int iD);

/**
 * @brief Gets the index of the human class arm model.
 *
 * @param iD        		The class index.
 * @return          		The model index.	
 **/
int HumanGetArmIndex(int iD);

#endif // _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_