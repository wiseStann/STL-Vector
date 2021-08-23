#ifndef _BASIC_VECTOR_HPP
#define _BASIC_VECTOR_HPP

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <utility>
#include <vector>
#include <iterator>
#include <type_traits>



// prints what function has thrown an error / a warning
#define __error_prepare() {} __helper("[ERROR]", __FUNCTION__)
#define __warning_prepare(__access) {} __helper("[WARNING]", __FUNCTION__, __access)

// the function that prints that information
inline void __helper(const char* _type, const char* _func, bool _access = 1)
{
    if (_access)
        std::cout << _type << " thrown in: " << _func << "(...)" << " function" << std::endl;
}


// Independent developments collection
namespace idc
{

    enum _Status_codes {
        // status for warnings
        STATUS_SUCCESS,

        // status for errors
        STATUS_FAILURE,

        // status for both arnings and errors
        STATUS_ZERO,

        STATUS_MAX_CAPACITY_EXCEEDED,
        STATUS_POP_FROM_EMPTY,
        STATUS_TRY_GET_FROM_EMPTY,
        STATUS_OUT_OF_BOUNDS,
    };

    struct _Status_code {
        public:
            _Status_code() noexcept :
                __code(STATUS_ZERO) { }

            _Status_code(_Status_codes _code) :
                __code(_code) { }

    #if __cplusplus >= 201103L
            _Status_code(_Status_codes&& _code) :
                __code(_code) { }
    #endif

            // give an explanation of the last executed operation
            std::string info() const noexcept
            {
                switch (__code) {
                    case STATUS_FAILURE:
                        return "Vector: last operation [failed], aborted, code "
                                + std::to_string(__code); break;
                    case STATUS_ZERO:
                        return "Vector: no operations were executed, code "
                                + std::to_string(__code); break;
                    case STATUS_SUCCESS:
                        return "Vector: last operation [completed successfully], code "
                                + std::to_string(__code); break;
                    case STATUS_MAX_CAPACITY_EXCEEDED:
                        return "Vector: last operation [maximal capacity exceeded], code "
                                + std::to_string(__code); break;
                    case STATUS_POP_FROM_EMPTY:
                        return "Vector: last operation [poped from empty container], code "
                                + std::to_string(__code); break;
                    case STATUS_TRY_GET_FROM_EMPTY:
                        return "Vector: last operation [try to get from empty container], code "
                                + std::to_string(__code); break;
                    case STATUS_OUT_OF_BOUNDS:
                        return "Vector: last operation [index is out of bounds], code "
                                + std::to_string(__code); break;
                }
                return "Vector: [code is inactive], code " + std::to_string(__code);
            }

            // returns the digit that corresponds to set code
            _Status_codes get_code() const noexcept
            { return __code; }

        private:
            _Status_codes __code;
    };


    class _Error {
        public:            
            _Error() noexcept :
                __err_message(), __status() {}

            explicit
            _Error(const std::string& _err_mess, _Status_codes _status) noexcept :
                __err_message(_err_mess),
                __status(_Status_code(_status)) { }

            explicit
            _Error(_Status_codes _status) noexcept :
                __err_message(),
                __status(_status) { }


#if __cplusplus >= 201103L
            explicit
            _Error(std::string&& _err_mess, _Status_codes&& _status) noexcept :
                __err_message(std::move(_err_mess)),
                __status(std::move(_Status_code(_status))) { }

            explicit
            _Error(_Status_codes&& _status) noexcept :
                __err_message(),
                __status(std::move(_Status_code(_status))) { }
#endif
            // returns current status 
            _Status_code get_status() const noexcept
            { return __status; }

            // throws an error message and code and exits the program
            void exec() const noexcept
            {
                std::cout << __status.info() << std::endl;
                if (__err_message.size())
                    std::cout  << "Hint: " << __err_message << std::endl;
                std::cout << std::endl;
                exit(__status.get_code());
            }
        
        private:
            std::string __err_message;
            _Status_code __status;
    };


    class _Warning {
        public:
            _Warning() noexcept :
                __warn_message(), __status() { }

            explicit
            _Warning(const std::string& _warn_mess, _Status_codes _status) noexcept :
                __warn_message(_warn_mess),
                __status(_Status_code(_status)) { }
            
            explicit
            _Warning(_Status_codes _status) noexcept :
                __warn_message(),
                __status(_status) { }


#if __cplusplus >= 201103L
            explicit
            _Warning(std::string&& _warn_mess, _Status_codes&& _status) noexcept :
                __warn_message(std::move(_warn_mess)),
                __status(std::move(_Status_code(_status))) { }
            
            explicit
            _Warning(_Status_codes&& _status) noexcept :
                __warn_message(),
                __status(std::move(_Status_code(_status))) { }
#endif
            // returns current status 
            _Status_code get_code() const noexcept
            { return __status; }

            // shows the warning message
            void show() const noexcept {
                std::cout << __status.info() << std::endl;
                if (__warn_message.size())
                    std::cout  << "Hint: " << __warn_message << std::endl;
                std::cout << std::endl;
            }
        
        private:
            std::string __warn_message;
            _Status_code __status;
    };

} // !idc namespace


#endif // !_BASIC_VECTOR_HPP