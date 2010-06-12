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
#include "fudge-cpp/message.hpp"
#include "fudge-cpp/exception.hpp"
#include "fudge/message.h"

namespace
{
    inline const FudgeString convertNameArg ( const fudge::optional<fudge::string> & name )
    {
        return name ? name.get ( ).raw ( ) : 0;
    }

    inline const fudge_i16 * convertOrdinalArg ( const fudge::optional<fudge_i16> & ordinal )
    {
        return ordinal ? &( *ordinal ) : 0;
    }

    template<class Type> inline void addFieldImpl ( FudgeMsg message,
                                                    FudgeStatus ( *function ) ( FudgeMsg, const FudgeString, const fudge_i16 *, Type ),
                                                    const Type & value,
                                                    const fudge::optional<fudge::string> & name,
                                                    const fudge::optional<fudge_i16> ordinal )
    {
        fudge::exception::throwOnError ( function ( message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), value ) );
    }

    template<class Type> inline void addArrayFieldImpl ( FudgeMsg message,
                                                         FudgeStatus ( *function ) ( FudgeMsg, const FudgeString, const fudge_i16 *, const Type *, fudge_i32 ),
                                                         const std::vector<Type> & value,
                                                         const fudge::optional<fudge::string> & name,
                                                         const fudge::optional<fudge_i16> ordinal )
    {
        fudge::exception::throwOnError ( function ( message,
                                                    convertNameArg ( name ),
                                                    convertOrdinalArg ( ordinal ),
                                                    value.empty ( ) ? 0 : &( value.front ( ) ), value.size ( ) ) );
    }
}

namespace fudge {

const optional<string> message::noname;
const optional<fudge_i16> message::noordinal;

message::message ( )
    : m_message ( 0 )
{
    exception::throwOnError ( FudgeMsg_create ( &m_message ) );
}

message::message ( FudgeMsg source )
    : m_message ( source )
{
    exception::throwOnError ( FudgeMsg_retain ( m_message ) );
}

message::message ( const message & source )
    : m_message ( source.m_message )
{
    exception::throwOnError ( FudgeMsg_retain ( m_message ) );
}

message & message::operator= ( const message & source )
{
    if ( this != &source )
    {
        FudgeMsg oldMessage ( m_message );

        exception::throwOnError ( FudgeMsg_retain ( source.m_message ) );
        m_message = source.m_message;

        exception::throwOnError ( FudgeMsg_release ( oldMessage ) );
    }
    return *this;
}

message::~message ( )
{
    exception::throwOnError ( FudgeMsg_release ( m_message ) );
}

size_t message::size ( ) const
{
    return FudgeMsg_numFields ( m_message );
}

field message::getFieldAt ( size_t index ) const
{
    FudgeField raw;
    exception::throwOnError ( FudgeMsg_getFieldAtIndex ( &raw, m_message, index ) );
    return raw;
}

field message::getField ( const string & name ) const
{
    FudgeField raw;
    exception::throwOnError ( FudgeMsg_getFieldByName ( &raw, m_message, name.raw ( ) ) );
    return raw;
}

field message::getField ( fudge_i16 ordinal ) const
{
    FudgeField raw;
    exception::throwOnError ( FudgeMsg_getFieldByOrdinal ( &raw, m_message, ordinal ) );
    return raw;
}

void message::getFields ( std::vector<field> & fields ) const
{
    fields.clear ( );

    const size_t count ( size ( ) );
    if ( ! count )
        return;

    FudgeField * array ( new FudgeField [ count ] );
    const fudge_i32 retrieved ( FudgeMsg_getFields ( array, count, m_message ) );
    if ( retrieved > 0 )
    {
        fields.resize ( retrieved );
        fields.assign ( array, array + retrieved );
    }
    delete [] array;
}

void message::addField ( const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldIndicator ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ) ) );
}

void message::addField ( bool value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldBool ( m_message,
                                                      convertNameArg ( name ),
                                                      convertOrdinalArg ( ordinal ),
                                                      value ? FUDGE_TRUE : FUDGE_FALSE ) );
}

void message::addField ( fudge_byte value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addFieldImpl<fudge_byte> ( m_message, &FudgeMsg_addFieldByte, value, name, ordinal );
}

void message::addField ( fudge_i16 value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addFieldImpl<fudge_i16> ( m_message, &FudgeMsg_addFieldI16, value, name, ordinal );
}

void message::addField ( fudge_i32 value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addFieldImpl<fudge_i32> ( m_message, &FudgeMsg_addFieldI32, value, name, ordinal );
}

void message::addField ( fudge_i64 value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addFieldImpl<fudge_i64> ( m_message, &FudgeMsg_addFieldI64, value, name, ordinal );
}

void message::addField ( fudge_f32 value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addFieldImpl<fudge_f32> ( m_message, &FudgeMsg_addFieldF32, value, name, ordinal );
}

void message::addField ( fudge_f64 value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addFieldImpl<fudge_f64> ( m_message, &FudgeMsg_addFieldF64, value, name, ordinal );
}

void message::addField ( message & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldMsg ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), value.raw ( ) ) );
}

void message::addField ( const std::vector<fudge_byte> & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addArrayFieldImpl<fudge_byte> ( m_message, &FudgeMsg_addFieldByteArray, value, name, ordinal );
}

void message::addField ( const std::vector<fudge_i16> & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addArrayFieldImpl<fudge_i16> ( m_message, &FudgeMsg_addFieldI16Array, value, name, ordinal );
}

void message::addField ( const std::vector<fudge_i32> & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addArrayFieldImpl<fudge_i32> ( m_message, &FudgeMsg_addFieldI32Array, value, name, ordinal );
}

void message::addField ( const std::vector<fudge_i64> & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addArrayFieldImpl<fudge_i64> ( m_message, &FudgeMsg_addFieldI64Array, value, name, ordinal );
}

void message::addField ( const std::vector<fudge_f32> & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addArrayFieldImpl<fudge_f32> ( m_message, &FudgeMsg_addFieldF32Array, value, name, ordinal );
}

void message::addField ( const std::vector<fudge_f64> & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    addArrayFieldImpl<fudge_f64> ( m_message, &FudgeMsg_addFieldF64Array, value, name, ordinal );
}

void message::addField ( const string & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldString ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), value.raw ( ) ) );
}

void message::addField ( const date & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldDate ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), &( value.raw ( ).date ) ) );
}

void message::addField ( const time & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldTime ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), &( value.raw ( ).time ) ) );
}

void message::addField ( const datetime & value, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldDateTime ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), &( value.raw ( ) ) ) );
}

void message::addField ( fudge_type_id type,
                         const fudge_byte * bytes,
                         fudge_i32 numbytes,
                         const optional<string> name,
                         const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldOpaque ( m_message, type, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes, numbytes ) );
}

void message::addField4ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField4ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField8ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField8ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField16ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField16ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField20ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField20ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField32ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField32ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField64ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField64ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField128ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField128ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField256ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField256ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addField512ByteArray ( const fudge_byte * bytes, const optional<string> & name, const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addField512ByteArray ( m_message, convertNameArg ( name ), convertOrdinalArg ( ordinal ), bytes ) );
}

void message::addFieldData ( fudge_type_id type,
                             FudgeFieldData * data,
                             fudge_i32 numbytes,
                             const optional<string> name,
                             const optional<fudge_i16> ordinal )
{
    exception::throwOnError ( FudgeMsg_addFieldData ( m_message, type, convertNameArg ( name ), convertOrdinalArg ( ordinal ), data, numbytes ) );
}

FudgeMsg message::raw ( ) const
{
    return m_message;
}

}

