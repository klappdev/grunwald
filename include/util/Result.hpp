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
#pragma once

#include <variant>

namespace grunwald {

    /*
     * When migrate to C++23, better to use std::expected
     */
    template<typename V, typename E>
    class /*[[nodiscard]]*/ Result final {
    public:
        constexpr Result() = default;
        ~Result() = default;

        constexpr Result(V value) : data(std::move(value)) {}
        constexpr Result(E error) : data(std::move(error)) {}

        constexpr Result(const Result&) = default;
        constexpr Result& operator=(const Result&) = default;

        constexpr Result(Result&&) noexcept = default;
        constexpr Result& operator=(Result&&) = default;

        constexpr Result& operator=(const V& value) {
            data = value;
            return *this;
        }

        constexpr Result& operator=(const E& error) {
            data = error;
            return *this;
        }

        constexpr bool hasValue() const { return data.index() == 0; }
        constexpr bool hasError() const { return data.index() == 1; }

        constexpr const V* operator->() const { return std::addressof(value()); }
        constexpr V* operator->() { return std::addressof(value()); }

        constexpr V& operator*() & { return value(); }
        constexpr const V& operator*() const& { return value(); }
        constexpr V&& operator*() && { return std::move(value()); }
        constexpr const V&& operator*() const && { return std::move(value()); }

        constexpr V& value() & { return std::get<V>(data); }
        constexpr const V& value() const& { return std::get<V>(data); }
        constexpr V&& value() && { return std::move(std::get<V>(data)); }
        constexpr const V&& value() const&& { return std::move(std::get<V>(data)); }

        constexpr E& error() & { return std::get<E>(data); }
        constexpr const E& error() const& { return std::get<E>(data); }
        constexpr E&& error() && { return std::move(std::get<E>(data)); }
        constexpr const E&& error() const&& { return std::move(std::get<E>(data)); }

        template<typename V2, typename E2>
        friend constexpr bool operator==(const Result& left, const Result<V2, E2>& right) {
            if (left.hasValue() != right.hasValue()) return false;
            if (!left.hasValue()) return left.error() == right.error();
            return *left == *right;
        }

    private:
        std::variant<V, E> data;
    };

    template<typename V, typename E>
        requires std::is_void_v<V>
    class /*[[nodiscard]]*/ Result<V, E> {
    public:
        constexpr Result() = default;
        ~Result() = default;

        constexpr Result(E error) : data(std::move(error)) {}

        constexpr Result(const Result&) = default;
        constexpr Result& operator=(const Result&) = default;

        constexpr Result(Result&&) noexcept = default;
        constexpr Result& operator=(Result&&) = default;

        constexpr Result& operator=(const E& error) {
            data = error;
            return *this;
        }

        constexpr bool hasError() const { return data.index() == 1; }

        constexpr E& error() & { return std::get<E>(data); }
        constexpr const E& error() const& { return std::get<E>(data); }
        constexpr E&& error() && { return std::move(std::get<E>(data)); }
        constexpr const E&& error() const&& { return std::move(std::get<E>(data)); }

        template<typename V2, typename E2> requires std::is_void_v<V2>
        friend constexpr bool operator==(const Result& left, const Result<V2, E2>& right) {
            if (left.hasError()) return left.error() == right.error();
            return false;
        }

    private:
        struct Empty {};
        std::variant<Empty, E> data;
    };
}


