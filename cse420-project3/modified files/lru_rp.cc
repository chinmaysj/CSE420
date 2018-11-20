/**
 * Copyright (c) 2018 Inria
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Daniel Carvalho
 */

#include "mem/cache/replacement_policies/lru_rp.hh"

#include <cassert>
#include <memory>
//------------------------------change: included to make cout later in the function work-------------//
#include <stdio.h>
using namespace std;

#include "params/LRURP.hh"

LRURP::LRURP(const Params *p)
    : BaseReplacementPolicy(p)
{
}

void
LRURP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    // // Reset last touch timestamp
    // std::static_pointer_cast<LRUReplData>(
    //     replacement_data)->lastTouchTick = Tick(0);
     // Reset last touch timestamp
    std::static_pointer_cast<LRUReplData>(
        replacement_data)->rrpv = 3;	//---------------------------------changes rrpv value to 3 when the block needs to be invalidated------
}

void
LRURP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Update last touch timestamp
    // std::static_pointer_cast<LRUReplData>(
    //     replacement_data)->lastTouchTick = curTick();
    std::static_pointer_cast<LRUReplData>(
        replacement_data)->rrpv = 0; 	//----------------------------------changed the way touch block records block activity-----
}

void
LRURP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // Set last touch timestamp
    // std::static_pointer_cast<LRUReplData>(
    //     replacement_data)->lastTouchTick = curTick();
    std::static_pointer_cast<LRUReplData>(
        replacement_data)->rrpv = 0;	//------------------------------------changed the way touch block records block activity-----
}

ReplaceableEntry*
LRURP::getVictim(const ReplacementCandidates& candidates) const
{
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    int vicFound =0;
    // Visit all candidates to find victim
    ReplaceableEntry* victim = candidates[0];
    for (const auto& candidate : candidates) {
        // Update victim entry if nssary
        //if (x <y)
        // if (std::static_pointer_cast<LRUReplData>(candidate->replacementData)->lastTouchTick < std::static_pointer_cast<LRUReplData>(victim->replacementData)->lastTouchTick) {
        //     victim = candidate;
        // }

         if (std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv ==3) {
            victim = candidate;
            vicFound =1;
            break;
        }
    }
    //--------------------------------------------since max valueof rrpv can be 3 the worst case scenario i.e. rrpv=0 required no more than 3 increrments and searches----------//
    if(vicFound == 0){
        for (const auto& candidate : candidates) {
            //---------------------change: cout used to see how the rrpv value cahnges thorughout the system flow------------//
            //cout << "before change"<<std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv<<endl;
            std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv = std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv +1;
            //cout << "after change"<<std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv<<endl;

        }
       // victim = LRURP::getVictim(candidates);
        for (const auto& candidate : candidates) {
        // Update victim entry if nssary
        //if (x <y)
        // if (std::static_pointer_cast<LRUReplData>(candidate->replacementData)->lastTouchTick < std::static_pointer_cast<LRUReplData>(victim->replacementData)->lastTouchTick) {
        //     victim = candidate;
        // }

            if (std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv ==3) {
                victim = candidate;
                vicFound =1;
                break;
            }
        }

        if(vicFound == 0){
            for (const auto& candidate : candidates) {
                std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv = std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv +1;

            }
           // victim = LRURP::getVictim(candidates);
            for (const auto& candidate : candidates) {
            // Update victim entry if nssary
            //if (x <y)
            // if (std::static_pointer_cast<LRUReplData>(candidate->replacementData)->lastTouchTick < std::static_pointer_cast<LRUReplData>(victim->replacementData)->lastTouchTick) {
            //     victim = candidate;
            // }

                if (std::static_pointer_cast<LRUReplData>(candidate->replacementData)->rrpv ==3) {
                    victim = candidate;
                    vicFound =1;
                    break;
                }
            }

         }

    }


    return victim;
}

std::shared_ptr<ReplacementData>
LRURP::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new LRUReplData());
}

LRURP*
LRURPParams::create()
{
    return new LRURP(this);
}
