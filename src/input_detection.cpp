
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "input_detection.h"

#include <cstddef>
#include <cstring>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>

#include "application_name.h"
#include "input_common.h"
#include "input_keywords.h"
#include "position_guarded_file.h"

using namespace panda;

namespace
{
   enum class Keyword
   {
      ConvexHull,
      ConicalHull,
      Inequalities
   };

   bool requiresParameter(char*) noexcept;
   std::set<Keyword> keywords(const PositionGuardedFile&);
   InputOrder inputOrder(char*);
   OperationMode commandLineModeOption(int, char**) noexcept;
}

Filename panda::getFilename(int argc, char** argv)
{
   Filename name;
   for ( int i = 1; i < argc; ++i )
   {
      if ( std::strncmp(argv[i], "--", 2) == 0 )
      {
         continue; // argv[i] is an option of the form "--option=parameter"
      }
      if (std::strncmp(argv[i], "-", 1) == 0 )
      {
         if ( requiresParameter(argv[i]) )
         {
            ++i; // argv[i] is an option of the form "-s <arg>", hence argv[i + 1] is the parameter.
         }
         continue;
      }
      if ( !name.empty() )
      {
         const auto message = std::string("Ambiguity in input: multiple file names provided (this may be an error due to mispelled option, such as \"./") + project::binary_name + " threads=10 file\" instead of \"./" + project::binary_name + " --threads=10 file\").";
         throw std::invalid_argument(message);
      }
      name = argv[i];
   }
   return name;
}

InputOrder panda::getInputOrder(int argc, char** argv)
{
   for ( int i = 1; i < argc; ++i )
   {
      if ( std::strcmp(argv[i], "-s") == 0 )
      {
         if ( i + 1 == argc )
         {
            throw std::invalid_argument("Expected argument to option \"-s\" / \"--sorting=\".");
         }
         return inputOrder(argv[i + 1]);
      }
      else if ( std::strncmp(argv[i], "-s=", 3) == 0 )
      {
         return inputOrder(argv[i] + 3);
      }
      else if ( std::strncmp(argv[i], "--sorting=", 10) == 0 )
      {
         return inputOrder(argv[i] + 10);
      }
      else if ( std::strncmp(argv[i], "-s", 2) == 0 || std::strncmp(argv[i], "--s", 3) == 0 )
      {
         throw std::invalid_argument("Illegal parameter. Did you mean \"-s <order>\" or \"--sorting=<order>\"?");
      }
   }
   return InputOrder::NoSorting;
}

OperationMode panda::detectOperationMode(int argc, char** argv)
{
   const auto cmd_mode = commandLineModeOption(argc, argv);
   if ( cmd_mode != OperationMode::Undecided )
   {
      return cmd_mode;
   }
   const auto filename = getFilename(argc, argv);
   std::ifstream file(filename.c_str());
   const auto words = keywords(file);
   if ( !words.empty() )
   {
      const auto has_conv = words.find(Keyword::ConvexHull) != words.end();
      const auto has_cone = words.find(Keyword::ConicalHull) != words.end();
      const auto has_ieqs = words.find(Keyword::Inequalities) != words.end();
      if ( (has_conv || has_cone) && has_ieqs )
      {
         return OperationMode::Invalid;
      }
      if ( has_ieqs )
      {
         return OperationMode::VertexEnumeration;
      }
      if ( has_cone || has_conv )
      {
         return OperationMode::FacetEnumeration;
      }
      return OperationMode::Invalid;
   }
   return OperationMode::FacetEnumeration;
}

namespace
{
   bool requiresParameter(char* argument) noexcept
   {
      return (std::strcmp(argument, "-c") != 0 && // --check
              std::strcmp(argument, "-f") != 0);  // --filter
   }

   std::set<Keyword> keywords(const PositionGuardedFile& pg)
   {
      std::set<Keyword> words;
      for ( std::string line; std::getline(pg.file, line); )
      {
         line = input::trimWhitespace(line);
         using namespace input::implementation;
         if ( isKeywordConicalHull(line) )
         {
            words.insert(Keyword::ConicalHull);
         }
         else if ( isKeywordReducedConicalHull(line) )
         {
            words.insert(Keyword::ConicalHull);
         }
         else if ( isKeywordConvexHull(line) )
         {
            words.insert(Keyword::ConvexHull);
         }
         else if ( isKeywordReducedConvexHull(line) )
         {
            words.insert(Keyword::ConvexHull);
         }
         else if ( isKeywordInequalities(line) )
         {
            words.insert(Keyword::Inequalities);
         }
         else if ( isKeywordReducedInequalities(line) )
         {
            words.insert(Keyword::Inequalities);
         }
      }
      return words;
   }

   bool ascending(char* argument) noexcept
   {
      return (std::strncmp(argument, "desc", 4) != 0 &&
              std::strncmp(argument, "down", 4) != 0 &&
              std::strncmp(argument, "dec", 3) != 0);
   }

   InputOrder inputOrderLexicographic(char* argument) noexcept
   {
      if ( std::strcmp(argument, "lex") == 0 || std::strcmp(argument, "lexicographic") == 0 )
      {
         return InputOrder::LexicographicAscending;
      }
      const auto offset = (std::strncmp(argument, "lex_", 4) == 0) ? 4 : 14;
      if ( ascending(argument + offset) )
      {
         return InputOrder::LexicographicAscending;
      }
      else
      {
         return InputOrder::LexicographicDescending;
      }
   }

   InputOrder inputOrderNonZeroEntries(char* argument) noexcept
   {
      if ( std::strcmp(argument, "nz") == 0 || std::strcmp(argument, "nonzero") == 0 )
      {
         return InputOrder::NonZeroEntriesAscending;
      }
      const int offset = (std::strncmp(argument, "nz_", 3) == 0) ? 3 : 8;
      if ( ascending(argument + offset) )
      {
         return InputOrder::NonZeroEntriesAscending;
      }
      else
      {
         return InputOrder::NonZeroEntriesDescending;
      }
   }

   InputOrder inputOrder(char* argument)
   {
      if ( std::strncmp(argument, "lex", 3) == 0 )
      {
         return inputOrderLexicographic(argument);
      }
      if ( std::strncmp(argument, "n", 1) == 0 )
      {
         return inputOrderNonZeroEntries(argument);
      }
      if ( std::strcmp(argument, "rev") == 0 || std::strcmp(argument, "reverse") == 0 )
      {
         return InputOrder::Reverse;
      }
      throw std::invalid_argument("Expected an argument to option \"--sorting\".\n");
   }

   OperationMode commandLineModeOption(int argc, char** argv) noexcept
   {
      if ( argc == 1 )
      {
         return OperationMode::Help;
      }
      for ( int i = 1; i < argc; ++i )
      {
         if ( (std::strcmp(argv[i], "-h") == 0 && i + 1 < argc) ||
              std::strncmp(argv[i], "-h=", 3) == 0 ||
              std::strncmp(argv[i], "--help=", 7) == 0 ||
              std::strncmp(argv[i], "--help-command=", 15) == 0 )
         {
            return OperationMode::HelpCommand;
         }
         if ( std::strcmp(argv[i], "?") == 0 ||
              std::strcmp(argv[i], "-h") == 0 ||
              std::strcmp(argv[i], "--h") == 0 ||
              std::strcmp(argv[i], "-help") == 0 ||
              std::strcmp(argv[i], "--help") == 0 )
         {
            return OperationMode::Help;
         }
         if ( std::strcmp(argv[i], "-v") == 0 ||
              std::strcmp(argv[i], "--v") == 0 ||
              std::strcmp(argv[i], "-version") == 0 ||
              std::strcmp(argv[i], "--version") == 0 )
         {
            return OperationMode::Version;
         }
      }
      return OperationMode::Undecided;
   }
}

