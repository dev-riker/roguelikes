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
#include <functional>
#include <algorithm>
#include "main.hpp"

Attribute::Attribute(std::string name)
{
    name_ = name;
    effects_.clear();
    baseValue_ = 0;
    currentValue_ = 0;
}

Attribute::Attribute(char name[])
{
    name_ = name;
    effects_.clear();
    baseValue_ = 0;
    currentValue_ = 0;
}

Attribute::Attribute(void)
{
    name_ = "NO_NAME";
    effects_.clear();
    baseValue_ = 0;
    currentValue_ = 0;
}

Attribute::~Attribute(void)
{
}

void Attribute::AddBaseValueBonus(float bonus)
{
    baseValue_ += bonus;
    if (baseValue_ < 0) {
        baseValue_ = 0;
    }
}

void Attribute::SetBaseValue(float newValue)
{
    baseValue_ = newValue;
}

float Attribute::GetBaseValue(void) const
{
    return baseValue_;
}

float Attribute::GetCurrValue(void) const
{
    return currentValue_;
}

const char *Attribute::GetName(void) const
{
    return name_.c_str();
}

void Attribute::SetName(std::string name)
{
    name_ = name;
}

bool Attribute::Restore(void)
{
    // L'effetto di restore può essere applicato solamente se un attributo è stato danneggiato
    // in precedenza con un effetto di Damage.
    bool damaged = false;

    for (uint32_t i = 0; i < effects_.size(); i++) {
        if (effects_[i].effect == damage) {
            damaged = true;
            // Elimina l'effetto dalla lista. Posso uscire dal loop ed evitare
            // eventuali ricomputazioni della dimensione massima perché sono
            // sicuro che l'effetto in questione sia presente al massimo 1 volta
            // all'interno degli effetti della lista perché effetti dello stesso
            // tipo NON sono cumulabili. Nel caso in cui si modificasse questa policy
            // sarebbe necessario modificare la funzione in una simile a Attribute::RoundTick.
            effects_.erase(effects_.begin() + i);
            break;
        }
    }

    return damaged;
}

bool Attribute::Damage(float value)
{
    return UpdateEffect(damage, -value, EFFECT_INFINITE_ROUNDS);
}

bool Attribute::Fortify(float value, int32_t rounds)
{
    return UpdateEffect(fortify, +value, rounds);
}

bool Attribute::Drain(float value, int32_t rounds)
{
    return UpdateEffect(drain, -value, rounds);
}

bool Attribute::Absorb(float value, int32_t rounds)
{
    return UpdateEffect(absorb, -value, rounds);
}

void Attribute::Recompute(void)
{
    currentValue_ = baseValue_;

    for (size_t i = 0; i < effects_.size(); i++) {
        currentValue_ += effects_[i].value;
    }
    if (currentValue_ < 0.0f) {
        currentValue_ = 0.0f;
    }
}

bool Attribute::IsEffectFinished(active_effect_t effect)
{
    bool ret = false;

    if ((effect.remainingRounds > EFFECT_INFINITE_ROUNDS) && (effect.remainingRounds <= 0)) {
        ret = true;
    }

    return ret;
}

void Attribute::RoundTick(void)
{
    // Scorre la lista degli effetti attivi decrementando il numero di round rimanenti
    for (size_t i = 0; i < effects_.size(); i++) {
        if (effects_[i].remainingRounds != EFFECT_INFINITE_ROUNDS) {
            effects_[i].remainingRounds--;
        }
    }

    // Elimina gli effetti che hanno raggiunto la loro durata prefissata (effetti con remainingRounds <= 0 E > EFFECT_INFINITE_ROUNDS)
    std::vector <active_effect_t>::iterator newEnd = std::remove_if(effects_.begin(), effects_.end(), std::bind1st(std::mem_fun(&Attribute::IsEffectFinished), this));
    effects_.erase(newEnd, effects_.end());
}

bool Attribute::UpdateEffect(effect_type effect, int32_t value, int32_t rounds)
{
    // Cerca un eventuale altro attributo già attivo e nel caso lo
    // modifica aggiornandone il valore e ricomputando il numero di round attivi.
    bool updated = false;
    for (size_t i = 0; i < effects_.size(); i++) {
        if (effects_[i].effect == effect) {
            updated = true;
            if (effects_[i].value < value) {
                effects_[i].value = value;
            }
            if (effects_[i].remainingRounds < rounds) {
                effects_[i].remainingRounds = rounds;
            }
        }
    }

    if (!updated) {
        active_effect_t tmpEffect;
        tmpEffect.effect = effect;
        tmpEffect.remainingRounds = rounds;
        tmpEffect.value = value;

        effects_.push_back(tmpEffect);
    }

    return true;
}

BasicAttributes::BasicAttributes(void)
{
    agility_ =       std::unique_ptr<Agility>       (new Agility());
    endurance_ =     std::unique_ptr<Endurance>     (new Endurance());
    intelligence_ =  std::unique_ptr<Intelligence>  (new Intelligence());
    luck_ =          std::unique_ptr<Luck>          (new Luck());
    personality_ =   std::unique_ptr<Personality>   (new Personality());
    speed_ =         std::unique_ptr<Speed>         (new Speed());
    strength_ =      std::unique_ptr<Strength>      (new Strength());
    willpower_ =     std::unique_ptr<Willpower>     (new Willpower());
    npcAttributes_ = std::unique_ptr<NPCAttributes> (new NPCAttributes());
    derivedAttributes_ = nullptr;
    level_ = 1;
    isNpc_ = false;
}

BasicAttributes::~BasicAttributes(void)
{

}

void BasicAttributes::SetDerivedAttributes(DerivedAttributes *newDerivedAttributes)
{
    derivedAttributes_ = newDerivedAttributes;
}

void BasicAttributes::Recompute(void)
{
    strength_->Recompute();
    intelligence_->Recompute();
    willpower_->Recompute();
    agility_->Recompute();
    speed_->Recompute();
    endurance_->Recompute();
    personality_->Recompute();
    luck_->Recompute();

    // Calcola il valore degli attributi derivati
    if (derivedAttributes_ != nullptr) {
        // Health = endurance_(current_value) * 2 + (endurance_(base_value) / 10) * level
        // La relazione vera al momento è ben più complicata, da aggiornare con successive modifiche
        if (!isNpc_) {
            derivedAttributes_->health_->SetBaseValue((endurance_->GetCurrValue() * 2.0f) + ((endurance_->GetBaseValue() / 10.0f) * level_));
        } else {
            derivedAttributes_->health_->SetBaseValue(((strength_->GetCurrValue() + endurance_->GetCurrValue()) / 10.0f) * 2.0f);
        }
        derivedAttributes_->health_->Recompute();
        // Magicka = (Intelligence(current_value) * 2) + birthsign_bonus + racial_bonus
        // La relazione vera al momento è ben più complicata, da aggiornare con successive modifiche
        if (!isNpc_) {
            derivedAttributes_->magicka_->SetBaseValue(intelligence_->GetCurrValue() * 2.0f);
        } else {
            derivedAttributes_->magicka_->SetBaseValue(intelligence_->GetCurrValue() * 2.5f);
        }
        derivedAttributes_->magicka_->Recompute();
        // Fatigue = Endurance(curr_value) + Strength(curr_value) + Agility(curr_value) + Willpower(curr_value)
        derivedAttributes_->fatigue_->SetBaseValue(endurance_->GetCurrValue() + strength_->GetCurrValue() + agility_->GetCurrValue() + willpower_->GetCurrValue());
        derivedAttributes_->fatigue_->Recompute();
        // Encumbrance = 5 * Strength(curr_value)
        derivedAttributes_->encumbrance_->SetBaseValue(strength_->GetCurrValue() * 5.0f);
        derivedAttributes_->encumbrance_->Recompute();
    }
}

DerivedAttributes::DerivedAttributes(void)
{
    health_ =      std::unique_ptr <Health>      (new Health());
    magicka_ =     std::unique_ptr <Magicka>     (new Magicka());
    fatigue_ =     std::unique_ptr <Fatigue>     (new Fatigue());
    encumbrance_ = std::unique_ptr <Encumbrance> (new Encumbrance());
}

DerivedAttributes::~DerivedAttributes(void)
{

}

NPCAttributes::NPCAttributes(void)
{
    aggression_ =     std::unique_ptr <Aggression>     (new Aggression());
    confidence_ =     std::unique_ptr <Confidence>     (new Confidence());
    disposition_ =    std::unique_ptr <Disposition>    (new Disposition());
    energyLevel_ =    std::unique_ptr <EnergyLevel>    (new EnergyLevel());
    responsibility_ = std::unique_ptr <Responsibility> (new Responsibility());
}

NPCAttributes::~NPCAttributes(void)
{

}

Agility::Agility(void)
{
    name_ = "Agility";
}

Endurance::Endurance(void)
{
    name_ = "Endurance";
}

Intelligence::Intelligence(void)
{
    name_ = "Intelligence";
}

Luck::Luck(void)
{
    name_ = "Luck";
}

Personality::Personality(void)
{
    name_ = "Personality";
}

Speed::Speed(void)
{
    name_ = "Speed";
}

Strength::Strength(void)
{
    name_ = "Strength";
}

Willpower::Willpower(void)
{
    name_ = "Willpower";
}

Health::Health(void)
{
    name_ = "Health";
    currHealthMod_ = 0;
}

void Health::Recompute(void)
{
    Attribute::Recompute();

    currentValue_ += currHealthMod_;

    if (currentValue_ < 0.0f) {
        currentValue_ = 0.0f;
    }
}

void Health::SubtractHealth(float damage)
{
    currHealthMod_ -= damage;
}

void Health::AddHealth(float healing)
{
    currHealthMod_ += healing;
    if (currHealthMod_ > 0.0f) {
        currHealthMod_ = 0.0f;
    }
}

Magicka::Magicka(void)
{
    name_ = "Magicka";
}

Fatigue::Fatigue(void)
{
    name_ = "Fatigue";
}

Encumbrance::Encumbrance(void)
{
    name_ = "Encumbrance";
}

Aggression::Aggression(void)
{
    name_ = "Aggression";
}

Confidence::Confidence(void)
{
    name_ = "Confidence";
}

Disposition::Disposition(void)
{
    name_ = "Disposition";
}

EnergyLevel::EnergyLevel(void)
{
    name_ = "EnergyLevel";
}

Responsibility::Responsibility(void)
{
    name_ = "Responsibility";
}

