# Copyright (C) 2011-2022 Project SkyFire <http://www.projectskyfire.org/
# Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# This file defines the following macros for developers to use in ensuring
# that installed software is of the right version:
#
# ENSURE_VERSION        - test that a version number is greater than
#                               or equal to some minimum
# ENSURE_VERSION_RANGE - test that a version number is greater than
#                               or equal to some minimum and less than some
#                               maximum
# ENSURE_VERSION2       - deprecated, do not use in new code
#

# ENSURE_VERSION
# This macro compares version numbers of the form "x.y.z" or "x.y"
# ENSURE_VERSION( FOO_MIN_VERSION FOO_VERSION_FOUND FOO_VERSION_OK)
# will set FOO_VERSION_OK to true if FOO_VERSION_FOUND >= FOO_MIN_VERSION
# Leading and trailing text is ok, e.g.
# ENSURE_VERSION( "2.5.31" "flex 2.5.4a" VERSION_OK)
# which means 2.5.31 is required and "flex 2.5.4a" is what was found on the system

# Copyright (c) 2006, David Faure, <faure@kde.org>
# Copyright (c) 2007, Will Stephenson <wstephenson@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# ENSURE_VERSION_RANGE
# This macro ensures that a version number of the form
# "x.y.z" or "x.y" falls within a range defined by
# min_version <= found_version < max_version.
# If this expression holds, FOO_VERSION_OK will be set TRUE
#
# Example: ENSURE_VERSION_RANGE3( "0.1.0" ${FOOCODE_VERSION} "0.7.0" FOO_VERSION_OK )
#
# This macro will break silently if any of x,y,z are greater than 100.
#
# Copyright (c) 2007, Will Stephenson <wstephenson@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# NORMALIZE_VERSION
# Helper macro to convert version numbers of the form "x.y.z"
# to an integer equal to 10^4 * x + 10^2 * y + z
#
# This macro will break silently if any of x,y,z are greater than 100.
#
# Copyright (c) 2006, David Faure, <faure@kde.org>
# Copyright (c) 2007, Will Stephenson <wstephenson@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# CHECK_RANGE_INCLUSIVE_LOWER
# Helper macro to check whether x <= y < z
#
# Copyright (c) 2007, Will Stephenson <wstephenson@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


MACRO(NORMALIZE_VERSION _requested_version _normalized_version)
    STRING(REGEX MATCH "[^0-9]*[0-9]+\\.[0-9]+\\.[0-9]+.*" _threePartMatch "${_requested_version}")
    if (_threePartMatch)
    # parse the parts of the version string
        STRING(REGEX REPLACE "[^0-9]*([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" _major_vers "${_requested_version}")
        STRING(REGEX REPLACE "[^0-9]*[0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" _minor_vers "${_requested_version}")
        STRING(REGEX REPLACE "[^0-9]*[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" _patch_vers "${_requested_version}")
    else (_threePartMatch)
        STRING(REGEX REPLACE "([0-9]+)\\.[0-9]+" "\\1" _major_vers "${_requested_version}")
        STRING(REGEX REPLACE "[0-9]+\\.([0-9]+)" "\\1" _minor_vers "${_requested_version}")
        set(_patch_vers "0")
    endif (_threePartMatch)

    # compute an overall version number which can be compared at once
    MATH(EXPR ${_normalized_version} "${_major_vers}*10000 + ${_minor_vers}*100 + ${_patch_vers}")
ENDMACRO(NORMALIZE_VERSION)

MACRO(CHECK_RANGE_INCLUSIVE_LOWER _lower_limit _value _upper_limit _ok)
   if (${_value} LESS ${_lower_limit})
      set( ${_ok} FALSE )
  elseif (${_value} EQUAL ${_lower_limit})
      set( ${_ok} TRUE )
  elseif (${_value} EQUAL ${_upper_limit})
      set( ${_ok} FALSE )
  elseif (${_value} GREATER ${_upper_limit})
      set( ${_ok} FALSE )
  else (${_value} LESS ${_lower_limit})
      set( ${_ok} TRUE )
  endif (${_value} LESS ${_lower_limit})
ENDMACRO(CHECK_RANGE_INCLUSIVE_LOWER)

MACRO(ENSURE_VERSION requested_version found_version var_too_old)
    NORMALIZE_VERSION( ${requested_version} req_vers_num )
    NORMALIZE_VERSION( ${found_version} found_vers_num )

    if (found_vers_num LESS req_vers_num)
        set( ${var_too_old} FALSE )
    else (found_vers_num LESS req_vers_num)
        set( ${var_too_old} TRUE )
    endif (found_vers_num LESS req_vers_num)

ENDMACRO(ENSURE_VERSION)

MACRO(ENSURE_VERSION2 requested_version2 found_version2 var_too_old2)
    ENSURE_VERSION( ${requested_version2} ${found_version2} ${var_too_old2})
ENDMACRO(ENSURE_VERSION2)

MACRO(ENSURE_VERSION_RANGE min_version found_version max_version var_ok)
    NORMALIZE_VERSION( ${min_version} req_vers_num )
    NORMALIZE_VERSION( ${found_version} found_vers_num )
    NORMALIZE_VERSION( ${max_version} max_vers_num )

    CHECK_RANGE_INCLUSIVE_LOWER( ${req_vers_num} ${found_vers_num} ${max_vers_num} ${var_ok})
ENDMACRO(ENSURE_VERSION_RANGE)
