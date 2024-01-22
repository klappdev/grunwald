/*
 * Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2024 https://github.com/klappdev
 *
 * Permission is hereby  granted, free of charge, to any  person obtaining a copy
 * of this software and associated  documentation files (the "Software"), to deal
 * in the Software  without restriction, including without  limitation the rights
 * to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 * copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 * IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 * FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 * AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 * LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <QtSql>

#include "common/Word.hpp"
#include "util/Result.hpp"
#include "util/Error.hpp"

namespace grunwald {
    using DbError = Error;

    class WordDao final { //FIXME: extends to work with WordType, WordImage
    public:
        WordDao(const WordDao&) = delete;
        WordDao& operator=(WordDao&) = delete;

        static auto getInstance() -> WordDao&;

        void reset();

        bool checkIfExists(const Word& word);

        auto add(const Word& word) -> Result<void, DbError>;
        auto update(const Word& word) -> Result<void, DbError>;
        auto remove(qint32 id) -> Result<void, DbError>;

        auto get(qint32 id) -> Result<Word, DbError>;
        auto getAll() -> Result<QVector<Word>, DbError>;

        auto search(const QString& text) -> Result<QVector<Word>, DbError>;
        auto sort(bool asc) -> Result<QVector<Word>, DbError>;

    private:
        WordDao();
        ~WordDao();

        auto createConnection() -> QSqlDatabase;
        void createTable();

        void init(Word& word);

        QSqlDatabase mDatabase;
        QSqlQuery mSqlQuery;
        QSqlRecord mSqlRecord;
    };
}
