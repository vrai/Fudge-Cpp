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
#ifndef INC_FUDGE_CPP_ENVELOPE_HPP
#define INC_FUDGE_CPP_ENVELOPE_HPP

#include "fudge-cpp/message.hpp"

namespace fudge {

class envelope
{
    public:
        envelope ( );
        envelope ( fudge_byte directives,
                   fudge_byte schemaversion,
                   fudge_i16 taxonomy,
                   message payload );
        envelope ( FudgeMsgEnvelope source );
        envelope ( const envelope & source );
        envelope & operator= ( const envelope & source );
        ~envelope ( );

        fudge_byte directives ( ) const;
        fudge_byte schemaversion ( ) const;
        fudge_i16 taxonomy ( ) const;
        message payload ( ) const;

        FudgeMsgEnvelope raw ( ) const;

    private:
        FudgeMsgEnvelope m_envelope;
};

}

#endif


