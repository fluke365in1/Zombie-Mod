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
#include <zombiemod>

#pragma newdecls required

/**
 * Record plugin info.
 **/
public Plugin ZombieMainMenuSystem =
{
	name        	= "[ZM] Addon: Menus",
	author      	= "qubka (Nikita Ushakov)", 	
	description 	= "Addon for adding menus",
	version     	= "1.0",
	url         	= "https://forums.alliedmods.net/showthread.php?t=290657"
}

/**
 * Main menu cases.
 **/
enum MainMenu
{
	MainMenu_ZombieClasses,		/** Zombie menu slot. */
	MainMenu_HumanClasses,		/** Human menu slot. */
	MainMenu_Unstuck			/** Unstuck menu slot. */
};

/**
 * Plugin is loading.
 **/
public void OnPluginStart(/*void*/)
{
	// Hook commands
	AddCommandListener(MenuCommandHook, "+lookatweapon");
	
	// Load translations phrases used by plugin
	LoadTranslations("zombiemod.pharses");
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

	// Validate spawn
	if(!antidotUse)
	{
		// Set translation target
		SetGlobalTransTarget(clientIndex);

		// Print translated phrase to client's hint
		PrintHintText(clientIndex, "%t", "Welcome Message");
	}
}

/**
 * Hook client command.
 *
 * @param clientIndex		The client index.
 * @param commandMsg		Command name, lower case. To get name as typed, use GetCmdArg() and specify argument 0.
 * @param iArguments		Argument count.
 **/
public Action MenuCommandHook(int clientIndex, const char[] commandMsg, int iArguments)
{
	// Create an main menu
	MenuMain(clientIndex);
	
	// Block command
	return Plugin_Handled;
}

/**
 * Create an main menu.
 *
 * @param clientIndex		The client index.
 **/
void MenuMain(int clientIndex)
{
	// Validate client 
	if(!IsPlayerExist(clientIndex, false))
	{
		return;
	}
	
	// Sets the language to target
	SetGlobalTransTarget(clientIndex);
	
	// Initialize menu
	Menu iMenu = CreateMenu(MenuMainSlots);

	// Set title
	SetMenuTitle(iMenu, "%t", "Menu Title");
	
	// Initialize char
	char sBuffer[128];

	// Show menu
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Choose zombieclass");
	AddMenuItem(iMenu, "1", sBuffer);
	
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Choose humanclass");
	AddMenuItem(iMenu, "2", sBuffer);
	
	FormatEx(sBuffer, sizeof(sBuffer), "%t", "Unstuck");
	AddMenuItem(iMenu, "3", sBuffer);

	// Set exit button
	SetMenuExitButton(iMenu, true);

	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT);
	DisplayMenu(iMenu, clientIndex, MENU_TIME_FOREVER); 
}

/**
 * Called when client selects option in the main menu, and handles it.
 *  
 * @param iMenu				The handle of the menu being used.
 * @param mAction			The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex		The client index.
 * @param mSlot				The slot index selected (starting from 0).
 **/ 
public int MenuMainSlots(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
{
	// Switch the menu action
	switch(mAction)
	{
		// Client hit 'Exit' button
		case MenuAction_End :
		{
			CloseHandle(iMenu);
		}

		// Client selected an option
		case MenuAction_Select :
		{
			// Select sub-menu
			MenuSubOpen(clientIndex, mSlot);
		}
	}
}

/**
 * Selected a sub menu.
 *
 * @param clientIndex		The client index.
 * @param mSlot				The slot index selected (starting from 0).
 **/
void MenuSubOpen(int clientIndex, int mSlot) 
{
	// Validate client
	if(!IsPlayerExist(clientIndex, false))
	{
		return;
	}

	// Initialize chars
	char sBuffer[128];
	char sName[64];
	char sInfo[32];

	// Create menu handle
	Menu iMenu;
	
	// Switch the menu slot
	switch(mSlot)
	{
		// '1' case
		case MainMenu_ZombieClasses :
		{
			// Set menu handle
			iMenu = CreateMenu(MenuZombieSlots);

			// Add formatted options to menu
			SetMenuTitle(iMenu, "%t", "Choose zombieclass");

			// i = Zombie class number
			for(int i = 0; i < ZM_GetNumberZombieClass(); i++)
			{
				// Get zombie class name
				ZM_GetZombieClassName(i, sName, sizeof(sName));

				// Formatex some chars for showing in menu
				FormatEx(sBuffer, sizeof(sBuffer), sName);

				// Show option
				IntToString(i, sInfo, sizeof(sInfo));
				AddMenuItem(iMenu, sInfo, sBuffer);
			}
		}
		
		// '2' case
		case MainMenu_HumanClasses :
		{
			// Set menu handle
			iMenu = CreateMenu(MenuHumanSlots);

			// Add formatted options to menu
			SetMenuTitle(iMenu, "%t", "Choose humanclass");

			// i = Human class number
			for(int i = 0; i < ZM_GetNumberHumanClass(); i++)
			{
				// Get human class name
				ZM_GetHumanClassName(i, sName, sizeof(sName));

				// Formatex some chars for showing in menu
				FormatEx(sBuffer, sizeof(sBuffer), sName);

				// Show option
				IntToString(i, sInfo, sizeof(sInfo));
				AddMenuItem(iMenu, sInfo, sBuffer);
			}
		}
		
		// '3' case
		case MainMenu_Unstuck :
		{
			if(IsPlayerExist(clientIndex) FakeClientCommandEx(clientIndex, "zstuck");
			return;
		}
	}

	// Set exit and back button
	SetMenuExitBackButton(iMenu, true);
	
	// Set options and display it
	SetMenuOptionFlags(iMenu, MENUFLAG_BUTTON_EXIT|MENUFLAG_BUTTON_EXITBACK);
	DisplayMenu(iMenu, clientIndex, MENU_TIME_FOREVER);
}

/**
 * Called when client selects option in the zombie class menu, and handles it.
 *  
 * @param iMenu				The handle of the menu being used.
 * @param mAction			The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex		The client index.
 * @param mSlot				The slot index selected (starting from 0).
 **/ 
public int MenuZombieSlots(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
{
	// Switch the menu action
	switch(mAction)
	{
		// Client hit 'Exit' button
		case MenuAction_End :
		{
			CloseHandle(iMenu);
		}
		
		// Client hit 'Back' button
		case MenuAction_Cancel :
		{
			if (mSlot == MenuCancel_ExitBack)
			{
				// Open main menu back
				MenuMain(clientIndex);
			}
		}

		// Client selected an option
		case MenuAction_Select :
		{
			// Validate client
			if(!IsPlayerExist(clientIndex, false))
			{
				return;
			}
			
			// Initialize char
			char sZombieName[32];

			// Get ID of zombie class
			GetMenuItem(iMenu, mSlot, sZombieName, sizeof(sZombieName));
			int iClass = StringToInt(sZombieName);
			
			// Set next zombie class
			ZM_SetClientZombieClass(clientIndex, iClass);

			// Get zombie name
			ZM_GetZombieClassName(iClass, sZombieName, sizeof(sZombieName));

			// Show class info
			char sMessage[128];
			FormatEx(sMessage, sizeof(sMessage), " \x03[ZM] \x01Zombie: \x02[%s] \x01HP: \x02[%i] \x01Speed: \x02[%.1f] \x01Gravity: \x02[%.1f]", sZombieName, ZM_GetZombieClassHealth(iClass), ZM_GetZombieClassSpeed(iClass), ZM_GetZombieClassGravity(iClass));	
			PrintToChat(clientIndex, sMessage);
		}
	}
}

/**
 * Called when client selects option in the human class menu, and handles it.
 *  
 * @param iMenu				The handle of the menu being used.
 * @param mAction			The action done on the menu (see menus.inc, enum MenuAction).
 * @param clientIndex		The client index.
 * @param mSlot				The slot index selected (starting from 0).
 **/ 
public int MenuHumanSlots(Menu iMenu, MenuAction mAction, int clientIndex, int mSlot)
{
	// Switch the menu action
	switch(mAction)
	{
		// Client hit 'Exit' button
		case MenuAction_End :
		{
			CloseHandle(iMenu);
		}
		
		// Client hit 'Back' button
		case MenuAction_Cancel :
		{
			if (mSlot == MenuCancel_ExitBack)
			{
				// Open main menu back
				MenuMain(clientIndex);
			}
		}

		// Client selected an option
		case MenuAction_Select :
		{
			// Validate client
			if(!IsPlayerExist(clientIndex, false))
			{
				return;
			}
			
			// Initialize char
			char sHumanName[32];

			// Get ID of human class
			GetMenuItem(iMenu, mSlot, sHumanName, sizeof(sHumanName));
			int iClass = StringToInt(sHumanName);
			
			// Set next human class
			ZM_SetClientHumanClass(clientIndex, iClass);

			// Get human name
			ZM_GetHumanClassName(iClass, sHumanName, sizeof(sHumanName));

			// Show class info
			char sMessage[128];
			FormatEx(sMessage, sizeof(sMessage), " \x03[ZM] \x01Human: \x02[%s] \x01HP: \x02[%i] \x01Speed: \x02[%.1f] \x01Gravity: \x02[%.1f]", sHumanName, ZM_GetHumanClassHealth(iClass), ZM_GetHumanClassSpeed(iClass), ZM_GetHumanClassGravity(iClass));	
			PrintToChat(clientIndex, sMessage);
		}
	}
}