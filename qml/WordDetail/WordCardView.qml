/*
 * Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-20245https://github.com/klappdev
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
import QtQuick.Controls 2.15

import Commons 1.0
import Theme 1.0

Rectangle {
    id: root
    color: Style.backgroundColor
    radius: 10
    border {
        color: "lightgrey"
        width: 1
    }

    property string titleCard: ""
    property string contentCard: ""

    RoundRectangle {
        id: titleRectangle
        width: parent.width
        height: parent.height * 1 / 3

        radius: 10
        topLeftCorner: true
        topRightCorner: true
        backgroundColor: "lightgrey"
        borderColor: "lightgrey"

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        Text {
            id: titleText
            text: titleCard
            color: "grey"
            font {
                pointSize: 14
                family: "Roboto Regular"
            }

            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
        }
    }

    Flickable {
        id: flickable
        contentHeight: contentText.implicitHeight
        contentWidth: contentText.implicitWidth
        //contentY: contentHeight - height
        boundsMovement: Flickable.StopAtBounds
        anchors {
            left: root.left
            right: root.right
            top: titleRectangle.bottom
            bottom: root.bottom

            leftMargin: Style.mediumSpacing
            rightMargin: Style.mediumSpacing
            topMargin: Style.mediumSpacing
            bottomMargin: Style.mediumSpacing
        }
        clip: true

        Text {
            id: contentText
            width: root.width
            height: root.height * 2 / 3
            text: contentCard
            color: Style.textColor
            font {
                pointSize: 12
                family: "Roboto Regular"
            }
            wrapMode: Text.Wrap
        }

        ScrollBar.vertical: ScrollBar {
            id: verticalScrollBar
            active: hovered || pressed
            orientation: Qt.Vertical
            position: 0.0
            parent: flickable.parent
            anchors {
                top: flickable.top
                right: flickable.right
                bottom: flickable.bottom
            }
        }

        ScrollBar.horizontal: ScrollBar {
            id: horizontalScrollBar
            active: hovered || pressed
            orientation: Qt.Horizontal
            parent: flickable.parent
            anchors {
                left: flickable.left
                right: flickable.right
                bottom: flickable.bottom
            }
        }
    }
}
