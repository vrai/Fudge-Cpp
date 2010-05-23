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
#include "fudge-cpp/datetime.hpp"
#include "fudge-cpp/exception.hpp"

DEFINE_TEST( Date )
    using fudge::date;
    using fudge::exception;

    // Test failure cases
    TEST_THROWS_EXCEPTION( date ( 8388608, 3, 15 ), exception );    // Invalid year
    TEST_THROWS_EXCEPTION( date ( -8388608, 3, 15 ), exception );   // Invalid year
    TEST_THROWS_EXCEPTION( date ( 2010, 13, 30 ), exception );      // Invalid month
    TEST_THROWS_EXCEPTION( date ( 2010, 6, 32 ), exception );       // Invalid day

    // Test boundaries
    TEST_THROWS_NOTHING( date ( FUDGEDATE_MAX_YEAR, 12, 31 ) );
    TEST_THROWS_NOTHING( date ( FUDGEDATE_MIN_YEAR, 0, 0 ) );

    // Test construction, access and comparison
    date date1, date2;
    TEST_THROWS_NOTHING( date1 = date ( 1688, 10, 16 ) );
    TEST_THROWS_NOTHING( date2 = date ( 1688, 10, 16 ) );

    TEST_EQUALS_INT( date1.year ( ), 1688 );
    TEST_EQUALS_INT( date1.month ( ), 10 );
    TEST_EQUALS_INT( date1.day ( ), 16 );

    TEST_EQUALS_TRUE( date1 > date ( ) );
    TEST_EQUALS_TRUE( date ( ) < date2 );
    TEST_EQUALS_TRUE( date1 == date2 );
    TEST_EQUALS_TRUE( date2 == date2 );

    TEST_THROWS_NOTHING( date1 = date ( 1688, 10, 15 ) );
    TEST_EQUALS_INT( date1.day ( ), 15 );
    TEST_EQUALS_TRUE( date1 < date2 );
    TEST_EQUALS_TRUE( date2 > date1 );
    TEST_EQUALS_TRUE( date1 != date2 );

    TEST_THROWS_NOTHING( date2 = date ( 1688, 9, 15 ) );
    TEST_EQUALS_INT( date2.month ( ), 9 );
    TEST_EQUALS_TRUE( date1 > date2 );
    TEST_EQUALS_TRUE( date2 < date1 );
    TEST_EQUALS_TRUE( date2 != date1 );

    TEST_THROWS_NOTHING( date1 = date ( 1687, 9, 15 ) );
    TEST_EQUALS_TRUE( date1 < date2 );
    TEST_EQUALS_TRUE( date2 > date1 );
    TEST_EQUALS_TRUE( date1 != date2 );

    TEST_EQUALS_TRUE( date1 == date1 );
    TEST_EQUALS_TRUE( date2 == date2 );
END_TEST

DEFINE_TEST( Time )
    using fudge::time;
    using fudge::exception;

    // Test failures cases
    TEST_THROWS_EXCEPTION( time ( FUDGETIME_MAX_SECONDS + 1, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ), exception );                             // Invalid seconds
    TEST_THROWS_EXCEPTION( time ( FUDGETIME_MAX_SECONDS + 1, FUDGETIME_MAX_NANOSECONDS + 1, FUDGE_DATETIME_PRECISION_NANOSECOND ), exception ); // Invalid nanoseconds

    // Test boundaries
    TEST_THROWS_NOTHING( time ( 0, 0, FUDGE_DATETIME_PRECISION_MILLENNIUM ) );
    TEST_THROWS_NOTHING( time ( FUDGETIME_MAX_SECONDS, 1000000000, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_THROWS_NOTHING( time ( 0, 0, FUDGE_DATETIME_PRECISION_MILLENNIUM, -48 ) );
    TEST_THROWS_NOTHING( time ( FUDGETIME_MAX_SECONDS, 1000000000, FUDGE_DATETIME_PRECISION_NANOSECOND, 48 ) );

    // Test precision - using 15:49:45.123456789 as a base, check the effect
    // of reducing precision.
    time time1, time2;
    TEST_THROWS_NOTHING( time1 = time ( 56985, 123456789, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_INT( time1.seconds ( ), 56985 );
    TEST_EQUALS_INT( time1.nanoseconds ( ), 123456789 );
    TEST_EQUALS_INT( time1.precision ( ), FUDGE_DATETIME_PRECISION_NANOSECOND );
    TEST_EQUALS_TRUE( ! time1.hasTimezone ( ) );

    TEST_THROWS_NOTHING( time1 = time ( 56985, 123456789, FUDGE_DATETIME_PRECISION_MICROSECOND ) );
    TEST_EQUALS_INT( time1.seconds ( ), 56985 );
    TEST_EQUALS_INT( time1.nanoseconds ( ), 123456000 );
    TEST_EQUALS_INT( time1.precision ( ), FUDGE_DATETIME_PRECISION_MICROSECOND );
    TEST_EQUALS_TRUE( ! time1.hasTimezone ( ) );

    TEST_THROWS_NOTHING( time1 = time ( 56985, 123456789, FUDGE_DATETIME_PRECISION_MILLISECOND ) );
    TEST_EQUALS_INT( time1.seconds ( ), 56985 );
    TEST_EQUALS_INT( time1.nanoseconds ( ), 123000000 );
    TEST_EQUALS_INT( time1.precision ( ), FUDGE_DATETIME_PRECISION_MILLISECOND );
    TEST_EQUALS_TRUE( ! time1.hasTimezone ( ) );

    TEST_THROWS_NOTHING( time1 = time ( 56985, 123456789, FUDGE_DATETIME_PRECISION_SECOND ) );
    TEST_EQUALS_INT( time1.seconds ( ), 56985 );
    TEST_EQUALS_INT( time1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( time1.precision ( ), FUDGE_DATETIME_PRECISION_SECOND );
    TEST_EQUALS_TRUE( ! time1.hasTimezone ( ) );

    TEST_THROWS_NOTHING( time1 = time ( 56985, 123456789, FUDGE_DATETIME_PRECISION_MINUTE ) );
    TEST_EQUALS_INT( time1.seconds ( ), 56940 );
    TEST_EQUALS_INT( time1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( time1.precision ( ), FUDGE_DATETIME_PRECISION_MINUTE );
    TEST_EQUALS_TRUE( ! time1.hasTimezone ( ) );

    TEST_THROWS_NOTHING( time1 = time ( 56985, 123456789, FUDGE_DATETIME_PRECISION_HOUR ) );
    TEST_EQUALS_INT( time1.seconds ( ), 54000 );
    TEST_EQUALS_INT( time1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( time1.precision ( ), FUDGE_DATETIME_PRECISION_HOUR );
    TEST_EQUALS_TRUE( ! time1.hasTimezone ( ) );

    TEST_THROWS_NOTHING( time1 = time ( 56985, 123456789, FUDGE_DATETIME_PRECISION_DAY ) );
    TEST_EQUALS_INT( time1.seconds ( ), 0 );
    TEST_EQUALS_INT( time1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( time1.precision ( ), FUDGE_DATETIME_PRECISION_DAY );
    TEST_EQUALS_TRUE( ! time1.hasTimezone ( ) );

    // Test construction and comparison
    TEST_THROWS_NOTHING( time1 = time ( 72057, 500000000, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_THROWS_NOTHING( time2 = time1 );

    TEST_EQUALS_TRUE( time1 > time ( ) );
    TEST_EQUALS_TRUE( time ( ) < time2 );
    TEST_EQUALS_TRUE( time1 == time1 );
    TEST_EQUALS_TRUE( time1 == time2 );

    TEST_THROWS_NOTHING( time1 = time ( 72057, 499999999, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_TRUE( time1 < time2 );
    TEST_EQUALS_TRUE( time2 > time1 );
    TEST_EQUALS_TRUE( time2 == time2 );
    TEST_EQUALS_TRUE( time1 != time2 );

    TEST_THROWS_NOTHING( time2 = time ( 72056, 499999999, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_TRUE( time1 > time2 );
    TEST_EQUALS_TRUE( time2 < time1 );
    TEST_EQUALS_TRUE( time1 != time2 );

    // Test timezone comparisons
    TEST_THROWS_NOTHING( time1 = time ( 30600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_THROWS_NOTHING( time2 = time ( 30600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, -8 ) );
    TEST_EQUALS_TRUE( time1 > time2 );

    TEST_THROWS_NOTHING( time1 = time ( 23400, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_TRUE( time1 == time2 );

    TEST_THROWS_NOTHING( time2 = time ( 30600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, -46 ) );
    TEST_EQUALS_TRUE( time2 < time1 );

    TEST_THROWS_NOTHING( time1 = time ( 75600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_EQUALS_TRUE( time1 > time2 );  // Same time - but time2 is referring to the previous day

    TEST_THROWS_NOTHING( time1 = time ( 0, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, -12 ) );
    TEST_EQUALS_TRUE( time2 == time1 );

    TEST_THROWS_NOTHING( time1 = time ( 75600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ) );
    TEST_THROWS_NOTHING( time2 = time ( 32400, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, 48 ) );
    TEST_EQUALS_TRUE( time1 == time2 );
END_TEST

DEFINE_TEST( DateTime )
    using fudge::datetime;
    using fudge::exception;

    // Test failure cases
    TEST_THROWS_EXCEPTION( datetime ( 8388608, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), exception );                     // Invalid year
    TEST_THROWS_EXCEPTION( datetime ( -8388608, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), exception );                    // Invalid year
    TEST_THROWS_EXCEPTION( datetime ( 2010, 13, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), exception );                       // Invalid month
    TEST_THROWS_EXCEPTION( datetime ( 2010, 3, 32, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), exception );                        // Invalid day
    TEST_THROWS_EXCEPTION( datetime ( 2010, 3, 20, FUDGETIME_MAX_SECONDS + 1, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND, 0 ), exception ); // Invalid seconds
    TEST_THROWS_EXCEPTION( datetime ( 2010, 3, 20, 62257, FUDGETIME_MAX_NANOSECONDS + 1, FUDGE_DATETIME_PRECISION_NANOSECOND, 0 ), exception );  // Invalid nanoseconds

    // Test precision - 1997/3/2 17:17:37.1020408010UTC+1 is the base value
    datetime datetime1;

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 20 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 62257 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 1020408010 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_NANOSECOND );
    TEST_EQUALS_TRUE( datetime1.hasTimezone ( ) );
    TEST_EQUALS_INT( datetime1.timezoneOffset ( ), 4 );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MICROSECOND, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 20 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 62257 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 1020408000 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_MICROSECOND );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MILLISECOND, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 20 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 62257 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 1020000000 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_MILLISECOND );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_SECOND, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 20 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 62257 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_SECOND );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MINUTE, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 20 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 62220 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_MINUTE );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_HOUR, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 20 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 61200 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_HOUR );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_DAY, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 20 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_DAY );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MONTH, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 3 );
    TEST_EQUALS_INT( datetime1.day ( ), 0 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_MONTH );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_YEAR, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1997 );
    TEST_EQUALS_INT( datetime1.month ( ), 0 );
    TEST_EQUALS_INT( datetime1.day ( ), 0 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_YEAR );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_CENTURY, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1900 );
    TEST_EQUALS_INT( datetime1.month ( ), 0 );
    TEST_EQUALS_INT( datetime1.day ( ), 0 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_CENTURY );

    TEST_THROWS_NOTHING( datetime1 = datetime ( 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MILLENNIUM, 4 ) );
    TEST_EQUALS_INT( datetime1.year ( ), 1000 );
    TEST_EQUALS_INT( datetime1.month ( ), 0 );
    TEST_EQUALS_INT( datetime1.day ( ), 0 );
    TEST_EQUALS_INT( datetime1.seconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.nanoseconds ( ), 0 );
    TEST_EQUALS_INT( datetime1.precision ( ), FUDGE_DATETIME_PRECISION_MILLENNIUM );
    TEST_EQUALS_TRUE( datetime1.hasTimezone ( ) );
    TEST_EQUALS_INT( datetime1.timezoneOffset ( ), 4 );
END_TEST

DEFINE_TEST_SUITE( DateTime )
    REGISTER_TEST( Date )
    REGISTER_TEST( Time )
    REGISTER_TEST( DateTime )
END_TEST_SUITE
