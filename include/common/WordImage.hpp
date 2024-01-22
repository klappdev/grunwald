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

#include <QMetaType>
#include <QDebug>
#include <QUrl>

namespace grunwald {

    struct WordImage final {
        Q_GADGET

        Q_PROPERTY(qint64 id MEMBER id)
        Q_PROPERTY(QUrl url MEMBER url)
        Q_PROPERTY(qint32 width MEMBER width)
        Q_PROPERTY(qint32 height MEMBER height)
    public:
        qint64 id{};
        QUrl url;
        qint32 width{};
        qint32 height{};
    };

    inline bool operator==(const WordImage& left, const WordImage& right) {
        return left.id == right.id && left.url == right.url &&
               left.width == right.width && left.height == right.height;
    }

    inline bool operator!=(const WordImage& left, const WordImage& right) {
        return !(left == right);
    }

    inline QDebug& operator<<(QDebug& debug, const WordImage& word) {
        debug.nospace() << "["
                        << word.id      << " ; "
                        << word.url     << " ; "
                        << word.width   << " ; "
                        << word.height  << "]\n";
        return debug.space();
    }
}
Q_DECLARE_METATYPE(grunwald::WordImage)
