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

#ifndef _WORKERTHREAD_H
#define _WORKERTHREAD_H

#include "Define.h"
#include <ace/Task.h>
#include <ace/Activation_Queue.h>

class MySQLConnection;

class DatabaseWorker : protected ACE_Task_Base
{
    public:
        DatabaseWorker(ACE_Activation_Queue* new_queue, MySQLConnection* con);

        ///- Inherited from ACE_Task_Base
        int svc();
        int wait() { return ACE_Task_Base::wait(); }

    private:
        ACE_Activation_Queue* m_queue;
        MySQLConnection* m_conn;
        DatabaseWorker(DatabaseWorker const& right) = delete;
        DatabaseWorker & operator=(DatabaseWorker const& right) = delete;
};

#endif
