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

/* ScriptData
SDName: Boss_Watchkeeper_Gargolmar
SD%Complete: 80
SDComment: Missing adds to heal him. Surge should be used on target furthest away, not random.
SDCategory: Hellfire Citadel, Hellfire Ramparts
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "hellfire_ramparts.h"

enum Says
{
    SAY_TAUNT              = 0,
    SAY_HEAL               = 1,
    SAY_SURGE              = 2,
    SAY_AGGRO              = 3,
    SAY_KILL               = 4,
    SAY_DIE                = 5
};

enum Spells
{
    SPELL_MORTAL_WOUND     = 30641,
    H_SPELL_MORTAL_WOUND   = 36814,
    SPELL_SURGE            = 34645,
    SPELL_RETALIATION      = 22857
};

enum Events
{
    EVENT_MORTAL_WOUND     = 1,
    EVENT_SURGE            = 2,
    EVENT_RETALIATION      = 3
};

class boss_watchkeeper_gargolmar : public CreatureScript
{
    public:
        boss_watchkeeper_gargolmar() : CreatureScript("boss_watchkeeper_gargolmar") { }

        struct boss_watchkeeper_gargolmarAI : public BossAI
        {
            boss_watchkeeper_gargolmarAI(Creature* creature) : BossAI(creature, DATA_WATCHKEEPER_GARGOLMAR) { }

            void Reset() OVERRIDE
            {
                hasTaunted    = false;
                yelledForHeal = false;
                retaliation   = false;
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_MORTAL_WOUND, 5000);
                events.ScheduleEvent(EVENT_SURGE, 4000);
                _EnterCombat();
            }

            void MoveInLineOfSight(Unit* who) OVERRIDE

            {
                if (!me->GetVictim() && me->CanCreatureAttack(who))
                {
                    if (!me->CanFly() && me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                        return;

                    float attackRadius = me->GetAttackDistance(who);
                    if (me->IsWithinDistInMap(who, attackRadius) && me->IsWithinLOSInMap(who))
                    {
                        //who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                        AttackStart(who);
                    }
                    else if (!hasTaunted && me->IsWithinDistInMap(who, 60.0f))
                    {
                        Talk(SAY_TAUNT);
                        hasTaunted = true;
                    }
                }
            }

            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                Talk(SAY_DIE);
                _JustDied();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MORTAL_WOUND:
                            DoCastVictim(SPELL_MORTAL_WOUND);
                            events.ScheduleEvent(EVENT_MORTAL_WOUND, urand (5000, 13000));
                            break;
                        case EVENT_SURGE:
                            Talk(SAY_SURGE);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_SURGE);
                            events.ScheduleEvent(EVENT_SURGE, urand (5000, 13000));
                            break;
                        case EVENT_RETALIATION:
                            DoCast(me, SPELL_RETALIATION);
                            events.ScheduleEvent(EVENT_RETALIATION, 30000);
                            break;
                        default:
                            break;
                    }
                }

                if (!retaliation)
                {
                    if (HealthBelowPct(20))
                    {
                        events.ScheduleEvent(EVENT_RETALIATION, 1000);
                        retaliation = true;
                    }
                }

                if (!yelledForHeal)
                {
                    if (HealthBelowPct(40))
                    {
                        Talk(SAY_HEAL);
                        yelledForHeal = true;
                    }
                }

                DoMeleeAttackIfReady();
            }

            private:
                bool hasTaunted;
                bool yelledForHeal;
                bool retaliation;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_watchkeeper_gargolmarAI(creature);
        }
};

void AddSC_boss_watchkeeper_gargolmar()
{
    new boss_watchkeeper_gargolmar();
}

