
//-------------------------------------------------------------------------------//
// Author: Stefan Lörwald, Universität Heidelberg                                //
// License: CC BY-NC 4.0 http://creativecommons.org/licenses/by-nc/4.0/legalcode //
//-------------------------------------------------------------------------------//

#define COMPILE_TEMPLATE_COMMUNICATION
#include "communication.h"
#undef COMPILE_TEMPLATE_COMMUNICATION

#ifdef MPI_SUPPORT

#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <thread>
#include <vector>

#include "cast.h"
#include "communication.h"
#include "mpi_no_warnings.h"

using namespace panda;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

namespace
{
   using Tag = int;
   using Bytes = int;
   namespace tag
   {
      constexpr static int matrix = 0;
      constexpr static int row = 1;
   }
   constexpr static int Master = 0;
   /// Send transmission to ID with Tag of Bytes from buffer.
   template <typename Pointer>
   MPI_Request send(Pointer, const Bytes, const int, const Tag) noexcept;
   /// Receive transmission with Tag of Bytes into buffer from ID.
   template <typename Pointer>
   void receive(Pointer, const Bytes, const int, const Tag);
   /// Returns true if there is a matching incoming transmission.
   bool isMatchingIncomingTransmissionAvailable(const int, const Tag) noexcept;
   /// Occasionally tests for completion of request, returns on success.
   void waitForCompletion(std::mutex&, MPI_Request) noexcept;
   /// Allow other threads to acquire lock.
   void pause();
}

template <typename Integer>
void panda::Communication::toMaster(const Matrix<Integer>& matrix) const
{
   toMaster(cast<int>(matrix));
}

template <typename Integer>
Matrix<Integer> panda::Communication::fromSlave(const int id) const
{
   return cast<Integer>(fromSlave<int>(id));
}

template <typename Integer>
void panda::Communication::toSlave(const Row<Integer>& row, const int id) const
{
   toSlave(cast<int>(row), id);
}

template <typename Integer>
Row<Integer> panda::Communication::fromMaster() const
{
   return cast<Integer>(fromMaster<int>());
}

/// It needs to be asserted that no Recv blocks a Send and vice versa if
/// the sending / receiving side isn't ready. Therefore, Probe is used to
/// assert the presence of an incoming message. Send instructions are
/// wrapped by an additional mutex. MPI send instructions are itself
/// non-blocking (Isend). But the send_mutex is only unlocked after the
/// Isend has actually transferred the data.

template <>
void panda::Communication::toMaster(const Matrix<int>& matrix) const
{
   std::vector<MPI_Request> requests;
   requests.reserve(2 + matrix.size());
   { // post sends, scoped because we only hold the lock for as long the MPI calls last.
      std::lock_guard<std::mutex> lock(mutex);
      const auto size = matrix.size();
      const auto row_size = matrix.back().size();
      requests.emplace_back(send(&size, sizeof(decltype(size)), Master, tag::matrix));
      requests.emplace_back(send(&row_size, sizeof(decltype(row_size)), Master, tag::matrix));
      for ( const auto& row : matrix )
      {
         const auto bytes = static_cast<int>(row_size * sizeof(int));
         requests.emplace_back(send(&row.front(), bytes, Master, tag::matrix));
      }
   }
   // wait for completion of all of the requests
   for ( auto& request : requests )
   {
      waitForCompletion(mutex, request);
   }
}

template <>
Matrix<int> panda::Communication::fromSlave(const int id) const
{
   for ( ; true; pause() ) // look for matching transmission, but pause to let others acquire lock.
   {
      std::lock_guard<std::mutex> lock(mutex);
      if ( isMatchingIncomingTransmissionAvailable(id, tag::matrix) )
      {
         std::size_t size;
         std::size_t row_size;
         receive(&size, sizeof(decltype(size)), id, tag::matrix);
         receive(&row_size, sizeof(decltype(row_size)), id, tag::matrix);
         Matrix<int> matrix(size);
         for ( auto& row : matrix )
         {
            row.resize(row_size);
            const auto bytes = static_cast<int>(row_size * sizeof(int));
            receive(&row.front(), bytes, id, tag::matrix);
         }
         return matrix;
      }
   }
}

template <>
void panda::Communication::toSlave<int>(const Row<int>& row, const int id) const
{
   MPI_Request request;
   {
      std::lock_guard<std::mutex> lock(mutex);
      const auto begin = row.empty() ? nullptr : &row.front();
      const auto bytes = static_cast<int>(row.size() * sizeof(int));
      request = send(begin, bytes, id, tag::row);
   }
   waitForCompletion(mutex, request);
}

template <>
Row<int> panda::Communication::fromMaster<int>() const
{
   for ( ; true; pause() ) // look for matching transmission, but pause to let others acquire lock.
   {
      std::lock_guard<std::mutex> lock(mutex);
      if ( isMatchingIncomingTransmissionAvailable(Master, tag::row) )
      {
         MPI_Status status;
         MPI_Probe(Master, tag::row, MPI_COMM_WORLD, &status);
         int count;
         MPI_Get_count(&status, MPI_BYTE, &count);
         const auto row_size = static_cast<std::size_t>(count) / sizeof(int);
         Row<int> row(row_size);
         const auto begin = row.empty() ? nullptr : &row.front();
         const auto bytes = static_cast<int>(row.size() * sizeof(int));
         receive(begin, bytes, Master, tag::row);
         return row;
      }
   }
}

namespace
{
   template <typename Pointer>
   MPI_Request send(Pointer data, const Bytes bytes, const int id, const Tag tag) noexcept
   {
      MPI_Request request;
      void* buffer = const_cast<void*>(reinterpret_cast<const void*>(data));
      MPI_Isend(buffer, bytes, MPI_BYTE, id, tag, MPI_COMM_WORLD, &request);
      return request;
   }

   template <typename Pointer>
   void receive(Pointer data, const Bytes bytes, const int id, const Tag tag)
   {
      MPI_Status status;
      MPI_Recv(data, bytes, MPI_BYTE, id, tag, MPI_COMM_WORLD, &status);
      int count;
      MPI_Get_count(&status, MPI_BYTE, &count);
      if ( count != static_cast<int>(bytes) )
      {
         throw std::invalid_argument("Bad number of bytes received.");
      }
   }

   bool isMatchingIncomingTransmissionAvailable(const int id, const Tag tag) noexcept
   {
      int flag;
      MPI_Status status;
      MPI_Iprobe(id, tag, MPI_COMM_WORLD, &flag, &status);
      return flag;
   }

   void waitForCompletion(std::mutex& mutex, MPI_Request request) noexcept
   {
      for ( ; true; pause() )
      {
         std::lock_guard<std::mutex> lock(mutex);
         int flag;
         MPI_Status status;
         MPI_Test(&request, &flag, &status);
         if ( flag )
         {
            break;
         }
      }
   }

   void pause()
   {
      #if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 7))
      std::this_thread::yield();
      #endif
      std::this_thread::sleep_for(std::chrono::nanoseconds(1));
   }
}

#pragma GCC diagnostic pop

#endif // MPI_SUPPORT

