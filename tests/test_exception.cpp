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
#ifdef FUDGE_HAVE_STRING_H
#   include <string.h>
#endif

DEFINE_TEST( FudgeException )
    using fudge::exception;

    // Make sure that FUDGE_OK doesn't trigger an exception
    TEST_THROWS_NOTHING( exception::throwOnError ( FUDGE_OK ) );

    // Now make sure that it will throw for non-FUDGE_OK values
    TEST_THROWS_EXCEPTION( exception::throwOnError ( FUDGE_OUT_OF_MEMORY ), exception );
    TEST_THROWS_EXCEPTION( exception::throwOnError ( FUDGE_NULL_POINTER ), exception );

    // Make sure that the correct text is being retrieved for the codes
    exception outOfBytes ( FUDGE_OUT_OF_BYTES );
    std::string actual ( FudgeStatus_strerror ( FUDGE_OUT_OF_BYTES ) );
    TEST_EQUALS_MEMORY ( outOfBytes.what ( ), strlen ( outOfBytes.what ( ) ), actual.c_str ( ), actual.size ( ) );
    TEST_EQUALS_INT( outOfBytes.status ( ), FUDGE_OUT_OF_BYTES );

    FudgeStatus unknownStatus ( static_cast<FudgeStatus> ( 99999 ) );
    exception unknown ( unknownStatus );
    actual = FudgeStatus_strerror ( unknownStatus );
    TEST_EQUALS_MEMORY ( unknown.what ( ), strlen ( unknown.what ( ) ), actual.c_str ( ), actual.size ( ) );
    TEST_EQUALS_INT( unknown.status ( ), unknownStatus );
END_TEST

DEFINE_TEST_SUITE( Exception )
    REGISTER_TEST( FudgeException )
END_TEST_SUITE
