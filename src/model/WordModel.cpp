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

static constexpr const char* const TAG =  "[WordModel] ";

namespace grunwald {

    WordModel::WordModel(QObject* parent)
        : QAbstractListModel(parent) {
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
            case WordRoles::DateRole: {
                return QVariant::fromValue(word.date);
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
        roles[WordRoles::DateRole] = "date";

        return roles;
    }

    void WordModel::setWords(QList<Word>&& list) {
        emit beginResetModel();
        mWords = std::move(list);
        emit endResetModel();

        qDebug() << "Update word model" << rowCount() << " success!";
    }

    const QList<Word>& WordModel::getWords() const {
        return mWords;
    }
}
