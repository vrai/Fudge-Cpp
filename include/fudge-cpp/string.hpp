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

#include "fudge/types.h"
#include <string>

namespace fudge {

class string
{
    public:
        enum UnicodeType
        {
            UTF8, UTF16, UTF32
        };

        // NULL string constructor
        string ( );

        // ASCII string constructors
        string ( const std::string & source );
        string ( const char * chars, size_t numchars );
        string ( const char * chars );

        // Unicode string constructor
        string ( const fudge_byte * bytes, size_t numbytes, UnicodeType type );

        string ( const string & source );
        string & operator= ( const string & source );

        ~string ( );

        size_t size ( ) const;
        const fudge_byte * data ( ) const;

        // Conversion methods - will allocate the memory required to hold the
        // converted strings. It is the job of the calling code to release
        // this memory if/when required. If called against a NULL string the
        // pointer and size (if present) are set to zero.
        void convertToASCIIZ ( char * & string ) const;
        void convertToUTF16 ( fudge_byte * & bytes, size_t & numbytes ) const;
        void convertToUTF32 ( fudge_byte * & bytes, size_t & numbytes ) const;

        // As convertToASCIIZ, but wraps the C string within a std::string and
        // returns it.. If called against a NULL string, an empty std::string
        // is returned.
        std::string convertToStdString ( ) const;

        const FudgeString raw ( ) const;

    private:
        FudgeString m_string;
};

bool operator< ( const fudge::string & left, const fudge::string & right );
bool operator> ( const fudge::string & left, const fudge::string & right );
bool operator== ( const fudge::string & left, const fudge::string & right );
bool operator!= ( const fudge::string & left, const fudge::string & right );

}

#endif

