// bslstl_array.h                                                     -*-C++-*-
#ifndef INCLUDED_BSLSTL_ARRAY
#define INCLUDED_BSLSTL_ARRAY

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide an STL compliant array.
//
//@CLASSES:
//   bsl::array: an STL compliant array
//
//@SEE_ALSO: bslstl_vector
//
//@AUTHOR: Jordan Levin (jlevin)
//
//@DESCRIPTION: This component defines a single class template, 'bsl::array',
// implementing the standard container 'std::array', holding a non-resizable
// array of values of a template parameter type where the size is specified as
// the second template parameter.
//
// An instantiation of 'array' is a value-semantic type whose salient attibutes
// are its size and the sequence of values the array contains.  If 'array' is
// instantiated with a value type that is not value-semantic, then the array
// will not retain all of its value-semantic qualities.  In particular, if a
// value type cannot be tested for equality, then an 'array' containing objects
// of that type will fail to compile the equality comparison operator.
// Similarly, if an 'array' is instantiated with a type that does not have a
// copy-constructor, then the 'array' will not be copyable.
//
// An array meets most the requirements of a container with random access
// iterators in the C++ standard [array].  The 'array' implemented here follows
// the C++11 standard when compiled with a C++11 compiler and follows the C++03
// standard otherwise.

// An array lacks certain requirements of a sequential container.  Array lacks
// insert, erase, emplace, and clear, as these functions would require
// modifying the size of the array.

// An array also meets the requirements of an aggregate.  This means that an
// array has: no user-declared constructors, no private or protected non-static
// data members, no base classes, and no virtual functions.  An array can be
// constructed using aggregate initialization.  Refer to the section
// [del.init.aggr] in the C++ standard for more detailed information.
//
///Operations
///----------
// This section describes the run-time complexity of operations on instances of
// 'array':
//..
//  Legend
//  ------
//  'V'                 - (template parameter) 'VALUE_TYPE' of the array
//  'S'                 - (template parameter) 'SIZE' of the array
//  'a', 'b'            - two distinct objects of type 'array<V, S>'
//  'k'                 - non-negative integer
//  'vt1', 'vt2', 'vt3' - objects of type 'VALUE_TYPE'
//
//  |-----------------------------------------+-------------------------------|
//  | Operation                               | Complexity                    |
//  |=========================================+===============================|
//  | array<V> a    (default construction)    | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | array<V> a(b) (copy construction)       | O[S]                          |
//  |-----------------------------------------+-------------------------------|
//  | array<V> a = {{vt1, vt2, vt3}}          | O[S]                          |
//  | array<V> a = {vt1, vt2, vt3}            |                               |
//  |               (aggregate initialization)|                               |
//  |-----------------------------------------+-------------------------------|
//  | a.~array<V>() (destruction)             | O[S]                          |
//  |-----------------------------------------+-------------------------------|
//  | a.begin(), a.end(),                     | O[1]                          |
//  | a.cbegin(), a.cend(),                   |                               |
//  | a.rbegin(), a.rend(),                   |                               |
//  | a.crbegin(), a.crend()                  |                               |
//  |-----------------------------------------+-------------------------------|
//  | a.size()                                | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | a.max_size()                            | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | a.empty()                               | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | a[k]                                    | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | a.at(k)                                 | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | a.front()                               | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | a.back()                                | O[1]                          |
//  |-----------------------------------------+-------------------------------|
//  | a.swap(b), swap(a,b)                    | O[S]                          |
//  |-----------------------------------------+-------------------------------|
//  | a = b;           (copy assignment)      | O[S]                          |
//  |-----------------------------------------+-------------------------------|
//  | a == b, a != b                          | O[S]                          |
//  |-----------------------------------------+-------------------------------|
//  | a < b, a <= b, a > b, a >= b            | O[S]                          |
//  |-----------------------------------------+-------------------------------|
//..
//
///Usage
///-----
// In this section we show intended use of this component.
//
///Example 1: Returning an array from a function
///- - - - - - - - - - - - - - - - - - - - - - -
// Suppose we want to define a function that will return an array of 'float's.
// If a raw array were used, the size would need to be tracked seperately
// because raw arrays decay to pointers when passed as function arguments, or
// returned by-value.  'bsl::array' does not decay, and so provides a simple
// solution to this problem.
//..
//  typedef bsl::array<float, 3> Point;
//
//  Point createPoint(float f1, float f2, float f3)
//  {
//      bsl::array<float, 3> ret = {f1, f2, f3};
//      return ret;
//  }
//..
// Create a bsl::array object containing three values set to the specified
// 'f1', 'f2', 'f3'.
//..
//  void usageExample(){
//      Point p1 = createPoint(1.0, 1.0, 1.0);
//      Point p2 = createPoint(2.0, 2.0, 2.0);
//      Point p3 = createPoint(3.0, 3.0, 3.0);
//
//      bsl::array<Point, 3> points = {p1, p2, p3};
//
//      for(size_t i = 0; i < points.size(); ++i){
//          for(size_t j = 0; j < points[i].size(); ++j){
//              points[i][j] *= 2.0f;
//          }
//      }
//  }
//..
// Use the 'createPoint' function to generate three 'float' arrays.  The arrays
// are returned by copy and the 'size()' member function is used to access the
// size of the arrays that could not be done with a raw array.
//..

// Prevent 'bslstl' headers from being included directly in 'BSL_OVERRIDES_STD'
// mode.  Doing so is unsupported, and is likely to cause compilation errors.
#if defined(BSL_OVERRIDES_STD) && !defined(BSL_STDHDRS_PROLOGUE_IN_EFFECT)
#error "include <bsl_array.h> instead of <bslstl_array.h> in BSL_OVERRIDES_STD\
 mode"
#endif

#include <bslscm_version.h>

#include <bslstl_iterator.h>
#include <bslstl_stdexceptutil.h>

#include <bslalg_rangecompare.h>
#include <bslalg_swaputil.h>

#include <bslh_hash.h>

#include <bslmf_assert.h>
#include <bslmf_movableref.h>

#include <bsls_assert.h>
#include <bsls_compilerfeatures.h>
#include <bsls_keyword.h>
#include <bsls_libraryfeatures.h>
#include <bsls_nativestd.h>

#include <stddef.h>

#if defined(BSLS_LIBRARYFEATURES_HAS_CPP11_TUPLE)
# include <tuple>  // 'std::tuple_size' etc.
#endif

#include <utility>

namespace bsl {

                                // ===========
                                // class array
                                // ===========

template <class VALUE_TYPE, size_t SIZE>
struct array {
    // This class template provides a standard conforming implementation of
    // 'std::array'.  'array' is an aggregate wrapper around a raw array,
    // supporting aggregate initialization and an iterator interface as
    // required for a standard container.

// BDE_VERIFY pragma: push
// BDE_VERIFY pragma: -MN01
    // DATA
    VALUE_TYPE d_data[(0 == SIZE) ? 1 : SIZE];
// BDE_VERIFY pragma: pop

    // PUBLIC TYPES
    typedef VALUE_TYPE                             value_type;
    typedef VALUE_TYPE                            *pointer;
    typedef const VALUE_TYPE                      *const_pointer;
    typedef VALUE_TYPE&                            reference;
    typedef const VALUE_TYPE&                      const_reference;
    typedef size_t                                 size_type;
    typedef ptrdiff_t                              difference_type;
    typedef pointer                                iterator;
    typedef const_pointer                          const_iterator;
    typedef bsl::reverse_iterator<iterator>        reverse_iterator;
    typedef bsl::reverse_iterator<const_iterator>  const_reverse_iterator;

    // CREATORS
    //! array() = default;
        // Create an 'array' object.  Every element is default constructed if
        // 'VALUE_TYPE' is default constructible; otherwise, 'array' is not
        // default constructible.
    //! array(const array& original) = default;
        // Create an 'array' object having the same value as the specified
        // 'original' object.  Every element is copy constructed from the
        // corresponding element in the specified 'original' if 'VALUE_TYPE' is
        // copy constructible; otherwise, 'array' is not copy constructible.
        // Only in C++11 and later.
    //! array(array&& original) = default;
        // Create an 'array' object having the same value as the specified
        // 'original' object.  Every element is move constructed from
        // the corresponding element in the specified 'original' if
        // 'VALUE_TYPE' is move constructible; otherwise, 'array' is not move
        // constructible.
    //! ~array() = default;
        // Destroy this object.  Evert element is destroyed if 'VALUE_TYPE' is
        // destructible; otherwise, array is not destructible.

    // MANIPULATORS
    void fill(const VALUE_TYPE& value);
        // Set every elemment in this array to the specified 'value' using the
        // 'operator=' of 'value_type'.

    void swap(array& rhs);
        // Exchange each corresponding element between this array and the
        // specified 'rhs' array by calling 'swap(a,b)' where 'swap' is found
        // by overload resolution including at least the namespaces 'std' and
        // the associated namespaces of 'VALUE_TYPE'.

    iterator begin();
        // Return an iterator providing modifiable access to the first element
        // in this array; return a past-the-end iterator if this array has size
        // 0.

    iterator end();
        // Return a past-the-end iterator providing modifiable access to this
        // array.

    reverse_iterator rbegin();
        // Return a reverse iterator providing modifiable access to the last
        // element in this array; return a past-the-end iterator if this array
        // has size 0.

    reverse_iterator rend();
        // Return the past-the-end reverse iterator providing modifiable access
        // to this array.

    reference operator[](size_type position);
        // Return a reference providing modifiable access to the element at the
        // specified 'position' in this array.  The behavior is undefined
        // unless 'position < size()'.

    reference at(size_type position);
        // Return a reference to the element at the specified 'position' in
        // this array.  Throw an 'out_of_range' exception if
        // 'position >= size()'.

    reference front();
        // Return a reference to the first element in this array.  The behavior
        // is undefined unless 'SIZE > 0'.

    reference back();
        // Return a reference to the last element in this array.  The behavior
        // is undefined unless 'SIZE > 0'.

    pointer data();
        // Return the address of the first element of the underlying raw array.
        // Return a valid 'T*' which cannot be dereferenced if the 'SIZE' is 0.

    //! array& operator=(const array& other);
        // Sets every element in this array to the corresponding element in the
        // specified 'other' if 'VALUE_TYPE' is copy assignable; otherwise,
        // 'array' is not copy assignable.
    //! array& operator=(array&& other);
        // Moves every element in the specified 'other' into the corresponding
        // element in this array in the if 'VALUE_TYPE' is moves assignable;
        // otherwise, 'array' is not move assignable.

    // BDE_VERIFY pragma: -FABC01
    // ACCESSORS
    const_iterator begin() const;
    const_iterator cbegin() const;
        // Return an iterator providing non-modifiable access to the first
        // element in this array; return a past-the-end iterator if this array
        // has size 0.

    const_iterator end() const;
    const_iterator cend() const;
        // Return a past-the-end iterator providing non-modifiable access to
        // this array.

    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
        // Return a reverse iterator providing non-modifiable access to the
        // last element in this array, and the past-the-end reverse iterator
        // if this array has size 0.

    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;
        // Return the past-the-end reverse iterator providing non-modifiable
        // access to this 'array'.

    bool empty() const;
        // Return 'true' if the array has size 0, and 'false' otherwise.

    size_type size() const;
    size_type max_size() const;
        // Return the number of elements in this array.

    const_reference operator[](size_type position) const;
        // Return a reference providing non-modifiable access to the element at
        // the specified 'position' in this array.  The behavior is undefined
        // unless 'position < size()'.

    const_reference at(size_type position) const;
        // Return a reference providing non-modifiable access to the element at
        // the specified 'position' in this array.  Throw an 'out_of_range'
        // exception if 'position >= size()'.

    const_reference front() const;
        // Return a reference providing non-modifiable access to the first
        // element in this array.  The behavior is undefined unless 'SIZE > 0'.

    const_reference back() const;
        // Return a reference providing non-modifiable access to the last
        // element in this array.  Behavior is undefined unless 'SIZE > 0'.

    const_pointer data() const;
        // Return the address of the first element of the underlying raw array.
        // Return a valid 'T*' which cannot be dereferenced if the 'SIZE' is 0.
};

// FREE OPERATORS
template <class VALUE_TYPE, size_t SIZE>
bool operator==(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs);
    // Return 'true' if the specified 'lhs' has the same value as the specified
    // 'rhs'; return false otherwise.  Two arrays have the same value if each
    // element has the same value as the corresponding element in the other
    // array.

template <class VALUE_TYPE, size_t SIZE>
bool operator!=(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs);
    // Return 'true' if the specified 'lhs' does not have the same value as the
    // specified 'rhs'; return false otherwise.  Two arrays do not have the
    // same value if  some element in the ordered sequence of elements of 'lhs'
    // does not have the same value as the corresponding element in the ordered
    // sequence of elements of 'rhs'.

template <class VALUE_TYPE, size_t SIZE>
bool operator<(const array<VALUE_TYPE, SIZE>& lhs,
               const array<VALUE_TYPE, SIZE>& rhs);
    // Return 'true' if the specified 'lhs' is lexicographically less than the
    // specified 'rhs' by using the comparison operators of 'VALUE_TYPE' on
    // each element; return 'false' otherwise.

template <class VALUE_TYPE, size_t SIZE>
bool operator>(const array<VALUE_TYPE, SIZE>& lhs,
               const array<VALUE_TYPE, SIZE>& rhs);
    // Return 'true' if the specified 'lhs' is lexicographically greater than
    // the specified 'rhs' by using the comparison operators of 'VALUE_TYPE' on
    // each element; return 'false' otherwise.

template <class VALUE_TYPE, size_t SIZE>
bool operator<=(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs);
    // Return 'true' if the specified 'lhs' is lexicographically less than the
    // specified 'rhs' by using the comparison operators of 'VALUE_TYPE' on
    // each element or if 'lhs' and 'rhs' are equal; return 'false' otherwise.

template <class VALUE_TYPE, size_t SIZE>
bool operator>=(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs);
    // Return 'true' if the specified 'lhs' is lexicographically greater than
    // the specified 'rhs' by using the comparison operators of 'VALUE_TYPE' on
    // each element or if 'lhs' and 'rhs' are equal; return 'false' otherwise.

// FREE FUNCTIONS
template <class VALUE_TYPE, size_t SIZE>
void swap(array<VALUE_TYPE, SIZE>& lhs, array<VALUE_TYPE, SIZE>& rhs);
    // Call 'swap' using ADL on each element of the specified 'lhs' with the
    // corresponding element in the specified 'rhs'.

template<size_t INDEX, class T, size_t N>
T& get(array<T, N>& a) BSLS_KEYWORD_NOEXCEPT;
    // Return a reference providing modifiable access to the element of the
    // specified 'a', having the ordinal number specified by the (template
    // parameter) 'INDEX'.  This function will not compile unless 'INDEX < N'.

template<size_t INDEX, class T, size_t N>
const T& get(const array<T, N>& a) BSLS_KEYWORD_NOEXCEPT;
    // Return a reference providing non-modifiable access to the element of the
    // specified 'a', having the ordinal number specified by the (template
    // parameter) 'INDEX'.  This function will not compile unless 'INDEX < N'.

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES)
template<size_t INDEX, class T, size_t N>
T&& get(array<T, N>&& a) BSLS_KEYWORD_NOEXCEPT;
    // Return an rvalue reference providing modifiable access to the element of
    // the specified 'a', having the ordinal number specified by the (template
    // parameter) 'INDEX'.  This function will not compile unless 'INDEX < N'.

template<size_t INDEX, class T, size_t N>
const T&& get(const array<T, N>&& a) BSLS_KEYWORD_NOEXCEPT;
    // Return an rvalue reference providing non-modifiable access to the
    // element of the specified 'a', having the ordinal number specified by the
    // (template parameter) 'INDEX'.  This function will not compile unless
    // 'INDEX < N'.
#endif  // BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES

// HASH SPECIALIZATIONS
template <class HASHALG, class T, size_t N>
void hashAppend(HASHALG& hashAlg, const array<T, N>&  input);
    // Pass the specified 'input' to the specified 'hashAlg'

}  // close namespace bsl

#if defined(BSLS_LIBRARYFEATURES_HAS_CPP11_TUPLE)

namespace std {

                             // ====================
                             // struct tuple_element
                             // ====================

template<size_t INDEX, class T, size_t N>
struct tuple_element<INDEX, bsl::array<T, N> >
{
    // This partial specialization of 'tuple_element' provides compile-time
    // access to the type of the array's elements.
    BSLMF_ASSERT(INDEX < N);
    // TYPES
    typedef T type;
};

                              // =================
                              // struct tuple_size
                              // =================

template<class T, size_t N>
struct tuple_size<bsl::array<T, N> > : integral_constant<size_t, N>
{
    // This meta-function provides a compile-time way to obtain the number of
    // elements in an array.
};

}  // close namespace std

#endif  // BSLS_LIBRARYFEATURES_HAS_CPP11_TUPLE

namespace bsl {

// ============================================================================
//                  TEMPLATE AND INLINE FUNCTION DEFINITIONS
// ============================================================================

                                // -----------
                                // class array
                                // -----------

// MANIPULATORS
template <class VALUE_TYPE, size_t SIZE>
void array<VALUE_TYPE, SIZE>::fill(const VALUE_TYPE& value)
{
    for (size_t i = 0; i < SIZE; ++i) {
        d_data[i] = value;
    }
}

template <class VALUE_TYPE, size_t SIZE>
void array<VALUE_TYPE, SIZE>::swap(array<VALUE_TYPE, SIZE>& rhs)
{
    for (size_t i = 0; i < SIZE; ++i) {
        BloombergLP::bslalg::SwapUtil::swap(d_data + i, rhs.d_data + i);
    }
}

// ACCESSORS
template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::iterator array<VALUE_TYPE, SIZE>::begin()
{
    return d_data;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_iterator
array<VALUE_TYPE, SIZE>::begin() const
{
    return d_data;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::iterator array<VALUE_TYPE, SIZE>::end()
{
    return d_data + SIZE;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_iterator
array<VALUE_TYPE, SIZE>::end() const
{
    return d_data + SIZE;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::reverse_iterator
array<VALUE_TYPE, SIZE>::rbegin()
{
    return array<VALUE_TYPE, SIZE>::reverse_iterator(d_data + SIZE);
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reverse_iterator
array<VALUE_TYPE, SIZE>::rbegin() const
{
    return array<VALUE_TYPE, SIZE>::const_reverse_iterator(d_data + SIZE);
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::reverse_iterator
array<VALUE_TYPE, SIZE>::rend()
{
    return array<VALUE_TYPE, SIZE>::reverse_iterator(d_data);
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reverse_iterator
array<VALUE_TYPE, SIZE>::rend() const
{
    return array<VALUE_TYPE, SIZE>::const_reverse_iterator(d_data);
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_iterator
array<VALUE_TYPE, SIZE>::cbegin() const
{
    return d_data;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_iterator
array<VALUE_TYPE, SIZE>::cend() const
{
    return d_data + SIZE;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reverse_iterator
array<VALUE_TYPE, SIZE>::crbegin() const
{
    return array<VALUE_TYPE, SIZE>::const_reverse_iterator(d_data + SIZE);
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reverse_iterator
array<VALUE_TYPE, SIZE>::crend() const
{
    return array<VALUE_TYPE, SIZE>::const_reverse_iterator(d_data);
}

template <class VALUE_TYPE, size_t SIZE>
bool array<VALUE_TYPE, SIZE>::empty() const
{
    return SIZE == 0;
}

template <class VALUE_TYPE, size_t SIZE>
size_t array<VALUE_TYPE, SIZE>::size() const
{
    return SIZE;
}

template <class VALUE_TYPE, size_t SIZE>
size_t array<VALUE_TYPE, SIZE>::max_size() const
{
    return SIZE;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::reference
array<VALUE_TYPE, SIZE>::operator[](size_type position)
{
    BSLS_ASSERT(position < SIZE);
    return d_data[position];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reference
array<VALUE_TYPE, SIZE>::operator[](size_type position) const
{
    BSLS_ASSERT(position < SIZE);
    return d_data[position];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::reference array<VALUE_TYPE, SIZE>::at(
                                                            size_type position)
{
    if (position >= SIZE) {
        BloombergLP::bslstl::StdExceptUtil::throwOutOfRange(
            "array<...>::at(position): invalid position");
    }
    return d_data[position];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reference array<VALUE_TYPE, SIZE>::at(
                                                      size_type position) const
{
    if (position >= SIZE) {
        BloombergLP::bslstl::StdExceptUtil::throwOutOfRange(
            "array<...>::at(position): invalid position");
    }
    return d_data[position];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::reference array<VALUE_TYPE, SIZE>::front()
{
    BSLS_ASSERT(SIZE > 0);
    return d_data[0];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reference
array<VALUE_TYPE, SIZE>::front() const
{
    BSLS_ASSERT(SIZE > 0);
    return d_data[0];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::reference array<VALUE_TYPE, SIZE>::back()
{
    BSLS_ASSERT(SIZE > 0);
    return d_data[SIZE - 1];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_reference
array<VALUE_TYPE, SIZE>::back() const
{
    BSLS_ASSERT(SIZE > 0);
    return d_data[SIZE - 1];
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::iterator array<VALUE_TYPE, SIZE>::data()
{
    return d_data;
}

template <class VALUE_TYPE, size_t SIZE>
typename array<VALUE_TYPE, SIZE>::const_iterator
array<VALUE_TYPE, SIZE>::data() const
{
    return d_data;
}

// FREE OPERATORS
template <class VALUE_TYPE, size_t SIZE>
bool operator==(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs)
{
    return BloombergLP::bslalg::RangeCompare::equal(lhs.begin(),
                                                    lhs.end(),
                                                    lhs.size(),
                                                    rhs.begin(),
                                                    rhs.end(),
                                                    rhs.size());
}

template <class VALUE_TYPE, size_t SIZE>
bool operator!=(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs)
{
    return !(lhs == rhs);
}

template <class VALUE_TYPE, size_t SIZE>
bool operator<(const array<VALUE_TYPE, SIZE>& lhs,
               const array<VALUE_TYPE, SIZE>& rhs)
{
    return 0 > BloombergLP::bslalg::RangeCompare::lexicographical(lhs.begin(),
                                                                  lhs.end(),
                                                                  lhs.size(),
                                                                  rhs.begin(),
                                                                  rhs.end(),
                                                                  rhs.size());
}

template <class VALUE_TYPE, size_t SIZE>
bool operator>(const array<VALUE_TYPE, SIZE>& lhs,
               const array<VALUE_TYPE, SIZE>& rhs)
{
    return rhs < lhs;
}

template <class VALUE_TYPE, size_t SIZE>
bool operator<=(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs)
{
    return !(rhs < lhs);
}

template <class VALUE_TYPE, size_t SIZE>
bool operator>=(const array<VALUE_TYPE, SIZE>& lhs,
                const array<VALUE_TYPE, SIZE>& rhs)
{
    return !(lhs < rhs);
}

// FREE FUNCTIONS
template <class VALUE_TYPE, size_t SIZE>
void swap(array<VALUE_TYPE, SIZE>& lhs, array<VALUE_TYPE, SIZE>& rhs)
{
    lhs.swap(rhs);
}

template<size_t INDEX, class T, size_t N>
T& get(array<T, N>& a) BSLS_KEYWORD_NOEXCEPT
{
    return a[INDEX];
}

template<size_t INDEX, class T, size_t N>
const T& get(const array<T, N>& a) BSLS_KEYWORD_NOEXCEPT
{
    return a[INDEX];
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES)
template<size_t INDEX, class T, size_t N>
T&& get(array<T, N>&& a) BSLS_KEYWORD_NOEXCEPT
{
    return BloombergLP::bslmf::MovableRefUtil::move(a[INDEX]);
}

template<size_t INDEX, class T, size_t N>
const T&& get(const array<T, N>&& a) BSLS_KEYWORD_NOEXCEPT
{
    return BloombergLP::bslmf::MovableRefUtil::move(a[INDEX]);
}
#endif  // BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES

// HASH SPECIALIZATIONS
template <class HASHALG, class T, size_t N>
void hashAppend(HASHALG& hashAlg, const array<T, N>&  input)
{
    using ::BloombergLP::bslh::hashAppend;

    hashAppend(hashAlg, N);
    for (size_t i = 0; i < N; ++i)
    {
        hashAppend(hashAlg, input[i]);
    }
}

}  // close namespace bsl

#endif

// ----------------------------------------------------------------------------
// Copyright 2019 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
