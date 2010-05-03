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
#ifndef INC_FUDGE_CPP_SIMPLETEST_HPP
#define INC_FUDGE_CPP_SIMPLETEST_HPP

#include <iostream>
#include <list>
#include <string>

// Test macros - perform the actual test validations
#define TEST_EQUALS_TRUE( x ) equalsTrue ( __FILE__, __LINE__, #x, x )
#define TEST_EQUALS_INT( x, y ) equalsInt ( __FILE__, __LINE__, #x, #y, x, y )
#define TEST_EQUALS_FLOAT( x, y, epsilon ) equalsFloat ( __FILE__, __LINE__, #x, #y, x, y, epsilon )
#define TEST_EQUALS_MEMORY( x, sx, y, sy ) equalsMemory ( __FILE__, __LINE__, #x, #y, x, sx, y, sy )
// TODO Add generic (templated) equality check

#define TEST_THROWS_NOTHING( x )                            \
    try                                                     \
    {                                                       \
        x;                                                  \
        throwsNothing ( __FILE__, __LINE__, #x, false );    \
    }                                                       \
    catch ( ... )                                           \
    {                                                       \
        throwsNothing ( __FILE__, __LINE__, #x, true );     \
    }

#define TEST_THROWS_EXCEPTION( x, type )                                        \
    {                                                                           \
        bool exiting_test ( false );                                            \
        try                                                                     \
        {                                                                       \
            x;                                                                  \
            exiting_test = true;                                                \
            throwsException ( __FILE__, __LINE__, #x, #type, false, false );    \
        }                                                                       \
        catch ( const type & )                                                  \
        {                                                                       \
            if ( exiting_test )                                                 \
                throw;                                                          \
            throwsException ( __FILE__, __LINE__, #x, #type, true, true );      \
        }                                                                       \
        catch ( ... )                                                           \
        {                                                                       \
            if ( exiting_test )                                                 \
                throw;                                                          \
            throwsException ( __FILE__, __LINE__, #x, #type, true, false );     \
        }                                                                       \
    }

// Helper macros - the test framework should be accessed using these
#define DEFINE_TEST_SUITE( name )                           \
    int main ( int argc, char * argv [ ] )                  \
    {                                                       \
        simpletest::Suite suite ( #name );

#define END_TEST_SUITE                                      \
        return suite.run ( );                               \
    }

// TODO Initialise Fudge library
#define DEFINE_TEST( name )                                 \
    struct SimpleTest_Test_##name : public simpletest::Test \
    {                                                       \
        SimpleTest_Test_##name ( )                          \
            : Test ( #name )                                \
        {                                                   \
        }                                                   \
                                                            \
        int test ( )                                        \
        {                                                   \

#define END_TEST                                            \
            return 0;                                       \
        }                                                   \
    };

#define REGISTER_TEST( name )                               \
    suite.registerTest ( new SimpleTest_Test_##name );

// Test suite implementation - don't call these directly, use the macros
namespace simpletest {

class Suite;

class Test
{
    public:
        Test ( const std::string & name );
        int run ( const Suite & suite );
        virtual int test ( ) = 0;
        const std::string & name ( ) const;
    protected:
        void equalsTrue ( const char * file, int line, const char * text, int value );
        void equalsInt ( const char * file, int line, const char * xStr, const char * yStr, int x, int y );
        void equalsFloat ( const char * file, int line, const char * xStr, const char * yStr, double x, double y, double epsilon );
        void equalsMemory ( const char * file, int line, const char * xStr, const char * yStr, const void * x, int sizeX, const void * y, int sizeY );
        void throwsNothing ( const char * file, int line, const char * text, bool threw );
        void throwsException ( const char * file, int line, const char * text, const char * type, bool threw, bool correct );
    private:
        std::string m_name, m_suite;
        std::ostream & log ( ) const;
};

class Suite
{
    public:
        Suite ( const std::string & name );
        ~Suite ( );
        void registerTest ( Test * test );
        int run ( );
        const std::string & name ( ) const;
    private:
        const std::string m_name;
        std::list<Test *> m_tests;
        std::ostream & log ( ) const;
};

}

#endif

