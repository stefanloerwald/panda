
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include <limits>
#include <stdexcept>
#include <string>

namespace panda
{
   namespace
   {
      using DataLimits = std::numeric_limits<SafeInteger::DataType>;
      template <typename Integer>
      void error(const std::string&, const Integer);
      void error(const std::string&, const SafeInteger::DataType, const SafeInteger::DataType);
   }
}

#include "safe_integer_arithmetics.tpp"
#include "safe_integer_comparisons.tpp"
#include "safe_integer_constructors.tpp"

panda::SafeInteger panda::SafeInteger::operator-() const
{
   constexpr static bool symmetric_range = (DataLimits::min() + DataLimits::max() == 0);
   if ( !symmetric_range && data == DataLimits::min() )
   {
      error("The value cannot be negated.", data);
   }
   SafeInteger result = *this;
   result.data *= -1;
   return result;
}

panda::SafeInteger panda::abs(SafeInteger n)
{
   return (n < 0) ? -n : n;
}

namespace panda
{
   namespace
   {
      template <typename Integer>
      void error(const std::string& reason, const Integer a)
      {
         std::string message = "Unsafe integer operation: " + reason;
         message += " Argument: " + std::to_string(a);
         throw std::invalid_argument(message);
      }
      void error(const std::string& reason, const SafeInteger::DataType a, const SafeInteger::DataType b)
      {
         std::string message = "Unsafe integer operation: " + reason;
         message += " Arguments: " + std::to_string(a) + ", " + std::to_string(b);
         throw std::invalid_argument(message);
      }
   }
}

