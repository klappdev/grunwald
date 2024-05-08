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

#include "storage/WordStorage.hpp"

#include <algorithm>

static constexpr const char* const TAG = "[WordStorage] ";

namespace grunwald {

    WordStorage::WordStorage(QObject* parent)
        : QObject(parent)
        , mCurrentWord(new Word()) {
    }

    WordStorage::~WordStorage() {
        auto* word = mCurrentWord.loadAcquire();
        delete word;
    }

    bool WordStorage::isWordCached(const QString& name) {
        return mWordDao.checkIfExists(name);
    }

    void WordStorage::updateCurrentWord(const Word& newWord) {
        auto* atomicWord = mCurrentWord.loadAcquire();
        *atomicWord = newWord;

        mCurrentWord.storeRelease(atomicWord);
    }

    auto WordStorage::prepareWords(const QList<Word>& words) -> QVariantList {
        QVariantList variantWords;
        variantWords.reserve(words.size());

        for (const Word& word : words) {
            variantWords.push_back(QVariant::fromValue(word));
        }

        return variantWords;
    }

    void WordStorage::preloadWords() {
        Result<QVector<Word>, DbError> result = mWordDao.getAll();

        if (result.hasValue() && !result.value().isEmpty()) {
            QList<Word> localWords = result.value();
            QVariantList variantWords = prepareWords(localWords);

            if (!localWords.isEmpty()) {
                updateCurrentWord(localWords.at(0));
            }

            qInfo() << TAG << "Load all words from db success!" << Qt::endl;
            emit localWordsPrecessed(QVariant::fromValue(variantWords));
        } else {
            const QString errorMessage = "Error load all words from db: " + result.error().getMessage();

            qWarning() << TAG << errorMessage << Qt::endl;
            emit wordError(errorMessage);
        }
    }

    void WordStorage::searchWord(const QString& name) {
        if (isWordCached(name)) {
            Result<QVector<Word>, DbError> result = mWordDao.search(name);

            if (result.hasValue() && !result.value().isEmpty()) {
                auto searchedWord = result.value().at(0);
                updateCurrentWord(searchedWord);

                qInfo() << TAG << "Search word into db: " << searchedWord.name << " success!" << Qt::endl;
                emit wordProcessed(searchedWord);
            } else {
                const QString errorMessage = "Error search word into db: " + result.error().getMessage();

                qWarning() << TAG << errorMessage << Qt::endl;
                emit wordError(errorMessage);
            }
        } else {
            mWordService.fetchWordContent(name);

            QObject::connect(&mWordService, &WordService::wordContentProcessed,
                             this, [this](const Word& searchedWord) {
                updateCurrentWord(searchedWord);

                qInfo() << TAG << "Search word from network: " << searchedWord.name << " success!" << Qt::endl;
                emit wordProcessed(searchedWord);
            });
            QObject::connect(&mWordService, &WordService::wordProcessedError,
                             this, [this](const QString& errorMessage) {
                const QString extendedErrorMessage = "Error search word from network: " + errorMessage;

                qWarning() << TAG << extendedErrorMessage << Qt::endl;
                emit wordError(extendedErrorMessage);
            });
        }
    }

    void WordStorage::insertWord() {
        Word* wordPointer = mCurrentWord.loadAcquire();

        if (wordPointer == nullptr) {
            qWarning() << TAG << "Cached word is NULL!" << Qt::endl;
            return;
        }
        const QString wordName = wordPointer->name;

        Result<void, DbError> result = mWordDao.add(*wordPointer);

        if (result.hasError()) {
            const QString errorMessage = "Error save word into db: " + result.error().getMessage();

            qWarning() << TAG << errorMessage << Qt::endl;
            emit wordError(errorMessage);
        } else {
            const QString infoMessage = "Save word into db: " + wordName + " success!";

            qInfo() << TAG << infoMessage << Qt::endl;
        }
    }

    void WordStorage::removeWord() {
        Word* wordPointer = mCurrentWord.loadAcquire();

        if (wordPointer == nullptr) {
            qWarning() << TAG << "Cached word is NULL!" << Qt::endl;
            return;
        }

        const QString wordName = wordPointer->name;

        Result<void, DbError> result = mWordDao.remove(*wordPointer);

        if (result.hasError()) {
            const QString errorMessage = "Error remove word from db: " + result.error().getMessage();

            qWarning() << TAG << errorMessage << Qt::endl;
            emit wordError(errorMessage);
        } else {
            const QString infoMessage = "Remove word from db: " + wordName + " success!";

            qInfo() << TAG << infoMessage << Qt::endl;
        }
    }
}

