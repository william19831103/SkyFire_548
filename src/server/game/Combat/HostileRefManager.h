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

#ifndef SF_HOSTILEREFMANAGER
#define SF_HOSTILEREFMANAGER

#include "Common.h"
#include "RefManager.h"

class Unit;
class ThreatManager;
class HostileReference;
class SpellInfo;

//=================================================

class HostileRefManager : public RefManager<Unit, ThreatManager>
{
    private:
        Unit* iOwner;
    public:
        explicit HostileRefManager(Unit* owner) { iOwner = owner; }
        ~HostileRefManager();

        Unit* GetOwner() { return iOwner; }

        // send threat to all my hateres for the victim
        // The victim is hated than by them as well
        // use for buffs and healing threat functionality
        void threatAssist(Unit* victim, float baseThreat, SpellInfo const* threatSpell = NULL);

        void addTempThreat(float threat, bool apply);

        void addThreatPercent(int32 percent);

        // The references are not needed anymore
        // tell the source to remove them from the list and free the mem
        void deleteReferences();

        // Remove specific faction references
        void deleteReferencesForFaction(uint32 faction);

        HostileReference* getFirst() { return ((HostileReference*) RefManager<Unit, ThreatManager>::getFirst()); }

        void updateThreatTables();

        void setOnlineOfflineState(bool isOnline);

        // set state for one reference, defined by Unit
        void setOnlineOfflineState(Unit* creature, bool isOnline);

        // delete one reference, defined by Unit
        void deleteReference(Unit* creature);

        void UpdateVisibility();
};
//=================================================
#endif
