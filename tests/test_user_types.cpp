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
#include "fudge/codec.h"
#include "fudge/message.h"

// Example user types
namespace
{
    static const fudge_type_id FUDGE_TYPE_EXAMPLEIP4  = 100,
                               FUDGE_TYPE_EXAMPLETICK = 101;

    class ExampleIp4
    {
        public:
            ExampleIp4 ( uint8_t first, uint8_t second, uint8_t third, uint8_t fourth )
            {
                quads [ 0 ] = first;
                quads [ 1 ] = second;
                quads [ 2 ] = third;
                quads [ 3 ] = fourth;
            }

            uint8_t quads [ 4 ];

            static void addField ( fudge::message & message,
                                   const fudge::string & name,
                                   const ExampleIp4 & ip )
            {
                fudge::exception::throwOnError ( FudgeMsg_addFieldOpaque (
                            message.raw ( ),
                            FUDGE_TYPE_EXAMPLEIP4,
                            name.raw ( ),
                            0,
                            reinterpret_cast<const fudge_byte *> ( &ip ),
                            sizeof ( ExampleIp4 ) ) );
            }

            static FudgeStatus coerceField ( const FudgeField * source,
                                             const fudge_type_id type,
                                             FudgeFieldData * target,
                                             fudge_i32 * numbytes )
            {
                if ( ! ( source && target && numbytes ) )
                    return FUDGE_NULL_POINTER;
                if ( source->type == type )
                    return FUDGE_COERCION_NOT_REQUIRED;

                // For test purposes, only a string conversion is supported
                if ( type == FUDGE_TYPE_STRING )
                {
                    const ExampleIp4 * ip4 = reinterpret_cast<const ExampleIp4 *> ( source->data.bytes );

                    char buffer [ 16 ];
                    sprintf ( buffer, "%03d.%03d.%03d.%03d", ip4->quads [ 0 ], ip4->quads [ 1 ], ip4->quads [ 2 ], ip4->quads [ 3 ] );

                    FudgeStatus status = FudgeString_createFromASCIIZ ( &( target->string ), buffer );
                    if ( status != FUDGE_OK )
                        return status;

                    *numbytes = FudgeString_getSize ( target->string );
                    return FUDGE_OK;
                }
                return FUDGE_INVALID_TYPE_COERCION;
            }
    };

    class ExampleTick
    {
        public:
            ExampleTick ( const std::string & ric_, fudge_f32 bid_, fudge_f32 ask_, fudge_i32 time_ )
            {
                memcpy ( ric, ric_.c_str ( ), ric_.size ( ) < 15 ? ric_.size ( ) + 1 : 16 );
                bid = bid_;
                ask = ask_;
                time = time_;
            }

            char ric [ 16 ];
            fudge_f32 bid, ask;
            fudge_i32 time;

            fudge::string getRic ( ) const
            {
                return fudge::string ( ric );
            }

            static void addField ( fudge::message & message,
                                   const fudge::string & name,
                                   const ExampleTick & tick )
            {
                fudge::exception::throwOnError ( FudgeMsg_addFieldOpaque
                        ( message.raw ( ),
                          FUDGE_TYPE_EXAMPLETICK,
                          name.raw ( ),
                          0,
                          reinterpret_cast<const fudge_byte *> ( &tick ),
                          sizeof ( ExampleTick ) ) );
            }

            static FudgeStatus coerceField ( const FudgeField * source,
                                             const fudge_type_id type,
                                             FudgeFieldData * target,
                                             fudge_i32 * numbytes )
            {
                if ( ! ( source && target && numbytes ) )
                    return FUDGE_NULL_POINTER;
                if ( source->type == type )
                    return FUDGE_COERCION_NOT_REQUIRED;

                // For test purposes, only a string conversion is supported
                if ( type == FUDGE_TYPE_STRING )
                {
                    const ExampleTick * tick = reinterpret_cast<const ExampleTick *> ( source->data.bytes );

                    char buffer [ 52 ];
                    sprintf ( buffer, "%16s %8f %8f %16d", tick->ric, tick->bid, tick->ask, tick->time );

                    FudgeStatus status = FudgeString_createFromASCIIZ ( &( target->string ), buffer );
                    if ( status != FUDGE_OK )
                        return status;

                    *numbytes = FudgeString_getSize ( target->string );
                    return FUDGE_OK;
                }
                return FUDGE_INVALID_TYPE_COERCION;
            }

            static FudgeStatus encodeField ( const FudgeField * field, fudge_byte * * data )
            {
                const ExampleTick * tick = reinterpret_cast<const ExampleTick *> ( field->data.bytes );

                FudgeCodec_encodeFieldLength ( field->numbytes, data );

                // Write the RIC - as the string is of fixed length, a fixed
                // width field can be used
                FudgeCodec_encodeByteArray ( reinterpret_cast<const fudge_byte *> ( tick->ric ), 16, FUDGE_TRUE, data );

                // Write the float and integer values - this will perform the
                // necessary endian conversions
                FudgeCodec_encodeF32 ( tick->bid, data );
                FudgeCodec_encodeF32 ( tick->ask, data );
                FudgeCodec_encodeI32 ( tick->time, data );
                return FUDGE_OK;
            }

            static FudgeStatus decodeField ( const fudge_byte * bytes, const fudge_i32 width, FudgeFieldData * data )
            {
                // Make sure that the width in the header matches that of
                // the target structure
                if ( width != sizeof ( ExampleTick ) )
                    return FUDGE_OUT_OF_BYTES;

                // Allocate the target structure and point the data
                // structure's bytes pointer at it: use malloc rather than
                // new as the reference counter will destroy it with free
                ExampleTick * tick = reinterpret_cast<ExampleTick *> ( malloc ( width ) );
                if ( ! tick )
                    return FUDGE_OUT_OF_MEMORY;
                data->bytes = reinterpret_cast<const fudge_byte *> ( tick );

                // Copy in the string component and advance the source pointer
                memcpy ( tick->ric, bytes, 16 );
                bytes += 16;

                // Copy in the float/integer values: advancing the source
                // pointer after each one
                tick->bid = FudgeCodec_decodeF32 ( bytes );     bytes += sizeof ( fudge_f32 );
                tick->ask = FudgeCodec_decodeF32 ( bytes );     bytes += sizeof ( fudge_f32 );
                tick->time = FudgeCodec_decodeI32 ( bytes );    bytes += sizeof ( fudge_i32 );
                return FUDGE_OK;
            }
    };
}

DEFINE_TEST( UserTypeHandling )
    using fudge::codec;
    using fudge::envelope;
    using fudge::field;
    using fudge::message;
    using fudge::string;

    // Populate test objects and arrays
    fudge_byte bytes [ 16 ];
    for ( size_t index ( 0 ); index < sizeof ( bytes ); ++index )
        bytes [ index ] = index;

    ExampleIp4 localhost ( 127, 0, 0, 1 );
    ExampleIp4 opendns ( 208, 67, 222, 222 );
    ExampleTick tick ( "GBP=", 1.605, 1.607, 1263138018u );

    // Fail to register user types
    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLEIP4,
                                                  FUDGE_TYPE_PAYLOAD_LOCAL, // User types must have a bytes payload!
                                                  FudgeCodec_decodeFieldByteArray,
                                                  FudgeCodec_encodeFieldByteArray,
                                                  ExampleIp4::coerceField ), FUDGE_INVALID_USER_TYPE );

    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLEIP4,
                                                  FUDGE_TYPE_PAYLOAD_BYTES,
                                                  FudgeCodec_decodeFieldByteArray,
                                                  0,    // Cannot have a null pointer as an encoder function!
                                                  ExampleIp4::coerceField ), FUDGE_NULL_POINTER );

    // Register user types
    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLEIP4,
                                                  FUDGE_TYPE_PAYLOAD_BYTES,
                                                  FudgeCodec_decodeFieldByteArray,
                                                  FudgeCodec_encodeFieldByteArray,
                                                  ExampleIp4::coerceField ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeRegistry_registerType ( FUDGE_TYPE_EXAMPLETICK,
                                                  FUDGE_TYPE_PAYLOAD_BYTES,
                                                  ExampleTick::decodeField,
                                                  ExampleTick::encodeField,
                                                  ExampleTick::coerceField ), FUDGE_OK );

    // Construct a test message with user and built-in types
    message message1;
    message1.addField ( 2147483648ll, string ( "bigint" ) );
    ExampleIp4::addField ( message1, string ( "localhost" ), localhost );
    ExampleIp4::addField ( message1, string ( "opendns" ), opendns );
    ExampleTick::addField ( message1, string ( "tick" ), tick );
    message1.addField16ByteArray ( bytes, string ( "byte[16]" ) );

    TEST_EQUALS_INT( message1.size ( ), 5 );

    // Encode the test message and clear the message container
    envelope envelope1 ( 0, 0, 0, message1 );
    fudge_byte * encoded;
    fudge_i32 encodedsize;
    codec codec1;
    TEST_THROWS_NOTHING( codec1.encode ( envelope1, encoded, encodedsize ) );
    message1 = message ( );
    envelope1 = envelope ( );

    // Decode the test message and clear the encoded form
    envelope1 = codec1.decode ( encoded, encodedsize );
    message1 = envelope1.payload ( );
    free ( encoded );

    TEST_EQUALS_INT( message1.size ( ), 5 );

    // Retrieve and check the decoded fields
    field field1 = message1.getField ( string ( "bigint" ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_LONG );
    TEST_EQUALS_INT( field1.getInt64 ( ), 2147483648ll );

    field1 = message1.getField ( string ( "byte[16]" ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_BYTE_ARRAY_16 );
    TEST_EQUALS_MEMORY( field1.bytes ( ), field1.numbytes ( ), bytes, sizeof ( bytes ) );

    field1 = message1.getField ( string ( "localhost" ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_EXAMPLEIP4 );
    TEST_EQUALS_MEMORY( field1.bytes ( ), field1.numbytes ( ), reinterpret_cast<const fudge_byte *> ( &localhost ), sizeof ( ExampleIp4 ) );

    FudgeFieldData fielddata;
    FudgeTypePayload payload;
    fudge_i32 numbytes;
    TEST_EQUALS_INT( FudgeMsg_getFieldAs ( &( field1.raw ( ) ), FUDGE_TYPE_EXAMPLEIP4, &fielddata, &payload, &numbytes ), FUDGE_COERCION_NOT_REQUIRED );
    TEST_EQUALS_INT( FudgeMsg_getFieldAs ( &( field1.raw ( ) ), FUDGE_TYPE_FUDGE_MSG,  &fielddata, &payload, &numbytes ), FUDGE_INVALID_TYPE_COERCION );
    TEST_THROWS_EXCEPTION( field1.getAsInt16 ( ), std::runtime_error );
    
    string string1 = field1.getAsString ( );
    TEST_EQUALS_TRUE( string1 == string ( "127.000.000.001" ) );
    
    field1 = message1.getField ( string ( "opendns" ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_EXAMPLEIP4 );
    TEST_EQUALS_MEMORY( field1.bytes ( ), field1.numbytes ( ), reinterpret_cast<const fudge_byte *> ( &opendns ), sizeof ( ExampleIp4 ) );

    string1 = field1.getAsString ( );
    TEST_EQUALS_TRUE( string1 == string ( "208.067.222.222" ) );

    field1 = message1.getField ( string ( "tick" ) );
    TEST_EQUALS_INT( field1.type ( ), FUDGE_TYPE_EXAMPLETICK );
    TEST_EQUALS_INT( field1.numbytes ( ), sizeof ( ExampleTick ) );
    const ExampleTick * fieldtick = reinterpret_cast<const ExampleTick *> ( field1.bytes ( ) );
    TEST_EQUALS_TRUE( fieldtick->getRic ( ) == tick.getRic ( ) );
    TEST_EQUALS_FLOAT( fieldtick->bid, tick.bid, 0.0001 );
    TEST_EQUALS_FLOAT( fieldtick->ask, tick.ask, 0.0001 );
    TEST_EQUALS_INT( fieldtick->time, tick.time );

    string1 = field1.getAsString ( );
    TEST_EQUALS_TRUE( string1 == string ( "            GBP= 1.605000 1.607000       1263138018" ) );
END_TEST

DEFINE_TEST_SUITE( UserTypes )
    REGISTER_TEST( UserTypeHandling )
END_TEST_SUITE
