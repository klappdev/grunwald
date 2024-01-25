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

#include "model/WordModel.hpp"

static constexpr const char* const TAG = "[WordModel] ";
static constexpr const char* const DATETIME_FORMAT = "dd.MM.yyyy HH:mm:ss";

namespace grunwald {

    WordModel::WordModel(QObject* parent)
        : QAbstractListModel(parent) {

        if (mWords.isEmpty()) {
            mWords.append(Word{
                .id = -1,
                .name = tr("No words"),
                .transcription = tr("<empty>"),
                .translation = tr("<empty>"),
                .association = tr("<empty>"),
                .etymology = tr("<empty>"),
                .description = tr("<empty>"),
                .type = WordType::Unknown,
                .image = WordImage{},
                .date = QDateTime::currentDateTime()
            });
        }
    }

    WordModel::~WordModel() {}

    int WordModel::rowCount(const QModelIndex&) const {
        return mWords.size();
    }

    QVariant WordModel::data(const QModelIndex& index, int role) const {
        QVariant value;

        if (!index.isValid() || index.row() > rowCount(index)) {
            qWarning() << TAG << "Index is not valied: [" << index.column() << ":" << index.row() << "]";
            return value;
        }

        const Word& word = mWords.at(index.row());

        switch (role) {
            case WordRoles::NameRole: {
                return QVariant::fromValue(word.name);
            }
            case WordRoles::TranscriptionRole: {
                return QVariant::fromValue(word.transcription);
            }
            case WordRoles::TranslationRole: {
                return QVariant::fromValue(word.translation);
            }
            case WordRoles::AssociationRole: {
                return QVariant::fromValue(word.association);
            }
            case WordRoles::EtymologyRole: {
                return QVariant::fromValue(word.etymology);
            }
            case WordRoles::DescriptionRole: {
                return QVariant::fromValue(word.description);
            }
            case WordRoles::WordTypeRole: {
                return QVariant::fromValue(word.type);
            }
            case WordRoles::DateRole: {
                return QVariant::fromValue(word.date.toString(DATETIME_FORMAT));
            }
            default: {
                qWarning() << TAG << "Index is not valied: [" << index.column() << ":" << index.row() << "]";
                return value;
            }
        }

        return value;
    }

    QHash<int, QByteArray> WordModel::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[WordRoles::NameRole] = "name";
        roles[WordRoles::TranscriptionRole] = "transcription";
        roles[WordRoles::TranslationRole] = "translation";
        roles[WordRoles::AssociationRole] = "association";
        roles[WordRoles::EtymologyRole] = "etymology";
        roles[WordRoles::DescriptionRole] = "description";
        roles[WordRoles::WordTypeRole] = "type";
        roles[WordRoles::DateRole] = "date";

        return roles;
    }

    void WordModel::storeWord(const Word& word) {
        emit beginResetModel();
        mWords.clear();
        mWords.insert(0, word);
        emit endResetModel();

        qDebug() << TAG << "Update word model" << rowCount() << " success!";
    }

    void WordModel::storeWords(const QList<Word>& list) {
        emit beginResetModel();
        mWords.clear();
        mWords.append(list);
        emit endResetModel();

        qDebug() << TAG << "Update word model" << rowCount() << " success!";
    }

    void WordModel::removeWord(int index) {
        if (index < 0 || index >= mWords.count()) {
            return;
        }

        emit beginRemoveRows(QModelIndex(), index, index);
        mWords.removeAt(index);
        emit endRemoveRows();
    }

    Word WordModel::getWord(int index) const {
        if (index < 0 || index >= mWords.count()) {
            return Word{};
        }

        return mWords.at(index);
    }
}
