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
#include "fudge-cpp/envelope.hpp"
#include "fudge-cpp/exception.hpp"
#include "fudge/envelope.h"

namespace fudge {

envelope::envelope ( )
    : m_envelope ( 0 )
{
}

envelope::envelope ( fudge_byte directives,
                     fudge_byte schemaversion,
                     fudge_i16 taxonomy,
                     message payload )
    : m_envelope ( 0 )
{
    exception::throwOnError ( FudgeMsgEnvelope_create ( &m_envelope,
                                                        directives,
                                                        schemaversion,
                                                        taxonomy,
                                                        payload.raw ( ) ) );
}

envelope::envelope ( FudgeMsgEnvelope source, bool takeReference )
    : m_envelope ( source )
{
    if ( takeReference && m_envelope )
        exception::throwOnError ( FudgeMsgEnvelope_retain ( m_envelope ) );
}

envelope::envelope ( const envelope & source )
    : m_envelope ( source.m_envelope )
{
    if ( m_envelope )
        exception::throwOnError ( FudgeMsgEnvelope_retain ( m_envelope ) );
}

envelope & envelope::operator= ( const envelope & source )
{
    if ( this != &source )
    {
        FudgeMsgEnvelope oldEnvelope ( m_envelope );

        if ( source.m_envelope )
        {
            exception::throwOnError ( FudgeMsgEnvelope_retain ( source.m_envelope ) );
            m_envelope = source.m_envelope;
        }
        else
            m_envelope = 0;

        if ( oldEnvelope )
            exception::throwOnError ( FudgeMsgEnvelope_release ( oldEnvelope ) );
    }
    return *this;
}

envelope::~envelope ( )
{
    if ( m_envelope )
        exception::throwOnError ( FudgeMsgEnvelope_release ( m_envelope ) );
}

fudge_byte envelope::directives ( ) const
{
    return FudgeMsgEnvelope_getDirectives ( m_envelope );
}

fudge_byte envelope::schemaversion ( ) const
{
    return FudgeMsgEnvelope_getSchemaVersion ( m_envelope );
}

fudge_i16 envelope::taxonomy ( ) const
{
    return FudgeMsgEnvelope_getTaxonomy ( m_envelope );
}

message envelope::payload ( ) const
{
    return FudgeMsgEnvelope_getMessage ( m_envelope );
}

FudgeMsgEnvelope envelope::raw ( ) const
{
    return m_envelope;
}

}


