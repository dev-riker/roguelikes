/*
 * ---------------------------------------------------------------------------------
 *                                     JB-TEST
 * ---------------------------------------------------------------------------------
 *
 * Copyright (c) 2014, dev-riker
 * Copyright (c) 2008, Jamis Buck
 *
 * Homepage: http://github.com/jamis/dnd-dungeon
 * Homepage: https://github.com/dev-riker/roguelikes
 *
 * ---------------------------------------------------------------------------------
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the {organization} nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---------------------------------------------------------------------------------
 *
 */

#include <fstream>
#include <stdio.h>
#include <string.h>

#include "jbmazemask.h"

JBMazeMask::JBMazeMask( int width, int height ) {
    int i;

    m_width  = width;
    m_height = height;

    m_mask = new char*[ m_width ];
    for( i = 0; i < m_width; i++ ) {
        m_mask[ i ] = new char[ m_height ];
        memset( m_mask[ i ], 1, m_height );
    }
}


JBMazeMask::JBMazeMask( char* filename ) {
    std::ifstream in( filename );
    char     line[ 1024 ];
    int      i;
    int      j;

    if( !in ) {
        m_width = 0;
        m_height = 0;
        m_mask = 0;
    }

    /* read the first line to get the width and height */

    in >> line;
    sscanf( line, "%d,%d", &m_width, &m_height );

    /* allocate and initialize the mask array */

    m_mask = new char*[ m_width ];
    for( i = 0; i < m_width; i++ ) {
        m_mask[ i ] = new char[ m_height ];
        memset( m_mask[ i ], 0, m_height );
    }

    /* loop until we hit the end of the file, or until we have read as many
     * lines as the mask is high. */

    j = 0;
    while( !in.eof() ) {
        if( j >= m_height ) {
            break;
        }

        /* read the next line and parse it */
        in >> line;

        for( i = 0; i < m_width; i++ ) {
            if( line[ i ] == 0 ) {
                break;
            }

            /* if the current char is a '1' then the mask is valid at that point */
            m_mask[ i ][ j ] = ( line[i] == '1' ? 1 : 0 );
        }

        j++;
    }
}


JBMazeMask::JBMazeMask( JBMazeMask& master ) {
    int i;

    m_width = master.m_width;
    m_height = master.m_height;

    m_mask = new char*[ m_width ];
    for( i = 0; i < m_width; i++ ) {
        m_mask[ i ] = new char[ m_height ];
        memcpy( m_mask[ i ], master.m_mask[ i ], m_height );
    }
}


JBMazeMask::~JBMazeMask() {
    int i;

    if( m_mask == 0 ) {
        return;
    }

    for( i = 0; i < m_width; i++ ) {
        delete[] m_mask[i];
    }
    delete m_mask;

    m_mask = 0;
    m_width = m_height = 0;
}
