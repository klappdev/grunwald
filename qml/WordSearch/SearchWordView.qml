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
import QtQuick.Window 2.2
import QtQuick.Controls 2.2 as QQC2

import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Styles 1.4

import Theme 1.0

Rectangle {
    id: root
    color: Style.secondaryColor

    QQC1.TextField {
        id: searchTextEdit
        width: Window.width * 3 / 4

        anchors {
            left: parent.left
            right: searchButton.left
            top: parent.top
            bottom: parent.bottom

            leftMargin: Style.mediumSpacing
            rightMargin: Style.mediumSpacing
            topMargin: Style.bigSpacing
            bottomMargin: Style.bigSpacing
        }

        placeholderText: qsTr("Search word ...")
        font {
            family: "Roboto Regular"
            pointSize: 12
        }
        style: TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                color: "#ffffff"
                radius: 4
                border {
                    color: "lightgrey"
                    width: 1
                }
            }
        }
    }

    QQC2.Button {
        id: searchButton
        width: Window.width * 1 / 4

        anchors {
            left: searchTextEdit.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom

            leftMargin: Style.mediumSpacing
            rightMargin: Style.mediumSpacing
            topMargin: Style.bigSpacing
            bottomMargin: Style.bigSpacing
        }

        text: qsTr("Search")

        font {
            family: "Roboto Regular"
            pointSize: 12
            bold: true
        }

        contentItem: Text {
            text: searchButton.text
            font: searchButton.font
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideMiddle
        }

        background: Rectangle {
            radius: 4
            color: Style.primaryColor

            border {
                color: "lightgrey"
                width: 1
            }
        }

        onClicked: {
            console.info("Click search button: ")
        }
    }
}