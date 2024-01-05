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

import QtQuick 2.11

Item {
    id: root

    property int radius: 5

    property bool topLeftCorner: false
    property bool topRightCorner: false
    property bool bottomRightCorner: false
    property bool bottomLeftCorner: false

    property color backgroundColor: "white"
    property color borderColor: "black"
    property int borderWidth: 1

    Canvas {
        id: paintCanvas
        anchors.fill: parent

        onPaint: {
            var context = getContext("2d");
            context.reset()
            context.beginPath();

            // Start position
            context.moveTo(0, height / 2)

            if (topLeftCorner){
                context.lineTo(0, radius)
                context.arcTo(0, 0, radius, 0, radius);
            } else {
                context.lineTo(0, 0)
            }

            if (topRightCorner){
                context.lineTo(width - radius, 0)
                context.arcTo(width, 0, width, radius, radius)
            } else {
                context.lineTo(width, 0)
            }

            if (bottomRightCorner) {
                context.lineTo(width, height-radius)
                context.arcTo(width, height, width - radius, height, radius)
            } else {
                context.lineTo(width, height)
            }

            if (bottomLeftCorner) {
                context.lineTo(radius, height)
                context.arcTo(0, height, 0, height - radius, radius)
            } else {
                context.lineTo(0, height)
            }

            // Close path
            context.lineTo(height / 2)
            context.closePath()

            // Draw border
            context.lineWidth = borderWidth
            context.strokeStyle = borderColor
            context.stroke()

            // Draw background
            context.fillStyle = backgroundColor
            context.fill();
        }
    }
}
