/*
 * Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2022 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
* @file cs_deserter.cpp
* @brief .deserter related commands
*
* This file contains the CommandScripts for all deserter sub-commands
*/

#include "Chat.h"
#include "Player.h"
#include "Language.h"
#include "ScriptMgr.h"
#include "SpellAuras.h"

enum Spells
{
    LFG_SPELL_DUNGEON_DESERTER = 71041,
    BG_SPELL_DESERTER = 26013
};

class deserter_commandscript : public CommandScript
{
public:
    deserter_commandscript() : CommandScript("deserter_commandscript") { }

    /**
    * @brief Returns the command structure for the system.
    */

    std::vector<ChatCommand> GetCommands() const OVERRIDE
    {
        static std::vector<ChatCommand> deserterInstanceCommandTable =
        {
            { "add",      rbac::RBAC_PERM_COMMAND_DESERTER_INSTANCE_ADD,    false, &HandleDeserterInstanceAdd,    "", },
            { "remove",   rbac::RBAC_PERM_COMMAND_DESERTER_INSTANCE_REMOVE, false, &HandleDeserterInstanceRemove, "", },
        };
        static std::vector<ChatCommand> deserterBGCommandTable =
        {
            { "add",      rbac::RBAC_PERM_COMMAND_DESERTER_BG_ADD,    false, &HandleDeserterBGAdd,    "", },
            { "remove",   rbac::RBAC_PERM_COMMAND_DESERTER_BG_REMOVE, false, &HandleDeserterBGRemove, "", },
        };

        static std::vector<ChatCommand> deserterCommandTable =
        {
            { "instance", rbac::RBAC_PERM_COMMAND_DESERTER_INSTANCE, false, NULL, "", deserterInstanceCommandTable },
            { "bg",       rbac::RBAC_PERM_COMMAND_DESERTER_BG,       false, NULL, "", deserterBGCommandTable },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "deserter", rbac::RBAC_PERM_COMMAND_DESERTER, false, NULL, "", deserterCommandTable },
        };
        return commandTable;
    }

    /**
    * @brief Applies the Deserter Debuff to a player
    *
    * This function applies a Deserter Debuff of the given type (Instance or BG) to the
    * selected player, with the provided duration in seconds.
    *
    * @param handler The ChatHandler, passed by the system.
    * @param args The provided duration in seconds.
    * @param isInstance provided by the relaying functions, so we don't have
    * to write that much code :)
    *
    * @return true if everything was correct, false if an error occured.
    *
    * Example Usage:
    * @code
    * .deserter instance add 3600 (one hour)
    * -or-
    * .deserter bg add 3600 (one hour)
    * @endcode
    */
    static bool HandleDeserterAdd(ChatHandler* handler, char const* args, bool isInstance)
    {
        if (!*args)
            return false;

        Player* player = handler->getSelectedPlayer();
        if (!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }
        char* timeStr = strtok((char*)args, " ");
        if (!timeStr)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }
        uint32 time = atoi(timeStr);

        if (!time)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }

        Aura* aura = player->AddAura(isInstance ? LFG_SPELL_DUNGEON_DESERTER : BG_SPELL_DESERTER, player);

        if (!aura)
        {
            handler->SendSysMessage(LANG_BAD_VALUE);
            handler->SetSentErrorMessage(true);
            return false;
        }
        aura->SetDuration(time * IN_MILLISECONDS);

        return true;
    }

    /**
    * @brief Removes the Deserter Debuff from a player
    *
    * This function removes a Deserter Debuff of the given type (Instance or BG) from the
    * selected player.
    *
    * @param handler The ChatHandler, passed by the system.
    * @param args Should be nothing.
    * @param isInstance provided by the relaying functions, so we don't have
    * to write that much code :)
    *
    * @return true if everything was correct, false if an error occured.
    *
    * Example Usage:
    * @code
    * .deserter instance remove
    * -or-
    * .deserter bg remove
    * @endcode
    */
    static bool HandleDeserterRemove(ChatHandler* handler, char const* /*args*/, bool isInstance)
    {
        Player* player = handler->getSelectedPlayer();
        if (!player)
        {
            handler->SendSysMessage(LANG_NO_CHAR_SELECTED);
            handler->SetSentErrorMessage(true);
            return false;
        }

        player->RemoveAura(isInstance ? LFG_SPELL_DUNGEON_DESERTER : BG_SPELL_DESERTER);

        return true;
    }

    /// @sa HandleDeserterAdd()
    static bool HandleDeserterInstanceAdd(ChatHandler* handler, char const* args)
    {
        return HandleDeserterAdd(handler, args, true);
    }

    /// @sa HandleDeserterAdd()
    static bool HandleDeserterBGAdd(ChatHandler* handler, char const* args)
    {
        return HandleDeserterAdd(handler, args, false);
    }

    /// @sa HandleDeserterRemove()
    static bool HandleDeserterInstanceRemove(ChatHandler* handler, char const* args)
    {
        return HandleDeserterRemove(handler, args, true);
    }

    /// @sa HandleDeserterRemove()
    static bool HandleDeserterBGRemove(ChatHandler* handler, char const* args)
    {
        return HandleDeserterRemove(handler, args, false);
    }
};

void AddSC_deserter_commandscript()
{
    new deserter_commandscript();
}
