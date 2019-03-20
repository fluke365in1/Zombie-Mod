/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod (C)2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This file is part of the SourceMod/SourcePawn SDK.
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

#if defined _zombiemod_included
 #endinput
#endif
#define _zombiemod_included


/**
 * @brief Called when a client became a zombie.
 * 
 * @param clientIndex		The client index.
 **/
forward void ZM_OnClientInfected(int clientIndex);

/**
 * @brief Called when a client became a human, also on spawn.
 * 
 * @param clientIndex		The client index.
 * @param antidotUse		False on the spawn, true otherelse.
 **/
forward void ZM_OnClientHumanized(int clientIndex, bool antidotUse);

/**
 * @brief Called after a zombie round is started.
 **/
forward void ZM_OnZombieModStarted();


/**
 * @brief Gets the zombie round started.
 *
 * @return 					True or false.
 */
native bool ZM_IsRoundStarted();

/**
 * @brief Gets the zombie status of the client.
 * 
 * @param clientIndex		The client index.
 * @return 					True or false.
 */
native bool ZM_IsPlayerZombie(int clientIndex);

/**
 * @brief Turn the client into the zombie.
 * 
 * @param clientIndex		The client index.
 * @return 					True on the success, false otherelse.
 */		
native bool ZM_MakePlayerZombie(int clientIndex);

/**
 * @brief Turn the client into the human.
 * 
 * @param clientIndex		The client index.
 * @return 					True on the success, false otherelse.
 */		
native bool ZM_MakePlayerHuman(int clientIndex);

/**
 * @brief Gets the amount of all zombie classes.
 *
 * @return          		The zombie class amount.	
 */
native int ZM_GetNumberZombieClass();

/**
 * @brief Gets the zombie class index of the client.
 * 
 * @param clientIndex		The client index.
 * @return          		The zombie class index.	
 */
native int ZM_GetClientZombieClass(int clientIndex);

/**
 * @brief Sets the zombie class index to the client.
 * 
 * @param clientIndex		The client index.
 * @param classIndex        The zombie class index.	
 */
native void ZM_SetClientZombieClass(int clientIndex, int classIndex);

/**
 * @brief Registers a custom class which will be added to the zombie classes menu of ZP.
 *
 * 	Note: The returned zombie class ID can be later used to identify
 * 			the class when calling the ZP_GetClientZombieClass() natives.
 *
 * @param name				Caption to display on the menu.
 * @param model				Player model to be used.
 * @param claw_model		Knife model to be used.
 * @param health			Initial health points.
 * @param speed				Maximum speed.
 * @param gravity			Gravity multiplier.
 * @param knockback			Knockback multiplier.
 * @param gender			Gender index.
 *
 * @return					An internal zombie class ID, or -1 on failure.
 **/
native int ZM_RegisterZombieClass(char[] name, char[] model, char[] claw_model, int health, float speed, float gravity, float knockback, int gender);

/**
 * @brief Gets the name of a zombie class at a given index.
 *
 * @param classID     	 	The class index.
 * @param name     			The string to return name in.
 * @param maxlenght			The lenght of string.
 **/
native void ZM_GetZombieClassName(int classID, char[] name, int maxlenght = 32);

/**
 * @brief Gets the player model of a zombie class at a given index.
 *
 * @param classID     	 	The class index.
 * @param model      		The string to return name in.
 * @param maxlenght			The lenght of string.
 **/
native void ZM_GetZombieClassModel(int classID, char[] model, int maxlenght = 512);

/**
 * @brief Gets the knife model of a zombie class at a given index.
 *
 * @param classID     	 	The class index.
 * @param model      		The string to return name in.
 * @param maxlenght			The lenght of string.
 **/
native void ZM_GetZombieClassClaw(int classID, char[] model, int maxlenght = 512);

/**
 * @brief Gets the health of the zombie class.
 *
 * @param classID        	The class index.
 * @return          		The health amount.	
 **/
native int ZM_GetZombieClassHealth(int classID);

/**
 * @brief Gets the speed of the zombie class.
 *
 * @param classID        	The class index.
 * @return          		The speed amount.	
 **/
native float ZM_GetZombieClassSpeed(int classID);

/**
 * @brief Gets the gravity of the zombie class.
 *
 * @param classID        	The class index.
 * @return          		The gravity amount.	
 **/
native float ZM_GetZombieClassGravity(int classID);

/**
 * @brief Gets the knockback of the zombie class.
 *
 * @param classID        	The class index.
 * @return          		The knockback amount.	
 **/
native float ZM_GetZombieClassKnockBack(int classID);

/**
 * @brief Check the gender of the zombie class.
 *
 * @param classID        	The class index.
 * @return          		True or false.
 **/
native int ZM_GetZombieClassGender(int classID);

/**
 * @brief Gets the index of the zombie class player model.
 *
 * @param classID        	The class index.
 * @return          		The model index.	
 **/
native int ZM_GetZombieClassBodyID(int classID);

/**
 * @brief Gets the index of the zombie class claw model.
 *
 * @param classID        	The class index.
 * @return          		The model index.	
 **/
native int ZM_GetZombieClassClawID(int classID);

/**
 * @brief Gets the amount of all human classes.
 *
 * @return          		The human class amount.	
 */
native int ZM_GetNumberHumanClass();

/**
 * @brief Gets the human class index of the client.
 * 
 * @param clientIndex		The client index.
 * @return          		The human class index.	
 */
native int ZM_GetClientHumanClass(int clientIndex);

/**
 * @brief Sets the human class index to the client.
 * 
 * @param clientIndex		The client index.
 * @param classIndex        The human class index.	
 */
native void ZM_SetClientHumanClass(int clientIndex, int classIndex);

/**
 * @brief Registers a custom class which will be added to the human classes menu of ZP.
 *
 * 	Note: The returned human class ID can be later used to identify
 * 			the class when calling the ZP_GetClientHumanClass() natives.
 *
 * @param name				Caption to display on the menu.
 * @param model				Player model to be used.
 * @param arm_model			Arm model to be used.
 * @param health			Initial health points.
 * @param speed				Maximum speed.
 * @param gravity			Gravity multiplier.
 * @param armor				Armor points.
 * @param gender			Gender index.
 *
 * @return					An internal human class ID, or -1 on failure.
 **/
native int ZM_RegisterHumanClass(char[] name, char[] model, char[] arm_model, int health, float speed, float gravity, int armor, int gender);

/**
 * @brief Gets the name of a human class at a given index.
 *
 * @param classID     	 	The class index.
 * @param name     			The string to return name in.
 * @param maxlenght			The lenght of string.
 **/
native void ZM_GetHumanClassName(int classID, char[] name, int maxlenght = 32);

/**
 * @brief Gets the player model of a human class at a given index.
 *
 * @param classID     	 	The class index.
 * @param model      		The string to return name in.
 * @param maxlenght			The lenght of string.
 **/
native void ZM_GetHumanClassModel(int classID, char[] model, int maxlenght = 512);

/**
 * @brief Gets the arm model of a human class at a given index.
 *
 * @param classID     	 	The class index.
 * @param model      		The string to return name in.
 * @param maxlenght			The lenght of string.
 **/
native void ZM_GetHumanClassArm(int classID, char[] model, int maxlenght = 512);

/**
 * @brief Gets the health of the human class.
 *
 * @param classID        	The class index.
 * @return          		The health amount.	
 **/
native int ZM_GetHumanClassHealth(int classID);

/**
 * @brief Gets the speed of the human class.
 *
 * @param classID        	The class index.
 * @return          		The speed amount.	
 **/
native float ZM_GetHumanClassSpeed(int classID);

/**
 * @brief Gets the gravity of the human class.
 *
 * @param classID        	The class index.
 * @return          		The gravity amount.	
 **/
native float ZM_GetHumanClassGravity(int classID);

/**
 * @brief Gets the armor of the human class.
 *
 * @param classID        	The class index.
 * @return          		The armor amount.	
 **/
native int ZM_GetHumanClassArmor(int classID);

/**
 * @brief Check the gender of the human class.
 *
 * @param classID        	The class index.
 * @return          		True or false.
 **/
native int ZM_GetHumanClassGender(int classID);

/**
 * @brief Gets the index of the human class player model.
 *
 * @param classID        	The class index.
 * @return          		The model index.	
 **/
native int ZM_GetHumanClassBodyID(int classID);

/**
 * @brief Gets the index of the human class arm model.
 *
 * @param classID        	The class index.
 * @return          		The model index.	
 **/
native int ZM_GetHumanClassArmID(int classID);

/**
 * @brief Gets amount of total humans.
 * 
 * @return	 				The amount of total humans.
 **/
native int ZM_GetHumanAmount();		

/**
 * @brief Gets amount of total zombies.
 *
 * @return	 				The amount of total zombies.
 **/
native int ZM_GetZombieAmount();

/**
 * @brief Gets amount of total alive players.
 *
 * @return	 				The amount of total alive players.
 **/
native int ZM_GetAliveAmount();

/**
 * @brief Gets amount of total playing players.
 *
 * @return					The amount of total playing players.
 **/
native int ZM_GetPlayingAmount();

/**
 * Returns true if the player is connected and alive, false if not.
 *
 * @param clientIndex		The client index.
 * @param aliveness			(Optional) Set to true to validate that the client is alive, false to ignore.
 *  
 * @return					True or false.
 **/
stock bool IsPlayerExist(int clientIndex, bool aliveness = true)
{
	// If client isn't valid
	if (clientIndex <= 0 || clientIndex > MaxClients)
	{
		return false;
	}
	
	// If client isn't connected
	if (!IsClientConnected(clientIndex))
	{
		return false;
	}

	// If client isn't in game
	if (!IsClientInGame(clientIndex))
	{
		return false;
	}

	// If client isn't alive
	if(aliveness && !IsPlayerAlive(clientIndex))
	{
		return false;
	}
	
	// If client exist
	return true;
}

/**
 * Precache folder and add it to download table.
 *
 * @param sPath    		The path to file.
 **/
stock void AddFolderToPrecache(char[] sDownloadPath)
{
	// If file exist
	if (FileExists(sDownloadPath))
	{
		// Precache file and add to downloads table
		AddFileToPrecache(sDownloadPath);
	}
	// If doesn't exist, may be this is directory ?
	else
	{
		// Open directory
		DirectoryListing sDirectory = OpenDirectory(sDownloadPath);

		// If directory doesn't exist, then skip
		if(sDirectory == INVALID_HANDLE)
		{
			return;
		}

		// Initialize some variables
		char sFile[64];
		char sLine[512];

		// File types
		FileType sType;

		// Search any files in directory and precache them
		while (ReadDirEntry(sDirectory, sFile, sizeof(sFile), sType)) 
		{ 
			if (sType == FileType_File) 
			{
				// Format full path to file
				Format(sLine, sizeof(sLine), "%s%s", sDownloadPath, sFile);

				// Add to server precache list
				AddFileToPrecache(sLine);
			}
		}

		// Close directory
		CloseHandle(sDirectory); 
	}
}

/**
 * Precache file and add it to download table.
 *
 * @param sPath    		The path to file.
 **/
stock void AddFileToPrecache(const char[] sPath)
{
	// Add to downloads
	AddFileToDownloadsTable(sPath);
	
	// Add to precache if path contains '.mdl'
	if(StrContains(sPath, ".mdl", false) != -1)
	{
		PrecacheModel(sPath, true);
	}
}