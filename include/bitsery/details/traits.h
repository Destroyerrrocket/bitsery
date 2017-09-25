//MIT License
//
//Copyright (c) 2017 Mindaugas Vinkelis
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#ifndef BITSERY_DETAILS_TRAITS_H
#define BITSERY_DETAILS_TRAITS_H

#include <type_traits>

namespace bitsery {
    namespace details {

        /*
         * core library traits, used to extend library for custom types
         */

        //traits for extension
        template<typename Extension, typename T>
        struct ExtensionTraits {
            //this type is used, when using extesion without custom lambda
            // eg.: extension4b>(obj, myextension{}) will call s.value4b(obj) for TValue
            // or extesion(obj, myextension{})  will call s.object(obj) for TValue
            //when this is void, it will compile, but value and object overloads will do nothing.
            using TValue = void;

            //does extension support ext<N>(...) syntax, by calling value<N> with TValue
            static constexpr bool SupportValueOverload = true;
            //does extension support ext(...) syntax, by calling object with TValue
            static constexpr bool SupportObjectOverload = true;
            //does extension support ext(..., lambda)
            static constexpr bool SupportLambdaOverload = true;
        };

        //primary traits for containers
        template<typename T>
        struct ContainerTraits {

            using TValue = void;

            static constexpr bool isResizable = false;
            //contiguous arrays has oppurtunity to memcpy whole buffer directly when using funtamental types
            //contiguous doesn't nesessary equal to random access iterator.
            //contiguous hopefully will be available in c++20
            static constexpr bool isContiguous = false;
            //resize function, called only if container is resizable
            static void resize(T& container, size_t size) {
                static_assert(std::is_void<T>::value,
                              "Define ContainerTraits or include from <bitsery/traits/...> to use as container");
            }
            //get container size
            static size_t size(const T& container) {
                static_assert(std::is_void<T>::value,
                              "Define ContainerTraits or include from <bitsery/traits/...> to use as container");
                return 0u;
            }
        };

        //specialization for C style array
        template<typename T, size_t N>
        struct ContainerTraits<T[N]> {
            using TValue = T;
            static constexpr bool isResizable = false;
            static constexpr bool isContiguous = true;
            static size_t size(const T (&container)[N]) {
                return N;
            }
        };

        //specialization for initializer list, even though it cannot be deserialized to.
        template<typename T>
        struct ContainerTraits<std::initializer_list<T>> {
            using TValue = T;
            static constexpr bool isResizable = false;
            static constexpr bool isContiguous = true;
            static size_t size(const std::initializer_list<T>& container) {
                return container.size();
            }
        };



        //traits for text, default adds null-terminated character at the end
        template<typename T>
        struct TextTraits {

            //if container is not null-terminated by default, add NUL at the end
            static constexpr bool addNUL = true;

            //get length of null terminated container
            static size_t length(const T& container) {
                static_assert(std::is_void<T>::value,
                              "Define TextTraits or include from <bitsery/traits/...> to use as text");
                return 0u;
            }
        };

        //traits only for buffer reader/writer
        template <typename T>
        struct BufferContainerTraits {
            //this function is only applies to resizable containers

            //this function is only used by BufferWriter, when writing data to buffer,
            //it is called only current buffer size is not enough to write.
            //it is used to dramaticaly improve performance by updating buffer directly
            //instead of using back_insert_iterator to append each byte to buffer.
            //thats why BufferWriter return range iterators

            static void increaseBufferSize(T& container) {
                static_assert(std::is_void<T>::value,
                              "Define BufferContainerTraits or include from <bitsery/traits/...> to use as buffer");
            }

            using TIterator = void;
        };

        //specialization for c-style buffer
        template <typename T, size_t N>
        struct BufferContainerTraits<T[N]> {
            using TIterator = T*;
        };

    }
}

#endif //BITSERY_DETAILS_TRAITS_H
