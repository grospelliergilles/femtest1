// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------

#include "IArcaneFemBC.h"
#include "DirichletBC_axl.h"

using namespace Arcane;


class DirichletBCService 
: public ArcaneDirichletBCObject
{
 public:
  explicit DirichletBCService(const ServiceBuildInfo & sbi)
  : ArcaneDirichletBCObject(sbi) {}
  
  ConstArrayView<EOS::IDirichletPointCondition*> dirichletPointConditions() { return options()->getDirichletPointCondition(); }

};
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_REGISTER_SERVICE_DIRICHLETBC(DirichletBC, DirichletBCService);
