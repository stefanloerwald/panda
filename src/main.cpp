
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include <iostream>

#include "application_name.h"
#include "git_revision.h"
#include "help.h"
#include "input_detection.h"
#include "method_facet_enumeration.h"
#include "method_vertex_enumeration.h"

using namespace panda;

namespace
{
   void printHelp() noexcept;
   void printVersion() noexcept;
}

int main(int argc, char** argv)
{
   const auto mode = detectOperationMode(argc, argv);
   switch ( mode )
   {
      case OperationMode::FacetEnumeration:
      {
         return method::facetEnumeration(argc, argv);
      }
      case OperationMode::VertexEnumeration:
      {
         return method::vertexEnumeration(argc, argv);
      }
      case OperationMode::HelpCommand:
      {
         return help::command(argc, argv);
      }
      case OperationMode::Help:
      {
         printHelp();
         return 0;
      }
      case OperationMode::Version:
      {
         printVersion();
         return 0;
      }
      case OperationMode::Undecided:
      case OperationMode::Invalid:
      {
         std::cerr << "The operation mode couldn't be deduced. Check the format of input file(s).\n";
         return 1;
      }
   }
}

namespace
{
   void printHelp() noexcept
   {
      printVersion();
      std::cerr << "Commands:\n"
                << "\t-i <n>\n\t--integer-type=<n>\n"
                << "\t\twith <n> being \"16\", \"32\", \"64\", \"safe\" or \"inf\".\n"
                << '\n'
                << "\t-k <path/to/file>\n\t--known-data=<path/to/file>\n\t--known-facets=<path/to/file>\n\t--known-vertices=<path/to/file>\n"
                << "\t\toptional way to provide initial data to the adjacency decomposition.\n"
                << '\n'
                << "\t-m <method>\n\t--method=<method>\n"
                << "\t\twith <method> being either \"adjacency-decomposition\" (\"ad\", default)\n"
                << "\t\t                        or \"double-description\" (\"dd\")\n"
                << '\n'
                << "\t-s <arg>\n\t--sorting=<arg>\n"
                << "\t\twith <arg> being \"lex_asc\" / \"lexicographic_ascending\"\n"
                << "\t\t              or \"lex_desc\" / \"lexicographic_descending\"\n"
                << "\t\t              or \"nz_asc\" / \"nonzero_ascending\"\n"
                << "\t\t              or \"nz_desc\" / \"nonzero_descending\"\n"
                << "\t\t              or \"rev\" / \"reverse\".\n"
                << '\n'
                << "\t-c\n\t--check\n"
                << "\t\tenables check if input is valid (e.g. checks if maps are actually bijections).\n"
                << '\n'
                << "\t-t <n>\n\t--threads=<n>\n"
                << "\t\twith <n> being a natural number greater than zero.\n"
                << '\n'
                << "\t-h <arg>\n\t--help=<arg>\n\t--help-command=<arg>\n"
                << "\t\twith <arg> being a valid command (i.e. one occuring in this list).\n"
                << '\n'
                << "\t-h, --h, -help, --help or ? for printing this.\n"
                << '\n'
                << "\t-v, --v, -version or --version for printing just the version number.\n"
                << '\n'
                << "Note: any other argument is interpreted as file name.\n";
   }

   void printVersion() noexcept
   {
      std::cerr << project::application_acronym << " -- "
                << project::application_name << " (revision "
                << project::git_revision << ")\n";
   }
}

