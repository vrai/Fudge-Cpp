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
#include "fudge-cpp/codec.hpp"
#include "fudge-cpp/exception.hpp"
#include "fudge/codec.h"

namespace fudge {

envelope codec::decode ( const fudge_byte * bytes, fudge_i32 numbytes ) const
{
    FudgeMsgEnvelope target;
    exception::throwOnError ( FudgeCodec_decodeMsg ( &target, bytes, numbytes ) );
    return target;
}

void codec::encode ( const envelope & source, fudge_byte * & bytes, fudge_i32 & numbytes ) const
{
    exception::throwOnError ( FudgeCodec_encodeMsg ( source.raw ( ), &bytes, &numbytes ) );
}

}

