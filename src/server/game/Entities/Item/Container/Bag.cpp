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

#include "Common.h"
#include "ObjectMgr.h"
#include "DatabaseEnv.h"

#include "Bag.h"
#include "Log.h"
#include "UpdateData.h"
#include "Player.h"

Bag::Bag(): Item()
{
    m_objectType |= TYPEMASK_CONTAINER;
    m_objectTypeId = TypeID::TYPEID_CONTAINER;

    m_valuesCount = CONTAINER_END;

    memset(m_bagslot, 0, sizeof(Item*) * MAX_BAG_SIZE);
}

Bag::~Bag()
{
    for (uint8 i = 0; i < MAX_BAG_SIZE; ++i)
        if (Item* item = m_bagslot[i])
        {
            if (item->IsInWorld())
            {
                SF_LOG_FATAL("entities.player.items", "Item %u (slot %u, bag slot %u) in bag %u (slot %u, bag slot %u, m_bagslot %u) is to be deleted but is still in world.",
                    item->GetEntry(), (uint32)item->GetSlot(), (uint32)item->GetBagSlot(),
                    GetEntry(), (uint32)GetSlot(), (uint32)GetBagSlot(), (uint32)i);
                item->RemoveFromWorld();
            }
            delete m_bagslot[i];
        }
}

void Bag::AddToWorld()
{
    Item::AddToWorld();

    for (uint32 i = 0; i < GetBagSize(); ++i)
        if (m_bagslot[i])
            m_bagslot[i]->AddToWorld();
}

void Bag::RemoveFromWorld()
{
    for (uint32 i = 0; i < GetBagSize(); ++i)
        if (m_bagslot[i])
            m_bagslot[i]->RemoveFromWorld();

    Item::RemoveFromWorld();
}

bool Bag::Create(uint32 guidlow, uint32 itemid, Player const* owner)
{
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemid);

    if (!itemProto || itemProto->ContainerSlots > MAX_BAG_SIZE)
        return false;

    Object::_Create(guidlow, 0, HIGHGUID_CONTAINER);

    SetEntry(itemid);
    SetObjectScale(1.0f);

    SetUInt64Value(ITEM_FIELD_OWNER, owner ? owner->GetGUID() : 0);
    SetUInt64Value(ITEM_FIELD_CONTAINED_IN, owner ? owner->GetGUID() : 0);

    SetUInt32Value(ITEM_FIELD_MAX_DURABILITY, itemProto->MaxDurability);
    SetUInt32Value(ITEM_FIELD_DURABILITY, itemProto->MaxDurability);
    SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);

    // Setting the number of Slots the Container has
    SetUInt32Value(CONTAINER_FIELD_NUM_SLOTS, itemProto->ContainerSlots);

    // Cleaning 20 slots
    for (uint8 i = 0; i < MAX_BAG_SIZE; ++i)
    {
        SetUInt64Value(CONTAINER_FIELD_SLOTS + (i*2), 0);
        m_bagslot[i] = NULL;
    }

    return true;
}

void Bag::SaveToDB(SQLTransaction& trans)
{
    Item::SaveToDB(trans);
}

bool Bag::LoadFromDB(uint32 guid, uint64 owner_guid, Field* fields, uint32 entry)
{
    if (!Item::LoadFromDB(guid, owner_guid, fields, entry))
        return false;

    ItemTemplate const* itemProto = GetTemplate(); // checked in Item::LoadFromDB
    SetUInt32Value(CONTAINER_FIELD_NUM_SLOTS, itemProto->ContainerSlots);
    // cleanup bag content related item value fields (its will be filled correctly from `character_inventory`)
    for (uint8 i = 0; i < MAX_BAG_SIZE; ++i)
    {
        SetUInt64Value(CONTAINER_FIELD_SLOTS + (i*2), 0);
        delete m_bagslot[i];
        m_bagslot[i] = NULL;
    }

    return true;
}

void Bag::DeleteFromDB(SQLTransaction& trans)
{
    for (uint8 i = 0; i < MAX_BAG_SIZE; ++i)
        if (m_bagslot[i])
            m_bagslot[i]->DeleteFromDB(trans);

    Item::DeleteFromDB(trans);
}

uint32 Bag::GetFreeSlots() const
{
    uint32 slots = 0;
    for (uint32 i=0; i < GetBagSize(); ++i)
        if (!m_bagslot[i])
            ++slots;

    return slots;
}

void Bag::RemoveItem(uint8 slot, bool /*update*/)
{
    ASSERT(slot < MAX_BAG_SIZE);

    if (m_bagslot[slot])
        m_bagslot[slot]->SetContainer(NULL);

    m_bagslot[slot] = NULL;
    SetUInt64Value(CONTAINER_FIELD_SLOTS + (slot * 2), 0);
}

void Bag::StoreItem(uint8 slot, Item* pItem, bool /*update*/)
{
    ASSERT(slot < MAX_BAG_SIZE);

    if (pItem && pItem->GetGUID() != this->GetGUID())
    {
        m_bagslot[slot] = pItem;
        SetUInt64Value(CONTAINER_FIELD_SLOTS + (slot * 2), pItem->GetGUID());
        pItem->SetUInt64Value(ITEM_FIELD_CONTAINED_IN, GetGUID());
        pItem->SetUInt64Value(ITEM_FIELD_OWNER, GetOwnerGUID());
        pItem->SetContainer(this);
        pItem->SetSlot(slot);
    }
}

void Bag::BuildCreateUpdateBlockForPlayer(UpdateData* data, Player* target) const
{
    Item::BuildCreateUpdateBlockForPlayer(data, target);

    for (uint32 i = 0; i < GetBagSize(); ++i)
        if (m_bagslot[i])
            m_bagslot[i]->BuildCreateUpdateBlockForPlayer(data, target);
}

// If the bag is empty returns true
bool Bag::IsEmpty() const
{
    for (uint32 i = 0; i < GetBagSize(); ++i)
        if (m_bagslot[i])
            return false;

    return true;
}

uint32 Bag::GetItemCount(uint32 item, Item* eItem) const
{
    Item* pItem;
    uint32 count = 0;
    for (uint32 i=0; i < GetBagSize(); ++i)
    {
        pItem = m_bagslot[i];
        if (pItem && pItem != eItem && pItem->GetEntry() == item)
            count += pItem->GetCount();
    }

    if (eItem && eItem->GetTemplate()->GemProperties)
    {
        for (uint32 i=0; i < GetBagSize(); ++i)
        {
            pItem = m_bagslot[i];
            if (pItem && pItem != eItem && pItem->GetTemplate()->Socket[0].Color)
                count += pItem->GetGemCountWithID(item);
        }
    }

    return count;
}

uint32 Bag::GetItemCountWithLimitCategory(uint32 limitCategory, Item* skipItem) const
{
    uint32 count = 0;
    for (uint32 i = 0; i < GetBagSize(); ++i)
        if (Item* pItem = m_bagslot[i])
            if (pItem != skipItem)
                if (ItemTemplate const* pProto = pItem->GetTemplate())
                    if (pProto->ItemLimitCategory == limitCategory)
                        count += m_bagslot[i]->GetCount();

    return count;
}

uint8 Bag::GetSlotByItemGUID(uint64 guid) const
{
    for (uint32 i = 0; i < GetBagSize(); ++i)
        if (m_bagslot[i] != 0)
            if (m_bagslot[i]->GetGUID() == guid)
                return i;

    return NULL_SLOT;
}

Item* Bag::GetItemByPos(uint8 slot) const
{
    if (slot < GetBagSize())
        return m_bagslot[slot];

    return NULL;
}
