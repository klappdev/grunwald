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

import Theme 1.0

Column {
    id: root
    spacing: Style.mediumOffset

    property string translationWord: ""
    property string associationWord: ""
    property string etymologyWord: ""
    property string descriptionWord: ""

    WordCardView {
        id: translationWordCard
        width: parent.width
        height: parent.height * 1 / 4 - 10

        titleCard: qsTr("Translation")
        contentCard: translationWord
    }

    WordCardView {
        id: associationWordCard
        width: parent.width
        height: parent.height * 1 / 4 - 10

        titleCard: qsTr("Association")
        contentCard: associationWord
    }

    WordCardView {
        id: etymologyWordCard
        width: parent.width
        height: parent.height * 1 / 4 - 10

        titleCard: qsTr("Etymology")
        contentCard: etymologyWord
    }

    WordCardView {
        id: descriptionCard
        width: parent.width
        height: parent.height * 1 / 4 - 10

        titleCard: qsTr("Description")
        contentCard: descriptionWord
    }
}

