///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//  This is OurUO Tech Demo, for more info on this project, please refer to:         //
//                                                                                   //
//  https://github.com/roguelikes/ouruo-alpha                                        //
//                                                                                   //
//  Copyright (C) 2013  Developer Riker (https://github.com/dev-riker)               //
//                                                                                   //
//  This program is free software; you can redistribute it and/or                    //
//  modify it under the terms of the GNU General Public License                      //
//  as published by the Free Software Foundation; either version 2                   //
//  of the License, or (at your option) any later version.                           //
//                                                                                   //
//  This program is distributed in the hope that it will be useful,                  //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of                   //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    //
//  GNU General Public License for more details.                                     //
//                                                                                   //
//  You should have received a copy of the GNU General Public License                //
//  along with this program; if not, write to the Free Software                      //
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.  //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

#include "main.hpp"

Container::Container(int32_t size) : size_(size)
{
}

Container::~Container(void)
{
    inventory.clearAndDelete();
}

bool Container::Add(Actor *actor)
{
    if ((size_ > 0) && (inventory.size() >= size_)) {
        // inventory full
        return false;
    }
    inventory.push(actor);

    return true;
}

void Container::Remove(Actor *actor)
{
    inventory.remove(actor);
}
