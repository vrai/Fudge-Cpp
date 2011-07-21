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

DEFINE_TEST( FieldFunctions )
    using fudge::date;
    using fudge::datetime;
    using fudge::exception;
    using fudge::field;
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
    TEST_THROWS_EXCEPTION( message1.getFieldAt (  0 ), exception );                         // Invalid index
    TEST_THROWS_EXCEPTION( message1.getField ( static_cast<fudge_i16> ( 0 ) ), exception ); // Invalid ordinal
    TEST_THROWS_EXCEPTION( message1.getField ( string ( "Field Name" ) ), exception );      // Invalid field name

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
    std::vector<field> fields;
    TEST_THROWS_NOTHING( message1.getFields ( fields ) );

    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_INDICATOR );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BOOLEAN );
    TEST_EQUALS_TRUE( fields [ 1 ].getBoolean ( ) );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_BYTE );
    TEST_EQUALS_INT( fields [ 2 ].getByte ( ), 127 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_SHORT );
    TEST_EQUALS_INT( fields [ 3 ].getInt16 ( ), 32767 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_INT );
    TEST_EQUALS_INT( fields [ 4 ].getInt32 ( ), -2147483647 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_LONG );
    TEST_EQUALS_INT( fields [ 5 ].getInt64 ( ), 9223372036854775807ll );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_FLOAT );
    TEST_EQUALS_FLOAT( fields [ 6 ].getFloat32 ( ), 2147483647.0f, 0.01f );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_DOUBLE );
    TEST_EQUALS_FLOAT( fields [ 7 ].getFloat64 ( ), -9223372036854775807.0, 0.01f );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_DATE );
    TEST_EQUALS_TRUE( date ( fields [ 8 ].getDate ( ) ) == exampleDate );
    TEST_EQUALS_INT( fields [ 9 ].type ( ), FUDGE_TYPE_TIME );
    TEST_EQUALS_TRUE( time ( fields [ 9 ].getTime ( ) ) == exampleTime );
    TEST_EQUALS_INT( fields [ 10 ].type ( ), FUDGE_TYPE_DATETIME );
    // TODO Compare field 10 datetime with exampleDateTime
    TEST_EQUALS_INT( fields [ 11 ].type ( ), FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 11 ].getMessage ( ) != 0 );
    TEST_EQUALS_INT( fields [ 12 ].type ( ), FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( string ( fields [ 12 ].getString ( ) ) == string ( ) );
    TEST_EQUALS_INT( fields [ 13 ].type ( ), FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( string ( fields [ 13 ].getString ( ) ) == string ( ) );
    TEST_EQUALS_INT( fields [ 14 ].type ( ), FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( string ( fields [ 14 ].getString ( ) ) == string ( "This is a string" ) );
    TEST_EQUALS_INT( fields [ 15 ].type ( ), FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 15 ].getMessage ( ) != 0 );
    TEST_EQUALS_INT( fields [ 16 ].type ( ), FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_TRUE( fields [ 16 ].getMessage ( ) != 0 );

    field field1;
    TEST_THROWS_NOTHING( field1 = message1.getField ( string ( "Null string" ) ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_STRING );
    TEST_EQUALS_TRUE( field1.getString ( ) == string ( ) );
    TEST_THROWS_NOTHING( field1 = message1.getField ( string ( "Empty SubMessage" ) ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_FUDGE_MSG );
    TEST_EQUALS_INT( message ( field1.raw ( ).data.message ).size ( ), 0 );

    TEST_THROWS_EXCEPTION( message1.getField ( string ( ) ), exception );
    TEST_THROWS_EXCEPTION( message1.getField ( string ( "null string" ) ), exception );
    TEST_THROWS_EXCEPTION( message1.getField ( string ( "Bytes" ) ), exception );

    TEST_THROWS_EXCEPTION( fields [ 0  ].numelements ( ), exception );
    TEST_THROWS_EXCEPTION( fields [ 2  ].numelements ( ), exception );
    TEST_THROWS_EXCEPTION( fields [ 16 ].numelements ( ), exception );

    // Check the fixed array message
    message2 = fields [ 11 ].getMessage ( );
    TEST_EQUALS_INT( message2.size ( ), 9 );
    message2.getFields ( fields );

    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_4 );
    TEST_EQUALS_INT( fields [ 0 ].numbytes ( ), 4 );
    TEST_EQUALS_INT( fields [ 0 ].numelements ( ), 4 );
    TEST_EQUALS_MEMORY( fields [ 0 ].bytes ( ), 4, largeByteArray, 4 );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_8 );
    TEST_EQUALS_INT( fields [ 1 ].numbytes ( ), 8 );
    TEST_EQUALS_INT( fields [ 1 ].numelements ( ), 8 );
    TEST_EQUALS_MEMORY( fields [ 1 ].bytes ( ), 8, largeByteArray, 8 );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_16 );
    TEST_EQUALS_INT( fields [ 2 ].numbytes ( ), 16 );
    TEST_EQUALS_INT( fields [ 2 ].numelements ( ), 16 );
    TEST_EQUALS_MEMORY( fields [ 2 ].bytes ( ), 16, largeByteArray, 16 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_20 );
    TEST_EQUALS_INT( fields [ 3 ].numbytes ( ), 20 );
    TEST_EQUALS_INT( fields [ 3 ].numelements ( ), 20 );
    TEST_EQUALS_MEMORY( fields [ 3 ].bytes ( ), 20, largeByteArray, 20 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_32 );
    TEST_EQUALS_INT( fields [ 4 ].numbytes ( ), 32 );
    TEST_EQUALS_INT( fields [ 4 ].numelements ( ), 32 );
    TEST_EQUALS_MEMORY( fields [ 4 ].bytes ( ), 32, largeByteArray, 32 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_64 );
    TEST_EQUALS_INT( fields [ 5 ].numbytes ( ), 64 );
    TEST_EQUALS_INT( fields [ 5 ].numelements ( ), 64 );
    TEST_EQUALS_MEMORY( fields [ 5 ].bytes ( ), 64, largeByteArray, 64 );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_128 );
    TEST_EQUALS_INT( fields [ 6 ].numbytes ( ), 128 );
    TEST_EQUALS_INT( fields [ 6 ].numelements ( ), 128 );
    TEST_EQUALS_MEMORY( fields [ 6 ].bytes ( ), 128, largeByteArray, 128 );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_256 );
    TEST_EQUALS_INT( fields [ 7 ].numbytes ( ), 256 );
    TEST_EQUALS_INT( fields [ 7 ].numelements ( ), 256 );
    TEST_EQUALS_MEMORY( fields [ 7 ].bytes ( ), 256, largeByteArray, 256 );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_BYTE_ARRAY_512 );
    TEST_EQUALS_INT( fields [ 8 ].numbytes ( ), 512 );
    TEST_EQUALS_INT( fields [ 8 ].numelements ( ), 512 );
    TEST_EQUALS_MEMORY( fields [ 8 ].bytes ( ), 512, largeByteArray, 512 );

    TEST_THROWS_NOTHING( field1 = message2.getField ( 1 ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_BYTE_ARRAY_4 );
    TEST_THROWS_NOTHING( field1 = message2.getField ( 4 ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_BYTE_ARRAY_20 );

    TEST_THROWS_EXCEPTION( message2.getField ( 0 ), exception );
    TEST_THROWS_EXCEPTION( message2.getField ( -1 ), exception );
    TEST_THROWS_EXCEPTION( message2.getField ( 10 ), exception );

    // Check the variable array message
    TEST_THROWS_NOTHING( field1 = message1.getFieldAt ( 15 ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_FUDGE_MSG );
    message2 = message ( field1.raw ( ).data.message );
    TEST_EQUALS_INT( message2.size ( ), 7 );
    TEST_THROWS_NOTHING( message2.getFields ( fields ) );

    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BYTE_ARRAY );
    std::vector<fudge_byte> byteArray;
    TEST_EQUALS_INT( fields [ 0 ].getArray ( byteArray ), 0 );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BYTE_ARRAY );
    TEST_EQUALS_INT( fields [ 1 ].getArray ( byteArray ), 16 );
    TEST_EQUALS_INT( fields [ 1 ].numelements ( ), 16 );
    TEST_EQUALS_VECTOR( byteArray, rawBytes );
    std::vector<fudge_i16> shortArray;
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_SHORT_ARRAY );
    TEST_EQUALS_INT( fields [ 2 ].getArray ( shortArray ), 10 );
    TEST_EQUALS_INT( fields [ 2 ].numelements ( ), 10 );
    TEST_EQUALS_VECTOR( shortArray, rawShorts );
    std::vector<fudge_i32> intArray;
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_INT_ARRAY );
    TEST_EQUALS_INT( fields [ 3 ].getArray ( intArray ), 4 );
    TEST_EQUALS_INT( fields [ 3 ].numelements ( ), 4 );
    TEST_EQUALS_VECTOR( intArray, rawInts );
    std::vector<fudge_i64> longArray;
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_LONG_ARRAY );
    TEST_EQUALS_INT( fields [ 4 ].getArray ( longArray ), 12 );
    TEST_EQUALS_INT( fields [ 4 ].numelements ( ), 12 );
    TEST_EQUALS_VECTOR( longArray, rawLongs );
    std::vector<fudge_f32> floatArray;
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_FLOAT_ARRAY );
    TEST_EQUALS_INT( fields [ 5 ].getArray ( floatArray ), 8 );
    TEST_EQUALS_INT( fields [ 5 ].numelements ( ), 8 );
    TEST_EQUALS_VECTOR( floatArray, rawFloats );
    std::vector<fudge_f64> doubleArray;
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_DOUBLE_ARRAY );
    TEST_EQUALS_INT( fields [ 6 ].getArray ( doubleArray ), 5 );
    TEST_EQUALS_INT( fields [ 6 ].numelements ( ), 5 );
    TEST_EQUALS_VECTOR( doubleArray, rawDoubles );

    TEST_THROWS_NOTHING( field1 = message2.getField ( string ( "Bytes" ) ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_BYTE_ARRAY );
    TEST_THROWS_EXCEPTION( field1.getArray ( shortArray ), exception );
    TEST_THROWS_NOTHING( field1 = message2.getField ( string ( "Floats" ) ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_FLOAT_ARRAY );
    TEST_THROWS_EXCEPTION( field1.getArray ( byteArray ), exception );

    TEST_THROWS_EXCEPTION( message2.getField ( string ( "Empty SubMessage" ) ), exception );
    TEST_THROWS_EXCEPTION( message2.getField ( 1 ), exception );

    // Check the empty message
    TEST_THROWS_NOTHING( field1 = message1.getFieldAt ( 16 ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_FUDGE_MSG );
    message2 = message ( field1.raw ( ).data.message );
    TEST_EQUALS_INT( message2.size ( ), 0 );
END_TEST

DEFINE_TEST( IntegerFieldDowncasting )
    using fudge::field;
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
    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 10 );

    TEST_EQUALS_INT( fields [ 0 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 0 ].getByte ( ), -127 );
    TEST_EQUALS_INT( fields [ 1 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 1 ].getByte ( ), -127 );
    TEST_EQUALS_INT( fields [ 2 ].type ( ), FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 2 ].getInt16 ( ), 32767 );
    TEST_EQUALS_INT( fields [ 3 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 3 ].getByte ( ), -127 );
    TEST_EQUALS_INT( fields [ 4 ].type ( ), FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 4 ].getInt16 ( ), 32767 );
    TEST_EQUALS_INT( fields [ 5 ].type ( ), FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 5 ].getInt32 ( ), -2147483647 );
    TEST_EQUALS_INT( fields [ 6 ].type ( ), FUDGE_TYPE_BYTE );      TEST_EQUALS_INT( fields [ 6 ].getByte ( ), -127 );
    TEST_EQUALS_INT( fields [ 7 ].type ( ), FUDGE_TYPE_SHORT );     TEST_EQUALS_INT( fields [ 7 ].getInt16 ( ), 32767 );
    TEST_EQUALS_INT( fields [ 8 ].type ( ), FUDGE_TYPE_INT );       TEST_EQUALS_INT( fields [ 8 ].getInt32 ( ), -2147483647 );
    TEST_EQUALS_INT( fields [ 9 ].type ( ), FUDGE_TYPE_LONG );      TEST_EQUALS_INT( fields [ 9 ].getInt64 ( ), ( fudge_i64 ) 2147483648ul );
END_TEST

DEFINE_TEST( FieldCoercion )
    using fudge::exception;
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // Create the test message
    message message1;
    message1.addField ( string ( "Indicator" ), message::noordinal );
    message1.addField ( true, string ( "True Bool" ) );
    message1.addField ( false, string ( "False Bool" ) );
    message1.addField ( static_cast<fudge_byte> ( 0 ), string ( "Zero Byte" ) );
    message1.addField ( static_cast<fudge_byte> ( -42 ), string ( "Non-zero Byte" ) );
    message1.addField ( static_cast<fudge_i16> ( 256 ), string ( "Non-zero Short" ) );
    message1.addField ( static_cast<fudge_i32> ( -40000 ), string ( "Non-zero Int" ) );
    message1.addField ( static_cast<fudge_i64> ( 10000000000ll ), string ( "Non-zero Long" ) );
    message1.addField ( static_cast<fudge_f32> ( 0.0f ), string ( "Zero Float" ) );
    message1.addField ( static_cast<fudge_f32> ( -1.234f ), string ( "Non-zero Float" ) );
    message1.addField ( static_cast<fudge_f64> ( 0.0 ), string ( "Zero Double" ) );
    message1.addField ( static_cast<fudge_f64> ( 123.4567 ), string ( "Non-zero Double" ) );
    message1.addField ( string ( "This is a string" ), string ( "String" ) );

    // Retrieve test fields and test casting
    std::vector<field> fields;
    message1.getFields ( fields );
    TEST_EQUALS_INT( fields.size ( ), 13 );

    bool boolean;
    TEST_THROWS_EXCEPTION( boolean = fields [ 0 ].getAsBoolean ( ), exception );                                    // Indicator
    TEST_THROWS_NOTHING( boolean = fields [ 1 ].getAsBoolean ( ) );     TEST_EQUALS_INT( boolean, FUDGE_TRUE );     // True bool 
    TEST_THROWS_NOTHING( boolean = fields [ 2 ].getAsBoolean ( ) );     TEST_EQUALS_INT( boolean, FUDGE_FALSE );    // False bool
    TEST_THROWS_NOTHING( boolean = fields [ 3 ].getAsBoolean ( ) );     TEST_EQUALS_INT( boolean, FUDGE_FALSE );    // Zero byte
    TEST_THROWS_NOTHING( boolean = fields [ 4 ].getAsBoolean ( ) );     TEST_EQUALS_INT( boolean, FUDGE_TRUE );     // Non-zero byte
    TEST_THROWS_NOTHING( boolean = fields [ 5 ].getAsBoolean ( ) );     TEST_EQUALS_INT( boolean, FUDGE_TRUE );     // Non-zero short
    TEST_THROWS_NOTHING( boolean = fields [ 6 ].getAsBoolean ( ) );     TEST_EQUALS_INT( boolean, FUDGE_TRUE );     // Non-zero int
    TEST_THROWS_NOTHING( boolean = fields [ 7 ].getAsBoolean ( ) );     TEST_EQUALS_INT( boolean, FUDGE_TRUE );     // Non-zero long
    for ( size_t index ( 8 ); index < fields.size ( ); ++index )
        TEST_THROWS_EXCEPTION( boolean = fields [ index ].getAsBoolean ( ), exception );
    
    fudge_byte byte;
    TEST_THROWS_EXCEPTION( byte = fields [ 0 ].getAsByte ( ), exception );                                          // Indicator
    TEST_THROWS_NOTHING( byte = fields [ 1 ].getAsByte ( ) );           TEST_EQUALS_INT( byte, FUDGE_TRUE );        // True bool
    TEST_THROWS_NOTHING( byte = fields [ 2 ].getAsByte ( ) );           TEST_EQUALS_INT( byte, FUDGE_FALSE );       // False bool
    TEST_THROWS_NOTHING( byte = fields [ 3 ].getAsByte ( ) );           TEST_EQUALS_INT( byte, 0 );                 // Zero byte
    TEST_THROWS_NOTHING( byte = fields [ 4 ].getAsByte ( ) );           TEST_EQUALS_INT( byte, -42 );               // Non-zero byte
    for ( size_t index ( 5 ); index < fields.size ( ); ++index )
        TEST_THROWS_EXCEPTION( byte = fields [ index ].getAsByte ( ), exception );

    fudge_i16 shortint;
    TEST_THROWS_EXCEPTION( shortint = fields [ 0 ].getAsInt16 ( ), exception );                                     // Indicator
    TEST_THROWS_NOTHING( shortint = fields [ 1 ].getAsInt16 ( ) );      TEST_EQUALS_INT( shortint, FUDGE_TRUE );    // True bool
    TEST_THROWS_NOTHING( shortint = fields [ 2 ].getAsInt16 ( ) );      TEST_EQUALS_INT( shortint, FUDGE_FALSE );   // False bool
    TEST_THROWS_NOTHING( shortint = fields [ 3 ].getAsInt16 ( ) );      TEST_EQUALS_INT( shortint, 0 );             // Zero byte
    TEST_THROWS_NOTHING( shortint = fields [ 4 ].getAsInt16 ( ) );      TEST_EQUALS_INT( shortint, -42 );           // Non-zero byte
    TEST_THROWS_NOTHING( shortint = fields [ 5 ].getAsInt16 ( ) );      TEST_EQUALS_INT( shortint, 256 );           // Non-zero short
    for ( size_t index ( 6 ); index < fields.size ( ); ++index )
        TEST_THROWS_EXCEPTION( shortint = fields [ index ].getAsInt16 ( ), exception );

    fudge_i32 integer;
    TEST_THROWS_EXCEPTION( integer = fields [ 0 ].getAsInt32 ( ), exception );                                      // Indicator
    TEST_THROWS_NOTHING( integer = fields [ 1 ].getAsInt32 ( ) );       TEST_EQUALS_INT( integer, FUDGE_TRUE );     // True bool
    TEST_THROWS_NOTHING( integer = fields [ 2 ].getAsInt32 ( ) );       TEST_EQUALS_INT( integer, FUDGE_FALSE );    // False bool
    TEST_THROWS_NOTHING( integer = fields [ 3 ].getAsInt32 ( ) );       TEST_EQUALS_INT( integer, 0 );              // Zero byte
    TEST_THROWS_NOTHING( integer = fields [ 4 ].getAsInt32 ( ) );       TEST_EQUALS_INT( integer, -42 );            // Non-zero byte
    TEST_THROWS_NOTHING( integer = fields [ 5 ].getAsInt32 ( ) );       TEST_EQUALS_INT( integer, 256 );            // Non-zero short
    TEST_THROWS_NOTHING( integer = fields [ 6 ].getAsInt32 ( ) );       TEST_EQUALS_INT( integer, -40000 );         // Non-zero int
    for ( size_t index ( 7 ); index < fields.size ( ); ++index )
        TEST_THROWS_EXCEPTION( integer = fields [ index ].getAsInt32 ( ), exception );

    fudge_i64 longint;
    TEST_THROWS_EXCEPTION( longint = fields [ 0 ].getAsInt64 ( ), exception );                                      // Indicator
    TEST_THROWS_NOTHING( longint = fields [ 1 ].getAsInt64 ( ) );      TEST_EQUALS_INT( longint, FUDGE_TRUE );      // True bool
    TEST_THROWS_NOTHING( longint = fields [ 2 ].getAsInt64 ( ) );      TEST_EQUALS_INT( longint, FUDGE_FALSE );     // False bool
    TEST_THROWS_NOTHING( longint = fields [ 3 ].getAsInt64 ( ) );      TEST_EQUALS_INT( longint, 0 );               // Zero byte
    TEST_THROWS_NOTHING( longint = fields [ 4 ].getAsInt64 ( ) );      TEST_EQUALS_INT( longint, -42 );             // Non-zero byte
    TEST_THROWS_NOTHING( longint = fields [ 5 ].getAsInt64 ( ) );      TEST_EQUALS_INT( longint, 256 );             // Non-zero short
    TEST_THROWS_NOTHING( longint = fields [ 6 ].getAsInt64 ( ) );      TEST_EQUALS_INT( longint, -40000 );          // Non-zero int
    TEST_THROWS_NOTHING( longint = fields [ 7 ].getAsInt64 ( ) );      TEST_EQUALS_INT( longint, 10000000000ll );   // Non-zero long
    for ( size_t index ( 8 ); index < fields.size ( ); ++index )
        TEST_THROWS_EXCEPTION( longint = fields [ index ].getAsInt64 ( ), exception );

    fudge_f32 floatval;
    for ( size_t index ( 0 ); index < 3; ++index )
        TEST_THROWS_EXCEPTION( floatval = fields [ index ].getAsFloat32 ( ), exception );
    TEST_THROWS_NOTHING( floatval = fields [ 3 ].getAsFloat32 ( ) );    TEST_EQUALS_FLOAT( floatval, 0.0f, 0.001f );            // Zero byte
    TEST_THROWS_NOTHING( floatval = fields [ 4 ].getAsFloat32 ( ) );    TEST_EQUALS_FLOAT( floatval, -42.0f, 0.001f );          // Non-zero byte
    TEST_THROWS_NOTHING( floatval = fields [ 5 ].getAsFloat32 ( ) );    TEST_EQUALS_FLOAT( floatval, 256.0f, 0.001f );          // Non-zero short 
    TEST_THROWS_NOTHING( floatval = fields [ 6 ].getAsFloat32 ( ) );    TEST_EQUALS_FLOAT( floatval, -40000.0f, 0.001f );       // Non-zero int 
    TEST_THROWS_NOTHING( floatval = fields [ 7 ].getAsFloat32 ( ) );    TEST_EQUALS_FLOAT( floatval, 10000000000.0f, 0.001f );  // Non-zero long
    TEST_THROWS_NOTHING( floatval = fields [ 8 ].getAsFloat32 ( ) );    TEST_EQUALS_FLOAT( floatval, 0.0f, 0.001f );            // Zero float
    TEST_THROWS_NOTHING( floatval = fields [ 9 ].getAsFloat32 ( ) );    TEST_EQUALS_FLOAT( floatval, -1.234f, 0.001f );         // Non-zero float
    TEST_THROWS_NOTHING( floatval = fields [ 10 ].getAsFloat32 ( ) );   TEST_EQUALS_FLOAT( floatval, 0.0f, 0.001f );            // Zero double
    TEST_THROWS_NOTHING( floatval = fields [ 11 ].getAsFloat32 ( ) );   TEST_EQUALS_FLOAT( floatval, 123.4567f, 0.0001f );      // Non-zero double

    fudge_f64 doubleval;
    for ( size_t index ( 0 ); index < 3; ++index )
        TEST_THROWS_EXCEPTION( doubleval = fields [ index ].getAsFloat64 ( ), exception );
    TEST_THROWS_NOTHING( doubleval = fields [ 3 ].getAsFloat64 ( ) );    TEST_EQUALS_FLOAT( doubleval, 0.0f, 0.001f );            // Zero byte
    TEST_THROWS_NOTHING( doubleval = fields [ 4 ].getAsFloat64 ( ) );    TEST_EQUALS_FLOAT( doubleval, -42.0f, 0.001f );          // Non-zero byte
    TEST_THROWS_NOTHING( doubleval = fields [ 5 ].getAsFloat64 ( ) );    TEST_EQUALS_FLOAT( doubleval, 256.0f, 0.001f );          // Non-zero short 
    TEST_THROWS_NOTHING( doubleval = fields [ 6 ].getAsFloat64 ( ) );    TEST_EQUALS_FLOAT( doubleval, -40000.0f, 0.001f );       // Non-zero int 
    TEST_THROWS_NOTHING( doubleval = fields [ 7 ].getAsFloat64 ( ) );    TEST_EQUALS_FLOAT( doubleval, 10000000000.0f, 0.001f );  // Non-zero long
    TEST_THROWS_NOTHING( doubleval = fields [ 8 ].getAsFloat64 ( ) );    TEST_EQUALS_FLOAT( doubleval, 0.0, 0.001 );              // Zero float
    TEST_THROWS_NOTHING( doubleval = fields [ 9 ].getAsFloat64 ( ) );    TEST_EQUALS_FLOAT( doubleval, -1.234, 0.001 );           // Non-zero float
    TEST_THROWS_NOTHING( doubleval = fields [ 10 ].getAsFloat64 ( ) );   TEST_EQUALS_FLOAT( doubleval, 0.0, 0.0001 );             // Zero double
    TEST_THROWS_NOTHING( doubleval = fields [ 11 ].getAsFloat64 ( ) );   TEST_EQUALS_FLOAT( doubleval, 123.4567, 0.0001 );        // Non-zero double

    string stringval;
    TEST_THROWS_NOTHING( stringval = fields [ 12 ].getAsString ( ) );   TEST_EQUALS_TRUE( stringval == string ( "This is a string" ) );
END_TEST

DEFINE_TEST_SUITE( Message )
    REGISTER_TEST( FieldFunctions )
    REGISTER_TEST( IntegerFieldDowncasting )
    REGISTER_TEST( FieldCoercion )
END_TEST_SUITE

