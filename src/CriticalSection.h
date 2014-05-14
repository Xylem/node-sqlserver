//---------------------------------------------------------------------------------------------------------------------------------
// File: CriticalSection.h
// Contents: Wrapper for a critical section that handles scope
// 
// Copyright Microsoft Corporation and contributors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// You may obtain a copy of the License at:
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//---------------------------------------------------------------------------------------------------------------------------------

#ifdef __linux__

    #include <pthread.h>

    #define CRITICAL_SECTION pthread_mutex_t
    #define InitializeCriticalSection(X) \
            pthread_mutexattr_t attr; \
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP); \
            pthread_mutex_init(X, &attr); \
            pthread_mutexattr_destroy(&attr)
            
    #define DeleteCriticalSection(X) \
            pthread_mutex_destroy(X) 
            
    #define EnterCriticalSection(X) \
            pthread_mutex_lock(X)
            
    #define LeaveCriticalSection(X) \
            pthread_mutex_unlock(X)

#else
    #include <windows.h>

#endif

namespace mssql {

   class CriticalSection {

   public:

       CriticalSection()
       {
           InitializeCriticalSection( &handle_ );
       }

       ~CriticalSection()
       {
           DeleteCriticalSection( &handle_ );
       }

       void lock( void )
       {
           EnterCriticalSection( &handle_ );
       }

       void unlock( void )
       {
           LeaveCriticalSection( &handle_ );
       }

   private:

       CRITICAL_SECTION handle_;
   };

   class ScopedCriticalSectionLock {

   public:

       ScopedCriticalSectionLock( CriticalSection& cs ) :
           criticalSection_( cs )
       {
           criticalSection_.lock();
       }

       ~ScopedCriticalSectionLock( void )
       {
           criticalSection_.unlock();
       }

   private:

       CriticalSection& criticalSection_;
   };

}  // mssql
