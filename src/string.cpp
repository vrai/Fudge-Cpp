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
#include "fudge-cpp/string.hpp"
#include "fudge-cpp/exception.hpp"
#include "fudge/string.h"

namespace
{
    typedef FudgeStatus ( *FudgeStringUTFConstructor ) ( FudgeString *, const fudge_byte *, size_t );

    inline FudgeStringUTFConstructor getConstructorForType ( fudge::string::UnicodeType type )
    {
        switch ( type )
        {
            case fudge::string::UTF8:  return FudgeString_createFromUTF8;
            case fudge::string::UTF16: return FudgeString_createFromUTF16;
            case fudge::string::UTF32: return FudgeString_createFromUTF32;
            default:                   return 0;
        }
    }
}

namespace fudge {

string::string ( )
    : m_string ( 0 )
{
}

string::string ( const std::string & source )
    : m_string ( 0 )
{
    exception::throwOnError ( FudgeString_createFromASCII ( &m_string, source.c_str ( ), source.size ( ) ) );
}

string::string ( const char * chars, size_t numchars )
    : m_string ( 0 )
{
    exception::throwOnError ( FudgeString_createFromASCII ( &m_string, chars, numchars ) );
}

string::string ( const char * chars )
    : m_string ( 0 )
{
    exception::throwOnError ( FudgeString_createFromASCIIZ ( &m_string, chars ) );
}

string::string ( const fudge_byte * bytes, size_t numbytes, UnicodeType type )
    : m_string ( 0 )
{
    FudgeStringUTFConstructor constructor ( getConstructorForType ( type ) );
    if ( constructor )
        exception::throwOnError ( constructor ( &m_string, bytes, numbytes ) );
    else
        throw exception ( FUDGE_STRING_UNKNOWN_UNICODE_TYPE );
}

string::string ( const string & source )
    : m_string ( 0 )
{
    if ( source.m_string )
    {
        exception::throwOnError ( FudgeString_retain ( source.m_string ) );
        m_string = source.m_string;
    }
}

string & string::operator= ( const string & source )
{
    FudgeString oldstring ( m_string );

    if ( source.m_string )
    {
        exception::throwOnError ( FudgeString_retain ( source.m_string ) );
        m_string = source.m_string;
    }
    else
        m_string = 0;

    if ( oldstring )
        exception::throwOnError ( FudgeString_release ( oldstring ) );

    return *this;
}

string::~string ( )
{
    if ( m_string )
        exception::throwOnError ( FudgeString_release ( m_string ) );
}

size_t string::size ( ) const
{
    return FudgeString_getSize ( m_string );
}

const fudge_byte * string::data ( ) const
{
    return FudgeString_getData ( m_string );
}

void string::convertToASCIIZ ( char * & string ) const
{
    if ( m_string )
        exception::throwOnError ( FudgeString_convertToASCIIZ ( &string, m_string ) );
    else
        string = 0;
}

std::string string::convertToStdString ( ) const
{
    char * cstring ( 0 );
    convertToASCIIZ ( cstring );
    if ( cstring )
    {
        std::string copy ( cstring );
        free ( cstring );
        return copy;
    }
    return std::string ( );
}

void string::convertToUTF16 ( fudge_byte * & bytes, size_t & numbytes ) const
{
    if ( m_string )
        exception::throwOnError ( FudgeString_convertToUTF16 ( &bytes, &numbytes, m_string ) );
    else
    {
        bytes = 0;
        numbytes = 0;
    }
}

void string::convertToUTF32 ( fudge_byte * & bytes, size_t & numbytes ) const
{
    if ( m_string )
        exception::throwOnError ( FudgeString_convertToUTF32 ( &bytes, &numbytes, m_string ) );
    else
    {
        bytes = 0;
        numbytes = 0;
    }
}

const FudgeString string::raw ( ) const
{
    return m_string;
}

bool operator< ( const fudge::string & left, const fudge::string & right )
{
    return FudgeString_compare ( const_cast<FudgeString> ( left.raw ( ) ),
                                 const_cast<FudgeString> ( right.raw ( ) ) ) < 0;
}

bool operator> ( const fudge::string & left, const fudge::string & right )
{
    return FudgeString_compare ( const_cast<FudgeString> ( left.raw ( ) ),
                                 const_cast<FudgeString> ( right.raw ( ) ) ) > 0;
}

bool operator== ( const fudge::string & left, const fudge::string & right )
{
    return FudgeString_compare ( const_cast<FudgeString> ( left.raw ( ) ),
                                 const_cast<FudgeString> ( right.raw ( ) ) ) == 0;
}

bool operator!= ( const fudge::string & left, const fudge::string & right )
{
    return FudgeString_compare ( const_cast<FudgeString> ( left.raw ( ) ),
                                 const_cast<FudgeString> ( right.raw ( ) ) ) != 0;
}

}

