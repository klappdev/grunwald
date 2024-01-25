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

pragma Singleton

import QtQuick 2.0

QtObject {
    property bool isDarkTheme: false
    readonly property color primaryColor: isDarkTheme ? "#BB86FC" : "#337ab7"
    readonly property color primaryVariantColor: "#3700B3"
    readonly property color secondaryColor: isDarkTheme ? "#3c3c3c" : "#ebebeb"
    readonly property color secondaryVariant: isDarkTheme ? "#018786" : secondaryColor
    readonly property color hoveredColor: isDarkTheme ? "#BB86FC" : "#286090"
    readonly property color backgroundColor: isDarkTheme ? "#121212" : "#FFFFFF"
    readonly property color errorColor: isDarkTheme ? "#CF6679" : "#B00020"
    readonly property color textColor: isDarkTheme ? "#FFFFFF" : "#000000"
    readonly property color themeDefaultColor: isDarkTheme ? "#000000" : "#FFFFFF"
    readonly property color themeInvertedColor: isDarkTheme ? "#FFFFFF" : "#000000"

    readonly property real defaultOpacity: 1
    readonly property real emphasisOpacity: 0.87
    readonly property real secondaryOpacity: 0.6
    readonly property real disabledOpacity: 0.38

    readonly property int defaultOffset: 15
    readonly property int mediumOffset: 10
    readonly property int tinyOffset: 5

    readonly property int bigSpacing: 30
    readonly property int mediumSpacing: 10
    readonly property int smallSpacing: 5

    function rgb(r: int, g: int, b: int, a: int): int {
        return "#"
             + (a ? a.toString(16).padStart(2, "0") : "")
             + r.toString(16).padStart(2, "0")
             + g.toString(16).padStart(2, "0")
             + b.toString(16).padStart(2, "0");
    }
}
