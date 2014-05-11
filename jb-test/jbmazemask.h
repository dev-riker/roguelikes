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
 * JBMazeMask represents a two-dimensional mask that may be applied to a
 * maze object (see JBMaze).  The mask then determines what areas of the
 * map are valid for generating the maze.
 *
 */
#ifndef __JBMAZEMASK_H__
#define __JBMAZEMASK_H__

class JBMazeMask {
public:

    /* ------------------------------------------------------------------ *
     * JBMazeMask( int width, int height )
     *
     * Creates a new JBMazeMask object with the given width and height.
     * The mask is initialized to be empty.
     * ------------------------------------------------------------------ */
    JBMazeMask( int width, int height );

    /* ------------------------------------------------------------------ *
     * JBMazeMask( char* filename )
     *
     * Creates a new JBMazeMask object from the data in the indicated
     * file.  The first line of the file must be the width and the height
     * of the mask (comma delimited).  Subsequent lines represent the rows
     * in the mask, where each character in the line must be a '0' or a '1'
     * and represents whether the mask at that point is 'on' (1) or 'off
     * (0).
     * ------------------------------------------------------------------ */
    JBMazeMask( char* filename );

    /* ------------------------------------------------------------------ *
     * JBMazeMask( JBMazeMask& master )
     *
     * Copy constructor -- creates a new JBMazeMask that is an exact
     * duplicate of the indicated mask object.
     * ------------------------------------------------------------------ */
    JBMazeMask( JBMazeMask& master );

    /* ------------------------------------------------------------------ *
     * ~JBMazeMask()
     *
     * Destroys the mask object and deallocates any resources it used.
     * ------------------------------------------------------------------ */
    virtual ~JBMazeMask();

    /* ------------------------------------------------------------------ *
     * int getWidth()
     *
     * Returns the width of the mask.
     * ------------------------------------------------------------------ */
    int getWidth() { return m_width; }

    /* ------------------------------------------------------------------ *
     * int getHeight()
     *
     * Returns the height of the mask.
     * ------------------------------------------------------------------ */
    int getHeight() { return m_height; }

    /* ------------------------------------------------------------------ *
     * int getMaskAt( int x, int y )
     *
     * Returns the value of the mask at the indicated point.  A 1 means
     * that the mask is "valid" at that point (ie, the maze may be drawn
     * there).  A 0 means that the maze must consider this point a wall.
     * ------------------------------------------------------------------ */
    int getMaskAt( int x, int y ) { return m_mask[ x ][ y ]; }

private:

    int    m_width;
    int    m_height;

    char** m_mask;
};

#endif /* __JBMAZEMASK_H__ */
