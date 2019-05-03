
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "help.h"

#include <cassert>
#include <cstring>
#include <iostream>

#include "application_name.h"
#include "git_revision.h"

using namespace panda;

namespace
{
   int printHelpCommand(const std::string&);
}

int panda::help::command(int argc, char** argv)
{
   assert( argc > 1 && argv != nullptr );
   int i = 1;
   while ( i < argc &&
           std::strcmp(argv[i], "-h") != 0 &&
           std::strncmp(argv[i], "-h=", 3) != 0 &&
           std::strncmp(argv[i], "--help=", 7) != 0 &&
           std::strncmp(argv[i], "--help-command=", 15) != 0 )
   {
      ++i;
   }
   if ( i == argc || (std::strcmp(argv[i], "-h") == 0 && i + 1 == argc) )
   {
      std::cerr << "No parameter to option \"--help-command\" provided.\n";
      return 1;
   }
   std::string arg;
   if ( std::strcmp(argv[i], "-h") == 0 )
   {
      arg = argv[i + 1];
   }
   else if ( std::strncmp(argv[i], "-h=", 3) == 0 )
   {
      arg = argv[i] + 3;
   }
   else if ( std::strncmp(argv[i], "--help=", 7) == 0 )
   {
      arg = argv[i] + 7;
   }
   else if ( std::strncmp(argv[i], "--help-command=", 15) == 0 )
   {
      arg = argv[i] + 15;
   }
   return printHelpCommand(arg);
}

namespace
{
   void printHelpCommandCheck()
   {
      std::cout << "By default, " << project::application_acronym << " assumes the user input to be correct.\n"
                << "While consistency is always checked (e.g. equal dimension of every vertex), it is considered very expensive to check if e.g. maps are actually bijections on the set of vertices, rays and inequalities.\n"
                << "If in doubt (e.g. if you get an unexpectedly huge amount of data), you may force " << project::application_acronym << " to check input data beforehand.\n"
                << "If applicable, " << project::application_acronym << " will check the following properties:\n"
                << "\tEach map is a bijection on the set of vertices.\n"
                << "\tEach map is a bijection on the set of rays.\n"
                << "\tEach map is a bijection on the set of inequalities.\n"
                << "\tEach given inequality is a facet-defining inequality.\n"
                << '\n'
                << "To enable checked input, add the parameter \"-c\" / \"--check\" to the command line.\n"
                << "Example usage:\n"
                << "t./" << project::binary_name << " myproblem -c\n"
                << "t./" << project::binary_name << " myproblem --checked\n"
                << "t./" << project::binary_name << " myproblem -k my_known_facets --checked\n";
   }

   void printHelpCommandHelp()
   {
      std::cout << "To get an overview on available commands, call ./" << project::binary_name << " --help\n"
                << "? / -h / --h / -help do the same\n"
                << "To get detailed help on a specific command (as you're doing right now), call ./" << project::binary_name << " -h <command> or ./" << project::binary_name << " --help=<command> or ./" << project::binary_name << " --help-command=<command>\n"
                << "Example usage:\n"
                << "\t./" << project::binary_name << " --help=sorting\n"
                << "\t./" << project::binary_name << " -h integer-type\n"
                << "\t./" << project::binary_name << " --help-command=-t\n"
                << "\t./" << project::binary_name << " --help-command=--threads\n";
   }

   void printHelpCommandIntegerType()
   {
      std::cout << "Fixed width integer types may be insufficient for large problems.\n"
                << "As overflows may occur silently, in general results obtained with a fixed width integer type may not be trusted.\n"
                << "To prove correctness, either an arbitrary precision integer type must be used, or it must be asserted that no operation on a fixed width integer type may result in an overflow.\n\n"
                << "By default " << project::binary_name << " uses a fixed width integer type.\n"
                << "You may select one of the following options:\n"
                << "\t16, 32, 64, safe, inf.\n"
                << "As the names suggest, 16, 32 and 64 guarantee fixed with integer arithmetic with 16bit, 32bit and 64bit types respectively.\n"
                << "\"inf\" will force " << project::binary_name << " to perform every operation with an arbitrary precision integer type. Attention: this will drastically slow down any calculations!\n"
                << "\"safe\" is a compromise between performance of fixed width integer type and the correctness guarantee. If a calculation results in an overflow, the process is aborted with an error message. All results so far did not suffer from that overflow, so they may be trusted.\n\n"
                << "To select one of the integer types, use the command line option \"-i\" / \"--integer-type=\".\n"
                << "Example usage:\n"
                << "\t./" << project::binary_name << " myproblem -i safe\n"
                << "\t./" << project::binary_name << " myproblem -i 64\n"
                << "\t./" << project::binary_name << " myproblem --integer-type=safe\n"
                << "\t./" << project::binary_name << " myproblem --integer-type=64\n";
   }

   void printHelpCommandKnownData()
   {
      std::cout << "Adjacency decomposition needs at least one initial facet / vertex to calculate neighbors of.\n"
                << "Without prior knowledge, " << project::application_acronym << " must calculate some initial facets / vertices.\n"
                << "Depending on the problem, this may take a while. If you do however have some prior knowledge, you may add this by creating a new file and passing it with the \"--known-data\" command.\n"
                << "Such a file is expected to have the following format:\n\n"
                << "\tInequalities\n"
                << "\t2x +y -3z <= 1\n\n"
                << "or, if used for vertex enumeration:\n\n"
                << "\tVertices\n"
                << "\t1 0 0\n"
                << "\t0 1 0\n"
                << "\tRays\n"
                << "\t0 0 1\n\n"
                << "There may be arbitrarily many input rows, as long as there is at most one data row per line.\n"
                << "You may inform " << project::application_acronym << " of this prior knowledge by using the \"-k\" / \"--known-data=\" / \"--known-facets=\" / \"--known-vertices=\" option.\n"
                << "Example usage:\n"
                << "\t./" << project::binary_name << " myproblem -k my_known_inequalities\n"
                << "\t./" << project::binary_name << " myproblem --known-data=my_known_vertices\n";
   }

   void printHelpCommandMethod()
   {
      std::cout << "There are two methods implemented in " << project::application_acronym << " to transform representations of polytopes: adjacency decomposition (AD) and double description (DD).\n"
                << "By default, " << project::application_acronym << " uses AD to calculate the outer description of polytopes.\n"
                << "However, for non-symmetrical polytopes, DD might be advantageous.\n"
                << "You may select the chosen method by passing the \"-m\" / \"--method=\" command.\n"
                << "Example usage:\n"
                << "\t./" << project::binary_name << " myproblem -m ad\n"
                << "\t./" << project::binary_name << " myproblem -m dd\n"
                << "\t./" << project::binary_name << " myproblem -m double-description\n"
                << "\t./" << project::binary_name << " myproblem --method=double-description\n"
                << "\t./" << project::binary_name << " myproblem --method=ad\n";
   }

   void printHelpCommandSorting()
   {
      std::cout << "An important implementation detail of " << project::application_acronym << " is the usage of double description method (either explicitely wanted by the user, or implicitely used in adjacency decomposition).\n"
                << "The performance of double description method is highly dependent on input order.\n"
                << "For altering the input order without touching the data files again, the user may specify an ordering as a command line argument (\"-s\" / \"--sorting=\").\n"
                << "Valid parameters are:\n"
                << "\tlexicographic_ascending / lex_asc / lex_up: as in a dictionary\n"
                << "\tlexicographic_descending / lex_desc / lex_down: reverse dictionary\n"
                << "\tnonzero_ascending / nz_asc / nz_up: number of non-zero entries per row\n"
                << "\tnonzero_descending / nz_desc / nz_down: reversed nonzero_ascending\n"
                << "\treverse / rev: as if the file was read from back to front\n"
                << "Example usage:\n"
                << "\t./" << project::binary_name << " myproblem -s lex_asc\n"
                << "\t./" << project::binary_name << " myproblem -s reverse\n"
                << "\t./" << project::binary_name << " myproblem --sorting=lex_asc\n"
                << "\t./" << project::binary_name << " myproblem --sorting=reverse\n";
   }

   void printHelpCommandThreads()
   {
      std::cout << "Adjacency decomposition (the default algorithm of " << project::application_acronym << ") is a parallel algorithm.\n"
                << "By default, it uses as many cores your system provides.\n"
                << "However, you may still specify a lower or higher number of threads, e.g. to allow other jobs to work simultaneously.\n"
                << "Use the \"-t\" / \"--threads=\" command. Only positive integral parameters are allowed.\n"
                << "Example usage:\n"
                << "\t./" << project::binary_name << " myproblem -t 1\n"
                << "\t./" << project::binary_name << " myproblem -t 10\n"
                << "\t./" << project::binary_name << " myproblem --threads=10\n";
   }

   void printHelpCommandVersion()
   {
      std::cout << "For bug reports, please include the version information.\n"
                << "Call ./" << project::binary_name << " -v   or   ./" << project::binary_name << " --version\n"
                << "The output will look like this:\n"
                << project::application_acronym << " -- " << project::application_name << " (revision " << project::git_revision << ")\n"
                << "The string \"" << project::git_revision << "\" is the version information.\n";

   }

   int printHelpCommand(const std::string& command)
   {
      if ( command == "c" || command == "-c" || command == "check" || command == "--check" )
      {
         printHelpCommandCheck();
      }
      else if ( command == "h" || command == "-h" || command == "--h" || command == "help" || command == "-help" || command == "--help" || command == "?" )
      {
         printHelpCommandHelp();
      }
      else if ( command == "i" || command == "-i" || command == "integer-type" || command == "--integer-type" )
      {
         printHelpCommandIntegerType();
      }
      else if ( command == "k" || command == "-k" || command == "known_facets" || command == "--known_facets" || command == "known_vertices" || command == "--known_vertices" || command == "known_data" || command == "--known_data" )
      {
         printHelpCommandKnownData();
      }
      else if ( command == "m" || command == "-m" || command == "method" || command == "--method" )
      {
         printHelpCommandMethod();
      }
      else if ( command == "s" || command == "-s" || command == "sorting" || command == "--sorting" )
      {
         printHelpCommandSorting();
      }
      else if ( command == "t" || command == "-t" || command == "threads" || command == "--threads" )
      {
         printHelpCommandThreads();
      }
      else if ( command == "v" || command == "-v" || command == "version" || command == "-version" || command == "--version" )
      {
         printHelpCommandVersion();
      }
      else
      {
         std::cerr << "Command not found.\n";
         return 1;
      }
      return 0;
   }
}

