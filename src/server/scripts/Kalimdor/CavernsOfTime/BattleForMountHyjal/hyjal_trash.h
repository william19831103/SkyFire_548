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

#ifndef SC_HYJAL_TRASH_AI_H
#define SC_HYJAL_TRASH_AI_H

#include "hyjal.h"
#include "ScriptedEscortAI.h"

#define MINRAIDDAMAGE  700000//minimal damage before trash can drop loot and reputation, resets if faction leader dies

struct hyjal_trashAI : public npc_escortAI
{
    hyjal_trashAI(Creature* creature);

    void UpdateAI(uint32 diff);

    void JustDied(Unit* /*killer*/);

    void DamageTaken(Unit* done_by, uint32 &damage);

    public:
        InstanceScript* instance;
        bool IsEvent;
        uint32 Delay;
        uint32 LastOverronPos;
        bool IsOverrun;
        bool SetupOverrun;
        uint32 OverrunType;
        uint8 faction;
        bool useFlyPath;
        uint32 damageTaken;
        float DummyTarget[3];

    //private:
};
#endif
