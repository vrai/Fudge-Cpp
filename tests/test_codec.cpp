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
#include "simpletest.hpp"
#include "fudge-cpp/exception.hpp"
#include "fudge-cpp/codec.hpp"
#include <fstream>
#include <iostream>

namespace
{
    static const std::string AllNames_Filename      ( "test_data/allNames.dat" ),
                             FixedWidth_Filename    ( "test_data/fixedWidthByteArrays.dat" ),
                             AllOrdinals_Filename   ( "test_data/allOrdinals.dat" ),
                             SubMsg_Filename        ( "test_data/subMsg.dat" ),
                             Unknown_Filename       ( "test_data/unknown.dat" ),
                             VariableWidth_Filename ( "test_data/variableWidthColumnSizes.dat" ),
                             DateTimes_Filename     ( "test_data/dateTimes.dat" ),
                             Deeper_Filename        ( "test_data/deeper_fudge_msg.dat" );

    void loadFile ( const std::string & filename, fudge_byte * & bytes, fudge_i32 & numbytes );
    fudge::message loadFudgeMessage ( const std::string & filename );

    // Possibly move this in to string.hpp? Too slow for non-test use?
    std::ostream & operator<< ( std::ostream & stream, const fudge::string & string )
    {
        return stream << string.convertToStdString ( );
    }
}

DEFINE_TEST( DecodeAllNames )
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( AllNames_Filename ) );

    // Get the message fields
    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 21 );

    // None of the fields should have ordinals
    for ( size_t index ( 0 ); index < fields.size ( ); ++index )
        TEST_EQUALS_TRUE( ! message1.getFieldAt ( index ).ordinal ( ) );

    // Check the field names and contents
    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BOOLEAN );   TEST_EQUALS( fields [ 0 ].name ( ).get ( ), string ( "boolean" ) );     TEST_EQUALS( fields [ 0 ].getBoolean ( ), true );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BOOLEAN );   TEST_EQUALS( fields [ 1 ].name ( ).get ( ), string ( "Boolean" ) );     TEST_EQUALS( fields [ 1 ].getBoolean ( ), false );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS( fields [ 2 ].name ( ).get ( ), string ( "byte" ) );        TEST_EQUALS_INT( fields [ 2 ].getByte ( ), 5 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS( fields [ 3 ].name ( ).get ( ), string ( "Byte" ) );        TEST_EQUALS_INT( fields [ 3 ].getByte ( ), 5 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_SHORT );     TEST_EQUALS( fields [ 4 ].name ( ).get ( ), string ( "short" ) );       TEST_EQUALS_INT( fields [ 4 ].getInt16 ( ), 127 + 5 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_SHORT );     TEST_EQUALS( fields [ 5 ].name ( ).get ( ), string ( "Short" ) );       TEST_EQUALS_INT( fields [ 5 ].getInt16 ( ), 127 + 5 );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_INT );       TEST_EQUALS( fields [ 6 ].name ( ).get ( ), string ( "int" ) );         TEST_EQUALS_INT( fields [ 6 ].getInt32 ( ), 32767 + 5 );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_INT );       TEST_EQUALS( fields [ 7 ].name ( ).get ( ), string ( "Integer" ) );     TEST_EQUALS_INT( fields [ 7 ].getInt32 ( ), 32767 + 5 );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_LONG );      TEST_EQUALS( fields [ 8 ].name ( ).get ( ), string ( "long" ) );        TEST_EQUALS_INT( fields [ 8 ].getInt64 ( ), 2147483647ll + 5ll );
    TEST_EQUALS_INT( fields [ 9 ].type ( ), FUDGE_TYPE_LONG );      TEST_EQUALS( fields [ 9 ].name ( ).get ( ), string ( "Long" ) );        TEST_EQUALS_INT( fields [ 9 ].getInt64 ( ), 2147483647ll + 5ll );
    TEST_EQUALS_INT( fields [ 10 ].type ( ), FUDGE_TYPE_FLOAT );    TEST_EQUALS( fields [ 10 ].name ( ).get ( ), string ( "float" ) );      TEST_EQUALS_FLOAT( fields [ 10 ].getFloat32 ( ), 0.5f, 0.00001f );
    TEST_EQUALS_INT( fields [ 11 ].type ( ), FUDGE_TYPE_FLOAT );    TEST_EQUALS( fields [ 11 ].name ( ).get ( ), string ( "Float" ) );      TEST_EQUALS_FLOAT( fields [ 11 ].getFloat32 ( ), 0.5f, 0.00001f );
    TEST_EQUALS_INT( fields [ 12 ].type ( ), FUDGE_TYPE_DOUBLE );   TEST_EQUALS( fields [ 12 ].name ( ).get ( ), string ( "double" ) );     TEST_EQUALS_FLOAT( fields [ 12 ].getFloat64 ( ), 0.27362, 0.000001 );
    TEST_EQUALS_INT( fields [ 13 ].type ( ), FUDGE_TYPE_DOUBLE );   TEST_EQUALS( fields [ 13 ].name ( ).get ( ), string ( "Double" ) );     TEST_EQUALS_FLOAT( fields [ 13 ].getFloat64 ( ), 0.27362, 0.000001 );

    TEST_EQUALS_INT( fields [ 14 ].type ( ), FUDGE_TYPE_STRING );   TEST_EQUALS( fields [ 14 ].name ( ).get ( ), string ( "String" ) );     TEST_EQUALS( fields [ 14 ].getString ( ), string ( "Kirk Wylie" ) );

    std::vector<fudge_f32> floatArray,  emptyFloatArray ( 24, 0.0f );
    std::vector<fudge_f64> doubleArray, emptyDoubleArray ( 273, 0.0 );
    std::vector<fudge_i16> shortArray,  emptyShortArray ( 32, 0 );
    std::vector<fudge_i32> intArray,    emptyIntArray ( 83, 0 );
    std::vector<fudge_i64> longArray,   emptyLongArray ( 837, 0ll );

    TEST_EQUALS_INT( fields [ 15 ].type ( ), FUDGE_TYPE_FLOAT_ARRAY );  TEST_EQUALS( fields [ 15 ].name ( ).get ( ), string ( "float array" ) );    TEST_EQUALS_INT( fields [ 15 ].getArray ( floatArray ), emptyFloatArray.size ( ) );
    TEST_EQUALS_INT( fields [ 16 ].type ( ), FUDGE_TYPE_DOUBLE_ARRAY ); TEST_EQUALS( fields [ 16 ].name ( ).get ( ), string ( "double array" ) );   TEST_EQUALS_INT( fields [ 16 ].getArray ( doubleArray ), emptyDoubleArray.size ( ) );
    TEST_EQUALS_INT( fields [ 17 ].type ( ), FUDGE_TYPE_SHORT_ARRAY );  TEST_EQUALS( fields [ 17 ].name ( ).get ( ), string ( "short array" ) );    TEST_EQUALS_INT( fields [ 17 ].getArray ( shortArray ), emptyShortArray.size ( ) );
    TEST_EQUALS_INT( fields [ 18 ].type ( ), FUDGE_TYPE_INT_ARRAY );    TEST_EQUALS( fields [ 18 ].name ( ).get ( ), string ( "int array" ) );      TEST_EQUALS_INT( fields [ 18 ].getArray ( intArray ), emptyIntArray.size ( ) );
    TEST_EQUALS_INT( fields [ 19 ].type ( ), FUDGE_TYPE_LONG_ARRAY );   TEST_EQUALS( fields [ 19 ].name ( ).get ( ), string ( "long array" ) );     TEST_EQUALS_INT( fields [ 19 ].getArray ( longArray ), emptyLongArray.size ( ) );

    // All the vectors should be empty
    TEST_EQUALS_VECTOR( floatArray,  emptyFloatArray );
    TEST_EQUALS_VECTOR( doubleArray, emptyDoubleArray );
    TEST_EQUALS_VECTOR( shortArray,  emptyShortArray );
    TEST_EQUALS_VECTOR( intArray,    emptyIntArray );
    TEST_EQUALS_VECTOR( longArray,   emptyLongArray );
END_TEST

DEFINE_TEST( DecodeFixedWidths )
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( FixedWidth_Filename ) );

    // The arrays are all populated in the same manner
    fudge_byte bytes [ 512 ];
    for ( size_t index ( 0 ); index < sizeof ( bytes ); ++index )
        bytes [ index ] = static_cast<fudge_byte> ( index );

    // Get the message fields
    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 10 );

    // Check the field contents
    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_4 );      TEST_EQUALS( fields [ 0 ].name ( ).get ( ), string ( "byte[4]" ) );     TEST_EQUALS_MEMORY ( fields [ 0 ].bytes ( ), fields [ 0 ].numbytes ( ), bytes, 4 );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_8 );      TEST_EQUALS( fields [ 1 ].name ( ).get ( ), string ( "byte[8]" ) );     TEST_EQUALS_MEMORY ( fields [ 1 ].bytes ( ), fields [ 1 ].numbytes ( ), bytes, 8 );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_16 );     TEST_EQUALS( fields [ 2 ].name ( ).get ( ), string ( "byte[16]" ) );    TEST_EQUALS_MEMORY ( fields [ 2 ].bytes ( ), fields [ 2 ].numbytes ( ), bytes, 16 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_20 );     TEST_EQUALS( fields [ 3 ].name ( ).get ( ), string ( "byte[20]" ) );    TEST_EQUALS_MEMORY ( fields [ 3 ].bytes ( ), fields [ 3 ].numbytes ( ), bytes, 20 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_32 );     TEST_EQUALS( fields [ 4 ].name ( ).get ( ), string ( "byte[32]" ) );    TEST_EQUALS_MEMORY ( fields [ 4 ].bytes ( ), fields [ 4 ].numbytes ( ), bytes, 32 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_64 );     TEST_EQUALS( fields [ 5 ].name ( ).get ( ), string ( "byte[64]" ) );    TEST_EQUALS_MEMORY ( fields [ 5 ].bytes ( ), fields [ 5 ].numbytes ( ), bytes, 64 );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_128 );    TEST_EQUALS( fields [ 6 ].name ( ).get ( ), string ( "byte[128]" ) );   TEST_EQUALS_MEMORY ( fields [ 6 ].bytes ( ), fields [ 6 ].numbytes ( ), bytes, 128 );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_256 );    TEST_EQUALS( fields [ 7 ].name ( ).get ( ), string ( "byte[256]" ) );   TEST_EQUALS_MEMORY ( fields [ 7 ].bytes ( ), fields [ 7 ].numbytes ( ), bytes, 256 );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_512 );    TEST_EQUALS( fields [ 8 ].name ( ).get ( ), string ( "byte[512]" ) );   TEST_EQUALS_MEMORY ( fields [ 8 ].bytes ( ), fields [ 8 ].numbytes ( ), bytes, 512 );

    TEST_EQUALS_INT( fields [ 9 ].type ( ), FUDGE_TYPE_BYTE_ARRAY );        TEST_EQUALS( fields [ 9 ].name ( ).get ( ), string ( "byte[28]" ) );    TEST_EQUALS_MEMORY ( fields [ 9 ].bytes ( ), fields [ 9 ].numbytes ( ), bytes, 28 );
END_TEST

DEFINE_TEST( DecodeAllOrdinals )
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( AllOrdinals_Filename ) );

    // Get the message fields
    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 17 );

    // None of the fields should have names 
    for ( size_t index ( 0 ); index < fields.size ( ); ++index )
        TEST_EQUALS_TRUE( ! message1.getFieldAt ( index ).name ( ) );

    // Check the field ordinals and contents
    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BOOLEAN );   TEST_EQUALS_INT( fields [ 0 ].ordinal ( ).get ( ), 1 );     TEST_EQUALS( fields [ 0 ].getBoolean ( ), true );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BOOLEAN );   TEST_EQUALS_INT( fields [ 1 ].ordinal ( ).get ( ), 2 );     TEST_EQUALS( fields [ 1 ].getBoolean ( ), false );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 2 ].ordinal ( ).get ( ), 3 );     TEST_EQUALS_INT( fields [ 2 ].getByte ( ), 5 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 3 ].ordinal ( ).get ( ), 4 );     TEST_EQUALS_INT( fields [ 3 ].getByte ( ), 5 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 4 ].ordinal ( ).get ( ), 5 );     TEST_EQUALS_INT( fields [ 4 ].getInt16 ( ), 127 + 5 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 5 ].ordinal ( ).get ( ), 6 );     TEST_EQUALS_INT( fields [ 5 ].getInt16 ( ), 127 + 5 );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 6 ].ordinal ( ).get ( ), 7 );     TEST_EQUALS_INT( fields [ 6 ].getInt32 ( ), 32767 + 5 );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 7 ].ordinal ( ).get ( ), 8 );     TEST_EQUALS_INT( fields [ 7 ].getInt32 ( ), 32767 + 5 );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_LONG );      TEST_EQUALS_INT( fields [ 8 ].ordinal ( ).get ( ), 9 );     TEST_EQUALS_INT( fields [ 8 ].getInt64 ( ), 2147483647ll + 5ll );
    TEST_EQUALS_INT( fields [ 9 ].type ( ), FUDGE_TYPE_LONG );      TEST_EQUALS_INT( fields [ 9 ].ordinal ( ).get ( ), 10 );    TEST_EQUALS_INT( fields [ 9 ].getInt64 ( ), 2147483647ll + 5ll );
    TEST_EQUALS_INT( fields [ 10 ].type ( ), FUDGE_TYPE_FLOAT );    TEST_EQUALS_INT( fields [ 10 ].ordinal ( ).get ( ), 11 );   TEST_EQUALS_FLOAT( fields [ 10 ].getFloat32 ( ), 0.5f, 0.00001f );
    TEST_EQUALS_INT( fields [ 11 ].type ( ), FUDGE_TYPE_FLOAT );    TEST_EQUALS_INT( fields [ 11 ].ordinal ( ).get ( ), 12 );   TEST_EQUALS_FLOAT( fields [ 11 ].getFloat32 ( ), 0.5f, 0.00001f );
    TEST_EQUALS_INT( fields [ 12 ].type ( ), FUDGE_TYPE_DOUBLE );   TEST_EQUALS_INT( fields [ 12 ].ordinal ( ).get ( ), 13 );   TEST_EQUALS_FLOAT( fields [ 12 ].getFloat64 ( ), 0.27362, 0.000001 );
    TEST_EQUALS_INT( fields [ 13 ].type ( ), FUDGE_TYPE_DOUBLE );   TEST_EQUALS_INT( fields [ 13 ].ordinal ( ).get ( ), 14 );   TEST_EQUALS_FLOAT( fields [ 13 ].getFloat64 ( ), 0.27362, 0.000001 );

    TEST_EQUALS_INT( fields [ 14 ].type ( ), FUDGE_TYPE_STRING );   TEST_EQUALS_INT( fields [ 14 ].ordinal ( ).get ( ), 15 );   TEST_EQUALS( fields [ 14 ].getString ( ), string ( "Kirk Wylie" ) );

    std::vector<fudge_f32> floatArray,  emptyFloatArray ( 24, 0.0f );
    std::vector<fudge_f64> doubleArray, emptyDoubleArray ( 273, 0.0 );

    TEST_EQUALS_INT( fields [ 15 ].type ( ), FUDGE_TYPE_FLOAT_ARRAY );  TEST_EQUALS_INT( fields [ 15 ].ordinal ( ).get ( ), 16 );   TEST_EQUALS_INT( fields [ 15 ].getArray ( floatArray ), emptyFloatArray.size ( ) );
    TEST_EQUALS_INT( fields [ 16 ].type ( ), FUDGE_TYPE_DOUBLE_ARRAY ); TEST_EQUALS_INT( fields [ 16 ].ordinal ( ).get ( ), 17 );   TEST_EQUALS_INT( fields [ 16 ].getArray ( doubleArray ), emptyDoubleArray.size ( ) );

    // All the vectors should be empty
    TEST_EQUALS_VECTOR( floatArray,  emptyFloatArray );
    TEST_EQUALS_VECTOR( doubleArray, emptyDoubleArray );
END_TEST

DEFINE_TEST( DecodeSubMsgs )
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( SubMsg_Filename ) );

    // Check the message contents
    TEST_EQUALS_INT( message1.size ( ), 2 );

    field field1 ( message1.getFieldAt ( 0 ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS( field1.name ( ).get ( ), string ( "sub1" ) );
    message message2 ( field1.getMessage ( ) );
    TEST_EQUALS_INT( message2.size ( ), 2 );

    field field2 ( message2.getFieldAt ( 0 ) );
    TEST_EQUALS_INT( field2.type ( ), FUDGE_TYPE_STRING );
    TEST_EQUALS( field2.name ( ).get ( ), string ( "bibble" ) );
    TEST_EQUALS( field2.getString ( ), string ( "fibble" ) );

    field2 = message2.getFieldAt ( 1 );
    TEST_EQUALS_INT( field2.type ( ), FUDGE_TYPE_STRING );
    TEST_EQUALS_INT( field2.ordinal ( ).get ( ), 827 );
    TEST_EQUALS( field2.getString ( ), string ( "Blibble" ) );

    field1 = message1.getFieldAt ( 1 );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS( field1.name ( ).get ( ), string ( "sub2" ) );

    message2 = field1.getMessage ( );
    TEST_EQUALS_INT( message2.size ( ), 2 );

    field2 = message2.getFieldAt ( 0 );
    TEST_EQUALS_INT( field2.type ( ), FUDGE_TYPE_INT );
    TEST_EQUALS( field2.name ( ).get ( ), string ( "bibble9" ) );
    TEST_EQUALS_INT( field2.getInt32 ( ), 9837438 );

    field2 = message2.getFieldAt ( 1 );
    TEST_EQUALS_INT( field2.type ( ), FUDGE_TYPE_FLOAT );
    TEST_EQUALS_INT( field2.ordinal ( ).get ( ), 828 );
    TEST_EQUALS_FLOAT( field2.getFloat32 ( ), 82.77f, 0.00001f );
END_TEST

DEFINE_TEST( DecodeUnknown )
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // "Unknown" type is just 10 empty bytes
    fudge_byte empty [ 10 ];
    memset ( empty, 0, sizeof ( empty ) );

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( Unknown_Filename ) );

    // Check field contents
    TEST_EQUALS_INT( message1.size ( ), 1 );
    field field1 ( message1.getField ( string ( "unknown" ) ) );
    TEST_EQUALS_INT( field1.type ( ), 200 );
    TEST_EQUALS( field1.name ( ).get ( ), string ( "unknown" ) );
    TEST_EQUALS_MEMORY( field1.bytes ( ), field1.numbytes ( ), empty, sizeof ( empty ) );
END_TEST

DEFINE_TEST( DecodeVariableWidths )
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // All byte arrays are empty
    fudge_byte empty [ 100000 ];
    memset ( empty, 0, sizeof ( empty ) );

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( VariableWidth_Filename ) );

    // Check field contents
    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 3 );

    // All fields are byte arrays, with names but no ordinals
    for ( std::vector<field>::const_iterator fieldIt ( fields.begin ( ) ); fieldIt != fields.end ( ); ++fieldIt )
    {
        TEST_EQUALS_INT( fieldIt->type ( ), FUDGE_TYPE_BYTE_ARRAY );
        TEST_EQUALS_TRUE( fieldIt->name ( ) );
        TEST_EQUALS_TRUE( ! fieldIt->ordinal ( ) );
    }

    TEST_EQUALS( fields [ 0 ].name ( ).get ( ), string ( "100" ) );     TEST_EQUALS_MEMORY( fields [ 0 ].bytes ( ), fields [ 0 ].numbytes ( ), empty, 100 );
    TEST_EQUALS( fields [ 1 ].name ( ).get ( ), string ( "1000" ) );    TEST_EQUALS_MEMORY( fields [ 1 ].bytes ( ), fields [ 1 ].numbytes ( ), empty, 1000 );
    TEST_EQUALS( fields [ 2 ].name ( ).get ( ), string ( "10000" ) );   TEST_EQUALS_MEMORY( fields [ 2 ].bytes ( ), fields [ 2 ].numbytes ( ), empty, 100000 );
END_TEST

DEFINE_TEST( DecodeDateTimes )
    using fudge::date;
    using fudge::field;
    using fudge::message;
    using fudge::string;
    using fudge::time;

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( DateTimes_Filename ) );

    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 16 );

    // Check date field contents
    TEST_EQUALS( fields [ 0 ].name ( ).get ( ), string ( "date-Year" ) );           TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_DATE );
    TEST_EQUALS( fields [ 1 ].name ( ).get ( ), string ( "date-Month" ) );          TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_DATE );
    TEST_EQUALS( fields [ 2 ].name ( ).get ( ), string ( "date-Day" ) );            TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_DATE );
    TEST_EQUALS_TRUE( fields [ 0 ].getDate ( ) == date ( 2010, 0, 0 ) );
    TEST_EQUALS_TRUE( fields [ 1 ].getDate ( ) == date ( 2010, 3, 0 ) );
    TEST_EQUALS_TRUE( fields [ 2 ].getDate ( ) == date ( 2010, 3, 4 ) );

    // Check time field contents
    TEST_EQUALS( fields [  3 ].name ( ).get ( ), string ( "time-Hour-UTC" ) );      TEST_EQUALS_INT( fields [  3 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS( fields [  4 ].name ( ).get ( ), string ( "time-Minute-UTC" ) );    TEST_EQUALS_INT( fields [  4 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS( fields [  5 ].name ( ).get ( ), string ( "time-Second-UTC" ) );    TEST_EQUALS_INT( fields [  5 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS( fields [  6 ].name ( ).get ( ), string ( "time-Milli-UTC" ) );     TEST_EQUALS_INT( fields [  6 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS( fields [  7 ].name ( ).get ( ), string ( "time-Micro-UTC" ) );     TEST_EQUALS_INT( fields [  7 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS( fields [  8 ].name ( ).get ( ), string ( "time-Nano-UTC" ) );      TEST_EQUALS_INT( fields [  8 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS( fields [  9 ].name ( ).get ( ), string ( "time-Nano" ) );          TEST_EQUALS_INT( fields [  9 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS( fields [ 10 ].name ( ).get ( ), string ( "time-Nano-+1h" ) );      TEST_EQUALS_INT( fields [ 10 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS_TRUE( fields [  3 ].getTime ( ) == time ( 11 * 3600,                        0, FUDGE_DATETIME_PRECISION_HOUR ) );
    TEST_EQUALS_TRUE( fields [  4 ].getTime ( ) == time ( 11 * 3600 + 12 * 60,              0, FUDGE_DATETIME_PRECISION_MINUTE ) );
    TEST_EQUALS_TRUE( fields [  5 ].getTime ( ) == time ( 11 * 3600 + 12 * 60 + 13,         0, FUDGE_DATETIME_PRECISION_SECOND ) );
    TEST_EQUALS_TRUE( fields [  6 ].getTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987000000, FUDGE_DATETIME_PRECISION_MILLISECOND ) );
    TEST_EQUALS_TRUE( fields [  7 ].getTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987654000, FUDGE_DATETIME_PRECISION_MICROSECOND ) );
    TEST_EQUALS_TRUE( fields [  8 ].getTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987654321, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_TRUE( fields [  9 ].getTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987654321, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_TRUE( fields [ 10 ].getTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987654321, FUDGE_DATETIME_PRECISION_NANOSECOND, 4 ) );

    // Check datetime field contents
    TEST_EQUALS( fields [ 11 ].name ( ).get ( ), string ( "datetime-Millenia" ) );   TEST_EQUALS_INT( fields [ 11 ].type ( ), FUDGE_TYPE_DATETIME );
    TEST_EQUALS( fields [ 12 ].name ( ).get ( ), string ( "datetime-Century" ) );    TEST_EQUALS_INT( fields [ 12 ].type ( ), FUDGE_TYPE_DATETIME );
    TEST_EQUALS( fields [ 13 ].name ( ).get ( ), string ( "datetime-Nano-UTC" ) );   TEST_EQUALS_INT( fields [ 13 ].type ( ), FUDGE_TYPE_DATETIME );
    TEST_EQUALS( fields [ 14 ].name ( ).get ( ), string ( "datetime-Nano" ) );       TEST_EQUALS_INT( fields [ 14 ].type ( ), FUDGE_TYPE_DATETIME );
    TEST_EQUALS( fields [ 15 ].name ( ).get ( ), string ( "datetime-Nano-+1h" ) );   TEST_EQUALS_INT( fields [ 15 ].type ( ), FUDGE_TYPE_DATETIME );
    TEST_EQUALS_TRUE( fields [ 11 ].getDateTime ( ) == date ( 1000, 0, 0 ) );
    TEST_EQUALS_TRUE( fields [ 12 ].getDateTime ( ) == date ( 1900, 0, 0 ) );
    TEST_EQUALS_TRUE( fields [ 13 ].getDateTime ( ) == date ( 2010, 3, 4 ) );
    TEST_EQUALS_TRUE( fields [ 14 ].getDateTime ( ) == date ( 2010, 3, 4 ) );
    TEST_EQUALS_TRUE( fields [ 15 ].getDateTime ( ) == date ( 2010, 3, 4 ) );
    TEST_EQUALS_TRUE( fields [ 11 ].getDateTime ( ) == time (                        0,         0, FUDGE_DATETIME_PRECISION_MILLENNIUM ) );
    TEST_EQUALS_TRUE( fields [ 12 ].getDateTime ( ) == time (                        0,         0, FUDGE_DATETIME_PRECISION_CENTURY ) );
    TEST_EQUALS_TRUE( fields [ 13 ].getDateTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987654321, FUDGE_DATETIME_PRECISION_NANOSECOND, 0 ) );
    TEST_EQUALS_TRUE( fields [ 14 ].getDateTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987654321, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_TRUE( fields [ 15 ].getDateTime ( ) == time ( 11 * 3600 + 12 * 60 + 13, 987654321, FUDGE_DATETIME_PRECISION_NANOSECOND, 4 ) );
END_TEST

DEFINE_TEST( DecodeDeepTree )
    using fudge::date;
    using fudge::field;
    using fudge::message;
    using fudge::string;
    using fudge::time;

    // Initialise comparison arrays
    fudge_byte bytes [ 512 ], empty [ 128 ];
    std::vector<fudge_i16> shorts ( 15, 0 );
    std::vector<fudge_i32> ints ( 15, 0 );
    std::vector<fudge_i64> longs ( 15, 0 );
    std::vector<fudge_f32> floats ( 15, 0.0f );
    std::vector<fudge_f64> doubles ( 15, 0.0 );

    memset ( empty, 0, sizeof ( empty ) );
    for ( size_t index ( 0 ); index < sizeof ( bytes ); ++index )   bytes [ index ] = static_cast<fudge_byte> ( index );
    for ( size_t index ( 0 ); index < shorts.size ( ); ++index )    shorts [ index ] = static_cast<fudge_i16> ( index );
    for ( size_t index ( 0 ); index < doubles.size ( ); ++index )   doubles [ index ] = ( static_cast<fudge_i64> ( index ) ) / 10.0;

    // Load and decode the source message
    message message1 ( loadFudgeMessage ( Deeper_Filename ) );

    // Check the top-level fields
    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 13 );

    for ( size_t index ( 0 ); index < fields.size ( ); ++index )
    {
        TEST_EQUALS_TRUE( fields [ index ].name ( ) );
        TEST_EQUALS_TRUE( ! fields [ index ].ordinal ( ) );
    }

    TEST_EQUALS_INT( fields [  0 ].type ( ), FUDGE_TYPE_INDICATOR );    TEST_EQUALS( fields [  0 ].name ( ).get ( ), string ( "Indicator" ) );
    TEST_EQUALS_INT( fields [  1 ].type ( ), FUDGE_TYPE_BOOLEAN );      TEST_EQUALS( fields [  1 ].name ( ).get ( ), string ( "Boolean" ) );        TEST_EQUALS_TRUE( fields [ 1 ].getBoolean ( ) );
    TEST_EQUALS_INT( fields [  2 ].type ( ), FUDGE_TYPE_BYTE );         TEST_EQUALS( fields [  2 ].name ( ).get ( ), string ( "Byte" ) );           TEST_EQUALS_INT( fields [ 2 ].getByte ( ), -128 );
    TEST_EQUALS_INT( fields [  3 ].type ( ), FUDGE_TYPE_SHORT );        TEST_EQUALS( fields [  3 ].name ( ).get ( ), string ( "Short" ) );          TEST_EQUALS_INT( fields [ 3 ].getInt16 ( ), -32768 );
    TEST_EQUALS_INT( fields [  4 ].type ( ), FUDGE_TYPE_INT );          TEST_EQUALS( fields [  4 ].name ( ).get ( ), string ( "Int" ) );            TEST_EQUALS_INT( fields [ 4 ].getInt32 ( ), 2147483647 );
    TEST_EQUALS_INT( fields [  5 ].type ( ), FUDGE_TYPE_LONG );         TEST_EQUALS( fields [  5 ].name ( ).get ( ), string ( "Long" ) );           TEST_EQUALS_INT( fields [ 5 ].getInt64 ( ), 9223372036854775807ll );
    TEST_EQUALS_INT( fields [  6 ].type ( ), FUDGE_TYPE_FLOAT );        TEST_EQUALS( fields [  6 ].name ( ).get ( ), string ( "Float" ) );          TEST_EQUALS_FLOAT( fields [ 6 ].getFloat32 ( ), 1.2346f, 0.0001f );
    TEST_EQUALS_INT( fields [  7 ].type ( ), FUDGE_TYPE_DOUBLE );       TEST_EQUALS( fields [  7 ].name ( ).get ( ), string ( "Double" ) );         TEST_EQUALS_FLOAT( fields [ 7 ].getFloat64 ( ), 1.2345678, 0.000001 );
    TEST_EQUALS_INT( fields [  8 ].type ( ), FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS( fields [  8 ].name ( ).get ( ), string ( "ByteArrays" ) );
    TEST_EQUALS_INT( fields [  9 ].type ( ), FUDGE_TYPE_STRING );       TEST_EQUALS( fields [  9 ].name ( ).get ( ), string ( "Empty String" ) );   TEST_EQUALS_INT( fields [ 9 ].getString ( ).size ( ), 0 );
    TEST_EQUALS_INT( fields [ 10 ].type ( ), FUDGE_TYPE_STRING );       TEST_EQUALS( fields [ 10 ].name ( ).get ( ), string ( "String" ) );         TEST_EQUALS( fields [ 10 ].getString ( ), string ( "This is a string." ) );
    TEST_EQUALS_INT( fields [ 11 ].type ( ), FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS( fields [ 11 ].name ( ).get ( ), string ( "Arrays" ) );
    TEST_EQUALS_INT( fields [ 12 ].type ( ), FUDGE_TYPE_FUDGE_MSG );    TEST_EQUALS( fields [ 12 ].name ( ).get ( ), string ( "Null Message" ) );

    // Check the fixed byte array message
    message bytemessage ( fields [ 8 ].getMessage ( ) );
    bytemessage.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 9 );

    for ( size_t index ( 0 ); index < fields.size ( ); ++index )
    {
        TEST_EQUALS_TRUE( ! fields [ index ].name ( ) );
        TEST_EQUALS_TRUE( fields [ index ].ordinal ( ) );
    }

    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_4 );      TEST_EQUALS_INT( fields [ 0 ].ordinal ( ).get ( ), 4 );     TEST_EQUALS_MEMORY( fields [ 0 ].bytes ( ), fields [ 0 ].numbytes ( ), bytes, 4 );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_8 );      TEST_EQUALS_INT( fields [ 1 ].ordinal ( ).get ( ), 8 );     TEST_EQUALS_MEMORY( fields [ 1 ].bytes ( ), fields [ 1 ].numbytes ( ), bytes, 8 );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_16 );     TEST_EQUALS_INT( fields [ 2 ].ordinal ( ).get ( ), 16 );    TEST_EQUALS_MEMORY( fields [ 2 ].bytes ( ), fields [ 2 ].numbytes ( ), bytes, 16 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_20 );     TEST_EQUALS_INT( fields [ 3 ].ordinal ( ).get ( ), 20 );    TEST_EQUALS_MEMORY( fields [ 3 ].bytes ( ), fields [ 3 ].numbytes ( ), bytes, 20 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_32 );     TEST_EQUALS_INT( fields [ 4 ].ordinal ( ).get ( ), 32 );    TEST_EQUALS_MEMORY( fields [ 4 ].bytes ( ), fields [ 4 ].numbytes ( ), bytes, 32 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_64);      TEST_EQUALS_INT( fields [ 5 ].ordinal ( ).get ( ), 64 );    TEST_EQUALS_MEMORY( fields [ 5 ].bytes ( ), fields [ 5 ].numbytes ( ), bytes, 64 );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_128 );    TEST_EQUALS_INT( fields [ 6 ].ordinal ( ).get ( ), 128 );   TEST_EQUALS_MEMORY( fields [ 6 ].bytes ( ), fields [ 6 ].numbytes ( ), bytes, 128 );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_256 );    TEST_EQUALS_INT( fields [ 7 ].ordinal ( ).get ( ), 256 );   TEST_EQUALS_MEMORY( fields [ 7 ].bytes ( ), fields [ 7 ].numbytes ( ), bytes, 256 );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_512 );    TEST_EQUALS_INT( fields [ 8 ].ordinal ( ).get ( ), 512 );   TEST_EQUALS_MEMORY( fields [ 8 ].bytes ( ), fields [ 8 ].numbytes ( ), bytes, 512 );

    // Check the arrays message
    message arraysmessage ( fields [ 11 ].getMessage ( ) );
    arraysmessage.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 9 );

    for ( size_t index ( 0 ); index < fields.size ( ); ++index )
    {
        TEST_EQUALS_TRUE( fields [ index ].name ( ) );
        TEST_EQUALS_TRUE( ! fields [ index ].ordinal ( ) );
    }

    std::vector<fudge_i16> fieldShorts;
    std::vector<fudge_i32> fieldInts;
    std::vector<fudge_i64> fieldLongs;

    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BYTE_ARRAY );        TEST_EQUALS( fields [ 0 ].name ( ).get ( ), string ( "Byte[0]"  ) );    TEST_EQUALS_MEMORY( fields [ 0 ].bytes ( ), fields [ 0 ].numbytes ( ), 0, 0 );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BYTE_ARRAY );        TEST_EQUALS( fields [ 1 ].name ( ).get ( ), string ( "Byte[15]"  ) );   TEST_EQUALS_MEMORY( fields [ 1 ].bytes ( ), fields [ 1 ].numbytes ( ), bytes, 15 );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_FUDGE_MSG );         TEST_EQUALS( fields [ 2 ].name ( ).get ( ), string ( "FP Arrays"  ) );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_SHORT_ARRAY );       TEST_EQUALS( fields [ 3 ].name ( ).get ( ), string ( "Short[0]"  ) );   TEST_EQUALS_INT( fields [ 3 ].getArray ( fieldShorts ), 0 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_SHORT_ARRAY );       TEST_EQUALS( fields [ 4 ].name ( ).get ( ), string ( "Short[15]"  ) );  TEST_EQUALS_INT( fields [ 4 ].getArray ( fieldShorts ), 15 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_INT_ARRAY );         TEST_EQUALS( fields [ 5 ].name ( ).get ( ), string ( "Int[0]"  ) );     TEST_EQUALS_INT( fields [ 5 ].getArray ( fieldInts ), 0 );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_INT_ARRAY );         TEST_EQUALS( fields [ 6 ].name ( ).get ( ), string ( "Int[15]"  ) );    TEST_EQUALS_INT( fields [ 6 ].getArray ( fieldInts ), 15 );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_LONG_ARRAY );        TEST_EQUALS( fields [ 7 ].name ( ).get ( ), string ( "Long[0]"  ) );    TEST_EQUALS_INT( fields [ 7 ].getArray ( fieldLongs ), 0 );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_LONG_ARRAY );        TEST_EQUALS( fields [ 8 ].name ( ).get ( ), string ( "Long[15]"  ) );   TEST_EQUALS_INT( fields [ 8 ].getArray ( fieldLongs ), 15 );

    TEST_EQUALS_VECTOR( fieldShorts, shorts );
    TEST_EQUALS_VECTOR( fieldInts, ints );
    TEST_EQUALS_VECTOR( fieldLongs, longs );

    // Check the FP arrays message
    message fparraymessage ( fields [ 2 ].getMessage ( ) );
    fparraymessage.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 4 );

    for ( size_t index ( 0 ); index < fields.size ( ); ++index )
    {
        TEST_EQUALS_TRUE( fields [ index ].name ( ) );
        TEST_EQUALS_TRUE( ! fields [ index ].ordinal ( ) );
    }

    std::vector<fudge_f32> fieldFloats;
    std::vector<fudge_f64> fieldDoubles;

    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_FLOAT_ARRAY );       TEST_EQUALS( fields [ 0 ].name ( ).get ( ), string ( "Float[0]"  ) );   TEST_EQUALS_INT( fields [ 0 ].getArray ( fieldFloats ), 0 );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_FLOAT_ARRAY );       TEST_EQUALS( fields [ 1 ].name ( ).get ( ), string ( "Float[15]"  ) );  TEST_EQUALS_INT( fields [ 1 ].getArray ( fieldFloats ), 15 );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_DOUBLE_ARRAY );      TEST_EQUALS( fields [ 2 ].name ( ).get ( ), string ( "Double[0]"  ) );  TEST_EQUALS_INT( fields [ 2 ].getArray ( fieldDoubles ), 0 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_DOUBLE_ARRAY );      TEST_EQUALS( fields [ 3 ].name ( ).get ( ), string ( "Double[15]"  ) ); TEST_EQUALS_INT( fields [ 3 ].getArray ( fieldDoubles ), 15 );

    TEST_EQUALS_VECTOR( fieldFloats, floats );
    TEST_EQUALS_VECTOR( fieldDoubles, doubles );

    // Check the empty message
    message nullmessage ( message1.getField ( string ( "Null Message" ) ).getMessage ( ) );
    TEST_EQUALS_INT( nullmessage.size ( ), 0 );
END_TEST

DEFINE_TEST( EncodeAllNames )
    using fudge::codec;
    using fudge::envelope;
    using fudge::message;
    using fudge::string;

    // Construct the message
    message message1;
    message1.addField ( true, string ( "boolean" ) );
    message1.addField ( false, string ( "Boolean" ) );
    message1.addField ( 5, string ( "byte" ) );
    message1.addField ( 5, string ( "Byte" ) );
    message1.addField ( 127 + 5, string ( "short" ) );
    message1.addField ( 127 + 5, string ( "Short" ) );
    message1.addField ( 32767 + 5, string ( "int" ) );
    message1.addField ( 32767 + 5, string ( "Integer" ) );
    message1.addField ( 2147483647ll + 5, string ( "long" ) );
    message1.addField ( 2147483647ll + 5, string ( "Long" ) );
    message1.addField ( 0.5f, string ( "float" ) );
    message1.addField ( 0.5f, string ( "Float" ) );
    message1.addField ( 0.27362, string ( "double" ) );
    message1.addField ( 0.27362, string ( "Double" ) );

    message1.addField ( string ( "Kirk Wylie" ), string ( "String" ) );

    message1.addField ( std::vector<fudge_f32> ( 24, 0.0f ), string ( "float array" ) );
    message1.addField ( std::vector<fudge_f64> ( 273, 0.0 ), string ( "double array" ) );
    message1.addField ( std::vector<fudge_i16> ( 32, 0 ), string ( "short array" ) );
    message1.addField ( std::vector<fudge_i32> ( 83, 0 ), string ( "int array" ) );
    message1.addField ( std::vector<fudge_i64> ( 837, 0 ), string ( "long array" ) );

    message1.addField ( string ( "indicator" ), message::noordinal );

    // Wrap the message in an envelope
    envelope envelope1 ( 0, 0, 0, message1 );

    // Encode the message
    fudge_byte * encoded, * reference;
    fudge_i32 encodedsize, referencesize;
    codec codec1;
    TEST_THROWS_NOTHING( codec1.encode ( envelope1, encoded, encodedsize ) );

    // Load the reference the file and compare it with the encoded message
    loadFile ( AllNames_Filename, reference, referencesize );
    TEST_EQUALS_MEMORY( encoded, encodedsize, reference, referencesize );

    delete [] encoded;
    delete [] reference;
END_TEST

DEFINE_TEST_SUITE( Codec )
    // Interop decode test files
    REGISTER_TEST( DecodeAllNames )
    REGISTER_TEST( DecodeFixedWidths )
    REGISTER_TEST( DecodeAllOrdinals )
    REGISTER_TEST( DecodeSubMsgs )
    REGISTER_TEST( DecodeUnknown )
    REGISTER_TEST( DecodeVariableWidths )
    REGISTER_TEST( DecodeDateTimes )
    
    // Other decode test files
    REGISTER_TEST( DecodeDeepTree )

    // Interop encode tests
    REGISTER_TEST( EncodeAllNames )
    // TODO REGISTER_TEST( EncodeFixedWidths )
    // TODO REGISTER_TEST( EncodeAllOrdinals )
    // TODO REGISTER_TEST( EncodeUnknown )
    // TODO REGISTER_TEST( EncodeSubMsgs )
    // TODO REGISTER_TEST( EncodeVariableWidths )
    // TODO REGISTER_TEST( EncodeDateTimes )

    // Other encode tests
    // TODO REGISTER_TEST( EncodeDeepTree );
END_TEST_SUITE

namespace
{
    void loadFile ( const std::string & filename, fudge_byte * & bytes, fudge_i32 & numbytes )
    {
        // When running "make distcheck" the tests will be triggered from the
        // binary directory, which makes the filenames invalid. Helpfully automake
        // sets the "srcdir" env var which contains the location of the source
        // code directory.
        std::string fullfilename;
        char * srcdir;
        if ( ( srcdir = getenv ( "srcdir" ) ) )
            fullfilename = srcdir + std::string ( "/" );
        fullfilename += filename;

        // Open the binary file
        std::ifstream input ( fullfilename.c_str ( ), std::ifstream::in | std::ifstream::binary );
        if ( ! input.is_open ( ) )
            throw simpletest::SimpleTestException ( "Failed to open \"" + fullfilename + "\"", __FILE__, __LINE__ );

        // Read in the file a KB at a time
        char buffer [ 1024 ];
        size_t numread;
        bytes = 0;
        numbytes = 0;
        while ( input.good ( ) )
        {
            input.read ( buffer, sizeof ( buffer ) );
            if ( ! ( numread = input.gcount ( ) ) )
                break;
    
            if ( bytes )
            {
                fudge_byte * oldbytes ( bytes );
                bytes = new fudge_byte [ numbytes + numread ];
                memcpy ( bytes, oldbytes, numbytes );
                delete [] oldbytes;
            }
            else
                bytes = new fudge_byte [ numread ];

            memcpy ( bytes + numbytes, buffer, numread );
            numbytes += numread;
        }

        // Clean up
        input.close ( );
    }

    fudge::message loadFudgeMessage ( const std::string & filename )
    {
        fudge_byte * bytes;
        fudge_i32 numbytes;
        loadFile ( filename, bytes, numbytes );

        fudge::codec codec;
        fudge::envelope decoded ( codec.decode ( bytes, numbytes ) );
        delete [] bytes;
        return decoded.payload ( );
    }
}

