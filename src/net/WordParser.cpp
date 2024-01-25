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

#if __has_include(<qgumbonode.h>)
# include <qgumbodocument.h>
# include <qgumbonode.h>
#else
# error "Couldn't find QGumboParser header files"
#endif

#include "util/EnumUtil.hpp"

static constexpr const char* const TAG = "[WordParser] ";
static constexpr const char* const DEFAULT_LANGUAGE = "German";  //FIXME: better to use constant or load from settings

namespace grunwald {

    WordParser::WordParser() {
    }

    WordParser::~WordParser() {
    }

    auto WordParser::parseWordContent(const QString& name, const QByteArray& remoteData) -> Result<Word, ParserError> {
        //qInfo() << TAG << "Parse word: " << name << ", remote data: " << remoteData << Qt::endl;

        QJsonParseError jsonParserError;

        const QJsonDocument jsonDocument = QJsonDocument::fromJson(remoteData, &jsonParserError);

        if (jsonParserError.error != QJsonParseError::NoError) {
            const ParserError error { "Json parse error: " + jsonParserError.errorString() };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        if (!jsonDocument.isObject()) {
            const ParserError error { "Json document is not object" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonObject jsonObject = jsonDocument.object();

        if (jsonObject.isEmpty()) {
            const ParserError error { "Parse json data is not correct" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonValue queryValue = jsonObject["query"];

        if (!queryValue.isObject()) {
            const ParserError error { "Parse json data is not correct, 'query' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonValue pagesValue = queryValue.toObject()["pages"];

        if (!pagesValue.isObject()) {
            const ParserError error { "Parse json data is not correct, 'pages' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonObject pagesObject = pagesValue.toObject();

        if (pagesObject.isEmpty()) {
            const ParserError error { "Parse json data is not correct, first 'page' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonObject firstPageObject = pagesObject.begin().value().toObject();
        const QJsonValue extractValue = firstPageObject["extract"];

        if (!extractValue.isString()) {
            const ParserError error { "Parse json data is not correct, 'extract' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        qInfo() << TAG << "Remote word content: " << extractValue << Qt::endl;

        return parseRemoteWord(name, extractValue.toString());
    }

    auto WordParser::parseWordImage(const QByteArray& remoteData) -> Result<QUrl, ParserError> {
        qInfo() << TAG << "Remote data: " << remoteData << Qt::endl;

        QJsonParseError jsonParserError;

        const QJsonDocument jsonDocument = QJsonDocument::fromJson(remoteData, &jsonParserError);

        if (jsonParserError.error != QJsonParseError::NoError) {
            const ParserError error { "Json parse error: " + jsonParserError.errorString() };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        if (!jsonDocument.isObject()) {
            const ParserError error { "Json document is not object" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonObject jsonObject = jsonDocument.object();

        if (jsonObject.isEmpty()) {
            const ParserError error { "Parse json data is not correct" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonValue queryValue = jsonObject["query"];

        if (!queryValue.isObject()) {
            const ParserError error { "Parse json data is not correct, 'query' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonValue pagesValue = queryValue.toObject()["pages"];

        if (!pagesValue.isObject()) {
            const ParserError error { "Parse json data is not correct, 'pages' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonObject pagesObject = pagesValue.toObject();

        if (pagesObject.isEmpty()) {
            const ParserError error { "Parse json data is not correct, first 'page' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonObject firstPageObject = pagesObject.begin().value().toObject();
        const QJsonValue originalValue = firstPageObject["original"];

        if (!originalValue.isObject()) {
            const ParserError error { "Parse json data is not correct, 'original' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonValue urlImageValue = originalValue.toObject()["source"];

        if (!urlImageValue.isString()) {
            const ParserError error { "Parse json data is not correct, 'source' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        qInfo() << TAG << "Remote image url: " << urlImageValue << Qt::endl;

        return QUrl(urlImageValue.toString());
    }

    auto WordParser::parseRemoteWord(const QString& name, const QString& remoteData) -> Result<Word, ParserError> {
        auto document = QGumboDocument::parse(remoteData);
        auto rootNode = document.rootNode();

        qDebug() << TAG << "Start parse word" << Qt::endl;

        const auto languageText = parseLanguageWord(rootNode);

        if (languageText.hasError() || languageText.value() != DEFAULT_LANGUAGE) {
            return languageText.error();
        }

        const auto etymologyText = parseEtymologyWord(rootNode);
        const auto transcriptionText = parseTranscriptionWord(rootNode);
        const auto translationText = parseTranslationWord(rootNode);

        const auto wordTypeText = parseTypeWord(rootNode);
        const auto wordTypeOption = EnumUtil::fromString<WordType>(wordTypeText);
        const auto wordType = wordTypeOption ? *wordTypeOption : WordType::Unknown;

        QString descriptionText;
        if (wordTypeOption) {
            descriptionText = parseDescriptionWord(wordTypeText, rootNode);
        }
        const auto associationText = parseAssociationWord(rootNode);

        qDebug() << TAG << "End parse word" << Qt::endl;

        return Word{ 1, name, transcriptionText, translationText, associationText,
                    etymologyText, descriptionText, wordType, WordImage{},
                    QDateTime::currentDateTime() };
    }

    auto WordParser::parseLanguageWord(const QGumboNode& node) -> Result<QString, ParserError> {
        //Search: h2 > span[id*='German']
        const QGumboNodes h2Nodes = node.getElementsByTagName(HtmlTag::H2);

        for (const QGumboNode& h2Node : h2Nodes) {
            const QGumboNodes spanNodes = h2Node.getElementsByTagName(HtmlTag::SPAN);

            if (!spanNodes.empty()) {
                const QGumboNode spanNode = spanNodes.front();
                const QGumboNodes languageNodes = spanNode.getElementById(DEFAULT_LANGUAGE);

                if (!languageNodes.empty()) {
                    return languageNodes.front().innerText();
                } else {
                    qWarning() << TAG << "Parse language is not correct, 'h2 > span[id*='" << DEFAULT_LANGUAGE << "']' doesn't exists" << Qt::endl;
                }
            } else {
                qWarning() << TAG << "Parse language is not correct, 'h2 > span' doesn't exists" << Qt::endl;
            }
        }

        return ParserError{ QString("Parse language %1 is not found").arg(DEFAULT_LANGUAGE) };
    }

    auto WordParser::parseEtymologyWord(const QGumboNode& node) -> QString {
        //Search: h3 > span[id*='Etymology'], p
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            const QGumboNodes spanNodes = h3Node.getElementsByTagName(HtmlTag::SPAN);

            if (spanNodes.empty()) {
                qWarning() << TAG << "Parse etymology is not correct, 'h3 > span' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNode spanNode = spanNodes.front();
            const QGumboNodes etymologyNodes = spanNode.getElementById("Etymology");

            if (!etymologyNodes.empty()) {
                continue;
            }

            const QGumboNode etymologyContentNode = const_cast<QGumboNode&>(h3Node).getParentNextNode();

            if (etymologyContentNode && etymologyContentNode.tagName() == QStringLiteral("p")) {
                const QString etymologyText = etymologyContentNode.outerHtml();

                qDebug() << TAG << "Etymology: " << etymologyText << Qt::endl;

                return etymologyText; //FIXME: delete html tag from text
            } else {
                qWarning() << TAG << "Parse etymology is not correct, content doesn't exists" << Qt::endl;
            }
        }

        return QStringLiteral("");
    }

    auto WordParser::parseTranscriptionWord(const QGumboNode& node) -> QString {
        //Search: h3 > span[id*='Pronunciation'], ul > li
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            const QGumboNodes spanNodes = h3Node.getElementsByTagName(HtmlTag::SPAN);

            if (spanNodes.empty()) {
                qWarning() << TAG << "Parse transcription is not correct, 'h3 > span' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNode spanNode = spanNodes.front();
            const QGumboNodes transcriptionNodes = spanNode.getElementById("Pronunciation");

            if (transcriptionNodes.empty()) {
                continue;
            }

            const QGumboNode transcriptionContentNode = const_cast<QGumboNode&>(h3Node).getParentNextNode();

            if (!transcriptionContentNode && transcriptionContentNode.tagName() != QStringLiteral("ul")) {
                qWarning() << TAG << "Parse transcription is not correct, 'h3 > span, ul' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNodes liNodes = transcriptionContentNode.getElementsByTagName(HtmlTag::LI);

            if (!liNodes.empty()) {
                const QString transcriptionText = liNodes.front().outerHtml();

                qDebug() << TAG << "Transcription: " << transcriptionText << Qt::endl;

                return transcriptionText; //FIXME: delete html tag from text
            } else {
                qWarning() << TAG << "Parse transcription is not correct, 'h3 > span, ul > li' doesn't exists" << Qt::endl;
            }
        }

        return QStringLiteral("");
    }

    auto WordParser::parseTranslationWord(const QGumboNode& node) -> QString {
        //Search: ol > li
        const QGumboNodes olNodes = node.getElementsByTagName(HtmlTag::OL);

        for (const QGumboNode& olNode : olNodes) {
            const QGumboNodes liNodes = olNode.getElementsByTagName(HtmlTag::LI);

            if (liNodes.empty()) {
                qWarning() << TAG << "Parse translation is not correct, 'ol > li' doesn't exists" << Qt::endl;
                continue;
            }

            const QString translationText = liNodes.front().outerHtml();

            qDebug() << TAG << "Translation: " << translationText << Qt::endl;

            return translationText;
        }

        return QStringLiteral("");
    }

    auto WordParser::parseTypeWord(const QGumboNode& node) -> QString {
        //Search: h3 > span[id*='$partOfSpeech']

        const QMetaEnum wordTypeEnum = QMetaEnum::fromType<WordType>();

        for (int i = 0; i < wordTypeEnum.keyCount(); ++i) {
            const QString wordType = QString::fromUtf8(wordTypeEnum.key(i));
                const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

            for (const QGumboNode& h3Node : h3Nodes) {
                const QGumboNodes spanNodes = h3Node.getElementsByTagName(HtmlTag::SPAN);

                if (spanNodes.empty()) {
                    qWarning() << TAG << "Parse partOfSpeach is not correct, 'h3 > span' doesn't exists" << Qt::endl;
                    continue;
                }

                const QGumboNode spanNode = spanNodes.front();
                const QGumboNodes wordTypeNodes = spanNode.getElementById(wordType);

                if (wordTypeNodes.empty()) {
                    //qWarning() << TAG << "Parse partOfSpeach is not correct, 'h3 > span[id*='" << wordType << "']' doesn't exists" << Qt::endl;
                    continue;
                }

                qDebug() << TAG << "Type word: " << wordType << Qt::endl;

                return wordType;
            }
        }

        return QStringLiteral("");
    }

    auto WordParser::parseDescriptionWord(const QString& wordType, const QGumboNode& node) -> QString {
        //Search: h3 > span[id*='$partOfSpeech'], p
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            const QGumboNodes spanNodes = h3Node.getElementsByTagName(HtmlTag::SPAN);

            if (spanNodes.empty()) {
                qWarning() << TAG << "Parse description is not correct, 'h3 > span' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNode spanNode = spanNodes.front();
            const QGumboNodes descriptionNodes = spanNode.getElementById(wordType);

            if (!descriptionNodes.empty()) {
                continue;
            }

            const QGumboNode descriptionContentNode = const_cast<QGumboNode&>(h3Node).getParentNextNode();

            if (descriptionContentNode && descriptionContentNode.tagName() == QStringLiteral("p")) {
                const QString descriptionText = descriptionContentNode.outerHtml();

                qDebug() << TAG << "Description: " << descriptionText << Qt::endl;

                return descriptionText; //FIXME: delete html tag from text
            }
        }

        return QStringLiteral("");
    }

    auto WordParser::parseAssociationWord(const QGumboNode& node) -> QString {
        QString result;

        result += parseAntonymsWord(node);
        result += parseSynonymsWord(node);

        return result;
    }

    auto WordParser::parseAntonymsWord(const QGumboNode& node) -> QString {
        //Search: h4 > span[id*='Antonyms'], p
        const QGumboNodes h4Nodes = node.getElementsByTagName(HtmlTag::H4);

        for (const QGumboNode& h4Node : h4Nodes) {
            const QGumboNodes spanNodes = h4Node.getElementsByTagName(HtmlTag::SPAN);

            if (spanNodes.empty()) {
                qWarning() << TAG << "Parse antonyms is not correct, 'h3 > span' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNode spanNode = spanNodes.front();
            const QGumboNodes antonymsNodes = spanNode.getElementById("Antonyms");

            if (!antonymsNodes.empty()) {
                continue;
            }

            const QGumboNode antonymsContentNode = const_cast<QGumboNode&>(h4Node).getParentNextNode();

            if (antonymsContentNode && antonymsContentNode.tagName() == QStringLiteral("p")) {
                const QString antonymsText = antonymsContentNode.outerHtml();

                qDebug() << TAG << "Antonyms: " << antonymsText << Qt::endl;

                return antonymsText; //FIXME: delete html tag from text
            }
        }

        return QStringLiteral("");
    }

    auto WordParser::parseSynonymsWord(const QGumboNode& node) -> QString {
        QString synonymsText;

        //Search: h3 > span[id*='Synonyms'], ul
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            const QGumboNodes spanNodes = h3Node.getElementsByTagName(HtmlTag::SPAN);

            if (spanNodes.empty()) {
                qWarning() << TAG << "Parse synonyms is not correct, 'h3 > span' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNode spanNode = spanNodes.front();
            const QGumboNodes synonymsNodes = spanNode.getElementById("Synonyms");

            if (!synonymsNodes.empty()) {
                continue;
            }

            const QGumboNode synonymsContentNode = const_cast<QGumboNode&>(h3Node).getParentNextNode();

            if (synonymsContentNode && synonymsContentNode.tagName() == QStringLiteral("ul")) {
                const QString h3SynonymsText = synonymsContentNode.outerHtml();

                qDebug() << TAG << "Synonyms, h3: " << h3SynonymsText << Qt::endl;

                synonymsText += h3SynonymsText; //FIXME: delete html tag from text
            }
        }

        //Search: h4 > span[id*='Synonyms'], ul
        const QGumboNodes h4Nodes = node.getElementsByTagName(HtmlTag::H4);

        for (const QGumboNode& h4Node : h4Nodes) {
            const QGumboNodes spanNodes = h4Node.getElementsByTagName(HtmlTag::SPAN);

            if (spanNodes.empty()) {
                qWarning() << TAG << "Parse synonyms is not correct, 'h4 > span' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNode spanNode = spanNodes.front();
            const QGumboNodes synonymsNodes = spanNode.getElementById("Synonyms");

            if (!synonymsNodes.empty()) {
                continue;
            }

            const QGumboNode synonymsContentNode = const_cast<QGumboNode&>(h4Node).getParentNextNode();

            if (synonymsContentNode && synonymsContentNode.tagName() == QStringLiteral("ul")) {
                const QString h4SynonymsText = synonymsContentNode.outerHtml();

                qDebug() << TAG << "Synonyms, h4: " << h4SynonymsText << Qt::endl;

                synonymsText += h4SynonymsText; //FIXME: delete html tag from text
            }
        }

        return synonymsText;
    }
}
