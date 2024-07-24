/*
 * created by: shunnobir
 * date: 24/07/2024
 */


#include "IError.hpp"
std::string const Step::IError::BOLD    = 
#if defined (__linux__) || defined (MAC)
                "\x1b[1m"
#else
                ""
#endif
                ;
std::string const Step::IError::RED     = 
#if defined (__linux__) || defined (MAC)
                "\x1b[31m"
#else
                ""
#endif
                ;
std::string const Step::IError::GREEN   = 
#if defined (__linux__) || defined (MAC)
                "\x1b[32m"
#else
                ""
#endif
                ;
std::string const Step::IError::YELLOW  = 
#if defined (__linux__) || defined (MAC)
                "\x1b[33m"
#else
                ""
#endif
                ;
std::string const Step::IError::BLUE    = 
#if defined (__linux__) || defined (MAC)
                "\x1b[34m"
#else
                ""
#endif
                ;
std::string const Step::IError::PURPLE  = 
#if defined (__linux__) || defined (MAC)
                "\x1b[35m"
#else
                ""
#endif
                ;
std::string const Step::IError::TEAL    = 
#if defined (__linux__) || defined (MAC)
                "\x1b[36m"
#else
                ""
#endif
                ;
std::string const Step::IError::NORMAL  = 
#if defined (__linux__) || defined (MAC)
                "\x1b[m"
#else
                ""
#endif
                ;
