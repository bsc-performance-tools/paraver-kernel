/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

#pragma once

#include <array>
#include <stddef.h>

/******************************************************************************
******************   cartesian_product      ***********************************
******************************************************************************/
template<typename T, class InputIt, class OutputIt, size_t sizeFactors, typename... Targs>
constexpr void cartesian_product_impl( OutputIt& product,
                                       size_t indexFactor,
                                       std::array<T, sizeFactors>& currentElement,
                                       InputIt beginInput, InputIt endInput )
{
  for( ; beginInput != endInput; ++beginInput )
  {
    currentElement[ indexFactor ] = *beginInput;
    *product++ = currentElement;
  }
}

template<typename T, class InputIt, class OutputIt, size_t sizeFactors, typename... Targs>
constexpr void cartesian_product_impl( OutputIt& product,
                                       size_t indexFactor,
                                       std::array<T, sizeFactors>& currentElement,
                                       InputIt beginInput, InputIt endInput, 
                                       Targs... args )
{
  for( ; beginInput != endInput; ++beginInput )
  {
    currentElement[ indexFactor ] = *beginInput;
    cartesian_product_impl( product, indexFactor + 1, currentElement, args... );
  }
}

// Makes cartesian product of containers 'args' using their begin/end iterators
// over 'product' iterator.
// 'product' must be a container of std::array<T, numContainers> 
// where 'numContainers' is the number of containers to combine.
// Example of use:
//   constexpr int SIZE_ARR1 = 3;
//   constexpr int SIZE_ARR2 = 3;
//   constexpr int SIZE_ARR3 = 6;

//   std::array<char, SIZE_ARR1> arr1{ 'a', 'b', 'c' };
//   std::array<char, SIZE_ARR2> arr2{ 'x', 'y', 'z' };
//   std::array<char, SIZE_ARR3> arr3{ 'a', 'b', 'c', 'd', 'e', 'f' };

//   std::array<std::array<char, 3>, SIZE_ARR1 * SIZE_ARR2 * SIZE_ARR3> product_char_array;

//   cartesian_product( product_char_array.begin(), arr1.begin(), arr1.end(), arr2.begin(), arr2.end(), arr3.begin(), arr3.end() );

template<class InputIt, class OutputIt, typename... Targs>
constexpr void cartesian_product( OutputIt product,
                                  InputIt beginInput, InputIt endInput, 
                                  Targs... args )
{
  using T = typename std::iterator_traits<InputIt>::value_type;
  std::array<T, ( sizeof...( Targs ) / 2 ) + 1 > currentElement;
  size_t indexFactor = 0;

  for( ; beginInput != endInput; ++beginInput )
  {
    currentElement[ indexFactor ] = *beginInput;
    cartesian_product_impl( product, indexFactor + 1, currentElement, args... );
  }
}
