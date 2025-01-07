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

#include "cache/WordCache.hpp"

namespace {
    constexpr const char* const TAG = "[WordCache] ";
}

namespace grunwald {

    const Word WordCache::EMPTY_WORD = {
        .id = -1,
        .name = "",
        .transcription = "",
        .translation = "",
        .association = "",
        .etymology = "",
        .description = "",
        .type = WordType::Unknown,
        .image = WordImage{},
        .date = QDateTime{}
    };

    WordCache::WordCache() : mData(new Word{EMPTY_WORD}) {
    }

    WordCache::~WordCache() {
        auto* data = mData.loadAcquire();

        if (data != nullptr) {
            delete mData;
        }
    }

    bool WordCache::isValid() const {
        return loadWordContent() == EMPTY_WORD;
    }

    void WordCache::clear() {
        storeWordContent(EMPTY_WORD);
    }

    void WordCache::storeWordContent(const Word& word) {
        auto* atomicData = mData.loadAcquire();
        *atomicData = word;

        mData.storeRelease(atomicData);
    }

    void WordCache::storeWordImage(const WordImage& wordImage) {
        auto* atomicData = mData.loadAcquire();
        atomicData->image = wordImage;

        mData.storeRelease(atomicData);
    }

    auto WordCache::loadWordContent() const -> Word {
        auto* atomicData = mData.loadAcquire();

        if (atomicData == nullptr) {
            qWarning() << TAG << "Cached data is NULL!" << Qt::endl;
            return {};
        }

        return *atomicData;
    }

    auto WordCache::loadWordImage() const -> WordImage {
        auto* atomicData = mData.loadAcquire();

        if (atomicData == nullptr) {
            qWarning() << TAG << "Cached data is NULL!" << Qt::endl;
            return {};
        }

        return atomicData->image;
    }

}
