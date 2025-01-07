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

#include "image/AsyncWordImageResponse.hpp"

namespace {
    constexpr const char* const TAG = "[AsyncWordImageResponse] ";
    constexpr const char* const NO_IMAGE_ID = "no_image";

    constexpr qint32 DEFAULT_IMAGE_WIDTH = 250;
    constexpr qint32 DEFAULT_IMAGE_HEIGHT = 250;
}

namespace grunwald {

    AsyncWordImageResponse::AsyncWordImageResponse(WordCache* wordCache, const QString& imageId, const QSize& requestedSize)
        : mWordCache(wordCache)
        , mRequestedSize(requestedSize) {
        if (imageId == NO_IMAGE_ID) {
            qWarning() << TAG << "Word hasn't image!" << Qt::endl;
            return;
        }

        searchWordImage(imageId);

        QObject::connect(&mWordImageService, &WordImageService::wordImageProcessed,
                         this, &AsyncWordImageResponse::onResponseCacheFinished);
        QObject::connect(&mWordImageService, &WordImageService::wordImageErrorProcessed,
                         this, &AsyncWordImageResponse::onResponseError);
    }

    AsyncWordImageResponse::~AsyncWordImageResponse() {
        qDebug() << TAG << "AsyncWordImageResponse was destroyed!" << Qt::endl;
    }

    QQuickTextureFactory* AsyncWordImageResponse::textureFactory() const {
        return QQuickTextureFactory::textureFactoryForImage(mImage);
    }

    void AsyncWordImageResponse::searchWordImage(const QString& name) {
        if (mWordCache->isValid()) {
            WordImage wordImage = mWordCache->loadWordImage();

            qInfo() << TAG << "Search word image from db success!" << Qt::endl;
            onResponseFinished(wordImage);

        } else {
            mWordImageService.fetchWordImage(name);
        }
    }

    void AsyncWordImageResponse::onResponseCacheFinished(const WordImage& wordImage) {
        qInfo() << TAG << "Search word image from network success!" << Qt::endl;

        mWordCache->storeWordImage(wordImage);

        onResponseFinished(wordImage);
    }

    void AsyncWordImageResponse::onResponseFinished(const WordImage& wordImage) {
        mImage = QImage::fromData(wordImage.data);

        if (mRequestedSize.isValid()) {
            mImage = mImage.scaled(qMin(mRequestedSize.width(), wordImage.width),
                                   qMin(mRequestedSize.height(), wordImage.height));
        } else {
            mImage = mImage.scaled(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT);
        }

        qDebug() << TAG << "Load image success!" << Qt::endl;

        emit finished();
    }

    void AsyncWordImageResponse::onResponseError(const QString& error) {
        mRequestedSize = QSize{-1, -1};
        mImage = QImage(mRequestedSize, QImage::Format::Format_Invalid);

        qWarning() << TAG << "Load image failed!" << Qt::endl;

        emit finished();
    }
}
