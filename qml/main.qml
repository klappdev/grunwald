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

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import WordSearch 1.0
import WordList 1.0
import WordDetail 1.0
import Theme 1.0

ApplicationWindow {
    visible: true
    width: 1080
    height: 840
    title: qsTr("Grunwald")
    color: Style.backgroundColor

    SearchWordView {
        id: searchWordView
        width: parent.width
        height: parent.height / 8

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
    }

    WordListView {
        id: wordListView
        width: Window.width * 1 / 3
        height: parent.height * 7 / 8

        anchors {
            left: parent.left
            right: wordDetailView.left
            top: searchWordView.bottom
            bottom: parent.bottom

            margins: 2
        }
        onSelectedWordChanged: {
            wordDetailView.nameWord = selectedWord.name
            wordDetailView.translationWord = selectedWord.translation
            wordDetailView.transcriptionWord = selectedWord.transcription
            wordDetailView.associationWord = selectedWord.association
            wordDetailView.etymologyWord = selectedWord.etymology
            wordDetailView.descriptionWord = selectedWord.description

            console.log(`Selected word changed: ${selectedWord.name}`)
        }
    }

    WordDetailView {
        id: wordDetailView
        width: parent.width * 2 / 3
        height: parent.height * 7 / 8

        anchors {
            left: wordListView.right
            right: parent.right
            top: searchWordView.bottom
            bottom: parent.bottom

            margins: 2
        }
    }
}
