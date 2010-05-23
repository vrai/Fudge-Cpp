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
        test_string 

check_PROGRAMS = $(TESTS)

noinst_HEADERS = simpletest.hpp

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

clean-local:
	$(RM) -f *.log
