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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "nexus.h"

enum Spells
{
    SPELL_ICE_NOVA                                = 47772,
    H_SPELL_ICE_NOVA                              = 56935,
    SPELL_FIREBOMB                                = 47773,
    H_SPELL_FIREBOMB                              = 56934,
    SPELL_GRAVITY_WELL                            = 47756,
    SPELL_TELESTRA_BACK                           = 47714,

    SPELL_FIRE_MAGUS_VISUAL                       = 47705,
    SPELL_FROST_MAGUS_VISUAL                      = 47706,
    SPELL_ARCANE_MAGUS_VISUAL                     = 47704
};

enum Creatures
{
    NPC_FIRE_MAGUS                                = 26928,
    NPC_FROST_MAGUS                               = 26930,
    NPC_ARCANE_MAGUS                              = 26929
};

enum Yells
{
    SAY_AGGRO                                     = 0,
    SAY_KILL                                      = 1,
    SAY_DEATH                                     = 2,
    SAY_MERGE                                     = 3,
    SAY_SPLIT                                     = 4
};

enum Misc
{
    ACTION_MAGUS_DEAD                             = 1,
    DATA_SPLIT_PERSONALITY                        = 2
};

const Position  CenterOfRoom = {504.80f, 89.07f, -16.12f, 6.27f};

class boss_magus_telestra : public CreatureScript
{
public:
    boss_magus_telestra() : CreatureScript("boss_magus_telestra") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_magus_telestraAI(creature);
    }

    struct boss_magus_telestraAI : public ScriptedAI
    {
        boss_magus_telestraAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint64 uiFireMagusGUID;
        uint64 uiFrostMagusGUID;
        uint64 uiArcaneMagusGUID;

        bool bFireMagusDead;
        bool bFrostMagusDead;
        bool bArcaneMagusDead;
        bool bIsWaitingToAppear;

        uint32 uiIsWaitingToAppearTimer;
        uint32 uiIceNovaTimer;
        uint32 uiFireBombTimer;
        uint32 uiGravityWellTimer;
        uint32 uiCooldown;

        uint8 Phase;
        uint8 splitPersonality;
        time_t time[3];

        void Reset() OVERRIDE
        {
            Phase = 0;
            //These times are probably wrong
            uiIceNovaTimer =  7*IN_MILLISECONDS;
            uiFireBombTimer =  0;
            uiGravityWellTimer = 15*IN_MILLISECONDS;
            uiCooldown = 0;

            uiFireMagusGUID = 0;
            uiFrostMagusGUID = 0;
            uiArcaneMagusGUID = 0;

            for (uint8 n = 0; n < 3; ++n)
                time[n] = 0;

            splitPersonality = 0;
            bIsWaitingToAppear = false;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetVisible(true);

            if (instance)
                instance->SetData(DATA_MAGUS_TELESTRA_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);

            if (instance)
                instance->SetData(DATA_MAGUS_TELESTRA_EVENT, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetData(DATA_MAGUS_TELESTRA_EVENT, DONE);
        }

        void KilledUnit(Unit* /*victim*/) OVERRIDE
        {
            Talk(SAY_KILL);
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_MAGUS_DEAD)
            {
                uint8 i = 0;
                while (time[i] != 0)
                    ++i;

                time[i] = sWorld->GetGameTime();
                if (i == 2 && (time[2] - time[1] < 5) && (time[1] - time[0] < 5))
                    ++splitPersonality;
            }
        }

        uint32 GetData(uint32 type) const OVERRIDE
        {
            if (type == DATA_SPLIT_PERSONALITY)
                return splitPersonality;

            return 0;
        }

        uint64 SplitPersonality(uint32 entry)
        {
            if (Creature* Summoned = me->SummonCreature(entry, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1*IN_MILLISECONDS))
            {
                switch (entry)
                {
                    case NPC_FIRE_MAGUS:
                    {
                        Summoned->CastSpell(Summoned, SPELL_FIRE_MAGUS_VISUAL, false);
                        break;
                    }
                    case NPC_FROST_MAGUS:
                    {
                        Summoned->CastSpell(Summoned, SPELL_FROST_MAGUS_VISUAL, false);
                        break;
                    }
                    case NPC_ARCANE_MAGUS:
                    {
                        Summoned->CastSpell(Summoned, SPELL_ARCANE_MAGUS_VISUAL, false);
                        break;
                    }
                }
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    Summoned->AI()->AttackStart(target);
                return Summoned->GetGUID();
            }
            return 0;
        }

        void SummonedCreatureDespawn(Creature* summon) OVERRIDE
        {
            if (summon->IsAlive())
                return;

            if (summon->GetGUID() == uiFireMagusGUID)
            {
                me->AI()->DoAction(ACTION_MAGUS_DEAD);
                bFireMagusDead = true;
            }
            else if (summon->GetGUID() == uiFrostMagusGUID)
            {
                me->AI()->DoAction(ACTION_MAGUS_DEAD);
                bFrostMagusDead = true;
            }
            else if (summon->GetGUID() == uiArcaneMagusGUID)
            {
                me->AI()->DoAction(ACTION_MAGUS_DEAD);
                bArcaneMagusDead = true;
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (bIsWaitingToAppear)
            {
                me->StopMoving();
                me->AttackStop();
                if (uiIsWaitingToAppearTimer <= diff)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    bIsWaitingToAppear = false;
                } else uiIsWaitingToAppearTimer -= diff;
                return;
            }

            if ((Phase == 1) ||(Phase == 3))
            {
                if (bFireMagusDead && bFrostMagusDead && bArcaneMagusDead)
                {
                    for (uint8 n = 0; n < 3; ++n)
                        time[n] = 0;
                    me->GetMotionMaster()->Clear();
                    me->SetPosition(CenterOfRoom.GetPositionX(), CenterOfRoom.GetPositionY(), CenterOfRoom.GetPositionZ(), CenterOfRoom.GetOrientation());
                    DoCast(me, SPELL_TELESTRA_BACK);
                    me->SetVisible(true);
                    if (Phase == 1)
                        Phase = 2;
                    if (Phase == 3)
                        Phase = 4;
                    uiFireMagusGUID = 0;
                    uiFrostMagusGUID = 0;
                    uiArcaneMagusGUID = 0;
                    bIsWaitingToAppear = true;
                    uiIsWaitingToAppearTimer = 4*IN_MILLISECONDS;
                    Talk(SAY_MERGE);
                }
                else
                    return;
            }

            if ((Phase == 0) && HealthBelowPct(50))
            {
                Phase = 1;
                me->CastStop();
                me->RemoveAllAuras();
                me->SetVisible(false);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                uiFireMagusGUID = SplitPersonality(NPC_FIRE_MAGUS);
                uiFrostMagusGUID = SplitPersonality(NPC_FROST_MAGUS);
                uiArcaneMagusGUID = SplitPersonality(NPC_ARCANE_MAGUS);
                bFireMagusDead = false;
                bFrostMagusDead = false;
                bArcaneMagusDead = false;
                Talk(SAY_SPLIT);
                return;
            }

            if (IsHeroic() && (Phase == 2) && HealthBelowPct(10))
            {
                Phase = 3;
                me->CastStop();
                me->RemoveAllAuras();
                me->SetVisible(false);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                uiFireMagusGUID = SplitPersonality(NPC_FIRE_MAGUS);
                uiFrostMagusGUID = SplitPersonality(NPC_FROST_MAGUS);
                uiArcaneMagusGUID = SplitPersonality(NPC_ARCANE_MAGUS);
                bFireMagusDead = false;
                bFrostMagusDead = false;
                bArcaneMagusDead = false;
                Talk(SAY_SPLIT);
                return;
            }

            if (uiCooldown)
            {
                if (uiCooldown <= diff)
                    uiCooldown = 0;
                else
                {
                    uiCooldown -= diff;
                    return;
                }
            }

            if (uiIceNovaTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_ICE_NOVA, false);
                    uiCooldown = 1500;
                }
                uiIceNovaTimer = 15*IN_MILLISECONDS;
            } else uiIceNovaTimer -= diff;

            if (uiGravityWellTimer <= diff)
            {
                if (Unit* target = me->GetVictim())
                {
                    DoCast(target, SPELL_GRAVITY_WELL);
                    uiCooldown = 6*IN_MILLISECONDS;
                }
                uiGravityWellTimer = 15*IN_MILLISECONDS;
            } else uiGravityWellTimer -= diff;

            if (uiFireBombTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_FIREBOMB, false);
                    uiCooldown = 2*IN_MILLISECONDS;
                }
                uiFireBombTimer = 2*IN_MILLISECONDS;
            } else uiFireBombTimer -=diff;

            DoMeleeAttackIfReady();
        }
    };
};

class achievement_split_personality : public AchievementCriteriaScript
{
    public:
        achievement_split_personality() : AchievementCriteriaScript("achievement_split_personality") { }

        bool OnCheck(Player* /*player*/, Unit* target) OVERRIDE
        {
            if (!target)
                return false;

            if (Creature* Telestra = target->ToCreature())
                if (Telestra->AI()->GetData(DATA_SPLIT_PERSONALITY) == 2)
                    return true;

            return false;
        }
};

void AddSC_boss_magus_telestra()
{
    new boss_magus_telestra();
    new achievement_split_personality();
}
