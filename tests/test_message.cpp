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
#include "fudge-cpp/message.hpp"

// TODO Remove this
#include <iostream>

DEFINE_TEST( FieldFunctions )
    using fudge::date;
    using fudge::datetime;
    using fudge::exception;
    using fudge::message;
    using fudge::string;
    using fudge::time;

    // Set up the source data
    static const fudge_byte rawBytesArray [ 16 ] = { 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
    static const fudge_i16 rawShortsArray [ 10 ] = { -32767, 32767, 0, 1, -1, 100, -100, 0, 16385 };
    static const fudge_i32 rawIntsArray [ 4 ] = { 2147483647, 0, -2147483647, 0 };
    static const fudge_i64 rawLongsArray [ 12 ] = { ( fudge_i64 ) -9223372036854775807ll, 0, ( fudge_i64 ) 9223372036854775807ll, -1, 2, -3, 5, -8, 13, -21, 34, -55 };
    static const fudge_f32 rawFloatsArray [ 8 ] = { 0.0f, 2147483647.0f, 214748364.7f, 21474836.47f, 2147483.647f, 2.147483647f, 21.47483647f, 214.7483647f };
    static const fudge_f64 rawDoublesArray [ 5 ] = { 9223372036854775807.0, 0.0, 0.0000000123456, 1234560000000.0, -9223372036854775807.0 };

    std::vector<fudge_byte> rawBytes ( rawBytesArray, rawBytesArray + sizeof ( rawBytesArray ) / sizeof ( fudge_byte ) );
    std::vector<fudge_i16> rawShorts ( rawShortsArray, rawShortsArray + sizeof ( rawShortsArray ) / sizeof ( fudge_i16 ) );
    std::vector<fudge_i32> rawInts ( rawIntsArray, rawIntsArray + sizeof ( rawIntsArray ) / sizeof ( fudge_i32 ) );
    std::vector<fudge_i64> rawLongs ( rawLongsArray, rawLongsArray + sizeof ( rawLongsArray ) / sizeof ( fudge_i64 ) );
    std::vector<fudge_f32> rawFloats ( rawFloatsArray, rawFloatsArray + sizeof ( rawFloatsArray ) / sizeof ( fudge_f32 ) );
    std::vector<fudge_f64> rawDoubles ( rawDoublesArray, rawDoublesArray + sizeof ( rawDoublesArray ) / sizeof ( fudge_f64 ) );

    static const date exampleDate ( 2010, 3, 21 );
    static const time exampleTime ( 15 * 3600 + 39 * 60 + 27, 0, FUDGE_DATETIME_PRECISION_SECOND );
    static const datetime exampleDateTime ( exampleDate.year ( ), exampleDate.month ( ), exampleDate.day ( ), exampleTime.seconds ( ), exampleTime.nanoseconds ( ), exampleTime.precision ( ) );

    // Will need a large byte array for the fixed array testing
    fudge_byte largeByteArray [ 512 ];
    for ( size_t index ( 0 ); index < sizeof ( largeByteArray ); ++index )
        largeByteArray [ index ] = index % 256 - 128;

    // Construct an empty message
    message message1;
    TEST_EQUALS_INT( message1.size ( ), 0 );

    // Test some failure cases
    FudgeField field;
    TEST_THROWS_EXCEPTION( message1.getFieldAt ( field, 0 ), exception );                               // Invalid index
    TEST_THROWS_EXCEPTION( message1.getField ( field, static_cast<fudge_i16> ( 0 ) ), exception );      // Invalid ordinal
    TEST_THROWS_EXCEPTION( message1.getField ( field, string ( "Field Name" ) ), exception );           // Invalid field name

    TEST_THROWS_EXCEPTION( message1.addField ( true, string ( "This message field name is too long. The limit for message field names is 255 characters; this is because "
                                                              "the name's length must be stored in a single leading byte (a bit like a length limited version Pascal). An "
                                                              "exception should be thrown containing the FUDGE_NAME_TOO_LONG code." ) ),
                           exception );

    // Add indicator, boolean, integer and float fields
    TEST_THROWS_NOTHING( message1.addField ( ) );
    TEST_THROWS_NOTHING( message1.addField ( true ) );
    TEST_THROWS_NOTHING( message1.addField ( static_cast<fudge_byte> ( 127 ) ) );
    TEST_THROWS_NOTHING( message1.addField ( static_cast<fudge_i16> ( 32767 ) ) );
    TEST_THROWS_NOTHING( message1.addField ( static_cast<fudge_i32> ( -2147483647 ) ) );
    TEST_THROWS_NOTHING( message1.addField ( static_cast<fudge_i64> ( 9223372036854775807ll ) ) );
    TEST_THROWS_NOTHING( message1.addField ( static_cast<fudge_f32> ( 2147483647.0f ) ) );
    TEST_THROWS_NOTHING( message1.addField ( static_cast<fudge_f64> ( -9223372036854775807.0 ) ) );

    TEST_EQUALS_INT( message1.size ( ), 8 );

    // Add date, time and datetime fields
    TEST_THROWS_NOTHING( message1.addField ( exampleDate ) );
    TEST_THROWS_NOTHING( message1.addField ( exampleTime ) );
    TEST_THROWS_NOTHING( message1.addField ( exampleDateTime ) );

    TEST_EQUALS_INT( message1.size ( ), 11 );

    // Add fixed width byte arrays in a sub message
    message message2;
    TEST_THROWS_NOTHING( message2.addField4ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 1 ) ) );
    TEST_THROWS_NOTHING( message2.addField8ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 2 ) ) );
    TEST_THROWS_NOTHING( message2.addField16ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 3 ) ) );
    TEST_THROWS_NOTHING( message2.addField20ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 4 ) ) );
    TEST_THROWS_NOTHING( message2.addField32ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 5 ) ) );
    TEST_THROWS_NOTHING( message2.addField64ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 6 ) ) );
    TEST_THROWS_NOTHING( message2.addField128ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 7 ) ) );
    TEST_THROWS_NOTHING( message2.addField256ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 8 ) ) );
    TEST_THROWS_NOTHING( message2.addField512ByteArray ( largeByteArray, message::noname, static_cast<fudge_i16> ( 9 ) ) );

    TEST_EQUALS_INT( message2.size ( ), 9 );

    TEST_THROWS_NOTHING( message1.addField ( message2, string ( "Byte Array SubMessage" ) ) );

    // Add null, empty and populated strings
    TEST_THROWS_NOTHING( message1.addField ( string ( ), string ( "Null string" ) ) );
    TEST_THROWS_NOTHING( message1.addField ( string ( "" ), string ( "Empty string" ) ) );
    TEST_THROWS_NOTHING( message1.addField ( string ( "This is a string" ), string ( "String" ) ) );

    TEST_EQUALS_INT( message1.size ( ), 15 );

    // Add empty and populated arrays in a sub message
    message2 = message ( );
    TEST_EQUALS_INT( message2.size ( ), 0 );
    TEST_THROWS_NOTHING( message2.addField ( std::vector<fudge_byte> ( ), string ( "No Bytes" ) ) );
    TEST_THROWS_NOTHING( message2.addField ( rawBytes, string ( "Bytes" ) ) );
    TEST_THROWS_NOTHING( message2.addField ( rawShorts, string ( "Shorts" ) ) );
    TEST_THROWS_NOTHING( message2.addField ( rawInts, string ( "Ints" ) ) );
    TEST_THROWS_NOTHING( message2.addField ( rawLongs, string ( "Longs" ) ) );
    TEST_THROWS_NOTHING( message2.addField ( rawFloats, string ( "Floats" ) ) );
    TEST_THROWS_NOTHING( message2.addField ( rawDoubles, string ( "Doubles" ) ) );

    TEST_EQUALS_INT( message2.size ( ), 7 );

    TEST_THROWS_NOTHING( message1.addField ( message2, string ( "Array SubMessage" ) ) );

    // Add an empty sub message
    message2 = message ( );
    TEST_THROWS_NOTHING( message1.addField ( message2, string ( "Empty SubMessage" ) ) );

    TEST_EQUALS_INT( message1.size ( ), 17 );

    // Retrieve the top-level fields and check their contents
    std::vector<FudgeField> fields;
    TEST_THROWS_NOTHING( message1.getFields ( fields ) );

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_INDICATOR );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BOOLEAN );
    TEST_EQUALS_TRUE( fields [ 1 ].data.boolean );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE );
    TEST_EQUALS_INT( fields [ 2 ].data.byte, 127 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_SHORT );
    TEST_EQUALS_INT( fields [ 3 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_INT );
    TEST_EQUALS_INT( fields [ 4 ].data.i32, -2147483647 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_LONG );
    TEST_EQUALS_INT( fields [ 5 ].data.i64, 9223372036854775807ll );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_FLOAT );
    TEST_EQUALS_FLOAT( fields [ 6 ].data.f32, 2147483647.0f, 0.01f );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_DOUBLE );
    TEST_EQUALS_FLOAT( fields [ 7 ].data.f64, -9223372036854775807.0, 0.01f );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_DATE );
    TEST_EQUALS_TRUE( date ( fields [ 8 ].data.datetime.date ) == exampleDate );
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_TIME );
    TEST_EQUALS_TRUE( time ( fields [ 9 ].data.datetime.time ) == exampleTime );
    TEST_EQUALS_INT( fields [ 10 ].type, FUDGE_TYPE_DATETIME );
    // TODO Compare field 10 datetime with exampleDateTime
    TEST_EQUALS_INT( fields [ 11 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 11 ].data.message != 0 );
    TEST_EQUALS_INT( fields [ 12 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( string ( fields [ 12 ].data.string ) == string ( ) );
    TEST_EQUALS_INT( fields [ 13 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( string ( fields [ 13 ].data.string ) == string ( ) );
    TEST_EQUALS_INT( fields [ 14 ].type, FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( string ( fields [ 14 ].data.string ) == string ( "This is a string" ) );
    TEST_EQUALS_INT( fields [ 15 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 15 ].data.message != 0 );
    TEST_EQUALS_INT( fields [ 16 ].type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 16 ].data.message != 0 );

    TEST_THROWS_NOTHING( message1.getField ( field, string ( "Null string" ) ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( string ( field.data.string ) == string ( ) );
    TEST_THROWS_NOTHING( message1.getField ( field, string ( "Empty SubMessage" ) ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_INT( message ( field.data.message ).size ( ), 0 );

    TEST_THROWS_EXCEPTION( message1.getField ( field, string ( ) ), exception );
    TEST_THROWS_EXCEPTION( message1.getField ( field, string ( "null string" ) ), exception );
    TEST_THROWS_EXCEPTION( message1.getField ( field, string ( "Bytes" ) ), exception );

    // Check the fixed array message
    message2 = message ( fields [ 11 ].data.message );
    TEST_EQUALS_INT( message2.size ( ), 9 );
    message2.getFields ( fields );

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE_ARRAY_4 );
    TEST_EQUALS_INT( fields [ 0 ].numbytes, 4 );
    TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, 4, largeByteArray, 4 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE_ARRAY_8 );
    TEST_EQUALS_INT( fields [ 1 ].numbytes, 8 );
    TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, 8, largeByteArray, 8 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_BYTE_ARRAY_16 );
    TEST_EQUALS_INT( fields [ 2 ].numbytes, 16 );
    TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, 16, largeByteArray, 16 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE_ARRAY_20 );
    TEST_EQUALS_INT( fields [ 3 ].numbytes, 20 );
    TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, 20, largeByteArray, 20 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_BYTE_ARRAY_32 );
    TEST_EQUALS_INT( fields [ 4 ].numbytes, 32 );
    TEST_EQUALS_MEMORY( fields [ 4 ].data.bytes, 32, largeByteArray, 32 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_BYTE_ARRAY_64 );
    TEST_EQUALS_INT( fields [ 5 ].numbytes, 64 );
    TEST_EQUALS_MEMORY( fields [ 5 ].data.bytes, 64, largeByteArray, 64 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_BYTE_ARRAY_128 );
    TEST_EQUALS_INT( fields [ 6 ].numbytes, 128 );
    TEST_EQUALS_MEMORY( fields [ 6 ].data.bytes, 128, largeByteArray, 128 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_BYTE_ARRAY_256 );
    TEST_EQUALS_INT( fields [ 7 ].numbytes, 256 );
    TEST_EQUALS_MEMORY( fields [ 7 ].data.bytes, 256, largeByteArray, 256 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_BYTE_ARRAY_512 );
    TEST_EQUALS_INT( fields [ 8 ].numbytes, 512 );
    TEST_EQUALS_MEMORY( fields [ 8 ].data.bytes, 512, largeByteArray, 512 );

    TEST_THROWS_NOTHING( message2.getField ( field, 1 ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY_4 );
    TEST_THROWS_NOTHING( message2.getField ( field, 4 ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY_20 );

    TEST_THROWS_EXCEPTION( message2.getField ( field, 0 ), exception );
    TEST_THROWS_EXCEPTION( message2.getField ( field, -1 ), exception );
    TEST_THROWS_EXCEPTION( message2.getField ( field, 10 ), exception );

    // Check the variable array message
    TEST_THROWS_NOTHING( message1.getFieldAt ( field, 15 ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FUDGE_MSG );
    message2 = message ( field.data.message );
    TEST_EQUALS_INT( message2.size ( ), 7 );
    TEST_THROWS_NOTHING( message2.getFields ( fields ) );

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE_ARRAY );
    TEST_EQUALS_INT( fields [ 0 ].numbytes, 0 );
    TEST_EQUALS_MEMORY( fields [ 0 ].data.bytes, 0, 0, 0 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE_ARRAY );
    TEST_EQUALS_INT( fields [ 1 ].numbytes, 16 );
    TEST_EQUALS_MEMORY( fields [ 1 ].data.bytes, 16, rawBytesArray, 16 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_SHORT_ARRAY );
    TEST_EQUALS_INT( fields [ 2 ].numbytes, 20 );
    TEST_EQUALS_MEMORY( fields [ 2 ].data.bytes, 20, rawShortsArray, 20 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_INT_ARRAY );
    TEST_EQUALS_INT( fields [ 3 ].numbytes, 16 );
    TEST_EQUALS_MEMORY( fields [ 3 ].data.bytes, 16, rawIntsArray, 16 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_LONG_ARRAY );
    TEST_EQUALS_INT( fields [ 4 ].numbytes, 96 );
    TEST_EQUALS_MEMORY( fields [ 4 ].data.bytes, 96, rawLongsArray, 96 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_FLOAT_ARRAY );
    TEST_EQUALS_INT( fields [ 5 ].numbytes, 32 );
    TEST_EQUALS_MEMORY( fields [ 5 ].data.bytes, 32, rawFloatsArray, 32 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_DOUBLE_ARRAY );
    TEST_EQUALS_INT( fields [ 6 ].numbytes, 40 );
    TEST_EQUALS_MEMORY( fields [ 6 ].data.bytes, 40, rawDoublesArray, 40 );

    TEST_THROWS_NOTHING( message2.getField ( field, string ( "Bytes" ) ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_BYTE_ARRAY );
    TEST_THROWS_NOTHING( message2.getField ( field, string ( "Floats" ) ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FLOAT_ARRAY );

    TEST_THROWS_EXCEPTION( message2.getField ( field, string ( "Empty SubMessage" ) ), exception );
    TEST_THROWS_EXCEPTION( message2.getField ( field, 1 ), exception );

    // Check the empty message
    TEST_THROWS_NOTHING( message1.getFieldAt ( field, 16 ) );
    TEST_EQUALS_INT( field.type, FUDGE_TYPE_FUDGE_MSG );
    message2 = message ( field.data.message );
    TEST_EQUALS_INT( message2.size ( ), 0 );
END_TEST

DEFINE_TEST( IntegerFieldDowncasting )
    using fudge::message;

    // Create the test message
    message message1;
    message1.addField ( static_cast<fudge_byte> ( -127 ) );
    message1.addField ( static_cast<fudge_i16> ( -127 ) );
    message1.addField ( static_cast<fudge_i16> ( 32767 ) );
    message1.addField ( static_cast<fudge_i32> ( -127 ) );
    message1.addField ( static_cast<fudge_i32> ( 32767 ) );
    message1.addField ( static_cast<fudge_i32> ( -2147483647 ) );
    message1.addField ( static_cast<fudge_i64> ( -127 ) );
    message1.addField ( static_cast<fudge_i64> ( 32767 ) );
    message1.addField ( static_cast<fudge_i64> ( -2147483647 ) );
    message1.addField ( static_cast<fudge_i64> ( 2147483648ul ) );

    // Check the fields are using the smallest integer type possible
    std::vector<FudgeField> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 10 );

    TEST_EQUALS_INT( fields [ 0 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 0 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 1 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 1 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 2 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 2 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 3 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 3 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 4 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 4 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 5 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 5 ].data.i32, -2147483647 );
    TEST_EQUALS_INT( fields [ 6 ].type, FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 6 ].data.byte, -127 );
    TEST_EQUALS_INT( fields [ 7 ].type, FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 7 ].data.i16, 32767 );
    TEST_EQUALS_INT( fields [ 8 ].type, FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 8 ].data.i32, -2147483647 );
    TEST_EQUALS_INT( fields [ 9 ].type, FUDGE_TYPE_LONG );      TEST_EQUALS_INT( fields [ 9 ].data.i64, ( fudge_i64 ) 2147483648ul );
END_TEST

DEFINE_TEST( FieldCoercion )
    // TODO Implement
END_TEST

DEFINE_TEST_SUITE( Message )
    REGISTER_TEST( FieldFunctions )
    REGISTER_TEST( IntegerFieldDowncasting )
    REGISTER_TEST( FieldCoercion )
END_TEST_SUITE

