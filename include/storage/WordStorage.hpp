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

#include "cache/WordCache.hpp"
#include "db/WordDao.hpp"
#include "net/WordContentService.hpp"

namespace grunwald {

    class WordStorage final : public QObject {
        Q_OBJECT
        Q_PROPERTY(bool wordCached READ isWordCached NOTIFY wordCachedChanged)
    public:
        WordStorage(WordCache* wordCache);
        ~WordStorage();

        Q_INVOKABLE void preloadWords();
        Q_INVOKABLE void searchWord(const QString& name);

        Q_INVOKABLE void insertWord();
        Q_INVOKABLE void removeWord();

        bool isWordCached() const;

    signals:
        void wordContentHandled(const Word& word);
        void localWordsHandled(const QVariant& words);

        void wordErrorHandled(const QString& error);
        void wordCachedChanged();

    private slots:
        void onWordContentProcessFinished(const Word& searchedWord);
        void onWordProcessErrorFinished(const QString& errorMessage);

    private:
        auto prepareWords(const QList<Word>& words) -> QVariantList;

        WordCache* mWordCache;

        WordDao mWordDao;
        WordContentService mWordContentService;
    };
}
