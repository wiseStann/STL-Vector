#include "Basic.hpp"

#ifndef _ITERATOR_HPP
#define _ITERATOR_HPP


// Independent developments collection
namespace idc
{

    template <typename _IterType, class _Container>
    class _Iterator
    {
        public:
            typedef std::iterator_traits<_IterType>           _iter_traits;
            typedef typename _iter_traits::iterator_category  iterator_category;
            typedef typename _iter_traits::difference_type    difference_type;
            typedef typename _iter_traits::pointer            pointer;
            typedef typename _iter_traits::reference          reference;
            typedef _IterType                                 iterator_type;

            constexpr _Iterator() noexcept :
                __current(iterator_type()) { }
            
            constexpr _Iterator(const iterator_type& _other) noexcept :
                __current(_other) { }
        
        public:

            /* Access and modification operators */

            constexpr pointer operator->() noexcept
            { return __current; }

            constexpr reference operator*() noexcept
            { return *__current; }

            
            constexpr _Iterator& operator+=(const difference_type& _n)
            const noexcept
            { __current += _n; return *this; }

            constexpr _Iterator& operator-=(const difference_type& _n)
            const noexcept
            { __current -= _n; return *this; }

            constexpr _Iterator& operator++() noexcept
            { __current++; return *this; }

            constexpr _Iterator operator++(int) noexcept
            { return _Iterator(this-> __current++); }

            constexpr _Iterator& operator--() noexcept
            { __current--; return *this; }

            constexpr _Iterator operator--(int) noexcept
            { return _Iterator(this->__current--); }

            constexpr _Iterator operator+(const difference_type& _n)
            const noexcept
            { return _Iterator(this->__current + _n); }

            constexpr _Iterator operator-(const difference_type& _n)
            const noexcept
            { return _Iterator(this->__current - _n); }


            iterator_type base() const noexcept
            { return __current; }

        private:
            pointer __current;
    };

       /* Comparison operators */

    template <class _Iter1, class _Iter2, class _Container>
    constexpr inline bool
    operator==(const _Iterator<_Iter1, _Container>& _lhs,
                const _Iterator<_Iter2, _Container>& _rhs) noexcept
    { return _lhs.base() == _rhs.base(); }

    template <class _It, class _Container>
    constexpr inline bool
    operator==(const _Iterator<_It, _Container>& _lhs,
                const _Iterator<_It, _Container>& _rhs) noexcept
    { return _lhs.base() == _rhs.base(); }


    template <class _Iter1, class _Iter2, class _Container>
    constexpr inline bool
    operator!=(const _Iterator<_Iter1, _Container>& _lhs,
                const _Iterator<_Iter2, _Container>& _rhs) noexcept
    { return _lhs.base() != _rhs.base(); }

    template <class _It, class _Container>
    constexpr inline bool
    operator!=(const _Iterator<_It, _Container>& _lhs,
                const _Iterator<_It, _Container>& _rhs) noexcept
    { return _lhs.base() != _rhs.base(); }


    template <class _Iter1, class _Iter2, class _Container>
    constexpr inline bool
    operator>(const _Iterator<_Iter1, _Container>& _lhs,
                const _Iterator<_Iter2, _Container>& _rhs) noexcept
    { return _lhs.base() > _rhs.base(); }

    template <class _It, class _Container>
    constexpr inline bool
    operator>(const _Iterator<_It, _Container>& _lhs,
                const _Iterator<_It, _Container>& _rhs) noexcept
    { return _lhs.base() > _rhs.base(); }


    template <class _Iter1, class _Iter2, class _Container>
    constexpr inline bool
    operator<(const _Iterator<_Iter1, _Container>& _lhs,
                const _Iterator<_Iter2, _Container>& _rhs) noexcept
    { return _lhs.base() < _rhs.base(); }

    template <class _It, class _Container>
    constexpr inline bool
    operator<(const _Iterator<_It, _Container>& _lhs,
                const _Iterator<_It, _Container>& _rhs) noexcept
    { return _lhs.base() < _rhs.base(); }


    template <class _Iter1, class _Iter2, class _Container>
    constexpr inline bool
    operator>=(const _Iterator<_Iter1, _Container>& _lhs,
                const _Iterator<_Iter2, _Container>& _rhs) noexcept
    { return _lhs.base() >= _rhs.base(); }

    template <class _It, class _Container>
    constexpr inline bool
    operator>=(const _Iterator<_It, _Container>& _lhs,
                const _Iterator<_It, _Container>& _rhs) noexcept
    { return _lhs.base() >= _rhs.base(); }


    template <class _Iter1, class _Iter2, class _Container>
    constexpr inline bool
    operator<=(const _Iterator<_Iter1, _Container>& _lhs,
                const _Iterator<_Iter2, _Container>& _rhs) noexcept
    { return _lhs.base() <= _rhs.base(); }

    template <class _It, class _Container>
    constexpr inline bool
    operator<=(const _Iterator<_It, _Container>& _lhs,
                const _Iterator<_It, _Container>& _rhs) noexcept
    { return _lhs.base() <= _rhs.base(); }


    template<typename _Iter1, typename _Iter2, typename _Container>
#if __cplusplus >= 201103L
    constexpr inline auto
    operator-(const _Iterator<_Iter1, _Container>& _lhs,
                const _Iterator<_Iter2, _Container>& _rhs) noexcept
    -> decltype(_lhs.base() - _rhs.base())
#else
    inline typename _Iterator<_Iter1, _Container>::difference_type
    operator-(const _Iterator<_Iter1, _Container>& _lhs,
        const _Iterator<_Iter2, _Container>& _rhs)
#endif
    { return _lhs.base() - _rhs.base(); }

    template<typename _It, typename _Container>
    constexpr inline typename _Iterator<_It, _Container>::difference_type
    operator-(const _Iterator<_It, _Container>& _lhs,
        const _Iterator<_It, _Container>& _rhs) noexcept
    { return _lhs.base() - _rhs.base(); }



    template <typename _Tp>
    class _ReverseIterator
    {
        public:

    };

} // !idc namespace


#endif // !_ITERATOR_HPP