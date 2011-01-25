/**
 * Copyright (C) 2011 - 2011, Vrai Stacey.
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

#include <fudge-cpp/codec.hpp>
#include <fudge-cpp/fudge.hpp>
#include <fudge-cpp/message.hpp>
#include <iostream>
#include <stdexcept>

// This is a simple example of using the Fudge-Cpp API to create a message,
// encoding the message, decoding the message and retrieving the field data.
//
// The message that is created and processed could be represented by the C
// structure:
//
//     struct
//     {
//         const char * name;
//         int64_t dob;
//         const char * address [ ];
//     };
//
// In this example the array of strings represented by a submessage containing
// string fields, indexed by ordinal. Note that this use of ordinals is not
// necessary as Fudge maintains field order. They are used in manner to allow
// their use to be shown while keeping the example as brief as possible.
//
// For the same reason the example only uses ASCII strings, albiet contained
// within the proper UTF8 container. The FudgeString container can be
// initialised with UTF8, UTF16 and UTF32 strings as well as ASCII.

int main ( int argc, char * argv [ ] )
{
    // Source data for the example message
    static const char * addressLines [ ] = { "123 Fake Street",
                                             "Some City",
                                             "P0S T4L",
                                             "Country",
                                             0 };
    static const fudge::string name ( "Random Person" );

    static const fudge::string nameFieldName ( "name" ),
                               dobFieldName ( "dob" ),
                               addressFieldName ( "address" );

    static const fudge_i64 dob ( 19801231 );

    try
    {
        // The fudge library must be constructed before it's used
        fudge::fudge::init ( );

        //////////////////////////////////////////////////////////////////////
        // Construct a Fudge message

        // First, construct and populate the inner (address) message. This
        // will hold lines for the street address, with the ordinal providing
        // ordering information.
        fudge::message addressMsg;

        for ( int index ( 0 ); addressLines [ index ]; ++index )
        {
            addressMsg.addField ( fudge::string ( addressLines [ index ] ),
                                  fudge::message::noname,
                                  index );  // Index used as the field ordinal
        }

        // Now construct the outer message and add the details fields
        fudge::message contactMsg;
        contactMsg.addField ( name, nameFieldName );
        contactMsg.addField ( dob, dobFieldName );
        contactMsg.addField ( addressMsg, addressFieldName );

        //////////////////////////////////////////////////////////////////////
        // Encode a Fudge message

        // Before begin encoded a Fudge message must be wrapped in an envelope
        fudge::codec codec;
        fudge_byte * bytes;
        fudge_i32 numbytes;
        codec.encode ( fudge::envelope ( 0, 1, 0, contactMsg ), bytes, numbytes );
        std::cout << "Contacts message encoded as a " << numbytes
                  << " byte Fudge message" << std::endl;

        // Now that the message has been encoded, the contacts message can be
        // released. This will destroy the message and its fields as well as
        // decrementing the reference count on the address message.
        contactMsg = addressMsg = fudge::message ( );

        //////////////////////////////////////////////////////////////////////
        // Decode a Fudge message

        fudge::envelope envelope ( codec.decode ( bytes, numbytes ) );
        std::cout << "Decoded Fudge message with schema version "
                  << static_cast<int> ( envelope.schemaversion ( ) ) << " and "
                  << envelope.payload ( ).size ( ) << " fields" << std::endl;

        // Now the message has been decoded it's sage to free the byte array
        // holding the encoded message.
        free ( bytes );

        // Re-use the local contacts message variable to save on typing. Grab
        // a local reference and release the, no longer needed, envelope.
        contactMsg = envelope.payload ( );
        envelope = fudge::envelope ( );

        // Retrieve and output the name; having checked that it is a string.
        fudge::field field ( contactMsg.getField ( nameFieldName ) );
        if ( field.type ( ) != FUDGE_TYPE_STRING )
            throw std::runtime_error ( "Name field is not a string" );
        std::cout << "Name (" << field.numbytes ( ) << " bytes): \""
                  << field.getString ( ).convertToStdString ( ) << "\""
                  << std::endl;

        // Retrieve and output the date-of-birth field. While this was added
        // as a 64bit integer it should have been stored as a 32bit value (the
        // smallest integer type that can hold the value provided).
        field = contactMsg.getField ( dobFieldName );
        if ( field.type ( ) != FUDGE_TYPE_INT )
            throw std::runtime_error ( "DOB field is not a 32bit int" );
        std::cout << "DOB: " << field.getAsInt64 ( ) << std::endl;

        // Retrieve the inner (address) message and place it in the local
        // address message variable.
        addressMsg = contactMsg.getField ( addressFieldName ).getMessage ( );

        // Force the outer (contacts) message to be released. This should free
        // the memory it was using, with the exception of the address message
        // as an additional reference has been retained for it.
        contactMsg = fudge::message ( );

        // Referencing the fields by ordinal, retrieve the address lines
        for ( size_t index ( 0 ); index < addressMsg.size ( ); ++index )
        {
            field = addressMsg.getField ( static_cast<fudge_i16> ( index ) );
            if ( field.type ( ) != FUDGE_TYPE_STRING )
                throw std::runtime_error ( "Address field is not a string" );
            std::cout << "Address line " << index << ": \""
                      << field.getString ( ).convertToStdString ( ) << "\" ("
                      << field.numbytes ( ) << " bytes)" << std::endl;
        }
    }
    catch ( const std::exception & exception )
    {
        std::cerr << "FATAL ERROR: " << exception.what ( ) << std::endl;
        return 1;
    }

    return 0;
}

