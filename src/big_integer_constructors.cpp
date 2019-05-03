
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "big_integer.h"

#include <cassert>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <stdexcept>

using namespace panda;

BigInteger::BigInteger()
:
   sign(Sign::Positive),
   data(1, 0)
{
}

#ifdef INT16_MAX
panda::BigInteger::BigInteger(const int16_t value)
:
   sign((value >= 0) ? Sign::Positive : Sign::Negative),
   data(1, 0)
{
   if ( value >= 0 )
   {
      data[0] = static_cast<DataType>(value);
   }
   else
   {
      auto safe_value = 1 + value;
      safe_value = -safe_value;
      data[0] = static_cast<DataType>(safe_value);
      ++data[0];
   }
}
#endif

#ifdef INT32_MAX
panda::BigInteger::BigInteger(const int32_t value)
:
   sign((value >= 0) ? Sign::Positive : Sign::Negative),
   data(1, 0)
{
   if ( value >= 0 )
   {
      data[0] = static_cast<DataType>(value);
   }
   else
   {
      auto safe_value = 1 + value;
      safe_value = -safe_value;
      data[0] = static_cast<DataType>(safe_value);
      ++data[0];
   }
}
#endif

#ifdef INT64_MAX
panda::BigInteger::BigInteger(const int64_t value)
:
   sign((value >= 0) ? Sign::Positive : Sign::Negative),
   data(1, 0)
{
   if ( value >= 0 )
   {
      data[0] = static_cast<DataType>(value);
   }
   else
   {
      auto safe_value = 1 + value;
      safe_value = -safe_value;
      data[0] = static_cast<DataType>(safe_value);
      ++data[0];
   }
}
#endif

#ifdef UINT16_MAX
panda::BigInteger::BigInteger(const uint16_t value)
:
   sign(Sign::Positive),
   data(1, value)
{
}
#endif

#ifdef UINT32_MAX
panda::BigInteger::BigInteger(const uint32_t value)
:
   sign(Sign::Positive),
   data(1, value)
{
}
#endif

#ifdef UINT64_MAX
panda::BigInteger::BigInteger(const uint64_t value)
:
   sign(Sign::Positive),
   data(1, value)
{
}
#endif

panda::BigInteger::operator int() const
{
   if ( data.size() > 1 )
   {
      throw std::invalid_argument("Number doesn't fit into int.");
   }
   assert( !data.empty() );
   constexpr bool symmetric_int_range = (std::numeric_limits<int>::max() + std::numeric_limits<int>::min() == 0);
   constexpr auto int_max = static_cast<DataType>(std::numeric_limits<int>::max());
   if ( symmetric_int_range )
   {
      if ( data[0] > int_max )
      {
         throw std::invalid_argument("Number doesn't fit into int.");
      }
   }
   else
   {
      if ( sign == Sign::Positive && data[0] > int_max )
      {
         std::stringstream stream;
         stream << "Number doesn't fit into int. Magnitude is " << data[0] << " and sign is positive.";
         throw std::invalid_argument(stream.str());
      }
      else if ( sign == Sign::Negative && data[0] - 1 > int_max )
      {
         std::stringstream stream;
         stream << "Number doesn't fit into int. Magnitude is " << data[0] << " and sign is negative.";
         throw std::invalid_argument(stream.str());
      }
   }
   if ( sign == Sign::Positive )
   {
      return static_cast<int>(data[0]);
   }
   else
   {
      return -static_cast<int>(data[0]);
   }
}

