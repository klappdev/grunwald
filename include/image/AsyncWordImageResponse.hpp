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

#pragma once

#include <QQuickImageProvider>

#include "cache/WordCache.hpp"
#include "net/WordImageService.hpp"

namespace grunwald {

    class AsyncWordImageResponse final : public QQuickImageResponse {
        Q_OBJECT
    public:
        AsyncWordImageResponse(WordCache* wordCache, const QString& imageId, const QSize& requestedSize);
        ~AsyncWordImageResponse();

        QQuickTextureFactory* textureFactory() const override;

    private slots:
        void onResponseFinished(const WordImage& wordImage);
        void onResponseCacheFinished(const WordImage& wordImage);
        void onResponseError(const QString& error);

    private:
        void searchWordImage(const QString& name);

        WordCache* mWordCache;
        WordImageService mWordImageService;

        QImage mImage;
        QSize mRequestedSize;
    };
}
