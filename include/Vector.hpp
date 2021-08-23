#include "Basic.hpp"
#include "Allocator.hpp"
#include "Iterator.hpp"

#define _VECTOR_HPP


// Independent developments collection
namespace idc
{

   template <typename _Tp, class _Allocator = idc::_Allocator<_Tp>>
   struct _VectorBase
   {
      typedef typename _Allocator::template rebind<_Tp>::other  _Tp_alloc_type;
      typedef typename std::allocator_traits<_Tp_alloc_type>    _alloc_traits;
      typedef typename _alloc_traits::size_type                 size_type;
      typedef typename _alloc_traits::pointer                   pointer;

      struct _BaseImplData
      {
         _BaseImplData() noexcept :
            __mem_begin(), __mem_end(), __mem_allocated() { }
         
         _BaseImplData(const _BaseImplData& _other) :
            __mem_begin(_other.__mem_begin), __mem_end(_other.__mem_end),
               __mem_allocated(_other.__mem_allocated)
            { __mem_begin = __mem_end = __mem_allocated = pointer(); }

#if __cplusplus >= 201103L
         _BaseImplData(_BaseImplData&& _other) noexcept :
            __mem_begin(_other.__mem_begin), __mem_end(_other.__mem_end),
               __mem_allocated(_other.__mem_allocated)
            { __mem_begin = __mem_end = __mem_allocated = pointer(); }
#endif
         void _data_copy(const _BaseImplData& _other) noexcept
         {
            this->__mem_begin = _other.__mem_begin;
            this->__mem_end = _other.__mem_end;
            this->__mem_allocated = _other.__mem_allocated;
         }

         void _data_swap(const _BaseImplData& _other) noexcept
         {
            _BaseImplData temp;
            temp._data_copy(*this);
            _data_copy(_other);
            _other._data_copy(temp);
         }

         pointer __mem_begin;
         pointer __mem_end;
         pointer __mem_allocated;

      }; // !_BaseImplData

      struct _BaseNotice
      {
         public:
            _BaseNotice() noexcept :
               __err(), __warn() { }
            
            // returns the warning code
            idc::_Status_code _get_warns_status() const noexcept
            { return __warn.get_code(); }
            
            // sets the status of the last operation
            void _set_status(idc::_Status_codes _status) noexcept
            { this->__warn = idc::_Warning(_status); }

            // setting a warning code
            void _throw_warning(const std::string& _err_mess, idc::_Status_codes _code) noexcept
            {
               this->__warn = idc::_Warning(_err_mess, _code);
               this->__warn.show();
            }

            // setting a warning code
            void _throw_warning(idc::_Status_codes _code) noexcept
            {
               this->__warn = idc::_Warning(_code);
               this->__warn.show();
            }

            // setting an error code
            void _throw_error(const std::string& _err_mess, idc::_Status_codes _code) noexcept
            {
               this->__err = idc::_Error(_err_mess, _code);
               this->__err.exec();
            }

            // setting an error code
            void _throw_error(idc::_Status_codes _code) noexcept
            {
               this->__err = idc::_Error(_code);
               this->__err.exec();
            }

            // returns current warnings status (allowed / disallowed)
            bool _warns_allowance() const noexcept
            { return __allow_warnings; }

            // enables/disables warnings throwing
            void _enable_warnings() noexcept { __allow_warnings = true; }
            void _disable_warnings() noexcept { __allow_warnings = false; }

         private:
            idc::_Error __err;
            idc::_Warning __warn;
            bool __allow_warnings = false;

      }; // !_BaseNotice


      struct _BaseImpl : public _BaseImplData,
         public _BaseNotice, public _Tp_alloc_type
      {

      }; // !_BaseImpl


      public:
         _BaseImpl _baseimpl;

         _Tp_alloc_type& __Get_Tp_allocator() noexcept
         { return this->_baseimpl; }

         const _Tp_alloc_type& __Get_Tp_allocator() const noexcept
         { return this->_baseimpl; }

         _Allocator __get_row_allocator() noexcept
         { return _Allocator(this->__Get_Tp_allocator()); }


#if __cplusplus >= 201103L
         _VectorBase() = default;
#else
         _VectorBase() { }
#endif

         _VectorBase(size_type _n, const _Tp_alloc_type& _al) :
            _baseimpl(_al)
         { __mem_create(_n); }


         ~_VectorBase() noexcept
         { __Mem_deallocate(_baseimpl.__mem_begin, _baseimpl.__mem_allocated - _baseimpl.__mem_begin); }


         pointer __Mem_allocate(size_type _n) noexcept
         { return _n != 0 ? _alloc_traits::allocate(_baseimpl, _n) : pointer(); }

         void __Mem_deallocate(pointer _ptr, size_type _n) noexcept
         { 
            if (_ptr)
               _alloc_traits::deallocate(_baseimpl, _ptr, _n);
         }


      private:
         void __mem_create(size_type _n)
         {
            this->_baseimpl.__mem_begin = __Mem_allocate(_n);
            this->_baseimpl.__mem_end = this->_baseimpl.__mem_begin;
            this->_baseimpl.__mem_allocated = this->_baseimpl.__mem_begin + _n;
         }

   }; // !_VectorBase


   // Vector linear data structure
   template <typename _Tp, class _Allocator = idc::_Allocator<_Tp>> 
   class Vector : protected _VectorBase<_Tp>
   {
      public:
         typedef _Tp                                   value_type;
         typedef size_t                                size_type;
         typedef std::ptrdiff_t                        difference_type;

         typedef _VectorBase<_Tp, _Allocator>          _Base;
         typedef typename _Base::_Tp_alloc_type        allocator_type;
         typedef std::allocator_traits<allocator_type> _alloc_traits;

         typedef value_type&                           reference;
         typedef const value_type&                     const_reference;
         typedef typename _alloc_traits::pointer       pointer;
         typedef typename _alloc_traits::const_pointer const_pointer;

         typedef idc::_Iterator<pointer, Vector>       iterator;
         typedef idc::_Iterator<const_pointer, Vector> const_iterator;
         typedef idc::_ReverseIterator<iterator>       reverse_iterator;
         typedef idc::_ReverseIterator<const_iterator> const_reverse_iterator;

         static const size_type mallocMinInPlaceExpandable = 4096;

      protected:
         using _Base::_baseimpl;
         using _Base::__Mem_allocate;
         using _Base::__Mem_deallocate;
         using _Base::__Get_Tp_allocator;
      public:
         Vector() {};
      
      public:

         // displays the vector to console in readable view
         constexpr void display() noexcept
         {
            std::cout << "\nVector: {" << std::endl;
            std::cout << "    contents: [";
            for (auto it = begin(); it != end(); it++) {
               if ((it + 1) != end()) std::cout << *it << ", ";
               else std::cout << *it;
            }
            std::cout << "]," << std::endl;
            std::cout << "    size: " << size() << "," << std::endl;
            std::cout << "    capacity: " << capacity() << "," << std::endl;
            std::cout << "}" << std::endl << std::endl;
         }

         // returns exact size of the vector<value_type>
         size_type size() const noexcept
         { return size_type(this->_baseimpl.__mem_end - this->_baseimpl.__mem_begin); }

         // returns the capacity of the vector
         size_type capacity() const noexcept
         { return size_type(this->_baseimpl.__mem_allocated - this->_baseimpl.__mem_begin); }

         // returns maximal number of elements that we can store in vector<value_type>
         size_type max_size() const noexcept;

         // returns true if vector contains no elements
         bool is_empty() const noexcept
         { return this->_baseimpl.__mem_begin == this->_baseimpl.__mem_end; }


         // NOTIFICATIONS
         // allows to throw warnings
         void allow_warnings() noexcept
         { this->_baseimpl._enable_warnings(); }

         // declines to throw warnings
         void decline_warnings() noexcept
         { this->_baseimpl._disable_warnings(); }

         // returns the current warnings status
         bool warns_status() const noexcept
         { return this->_baseimpl._warns_allowance(); }

         // returns the code information of the last executed operation
         std::string code() const noexcept
         {
            const idc::_Status_code code = this->_baseimpl._get_warns_status();
            return code.info();
         }


         // ITERATORS
         // returns a read/write iterator that points to the first element in the Vector
         iterator begin() noexcept
         { return iterator(this->_baseimpl.__mem_begin); }

         // returns a read-only iterator that pointer to the first element in the Vector
         const_iterator cbegin() const noexcept
         { return const_iterator(this->_baseimpl.__mem_begin); }

         // returns a read/write iterator that points to the last element in the Vector
         iterator end() noexcept
         { return iterator(this->_baseimpl.__mem_end); }

         // returns a read-only iterator that pointer to the last element in the Vector
         const_iterator cend() const noexcept
         { return const_iterator(this->_baseimpl.__mem_end); }


         // ACCESS
         // returns an element or throws a warning
         reference operator[](size_type);
         const_reference operator[](size_type) const;

         // returns an element or throws a warning
         reference front();
         const_reference front() const;

         // return an element or throws a warning
         reference back();
         const_reference back() const;


         // HELPERS
         // 
         size_type _check_len(size_type);

#if __cplusplus >= 201103L
         //
         static constexpr bool _S_nothrow_relocate(std::true_type);
         //
         static constexpr bool _S_nothrow_relocate(std::false_type);

         //
         static constexpr bool _S_use_relocate();

         //
         template<typename _Tn, typename _Up, typename _Alloc>
         inline void __relocate_object_a(_Tn* __restrict __dest, _Up* __restrict __orig, _Alloc& __alloc)
         noexcept(noexcept(std::allocator_traits<_Alloc>::construct(__alloc, __dest, std::move(*__orig)))
               && noexcept(std::allocator_traits<_Alloc>::destroy(__alloc, std::__addressof(*__orig))))
         {
            typedef std::allocator_traits<_Alloc> __traits;
            __traits::construct(__alloc, __dest, std::move(*__orig));
            __traits::destroy(__alloc, std::__addressof(*__orig));
         }

         //
         template <typename _Tn, typename _Up>
         inline _Tn* __relocate_a_1(_Tn*, _Tn*, _Tn*, std::allocator<_Up>&) noexcept;

         //
         template <typename _InputIterator, typename _ForwardIterator, typename _Alloc>
         inline _ForwardIterator __relocate_a_1(_InputIterator, _InputIterator, _ForwardIterator, _Alloc&);

         //
         static pointer _S_do_relocate(pointer, pointer, pointer, allocator_type&, std::true_type) noexcept;

         //
         static pointer _S_do_relocate(pointer, pointer, pointer, allocator_type&, std::false_type) noexcept;

         //
         static pointer _S_relocate(pointer, pointer, pointer, allocator_type&) noexcept;
#endif // !CPP11

         // reallocates memory and pushes element to the end
#if __cplusplus >= 201103L
         template<typename... _Args>
         void _realloc_insert(iterator, _Args&&...);
#else
         void _realloc_insert(iterator, const_reference);
#endif

         // MODIFIERS

         // pushs a given enitity to the end of the Vector
         void push_back(const_reference);

#if __cplusplus >= 201103L
         void push_back(value_type&&);
#endif

         template <typename ... _Args>
#if __cplusplus > 201402L
         reference
#else
         void
#endif
         // emplaces back given arguments by forwarding
         emplace_back(_Args&&...);

         // pops back of the vector
         void pop_back();

         // clears the vector
         void clear() noexcept;

   };

} // !idc namespace

