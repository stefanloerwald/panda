
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "input_common.h"

#include <sstream>
#include <stdexcept>

using namespace panda;

namespace
{
   void is_relation();
   void get_relation_operator();
   void is_identifier();
   void get_identifier();
   void is_whitespace();
   void skip_whitespace();
   void trim_whitespace();
   void errors();
   void normalization();
   void string_to_int();
}

int main()
try
{
   is_relation();
   get_relation_operator();
   is_identifier();
   get_identifier();
   is_whitespace();
   skip_whitespace();
   trim_whitespace();
   errors();
   normalization();
   string_to_int();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void is_relation()
   {
      for ( const auto seq : {"< ", "<\t", "<=", "= ", "==", "> ", ">\t", ">="} )
      {
         std::istringstream stream(seq);
         const auto pos = stream.tellg();
         ASSERT_NOTHROW(input::isRelation(stream), "check for isRelation may not throw.");
         ASSERT(input::isRelation(stream), "Didn't recognize relation.");
         ASSERT(stream.tellg() == pos, "isRelation modified position.");
      }
      for ( const auto seq : {"<", "=", ">", "?", "<?", ">?", "=?", ""} )
      {
         std::istringstream stream(seq);
         const auto pos = stream.tellg();
         ASSERT_NOTHROW(!input::isRelation(stream), "check for isRelation may not throw.");
         ASSERT(!input::isRelation(stream), "Falsely recognized relation.");
         ASSERT(stream.tellg() == pos, "isRelation modified position.");
      }
   }

   void get_relation_operator()
   {
      using input::RelationOperator;
      for ( const auto pair : {std::make_pair("<=", RelationOperator::LessEqual),
                               std::make_pair("= ", RelationOperator::Equal),
                               std::make_pair("==", RelationOperator::Equal),
                               std::make_pair(">=", RelationOperator::GreaterEqual)} )
      {
         const auto seq = pair.first;
         std::istringstream stream(seq);
         ASSERT_NOTHROW(input::relationOperator(stream), "Didn't recognize relation.");
      }
      for ( const auto pair : {std::make_pair("<=", RelationOperator::LessEqual),
                               std::make_pair("= ", RelationOperator::Equal),
                               std::make_pair("==", RelationOperator::Equal),
                               std::make_pair(">=", RelationOperator::GreaterEqual)} )
      {
         const auto seq = pair.first;
         const auto rel = pair.second;
         std::istringstream stream(seq);
         ASSERT(input::relationOperator(stream) == rel, "Didn't recognize relation.");
      }
      for ( const auto seq : {"<", "=", ">", "?", "<?", ">?", "=?", ""} )
      {
         std::istringstream stream(seq);
         ASSERT_EXCEPTION(input::relationOperator(stream), std::invalid_argument, "Parsed invalid relation.");
      }
   }

   void is_identifier()
   {
      for ( const auto seq : {"("} )
      {
         std::istringstream stream(seq);
         const auto pos = stream.tellg();
         ASSERT_NOTHROW(input::isIdentifier(stream), "check for isIdentifier may not throw.");
         ASSERT(input::isIdentifier(stream), "Didn't recognize identifier.");
         ASSERT(stream.tellg() == pos, "isIdentifier modified position.");
      }
      for ( const auto seq : {"", "?"} )
      {
         std::istringstream stream(seq);
         const auto pos = stream.tellg();
         ASSERT_NOTHROW(!input::isIdentifier(stream), "check for isIdentifier may not throw.");
         ASSERT(!input::isIdentifier(stream), "Falsely recognized identifier.");
         ASSERT(stream.tellg() == pos, "isIdentifier modified position.");
      }
   }

   void get_identifier()
   {
      for ( const auto string : {"()",
                                 "( 123)",
                                 "( 123 )",
                                 "( 123 ) ",
                                 "( 1 2 3)",
                                 "( 1 2 3 )",
                                 "( 1 2 3 ) ",
                                 "( \t123)",
                                 "( \t123 \t)",
                                 "( \t123 \t) ",
                                 "( \t1 2\t 3)",
                                 "( \t1 \t2 3\t )",
                                 "( \t1 \t2 3\t ) "} )
      {
         std::stringstream stream(string);
         ASSERT_NOTHROW(input::identifier(stream), "Extracting identifier may not throw.");
      }
      for ( const auto pair : {std::make_pair("()", ""),
                               std::make_pair("( 123)", "123"),
                               std::make_pair("( 123 )", "123"),
                               std::make_pair("( 123 ) ", "123"),
                               std::make_pair("( 1 2 3)", "1 2 3"),
                               std::make_pair("( 1 2 3 )", "1 2 3"),
                               std::make_pair("( 1 2 3 ) ", "1 2 3"),
                               std::make_pair("( \t123)", "123"),
                               std::make_pair("( \t123 \t)", "123"),
                               std::make_pair("( \t123 \t) ", "123"),
                               std::make_pair("( \t1 2\t 3)", "1 2\t 3"),
                               std::make_pair("( \t1 \t2 3\t )", "1 \t2 3"),
                               std::make_pair("( \t1 \t2 3\t ) ", "1 \t2 3")} )
      {
         std::stringstream stream(pair.first);
         ASSERT(input::identifier(stream) == pair.second, "Data mismatch.");
      }
   }

   void is_whitespace()
   {
      for ( const auto seq : {" ", "\t"} )
      {
         std::istringstream stream(seq);
         const auto pos = stream.tellg();
         ASSERT_NOTHROW(input::isWhitespace(stream), "check for isWhitespace may not throw.");
         ASSERT(input::isWhitespace(stream), "Didn't recognize whitespace.");
         ASSERT(stream.tellg() == pos, "isWhitespace modified position.");
      }
      for ( const auto seq : {"?", "", "\n"} )
      {
         std::istringstream stream(seq);
         const auto pos = stream.tellg();
         ASSERT_NOTHROW(!input::isWhitespace(stream), "check for isWhitespace may not throw.");
         ASSERT(!input::isWhitespace(stream), "Falsely recognized whitespace.");
         ASSERT(stream.tellg() == pos, "isWhitespace modified position.");
      }
   }

   void skip_whitespace()
   {
      for ( const auto seq : {" \t \t\t ?", "?"} )
      {
         std::istringstream stream(seq);
         input::skipWhitespace(stream);
         std::string token;
         ASSERT((stream >> token) && token == "?", "whitespace is not skipped correctly.");
      }
      for ( const auto seq : {" ", "\t", " \t", "", " \t \t\t ?", "?"} )
      {
         std::istringstream stream(seq);
         ASSERT_NOTHROW(input::skipWhitespace(stream), "whitespace skipping may not throw.");
      }
   }

   void trim_whitespace()
   {
      using namespace input;
      ASSERT(trimWhitespace(" \t ") == "", "incorrectly trimmed.");
      ASSERT(trimWhitespace(" a ") == "a", "incorrectly trimmed.");
      ASSERT(trimWhitespace(" a\t ") == "a", "incorrectly trimmed.");
      ASSERT(trimWhitespace("a\t ") == "a", "incorrectly trimmed.");
      ASSERT(trimWhitespace("a") == "a", "incorrectly trimmed.");
      ASSERT(trimWhitespace(" a b ") == "a b", "incorrectly trimmed.");
      ASSERT(trimWhitespace(" a b c ") == "a b c", "incorrectly trimmed.");
      ASSERT(trimWhitespace(" a b c     ") == "a b c", "incorrectly trimmed.");
      ASSERT(trimWhitespace(" a b c") == "a b c", "incorrectly trimmed.");
      ASSERT(trimWhitespace("a b c") == "a b c", "incorrectly trimmed.");
      ASSERT(trimWhitespace("no    trailing") == "no    trailing", "incorrectly trimmed.");
      ASSERT(trimWhitespace("     ") == "", "incorrectly trimmed.");
   }

   void errors()
   {
      using input::errorWithLineInformation;
      using except = std::invalid_argument;
      ASSERT_EXCEPTION(errorWithLineInformation<except>("", ""), except, "Exception type must match.");
      ASSERT_EXCEPTION(errorWithLineInformation<except>("", "1"), except, "Exception type must match.");
      ASSERT_EXCEPTION(errorWithLineInformation<except>("1", ""), except, "Exception type must match.");
      ASSERT_EXCEPTION(errorWithLineInformation<except>("1", "1"), except, "Exception type must match.");
   }

   void normalization()
   {
      Row<int> r{1, 2, 3, 4};
      using input::normalize;
      using input::RelationOperator;
      ASSERT((normalize(r, RelationOperator::LessEqual) == Row<int>{1, 2, 3, 4}), "Normalization failed.");
      ASSERT((normalize(r, RelationOperator::Equal) == Row<int>{1, 2, 3, 4}), "Normalization failed.");
      ASSERT((normalize(r, RelationOperator::GreaterEqual) == Row<int>{-1, -2, -3, -4}), "Normalization failed.");
   }

   void string_to_int()
   {
      using input::toInt;
      ASSERT(toInt("") == 1, "The empty string represents a factor of 1.");
      ASSERT(toInt("+") == 1, "Data mismatch.");
      ASSERT(toInt("-") == -1, "Data mismatch.");
      ASSERT(toInt("10") == 10, "Data mismatch.");
      ASSERT(toInt("+10") == 10, "Data mismatch.");
      ASSERT(toInt("-10") == -10, "Data mismatch.");
      ASSERT_ANY_EXCEPTION(toInt("a"), "Invalid data must result in an exception.");
   }
}

