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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_METAMOD_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_METAMOD_H_

/**
 * @file metamod.h
 * @brief Metamod module code header.
 */
 
 
/**
 * @brief Initialize metamod hooks.
 */
void MetamodInit();
 
/**
 * @brief Initializing metamod dependencies.
 */
void MetamodLoad();
 
/**
 * @brief Destroy metamod hooks.
 */
void MetamodPurge();

/**
 * @brief Validate metamod bintools dependency.
 *
 * @param pInterface		Pointer to interface being dropped.  This
 * 							pointer may be opaque, and it should not 
 *							be queried using SMInterface functions unless 
 *							it can be verified to match an existing 
 */
bool IsMetamodBintools(SMInterface *pInterface);
 
#endif // _INCLUDE_SOURCEMOD_EXTENSION_METAMOD_H_