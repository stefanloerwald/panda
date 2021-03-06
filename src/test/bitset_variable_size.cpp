
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#include "testing_gear.h"

#include "bitset_variable_size.h"

using namespace panda;

namespace
{
   void construction();
   void equality();
   void merge();
   void intersect();
   void count();
}

int main()
try
{
   construction();
   equality();
   merge();
   intersect();
   count();
}
catch ( const TestingGearException& e )
{
   std::cerr << e.what() << "\n";
   return 1;
}

namespace
{
   void construction()
   {
      ASSERT_NOTHROW(BitsetVariableSize(10), "Reasonably small bitset");
      ASSERT_NOTHROW(BitsetVariableSize(1000), "Reasonably small bitset");
      ASSERT_NOTHROW(BitsetVariableSize(100000), "Reasonably small bitset");
   }
   void equality()
   {
      ASSERT(BitsetVariableSize(10).equals(BitsetVariableSize(10), 10), "Just constructed should be equal");
      BitsetVariableSize a(10);
      BitsetVariableSize b(10);
      a.set(7);
      a.set(9);
      b.set(7);
      b.set(9);
      ASSERT(a.equals(b, 10), "Same bits set should be equal");
   }
   void merge()
   {
      BitsetVariableSize a(10);
      BitsetVariableSize b(10);
      BitsetVariableSize c(10);
      a.set(7);
      b.set(9);
      c.set(7);
      c.set(9);
      ASSERT(c.equals(a.merge(b, 10), 10), "Merge of {7} and {9} should be {7, 9}");
   }
   void intersect()
   {
      BitsetVariableSize a(10);
      BitsetVariableSize b(10);
      BitsetVariableSize c(10);
      a.set(7);
      a.set(8);
      a.set(9);
      b.set(7);
      b.set(8);
      c.set(1);
      ASSERT(a.contains(b, 10), "{7, 8} is contained in {7, 8, 9}");
      ASSERT(!a.contains(c, 10), "{1} is not contained in {7, 8, 9}");
   }
   void count()
   {
      BitsetVariableSize a(10);
      for ( std::size_t i = 0; i < 10; ++i )
      {
         a.set(i);
         ASSERT(a.count(10) == i + 1, "Count mismatch");
      }
   }
}

