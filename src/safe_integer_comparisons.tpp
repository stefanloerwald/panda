
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

bool panda::SafeInteger::operator==(const int second) const noexcept
{
   return data == second;
}

bool panda::SafeInteger::operator!=(const int second) const noexcept
{
   return !(*this == second);
}

bool panda::SafeInteger::operator<(const int second) const noexcept
{
   return data < second;
}

bool panda::SafeInteger::operator>(const int second) const noexcept
{
   return data > second;
}

bool panda::SafeInteger::operator<=(const int second) const noexcept
{
   return !(*this > second);
}

bool panda::SafeInteger::operator>=(const int second) const noexcept
{
   return !(*this < second);
}

bool panda::SafeInteger::operator==(const SafeInteger& second) const noexcept
{
   return data == second.data;
}

bool panda::SafeInteger::operator!=(const SafeInteger& second) const noexcept
{
   return !(*this == second);
}

bool panda::SafeInteger::operator<(const SafeInteger& second) const noexcept
{
   return data < second.data;
}

bool panda::SafeInteger::operator>(const SafeInteger& second) const noexcept
{
   return second < *this;
}

bool panda::SafeInteger::operator<=(const SafeInteger& second) const noexcept
{
   return !(*this > second);
}

bool panda::SafeInteger::operator>=(const SafeInteger& second) const noexcept
{
   return !(*this < second);
}

