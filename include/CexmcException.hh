/*
 * =============================================================================
 *
 *       Filename:  CexmcException.hh
 *
 *    Description:  exceptions
 *
 *        Version:  1.0
 *        Created:  04.11.2009 00:00:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_EXCEPTION_HH
#define CEXMC_EXCEPTION_HH

#include <stdexcept>


enum  CexmcExceptionType
{
    CexmcUnknownException,
    CexmcPreinitException
};


class  CexmcException : public std::exception
{
    public:
        explicit CexmcException( CexmcExceptionType  type );

        ~CexmcException() throw();

    public:
        const char *  what( void ) const throw();

    private:
        CexmcExceptionType  type;
};


inline CexmcException::CexmcException( CexmcExceptionType  type ) : type( type )
{
}


inline CexmcException::~CexmcException() throw()
{
}


inline const char *  CexmcException::what( void ) const throw()
{
    switch ( type )
    {
    case CexmcPreinitException :
        return "--Cexmc-- Exception in PreInit phase. "
               "Check your preinit macro validity.";
    default :
        return "--Cexmc-- Unknown exception";
    }
}


#endif

