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
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

// Output functions
void outputEnvelope ( const fudge::envelope & envelope );
void outputMessage ( const fudge::message & message, unsigned int indent );
void outputField ( const fudge::field & field, unsigned int indent );
void outputString ( const fudge::string & string );
void outputDate ( const fudge::date & date );
void outputTime ( const fudge::time & time );
void outputDateTime ( const fudge::datetime & datetime );
template<class T> void outputArrayField ( const fudge::field & field, const std::string & format, size_t truncate );
template<class T> void outputArray ( const std::vector<T> & array, const std::string & format, size_t truncate );
std::string typeAsString ( fudge_type_id type );

// Other convenience functions
void displayUsage ( );
void loadFile ( fudge_byte * & bytes, fudge_i32 & numbytes, const std::string & filename );

// For a more basic example of Fudge-cpp use, see the "simple.cpp" file that
// should be in the same directory as this one.
//
// This is a basic "pretty printer" for Fudge encoded messages files. It takes
// the filename of the message to display and dumps the contents, in human
// readable form, to the standard output.
//
// Rather than assume a Unicode compatible console, all strings are converted
// in to 7bit ASCII before being output.
int main ( int argc, char * argv [ ] )
{
    // Get the filenamd from the command-line arguments
    if ( argc != 2 )
        displayUsage ( );
    const std::string filename ( argv [ 1 ] );

    try
    {
        // Initialise the Fudge library
        fudge::fudge::init ( );

        // Load in the file and attempt to decode it
        fudge_byte * bytes;
        fudge_i32 numbytes;
        loadFile ( bytes, numbytes, filename );

        fudge::codec codec;
        fudge::envelope envelope ( codec.decode ( bytes, numbytes ) );
        delete [] bytes;

        // Output the file contents
        outputEnvelope ( envelope );
    }
    catch ( const std::exception & exception )
    {
        std::cerr << "FATAL ERROR: " << exception.what ( ) << std::endl;
        return 1;
    }

    return 0;
}

void outputEnvelope ( const fudge::envelope & envelope )
{
    std::cout << "Schema version: " << static_cast<int> ( envelope.schemaversion ( ) ) << std::endl
              << "Taxonomy: " << static_cast<int> ( envelope.taxonomy ( ) ) << std::endl
              << "Message:" << std::endl << "{" << std::endl;
    outputMessage ( envelope.payload ( ), 0 );
    std::cout << "}" << std::endl;
}

void outputMessage ( const fudge::message & message, unsigned int indent )
{
    // Retrieve all of the fields in the message and output them in turn
    std::vector<fudge::field> fields;
    message.getFields ( fields );
    for ( std::vector<fudge::field>::const_iterator it ( fields.begin ( ) ); it != fields.end ( ); ++it )
        outputField ( *it, indent + 1 );
}

void outputField ( const fudge::field & field, unsigned int indent )
{
    const std::string indentStr ( indent * 2, ' ' );

    // Output the field's type, name (if present) and ordinal (if present)
    std::cout << indentStr
              << typeAsString ( field.type ( ) ) << " ";
    if ( field.name ( ) )
    {
        outputString ( *field.name ( ) );
        if ( field.ordinal ( ) )
            std::cout << "/";
    }
    if ( field.ordinal ( ) )
        std::cout << "ord(" << *field.ordinal ( ) << ")";
    std::cout << ": ";

    // Output the field contents
    switch ( field.type ( ) )
    {
        case FUDGE_TYPE_INDICATOR:      break;
        case FUDGE_TYPE_BOOLEAN:        std::cout << ( field.getBoolean ( ) ? "true" : "false" ); break;
        case FUDGE_TYPE_BYTE:           std::cout << static_cast<int> ( field.getByte ( ) ); break;
        case FUDGE_TYPE_SHORT:          std::cout << field.getInt16 ( ); break;
        case FUDGE_TYPE_INT:            std::cout << field.getInt32 ( ); break;
        case FUDGE_TYPE_LONG:           std::cout << field.getInt64 ( ); break;
        case FUDGE_TYPE_FLOAT:          std::cout << field.getFloat32 ( ); break;
        case FUDGE_TYPE_DOUBLE:         std::cout << field.getFloat64 ( ); break;

        case FUDGE_TYPE_SHORT_ARRAY:    outputArrayField<fudge_i16> ( field, "%d", 8 ); break;
        case FUDGE_TYPE_INT_ARRAY:      outputArrayField<fudge_i32> ( field, "%d", 8 ); break;
        case FUDGE_TYPE_LONG_ARRAY:     outputArrayField<fudge_i64> ( field, "%lu", 8 ); break;
        case FUDGE_TYPE_FLOAT_ARRAY:    outputArrayField<fudge_f32> ( field, "%f", 4 ); break;
        case FUDGE_TYPE_DOUBLE_ARRAY:   outputArrayField<fudge_f64> ( field, "%f", 4 ); break;
        case FUDGE_TYPE_STRING:         outputString ( field.getString ( ) ); break;
        case FUDGE_TYPE_DATE:           outputDate ( field.getDate ( ) ); break;
        case FUDGE_TYPE_TIME:           outputTime ( field.getTime ( ) ); break;
        case FUDGE_TYPE_DATETIME:       outputDateTime ( field.getDateTime ( ) ); break;
        case FUDGE_TYPE_BYTE_ARRAY:
        case FUDGE_TYPE_BYTE_ARRAY_4:
        case FUDGE_TYPE_BYTE_ARRAY_8:
        case FUDGE_TYPE_BYTE_ARRAY_16:
        case FUDGE_TYPE_BYTE_ARRAY_20:
        case FUDGE_TYPE_BYTE_ARRAY_32:
        case FUDGE_TYPE_BYTE_ARRAY_64:
        case FUDGE_TYPE_BYTE_ARRAY_128:
        case FUDGE_TYPE_BYTE_ARRAY_256:
        case FUDGE_TYPE_BYTE_ARRAY_512:
        {
            std::vector<fudge_byte> bytes ( field.bytes ( ), field.bytes ( ) + field.numbytes ( ) );
            outputArray ( bytes, "0x%02x", 10 );
            break;
        }
        case FUDGE_TYPE_FUDGE_MSG:
            std::cout << std::endl << indentStr << "{" << std::endl;
            outputMessage ( field.getMessage ( ), indent + 1 );
            std::cout << indentStr << "}";
            break;
        default:
        {
            std::cout << field.numbytes ( ) << " bytes ";
            std::vector<fudge_byte> bytes ( field.bytes ( ), field.bytes ( ) + field.numbytes ( ) );
            outputArray ( bytes, "0x%02x", 10 );
            break;
        }
    }

    std::cout << std::endl;
}

void outputString ( const fudge::string & string )
{
    std::cout << "\"" + string.convertToStdString ( ) + "\"";
}

void outputDate ( const fudge::date & date )
{
    std::cout << date.year ( ) << "/" << static_cast<int> ( date.month ( ) ) << "/" << static_cast<int> ( date.day ( ) );
}

void outputTime ( const fudge::time & time )
{
    const uint32_t hours ( time.seconds ( ) / 3600 );
    const uint32_t minutes ( ( time.seconds ( ) - ( hours * 3600 ) ) / 60 );
    const uint32_t seconds ( time.seconds ( ) - ( hours * 3600 ) - ( minutes * 60 ) );

    std::cout << std::setw ( 2 ) << std::setfill ( '0' ) << hours << ":"
              << std::setw ( 2 ) << std::setfill ( '0' ) << minutes << ":"
              << std::setw ( 2 ) << std::setfill ( '0' ) << seconds << "."
              << time.nanoseconds ( );

    if ( time.hasTimezone ( ) )
    {
        std::cout << "UTC";
        if ( time.timezoneOffset ( ) )
        {
            if ( time.timezoneOffset ( ) >= 0 )
                std::cout << '+';
            if ( time.timezoneOffset ( ) % 4 )
                std::cout << ( static_cast<float> ( time.timezoneOffset ( ) ) / 4.0f );
            else
                std::cout << ( time.timezoneOffset ( ) / 4 );
        }
    }
}

void outputDateTime ( const fudge::datetime & datetime )
{
    outputDate ( datetime );
    std::cout << " ";
    outputTime ( datetime );
}

template<class T> void outputArrayField ( const fudge::field & field, const std::string & format, size_t truncate )
{
    // Get a local, typed copy of the array
    std::vector<T> array;
    field.getArray ( array );

    outputArray ( array, format, truncate );
}

template<class T> void outputArray ( const std::vector<T> & array, const std::string & format, size_t truncate )
{
    // Output the array - up to the truncation limit
    truncate = std::min ( truncate, array.size ( ) );
    std::cout << "[ ";
    for ( size_t index ( 0 ); index < truncate; ++index )
    {
        if ( index )
            std::cout << ", ";
        printf ( format.c_str ( ), array [ index ] );
    }

    // If the array was truncated, add a suffix
    if ( truncate < array.size ( ) )
        std::cout << " ... " << ( array.size ( ) - truncate ) << " more";
    std::cout << " ]";
}

std::string typeAsString ( fudge_type_id type )
{
    static const int byteArraySizes [] = { 4, 8, 16, 20, 32, 64, 128, 256, 512 };

    switch ( type )
    {
        case FUDGE_TYPE_INDICATOR:      return "indicator"; break;
        case FUDGE_TYPE_BOOLEAN:        return "boolean";   break;
        case FUDGE_TYPE_BYTE:           return "byte";      break;
        case FUDGE_TYPE_SHORT:          return "short";     break;
        case FUDGE_TYPE_INT:            return "int";       break;
        case FUDGE_TYPE_LONG:           return "long";      break;
        case FUDGE_TYPE_FLOAT:          return "float";     break;
        case FUDGE_TYPE_DOUBLE:         return "double";    break;
        case FUDGE_TYPE_BYTE_ARRAY:     return "byte[]";    break;
        case FUDGE_TYPE_SHORT_ARRAY:    return "short[]";   break;
        case FUDGE_TYPE_INT_ARRAY:      return "int[]";     break;
        case FUDGE_TYPE_LONG_ARRAY:     return "long[]";    break;
        case FUDGE_TYPE_FLOAT_ARRAY:    return "float[]";   break;
        case FUDGE_TYPE_DOUBLE_ARRAY:   return "double[]";  break;
        case FUDGE_TYPE_STRING:         return "string";    break;
        case FUDGE_TYPE_FUDGE_MSG:      return "message";   break;
        case FUDGE_TYPE_DATE:           return "date";      break;
        case FUDGE_TYPE_TIME:           return "time";      break;
        case FUDGE_TYPE_DATETIME:       return "datetime";  break;
        default:                                            break;
    }

    std::ostringstream buffer;
    switch ( type )
    {
        case FUDGE_TYPE_BYTE_ARRAY_4:
        case FUDGE_TYPE_BYTE_ARRAY_8:
        case FUDGE_TYPE_BYTE_ARRAY_16:
        case FUDGE_TYPE_BYTE_ARRAY_20:
        case FUDGE_TYPE_BYTE_ARRAY_32:
        case FUDGE_TYPE_BYTE_ARRAY_64:
        case FUDGE_TYPE_BYTE_ARRAY_128:
        case FUDGE_TYPE_BYTE_ARRAY_256:
        case FUDGE_TYPE_BYTE_ARRAY_512:
            buffer << "byte[" << byteArraySizes [ type - FUDGE_TYPE_BYTE_ARRAY_4 ] << "]";
            break;
        default:
            buffer << "unknown(" << static_cast<int> ( type ) << ")";
            break;
    }
    return buffer.str ( );
}

void displayUsage ( )
{
    std::cerr << "Usage prettyprint FUDGE_MESSAGE_FILE" << std::endl << std::endl;
    exit ( 1 );
}

void loadFile ( fudge_byte * & bytes, fudge_i32 & numbytes, const std::string & filename )
{
    // Open the binary file
    std::ifstream input ( filename.c_str ( ), std::ifstream::in | std::ifstream::binary );
    if ( ! input.is_open ( ) )
    {
        std::cerr << "Failed to open input file \"" << filename << "\"" << std::endl;
        exit ( 1 );
    }

    // Read the file in a KB at a time
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

    input.close ( );
}

