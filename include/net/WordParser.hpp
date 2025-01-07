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

#include <QUrl>

#include "common/Word.hpp"
#include "util/Result.hpp"
#include "util/Error.hpp"

class QGumboNode;

namespace grunwald {
    using ParserError = Error;

    class WordParser final {
    public:
        WordParser();
        ~WordParser();

        auto parseWordContent(const QString& name, const QByteArray& remoteData) -> Result<Word, ParserError>;
        auto parseWordImage(const QByteArray& remoteData) -> Result<WordImage, ParserError>;

    private:
        auto parseRemoteWord(const QString& name, const QString& remoteData) -> Result<Word, ParserError>;
        auto parseLanguageWord(const QGumboNode& node) -> Result<QString, ParserError>;
        auto parseEtymologyWord(const QGumboNode& node) -> QString;
        auto parseTranscriptionWord(const QGumboNode& node) -> QString;
        auto parseTranslationWord(const QGumboNode& node) -> QString;
        auto parseTypeWord(const QGumboNode& node) -> QString;
        auto parseDescriptionWord(const QString& wordType, const QGumboNode& node) -> QString;
        auto parseAssociationWord(const QGumboNode& node) -> QString;
        auto parseAntonymsWord(const QGumboNode& node) -> QString;
        auto parseSynonymsWord(const QGumboNode& node) -> QString;
    };
}
