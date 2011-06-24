# Copyright (C) 2010 - 2010, Vrai Stacey.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
#     
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TESTS = test_exception  \
        test_datetime   \
        test_string     \
        test_optional   \
        test_message    \
        test_codec      \
        test_user_types

check_PROGRAMS = $(TESTS)

noinst_HEADERS = simpletest.hpp \
		 ansi_compat.h

INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/src

FRAMEWORK_SOURCE = simpletest.cpp
EXTRA_DIST = test_data

test_exception_SOURCES = test_exception.cpp $(FRAMEWORK_SOURCE)
test_exception_LDADD = $(top_builddir)/src/libfudgecpp.la

test_datetime_SOURCES = test_datetime.cpp $(FRAMEWORK_SOURCE)
test_datetime_LDADD = $(top_builddir)/src/libfudgecpp.la

test_string_SOURCES = test_string.cpp $(FRAMEWORK_SOURCE)
test_string_LDADD = $(top_builddir)/src/libfudgecpp.la

test_optional_SOURCES = test_optional.cpp $(FRAMEWORK_SOURCE)
test_optional_LDADD = $(top_builddir)/src/libfudgecpp.la

test_message_SOURCES = test_message.cpp $(FRAMEWORK_SOURCE)
test_message_LDADD = $(top_builddir)/src/libfudgecpp.la

test_codec_SOURCES = test_codec.cpp $(FRAMEWORK_SOURCE)
test_codec_LDADD = $(top_builddir)/src/libfudgecpp.la

test_user_types_SOURCES = test_user_types.cpp $(FRAMEWORK_SOURCE)
test_user_types_LDADD = $(top_builddir)/src/libfudgecpp.la

clean-local:
	$(RM) -f *.log
