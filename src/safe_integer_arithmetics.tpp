
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

panda::SafeInteger& panda::SafeInteger::operator*=(const SafeInteger& second)
{
   if ( data == 0 || second.data == 0 )
   {
      data = 0;
      return *this;
   }
   if ( second.data == -1 ) // sign change is dangerous
   {
      *this = -*this;
      return *this;
   }
   const auto limit_result = ((data > 0) == (second.data > 0)) ? DataLimits::max() : DataLimits::min();
   const auto limit_this = limit_result / second.data;
   if ( (data > 0 && limit_this < data) || (data < 0 && limit_this > data) )
   {
      error("Multiplication did overflow.", data, second.data);
   }
   data *= second.data;
   return *this;
}

panda::SafeInteger& panda::SafeInteger::operator/=(const SafeInteger& second)
{
   if ( second.data == 0 )
   {
      error("Dividing by zero.", data, second.data);
   }
   if ( second.data == -1 )
   {
      *this = -*this;
      return *this;
   }
   data /= second.data; // cannot overflow, as either it's division by 1 or the magnitude will decrease.
   return *this;
}

panda::SafeInteger& panda::SafeInteger::operator+=(const SafeInteger& second)
{
   if ( (data > 0 && second.data > 0 && second.data > DataLimits::max() - data) ||
        (data < 0 && second.data < 0 && second.data < DataLimits::min() - data) )
   {
      error("Addition did overflow.", data, second.data);
   }
   data += second.data;
   return *this;
}

panda::SafeInteger& panda::SafeInteger::operator-=(const SafeInteger& second)
{
   if ( second.data < 0 && data > DataLimits::max() + second.data )
   {  // A <= A - (-B), therefore MIN <= A - (-B) <= MAX, if A <= MAX + (-B)
      error("Subtraction did overflow.", data, second.data);
   }
   if ( second.data > 0 && data < DataLimits::min() + second.data )
   {  // A - B <= A, therefore MIN <= A - B <= MAX, if MIN + B <= A
      error("Subtraction did overflow.", data, second.data);
   }
   data -= second.data;
   return *this;
}

panda::SafeInteger& panda::SafeInteger::operator%=(const SafeInteger& second)
{
   if ( second.data == 0 )
   {
      error("Modulo zero is undefined.", data, second.data);
   }
   if ( second.data < 0 )
   {
      error("Modulo a negative number is non-sense.", data, second.data);
   }
   data %= second.data; // cannot overflow
   if ( data < 0 )
   {
      data += second.data;
   }
   return *this;
}

panda::SafeInteger panda::SafeInteger::operator*(const SafeInteger& second) const
{
   auto copy = *this;
   copy *= second;
   return copy;
}

panda::SafeInteger panda::SafeInteger::operator/(const SafeInteger& second) const
{
   auto copy = *this;
   copy /= second;
   return copy;
}

panda::SafeInteger panda::SafeInteger::operator+(const SafeInteger& second) const
{
   auto copy = *this;
   copy += second;
   return copy;
}

panda::SafeInteger panda::SafeInteger::operator-(const SafeInteger& second) const
{
   auto copy = *this;
   copy -= second;
   return copy;
}

panda::SafeInteger panda::SafeInteger::operator%(const SafeInteger& second) const
{
   auto copy = *this;
   copy %= second;
   return copy;
}

