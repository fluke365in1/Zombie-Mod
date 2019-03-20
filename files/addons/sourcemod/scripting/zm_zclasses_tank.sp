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
#include <zombiemod>

#pragma newdecls required

/**
 * Record plugin info.
 **/
public Plugin ZombieClassTank =
{
	name        	= "[ZM] Zombie Class: Tank",
	author      	= "qubka (Nikita Ushakov)",
	description 	= "Addon of zombie classses",
	version     	= "1.0",
	url         	= "https://forums.alliedmods.net/showthread.php?t=290657"
}

/**
 * List of cvars.
 **/
enum ConVarList
{
	ConVar:CVAR_ZOMBIE_CLASS_NAME,
	ConVar:CVAR_ZOMBIE_CLASS_MODEL,
	ConVar:CVAR_ZOMBIE_CLASS_CLAW,
	ConVar:CVAR_ZOMBIE_CLASS_HEALTH,
	ConVar:CVAR_ZOMBIE_CLASS_SPEED,
	ConVar:CVAR_ZOMBIE_CLASS_GRAVITY,
	ConVar:CVAR_ZOMBIE_CLASS_KNOCKBACK,
	ConVar:CVAR_ZOMBIE_CLASS_FEMALE
};

// Array to store cvar data in
ConVar gCvarList[ConVarList];

// Initialize zombie class index
int gZombieTank; 
#pragma unused gZombieTank

/**
 * Plugin is loading.
 **/
public void OnPluginStart(/*void*/)
{
	// Load cvars
	gCvarList[CVAR_ZOMBIE_CLASS_NAME] 		= CreateConVar("zm_tank_name", 		"Tank");
	gCvarList[CVAR_ZOMBIE_CLASS_MODEL] 		= CreateConVar("zm_tank_model",	 	"models/player/vad36CSO2/sherif_fix.mdl");
	gCvarList[CVAR_ZOMBIE_CLASS_CLAW] 		= CreateConVar("zm_tank_claw", 		"models/zombie/normal/hand/hand_zombie_normal_fix.mdl");
	gCvarList[CVAR_ZOMBIE_CLASS_HEALTH] 	= CreateConVar("zm_tank_health", 	"8000");
	gCvarList[CVAR_ZOMBIE_CLASS_SPEED]		= CreateConVar("zm_tank_speed", 	"0.8");
	gCvarList[CVAR_ZOMBIE_CLASS_GRAVITY] 	= CreateConVar("zm_tank_gravity", 	"1.0");
	gCvarList[CVAR_ZOMBIE_CLASS_KNOCKBACK] 	= CreateConVar("zm_tank_knockback", "50.0");
	gCvarList[CVAR_ZOMBIE_CLASS_FEMALE] 	= CreateConVar("zm_tank_female", 	"0");

	// Create config
	AutoExecConfig(true, "zm_zombie_tank");
}

/**
 * The map is starting.
 **/
public void OnMapStart(/*void*/)
{
	// Get zombie class name
	char sZombieName[32];
	GetConVarString(gCvarList[CVAR_ZOMBIE_CLASS_NAME], sZombieName, sizeof(sZombieName)); 
	
	// Get zombie class models
	char sZombieModel[2][512];
	GetConVarString(gCvarList[CVAR_ZOMBIE_CLASS_MODEL], sZombieModel[0], sizeof(sZombieModel[])); 
	GetConVarString(gCvarList[CVAR_ZOMBIE_CLASS_CLAW],  sZombieModel[1], sizeof(sZombieModel[])); 
	
	// Initilizate zombie class
	gZombieTank = ZM_RegisterZombieClass(sZombieName, 
	sZombieModel[0], 
	sZombieModel[1], 
	GetConVarInt(gCvarList[CVAR_ZOMBIE_CLASS_HEALTH]), 
	GetConVarFloat(gCvarList[CVAR_ZOMBIE_CLASS_SPEED]), 
	GetConVarFloat(gCvarList[CVAR_ZOMBIE_CLASS_GRAVITY]), 
	GetConVarFloat(gCvarList[CVAR_ZOMBIE_CLASS_KNOCKBACK]), 
	GetConVarInt(gCvarList[CVAR_ZOMBIE_CLASS_FEMALE]));
	
	// Fast download
	AddFolderToPrecache("materials/models/player/vad36CSO2/");
	AddFolderToPrecache("materials/models/player/vad36CSO2/arm/");
	
	AddFolderToPrecache("materials/models/zombie/normal/hand/");
	AddFolderToPrecache("materials/models/zombie/normal/");
	AddFolderToPrecache("models/zombie/normal/hand/");
	AddFolderToPrecache("models/player/vad36CSO2/");
}