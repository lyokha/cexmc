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
#include "CexmcCommon.hh"


enum  CexmcExceptionType
{
    CexmcUnknownException,
    CexmcPreinitException,
    CexmcWeirdException
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
        return CEXMC_LINE_START "Exception in PreInit phase. "
               "Check your preinit macro validity.";
    case CexmcWeirdException :
        return CEXMC_LINE_START "A weird exception occured. "
                                "The world must collapse now.";
    default :
        return CEXMC_LINE_START "Unknown exception.";
    }
}


#endif

