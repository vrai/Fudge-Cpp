# Copyright (C) 2011 - 2011, Vrai Stacey.
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

# By default assume that Fudge-Cpp and Fudge-C have been installed in the
# standard system location. If the headers and libraries are installed in
# directories that the compiler will not be able to find by default, add
# the relevant -I / -L arguments here.
INCLUDES=
LIBS=-lfudgecpp -lfudgec

TARGETS=simple prettyprint

.PHONY: all clean

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)
	$(RM) -rf *.dSYM

$(TARGETS):
	$(CXX) $(CFLAGS) $(CXXFLAGS) -Wall -pedantic $(INCLUDES) $(LIBS) -o $@ $@.cpp
