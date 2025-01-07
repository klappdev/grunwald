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

#include "util/EnumHelper.hpp"

using namespace Qt::Literals::StringLiterals;

namespace {
    constexpr const char* const TAG = "[WordParser] ";
    constexpr const char* const DEFAULT_LANGUAGE = "German";  //FIXME: better to use constant or load from settings
}

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

    auto WordParser::parseWordImage(const QByteArray& remoteData) -> Result<WordImage, ParserError> {
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

        qDebug() << TAG << "Remote image url: " << urlImageValue << Qt::endl;

        const QJsonValue widthImageValue = originalValue.toObject()["width"];

        if (!widthImageValue.isDouble()) {
            const ParserError error { "Parse json data is not correct, 'width' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        const QJsonValue heightImageValue = originalValue.toObject()["height"];

        if (!heightImageValue.isDouble()) {
            const ParserError error { "Parse json data is not correct, 'height' doesn't exists" };
            qWarning() << TAG << error << Qt::endl;

            return error;
        }

        return WordImage{0, QUrl(urlImageValue.toString()), static_cast<qint32>(widthImageValue.toDouble()), static_cast<qint32>(heightImageValue.toDouble())};
    }

    auto WordParser::parseRemoteWord(const QString& name, const QString& remoteData) -> Result<Word, ParserError> {
        auto document = QGumboDocument::parse(remoteData);
        auto rootNode = document.rootNode();

        qDebug() << TAG << "Start parse word" << Qt::endl;

        const auto languageText = parseLanguageWord(rootNode);

        if (languageText.hasError()) {
            return languageText.error();
        } else if (languageText.value() != DEFAULT_LANGUAGE) {
            const auto detectLanguage = languageText.value();
            return ParserError{ QString("Detect language %1 is not found. Found language %2").arg(DEFAULT_LANGUAGE).arg(detectLanguage) };
        }

        const auto etymologyText = parseEtymologyWord(rootNode);
        const auto transcriptionText = parseTranscriptionWord(rootNode);
        const auto translationText = parseTranslationWord(rootNode);

        const auto wordTypeText = parseTypeWord(rootNode);
        const auto wordTypeOption = EnumHelper::fromString<WordType>(wordTypeText);
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
        //Search: h2[data-mw-anchor*='German']
        const QGumboNodes h2Nodes = node.getElementsByTagName(HtmlTag::H2);

        for (const QGumboNode& h2Node : h2Nodes) {
            if (!h2Node.hasAttribute(u"data-mw-anchor"_s) || h2Node.getAttribute(u"data-mw-anchor"_s) != DEFAULT_LANGUAGE) {
                continue;
            }

            return h2Node.innerText();
        }

        return ParserError{ QString("Parse language %1 is not found").arg(DEFAULT_LANGUAGE) };
    }

    auto WordParser::parseEtymologyWord(const QGumboNode& node) -> QString {
        //Search: h3[data-mw-anchor*='Etymology'], p
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            if (!h3Node.hasAttribute(u"data-mw-anchor"_s) || h3Node.getAttribute(u"data-mw-anchor"_s) != u"Etymology"_s) {
                continue;
            }

            const QGumboNode etymologyContentNode = h3Node.getParentNextNode();

            if (etymologyContentNode && etymologyContentNode.tagName() == u"p"_s) {
                const QString etymologyText = etymologyContentNode.outerHtml();

                qDebug() << TAG << "Etymology: " << etymologyText << Qt::endl;

                return etymologyText;
            } else {
                qWarning() << TAG << "Parse etymology is not correct, content doesn't exists" << Qt::endl;
            }
        }

        return u""_s;
    }

    auto WordParser::parseTranscriptionWord(const QGumboNode& node) -> QString {
        //Search: h3[data-mw-anchor*='Pronunciation'], ul > li
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            if (!h3Node.hasAttribute(u"data-mw-anchor"_s) || h3Node.getAttribute(u"data-mw-anchor"_s) != u"Pronunciation"_s) {
                continue;
            }

            const QGumboNode transcriptionContentNode = h3Node.getParentNextNode();

            if (!transcriptionContentNode && transcriptionContentNode.tagName() != u"ul"_s) {
                qWarning() << TAG << "Parse transcription is not correct, 'h3[data-mw-anchor], ul' doesn't exists" << Qt::endl;
                continue;
            }

            const QGumboNodes liNodes = transcriptionContentNode.getElementsByTagName(HtmlTag::LI);

            if (!liNodes.empty()) {
                const QString transcriptionText = liNodes.front().outerHtml();

                qDebug() << TAG << "Transcription: " << transcriptionText << Qt::endl;

                return transcriptionText;
            } else {
                qWarning() << TAG << "Parse transcription is not correct, 'h3[data-mw-anchor], ul > li' doesn't exists" << Qt::endl;
            }
        }

        return u""_s;
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

        return u""_s;
    }

    auto WordParser::parseTypeWord(const QGumboNode& node) -> QString {
        //Search: h3[data-mw-anchor*='$partOfSpeech']

        const QMetaEnum wordTypeEnum = QMetaEnum::fromType<WordType>();
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            for (int i = 0; i < wordTypeEnum.keyCount(); ++i) {
                const QString wordType = QString::fromUtf8(wordTypeEnum.key(i));

                if (!h3Node.hasAttribute(u"data-mw-anchor"_s) || h3Node.getAttribute(u"data-mw-anchor"_s) != wordType) {
                    continue;
                }

                qDebug() << TAG << "Type word: " << wordType << Qt::endl;

                return wordType;
            }
        }

        return u""_s;
    }

    auto WordParser::parseDescriptionWord(const QString& wordType, const QGumboNode& node) -> QString {
        //Search: h3[data-mw-anchor*='$partOfSpeech'], p
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            if (!h3Node.hasAttribute(u"data-mw-anchor"_s) || h3Node.getAttribute(u"data-mw-anchor"_s) != wordType) {
                continue;
            }

            const QGumboNode descriptionContentNode = h3Node.getParentNextNode();

            if (descriptionContentNode && descriptionContentNode.tagName() == u"p"_s) {
                const QString descriptionText = descriptionContentNode.outerHtml();

                qDebug() << TAG << "Description: " << descriptionText << Qt::endl;

                return descriptionText;
            }
        }

        return u""_s;
    }

    auto WordParser::parseAssociationWord(const QGumboNode& node) -> QString {
        QString result;

        result += parseAntonymsWord(node);
        result += parseSynonymsWord(node);

        return result;
    }

    auto WordParser::parseAntonymsWord(const QGumboNode& node) -> QString {
        //Search: h4[data-mw-anchor*='Antonyms'], p
        const QGumboNodes h4Nodes = node.getElementsByTagName(HtmlTag::H4);

        for (const QGumboNode& h4Node : h4Nodes) {
            if (!h4Node.hasAttribute(u"data-mw-anchor"_s) || h4Node.getAttribute(u"data-mw-anchor"_s) != u"Antonyms"_s) {
                continue;
            }

            const QGumboNode antonymsContentNode = h4Node.getParentNextNode();

            if (antonymsContentNode && antonymsContentNode.tagName() == u"p"_s) {
                const QString antonymsText = antonymsContentNode.outerHtml();

                qDebug() << TAG << "Antonyms: " << antonymsText << Qt::endl;

                return antonymsText;
            }
        }

        return u""_s;
    }

    auto WordParser::parseSynonymsWord(const QGumboNode& node) -> QString {
        QString synonymsText;

        //Search: h3[data-mw-anchor*='Synonyms'], ul
        const QGumboNodes h3Nodes = node.getElementsByTagName(HtmlTag::H3);

        for (const QGumboNode& h3Node : h3Nodes) {
            if (!h3Node.hasAttribute(u"data-mw-anchor"_s) || h3Node.getAttribute(u"data-mw-anchor"_s) != u"Synonyms"_s) {
                continue;
            }

            const QGumboNode synonymsContentNode = h3Node.getParentNextNode();

            if (synonymsContentNode && synonymsContentNode.tagName() == u"ul"_s) {
                const QString h3SynonymsText = synonymsContentNode.outerHtml();

                qDebug() << TAG << "Synonyms, h3: " << h3SynonymsText << Qt::endl;

                synonymsText += h3SynonymsText;
            }
        }

        //Search: h4[data-mw-anchor*='Synonyms'], ul
        const QGumboNodes h4Nodes = node.getElementsByTagName(HtmlTag::H4);

        for (const QGumboNode& h4Node : h4Nodes) {
            if (!h4Node.hasAttribute(u"data-mw-anchor"_s) || h4Node.getAttribute(u"data-mw-anchor"_s) != u"Synonyms"_s) {
                continue;
            }

            const QGumboNode synonymsContentNode = h4Node.getParentNextNode();

            if (synonymsContentNode && synonymsContentNode.tagName() == u"ul"_s) {
                const QString h4SynonymsText = synonymsContentNode.outerHtml();

                qDebug() << TAG << "Synonyms, h4: " << h4SynonymsText << Qt::endl;

                synonymsText += h4SynonymsText;
            }
        }

        return synonymsText;
    }
}
