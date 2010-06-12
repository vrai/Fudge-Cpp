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
    static const std::string AllNames_Filename ( "test_data/allNames.dat" );

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

DEFINE_TEST_SUITE( Codec )
    // Interop decode test files
    REGISTER_TEST( DecodeAllNames )
    // TODO REGISTER_TEST( DecodeFixedWidths )
    // TODO REGISTER_TEST( DecodeAllOrdinals )
    // TODO REGISTER_TEST( DecodeSubMsgs )
    // TODO REGISTER_TEST( DecodeUnknown )
    // TODO REGISTER_TEST( DecodeVariableWidths )
    // TODO REGISTER_TEST( DecodeDateTimes )
    
    // Other decode test files
    // TODO REGISTER_TEST( DecodeDeepTree )

    // Interop encode tests
    // TODO REGISTER_TEST( EncodeAllNames )
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

