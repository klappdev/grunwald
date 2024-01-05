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

    property string nameWord
    property string transcriptionWord

    Text {
        id: nameText
        text: nameWord
        color: Style.primaryColor
        font {
            pointSize: 18
            family: "Roboto Regular"
        }
    }

    Text {
        id: transcriptionText
        text: transcriptionWord
        color: Style.textColor
        font {
            pointSize: 13
            family: "Roboto Regular"
        }
    }

    Row {
        spacing: Style.tinyOffset

        Text {
            id: audioText
            text: "Audio: "
            color: Style.textColor
            font {
                pointSize: 13
                family: "Roboto Regular"
            }
        }

        Image {
            id: audioImage

            width: 24
            height: 24

            source: Style.isDarkTheme ? "qrc:/res/image/speak_inverted.png"
                                      : "qrc:/res/image/speak.png"
        }
    }
}
