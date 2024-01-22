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

#include "db/WordDao.hpp"

static constexpr const char* const TAG = "[WordDao] ";
static constexpr const char* const DB_NAME = "grunwald";
static constexpr const char* const DB_FILE = "grunwald.sqlite";
static constexpr const char* const DATETIME_FORMAT = "HH:mm:ss dd.MM.yyyy";

namespace grunwald {

    WordDao::WordDao()
        : mDatabase(createConnection())
        , mSqlQuery(QSqlQuery(mDatabase))
        , mSqlRecord(mSqlQuery.record()) {
        createTable();
    }

    WordDao::~WordDao() {
        mDatabase.close();

        QSqlDatabase::removeDatabase(DB_NAME);
    }

    auto WordDao::getInstance() -> WordDao& {
        static WordDao instance;

        return instance;
    }

    auto WordDao::createConnection() -> QSqlDatabase {
        QDir databaseFile(QString::fromStdString(DB_FILE));

        auto database = QSqlDatabase::addDatabase("QSQLITE", DB_NAME);
        database.setDatabaseName(databaseFile.absolutePath());

        if (!database.open()) {
            qWarning() << TAG << "Can't open db: " << database.lastError() << Qt::endl;
        } else {
            qInfo() << TAG << "DB connected!" << Qt::endl;
        }

        database.exec("PRAGMA locking_mode = EXCLUSIVE");

        return database;
    }

    void WordDao::createTable() {
        if (!mSqlQuery.exec(R"xxx(CREATE TABLE IF NOT EXISTS word (
                                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                                    name TEXT NOT NULL,
                                    transcription TEXT NOT NULL,
                                    translation TEXT NOT NULL,
                                    association TEXT NOT NULL,
                                    etymology TEXT NOT NULL,
                                    description TEXT NOT NULL,
                                    date TEXT NOT NULL);
                            )xxx")) {
            qWarning() << TAG << "Table word was not created!" << mDatabase.lastError() << Qt::endl;
        } else {
            qInfo() << TAG << "Table word was created!" << Qt::endl;
        }

        mSqlQuery.executedQuery();
    }

    bool WordDao::checkIfExists(const Word& word) {
        bool success = false;

        mSqlQuery.prepare("SELECT COUNT(*) FROM word WHERE id=?");
        mSqlQuery.addBindValue(word.id);

        if (!mSqlQuery.exec() || !mSqlQuery.first()) {
           qWarning() << TAG << "check if exists word failed:  " << mSqlQuery.lastError();
        } else if (mSqlQuery.value(0) == 0) {
            success = false;
        } else {
           success = true;
        }

        return success;
    }

    void WordDao::init(Word& word) {
        word.id = mSqlQuery.value(mSqlRecord.indexOf("id")).toInt();
        word.name = mSqlQuery.value(mSqlRecord.indexOf("name")).toString();

        word.transcription = mSqlQuery.value(mSqlRecord.indexOf("transcription")).toString();
        word.translation = mSqlQuery.value(mSqlRecord.indexOf("translation")).toString();

        word.association = mSqlQuery.value(mSqlRecord.indexOf("association")).toString();
        word.etymology = mSqlQuery.value(mSqlRecord.indexOf("etymology")).toString();
        word.description = mSqlQuery.value(mSqlRecord.indexOf("description")).toString();

        const QString dateTime = mSqlQuery.value(mSqlRecord.indexOf("date")).toString();
        word.date = QDateTime::fromString(dateTime, "HH:mm:ss dd.MM.yyyy");
    }

    void WordDao::reset() {
        if (mSqlQuery.exec("DELETE FROM word")) {
        } else {
            qWarning() << TAG << "delete word error:  " << mSqlQuery.lastError();
        }
    }

    auto WordDao::add(const Word& word) -> Result<void, DbError> {
        mSqlQuery.prepare(R"xxx(INSERT INTO word (
                                    name,
                                    transcription,
                                    translation,
                                    association,
                                    etymology,
                                    description,
                                    date)
                                VALUES (?, ?, ?, ?, ?, ?, ?);
                          )xxx");

        mSqlQuery.addBindValue(word.name);
        mSqlQuery.addBindValue(word.transcription);
        mSqlQuery.addBindValue(word.translation);
        mSqlQuery.addBindValue(word.association);
        mSqlQuery.addBindValue(word.etymology);
        mSqlQuery.addBindValue(word.description);
        mSqlQuery.addBindValue(word.date.toString(DATETIME_FORMAT));

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Add word error:  " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        return {};
    }

    auto WordDao::update(const Word& word) -> Result<void, DbError> {
        mSqlQuery.prepare(R"xxx(UPDATE word SET name=?,
                                                transcription=?,
                                                translation=?,
                                                association=?,
                                                etymology=?,
                                                description=?,
                                                date=?
                                WHERE id=?;
                          )xxx");

        mSqlQuery.addBindValue(word.name);
        mSqlQuery.addBindValue(word.transcription);
        mSqlQuery.addBindValue(word.translation);
        mSqlQuery.addBindValue(word.association);
        mSqlQuery.addBindValue(word.etymology);
        mSqlQuery.addBindValue(word.description);
        mSqlQuery.addBindValue(word.date.toString(DATETIME_FORMAT));
        mSqlQuery.addBindValue(word.id);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Update word error:  " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        return {};
    }

    auto WordDao::remove(qint32 id) -> Result<void, DbError> {
        mSqlQuery.prepare("DELETE FROM word WHERE id=?;");
        mSqlQuery.addBindValue(id);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Delete word error:  " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        return {};
    }

    auto WordDao::get(qint32 id) -> Result<Word, DbError> {
        Word word = {};

        mSqlQuery.prepare("SELECT * FROM word WHERE id=?;");
        mSqlQuery.addBindValue(id);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Select word error: " << id << "," << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        mSqlRecord = mSqlQuery.record();

        if (mSqlQuery.next()) {
            init(word);
        }

        return word;
    }

    auto WordDao::getAll() -> Result<QVector<Word>, DbError> {
        QVector<Word> words;
        Word word = {};

        if (!mSqlQuery.exec("SELECT * FROM word;")) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Select all word error: " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        mSqlRecord = mSqlQuery.record();

        while (mSqlQuery.next()) {
            init(word);
            words.push_back(word);
        }

        return words;
    }

    auto WordDao::search(const QString& name) -> Result<QVector<Word>, DbError> {
        QVector<Word> words;
        Word word = {};

        mSqlQuery.prepare("SELECT * FROM word WHERE name LIKE ?;");
        mSqlQuery.addBindValue("%" + name + "%");

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Search words error: " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        mSqlRecord = mSqlQuery.record();

        while (mSqlQuery.next()) {
            init(word);
            words.push_back(word);
        }

        return words;
    }

    auto WordDao::sort(bool asc) -> Result<QVector<Word>, DbError> {
        QVector<Word> words;
        Word word = {};

        const QString ascending = asc ? "ASC" : "DESC";

        mSqlQuery.prepare("SELECT * FROM word ORDER BY name ?;");
        mSqlQuery.addBindValue(ascending);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Sort words error: " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        mSqlRecord = mSqlQuery.record();

        while (mSqlQuery.next()) {
            init(word);
            words.append(word);
        }

        return words;
    }
}
