/*
 * ============================================================================
 *
 *       Filename:  CexmcException.cc
 *
 *    Description:  cexmc exceptions
 *
 *        Version:  1.0
 *        Created:  23.12.2009 13:42:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * ============================================================================
 */

#include "CexmcException.hh"
#include "CexmcCommon.hh"


CexmcException::CexmcException( CexmcExceptionType  type ) : type( type )
{
}


CexmcException::~CexmcException() throw()
{
}


const char *  CexmcException::what( void ) const throw()
{
    switch ( type )
    {
    case CexmcCmdLineParseException :
        return CEXMC_LINE_START "Exception in command line arguments. "
               "Check your command line arguments.";
    case CexmcPreinitException :
        return CEXMC_LINE_START "Exception in PreInit phase. "
               "Check your preinit macro validity.";
    case CexmcFileCompressException :
        return CEXMC_LINE_START "File compress exception. "
               "Check if the file exists and output directory is writable.";
    case CexmcReadProjectIncompleteException :
        return CEXMC_LINE_START "Read project is incomplete. "
               "Check if the read project has complete data files.";
    case CexmcWeirdException :
        return CEXMC_LINE_START "A weird exception occured. "
                "The world must collapse now.";
    default :
        return CEXMC_LINE_START "Unknown exception.";
    }
}

