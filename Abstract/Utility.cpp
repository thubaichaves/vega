/*
 * Copyright (C) Alneos, s. a r. l. (contact@alneos.fr)
 * Released under the GNU General Public License
 *
 * Utility.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: devel
 */

#include "Utility.h"
#include "Value.h"
#include <math.h>       /* pow */
#include <boost/numeric/ublas/vector_expression.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <cmath>

namespace ublas = boost::numeric::ublas;

namespace vega {

using namespace std;

bool InvertMatrix(const ublas::matrix<double>& input, ublas::matrix<double>& inverse)
{
   typedef ublas::permutation_matrix<std::size_t> pmatrix;

   // create a working copy of the input
   ublas::matrix<double> A(input);

   // create a permutation matrix for the LU-factorization
   pmatrix pm(A.size1());

   // perform LU-factorization
   int res = boost::numeric_cast<int>(lu_factorize(A, pm));
   if (res != 0)
       return false;

   // create identity matrix of "inverse"
   inverse.assign(ublas::identity_matrix<double> (A.size1()));

   // backsubstitute to get the inverse
   ublas::lu_substitute(A, pm, inverse);
   return true;
}

#if defined(__linux__)

// Call this function to get a backtrace.
void backtrace() {
  unw_cursor_t cursor;
  unw_context_t context;

  // Initialize cursor to current frame for local unwinding.
  unw_getcontext(&context);
  unw_init_local(&cursor, &context);

  // Unwind frames one by one, going up the frame stack.
  while (unw_step(&cursor) > 0) {
    unw_word_t offset, pc;
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    if (pc == 0) {
      break;
    }
    printf("0x%lx:", pc);

    char sym[256];
    if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
      printf(" (%s+0x%lx)\n", sym, offset);
    } else {
      printf(" -- error: unable to obtain symbol name for this frame\n");
    }
  }
}
#endif

void handler(int sig) {
    // print out all the frames to stderr
    std::cerr << "Error: signal " << sig << std::endl;
    backtrace();
    exit(1);
}

//__________ ValueOrReference

} /* namespace vega */
