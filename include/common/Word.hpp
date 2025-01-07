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
#include <QString>
#include <QDateTime>

#include "WordType.hpp"
#include "WordImage.hpp"

namespace grunwald {

    struct Word final {
        Q_GADGET

        Q_PROPERTY(qint64 id MEMBER id)
        Q_PROPERTY(QString name MEMBER name)
        Q_PROPERTY(QString transcription MEMBER transcription)
        Q_PROPERTY(QString translation MEMBER translation)
        Q_PROPERTY(QString association MEMBER association)
        Q_PROPERTY(QString etymology MEMBER etymology)
        Q_PROPERTY(QString description MEMBER description)
        Q_PROPERTY(WordType type MEMBER type)
        Q_PROPERTY(WordImage image MEMBER image)
        Q_PROPERTY(QDateTime date MEMBER date)
    public:
        qint64 id {};
        QString name;
        QString transcription;
        QString translation;
        QString association;
        QString etymology;
        QString description;
        WordType type;
        WordImage image;
        QDateTime date;

        Word& operator=(const Word&) = default;

        Q_INVOKABLE bool hasImage() const {
            return type == WordType::Noun;
        }
    };

    inline bool operator==(const Word& left, const Word& right) {
        return left.id == right.id && left.name == right.name &&
               left.transcription == right.transcription && left.translation == right.translation &&
               left.association == right.association && left.etymology == right.etymology &&
               left.description == right.description && left.type == right.type &&
               left.image == right.image && left.date == right.date;
    }

    inline bool operator!=(const Word& left, const Word& right) {
        return !(left == right);
    }

    inline QDebug& operator<<(QDebug& debug, const Word& word) {
        debug.nospace() << "["
                        << word.id              << " ; "
                        << word.name            << " ; "
                        << word.transcription   << " ; "
                        << word.translation     << " ; "
                        << word.association     << " ; "
                        << word.etymology       << " ; "
                        << word.description     << " ; "
                        << word.type            << " ; "
                        << word.image           << " ; "
                        << word.date            << "]\n";
        return debug.space();
    }
}
Q_DECLARE_METATYPE(grunwald::Word)
