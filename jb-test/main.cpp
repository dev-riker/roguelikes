#include <iostream>

#include "jbmaze.h"
#include "jbdungeon.h"

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

    for( i = 0; i < dungeon->getX(); i++ ) {
        delete map[ i ];
    }
    delete map;

    delete dungeon;

    return 0;
}
