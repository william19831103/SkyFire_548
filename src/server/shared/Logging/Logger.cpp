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

#include "Logger.h"

Logger::Logger(): name(""), level(LogLevel::LOG_LEVEL_DISABLED) { }

void Logger::Create(std::string const& _name, LogLevel _level)
{
    name = _name;
    level = _level;
}

std::string const& Logger::getName() const
{
    return name;
}

LogLevel Logger::getLogLevel() const
{
    return level;
}

void Logger::addAppender(uint8 id, Appender* appender)
{
    appenders[id] = appender;
}

void Logger::delAppender(uint8 id)
{
    appenders.erase(id);
}

void Logger::setLogLevel(LogLevel _level)
{
    level = _level;
}

void Logger::write(LogMessage& message) const
{
    if (level == LogLevel::LOG_LEVEL_DISABLED || level > message.level || message.text.empty())
    {
        //fprintf(stderr, "Logger::write: Logger %s, Level %u. Msg %s Level %u WRONG LEVEL MASK OR EMPTY MSG\n", getName().c_str(), message.level, message.text.c_str(), message.level); // DEBUG - RemoveMe
        return;
    }

    for (AppenderMap::const_iterator it = appenders.begin(); it != appenders.end(); ++it)
        if (it->second)
            it->second->write(message);
}
