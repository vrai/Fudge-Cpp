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
#ifndef INC_FUDGE_CPP_STRING_HPP
#define INC_FUDGE_CPP_STRING_HPP

#include "fudge-cpp/exception.hpp"
#include "fudge/types.h"
#include <string>

namespace fudge {

class FudgeString
{
    public:
        enum UnicodeType
        {
            UTF8, UTF16, UTF32
        };

        // NULL string constructor
        FudgeString ( );

        // ASCII string constructors
        FudgeString ( const std::string & source );
        FudgeString ( const char * chars, size_t numchars );
        FudgeString ( const char * chars );

        // Unicode string constructor
        FudgeString ( const fudge_byte * bytes, size_t numbytes, UnicodeType type );

        FudgeString ( const FudgeString & source );
        FudgeString & operator= ( const FudgeString & source );

        ~FudgeString ( );

        // TODO size
        // TODO data
        // TODO getAsASCIIZ
        // TODO getAsStdString
        // TODO getAsUTF16
        // TODO getAsUTF32

    private:
        FudgeStringImpl * m_string;
};
        
// TODO operator< 
// TODO operator>
// TODO operator==

}

#endif

