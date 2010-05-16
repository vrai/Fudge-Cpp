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
#include "fudge-cpp/string.hpp"

// Source strings - converted using iconv
static const fudge_byte StringTest_utf8Source []  = { 0xe2, 0x80, 0xbc, 0x20, 0xe2, 0x88, 0x9a, 0xe2,
                                                      0x84, 0xa6, 0x54, 0x65, 0x73, 0x74, 0xe2, 0x88,
                                                      0x91, 0xd1, 0x84 };
static const fudge_byte StringTest_utf16Source [] = { 0xff, 0xfe, 0x3c, 0x20, 0x20, 0x00, 0x1a, 0x22,
                                                      0x26, 0x21, 0x54, 0x00, 0x65, 0x00, 0x73, 0x00,
                                                      0x74, 0x00, 0x11, 0x22, 0x44, 0x04 };
static const fudge_byte StringTest_utf32Source [] = { 0xff, 0xfe, 0x00, 0x00, 0x3c, 0x20, 0x00, 0x00,
                                                      0x20, 0x00, 0x00, 0x00, 0x1a, 0x22, 0x00, 0x00,
                                                      0x26, 0x21, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00,
                                                      0x65, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00,
                                                      0x74, 0x00, 0x00, 0x00, 0x11, 0x22, 0x00, 0x00,
                                                      0x44, 0x04, 0x00, 0x00 };

// UTF8 version of the UTF16/32 source strings. These contained a BOM at the
// start of the strings which should be present in the UTF8 version.
static const fudge_byte StringTest_utf8Converted [] =  { 0xef, 0xbb, 0xbf, 0xe2, 0x80, 0xbc, 0x20, 0xe2,
                                                         0x88, 0x9a, 0xe2, 0x84, 0xa6, 0x54, 0x65, 0x73,
                                                         0x74, 0xe2, 0x88, 0x91, 0xd1, 0x84 };

// ASCII version of the source strings
static const fudge_byte StringTest_asciiConverted [] = { 0x3f, 0x20, 0x3f, 0x3f, 0x54, 0x65, 0x73, 0x74,
                                                         0x3f, 0x3f };


DEFINE_TEST( CreateFromASCII )
    using fudge::FudgeException;
    using fudge::FudgeString;

    // Test NULL failure cases
    TEST_THROWS_EXCEPTION( FudgeString failure ( static_cast<const char *> ( 0 ), 10 ), FudgeException );

    // Test NULL sucess cases
    FudgeString empty;
    TEST_EQUALS_INT( empty.size ( ), 0 );
    TEST_EQUALS_TRUE( ! empty.data ( ) );
    FudgeString emptyPtr ( static_cast<const char *> ( 0 ) );
    TEST_EQUALS_INT( emptyPtr.size ( ), 0 );
    TEST_EQUALS_TRUE( ! emptyPtr.data ( ) );

    // Construct strings and check content
    FudgeString stringOne ( "String One", 10 );
    FudgeString stringTwo ( std::string ( "String Two" ) );

    TEST_EQUALS_MEMORY( stringOne.data ( ), stringOne.size ( ), "String One", 10 );
    TEST_EQUALS_MEMORY( stringTwo.data ( ), stringTwo.size ( ), "String Two", 10 );

    // Test comparisons
    TEST_EQUALS_TRUE( stringOne < stringTwo );
    TEST_EQUALS_TRUE( stringTwo > stringOne );
    TEST_EQUALS_TRUE( stringOne == stringOne );
    TEST_EQUALS_TRUE( stringTwo == stringTwo );
    TEST_EQUALS_TRUE( stringOne != stringTwo );
END_TEST

DEFINE_TEST( CreateFromUTF8 )
    using fudge::FudgeString;

    // Construct and check contents
    FudgeString string ( StringTest_utf8Source, sizeof ( StringTest_utf8Source ), FudgeString::UTF8 );
    TEST_EQUALS_MEMORY( string.data ( ), string.size ( ), StringTest_utf8Source, sizeof ( StringTest_utf8Source ) );

    // Test ASCII output
    const std::string converted ( string.convertToStdString ( ) );
    TEST_EQUALS_MEMORY( converted.c_str ( ), converted.size ( ), StringTest_asciiConverted, sizeof ( StringTest_asciiConverted ) );

    // Test UTF16 output - first two bytes of UTF16 output jumped because the
    // source string had no BOM, so the converted string also lacks one.
    fudge_byte * convertedBytes ( 0 );
    size_t convertedSize ( 0 );
    string.convertToUTF16 ( convertedBytes, convertedSize );
    TEST_EQUALS_MEMORY( convertedBytes, convertedSize, StringTest_utf16Source + 2, sizeof ( StringTest_utf16Source ) - 2 );
    delete convertedBytes;

    // Test UTF32 output - as for the UTF16 test the lack of a BOM in the
    // source string means the comparison output has to have the first
    // four bytes skipped.
    string.convertToUTF32 ( convertedBytes, convertedSize );
    TEST_EQUALS_MEMORY( convertedBytes, convertedSize, StringTest_utf32Source + 4, sizeof ( StringTest_utf32Source ) - 4 );
    delete convertedBytes;
END_TEST

DEFINE_TEST( CreateFromUTF16 )
    using fudge::FudgeString;

    // Construct and check contents
    FudgeString string ( StringTest_utf16Source, sizeof ( StringTest_utf16Source ), FudgeString::UTF16 );
    TEST_EQUALS_MEMORY( string.data ( ), string.size ( ), StringTest_utf8Converted, sizeof ( StringTest_utf8Converted ) );

    // Test ASCII output
    const std::string converted ( string.convertToStdString ( ) );
    TEST_EQUALS_MEMORY( converted.c_str ( ), converted.size ( ), StringTest_asciiConverted, sizeof ( StringTest_asciiConverted ) );

    // Test UTF16 output
    fudge_byte * convertedBytes ( 0 );
    size_t convertedSize ( 0 );
    string.convertToUTF16 ( convertedBytes, convertedSize );
    TEST_EQUALS_MEMORY( convertedBytes, convertedSize, StringTest_utf16Source, sizeof ( StringTest_utf16Source ) );
    delete convertedBytes;

    // Test UTF32 output
    string.convertToUTF32 ( convertedBytes, convertedSize );
    TEST_EQUALS_MEMORY( convertedBytes, convertedSize, StringTest_utf32Source, sizeof ( StringTest_utf32Source ) );
    delete convertedBytes;
END_TEST

DEFINE_TEST( CreateFromUTF32 )
    using fudge::FudgeString;

    // Construct and check contents
    FudgeString string ( StringTest_utf32Source, sizeof ( StringTest_utf32Source ), FudgeString::UTF32 );
    TEST_EQUALS_MEMORY( string.data ( ), string.size ( ), StringTest_utf8Converted, sizeof ( StringTest_utf8Converted ) );

    // Test ASCII output
    const std::string converted ( string.convertToStdString ( ) );
    TEST_EQUALS_MEMORY( converted.c_str ( ), converted.size ( ), StringTest_asciiConverted, sizeof ( StringTest_asciiConverted ) );

    // Test UTF16 output
    fudge_byte * convertedBytes ( 0 );
    size_t convertedSize ( 0 );
    string.convertToUTF16 ( convertedBytes, convertedSize );
    TEST_EQUALS_MEMORY( convertedBytes, convertedSize, StringTest_utf16Source, sizeof ( StringTest_utf16Source ) );
    delete convertedBytes;

    // Test UTF32 output
    string.convertToUTF32 ( convertedBytes, convertedSize );
    TEST_EQUALS_MEMORY( convertedBytes, convertedSize, StringTest_utf32Source, sizeof ( StringTest_utf32Source ) );
    delete convertedBytes;
END_TEST

DEFINE_TEST( Comparison )
    using fudge::FudgeString;

    // Construct the test strings
    FudgeString utf8String ( StringTest_utf8Source, sizeof ( StringTest_utf8Source ), FudgeString::UTF8 );
    FudgeString utf8Duplicate ( utf8String );
    FudgeString utf16String ( StringTest_utf16Source, sizeof ( StringTest_utf16Source ), FudgeString::UTF16 );
    FudgeString utf16Truncated ( StringTest_utf16Source, sizeof ( StringTest_utf16Source ) - 2, FudgeString::UTF16 );
    FudgeString lowString ( "\b\n\t\t" );

    // Construct null and reference test strings
    FudgeString nullString, utf8Reference ( "Will be replaced by utf8String contents" );
    utf8Reference = utf8String;

    // Test NULL pointer comparions
    TEST_EQUALS_TRUE( nullString == nullString );
    TEST_EQUALS_TRUE( utf8String != nullString );
    TEST_EQUALS_TRUE( nullString != utf8String );

    // Test identical strings
    TEST_EQUALS_TRUE( utf8String == utf8String );
    TEST_EQUALS_TRUE( utf8String == utf8Duplicate );
    TEST_EQUALS_TRUE( utf8Reference == utf8Duplicate );
    TEST_EQUALS_TRUE( utf16String == utf16String );
    TEST_EQUALS_TRUE( utf16String == utf16String );
    TEST_EQUALS_TRUE( lowString == lowString );

    // Test differing strings
    TEST_EQUALS_TRUE( lowString != utf8String );
    TEST_EQUALS_TRUE( utf16String != lowString );

    TEST_EQUALS_TRUE( utf8String != utf16Truncated );
    TEST_EQUALS_TRUE( utf16Truncated != utf16String );
END_TEST

DEFINE_TEST_SUITE( String )
    REGISTER_TEST( CreateFromASCII )
    REGISTER_TEST( CreateFromUTF8 )
    REGISTER_TEST( CreateFromUTF16 )
    REGISTER_TEST( CreateFromUTF32 )
    REGISTER_TEST( Comparison )
END_TEST_SUITE
