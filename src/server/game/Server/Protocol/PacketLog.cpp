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

#include "PacketLog.h"
#include "Config.h"
#include "ByteBuffer.h"
#include "WorldPacket.h"

PacketLog::PacketLog() : _file(NULL)
{
    Initialize();
}

PacketLog::~PacketLog()
{
    if (_file)
        fclose(_file);

    _file = NULL;
}

void PacketLog::Initialize()
{
    std::string logsDir = sConfigMgr->GetStringDefault("LogsDir", "");

    if (!logsDir.empty())
        if ((logsDir.at(logsDir.length()-1) != '/') && (logsDir.at(logsDir.length()-1) != '\\'))
            logsDir.push_back('/');

    std::string logname = sConfigMgr->GetStringDefault("PacketLogFile", "");
    if (!logname.empty())
        _file = fopen((logsDir + logname).c_str(), "wb");
}

void PacketLog::LogPacket(WorldPacket const& packet, Direction direction)
{
    ByteBuffer data(4+4+4+1+packet.size());
    uint32 opcode = direction == CLIENT_TO_SERVER ? const_cast<WorldPacket&>(packet).GetReceivedOpcode() : serverOpcodeTable[packet.GetOpcode()]->OpcodeNumber;

    data << int32(opcode);
    data << int32(packet.size());
    data << uint32(time(NULL));
    data << uint8(direction);

    for (uint32 i = 0; i < packet.size(); i++)
        data << packet[i];

    fwrite(data.contents(), 1, data.size(), _file);
    fflush(_file);
}
