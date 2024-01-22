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

#include <QtVersionChecks>

#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
#include <QNetworkConfigurationManager>
#endif
#include <QNetworkAccessManager>
#include <QUrl>

#include "net/WordParser.hpp"
#include "common/Word.hpp"
#include "util/Error.hpp"

namespace grunwald {
    using NetworkError = Error;

    class DictionaryService final : public QObject {
        Q_OBJECT
    public:
        DictionaryService(QObject* parent = nullptr);
        ~DictionaryService();

        void getWordContent(const QString& name);
        void getWordImage(const QString& name);

    signals:
        void wordContentProcessed(const Word&);
        void wordImageProcessed(const QUrl&);

        void wordProcessedError(const NetworkError&);

    private slots:
        void handleWordContentRequest();
        void handleWordImageRequest();

    private:
        bool checkInternetConnection();

        QNetworkAccessManager mNetworkManager;
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
        QNetworkConfigurationManager mNetworkConfigurationManager;
#endif
        WordParser mWordParser;
        QString mWordName;
    };
}
