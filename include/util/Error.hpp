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

#include <QDebug>
#include <QString>

namespace grunwald {

    class Error final {
    public:
        explicit Error(const QString& message) : Error(message, -1) {}
        Error(const QString& message, qint32 code) : mMessage(message), mCode(code) {}
        ~Error() = default;

        auto getMessage() const -> QString { return mMessage; }
        auto getCode() const -> qint32 { return mCode; }

    private:
        QString mMessage;
        qint32 mCode;
    };

    inline QDebug& operator<<(QDebug& debug, const Error& error) {
        debug.nospace() << "Error {";

        if (error.getCode() != -1) {
            debug << error.getMessage() << "," << error.getCode();
        } else {
            debug << error.getMessage();
        }

        debug << "}" << Qt::endl;

        return debug.space();
    }
}
