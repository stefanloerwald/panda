
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

template <typename ExceptionType>
void panda::input::errorWithLineInformation(const std::string& message, const std::string& id)
{
   if ( id.empty() )
   {
      throw ExceptionType(message + ".");
   }
   else
   {
      throw ExceptionType(message + " (Row \"" + id + "\").");
   }
}

