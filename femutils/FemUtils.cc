﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2023 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* FemUtils.cc                                                 (C) 2022-2023 */
/*                                                                           */
/* Utilitary classes for FEM.                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "FemUtils.h"

#include <arcane/utils/FatalErrorException.h>
#include <arcane/utils/PlatformUtils.h>
#include <arcane/utils/ValueConvert.h>
#include <arcane/utils/ITraceMng.h>
#include <arcane/IParallelMng.h>
#include <arcane/VariableTypes.h>
#include <arcane/IItemFamily.h>
#include <map>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::FemUtils
{
using namespace Arcane::MatVec;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void _convertNumArrayToCSRMatrix(Matrix& out_matrix, MDSpan<const Real, MDDim2> in_matrix)
{
  const Int32 matrix_size = in_matrix.extent0();
  if (matrix_size != in_matrix.extent1())
    ARCANE_FATAL("Not a square matrix ({0},{1})", matrix_size, in_matrix.extent1());
  UniqueArray<Int32> nb_non_zero_in_row(matrix_size);

  Int32 nb_non_zero = 0;
  for (Int32 i = 0; i < matrix_size; ++i) {
    Int32 row_non_zero = 0;
    for (Int32 j = 0; j < matrix_size; ++j)
      if (in_matrix(i, j) != 0)
        ++row_non_zero;
    nb_non_zero_in_row[i] = row_non_zero;
    nb_non_zero += row_non_zero;
  }

  UniqueArray<Int32> columns(nb_non_zero);
  UniqueArray<Real> values(nb_non_zero);

  std::ostringstream ostr;
  ostr.flags(ios::scientific);
  ostr.precision(17);
  {
    Int32 fill_index = 0;
    for (Int32 i = 0; i < matrix_size; ++i) {
      for (Int32 j = 0; j < matrix_size; ++j) {
        Real v = in_matrix(i, j);
        if (matrix_size<200 && v!=0.0)
          ostr << "MAT[" << i << "][" << j << "] = " << v << endl;
        if (v != 0) {
          columns[fill_index] = j;
          values[fill_index] = v;
          ++fill_index;
        }
      }
    }
  }
  std::cout << ostr.str();
  out_matrix.setRowsSize(nb_non_zero_in_row);
  out_matrix.setValues(columns, values);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void checkNodeResultFile(ITraceMng* tm, const String& filename,
                         const VariableNodeReal& node_values, double epsilon)
{
  ARCANE_CHECK_POINTER(tm);

  tm->info() << "CheckNodeResultFile filename=" << filename;
  if (filename.empty())
    ARCANE_FATAL("Invalid empty filename");
  IItemFamily* node_family = node_values.variable()->itemFamily();
  if (!node_family)
    ARCANE_FATAL("Variable '{0}' is not allocated", node_values.name());

  std::map<Int64, double> item_reference_values;
  {
    std::ifstream sbuf(filename.localstr());
    double read_value = 0.0;
    Int64 read_uid = 0;
    if (!sbuf.eof())
      sbuf >> ws;
    while (!sbuf.eof()) {
      sbuf >> read_uid >> ws >> read_value;
      if (sbuf.fail() || sbuf.bad())
        ARCANE_FATAL("Error during parsing of file '{0}'", filename);
      item_reference_values.insert(std::make_pair(read_uid, read_value));
      sbuf >> ws;
    }
  }

  tm->info() << "NB_Values=" << item_reference_values.size();

  // Get Max UID
  Int64 max_uid = 0;
  ENUMERATE_ (Node, inode, node_family->allItems()) {
    Node node = *inode;
    if (node.uniqueId() > max_uid)
      max_uid = node.uniqueId();
  }

  std::map<Int64, double> item_current_values;

  ENUMERATE_ (Node, inode, node_family->allItems()) {
    Node node = *inode;
    item_current_values[node.uniqueId()] = node_values[node];
  }

  Int64 nb_error = 0;
  for (const auto& x : item_current_values) {
    Int64 uid = x.first;
    Real v = x.second;
    auto x_ref = item_reference_values.find(uid);
    if (x_ref != item_reference_values.end()) {
      Real ref_v = x_ref->second;
      if (!TypeEqualT<double>::isNearlyEqualWithEpsilon(ref_v, v, epsilon)) {
        ++nb_error;
        if (nb_error < 50)
          tm->info() << String::format("ERROR: uid={0} ref={1} v={2} diff={3}",
                                       uid, ref_v, v, ref_v - v);
      }
    }
  }
  if (nb_error > 0)
    ARCANE_FATAL("Error checking values nb_error={0}", nb_error);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Sample to read value from a file and create an associated CaseTable.
 *
 * The file should contains 3 values for each time step (so the number of
 * values should be a multiple of 4).
 */
CaseTable* readFileAsCaseTable(IParallelMng* pm, const String& filename, const Int32& ndim)
{
  ITraceMng* tm = pm->traceMng();
  UniqueArray<Byte> bytes;
  bool is_bad = pm->ioMng()->collectiveRead(filename, bytes, false);
  if (is_bad)
    ARCANE_FATAL("Can not read file '{0}'");
  String file_as_str(bytes);
  tm->info() << "FILE=" << file_as_str;
  UniqueArray<Real> file_values;
  is_bad = builtInGetValue(file_values, file_as_str);
  if (is_bad)
    ARCANE_FATAL("Can not read file '{0}' as Array<Real>");
  Int32 nb_value = file_values.size();

  // For each line, the first value is the time step and the 3 following values
  // are the values of the case table. So the total number should be a multiple of 4
  tm->info() << "NB_VALUE=" << file_values.size();
  Int32 nb_func_value = nb_value / (ndim+1);
  tm->info() << "NB_FUNC_VALUE=" << nb_func_value;
  if ((nb_func_value * (ndim+1)) != nb_value)
    ARCANE_FATAL("Bad number of values: {0} should be a multiple of {1}", nb_value, ndim+1);

  CaseFunctionBuildInfo cfbi(tm, "MyTestTable");
  cfbi.m_param_type = ICaseFunction::ParamReal;
  cfbi.m_value_type = ICaseFunction::ValueReal3;

  // Create the associated CaseTable and fill it with the values from the file
  auto table = new CaseTable(cfbi, CaseTable::CurveLinear);
  for (Int32 i = 0; i < nb_func_value; ++i) {
    Int32 index = i * (ndim+1);
    Real func_time = file_values[index];
    Real3 v;
    for (int j = 0; j < ndim; ++j) {
      v[j] = file_values[index + j + 1];
    }
    String func_param = String::fromNumber(func_time);
    String func_value;
    switch (ndim) {
      default:
      case 1: func_value = String::format("{0}", v[0]); break;
      case 2: func_value = String::format("{0} {1}", v[0], v[1]); break;
      case 3: func_value = String::format("{0} {1} {2}", v[0], v[1], v[2]); break;
    }
    table->appendElement(func_param, func_value);
  }
  return table;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
