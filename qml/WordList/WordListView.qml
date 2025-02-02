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

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import grunwald.WordModel 1.0
import grunwald.WordStorage 1.0
import grunwald.Word 1.0

import Theme 1.0

ListView {
    id: wordListView

    property remoteWord selectedWord

    Component.onCompleted: {

        WordStorage.wordContentHandled.connect(function(word) {
            console.log(`Search word: ${word.name}`)

            wordModel.storeWord(word)

            wordListView.selectedWord = word
        })

        WordStorage.localWordsHandled.connect(function(wordArray) {
            console.log(`Search words: ${wordArray.length}`)

            if (wordArray.length !== 0) {
                wordModel.storeWords(wordArray)

                wordListView.selectedWord = wordArray[0]
            }
        })
    }

    spacing: 1
    section.criteria: ViewSection.FirstCharacter
    section.property: "name"

    WordModel {
        id: wordModel
    }
    model: wordModel

    delegate: WordDelegate {
        width: wordListView.width
        height: 80
    }

    highlight: Rectangle {
        color: Style.themeDefaultColor
    }
    focus: true
    onCurrentItemChanged: {
        var currentWord = model.getWord(wordListView.currentIndex)
        wordListView.selectedWord = currentWord

        console.log('Word "' + currentWord.name + '" selected')
    }
}
