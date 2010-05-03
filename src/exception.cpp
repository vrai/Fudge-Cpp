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

FudgeException::FudgeException ( FudgeStatus status )
    : std::runtime_error ( FudgeStatus_strerror ( status ) )
    , m_status ( status )
{
}

FudgeException::FudgeException ( const FudgeException & exception )
    : std::runtime_error ( exception )
    , m_status ( exception.m_status )
{
}

FudgeStatus FudgeException::status ( ) const
{
    return m_status;
}

void FudgeException::throwOnError ( FudgeStatus status )
{
    if ( status != FUDGE_OK )
        throw FudgeException ( status );
}

}

