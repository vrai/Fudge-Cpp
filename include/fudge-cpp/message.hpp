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
#ifndef INC_FUDGE_CPP_MESSAGE_HPP
#define INC_FUDGE_CPP_MESSAGE_HPP

#include "fudge-cpp/datetime.hpp"
#include "fudge-cpp/string.hpp"
#include "fudge-cpp/optional.hpp"
#include <vector>

namespace fudge {

class message
{
    public:
        static const optional<string> noname;
        static const optional<fudge_i16> noordinal;

        message ( );
        message ( const message & source );
        message & operator= ( const message & source );

        ~message ( );

        size_t size ( ) const;

        void getField ( FudgeField & field, size_t index ) const;
        void getField ( FudgeField & field, const string & name ) const;
        void getField ( FudgeField & field, fudge_i16 ordinal ) const;

        void getFields ( std::vector<FudgeField> & fields ) const;

        void addField ( const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );

        void addField ( bool value,       const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( fudge_byte value, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( fudge_i16 value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( fudge_i32 value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( fudge_i64 value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( fudge_f32 value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( fudge_f64 value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );

        void addField ( message & value, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );

        void addField ( const std::vector<fudge_byte> & value, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const std::vector<fudge_i16> & value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const std::vector<fudge_i32> & value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const std::vector<fudge_i64> & value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const std::vector<fudge_f32> & value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const std::vector<fudge_f64> & value,  const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );

        void addField ( const string & value,   const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const date & value,     const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const time & value,     const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField ( const datetime & value, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );

        void addField ( fudge_type_id type,
                        const fudge_byte * bytes,
                        fudge_i32 numbytes,
                        const optional<string> name = noname,
                        const optional<fudge_i16> ordinal = noordinal );

        void addField4ByteArray   ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField8ByteArray   ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField16ByteArray  ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField20ByteArray  ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField32ByteArray  ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField64ByteArray  ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField128ByteArray ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField256ByteArray ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        void addField512ByteArray ( const fudge_byte * bytes, const optional<string> & name = noname, const optional<fudge_i16> ordinal = noordinal );
        
        void addFieldData ( fudge_type_id type,
                            FudgeFieldData * data,
                            fudge_i32 numbytes,
                            const optional<string> name = noname,
                            const optional<fudge_i16> ordinal = noordinal );

        FudgeMsg raw ( ) const;
    private:
        FudgeMsg m_message;
};

}

#endif

