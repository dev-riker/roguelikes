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

MobileBaseClass::MobileBaseClass(void)
{
}

WarriorClass::WarriorClass(void)
{
    skills_.armorer_->SetMajor(true);
    skills_.athletics_->SetMajor(true);
    skills_.blade_->SetMajor(true);
    skills_.block_->SetMajor(true);
    skills_.blunt_->SetMajor(true);
    skills_.handToHand_->SetMajor(true);
    skills_.heavyArmor_->SetMajor(true);

    skills_.blade_->AddBaseValueBonus(5);
    skills_.blunt_->AddBaseValueBonus(5);
    skills_.handToHand_->AddBaseValueBonus(5);
    skills_.heavyArmor_->AddBaseValueBonus(10);
    skills_.mercantile_->AddBaseValueBonus(10);
    skills_.speechCraft_->AddBaseValueBonus(10);
    skills_.UpdateSkillTree();

    basicAttributes_.SetDerivedAttributes(&derivedAttributes_);
    basicAttributes_.strength_->SetBaseValue(40);
    basicAttributes_.intelligence_->SetBaseValue(40);
    basicAttributes_.willpower_->SetBaseValue(30);
    basicAttributes_.agility_->SetBaseValue(30);
    basicAttributes_.speed_->SetBaseValue(40);
    basicAttributes_.endurance_->SetBaseValue(40);
    basicAttributes_.personality_->SetBaseValue(50);
    basicAttributes_.luck_->SetBaseValue(50);
    basicAttributes_.Recompute();

}
