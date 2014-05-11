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
#include <iostream>

#include "jbmaze.h"
#include "jbdungeon.h"

#define TEMPATH "tem/"

static char url[512];
static time_t seedn;

#define NORTH ( 1 )
#define SOUTH ( 2 )
#define EAST  ( 3 )
#define WEST  ( 4 )

int getWallOrientation( JBDungeonRoom* room, JBDungeonWall* wall, int *ofs )
{
    int minx;
    int maxx;
    int miny;
    int maxy;

    minx = ( wall->pt1.x < wall->pt2.x ? wall->pt1.x : wall->pt2.x );
    maxx = ( wall->pt1.x > wall->pt2.x ? wall->pt1.x : wall->pt2.x );
    miny = ( wall->pt1.y < wall->pt2.y ? wall->pt1.y : wall->pt2.y );
    maxy = ( wall->pt1.y > wall->pt2.y ? wall->pt1.y : wall->pt2.y );

    if( minx < room->topLeft.x ) {
        *ofs = miny - room->topLeft.y + 1;
        return WEST;
    }

    if( maxx >= room->topLeft.x + room->size.x ) {
        *ofs = miny - room->topLeft.y + 1;
        return EAST;
    }

    if( miny < room->topLeft.y ) {
        *ofs = minx - room->topLeft.x + 1;
        return NORTH;
    }

    if( maxy >= room->topLeft.y + room->size.y ) {
        *ofs = minx - room->topLeft.x + 1;
        return SOUTH;
    }

    *ofs = 0;
    return 0;
}

JBDungeon* prepareDungeon(void)
{
    char* width;
    char* roomcnt;
    char* height;
    char* minroomw;
    char* maxroomw;
    char* minroomh;
    char* maxroomh;
    char* sparse;
    char* secret;
    char* random;
    char* concealed;
    char* deadends;
    char* resolution;

    float minMod = 0;
    float maxMod = 0;

    JBDungeonOptions dungeonOpts;
    JBDungeon *dungeon;

    int area;
    int aveRoomArea;
    int lw;

    width = "16";
    roomcnt = "some";
    height = "16";
    minroomw = "2";
    maxroomw = "5";
    minroomh = "2";
    maxroomh = "5";
    sparse = "some";
    secret = "0";
    random = "30";
    concealed = "0";
    deadends = "15";
    resolution = "20";

    if( atoi(width) > 30 || atoi(height) > 30 ) {
        printf( "Nice try!  Attempting to bypass the limits set in the program can get you and me both "
                "in trouble!\n" );
        printf( "<p />\n" );
        printf( "In the past, large dungeons like the one you just tried to build put heavy loads on the "
                "server and caused lots of problems.  I have had to disable the larger dungeons for the "
                "online version. If this frustrates you, then I encourage you to grab the downloadable version "
                "-- you can build much larger dungeons with it!\n" );
        printf( "<p />\n" );
        printf( "Thanks!\n" );
        printf( "<p />\n" );
        printf( "Jamis Buck (<a href=\"mailto:jgb3@email.byu.edu\">jgb3@email.byu.edu</a>)" );
        exit(0);
    }


    dungeonOpts.size.x = atoi( width );
    dungeonOpts.size.y = atoi( height );
    dungeonOpts.seed = seedn;
    dungeonOpts.randomness = atoi( random );
    dungeonOpts.clearDeadends = atoi( deadends );

//  dungeonOpts.mask = new JBMazeMask( "d:\\dev\\roger.txt" );

    if( dungeonOpts.mask != 0 ) {
        dungeonOpts.size.x = dungeonOpts.mask->getWidth();
        dungeonOpts.size.y = dungeonOpts.mask->getHeight();
    }

    area = dungeonOpts.size.x * dungeonOpts.size.y;
    lw = (int)( ( dungeonOpts.size.x + dungeonOpts.size.y ) * 0.5 );

    if( strcmp( sparse, "very" ) == 0 ) {
        dungeonOpts.sparseness = lw;
    } else if( strcmp( sparse, "quite" ) == 0 ) {
        dungeonOpts.sparseness = (int)( lw * 0.75 );
    } else if( strcmp( sparse, "some" ) == 0 ) {
        dungeonOpts.sparseness = (int)( lw * 0.5 );
    } else if( strcmp( sparse, "crowd" ) == 0 ) {
        dungeonOpts.sparseness = (int)( lw * 0.25 );
    } else if( strcmp( sparse, "dense" ) == 0 ) {
        dungeonOpts.sparseness = 0;
    }

    dungeonOpts.minRoomX = atoi( minroomw );
    dungeonOpts.maxRoomX = atoi( maxroomw );
    dungeonOpts.minRoomY = atoi( minroomh );
    dungeonOpts.maxRoomY = atoi( maxroomh );

    aveRoomArea = (int)
            ( ( ( dungeonOpts.maxRoomX + dungeonOpts.minRoomX ) / 2.0 ) +
                    ( ( dungeonOpts.maxRoomY + dungeonOpts.minRoomY ) / 2.0 ) );

    if( strcmp( roomcnt, "none" ) == 0 ) {
        minMod = 0;
        maxMod = 0;
    } else if( strcmp( roomcnt, "few" ) == 0 ) {
        minMod = 0.5;
        maxMod = 1;
    } else if( strcmp( roomcnt, "some" ) == 0 ) {
        minMod = 1;
        maxMod = 2;
    } else if( strcmp( roomcnt, "many" ) == 0 ) {
        minMod = 2;
        maxMod = 4;
    } else if( strcmp( roomcnt, "lots" ) == 0 ) {
        minMod = 4;
        maxMod = 8;
    }

    if( aveRoomArea == 0 ) {
        dungeonOpts.minRoomCount = dungeonOpts.maxRoomCount = 0;
    } else {
        dungeonOpts.minRoomCount = (int)( ( lw * 4.0 / aveRoomArea ) * minMod );
        dungeonOpts.maxRoomCount = (int)( ( lw * 4.0 / aveRoomArea ) * maxMod );
    }

    dungeonOpts.secretDoors = atoi( secret );
    dungeonOpts.concealedDoors = atoi( concealed );

    dungeon = new JBDungeon( dungeonOpts );
    dungeon->setDataPath( TEMPATH );

    return dungeon;
}

int main(int argc, char *argv[])
{
    JBDungeon* dungeon;
    char**     map;
    int        i;
    int        j;

    printf( "content-type: text/plain\r\n" );
    printf( "Pragma: no-cache\r\n" );
    printf( "Expires: Thu, 1 Jan 1970 00:00:01 GMT\r\n\r\n" );

    dungeon = prepareDungeon();
    map = new char*[ dungeon->getX() ];
    for( i = 0; i < dungeon->getX(); i++ ) {
        map[ i ] = new char[ dungeon->getY() ];
        for( j = 0; j < dungeon->getY(); j++ ) {
            int cell = dungeon->getDungeonAt( i, j, 0 );

            if( cell == JBDungeon::c_WALL ) {
                map[ i ][ j ] = '#';
            } else if( cell == JBDungeon::c_PASSAGE || cell == JBDungeon::c_ROOM ) {
                map[ i ][ j ] = '.';
            }

            if( cell == JBDungeon::c_ROOM ) {
                JBMazePt p1( i, j, 0 );
                if( i > 0 ) {
                    JBMazePt p2( i-1, j, 0 );
                    int wall = dungeon->getWallBetween( p1, p2 );
                    if( wall != JBDungeonWall::c_NONE && wall != JBDungeonWall::c_WALL ) {
                        map[ i ][ j ] = 'w';
                    }
                }
                if( i+1 < dungeon->getX() ) {
                    JBMazePt p2( i+1, j, 0 );
                    int wall = dungeon->getWallBetween( p1, p2 );
                    if( wall != JBDungeonWall::c_NONE && wall != JBDungeonWall::c_WALL ) {
                        map[ i ][ j ] = 'e';
                    }
                }
                if( j > 0 ) {
                    JBMazePt p2( i, j-1, 0 );
                    int wall = dungeon->getWallBetween( p1, p2 );
                    if( wall != JBDungeonWall::c_NONE && wall != JBDungeonWall::c_WALL ) {
                        map[ i ][ j ] = 'n';
                    }
                }
                if( j < dungeon->getY() ) {
                    JBMazePt p2( i, j+1, 0 );
                    int wall = dungeon->getWallBetween( p1, p2 );
                    if( wall != JBDungeonWall::c_NONE && wall != JBDungeonWall::c_WALL ) {
                        map[ i ][ j ] = 's';
                    }
                }
            }
        }
    }

    // Print map
    for( i = 0; i < dungeon->getX(); i++ ) {
        for( j = 0; j < dungeon->getY(); j++ ) {
            std::cout<<map[i][j];
        }
        std::cout<<std::endl;
    }

    for( i = 0; i < dungeon->getX(); i++ ) {
        delete map[ i ];
    }
    delete map;

    delete dungeon;

    return 0;
}
