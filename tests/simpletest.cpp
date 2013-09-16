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
#include <stdio.h>
#ifdef FUDGE_HAVE_MATH_H
#   include <math.h>
#endif
#ifdef FUDGE_HAVE_TIME_H
#   include <time.h>
#endif
#ifdef FUDGE_HAVE_STDLIB_H
#   include <stdlib.h>
#endif
#ifdef FUDGE_HAVE_UNISTD_H
#   include <unistd.h>
#endif
#include "ansi_compat.h"

namespace
{
    std::string getTimeAsString ( )
    {
        time_t now = time ( 0 );
        struct tm utcTime;
        gmtime_r ( &now, &utcTime );
        char timeString [ 64 ];
        strftime ( timeString, sizeof ( timeString ), "%Y-%m-%d %H:%M:%S", &utcTime );
        return timeString;
    }
}

namespace simpletest {

SimpleTestException::SimpleTestException ( const std::string & error, const char * file, int line ) throw ( )
    : std::runtime_error ( error )
    , m_file ( file )
    , m_line ( line )
{
}

SimpleTestException::~SimpleTestException ( ) throw ( )
{
}

Suite::Suite ( const std::string & name )
    : m_name ( name )
{
    // Redirect stdout to a file
    if ( ! freopen ( ( "test_" + m_name + ".log" ).c_str ( ), "wb", stdout ) )
    {
        std::cerr << "Failed to redirect stdout to \"test_" << m_name << ".log\"" << std::endl;
        exit ( 1 );
    }

    // Log output header
    log ( ) << "Log file for test suite \"" << m_name << "\"" << std::endl;

    // Log host and PID (if available)
    char hostname [ 256 ];
    const int gotHostname ( gethostname ( hostname, sizeof ( hostname ) ) );
#ifdef FUDGE_HAS_GETPID
    const int pid ( getpid ( ) );
#else
    const int pid ( -1 );
#endif
    log ( ) << "Running on host \"" << ( gotHostname ? "UNKNOWN" : hostname ) << "\" with PID " << pid << std::endl;

}

Suite::~Suite ( )
{
    for ( std::list<Test *>::iterator testIt ( m_tests.begin ( ) ); testIt != m_tests.end ( ); ++testIt )
        delete *testIt;
}

void Suite::registerTest ( Test * test )
{
    m_tests.push_back ( test );
}

int Suite::run ( )
{
    std::cerr << "Running test suite \"" << m_name << "\" with " << m_tests.size ( ) << " tests:" << std::endl;
    for ( std::list<Test *>::iterator testIt ( m_tests.begin ( ) ); testIt != m_tests.end ( ); ++testIt )
    {
        std::cerr << "    " << ( *testIt )->name ( ) << " ... " << std::flush;
        const int result ( ( *testIt )->run ( *this ) );
        if ( result )
            return result;
        else
            std::cerr << "OK" << std::endl;
    }
    return 0;
}

const std::string & Suite::name ( ) const
{
    return m_name;
}

std::ostream & Suite::log ( ) const
{
    std::cout << "--- " << getTimeAsString ( ) << " [" << m_name << "] ";
    return std::cout;
}

Test::Test ( const std::string & name )
    : m_name ( name )
{
}

int Test::run ( const Suite & suite )
{
    m_suite = suite.name ( );
    log ( ) << "Running test \"" << m_name << "\"" << std::endl;
    int returnCode ( 0 );

    try
    {
        returnCode = test ( );
    }
    catch ( const SimpleTestException & exception )
    {
        std::cerr << "FAILED : " << exception.m_file << ":" << exception.m_line << " : " << exception.what ( ) << std::endl;
        returnCode = 1;
    }
    catch ( const std::exception & exception )
    {
        std::cerr << "FAILED : Unexcepted exception caught: " << exception.what ( ) << std::endl;
        returnCode = 1;
    }
    catch ( ... )
    {
        std::cerr << "FAILED: Unexcepted, non-std::exception caught" << std::endl;
        returnCode = 1;
    }

    log ( ) << "Finished test \"" << m_name << "\" with result code " << returnCode << std::endl;
    m_suite.clear ( );
    return returnCode;
}

const std::string & Test::name ( ) const
{
    return m_name;
}

std::ostream & Test::log ( ) const
{
    std::cout << "--- " << getTimeAsString ( ) << " [" << m_suite << "/" << m_name << "] ";
    return std::cout;
}

void Test::equalsTrue ( const char * file, int line, const char * text, int value )
{
    if ( ! value )
    {
        std::ostringstream error;
        error << "!( " << text << " )";
        throw SimpleTestException ( error.str ( ), file, line );
    }
    log ( ) << file << ":" << line << " : " << text << " = true" << std::endl;
}

void Test::equalsInt ( const char * file, int line, const char * xStr, const char * yStr, int64_t x, int64_t y )
{
    if ( x != y )
    {
        std::ostringstream error;
        error << xStr << " (" << x << ") != " << yStr << " (" << y << ")";
        throw SimpleTestException ( error.str ( ), file, line );
    }
    log ( ) << file << ":" << line << " : " << xStr << " (" << x << ") == " << yStr << " (" << y << ")" << std::endl;
}

void Test::equalsFloat ( const char * file, int line, const char * xStr, const char * yStr, double x, double y, double epsilon )
{
    const double diff ( fabs ( x - y ) );
    if ( diff > epsilon )
    {
        std::ostringstream error;
        error << xStr << " (" << x << ") and " << yStr << " (" << y << ") differ by " << diff << " (limit is " << epsilon << ")";
        throw SimpleTestException ( error.str ( ), file, line );
    }
    log ( ) << file << ":" << line << " : " << xStr << " (" << x << ") and " << yStr << " (" << y << ") differ by "
            << diff << " (limit is " << epsilon << ")" << std::endl;
}

void Test::equalsMemory ( const char * file, int line, const char * xStr, const char * yStr, const void * x, int sizeX, const void * y, int sizeY )
{
    std::ostringstream error;

    if ( sizeX != sizeY )
    {
        error << xStr << " != " << yStr << " as size " << sizeX << " != " << sizeY;
        throw SimpleTestException ( error.str ( ), file, line );
    }

    if ( x && y )
    {
        const int8_t * xBytes ( static_cast<const int8_t *> ( x ) ),
                     * yBytes ( static_cast<const int8_t *> ( y ) );

        for ( int index ( 0 ); index < sizeX; ++index )
            if ( xBytes [ index ] != yBytes [ index ] )
            {
                error << xStr << " != " << yStr << ", difference at byte " << index << ": "
                      << static_cast<unsigned int> ( xBytes [ index ] ) << " != "
                      << static_cast<unsigned int> ( yBytes [ index ] );
                throw SimpleTestException ( error.str ( ), file, line );
            }
    }
    else
        if ( x != y )
        {
            error << xStr << " != " << yStr << " cannot be compared as one is null";
            throw SimpleTestException ( error.str ( ), file, line );
        }

    log ( ) << file << ":" << line << " : " << xStr << " (" << sizeX << " bytes) == "
            << yStr << " (" << sizeY << " bytes)" << std::endl;
}

void Test::throwsNothing ( const char * file, int line, const char * text, bool threw )
{
    if ( threw )
    {
        std::ostringstream error;
        error << " { " << text << " } was not supposed to throw an exception, but did";
        throw SimpleTestException ( error.str ( ), file, line );
    }
    log ( ) << file << ":" << line << " { " << text << " } did not throw an exception" << std::endl;
}

void Test::throwsException ( const char * file, int line, const char * text, const char * type, bool threw, bool correct )
{
    std::ostringstream error;

    if ( ! threw )
    {
        error << "{ " << text << " } was supposed to throw " << type << ", but did not throw";
        throw SimpleTestException ( error.str ( ), file, line );
    }
    if ( ! correct )
    {
        error << "{ " << text << " } was supposed to throw " << type << ", but threw something else";
        throw SimpleTestException ( error.str ( ), file, line );
    }
    log ( ) << file << ":" << line << " { " << text << " } threw " << type << std::endl;
}

}

