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

#ifndef DEF_ULDAMAN_H
#define DEF_ULDAMAN_H

#define MAX_ENCOUNTER                   3

enum DataTypes
{
    DATA_ALTAR_DOORS                    = 1,
    DATA_ANCIENT_DOOR                   = 2,
    DATA_IRONAYA_DOOR                   = 3,
    DATA_STONE_KEEPERS                  = 4,
    DATA_MINIONS                        = 5,
    DATA_IRONAYA_SEAL                   = 6,
};

enum GameObjectIds
{
    GO_ARCHAEDAS_TEMPLE_DOOR            = 141869,
    GO_ALTAR_OF_THE_KEEPER_TEMPLE_DOOR  = 124367,
    GO_ANCIENT_VAULT_DOOR               = 124369,
    GO_IRONAYA_SEAL_DOOR                = 124372,
    GO_KEYSTONE                         = 124371,
};

#endif
