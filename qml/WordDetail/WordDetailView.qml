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

import QtQuick 2.9
//import QtQuick.Window 2.2
//import QtQuick.Controls 2.2

import Theme 1.0

Rectangle {
    id: root
    color: Style.backgroundColor
    border {
        color: "lightgrey"
        width: 0.5
    }

    WordDetailHeaderView {
        id: wordDetailHeaderView
        width: parent.width
        height: parent.height * 1 / 8
        nameWord: "Katze"                                //FIXME: use from model item
        transcriptionWord: "Transcription: " + " /ˈkatsə/" //FIXME: use from model item

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top

            margins: Style.mediumSpacing
        }
    }

    WordDetailActionView {
        id: wordDetailActionView
        width: parent.width
        //height: parent.height * 1 / 8

        anchors {
            left: parent.left
            right: parent.right
            top: wordDetailHeaderView.bottom

            margins: Style.mediumSpacing
        }
    }

    WordDetailContentView {
        id: wordDetailContentView
        width: parent.width
        //height: parent.height * 6 / 8

        translationWord: "cat"                              //FIXME: use from model item
        associationWord: "Katze f (genitive Katze, plural Katzen, diminutive Kätzchen n or Kätzlein n, masculine männliche Katze or Kater, feminine weibliche Katze or Kätzin or Katerin)"                              //FIXME: use from model item
        etymologyWord: "From Middle High German katze, Old High German kazza, from Proto-West Germanic *kattā, from Late Latin catta, feminine of cattus. Akin to Old English catt (“cat”)."                                  //FIXME: use from model item
        descriptionWord: "Katze is the common term to refer to a cat (both male and female ones). The derived form Kätzin is mostly restricted to poetic language and technical language."                                //FIXME: use from model item

        anchors {
            left: parent.left
            right: parent.right
            top: wordDetailActionView.bottom
            bottom: parent.bottom

            margins: Style.mediumSpacing
        }
    }
}
