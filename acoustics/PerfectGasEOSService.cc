// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------

#include "IEquationOfState.h"
#include "PerfectGasEOS_axl.h"

using namespace Arcane;


class PerfectGasEOSService 
: public ArcanePerfectGasEOSObject
{
 public:
  explicit PerfectGasEOSService(const ServiceBuildInfo & sbi)
  : ArcanePerfectGasEOSObject(sbi) {}
  
  void initEOS(const CellGroup& group) override;
  void applyEOS(const CellGroup & group) override;
  String namebc() override;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

String PerfectGasEOSService::
namebc(){

    return "toto" ;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void PerfectGasEOSService::
initEOS(const CellGroup& group)
{
  ENUMERATE_CELL(icell, group){
    std::cout << " ARCANEFEM INIT "<< std::endl;
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void PerfectGasEOSService::
applyEOS(const CellGroup & group)
{
  ENUMERATE_CELL(icell, group){
     std::cout << " ARCANEFEM applyEOS"<< std::endl;
  }
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_SERVICE_PERFECTGASEOS(PerfectGas, PerfectGasEOSService);