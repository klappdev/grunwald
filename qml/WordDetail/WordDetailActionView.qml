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
import QtQuick.Controls 2.2

import Theme 1.0

Row {
    id: root
    spacing: Style.mediumOffset
    anchors.margins: Style.mediumOffset

    RoundButton {
        id: saveButton
        width: 100
        height: 35

        text: qsTr("Save")
        icon.source: Style.isDarkTheme ? "qrc:/res/image/star_inverted.png"
                                       : "qrc:/res/image/star.png"
        font {
            family: "Roboto Regular"
            pointSize: 12
            bold: true
        }

        contentItem: Item {
            Row {
                spacing: 5

                Image {
                    source: saveButton.icon.source
                    width: saveButton.icon.width
                    height: saveButton.icon.height
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: saveButton.text
                    font: saveButton.font
                    color: "lightgrey"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        background: Rectangle {
            height: parent.height
            radius: 20
            color: Style.themeDefaultColor

            border {
                color: "lightgrey"
                width: 2
            }
        }

        onClicked: {
            console.info("Click save button: ")
        }
    }

    RoundButton {
        id: shareButton
        width: 100
        height: 35

        text: qsTr("Share")
        icon.source: Style.isDarkTheme ? "qrc:/res/image/share_inverted.png"
                                       : "qrc:/res/image/share.png"
        font {
            family: "Roboto Regular"
            pointSize: 12
            bold: true
        }

        contentItem: Item {
            Row {
                spacing: 5

                Image {
                    source: shareButton.icon.source
                    width: shareButton.icon.width
                    height: shareButton.icon.height
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: shareButton.text
                    font: shareButton.font
                    color: "lightgrey"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        background: Rectangle {
            height: parent.height
            radius: 20
            color: Style.themeDefaultColor

            border {
                color: "lightgrey"
                width: 2
            }
        }

        onClicked: {
            console.info("Click share button: ")
        }
    }
}
