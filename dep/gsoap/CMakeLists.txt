# Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
# Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB sources *.cpp *.h)

set(gsoap_STAT_SRCS
   ${sources}
)

include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
)

# Little fix for MSVC / Windows platforms
add_definitions(-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=0)

add_library(gsoap STATIC ${gsoap_STAT_SRCS})

set_target_properties(gsoap PROPERTIES LINKER_LANGUAGE CXX)
