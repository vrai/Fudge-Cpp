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
#include "fudge-cpp/optional.hpp"
#include "fudge-cpp/string.hpp"
#include <stdexcept>

DEFINE_TEST( Null )
    using fudge::optional;

    // Test NULL construction and access
    TEST_THROWS_NOTHING( optional<int> temp );

    optional<double> null;
    TEST_EQUALS_TRUE( ! null );
    TEST_THROWS_EXCEPTION( null.get ( ), std::runtime_error );
    TEST_THROWS_EXCEPTION( *null, std::runtime_error );

    // Test NULL copy constructors
    TEST_THROWS_NOTHING ( optional<double> copynull ( null ) );
    optional<double> anothernull;
    TEST_THROWS_NOTHING( null = null );
    TEST_THROWS_NOTHING( anothernull = null );
END_TEST

DEFINE_TEST( Primitive )
    using fudge::optional;

    // Test constructor and access
    TEST_THROWS_NOTHING( optional<int> temp ( 123 ) );

    optional<int> integer1 ( 123 );
    TEST_EQUALS_TRUE( integer1 );
    TEST_THROWS_NOTHING( integer1.get ( ) );
    TEST_EQUALS_INT( integer1.get ( ), 123 );
    TEST_THROWS_NOTHING( *integer1 );
    TEST_EQUALS_INT( *integer1, 123 );

    // Test copy constructors
    TEST_THROWS_NOTHING( optional<int> temp ( integer1 ) );

    optional<int> integer2 ( integer1 ), integer3;
    TEST_EQUALS_TRUE( integer2 );
    TEST_EQUALS_TRUE( ! integer3 );

    TEST_THROWS_NOTHING( integer3 = integer1 );
    TEST_EQUALS_TRUE( integer3 );
    TEST_EQUALS_INT( integer1.get ( ), *integer2 );
    TEST_EQUALS_INT( integer2.get ( ), *integer3 );

    TEST_THROWS_NOTHING( integer1 = optional<int> ( 321 ) );
    TEST_THROWS_NOTHING( integer2 = optional<int> ( ) );
    TEST_EQUALS_TRUE( integer1 );
    TEST_EQUALS_TRUE( ! integer2 );

    TEST_EQUALS_INT( integer1.get ( ), 321 );
    TEST_EQUALS_INT( integer3.get ( ), 123 );
    TEST_THROWS_EXCEPTION( integer2.get ( ), std::runtime_error );

    // Test destruction
    TEST_THROWS_NOTHING( integer1 = integer2 );
    TEST_THROWS_NOTHING( integer3 = integer1 );
    TEST_EQUALS_TRUE( ! integer1 );
    TEST_EQUALS_TRUE( ! integer2 );
    TEST_EQUALS_TRUE( ! integer3 );
    TEST_THROWS_EXCEPTION( *integer1, std::runtime_error );
    TEST_THROWS_EXCEPTION( *integer3, std::runtime_error );
END_TEST

DEFINE_TEST( Object )
    using fudge::optional;
    using fudge::string;

    // Test constructor and access
    TEST_THROWS_NOTHING( optional<string> temp ( string ( "This is a string" ) ) );

    optional<string> string1 ( string ( "String One" ) );
    TEST_EQUALS_TRUE( string1 );
    TEST_THROWS_NOTHING( string1.get ( ) );
    TEST_EQUALS_TRUE( string1.get ( ) == string ( "String One" ) );

    // Test copy constructors
    TEST_THROWS_NOTHING( optional<string> temp ( string1 ) );

    optional<string> string2 ( string1 ), string3;
    TEST_EQUALS_TRUE( string2 );
    TEST_EQUALS_TRUE( ! string3 );

    TEST_THROWS_NOTHING( string3 = string1 );
    TEST_EQUALS_TRUE( string3 );
    TEST_EQUALS_TRUE( *string1 == *string2 );
    TEST_EQUALS_TRUE( *string2 == *string3 );

    TEST_THROWS_NOTHING( string1 = optional<string> ( "String Two" ) );
    TEST_THROWS_NOTHING( string2 = optional<string> ( ) );
    TEST_EQUALS_TRUE( string1 );
    TEST_EQUALS_TRUE( ! string2 );

    TEST_EQUALS_TRUE( *string1 != *string3 );
    TEST_EQUALS_TRUE( string1.get ( ) == string ( "String Two" ) );
    TEST_EQUALS_TRUE( string3.get ( ) == string ( "String One" ) );
    TEST_THROWS_EXCEPTION( *string2, std::runtime_error );

    // Test destruction
    TEST_THROWS_NOTHING( string1 = string2 );
    TEST_THROWS_NOTHING( string3 = string1 );
    TEST_EQUALS_TRUE( ! string1 );
    TEST_EQUALS_TRUE( ! string2 );
    TEST_EQUALS_TRUE( ! string3 );
    TEST_THROWS_EXCEPTION( string1.get ( ), std::runtime_error );
    TEST_THROWS_EXCEPTION( string3.get ( ), std::runtime_error );
END_TEST

DEFINE_TEST_SUITE( Optional )
    REGISTER_TEST( Null )
    REGISTER_TEST( Primitive )
    REGISTER_TEST( Object )
END_TEST_SUITE

