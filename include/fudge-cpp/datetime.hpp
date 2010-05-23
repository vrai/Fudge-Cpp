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
#ifndef INC_FUDGE_CPP_DATETIME_HPP
#define INC_FUDGE_CPP_DATETIME_HPP

#include "fudge-cpp/datetimebase.hpp"

namespace fudge {

class date : public virtual datetimebase
{
    public:
        date ( );
        date ( int32_t year, uint8_t month, uint8_t day );
        virtual ~date ( );

        inline int32_t year ( ) const  { return m_datetime.date.year; }
        inline uint8_t month ( ) const { return m_datetime.date.month; }
        inline uint8_t day ( ) const   { return m_datetime.date.day; }
};

class time : public virtual datetimebase
{
    public:
        time ( );
        time ( uint32_t seconds,
               uint32_t nanoseconds,
               FudgeDateTimePrecision precision );
        time ( uint32_t seconds,
               uint32_t nanoseconds,
               FudgeDateTimePrecision precision,
               int8_t timezoneOffset );
        virtual ~time ( );

        inline uint32_t seconds ( ) const                 { return m_datetime.time.seconds; }
        inline uint32_t nanoseconds ( ) const             { return m_datetime.time.nanoseconds; }
        inline FudgeDateTimePrecision precision ( ) const { return m_datetime.time.precision; }
        inline bool hasTimezone ( ) const                 { return m_datetime.time.hasTimezone == FUDGE_TRUE; }
        inline int8_t timezoneOffset ( ) const            { return m_datetime.time.timezoneOffset; }
};

class datetime : public time, public date
{
    public:
        datetime ( );
        datetime ( int32_t year,
                   uint8_t month,
                   uint8_t day,
                   uint32_t seconds,
                   uint32_t nanoseconds,
                   FudgeDateTimePrecision precision );
        datetime ( int32_t year,
                   uint8_t month,
                   uint8_t day,
                   uint32_t seconds,
                   uint32_t nanoseconds,
                   FudgeDateTimePrecision precision,
                   int8_t timezoneOffset );
};

}

bool operator< ( const fudge::date & left, const fudge::date & right );
bool operator> ( const fudge::date & left, const fudge::date & right );
bool operator== ( const fudge::date & left, const fudge::date & right );
bool operator!= ( const fudge::date & left, const fudge::date & right );

bool operator< ( const fudge::time & left, const fudge::time & right );
bool operator> ( const fudge::time & left, const fudge::time & right );
bool operator== ( const fudge::time & left, const fudge::time & right );
bool operator!= ( const fudge::time & left, const fudge::time & right );

#endif

