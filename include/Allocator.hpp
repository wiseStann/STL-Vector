#include "MemoryPool.hpp"

#ifndef _ALLOCATOR_HPP
#define _ALLOCATOR_HPP


// Independent developments collection
namespace idc {

    template<typename _Tp>
    class _Allocator {
        public:
            typedef _Tp                 value_type;
            typedef size_t              size_type;
            typedef std::ptrdiff_t      difference_type;

#if __cplusplus >= 201103L && __cplusplus <= 201703L
            typedef _Tp*                pointer;
            typedef const _Tp*          const_pointer;
            typedef _Tp&                reference;
            typedef const _Tp&          const_reference;

            template<class U>
            struct rebind {
                typedef _Allocator<U> other;
            };
#endif

        public:
#if __cplusplus >= 201103L && __cplusplus <= 201703L
            pointer address(reference _entity) const noexcept
            { return &_entity; }

            const_pointer address(const_reference _centity) const noexcept
            { return &_centity; }

            size_type max_size() const noexcept
            { return std::numeric_limits<size_type>::max() / sizeof(value_type); }

            template <typename _U, class ... Args>
            void construct(_U* _ptr, Args&&... _args)
            { ::new((void*)_ptr) value_type(std::forward<Args>(_args)...); }

            template <typename _U>
            void destroy(_U* _ptr)
            { _ptr->~value_type(); }

#endif
            pointer allocate(size_type _n)
            { return static_cast<pointer>(__mempool.allocate(_n)); }

            void deallocate(pointer _ptr, size_type _n)
            { __mempool.deallocate(_ptr, _n); }

        public:
            
#if __cplusplus >= 201103L && __cplusplus <= 201703L
            template <typename _U1, typename _U2>
            inline bool
            operator==(const _Allocator<_U2>& other) noexcept
            { return true; }

            template <typename _U1, typename _U2>
            inline bool
            operator!=(const _Allocator<_U2>& other) noexcept
            { return false; }
#else
            template <typename _U1, typename _U2>
            inline constexpr bool
            operator==(const _Allocator<_U2>& other) noexcept
            { return true; } 

#endif

        private:
            static idc::_MemoryPool __mempool;
    };

    template <typename T>
    idc::_MemoryPool _Allocator<T>::__mempool = idc::_MemoryPool(64, 1024);

} // !namespace idc


#endif // !_ALLOCATOR_HPP