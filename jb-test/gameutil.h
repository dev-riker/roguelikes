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
#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__

#ifdef unix
#define rand  random
#define srand srandom
#endif

typedef struct __weightedlist__ WEIGHTEDLIST;

struct __weightedlist__ {
    long          data;
    int           weight;
    WEIGHTEDLIST* next;
};

/* ---------------------------------------------------------------------- *
 * Functions
 * ---------------------------------------------------------------------- */

//#ifdef __cplusplus
//extern "C" {
//#endif

/* ---------------------------------------------------------------------- *
 * Formats 'n', inserting commas as the thousands and millions separator.
 * ---------------------------------------------------------------------- */
void commify( char* buf, int n );

/* ---------------------------------------------------------------------- *
 * Rolls the given-sided die, 'count' number of times.  Thus, 3d20 would
 * be rolled as "rollDice( 3, 20 )".
 * ---------------------------------------------------------------------- */
int rollDice( int count, int sides );

/* ---------------------------------------------------------------------- *
 * Randomly selects a number between 'lo' and 'hi' (inclusive)
 * ---------------------------------------------------------------------- */
long selectBetween( long lo, long hi );

/* ---------------------------------------------------------------------- *
 * Selects a very large random number
 * ---------------------------------------------------------------------- */
long longRand( void );

/* ---------------------------------------------------------------------- *
 * Returns the ordinal suffix (th, st, nd, rd) for the given number.
 * ---------------------------------------------------------------------- */
char* getOrdinalSuffix( int num );

/* ---------------------------------------------------------------------- *
 * Adds the given data to the weighted list with the given weight.
 * ---------------------------------------------------------------------- */
int addToWeightedList( WEIGHTEDLIST** list, long data, int weight );

/* ---------------------------------------------------------------------- *
 * Retrieves the item with the given index in it's weight range from the
 * weighted list.
 * ---------------------------------------------------------------------- */
long getWeightedItem( WEIGHTEDLIST** list, int index, int* count );

/* ---------------------------------------------------------------------- *
 * Destroys the given weighted list.
 * ---------------------------------------------------------------------- */
void destroyWeightedList( WEIGHTEDLIST** list );

//#ifdef __cplusplus
//}
//#endif

#endif /* __GAMEUTIL_H__ */
