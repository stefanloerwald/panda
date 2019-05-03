
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#ifdef INT16_MAX
panda::SafeInteger::SafeInteger(const int16_t value)
:
   data()
{
   if ( value > DataLimits::max() || value < DataLimits::min() )
   {
      error("The provided value of type int16_t cannot be converted into a SafeInteger.", value);
   }
   data = value;
}
#endif

#ifdef INT32_MAX
panda::SafeInteger::SafeInteger(const int32_t value)
:
   data()
{
   if ( value > DataLimits::max() || value < DataLimits::min() )
   {
      error("The provided value of type int32_t cannot be converted into a SafeInteger.", value);
   }
   data = value;
}
#endif

#ifdef INT64_MAX
panda::SafeInteger::SafeInteger(const int64_t value)
:
   data()
{
   if ( value > DataLimits::max() || value < DataLimits::min() )
   {
      error("The provided value of type int64_t cannot be converted into a SafeInteger.", value);
   }
   data = value;
}
#endif

#ifdef UINT16_MAX
panda::SafeInteger::SafeInteger(const uint16_t value)
:
   data()
{
   if ( static_cast<uint64_t>(value) > static_cast<uint64_t>(DataLimits::max()) )
   {
      error("The provided value of type uint16_t cannot be converted into a SafeInteger.", value);
   }
   data = value;
}
#endif

#ifdef UINT32_MAX
panda::SafeInteger::SafeInteger(const uint32_t value)
:
   data()
{
   if ( static_cast<uint64_t>(value) > static_cast<uint64_t>(DataLimits::max()) )
   {
      error("The provided value of type uint32_t cannot be converted into a SafeInteger.", value);
   }
   data = value;
}
#endif

#ifdef UINT64_MAX
panda::SafeInteger::SafeInteger(const uint64_t value)
:
   data()
{
   if ( static_cast<uint64_t>(value) > static_cast<uint64_t>(DataLimits::max()) )
   {
      error("The provided value of type uint64_t cannot be converted into a SafeInteger.", value);
   }
   data = static_cast<DataType>(value);
}
#endif

panda::SafeInteger::operator int() const
{
   if ( data > std::numeric_limits<int>::max() || data < std::numeric_limits<int>::min() )
   {
      error("The value of type SafeInteger cannot be safely converted into an int.", data);
   }
   return static_cast<int>(data);
}
#pragma clang diagnostic pop

