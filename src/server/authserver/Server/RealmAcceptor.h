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

#ifndef SF_REALMACCEPTOR_H
#define SF_REALMACCEPTOR_H

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include "RealmSocket.h"
#include "AuthSocket.h"
#include "Log.h"

class RealmAcceptor : public ACE_Acceptor<RealmSocket, ACE_SOCK_Acceptor>
{
public:
    RealmAcceptor(void) { }
    virtual ~RealmAcceptor(void)
    {
        if (reactor())
            reactor()->cancel_timer(this, 1);
    }

protected:
    virtual int make_svc_handler(RealmSocket* &sh)
    {
        if (sh == 0)
            ACE_NEW_RETURN(sh, RealmSocket, -1);

        sh->reactor(reactor());
        sh->set_session(new AuthSocket(*sh));
        return 0;
    }

    virtual int handle_timeout(const ACE_Time_Value& /*current_time*/, const void* /*act = 0*/)
    {
        SF_LOG_DEBUG("server.authserver", "Resuming acceptor");
        reactor()->cancel_timer(this, 1);
        return reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
    }

    virtual int handle_accept_error(void)
    {
#if defined(ENFILE) && defined(EMFILE)
        if (errno == ENFILE || errno == EMFILE)
        {
            SF_LOG_ERROR("server.authserver", "Out of file descriptors, suspending incoming connections for 10 seconds");
            reactor()->remove_handler(this, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL);
            reactor()->schedule_timer(this, NULL, ACE_Time_Value(10));
        }
#endif
        return 0;
    }
};

#endif
