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

/* Script Data Start
SDName: Boss Commander Kolurg
SDAuthor: LordVanMartin
SD%Complete:
SDComment:  Only Alliance Heroic
SDCategory:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_BATTLE_SHOUT           = 31403,
    SPELL_CHARGE                 = 60067,
    SPELL_FRIGHTENING_SHOUT      = 19134,
    SPELL_WHIRLWIND_1            = 38619,
    SPELL_WHIRLWIND_2            = 38618

};

//not used
//Yell
#define SAY_AGGRO                                          -1576024
#define SAY_KILL                                           -1576025
#define SAY_DEATH                                          -1576026

class boss_commander_kolurg : public CreatureScript
{
public:
    boss_commander_kolurg() : CreatureScript("boss_commander_kolurg") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_commander_kolurgAI(creature);
    }

    struct boss_commander_kolurgAI : public ScriptedAI
    {
        boss_commander_kolurgAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE { }
        void EnterCombat(Unit* /*who*/) OVERRIDE { }
        void AttackStart(Unit* /*who*/) OVERRIDE { }
        void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
        void JustDied(Unit* /*killer*/)  OVERRIDE { }
    };

};

void AddSC_boss_commander_kolurg()
{
    new boss_commander_kolurg();
}
