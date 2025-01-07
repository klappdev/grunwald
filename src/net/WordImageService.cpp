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

#include "net/WordImageService.hpp"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

namespace {
    constexpr const char* const TAG = "[WordImageService] ";
    const QString BASE_API_URL = "https://en.wiktionary.org";
    const QString WORD_IMAGE_API_TEMPLATE = "/w/api.php?format=json&action=%1&prop=%2&piprop=%3&redirects&continue&titles=%4";
    const QString REMOTE_SERVER_UNAVAILABLE = "Remote %1 server is not available";
}

namespace grunwald {

    WordImageService::WordImageService(QObject* parent) : QObject(parent) {
    }

    WordImageService::~WordImageService() {
    }

    void WordImageService::fetchWordImage(const QString& name) {
        if (!checkInternetConnection()) {
            const NetworkError networkError { REMOTE_SERVER_UNAVAILABLE.arg(BASE_API_URL) };
            qWarning() << TAG << networkError << Qt::endl;

            emit wordImageErrorProcessed(networkError.getMessage());
            return;
        }

        const QString prepareApiUrl = WORD_IMAGE_API_TEMPLATE.arg("query")
            .arg("pageimages")
            .arg("original")
            .arg(name);

        QNetworkRequest request;
        request.setUrl(QUrl(BASE_API_URL + prepareApiUrl));
        request.setRawHeader("Content-Type","application/json");

        qInfo() << TAG << "Prepare request: " << request.url().toString() << Qt::endl;

        QNetworkReply* reply = mNetworkManager.get(request);

        QObject::connect(reply, &QNetworkReply::finished,
                         this, &WordImageService::onWordImageUrlRequestFinished);
    }

    void WordImageService::fetchWordImageData(const WordImage& wordImage) {
        if (!checkInternetConnection()) {
            const NetworkError networkError { REMOTE_SERVER_UNAVAILABLE.arg(BASE_API_URL) };
            qWarning() << TAG << networkError << Qt::endl;

            emit wordImageErrorProcessed(networkError.getMessage());
            return;
        }

        mWordImage = wordImage;

        QNetworkRequest request;
        request.setUrl(wordImage.url);
        request.setRawHeader("Content-Type","application/json");

        qInfo() << TAG << "Prepare request: " << request.url().toString() << Qt::endl;

        QNetworkReply* reply = mNetworkManager.get(request);

        QObject::connect(reply, &QNetworkReply::finished,
                         this, &WordImageService::onWordImageDataRequestFinished);
    }

    void WordImageService::onWordImageUrlRequestFinished() {
        auto* reply = static_cast<QNetworkReply*>(sender());

        const QByteArray remoteData = reply->readAll();
        const Result<WordImage, ParserError> wordImageUrlResult = mWordParser.parseWordImage(remoteData);
        const QNetworkReply::NetworkError replyError = reply->error();

        if (replyError == QNetworkReply::NoError && wordImageUrlResult.hasValue()) {
            const int errorCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if (errorCode >= 200 && errorCode < 300 && !wordImageUrlResult->url.isEmpty()) {
                fetchWordImageData(wordImageUrlResult.value());
            }
        } else {
            if (replyError == QNetworkReply::ContentNotFoundError ||
                replyError == QNetworkReply::ContentAccessDenied ||
                replyError == QNetworkReply::ProtocolInvalidOperationError) {
                emit wordImageErrorProcessed(reply->errorString() + ", " + wordImageUrlResult.error().getMessage());
            }
        }

        reply->close();
        reply->deleteLater();
    }

    void WordImageService::onWordImageDataRequestFinished() {
        auto* reply = static_cast<QNetworkReply*>(sender());

        const QByteArray imageData = reply->readAll();
        const QNetworkReply::NetworkError replyError = reply->error();

        if (replyError == QNetworkReply::NoError) {
            const int errorCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            if (errorCode >= 200 && errorCode < 300 && !imageData.isEmpty()) {
                mWordImage.data = imageData;

                emit wordImageProcessed(mWordImage);
            }
        } else {
            if (replyError == QNetworkReply::ContentNotFoundError ||
                replyError == QNetworkReply::ContentAccessDenied ||
                replyError == QNetworkReply::ProtocolInvalidOperationError) {
                emit wordImageErrorProcessed(reply->errorString());
            }
        }

        reply->close();
        reply->deleteLater();
    }

    bool WordImageService::checkInternetConnection() {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        QEventLoop eventLoop;
        QNetworkRequest request(QUrl(QStringLiteral("http://www.google.com")));

        QNetworkReply* reply = mNetworkManager.get(request);

        QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        const bool result = reply->bytesAvailable();

        reply->close();
        reply->deleteLater();

        return result;
#else
        return mNetworkConfigurationManager.isOnline();
#endif
    }
}
