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

#include <iostream>
#include <stdio.h>
#include "main.hpp"

Destructible::Destructible(float maxHp, float defense, const char *corpseName) :
        defense_(defense), corpseName_(corpseName)
{
    /*
    // Basic attributes
    agility_ = std::unique_ptr<Attribute> (new Attribute());
    agility_->SetName("Agility");
    endurance_ = std::unique_ptr<Attribute> (new Attribute());
    endurance_->SetName("Endurance");
    intelligence_ = std::unique_ptr<Attribute> (new Attribute());
    intelligence_->SetName("Intelligence");
    luck_ = std::unique_ptr<Attribute> (new Attribute());
    luck_->SetName("Luck");
    personality_ = std::unique_ptr<Attribute> (new Attribute());
    personality_->SetName("Personality");
    speed_ = std::unique_ptr<Attribute> (new Attribute());
    speed_->SetName("Speed");
    strength_ = std::unique_ptr<Attribute> (new Attribute());
    strength_->SetName("Strength");
    willpower_ = std::unique_ptr<Attribute> (new Attribute());
    willpower_->SetName("Willpower");
    // Derived attributes
    health_ = std::unique_ptr<Attribute> (new Attribute());
    health_->SetName("Health");
    magicka_ = std::unique_ptr<Attribute> (new Attribute());
    magicka_->SetName("Magicka");
    fatigue_ = std::unique_ptr<Attribute> (new Attribute());
    fatigue_->SetName("Fatigue");
    encumbrance_ = std::unique_ptr<Attribute> (new Attribute());
    encumbrance_->SetName("Encumbrance");
    */
    mobileClass_ = nullptr;
}

float Destructible::TakeDamage(Actor *owner, float damage)
{
    int32_t tmpHp = mobileClass_->basicAttributes_.GetHealth()->GetCurrValue();
    damage -= defense_;

    if (damage > 0) {
        tmpHp -= damage;
        mobileClass_->basicAttributes_.GetHealth()->SubtractHealth((int32_t) damage);
        mobileClass_->basicAttributes_.GetHealth()->Recompute();

        if (mobileClass_->basicAttributes_.GetHealth()->GetCurrValue() <= 0) {
            //mobileClass_.basicAttributes_.GetHealth()->SetBaseValue(0);
            Die(owner);
        }
    } else {
        damage = 0;
    }

    return damage;
}

float Destructible::Heal(float amount)
{
    int32_t tmpHp = mobileClass_->basicAttributes_.GetHealth()->GetCurrValue();
    int32_t tmpBaseValue = mobileClass_->basicAttributes_.GetHealth()->GetBaseValue();
    tmpHp += amount;

    mobileClass_->basicAttributes_.GetHealth()->AddHealth((int32_t) amount);
    mobileClass_->basicAttributes_.GetHealth()->Recompute();

    return amount;
}

void Destructible::Die(Actor *owner)
{
    // transform the actor into a corpse!
    owner->ch_ = '%';
    owner->col_ = TCODColor::darkRed;
    owner->name_ = corpseName_;
    owner->blocks_ = false;
    // make sure corpses are drawn before living actors
    engine.SendToBack(owner);
}

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp, defense, corpseName)
{
    mobileClass_ = new NPCWarriorClass();
    // NPC attributes
    aggression_ = std::unique_ptr<Attribute> (new Attribute());
    aggression_->SetName("Aggression");
    confidence_ = std::unique_ptr<Attribute> (new Attribute());
    confidence_->SetName("Confidence");
    disposition_ = std::unique_ptr<Attribute> (new Attribute());
    disposition_->SetName("Disposition");
    energyLevel_ = std::unique_ptr<Attribute> (new Attribute());
    energyLevel_->SetName("Energy Level");
    responsibility_ = std::unique_ptr<Attribute> (new Attribute());
    responsibility_->SetName("Responsibility");
}

void MonsterDestructible::Die(Actor *owner)
{
    // transform it into a nasty corpse! it doesn't block, can't be
    // attacked and doesn't move
    engine.gui_->Message(TCODColor::lightGrey, "%s e' morto", owner->name_);
    Destructible::Die(owner);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
        Destructible(maxHp, defense, corpseName)
{
    mobileClass_ = new WarriorClass();
}

void PlayerDestructible::Die(Actor *owner)
{
    engine.gui_->Message(TCODColor::red,"Sei morto!");
    Destructible::Die(owner);
    engine.gameStatus = Engine::DEFEAT;
}
