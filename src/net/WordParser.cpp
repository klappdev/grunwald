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

#include "net/WordParser.hpp"

#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

static constexpr const char* const TAG = "[WordParser] ";

namespace grunwald {

    WordParser::WordParser() {
    }

    WordParser::~WordParser() {
    }

    auto WordParser::parseWordContent(const QByteArray& remoteData) -> Result<Word, ParserError> {
        qInfo() << TAG << "Remote data: " << remoteData << Qt::endl;

        QJsonParseError jsonParserError;

        const QJsonDocument jsonDocument = QJsonDocument::fromJson(remoteData, &jsonParserError);

        if (jsonParserError.error != QJsonParseError::NoError) {
            const QString errorMessage = "Json parse error: " + jsonParserError.errorString();
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        if (!jsonDocument.isObject()) {
            const QString errorMessage = "Json document is not object";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonObject jsonObject = jsonDocument.object();

        if (jsonObject.isEmpty()) {
            const QString errorMessage = "Parse json data is not correct";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonValue queryValue = jsonObject["query"];

        if (!queryValue.isObject()) {
            const QString errorMessage = "Parse json data is not correct, 'query' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonValue pagesValue = queryValue.toObject()["pages"];

        if (!pagesValue.isObject()) {
            const QString errorMessage = "Parse json data is not correct, 'pages' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonObject pagesObject = pagesValue.toObject();

        if (pagesObject.isEmpty()) {
            const QString errorMessage = "Parse json data is not correct, first 'page' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonObject firstPageObject = pagesObject.begin().value().toObject();
        const QJsonValue extractValue = firstPageObject["extract"];

        if (!extractValue.isString()) {
            const QString errorMessage = "Parse json data is not correct, 'extract' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        qInfo() << TAG << "Remote word content: " << extractValue << Qt::endl;

        return parseRemoteWord(extractValue.toString());
    }

    auto WordParser::parseWordImage(const QByteArray& remoteData) -> Result<QUrl, ParserError> {
        qInfo() << TAG << "Remote data: " << remoteData << Qt::endl;

        QJsonParseError jsonParserError;

        const QJsonDocument jsonDocument = QJsonDocument::fromJson(remoteData, &jsonParserError);

        if (jsonParserError.error != QJsonParseError::NoError) {
            const QString errorMessage = "Json parse error: " + jsonParserError.errorString();
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        if (!jsonDocument.isObject()) {
            const QString errorMessage = "Json document is not object";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonObject jsonObject = jsonDocument.object();

        if (jsonObject.isEmpty()) {
            const QString errorMessage = "Parse json data is not correct";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonValue queryValue = jsonObject["query"];

        if (!queryValue.isObject()) {
            const QString errorMessage = "Parse json data is not correct, 'query' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonValue pagesValue = queryValue.toObject()["pages"];

        if (!pagesValue.isObject()) {
            const QString errorMessage = "Parse json data is not correct, 'pages' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonObject pagesObject = pagesValue.toObject();

        if (pagesObject.isEmpty()) {
            const QString errorMessage = "Parse json data is not correct, first 'page' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonObject firstPageObject = pagesObject.begin().value().toObject();
        const QJsonValue originalValue = firstPageObject["original"];

        if (!originalValue.isObject()) {
            const QString errorMessage = "Parse json data is not correct, 'original' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        const QJsonValue urlImageValue = originalValue.toObject()["source"];

        if (!urlImageValue.isString()) {
            const QString errorMessage = "Parse json data is not correct, 'source' doesn't exists";
            qWarning() << TAG << errorMessage << Qt::endl;

            return errorMessage;
        }

        qInfo() << TAG << "Remote image url: " << urlImageValue << Qt::endl;

        return QUrl(urlImageValue.toString());
    }

    auto WordParser::parseRemoteWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseLanguageWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseEtymologyWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseTranscriptionWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseTranslationWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseDescriptionWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseAssociationWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseAntonymsWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }

    auto WordParser::parseSynonymsWord(const QString& remoteData) -> Result<Word, ParserError> {
        //FIXME: implement in future
        return {};
    }
}
