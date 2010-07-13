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
#include "fudge-cpp/field.hpp"
#include "fudge-cpp/exception.hpp"
#include "fudge/message_ex.h"

namespace
{
    template<class Type, fudge_type_id Desired> inline Type getImpl ( fudge_type_id type, Type value )
    {
        if ( Desired == type )
            return value;
        throw fudge::exception ( FUDGE_INVALID_TYPE_ACCESSOR );
    }

    template<class Type> inline Type getAsImpl ( const FudgeField * field,
                                                 FudgeStatus ( *function ) ( const FudgeField *, Type * ) )
    {
        Type value;
        fudge::exception::throwOnError ( function ( field, &value ) );
        return value;
    }

    template<class Type> inline size_t getArrayImpl ( fudge_type_id type,
                                                        const FudgeField & field,
                                                        std::vector<Type> & target )
    {
        if ( field.type != type )
            throw fudge::exception ( FUDGE_INVALID_TYPE_ACCESSOR );

        const size_t numelements ( field.numbytes / sizeof ( Type ) );
        if ( numelements )
        {
            target.resize ( numelements );
            for ( size_t index ( 0 ); index < numelements; ++index )
                target [ index ] = reinterpret_cast<const Type *> ( field.data.bytes ) [ index ];
        }
        else
            target.clear ( );

        return target.size ( );
    }
}

namespace fudge {

field::field ( )
{
    m_field.type = FUDGE_TYPE_INDICATOR;
}

field::field ( const FudgeField & field )
{
    m_field = field;
}

optional<string> field::name ( ) const
{
    if ( m_field.flags & FUDGE_FIELD_HAS_NAME )
        return string ( m_field.name );
    return optional<string> ( );
}

optional<fudge_i16> field::ordinal ( ) const
{
    if ( m_field.flags & FUDGE_FIELD_HAS_ORDINAL )
        return m_field.ordinal;
    return optional<fudge_i16> ( );
}

bool field::getBoolean ( ) const
{
    if ( m_field.type == FUDGE_TYPE_BOOLEAN )
        return m_field.data.boolean == FUDGE_TRUE;
    throw exception ( FUDGE_INVALID_TYPE_ACCESSOR );
}

fudge_byte field::getByte ( ) const
{
    return getImpl<fudge_byte, FUDGE_TYPE_BYTE> ( m_field.type, m_field.data.byte );
}

fudge_i16 field::getInt16 ( ) const
{
    return getImpl<fudge_i16, FUDGE_TYPE_SHORT> ( m_field.type, m_field.data.i16 );
}

fudge_i32 field::getInt32 ( ) const
{
    return getImpl<fudge_i32, FUDGE_TYPE_INT> ( m_field.type, m_field.data.i32 );
}

fudge_i64 field::getInt64 ( ) const
{
    return getImpl<fudge_i64, FUDGE_TYPE_LONG> ( m_field.type, m_field.data.i64 );
}

fudge_f32 field::getFloat32 ( ) const
{
    return getImpl<fudge_f32, FUDGE_TYPE_FLOAT> ( m_field.type, m_field.data.f32 );
}

fudge_f64 field::getFloat64 ( ) const
{
    return getImpl<fudge_f64, FUDGE_TYPE_DOUBLE> ( m_field.type, m_field.data.f64 );
}

string field::getString ( ) const
{
    return getImpl<string, FUDGE_TYPE_STRING> ( m_field.type, m_field.data.string );
}

FudgeMsg field::getMessage ( ) const
{
    if ( m_field.type != FUDGE_TYPE_FUDGE_MSG )
        throw exception ( FUDGE_INVALID_TYPE_ACCESSOR );
    return m_field.data.message;
}

date field::getDate ( ) const
{
    return getImpl<date, FUDGE_TYPE_DATE> ( m_field.type, m_field.data.datetime.date );
}

time field::getTime ( ) const
{
    return getImpl<time, FUDGE_TYPE_TIME> ( m_field.type, m_field.data.datetime.time );
}

datetime field::getDateTime ( ) const
{
    return getImpl<datetime, FUDGE_TYPE_DATETIME> ( m_field.type, m_field.data.datetime );
}

bool field::getAsBoolean ( ) const
{
    fudge_bool value;
    exception::throwOnError ( FudgeMsg_getFieldAsBoolean ( &m_field, &value ) );
    return value == FUDGE_TRUE;
}

fudge_byte field::getAsByte ( ) const
{
    return getAsImpl<fudge_byte> ( &m_field, &FudgeMsg_getFieldAsByte );
}

fudge_i16 field::getAsInt16 ( ) const
{
    return getAsImpl<fudge_i16> ( &m_field, &FudgeMsg_getFieldAsI16 );
}

fudge_i32 field::getAsInt32 ( ) const
{
    return getAsImpl<fudge_i32> ( &m_field, &FudgeMsg_getFieldAsI32 );
}

fudge_i64 field::getAsInt64 ( ) const
{
    return getAsImpl<fudge_i64> ( &m_field, &FudgeMsg_getFieldAsI64 );
}

fudge_f32 field::getAsFloat32 ( ) const
{
    return getAsImpl<fudge_f32> ( &m_field, &FudgeMsg_getFieldAsF32 );
}

fudge_f64 field::getAsFloat64 ( ) const
{
    return getAsImpl<fudge_f64> ( &m_field, &FudgeMsg_getFieldAsF64 );
}

string field::getAsString ( ) const
{
    FudgeFieldData data;
    FudgeTypePayload payload;
    fudge_i32 numbytes;
    const FudgeStatus status ( FudgeMsg_getFieldAs ( &m_field, FUDGE_TYPE_STRING, &data, &payload, &numbytes ) );
    if ( status == FUDGE_COERCION_NOT_REQUIRED )
        return getString ( );

    exception::throwOnError ( status );
    return string ( data.string );
}

size_t field::getArray ( std::vector<fudge_byte> & target ) const
{
    return getArrayImpl<fudge_byte> ( FUDGE_TYPE_BYTE_ARRAY, m_field, target );
}

size_t field::getArray ( std::vector<fudge_i16> & target ) const
{
    return getArrayImpl<fudge_i16> ( FUDGE_TYPE_SHORT_ARRAY, m_field, target );
}

size_t field::getArray ( std::vector<fudge_i32> & target ) const
{
    return getArrayImpl<fudge_i32> ( FUDGE_TYPE_INT_ARRAY, m_field, target );
}

size_t field::getArray ( std::vector<fudge_i64> & target ) const
{
    return getArrayImpl<fudge_i64> ( FUDGE_TYPE_LONG_ARRAY, m_field, target );
}

size_t field::getArray ( std::vector<fudge_f32> & target ) const
{
    return getArrayImpl<fudge_f32> ( FUDGE_TYPE_FLOAT_ARRAY, m_field, target );
}

size_t field::getArray ( std::vector<fudge_f64> & target ) const
{
    return getArrayImpl<fudge_f64> ( FUDGE_TYPE_DOUBLE_ARRAY, m_field, target );
}

}

