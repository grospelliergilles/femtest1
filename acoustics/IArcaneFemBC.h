// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
#ifndef IARCANEFEMBC_H
#define IARCANEFEMBC_H

#include <arcane/ItemTypes.h>
#include <arcane/VariableTypedef.h>

using namespace Arcane;

namespace EOS
{

class IDirichletPointCondition
{
 public:
  virtual Arcane::NodeGroup getNode() =0;
  virtual Real getValue() =0;
  virtual Real getPenalty() =0;
  virtual String getEnforceDirichletMethod() =0;
};

class IArcaneFemBC
{
 public:
  virtual ~IArcaneFemBC() = default; 
  virtual ConstArrayView<EOS::IDirichletPointCondition*> dirichletPointConditions() =0;
};

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif
