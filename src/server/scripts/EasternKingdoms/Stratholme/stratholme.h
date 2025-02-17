/*
 * Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2022 MaNGOS <https://www.getmangos.eu/>
 * Copyright (C) 2006-2014 ScriptDev2 <https://github.com/scriptdev2/scriptdev2/>
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

#ifndef DEF_STRATHOLME_H
#define DEF_STRATHOLME_H

enum DataTypes
{
    TYPE_BARON_RUN                      = 1,
    TYPE_BARONESS                       = 2,
    TYPE_NERUB                          = 3,
    TYPE_PALLID                         = 4,
    TYPE_RAMSTEIN                       = 5,
    TYPE_BARON                          = 6,

    TYPE_MALOWN                         = 7,

    DATA_BARON                          = 10,
    DATA_YSIDA_TRIGGER                  = 11,

    TYPE_SH_QUEST                       = 20,
    TYPE_SH_CATHELA                     = 21,
    TYPE_SH_GREGOR                      = 22,
    TYPE_SH_NEMAS                       = 23,
    TYPE_SH_VICAR                       = 24,
    TYPE_SH_AELMAR                      = 25
};

enum CreatureIds
{
    NPC_CRYSTAL                         = 10415, // ziggurat crystal
    NPC_BARON                           = 10440, // ziggurat crystal
    NPC_YSIDA_TRIGGER                   = 16100, // ziggurat crystal

    NPC_RAMSTEIN                        = 10439,
    NPC_ABOM_BILE                       = 10416,
    NPC_ABOM_VENOM                      = 10417,
    NPC_BLACK_GUARD                     = 10394,
    NPC_YSIDA                           = 16031,
};

enum GameobjectIds
{
    GO_DOOR_HALAZZI                     = 186303,
    GO_SERVICE_ENTRANCE                 = 175368,
    GO_GAUNTLET_GATE1                   = 175357,
    GO_ZIGGURAT1                        = 175380,  // baroness
    GO_ZIGGURAT2                        = 175379,  // nerub'enkan
    GO_ZIGGURAT3                        = 175381,  // maleki
    GO_ZIGGURAT4                        = 175405,  // rammstein
    GO_ZIGGURAT5                        = 175796,  // baron
    GO_PORT_GAUNTLET                    = 175374,  // port from gauntlet to slaugther
    GO_PORT_SLAUGTHER                   = 175373,  // port at slaugther
    GO_PORT_ELDERS                      = 175377   // port at elders square
};

enum QuestIds
{
    QUEST_DEAD_MAN_PLEA                 = 8945
};

enum SpellIds
{
    SPELL_BARON_ULTIMATUM               = 27861
};

#endif

