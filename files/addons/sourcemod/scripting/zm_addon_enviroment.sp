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
public Plugin ZombieEnviromentSystem =
{
	name        	= "[ZM] Addon: Enviroment System",
	author      	= "qubka (Nikita Ushakov)", 	
	description 	= "Addon for adding knockback to zombies",
	version     	= "1.0",
	url         	= "https://forums.alliedmods.net/showthread.php?t=290657"
}

/**
 * @section Damage flags.
 **/
#define DMG_CSGO_FALL        (DMG_FALL)      /** Client was damaged by falling.	 **/
#define DMG_CSGO_BLAST       (DMG_BLAST)     /** Client was damaged by explosion.**/
#define DMG_CSGO_BURN        (DMG_BURN)    	 /** Client was damaged by inferno.	 **/
#define DMG_CSGO_FIRE        (DMG_DIRECT)    /** Client was damaged by fire.	 **/
#define DMG_CSGO_BULLET      (DMG_NEVERGIB)  /** Client was shot or knifed. 	 **/
#define DMG_CSGO_DROWN    	 (DMG_DROWN)     /** Client was damaged by water. 	 **/
/**
 * @endsection
 **/

/**
 * The list of zombie footsteps sounds.
 **/
char FootstepSound[24][128] =
{
	"*/zpzs/player_zombie_nomal_male_footstep1.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep2.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep3.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep4.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep5.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep6.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep7.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep8.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep9.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep10.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep11.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_nomal_male_footstep12.mp3",		/** Footstep Male  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep1.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep2.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep3.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep4.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep5.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep6.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep7.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep8.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep9.mp3",		/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep10.mp3",	/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep11.mp3",	/** Footstep Fale  Sound **/
	"*/zpzs/player_zombie_normal_female_footstep12.mp3"		/** Footstep Fale  Sound **/
};

/**
 * The list of zombie damage sounds.
 **/
char DamageSound[2][128] =
{
	"*/zbm3/zombi_hurt_1.mp3",				/** Damage Sound **/
	"*/zbm3/zombi_hurt_2.mp3"				/** Damage Sound **/
};

/**
 * The list of zombie death sounds.
 **/
char DeathSound[2][128] =
{
	"*/zbm3/zombi_death_1.mp3",				/** Death Sound **/
	"*/zbm3/zombi_death_2.mp3"				/** Death Sound **/
};

/**
 * The list of zombie infect sounds.
 **/
char InfectSound[2][128] =
{
	"*/zpzs/zombie_infection_male.mp3",		/** Infect Male Sound **/
	"*/zpzs/zombie_infection_female.mp3"	/** Infect Fale Sound **/
};

/**
 * The list of zombie death sounds.
 **/
char RoundSound[4][128] =
{
	"*/zbm3/the_horror1.mp3",				/** Round Sound **/
	"*/zbm3/the_horror2.mp3",				/** Round Sound **/
	"*/zbm3/the_horror3.mp3",				/** Round Sound **/
	"*/zbm3/the_horror4.mp3"				/** Round Sound **/
};
 
/**
 * Plugin is loading.
 **/
public void OnPluginStart(/*void*/)
{
	// Hook spawn event
	HookEvent("player_death", EventPlayerDeath, EventHookMode_Post);
	
	// Load translations phrases used by plugin
	LoadTranslations("zombiemod.pharses");
}
 
/**
 * The map is starting.
 **/
public void OnMapStart(/*void*/)
{
	// Hooks all played normal sounds
	AddNormalSoundHook(view_as<NormalSHook>(SoundsHook));
	
	// Initialize char
	char sPath[128];
	
	//*********************************************************************
	//*              PRECACHE OF ZOMBIE FOOTSTEPS SOUNDS                  *
	//*********************************************************************
	
	// Precache of the sounds
	for (int i = 0; i < sizeof(FootstepSound); i++)
	{
		Format(sPath, sizeof(sPath), "%s", FootstepSound[i]);
		AddToStringTable(FindStringTable("soundprecache"), sPath);
		
		ReplaceString(sPath, sizeof(sPath), "*/", "sound/");
		AddFileToPrecache(sPath);
	}
	
	//*********************************************************************
	//*              	PRECACHE OF ZOMBIE DAMAGE SOUNDS                  *
	//*********************************************************************
	
	// Precache of the sounds
	for (int i = 0; i < sizeof(DamageSound); i++)
	{
		Format(sPath, sizeof(sPath), "%s", DamageSound[i]);
		AddToStringTable(FindStringTable("soundprecache"), sPath);
		
		ReplaceString(sPath, sizeof(sPath), "*/", "sound/");
		AddFileToPrecache(sPath);
	}
	
	//*********************************************************************
	//*             	 PRECACHE OF ZOMBIE DEATH SOUNDS                  *
	//*********************************************************************
	
	// Precache of the sounds
	for (int i = 0; i < sizeof(DeathSound); i++)
	{
		Format(sPath, sizeof(sPath), "%s", DeathSound[i]);
		AddToStringTable(FindStringTable("soundprecache"), sPath);
		
		ReplaceString(sPath, sizeof(sPath), "*/", "sound/");
		AddFileToPrecache(sPath);
	}
	
	//*********************************************************************
	//*             	 PRECACHE OF ZOMBIE INFECT SOUNDS                  *
	//*********************************************************************
	
	// Precache of the sounds
	for (int i = 0; i < sizeof(InfectSound); i++)
	{
		Format(sPath, sizeof(sPath), "%s", InfectSound[i]);
		AddToStringTable(FindStringTable("soundprecache"), sPath);
		
		ReplaceString(sPath, sizeof(sPath), "*/", "sound/");
		AddFileToPrecache(sPath);
	}
	
	//*********************************************************************
	//*             	 	PRECACHE OF ROUND SOUNDS                 	  *
	//*********************************************************************
	
	// Precache of the sounds
	for (int i = 0; i < sizeof(RoundSound); i++)
	{
		Format(sPath, sizeof(sPath), "%s", RoundSound[i]);
		AddToStringTable(FindStringTable("soundprecache"), sPath);
		
		ReplaceString(sPath, sizeof(sPath), "*/", "sound/");
		AddFileToPrecache(sPath);
	}
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
	// Hook player events
	SDKHook(clientIndex, SDKHook_TraceAttack,	DamageOnTraceAttack);
	SDKHook(clientIndex, SDKHook_OnTakeDamage,	DamageOnTakeDamage);
}

/**
 * Hook: OnTraceAttack
 * Called right before the bullet enters a client.
 * 
 * @param victimIndex		The victim index.
 * @param attackerIndex		The attacker index.
 * @param inflicterIndex	The inflictor index.
 * @param damageAmount		The amount of damage inflicted.
 * @param damageBits		The type of damage inflicted.
 * @param ammoType			The ammo type of the attacker's weapon.
 * @param hitroupBox		The hitbox index.  
 * @param hitgroupIndex		The hitgroup index.  
 **/
public Action DamageOnTraceAttack(int victimIndex, int &attackerIndex, int &inflicterIndex, float &damageAmount, int &damageBits, int &ammoType, int hitroupBox, int hitgroupIndex)
{
	// If mode doesn't started yet, then stop trace
	if (!ZM_IsRoundStarted())
	{
		// Stop trace
		return Plugin_Handled;
	}

	// Verify that the clients are exists
	if (!IsPlayerExist(victimIndex) || !IsPlayerExist(attackerIndex))
	{
		// Stop trace
		return Plugin_Handled;
	}

	// If clients have same team, then stop trace
	if (GetClientTeam(victimIndex) == GetClientTeam(attackerIndex))
	{
		// Stop trace
		return Plugin_Handled;
	}

	// Allow trace
	return Plugin_Continue;
}
 
/**
 * Hook: OnTakeDamage
 * Called right before damage is done.
 * 
 * @param victimIndex		The victim index.
 * @param attackerIndex		The attacker index.
 * @param inflicterIndex	The inflicter index.
 * @param damageAmount		The amount of damage inflicted.
 * @param damageBits		The type of damage inflicted.
 **/
public Action DamageOnTakeDamage(int victimIndex, int &attackerIndex, int &inflicterIndex, float &damageAmount, int &damageBits)
{
	// If mode doesn't started yet, then stop trace
	if (!ZM_IsRoundStarted())
	{
		// Block damage
		return Plugin_Handled;
	}

	// Get classname of the inflictor
	char sClassname[32];
	GetEdictClassname(inflicterIndex, sClassname, sizeof(sClassname));

	// If entity is a trigger, then allow damage (Map is damaging client)
	if (StrContains(sClassname, "trigger") > -1)
	{
		// Allow damage
		return Plugin_Continue;
	}

	// If client is attacking himself, then stop
	if(victimIndex == attackerIndex)
	{
		// Block damage
		return Plugin_Handled;
	}

	// Verify that the victim is exist
	if (!IsPlayerExist(victimIndex))
	{
		// Block damage
		return Plugin_Handled;
	}
	
	//*********************************************************************
	//*            		  FIRE AND EXPLOSION DAMAGE           	  		  *
	//*********************************************************************

	// Client was damaged by 'explosion' or 'fire' or 'burn
	if (damageBits & DMG_CSGO_BLAST || damageBits & DMG_CSGO_BURN || damageBits & DMG_CSGO_FIRE)
    {
		// Block damage for human 
		if(!ZM_IsPlayerZombie(victimIndex)) damageAmount = 0.0;
	}
	

	//*********************************************************************
	//*            			FALLING DAMAGE           	  			      *
	//*********************************************************************
	
	// Client was damaged by 'falling' or 'drowning'
	else if(damageBits & DMG_CSGO_FALL || damageBits & DMG_CSGO_DROWN)
	{
		// Block damage for zombie 
		return ZM_IsPlayerZombie(victimIndex) ? Plugin_Handled : Plugin_Continue;
	}
	
	
	//*********************************************************************
	//*            			NEVERGIB DAMAGE           	  			      *
	//*********************************************************************
	
	// Client was damaged by 'bullet' or 'knife'
	else if (damageBits & DMG_CSGO_BULLET)
    {
		// Verify that the attacker is exist
		if (!IsPlayerExist(attackerIndex))
		{
			// Block damage
			return Plugin_Handled;
		}
		
		// If clients have same team, then stop trace
		if (GetClientTeam(victimIndex) == GetClientTeam(attackerIndex))
		{
			// Block damage
			return Plugin_Handled;
		}
		
		// Verify that the attacker is zombie 
		if(ZM_IsPlayerZombie(attackerIndex))
		{
			// Allow damage
			return Plugin_Continue;
		}
		// If the attacker is human
		else
		{
			// Emit damage sound
			EmitSoundToAll(DamageSound[GetRandomInt(0, 1)], victimIndex, SNDCHAN_STATIC, SNDLEVEL_NORMAL);
		
			// Apply knockback
			DamageOnClientKnockBack(victimIndex, attackerIndex, damageAmount);

			// Give money for applied damage
			DamageOnClientBonus(attackerIndex, damageAmount);
		}
	}
	
	// Apply fake damage
	return DamageOnClientFakeDamage(victimIndex, damageAmount);
}

/**
 * Hook: OnTakeDamage
 * Damage without pain shock.
 *
 * @param victimIndex		The victim index.
 * @param damageAmount		The amount of damage inflicted. 
 **/
Action DamageOnClientFakeDamage(int victimIndex, float damageAmount)
{
	// Get health
	int healthAmount = GetClientHealth(victimIndex);
	
	// Verify that the victim has a health
	if(healthAmount)
	{
		// Count the damage
		healthAmount -= RoundFloat(damageAmount);
		
		// If amount of damage to big, then stop
		if(healthAmount <= 0)
		{
			// Allow damage
			return Plugin_Changed;
		}
		else
		{
			// Apply fake damage
			SetEntProp(victimIndex, Prop_Send, "m_iHealth", healthAmount, 4);
			
			// Block damage
			return Plugin_Handled;
		}
	}
	else // Allow damage
		return Plugin_Changed;
}

 
/**
 * Hook: OnTakeDamage
 * Reward ammopacks for applied damage.
 *
 * @param attackerIndex		The attacker index.
 * @param damageAmount		The amount of damage inflicted. 
 **/
void DamageOnClientBonus(int attackerIndex, float damageAmount)
{
	// Increment total money
	SetEntProp(attackerIndex, Prop_Send, "m_iAccount", GetEntProp(attackerIndex, Prop_Send, "m_iAccount") + RoundFloat(damageAmount));
}

/** 
 * Hook: OnTakeDamage
 * Set velocity knockback for applied damage.
 *
 * @param victimIndex		The client index.
 * @param attackerIndex		The attacker index.
 * @param damageAmount		The amount of damage inflicted.
 * @param weaponID			The weapon id.
 **/
void DamageOnClientKnockBack(int victimIndex, int attackerIndex, float damageAmount)
{
	// Initialize vectors
	float flClientLoc[3];
	float flEyeAngle[3];
	float flAttackerLoc[3];
	float flVector[3];

	// Get victim's position
	GetClientAbsOrigin(victimIndex, flClientLoc);
	
	// Get attacker's position
	GetClientEyeAngles(attackerIndex, flEyeAngle);
	GetClientEyePosition(attackerIndex, flAttackerLoc);

	// Calculate knockback end-vector
	TR_TraceRayFilter(flAttackerLoc, flEyeAngle, MASK_ALL, RayType_Infinite, FilterPlayers);
	TR_GetEndPosition(flClientLoc);
	
	// Get vector from the given starting and ending points
	MakeVectorFromPoints(flAttackerLoc, flClientLoc, flVector);
	
	// Normalize the vector (equal magnitude at varying distances)
	NormalizeVector(flVector, flVector);

	// Apply the magnitude by scaling the vector
	ScaleVector(flVector, ZM_GetZombieClassKnockBack(ZM_GetClientZombieClass(victimIndex)) * damageAmount);

	// Push the player
	TeleportEntity(victimIndex, NULL_VECTOR, NULL_VECTOR, flVector);
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

	// Emit infect sound
	EmitSoundToAll(InfectSound[!ZM_GetZombieClassGender(ZM_GetClientZombieClass(clientIndex)) ? 0 : 1], clientIndex, SNDCHAN_STATIC, SNDLEVEL_NORMAL);
}

/**
 * @brief Called after a zombie round is started.
 **/
public void ZM_OnZombieModStarted(/*void*/)
{
	// i = index of the client
	for (int i = 1; i <= MaxClients; i++)
	{
		// Validate client
		if (IsPlayerExist(i, false))
		{
			// Emit sound
			EmitSoundToClient(i, RoundSound[GetRandomInt(0, 3)], SOUND_FROM_PLAYER, SNDCHAN_STATIC, SNDLEVEL_NORMAL);
			
			// Set translation target
			SetGlobalTransTarget(i);

			// Print translated phrase to client's hint
			PrintHintText(i, "%t", "Mode start");
		}
	}
}

/**
 * Event callback (player_death)
 * The player was died.
 * 
 * @param gEventHook      	The event handle.
 * @param gEventName        The name of the event.
 * @dontBroadcast   	    If true, event is broadcasted to all clients, false if not.
 **/
public Action EventPlayerDeath(Event gEventHook, const char[] gEventName, bool dontBroadcast)
{
	// Get real player index from event key
	int clientIndex = GetClientOfUserId(GetEventInt(gEventHook, "userid")); 

	#pragma unused clientIndex
	
	// Validate client
	if(!IsPlayerExist(clientIndex, false))
	{
		return;
	}
	
	// Verify that the client is zombie
	if(ZM_IsPlayerZombie(clientIndex))
	{
		// Emit death sound
		EmitSoundToAll(DeathSound[GetRandomInt(0, 1)], clientIndex, SNDCHAN_STATIC, SNDLEVEL_NORMAL);
	}
}

/**
 * Called when a sound is going to be emitted to one or more clients. NOTICE: all params can be overwritten to modify the default behaviour.
 *  
 * @param clients			Array of client's indexes.
 * @param numClients		Number of clients in the array (modify this value if you add/remove elements from the client array).
 * @param sSample			Sound file name relative to the "sounds" folder.
 * @param clientIndex		Entity emitting the sound.
 * @param iChannel			Channel emitting the sound.
 * @param flVolume			The sound volume.
 * @param iLevel			The sound level.
 * @param iPitch			The sound pitch.
 * @param iFrags			The sound flags.
 **/ 
public Action SoundsHook(int clients[MAXPLAYERS-1], int &numClients, char[] sSample, int &clientIndex, int &iChannel, float &flVolume, int &iLevel, int &iPitch, int &iFrags)
{
	// Validate client
	if(!IsPlayerExist(clientIndex))
	{
		return Plugin_Continue;
	}

	// Verify that the client is zombie
	if(ZM_IsPlayerZombie(clientIndex))
	{
		// If this footstep sounds, change its
		if(StrContains(sSample, "footsteps") != -1)
		{
			// Emit footsteps sound
			EmitSoundToAll(FootstepSound[!ZM_GetZombieClassGender(ZM_GetClientZombieClass(clientIndex)) ? GetRandomInt(0, 11) : GetRandomInt(12, 23)], clientIndex, SNDCHAN_STREAM, SNDLEVEL_LIBRARY);
			return Plugin_Changed; 
		}
	}
	
	// Allow sounds
	return Plugin_Continue;
}

/**
 * Trace filter.
 *  
 * @param nEntity			The entity index.
 * @param contentsMask		The contents mask.
 *
 * @return					True or false.
 **/
public bool FilterPlayers(int nEntity, int contentsMask)
{
	return !(1 <= nEntity <= MaxClients);
}