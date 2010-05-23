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
#ifndef INC_FUDGE_CPP_OPTIONAL_HPP
#define INC_FUDGE_CPP_OPTIONAL_HPP

#include <stdexcept>

namespace fudge {

template<class T> class optional
{
    public:
        optional ( )
            : m_payload ( 0 )
        {
        }

        optional ( const T & value )
            : m_payload ( new Payload )
        {
            m_payload->count = 1;
            m_payload->value = value;
        }

        optional ( const optional & source )
            : m_payload ( source.m_payload )
        {
            if ( m_payload )
                ++( m_payload->count );
        }

        optional & operator= ( const optional & source )
        {
            if ( this != &source )
            {
                release ( );
                m_payload = source.m_payload;
                retain ( );
            }
            return *this;
        }

        ~optional ( )
        {
            release ( );
        }

        T & get ( ) const
        {
            if ( ! m_payload )
                throw std::runtime_error ( "Deferenced NULL fudge::optional" );
            return m_payload->value;
        }

        T & operator* ( ) const
        {
            return get ( );
        }

        operator bool ( ) const
        {
            return m_payload != 0;
        }

    private:
        struct Payload
        {
            T value;
            int count;
        };

        Payload * m_payload;

        void retain ( )
        {
            if ( m_payload )
                ++( m_payload->count );
        }

        void release ( )
        {
            if ( m_payload )
            {
                if ( --( m_payload->count ) < 1 )
                    delete m_payload;
            }
        }
};

}

#endif

