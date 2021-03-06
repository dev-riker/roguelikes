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

#ifndef _DESTRUCTIBLE_H_
#define _DESTRUCTIBLE_H_

class Destructible : public Persistent
{
public:
    //float		maxHp_;			// maximum health points
    //float		hp_;			// current health points
    float		  defense_;     // hit points deflected
    const char *  corpseName_;  // the actor's name once dead/destroyed

    MobileBaseClass  *mobileClass_;

public:
                          Destructible  (float maxHp, float defense, const char *corpseName);
    virtual               ~Destructible (void) { };
    inline bool           IsDead        (void) { return  mobileClass_->basicAttributes_.GetHealth()->GetCurrValue() <= 0; }
    float                 TakeDamage    (Actor *owner, float damage);
    float                 Heal          (float amount);
    virtual void          Die           (Actor *owner);
    void                  Load          (TCODZip &zip);
    void                  Save          (TCODZip &zip);
    static Destructible * Create        (TCODZip &zip);

protected:
    enum DestructibleType {
        MONSTER, PLAYER
    };
};

class MonsterDestructible : public Destructible
{
public:
         MonsterDestructible (float maxHp, float defense, const char *corpseName);
    void Die                 (Actor *owner);
    void Save                (TCODZip &zip);

public:
    // Insieme degli NPC Attributes
    std::unique_ptr <Attribute>	aggression_;
    std::unique_ptr <Attribute>	confidence_;
    std::unique_ptr <Attribute>	disposition_;
    std::unique_ptr <Attribute>	energyLevel_;
    std::unique_ptr <Attribute>	responsibility_;
};

class PlayerDestructible : public Destructible
{
public:
         PlayerDestructible (float maxHp, float defense, const char *corpseName);
    void Die                (Actor *owner);
    void Save               (TCODZip &zip);
};

#endif // _DESTRUCTIBLE_H_
