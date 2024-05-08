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
static constexpr const char* const DB_CONNECTION = "grunwald_connection";
static constexpr const char* const DB_FILE = "grunwald.sqlite";
static constexpr const char* const DATETIME_FORMAT = "dd.MM.yyyy HH:mm:ss";

namespace grunwald {

    WordDao::WordDao()
        : mDatabase(openDatabase())
        , mSqlQuery(QSqlQuery(mDatabase))
        , mSqlRecord(mSqlQuery.record()) {
        createTables();
    }

    WordDao::~WordDao() {
        closeDatabase();
    }

    auto WordDao::openDatabase() -> QSqlDatabase {
        auto database = QSqlDatabase::addDatabase("QSQLITE", DB_CONNECTION);
        database.setDatabaseName(DB_FILE);

        if (!database.open()) {
            qWarning() << TAG << "Can't open database: " << database.lastError() << Qt::endl;
        } else {
            qInfo() << TAG << "Database connected!" << Qt::endl;
        }

        return database;
    }

    void WordDao::closeDatabase() {
        mDatabase.close();
        QSqlDatabase::removeDatabase(DB_CONNECTION);

        qInfo() << TAG << "Database closed!" << Qt::endl;
    }

    void WordDao::createTables() {
        if (!mSqlQuery.exec("PRAGMA locking_mode = EXCLUSIVE")) {
            qWarning() << TAG << "Database could not set locking mode!" << mSqlQuery.lastError() << Qt::endl;
        }

        if (!mSqlQuery.exec(R"xxx(CREATE TABLE IF NOT EXISTS word_image (
                                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                                    url TEXT,
                                    width INT NOT NULL,
                                    height INT NOT NULL,
                                    data BLOB);
                            )xxx")) {
            qWarning() << TAG << "Table `word_image` was not created!" << mSqlQuery.lastError() << Qt::endl;
        } else {
            qInfo() << TAG << "Table `word_image` was created!" << Qt::endl;
        }

        mSqlQuery.executedQuery();

        if (!mSqlQuery.exec(R"xxx(CREATE TABLE IF NOT EXISTS word (
                                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                                    id_image INT NOT NULL,
                                    name TEXT NOT NULL,
                                    transcription TEXT NOT NULL,
                                    translation TEXT NOT NULL,
                                    association TEXT NOT NULL,
                                    etymology TEXT NOT NULL,
                                    description TEXT NOT NULL,
                                    type INT NOT NULL DEFAULT (1),
                                    date DATETIME,
                                    FOREIGN KEY (id_image) REFERENCES word_image(id));
                            )xxx")) {
            qWarning() << TAG << "Table `word` was not created!" << mSqlQuery.lastError() << Qt::endl;
        } else {
            qInfo() << TAG << "Table `word` was created!" << Qt::endl;
        }

        mSqlQuery.executedQuery();
    }

    bool WordDao::checkIfExists(const QString& name) {
        bool success = false;

        mSqlQuery.prepare("SELECT COUNT(*) FROM word WHERE name=?");
        mSqlQuery.addBindValue(name);

        if (!mSqlQuery.exec() || !mSqlQuery.first()) {
           qWarning() << TAG << "check if exists `word` failed:  " << mSqlQuery.lastError();
        } else if (mSqlQuery.value(0) == 0) {
            success = false;
        } else {
           success = true;
        }

        return success;
    }

    void WordDao::reset() {
        if (mSqlQuery.exec("DELETE FROM word")) {
        } else {
            qWarning() << TAG << "delete `word` error:  " << mSqlQuery.lastError();
        }
    }

    auto WordDao::add(const Word& word) -> Result<void, DbError> {
        int lastWordImageId = -1;

        if (word.hasImage()) {
            mSqlQuery.prepare(R"xxx(INSERT INTO word_image (url, width, height, data)
                                    VALUES (?, ?, ?, ?);
                              )xxx");

            mSqlQuery.addBindValue(word.image.url.toString());
            mSqlQuery.addBindValue(word.image.width);
            mSqlQuery.addBindValue(word.image.height);
            mSqlQuery.addBindValue(word.image.data);

            if (!mSqlQuery.exec()) {
                const QSqlError sqlError = mSqlQuery.lastError();

                qWarning() << TAG << "Add `word_image` error:  " << sqlError;
                return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
            }

            lastWordImageId = mSqlQuery.lastInsertId().toInt();
        }

        mSqlQuery.prepare(R"xxx(INSERT INTO word (
                                    id_image, name, transcription, translation,
                                    association, etymology, description,
                                    type, date)
                                VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
                          )xxx");

        mSqlQuery.addBindValue(lastWordImageId);
        mSqlQuery.addBindValue(word.name);
        mSqlQuery.addBindValue(word.transcription);
        mSqlQuery.addBindValue(word.translation);
        mSqlQuery.addBindValue(word.association);
        mSqlQuery.addBindValue(word.etymology);
        mSqlQuery.addBindValue(word.description);
        mSqlQuery.addBindValue(static_cast<std::underlying_type_t<WordType>>(word.type));
        mSqlQuery.addBindValue(word.date);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Add `word` error:  " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        qInfo() << TAG << "Add " << word.name << " into `word` table success!" << Qt::endl;

        return {};
    }

    auto WordDao::update(const Word& word) -> Result<void, DbError> {
        int lastWordImageId = -1;

        if (word.hasImage()) {
            mSqlQuery.prepare(R"xxx(UPDATE word_image SET
                                        url=?, width=?, height=?, data=?
                                    WHERE id=?;
                              )xxx");

            mSqlQuery.addBindValue(word.image.url.toString());
            mSqlQuery.addBindValue(word.image.width);
            mSqlQuery.addBindValue(word.image.height);
            mSqlQuery.addBindValue(word.image.data);
            mSqlQuery.addBindValue(word.image.id);

            if (!mSqlQuery.exec()) {
                const QSqlError sqlError = mSqlQuery.lastError();

                qWarning() << TAG << "Update `word_image` error:  " << sqlError;
                return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
            }

            lastWordImageId = mSqlQuery.lastInsertId().toInt();
        }

        mSqlQuery.prepare(R"xxx(UPDATE word SET
                                    id_image=?, name=?, transcription=?, translation=?,
                                    association=?, etymology=?, description=?,
                                    type=?, date=?
                                WHERE id=?;
                          )xxx");

        mSqlQuery.addBindValue(lastWordImageId);
        mSqlQuery.addBindValue(word.name);
        mSqlQuery.addBindValue(word.transcription);
        mSqlQuery.addBindValue(word.translation);
        mSqlQuery.addBindValue(word.association);
        mSqlQuery.addBindValue(word.etymology);
        mSqlQuery.addBindValue(word.description);
        mSqlQuery.addBindValue(static_cast<std::underlying_type_t<WordType>>(word.type));
        mSqlQuery.addBindValue(word.date);
        mSqlQuery.addBindValue(word.id);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Update `word` error:  " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        qInfo() << TAG << "Update " << word.name << " into `word` table success!" << Qt::endl;

        return {};
    }

    auto WordDao::remove(const Word& word) -> Result<void, DbError> {
        if (word.hasImage()) {
            mSqlQuery.prepare("DELETE FROM word_image WHERE id=?;");
            mSqlQuery.addBindValue(word.image.id);

            if (!mSqlQuery.exec()) {
                const QSqlError sqlError = mSqlQuery.lastError();

                qWarning() << TAG << "Delete `word_image` error: " << sqlError;
                return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
            }
        }

        mSqlQuery.prepare("DELETE FROM word WHERE id=?;");
        mSqlQuery.addBindValue(word.id);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Delete `word` error:  " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        qInfo() << TAG << "Remove " << word.name << " from `word` table success!" << Qt::endl;

        return {};
    }

    auto WordDao::prepareWord() -> Word {
        return Word {
            .id = mSqlQuery.value(mSqlRecord.indexOf("word_id")).toInt(),
            .name = mSqlQuery.value(mSqlRecord.indexOf("name")).toString(),
            .transcription = mSqlQuery.value(mSqlRecord.indexOf("transcription")).toString(),
            .translation = mSqlQuery.value(mSqlRecord.indexOf("translation")).toString(),
            .association = mSqlQuery.value(mSqlRecord.indexOf("association")).toString(),
            .etymology = mSqlQuery.value(mSqlRecord.indexOf("etymology")).toString(),
            .description = mSqlQuery.value(mSqlRecord.indexOf("description")).toString(),
            .type = static_cast<WordType>(mSqlQuery.value(mSqlRecord.indexOf("type")).toInt()),
            .image = WordImage {
                .id = mSqlQuery.value(mSqlRecord.indexOf("image_id")).toInt(),
                .url = mSqlQuery.value(mSqlRecord.indexOf("image_url")).toUrl(),
                .width = mSqlQuery.value(mSqlRecord.indexOf("image_width")).toInt(),
                .height = mSqlQuery.value(mSqlRecord.indexOf("image_height")).toInt(),
                .data = mSqlQuery.value(mSqlRecord.indexOf("image_data")).toByteArray(),
            },
            .date = mSqlQuery.value(mSqlRecord.indexOf("date")).toDateTime(),
        };
    }

    auto WordDao::get(qint32 id) -> Result<Word, DbError> {
        mSqlQuery.prepare(R"xxx(SELECT word.id AS word_id,
                                       word.name AS name,
                                       word.transcription AS transcription,
                                       word.translation AS translation,
                                       word.association AS association,
                                       word.etymology AS etymology,
                                       word.description AS description,
                                       word.type AS type,
                                       word.date AS date,
                                       word_image.id AS image_id,
                                       word_image.url AS image_url,
                                       word_image.width AS image_width,
                                       word_image.height AS image_height,
                                       word_image.data AS image_data
                                FROM word
                                LEFT JOIN word_image ON word.id = word_image.id
                                WHERE word.id=?)xxx");
        mSqlQuery.addBindValue(id);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Select `word` error: " << id << "," << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        mSqlRecord = mSqlQuery.record();

        if (mSqlQuery.next()) {
            qInfo() << TAG << "Get by id=" << id << " from `word` table success!" << Qt::endl;
            return prepareWord();
        }

        return {};
    }

    auto WordDao::getAll() -> Result<QVector<Word>, DbError> {
        QVector<Word> words;
        Word word = {};

        if (!mSqlQuery.exec(R"xxx(SELECT word.id AS word_id,
                                       word.name AS name,
                                       word.transcription AS transcription,
                                       word.translation AS translation,
                                       word.association AS association,
                                       word.etymology AS etymology,
                                       word.description AS description,
                                       word.type AS type,
                                       word.date AS date,
                                       word_image.id AS image_id,
                                       word_image.url AS image_url,
                                       word_image.width AS image_width,
                                       word_image.height AS image_height,
                                       word_image.data AS image_data
                                FROM word
                                LEFT JOIN word_image ON word.id = word_image.id)xxx")) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Select all `word`s error: " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        mSqlRecord = mSqlQuery.record();

        while (mSqlQuery.next()) {
            words.push_back(prepareWord());
        }

        qInfo() << TAG << "Get all from `word` table success!" << Qt::endl;

        return words;
    }

    auto WordDao::search(const QString& name) -> Result<QVector<Word>, DbError> {
        QVector<Word> words;
        Word word = {};

        mSqlQuery.prepare(R"xxx(SELECT word.id AS word_id,
                                       word.name AS name,
                                       word.transcription AS transcription,
                                       word.translation AS translation,
                                       word.association AS association,
                                       word.etymology AS etymology,
                                       word.description AS description,
                                       word.type AS type,
                                       word.date AS date,
                                       word_image.id AS image_id,
                                       word_image.url AS image_url,
                                       word_image.width AS image_width,
                                       word_image.height AS image_height,
                                       word_image.data AS image_data
                                FROM word
                                LEFT JOIN word_image ON word.id = word_image.id
                                WHERE word.name = :word_name)xxx");
        mSqlQuery.bindValue(":word_name", name);

        if (!mSqlQuery.exec()) {
            const QSqlError sqlError = mSqlQuery.lastError();

            qWarning() << TAG << "Search `word`s error: " << sqlError;
            return DbError { sqlError.text() , static_cast<qint32>(sqlError.type()) };
        }

        mSqlRecord = mSqlQuery.record();

        while (mSqlQuery.next()) {
            words.push_back(prepareWord());
        }

        qInfo() << TAG << "Search " << name << " into `word` table success!" << Qt::endl;

        return words;
    }
}
