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
#ifndef INC_FUDGE_CPP_FIELD_HPP
#define INC_FUDGE_CPP_FIELD_HPP

#include "fudge-cpp/datetime.hpp"
#include "fudge-cpp/optional.hpp"
#include "fudge-cpp/string.hpp"
#include <vector>

namespace fudge {

class field
{
    public:
        field ( );
        field ( const FudgeField & field );

        optional<string> name ( ) const;
        optional<fudge_i16> ordinal ( ) const;

        inline fudge_type_id type ( ) const     { return m_field.type; }
        inline fudge_i32 numbytes ( ) const     { return m_field.numbytes; }

        bool getBoolean ( ) const;
        fudge_byte getByte ( ) const;
        fudge_i16 getInt16 ( ) const;
        fudge_i32 getInt32 ( ) const;
        fudge_i64 getInt64 ( ) const;
        fudge_f32 getFloat32 ( ) const;
        fudge_f64 getFloat64 ( ) const;

        string getString ( ) const;

        FudgeMsg getMessage ( ) const;

        date getDate ( ) const;       
        time getTime ( ) const;       
        datetime getDateTime ( ) const;

        size_t getArray ( std::vector<fudge_byte> & target ) const;
        size_t getArray ( std::vector<fudge_i16> & target ) const;
        size_t getArray ( std::vector<fudge_i32> & target ) const;
        size_t getArray ( std::vector<fudge_i64> & target ) const;
        size_t getArray ( std::vector<fudge_f32> & target ) const;
        size_t getArray ( std::vector<fudge_f64> & target ) const;

        bool getAsBoolean ( ) const;
        fudge_byte getAsByte ( ) const;
        fudge_i16 getAsInt16 ( ) const;
        fudge_i32 getAsInt32 ( ) const;
        fudge_i64 getAsInt64 ( ) const;
        fudge_f32 getAsFloat32 ( ) const;
        fudge_f64 getAsFloat64 ( ) const;

        string getAsString ( ) const;

        inline const fudge_byte * bytes ( ) const { return m_field.data.bytes; }

        inline const FudgeField & raw ( ) const { return m_field; }

    private:
        FudgeField m_field;
};

}

#endif

