/**
 * ============================================================================
 *
 *  Zombie Mod #4 Generation
 *
 *
 *  Copyright (C) 2017-2018 Nikita Ushakov (Ireland, Dublin)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 **/

#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <zombiemod>

#pragma newdecls required

/**
 * Record plugin info.
 **/
public Plugin ZombieClawsSystem =
{
	name        	= "[ZM] Addon: Hands",
	author      	= "qubka (Nikita Ushakov)", 	
	description 	= "Addon for adding claws to zombies and arms to humans",
	version     	= "2.0",
	url         	= "https://forums.alliedmods.net/showthread.php?t=290657"
}

// Main arrays
int viewModel[MAXPLAYERS+1];
char sWeapon[2][32];

/**
 * Plugin is loading.
 **/
public void OnPluginStart(/*void*/)
{
	// Hook spawn event
	HookEvent("player_spawn", EventPlayerSpawn, EventHookMode_Post);
}

/**
 * Map is loaded.
 **/
public void OnMapStart(/*void*/)
{
	// Get standart weapon aliases
	GetConVarString(FindConVar("mp_t_default_secondary"), sWeapon[0], sizeof(sWeapon[])); 
	GetConVarString(FindConVar("mp_ct_default_secondary"), sWeapon[1], sizeof(sWeapon[])); 
}

/**
 * Called when a client became a zombie.
 * 
 * @param clientIndex       The client to infect.
 */
public void ZM_OnClientInfected(int clientIndex)
{
	// Validate client
	if(!IsPlayerExist(clientIndex))
	{
		return;
	}

	// Give claws
	GivePlayerItem(clientIndex, "weapon_knife");
	FakeClientCommandEx(clientIndex, "use weapon_knife");
}

/**
 * @brief Called when a client became a human, also on spawn.
 * 
 * @param clientIndex		The client index.
 * @param antidotUse		False on the spawn, true otherelse.
 **/
public void ZM_OnClientHumanized(int clientIndex, bool antidotUse)
{
	// Validate client
	if(!IsPlayerExist(clientIndex))
	{
		return;
	}
	
	// Get arms path
	char sArmsPath[512];
	ZM_GetHumanClassArm(ZM_GetClientHumanClass(clientIndex), sArmsPath, sizeof(sArmsPath));
	
	// Set arms prop
	SetEntPropString(clientIndex, Prop_Send, "m_szArmsModel", sArmsPath);

	// If player was humanized
	if(antidotUse)
	{
		// Give weapons
		GivePlayerItem(clientIndex, "weapon_knife");
		GivePlayerItem(clientIndex, sWeapon[1]);
	}
	
	// Select them
	FakeClientCommandEx(clientIndex, "use %s", sWeapon[0]);
	FakeClientCommandEx(clientIndex, "use %s", sWeapon[1]);
}

/**
 * Event callback (player_spawn)
 * The player is spawning.
 * 
 * @param gEventHook      	The event handle.
 * @param gEventName        The name of the event.
 * @dontBroadcast   	    If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerSpawn(Event gEventHook, const char[] gEventName, bool dontBroadcast)
{
	// Get real player index from event key
	int clientIndex = GetClientOfUserId(GetEventInt(gEventHook, "userid")); 

	#pragma unused clientIndex
	
	// Validate client
	if(!IsPlayerExist(clientIndex))
	{
		return;
	}
	
	// Get client's view model
	viewModel[clientIndex] = WeaponsGetViewModelIndex(clientIndex);
}

/**
 * Called once a client is authorized and fully in-game, and 
 * after all post-connection authorizations have been performed.  
 *
 * This callback is gauranteed to occur on all clients, and always 
 * after each OnClientPutInServer() call.
 * 
 * @param clientIndex		The client index. 
 **/
public void OnClientPostAdminCheck(int clientIndex)
{
	// Hook weapon events
	SDKHook(clientIndex, SDKHook_WeaponCanUse,     WeaponsOnCanUse);
	SDKHook(clientIndex, SDKHook_WeaponSwitchPost, WeaponsOnDeployPost);
}

/**
 * Hook: WeaponSwitchPost
 * Called, when player deploy any weapon.
 *
 * @param clientIndex	 	The client index.
 * @param weaponIndex    	The weapon index.
 **/
public void WeaponsOnDeployPost(int clientIndex, int weaponIndex) 
{
	// If weapon isn't valid, then stop
	if(!IsValidEdict(weaponIndex))
	{
		return;
	}

	// Validate client
	if(!IsPlayerExist(clientIndex))
	{
		return;
	}

	// Validate zombie
	if(ZM_IsPlayerZombie(clientIndex))
	{
		// Get weapon classname
		char sClassname[32];
		GetEntityClassname(weaponIndex, sClassname, sizeof(sClassname));

		// Validate weapons
		if (StrEqual(sClassname, "weapon_knife", false))
		{
			WeaponsSetViewModel(weaponIndex, viewModel[clientIndex], ZM_GetZombieClassClawID(ZM_GetClientZombieClass(clientIndex)));
		}
	}
} 

/**
 * Hook: WeaponCanUse
 * Called, when player deploy or pick-up any weapon.
 *
 * @param clientIndex	 	The client index.
 * @param weaponIndex    	The weapon index.
 **/
public Action WeaponsOnCanUse(int clientIndex, int weaponIndex)
{
	// If weapon isn't valid, then stop
	if(!IsValidEdict(weaponIndex))
	{
		return Plugin_Handled;
	}
	
	// Get weapon classname
	char sClassname[32];
	GetEntityClassname(weaponIndex, sClassname, sizeof(sClassname));

	// Validate weapons
	if (StrEqual(sClassname, "weapon_knife", false))
	{
		return Plugin_Continue;
	}

	// Validate client
	if (!IsPlayerExist(clientIndex))
	{
		return Plugin_Handled;
	}

	// Block pickup anything, if you a zombie
	if(ZM_IsPlayerZombie(clientIndex))
	{
		return Plugin_Handled;
	}
	
	// Allow pickup
	return Plugin_Continue;
}
	
/**
 * Gets view model index.
 *
 * @param clientIndex    	The client index.
 *
 * @return 				 	The view entity index.	
 **/
stock int WeaponsGetViewModelIndex(int clientIndex)
{
	// Initialize index
	int nIndex = -1;
	
	// Find entity with the view model and return index
	while ((nIndex = WeaponsGetEntityByClassname(nIndex, "predicted_viewmodel")) != -1)
	{
		// Get the owner of the view model
		int ownerIndex = GetEntPropEnt(nIndex, Prop_Data, "m_hOwner");

		// If owner isn't equal to client index, skip
		if (ownerIndex != clientIndex)
		{	
			continue;
		}
		
		// Return index of the entity
		return nIndex;
	}
	
	// If didn't find, then stop
	return -1;
}

/**
 * Function for finding entity by classname.
 *
 * @param startIndex		The entity index.
 * @param sClassname		The entity classname.
 *
 * @return					The entity index.	
 **/
stock int WeaponsGetEntityByClassname(int startIndex, char[] sClassname)
{
	// Substitute any invalid entities
	while (startIndex > -1 && !IsValidEdict(startIndex)) startIndex--;
	
	// Index of an entity by the classname
	return FindEntityByClassname(startIndex, sClassname);
}
	
/**
 * Sets the view weapon model.
 *
 * @param weaponIndex		The weapon index.
 * @param viewIndex			The view model index.
 * @param modelIndex		The model index. (Must be precached)
 **/
stock void WeaponsSetViewModel(int weaponIndex, int viewIndex, int modelIndex)
{
	// Delete default model index
	SetEntProp(weaponIndex, Prop_Send, "m_nModelIndex", 0);
	
	// Set a new view model index for the weapon
	SetEntProp(viewIndex, Prop_Send, "m_nModelIndex", modelIndex);
}