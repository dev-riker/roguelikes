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

#ifndef _GUI_H_
#define _GUI_H_

class Menu
{
public:
    enum MenuItemCode  {
        NONE,
        NEW_GAME,
        CONTINUE,
        EXIT
    };

public:
                 ~Menu   (void);
    void         Clear   (void);
    void         AddItem (MenuItemCode code, const char *label);
    MenuItemCode Pick    (void);

protected:
    struct MenuItem {
        MenuItemCode code;
        const char * label;
    };

    TCODList <MenuItem *> items_;
};

class Gui : public Persistent
{
public:
    Menu menu_;

public:
            Gui     (void);
    virtual ~Gui    (void);
    void    Render  (void);
    void    Message (const TCODColor &col, const char *text, ...);
    void    Load    (TCODZip &zip);
    void    Save    (TCODZip &zip);
    void    Clear   (void);

protected:
    struct message {
        char *    text;
        TCODColor col;
        message   (const char *text, const TCODColor &col);
        ~message  (void);
    };
    TCODList <message *> log_;
    TCODConsole *        con_;

protected:
    void RenderBar       (int32_t x, int32_t y, int32_t width, const char *name,
                           float value, float maxValue, const TCODColor &barColor,
                           const TCODColor &backColor);
    void RenderMouseLook (void);
};

#endif // _GUI_H_
