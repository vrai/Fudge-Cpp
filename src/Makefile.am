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
 
lib_LTLIBRARIES = libfudgecpp.la

INCLUDES = -I$(top_srcdir)/include

noinst_HEADERS =

libfudgecpp_la_SOURCES = codec.cpp      \
                         datetime.cpp   \
                         envelope.cpp   \
                         exception.cpp  \
                         field.cpp      \
                         fudge.cpp      \
                         message.cpp    \
                         string.cpp

libfudgecpp_la_LDFLAGS = -no-undefined -version-info @API_VERSION@

