
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#ifdef __linux__
#include <unistd.h>
#endif

class TestingGearException
{
   public:
      explicit TestingGearException(const std::string& message_)
      :
         message(message_)
      {
      }
      const char* what() const
      {
         return message.c_str();
      }
   private:
      std::string message;
};

/// helper macro: stringification of an arbitrary expression.
#define STRINGIFY(x) #x

/// helper macro: pretty printing assertions.
#define ASSERT(condition, message) \
   myAssert((condition), __FILE__, __LINE__, message, STRINGIFY(condition));

/// helper macro: pretty printing assertions that code does not throw.
#define ASSERT_NOTHROW(condition, message) \
   try \
   { \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
      (condition); \
      _Pragma("clang diagnostic pop") \
   } \
   catch ( const std::exception& e ) \
   { \
      auto complete_message = std::string((message)) + "(Exception caught: " + e.what() + ")"; \
      myAssert(false, __FILE__, __LINE__, complete_message, STRINGIFY(condition)); \
   } \
   catch ( ... ) \
   { \
      myAssert(false, __FILE__, __LINE__, (message), STRINGIFY(condition)); \
   }

/// helper macro: pretty printing assertions that code throws a specific exception.
#define ASSERT_EXCEPTION(condition, exception, message) \
   try \
   { \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
      (condition); \
      _Pragma("clang diagnostic pop") \
      myAssert(false, __FILE__, __LINE__, std::string("No exception thrown: ") + (message), STRINGIFY(condition)); \
   } \
   catch ( const TestingGearException& ) \
   { \
      throw; \
   } \
   catch ( const exception& ) \
   { \
   } \
   catch ( ... ) \
   { \
      myAssert(false, __FILE__, __LINE__, std::string("Non-matching exception thrown") + (message), STRINGIFY(condition)); \
   }

/// helper macro: pretty printing assertions that code throws some exception.
#define ASSERT_ANY_EXCEPTION(condition, message) \
   try \
   { \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wunused-value\"") \
      (condition); \
      _Pragma("clang diagnostic pop") \
      myAssert(false, __FILE__, __LINE__, (message), STRINGIFY(condition)); \
   } \
   catch ( const TestingGearException& ) \
   { \
      throw; \
   } \
   catch ( ... ) \
   { \
   }

/// TEST_CERR_BUFFER, TestAuxiliaryScopeGuard and the SILENCE_CERR macro are
/// intended to hide output of std::cerr temporarily. This silencing is disabled
/// in myAssert, because error messages originating from failed tests should
/// always be displayed. Silencing is only active in the current scope.


template <typename Function>
struct TestAuxiliaryScopeGuard;

template <typename Function, typename decayType = typename std::decay<Function>::type>
TestAuxiliaryScopeGuard<decayType> makeTestAuxiliaryScopeGuard(Function&&);

template <typename Function>
struct TestAuxiliaryScopeGuard
{
      friend TestAuxiliaryScopeGuard<Function> makeTestAuxiliaryScopeGuard<Function>(Function&&);
   private:
      TestAuxiliaryScopeGuard(const Function& f) : function(f) {}
      TestAuxiliaryScopeGuard(Function&& f) : function(std::move(f)) {}
      Function function;
   public:
      ~TestAuxiliaryScopeGuard()
      {
         function();
      }
      TestAuxiliaryScopeGuard(const TestAuxiliaryScopeGuard&) = delete;
      TestAuxiliaryScopeGuard(TestAuxiliaryScopeGuard&&) = default;
};

template <typename Function, typename decayType>
TestAuxiliaryScopeGuard<decayType> makeTestAuxiliaryScopeGuard(Function&& function)
{
   return TestAuxiliaryScopeGuard<decayType>(std::forward<Function>(function));
}

#define SILENCE_CERR() \
   const auto TEST_CERR_BUFFER = std::cerr.rdbuf(); \
   std::cerr.rdbuf(nullptr); \
   const auto TEST_AUX_SCOPE_GUARD_CERR = makeTestAuxiliaryScopeGuard([TEST_CERR_BUFFER]() \
   { \
      std::cerr.rdbuf(TEST_CERR_BUFFER); \
   });

namespace
{
   using Condition = bool;
   using Message = std::string;
   using Filename = std::string;
   using Expression = std::string;
   using Line = std::size_t;

   void myAssert(Condition condition, Filename filename, Line line, Message message, Expression expression)
   {
      if ( !condition )
      {
         #ifdef __linux__
         const bool color_support = isatty(fileno(stderr));
         #else
         const bool color_support = false;
         #endif
         const std::string red = (color_support) ? "\x1b[31m" : "";
         const std::string reset = (color_support) ? "\x1b[0m" : "";
         throw TestingGearException(red + "[Error (" + filename + ":" + std::to_string(line) + ")] " + message + " [" + expression + "]" + reset);
      }
   }
}

