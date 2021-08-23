#include "../include/Basic.hpp"
#include "../include/Vector.hpp"


/*
 *
 */
template <typename _Tp, class _Allocator>
size_t idc::Vector<_Tp, _Allocator>::max_size() const noexcept
{
   const size_type _ptr_diff_max = std::numeric_limits<std::ptrdiff_t>::max() / sizeof(value_type);
   const size_type _alloc_max = _alloc_traits::max_size(this->__Get_Tp_allocator());
   return std::min(_ptr_diff_max, _alloc_max);
}


/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::reference
idc::Vector<_Tp, _Allocator>::operator[](size_type __index)
{
   if (__index >= size()) {
      __error_prepare();
      this->_baseimpl._throw_error(
         "Index must be less than the vector size", STATUS_OUT_OF_BOUNDS
      );
   }
   this->_baseimpl._set_status(STATUS_SUCCESS);
   return this->_baseimpl.__mem_begin[__index];
}

/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::const_reference
idc::Vector<_Tp, _Allocator>::operator[](size_type __index) const
{
   if (__index >= size()) {
      __error_prepare();
      this->_baseimpl._throw_error(
         "Index must be less of the vector size", STATUS_OUT_OF_BOUNDS
      );
   }
   this->_baseimpl._set_status(STATUS_SUCCESS);
   return this->_baseimpl.__mem_begin[__index];
}

/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::reference
idc::Vector<_Tp, _Allocator>::front()
{
   if (is_empty()) {
      __warning_prepare(warns_status());
      this->_baseimpl._throw_warning(STATUS_TRY_GET_FROM_EMPTY);
   } else {
      this->_baseimpl._set_status(STATUS_SUCCESS);
      return *this->_baseimpl.__mem_begin;
   }
}

/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::const_reference
idc::Vector<_Tp, _Allocator>::front() const
{
   if (is_empty()) {
      __warning_prepare(warns_status());
      this->_baseimpl._throw_warning(STATUS_TRY_GET_FROM_EMPTY);
   } else {
      this->_baseimpl._set_status(STATUS_SUCCESS);
      return *this->_baseimpl.__mem_begin;
   }
}

/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::reference
idc::Vector<_Tp, _Allocator>::back()
{
   if (is_empty()) {
      __warning_prepare(warns_status());
      this->_baseimpl._throw_warning(STATUS_TRY_GET_FROM_EMPTY);
   } else {
      this->_baseimpl._set_status(STATUS_SUCCESS);
      return *this->_baseimpl.__mem_end;
   }
}

/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::const_reference
idc::Vector<_Tp, _Allocator>::back() const
{
   if (is_empty()) {
      __warning_prepare(warns_status());
      this->_baseimpl._throw_warning(STATUS_TRY_GET_FROM_EMPTY);
   } else {
      this->_baseimpl._set_status(STATUS_SUCCESS);
      return *this->_baseimpl.__mem_end;
   }
}

/*
 *
 */
template <typename _Tp, class _Allocator>
void idc::Vector<_Tp, _Allocator>::push_back(const_reference __entity)
{
   if (this->_baseimpl.__mem_end != this->_baseimpl.__mem_allocated) {
      _alloc_traits::construct(this->_baseimpl, this->_baseimpl.__mem_end, __entity);
      this->_baseimpl.__mem_end++;
   } else {
      this->_realloc_insert(end(), __entity);
   }
   this->_baseimpl._set_status(STATUS_SUCCESS);
}

/*
 *
 */
#if __cplusplus >= 201103L
   template <typename _Tp, class _Allocator>

   void idc::Vector<_Tp, _Allocator>::push_back(value_type&& __enitity)
   {
      emplace_back(std::move(__enitity));
   }
#endif

/*
 *
 */
template <typename _Tp, class _Allocator>
size_t idc::Vector<_Tp, _Allocator>::_check_len(size_type __n)
{
   if (max_size() - size() < __n) {
      __error_prepare();
      this->_baseimpl._throw_error(
         "The Vector container reached the max number of elements",
         STATUS_MAX_CAPACITY_EXCEEDED);
   }
      
   const size_type __len = size() + std::max(size(), __n);
   this->_baseimpl._set_status(STATUS_SUCCESS);
   return (__len < size() || __len > max_size()) ? max_size() : __len;
}


#if __cplusplus >= 201103L
/*
 *
 */
template <typename _Tp, class _Allocator>
constexpr bool
idc::Vector<_Tp, _Allocator>::_S_nothrow_relocate(std::true_type)
{
   return noexcept(__relocate_a_1(std::declval<pointer>(),
                     std::declval<pointer>(),
                     std::declval<pointer>(),
                     std::declval<allocator_type&>()));
}

/*
 *
 */
template <typename _Tp, class _Allocator>
constexpr bool
idc::Vector<_Tp, _Allocator>::_S_nothrow_relocate(std::false_type)
{ return false; }

/*
 *
 */
template <typename _Tp, class _Allocator>
constexpr bool
idc::Vector<_Tp, _Allocator>::_S_use_relocate()
{
// Instantiating std::__relocate_a might cause an error outside the
// immediate context (in __relocate_object_a's noexcept-specifier),
// so only do it if we know the type can be move-inserted into *this.
   return _S_nothrow_relocate(std::__bool_constant<true>{});
}

/*
 *
 */
template <typename _Tp, class _Allocator>
template <typename _Tn, typename _Up>
inline _Tn*
idc::Vector<_Tp, _Allocator>::__relocate_a_1(_Tn* __first, _Tn* __last, _Tn* __result, std::allocator<_Up>&) noexcept
{
   ptrdiff_t __count = __last - __first;
   if (__count > 0)
      __builtin_memmove(__result, __first, __count * sizeof(_Tn));
   return __result + __count;
}

/*
 *
 */
template <typename _Tp, class _Allocator>
template <typename _InputIterator, typename _ForwardIterator, typename _Alloc>
inline _ForwardIterator
idc::Vector<_Tp, _Allocator>::__relocate_a_1(_InputIterator __first, _InputIterator __last, _ForwardIterator __result, _Alloc& __alloc)
   // noexcept(noexcept(std::__relocate_object_a(std::addressof(*__result),
            // std::addressof(*__first),
            // __alloc)))
{
   typedef typename std::iterator_traits<_InputIterator>::value_type
   _ValueType;
   typedef typename std::iterator_traits<_ForwardIterator>::value_type
   _ValueType2;
   static_assert(std::is_same<_ValueType, _ValueType2>::value, "relocation is only possible for values of the same type");
   _ForwardIterator __cur = __result;
   for (; __first != __last; ++__first, (void)++__cur)
      __relocate_object_a(std::__addressof(*__cur), std::__addressof(*__first), __alloc);
   return __cur;
}

/*
 * 
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::pointer
idc::Vector<_Tp, _Allocator>::_S_do_relocate(pointer __first, pointer __last, pointer __result,
                                             allocator_type& __alloc, std::true_type) noexcept
{
   return __relocate_a_1(std::__niter_base(__first),
			    std::__niter_base(__last),
			    std::__niter_base(__result), __alloc);
   // std::__relocate_a(__first, __last, __result, __alloc);
}

/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::pointer
idc::Vector<_Tp, _Allocator>::_S_do_relocate(pointer __first, pointer __last, pointer __result,
                                             allocator_type& __alloc, std::false_type) noexcept
{ return __result; }

/*
 *
 */
template <typename _Tp, class _Allocator>
typename idc::Vector<_Tp, _Allocator>::pointer
idc::Vector<_Tp, _Allocator>::_S_relocate(pointer __first, pointer __last, pointer __result,
                                             allocator_type& __alloc) noexcept
{
   using __do_it = std::__bool_constant<true>;
   return _S_do_relocate(__first, __last, __result, __alloc, __do_it{});
}

#endif // !CPP11


/*
 *
 */
#if __cplusplus >= 201103L
   template<typename _Tp, typename _Allocator>
   template<typename... _Args>
   void idc::Vector<_Tp, _Allocator>::_realloc_insert(iterator __pos, _Args&&... __args)
#else
   template<typename _Tp, typename _Allocator>
   void idc::Vector<_Tp, _Allocator>::_realloc_insert(iterator __pos, const _Tp& __enitity)
#endif
{
   const size_type __len = _check_len(size_type(1));
   pointer __old_start = this->_baseimpl.__mem_begin;
   pointer __old_finish = this->_baseimpl.__mem_end;
   const size_type __elems_before = __pos - begin();

   pointer __new_start(__Mem_allocate(__len));
   pointer __new_finish(__new_start);
   __try {
      // The order of the three operations is dictated by the C++11
      // case, where the moves could alter a new element belonging
      // to the existing vector.  This is an issue only for callers
      // taking the element by lvalue ref (see last bullet of C++11
      // [res.on.arguments]).
      _alloc_traits::construct(this->_baseimpl, __new_start + __elems_before,
#if __cplusplus >= 201103L
                                 std::forward<_Args>(__args)...);
#else
                                 __enitity);
#endif
      __new_finish = pointer();

#if __cplusplus >= 201103L
      if (_GLIBCXX17_CONSTEXPR(_S_use_relocate())) {
         __new_finish = _S_relocate(__old_start, __pos.base(), __new_start, __Get_Tp_allocator());
         ++__new_finish;
         __new_finish = _S_relocate(__pos.base(), __old_finish, __new_finish, __Get_Tp_allocator());
      }
      else {
#endif
         __new_finish = std::__uninitialized_move_if_noexcept_a(__old_start, __pos.base(),
                                                                __new_start, __Get_Tp_allocator());
         ++__new_finish;
         __new_finish = std::__uninitialized_move_if_noexcept_a(__pos.base(), __old_finish,
                                                                __new_finish, __Get_Tp_allocator());
      }
   }
   __catch(...) {
	  if (!__new_finish)
	    _alloc_traits::destroy(this->_baseimpl, __new_start + __elems_before);
	  else
	    std::_Destroy(__new_start, __new_finish, __Get_Tp_allocator());
	  __Mem_deallocate(__new_start, __len);
	  __error_prepare();
     this->_baseimpl._throw_error("Error occured while reallocation", STATUS_FAILURE);
	}
#if __cplusplus >= 201103L
      if _GLIBCXX17_CONSTEXPR(!_S_use_relocate())
#endif
	std::_Destroy(__old_start, __old_finish, __Get_Tp_allocator());
   __Mem_deallocate(__old_start, this->_baseimpl.__mem_allocated - __old_start);
   this->_baseimpl.__mem_begin = __new_start;
   this->_baseimpl.__mem_end = __new_finish;
   this->_baseimpl.__mem_allocated = __new_start + __len;
}

/*
 *
 */
template <typename _Tp, class _Allocator>
template <typename ... _Args>
#if __cplusplus > 201402L
   typename idc::Vector<_Tp, _Allocator>::reference
#else
   void
#endif
idc::Vector<_Tp, _Allocator>::emplace_back(_Args&&... __args)
{
   if (this->_baseimpl.__mem_end != this->_baseimpl.__mem_allocated) {
      _alloc_traits::construct(this->_baseimpl, this->_baseimpl.__mem_end,
                                 std::forward<_Args>(__args)...);
      this->_baseimpl.__mem_end++;
   } else {
      this->_realloc_insert(end(), std::forward<_Args>(__args)...);
   }
   this->_baseimpl._set_status(STATUS_SUCCESS);
#if __cplusplus > 201402L
   return back();
#endif
}


/*
 *
 */
template <typename _Tp, class _Allocator>
void idc::Vector<_Tp, _Allocator>::pop_back()
{
   if (is_empty()) {
      __warning_prepare(warns_status());
      this->_baseimpl._throw_warning(
         "The container is empty, no elements can be popped", STATUS_POP_FROM_EMPTY
      );
   } else {
      this->_baseimpl.__mem_end--;
      _alloc_traits::destroy(this->_baseimpl, this->_baseimpl.__mem_end);
      this->_baseimpl._set_status(STATUS_SUCCESS);
   }
}

/*
 *
 */
template <typename _Tp, class _Allocator>
void idc::Vector<_Tp, _Allocator>::clear() noexcept
{
   std::_Destroy(this->_baseimpl.__mem_begin, this->_baseimpl.__mem_end, __Get_Tp_allocator());
   this->_baseimpl.__mem_begin = this->_baseimpl.__mem_end;
   this->_baseimpl._set_status(STATUS_SUCCESS);
}

int main()
{
   idc::Vector<int> vec;
   vec.allow_warnings();
   int a = 10;
   vec.push_back(a);
   // vec.push_back(20);
   // vec.push_back(30);
   // vec.push_back(40);
   // vec.push_back(50);
   // vec.push_back(60);
   // vec.push_back(70);

   vec.display();

   // vec.clear();
   
   vec.display();

   if (vec.is_empty())
      std::cout << vec.code() << std::endl;

   return 0;
}