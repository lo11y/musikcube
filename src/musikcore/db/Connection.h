//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004-2021 musikcube team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <musikcore/config.h>
#include <musikcore/db/Statement.h>
#include <musikcore/db/ScopedTransaction.h>

#include <map>
#include <mutex>

struct sqlite3;
struct sqlite3_stmt;

namespace musik { namespace core { namespace db {

    typedef enum {
        Okay = 0,
        Row = 100,
        Done = 101,
        Error = 1
    } ReturnCode;

    class Connection {
        public:
            DELETE_COPY_AND_ASSIGNMENT_DEFAULTS(Connection)

            Connection() noexcept;
            ~Connection();

            int Open(const std::string &database, unsigned int options = 0, unsigned int cache = 0);
            int Close() noexcept;
            int Execute(const char* sql);

            int64_t LastInsertedId() noexcept;

            int LastModifiedRowCount() noexcept;

            void Interrupt();
            void Checkpoint() noexcept;

        private:
            void Initialize(unsigned int cache);
            void UpdateReferenceCount(bool init);
            int StepStatement(sqlite3_stmt *stmt) noexcept;

            friend class Statement;
            friend class ScopedTransaction;

            int transactionCounter;
            sqlite3 *connection;
            std::mutex mutex;
    };

} } }

