/**
 * Copyright (C) 2010 - 2010, Vrai Stacey.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "fudge-cpp/datetime.hpp"
#include "fudge-cpp/exception.hpp"
#include "fudge/datetime.h"

namespace fudge {

datetimebase::datetimebase ( )
{
}

datetimebase::~datetimebase ( )
{
}

date::date ( )
{
    memset ( &m_datetime.date, 0, sizeof ( FudgeDate ) );
}

date::date ( int32_t year, uint8_t month, uint8_t day )
{
    exception::throwOnError ( FudgeDate_initialise ( &m_datetime.date, year, month, day ) );
}

date::~date ( )
{
}

time::time ( )
{
    memset ( &m_datetime.time, 0, sizeof ( FudgeTime ) );
}

time::time ( uint32_t seconds, uint32_t nanoseconds, FudgeDateTimePrecision precision )
{
    exception::throwOnError ( FudgeTime_initialise ( &m_datetime.time, seconds, nanoseconds, precision ) );
}

time::time ( uint32_t seconds, uint32_t nanoseconds, FudgeDateTimePrecision precision, int8_t timezoneOffset )
{
    exception::throwOnError ( FudgeTime_initialiseWithTimezone ( &m_datetime.time, seconds, nanoseconds, precision, timezoneOffset ) );
}

time::~time ( )
{
}

datetime::datetime ( )
{
}

datetime::datetime ( int32_t year, uint8_t month, uint8_t day, uint32_t seconds, uint32_t nanoseconds, FudgeDateTimePrecision precision )
{
    exception::throwOnError ( FudgeDateTime_initialise ( &m_datetime, year, month, day, seconds, nanoseconds, precision ) );
}

datetime::datetime ( int32_t year, uint8_t month, uint8_t day, uint32_t seconds, uint32_t nanoseconds, FudgeDateTimePrecision precision, int8_t timezoneOffset )
{
    exception::throwOnError ( FudgeDateTime_initialiseWithTimezone ( &m_datetime, year, month, day, seconds, nanoseconds, precision, timezoneOffset ) );
}

}

bool operator< ( const fudge::date & left, const fudge::date & right )
{
    return FudgeDate_compare ( &( left.raw ( ).date ), &( right.raw ( ).date ) ) < 0;
}

bool operator> ( const fudge::date & left, const fudge::date & right )
{
    return FudgeDate_compare ( &( left.raw ( ).date ), &( right.raw ( ).date ) ) > 0;
}

bool operator== ( const fudge::date & left, const fudge::date & right )
{
    return FudgeDate_compare ( &( left.raw ( ).date ), &( right.raw ( ).date ) ) == 0;
}

bool operator!= ( const fudge::date & left, const fudge::date & right )
{
    return FudgeDate_compare ( &( left.raw ( ).date ), &( right.raw ( ).date ) ) != 0;
}

bool operator< ( const fudge::time & left, const fudge::time & right )
{
    return FudgeTime_compare ( &( left.raw ( ).time ), &( right.raw ( ).time ) ) < 0;
}

bool operator> ( const fudge::time & left, const fudge::time & right )
{
    return FudgeTime_compare ( &( left.raw ( ).time ), &( right.raw ( ).time ) ) > 0;
}

bool operator== ( const fudge::time & left, const fudge::time & right )
{
    return FudgeTime_compare ( &( left.raw ( ).time ), &( right.raw ( ).time ) ) == 0;
}

bool operator!= ( const fudge::time & left, const fudge::time & right )
{
    return FudgeTime_compare ( &( left.raw ( ).time ), &( right.raw ( ).time ) ) != 0;
}


