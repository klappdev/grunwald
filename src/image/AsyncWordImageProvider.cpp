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

#include "image/AsyncWordImageProvider.hpp"
#include "image/AsyncWordImageResponse.hpp"

namespace {
    constexpr const char* const TAG = "[AsyncWordImageProvider] ";
}

namespace grunwald {

    AsyncWordImageProvider::AsyncWordImageProvider(WordCache* wordCache)
        : mWordCache(wordCache) {
    }

    AsyncWordImageProvider::~AsyncWordImageProvider() {
        qDebug() << TAG << "AsyncWordImageProvider was destroyed!" << Qt::endl;
    }

    QQuickImageResponse* AsyncWordImageProvider::requestImageResponse(const QString& imageId, const QSize& requestedSize) {
        return new AsyncWordImageResponse{mWordCache, imageId, requestedSize};
    }
}
