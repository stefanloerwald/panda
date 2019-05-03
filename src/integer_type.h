
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

namespace panda
{
   enum class IntegerType
   {
      Fixed16,  /// Guaranteed 16bit integer.
      Fixed32,  /// Guaranteed 32bit integer.
      Fixed64,  /// Guaranteed 64bit integer.
      Safe,     /// 64bit integer that throws an exception if an operation is unsafe.
      Variable, /// Arbitrary precision integer type (BigInteger).
      Default   /// Integer type that the system uses as "int".
   };
}

