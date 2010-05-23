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
#include "fudge-cpp/exception.hpp"

namespace fudge {

exception::exception ( FudgeStatus status )
    : std::runtime_error ( FudgeStatus_strerror ( status ) )
    , m_status ( status )
{
}

exception::exception ( const exception & source )
    : std::runtime_error ( source )
    , m_status ( source.m_status )
{
}

FudgeStatus exception::status ( ) const
{
    return m_status;
}

void exception::throwOnError ( FudgeStatus status )
{
    if ( status != FUDGE_OK )
        throw exception ( status );
}

}

