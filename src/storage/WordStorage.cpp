/*
 * Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2025 https://github.com/klappdev
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

namespace {
    constexpr const char* const TAG = "[WordStorage] ";
}

namespace grunwald {

    WordStorage::WordStorage(WordCache* wordCache) : mWordCache(wordCache) {

        QObject::connect(&mWordContentService, &WordContentService::wordContentProcessed,
                         this, &WordStorage::onWordContentProcessFinished);
        QObject::connect(&mWordContentService, &WordContentService::wordContentErrorProcessed,
                         this, &WordStorage::onWordProcessErrorFinished);
    }

    WordStorage::~WordStorage() {
    }

    bool WordStorage::isWordCached() const {
        return mWordCache->isValid();
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
                mWordCache->storeWordContent(localWords.at(0));
            }

            qInfo() << TAG << "Load all words from db success!" << Qt::endl;
            emit localWordsHandled(QVariant::fromValue(variantWords));
        } else {
            mWordCache->clear();

            const QString errorMessage = "Error load all words from db: " + (result.hasError() ? result.error().getMessage() : "unknown");

            qWarning() << TAG << errorMessage << Qt::endl;
            emit wordErrorHandled(errorMessage);
        }
    }

    void WordStorage::searchWord(const QString& name) {
        if (mWordDao.checkIfExists(name)) {
            Result<QVector<Word>, DbError> result = mWordDao.search(name);

            if (result.hasValue() && !result.value().isEmpty()) {
                auto searchedWord = result.value().at(0);
                mWordCache->storeWordContent(searchedWord);


                qInfo() << TAG << "Search word into db: " << searchedWord.name << " success!" << Qt::endl;
                emit wordContentHandled(searchedWord);
            } else {
                mWordCache->clear();

                const QString errorMessage = "Error search word into db: " + result.error().getMessage();

                qWarning() << TAG << errorMessage << Qt::endl;
                emit wordErrorHandled(errorMessage);
            }
        } else {
            mWordCache->clear();
            mWordContentService.fetchWordContent(name);
        }
    }

    void WordStorage::insertWord() {
        Word word = mWordCache->loadWordContent();

        Result<void, DbError> result = mWordDao.add(word);

        if (result.hasError()) {
            const QString errorMessage = "Error save word into db: " + result.error().getMessage();

            qWarning() << TAG << errorMessage << Qt::endl;
            emit wordErrorHandled(errorMessage);
        } else {
            const QString infoMessage = "Save word into db: " + word.name + " success!";

            qInfo() << TAG << infoMessage << Qt::endl;
        }
    }

    void WordStorage::removeWord() {
        Word word = mWordCache->loadWordContent();

        Result<void, DbError> result = mWordDao.remove(word);

        if (result.hasError()) {
            const QString errorMessage = "Error remove word from db: " + result.error().getMessage();

            qWarning() << TAG << errorMessage << Qt::endl;
            emit wordErrorHandled(errorMessage);
        } else {
            const QString infoMessage = "Remove word from db: " + word.name + " success!";

            qInfo() << TAG << infoMessage << Qt::endl;
        }
    }

    void WordStorage::onWordContentProcessFinished(const Word& searchedWord) {
        mWordCache->storeWordContent(searchedWord);

        qInfo() << TAG << "Search word from network: " << searchedWord.name << " success!" << Qt::endl;
        emit wordContentHandled(searchedWord);
    }

    void WordStorage::onWordProcessErrorFinished(const QString& errorMessage) {
        const QString extendedErrorMessage = "Error search word from network: " + errorMessage;

        qWarning() << TAG << extendedErrorMessage << Qt::endl;
        emit wordErrorHandled(extendedErrorMessage);
    }
}

