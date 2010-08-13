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
    case CexmcSystemException :
        return CEXMC_LINE_START "Unspecified system exception.";
    case CexmcCmdLineParseException :
        return CEXMC_LINE_START "Exception in command line arguments. "
               "Check your command line arguments.";
    case CexmcPreinitException :
        return CEXMC_LINE_START "Exception in PreInit phase. "
               "Probably you did not specify preinit macro or production model "
               "in it.";
    case CexmcFileCompressException :
        return CEXMC_LINE_START "File compress exception. "
               "Check if the file exists and output directory is writable.";
    case CexmcReadProjectIncompleteException :
        return CEXMC_LINE_START "Read project is incomplete. "
               "Check if the read project has complete data files.";
    case CexmcProjectExists :
        return CEXMC_LINE_START "Project you try to write exists. "
               "Use command line '-y' to force project override.";
    case CexmcCmdIsNotAllowed :
        return CEXMC_LINE_START "Command is not allowed here. "
                "Probably you try to issue a command which will override "
                "read-only settings in the read project).";
    case CexmcBadAngularRange :
        return CEXMC_LINE_START "Bad angular range. "
                "Check specified angular ranges (probably they mismatch ranges "
                "in the read project).";
    case CexmcBadThreshold :
        return CEXMC_LINE_START "Bad threshold. "
                "Check specified thresholds (probably they mismatch thresholds "
                "in the read project).";
    case CexmcBadCalorimeterTriggerAlgorithm :
        return CEXMC_LINE_START "Bad calorimeter trigger algorithm. "
                "Check specified calorimeter trigger algorithm (probably it "
                "mismatches algorithm in the read project).";
    case CexmcBadOCVetoAlgorithm :
        return CEXMC_LINE_START "Bad outer crystals veto algorithm. "
                "Check specified outer crystals veto algorithm (probably it "
                "mismatches algorithm in the read project).";
    case CexmcBadOCVetoFraction :
        return CEXMC_LINE_START "Bad outer crystals veto fraction. "
                "Check specified outer crystals veto fraction (probably it "
                "has higher value than in the read project).";
    case CexmcCalorimeterRegionNotInitialized :
        return CEXMC_LINE_START "Calorimeter sensitive region was not "
                "initialized. Check gdml source file.";
#ifdef CEXMC_USE_CUSTOM_FILTER
    case CexmcCFBadSource :
        return CEXMC_LINE_START "Custom filter source file does not exist or "
                "damaged. Check your command line arguments or source file.";
    case CexmcCFParseError :
        return CEXMC_LINE_START "Custom filter source file contains errors. "
                "Check your custom filter source file.";
    case CexmcCFUninitialized :
        return CEXMC_LINE_START "Custom filter was not properly initialized.";
    case CexmcCFUninitializedVector :
        return CEXMC_LINE_START "Custom filter: A vector variable was not "
                "initialized.";
    case CexmcCFUnexpectedContext :
        return CEXMC_LINE_START "Custom filter: a variable or function call is "
                "in wrong type context (scalar when a vector is expected or "
                "vice versa). Check your custom filter script.";
    case CexmcCFUnexpectedFunction :
        return CEXMC_LINE_START "Custom filter: a function is unknown or in "
                "wrong type context. Check your custom filter script.";
    case CexmcCFUnexpectedVariable :
        return CEXMC_LINE_START "Custom filter: a variable is unknown or in "
                "wrong type context. Check your custom filter script.";
    case CexmcCFUnexpectedVariableUsage :
        return CEXMC_LINE_START "Custom filter: a variable is used in wrong "
                "type context (scalar when a vector is expected or vice "
                "versa). Check your custom filter script.";
    case CexmcCFUnexpectedVectorIndex :
        return CEXMC_LINE_START "Custom filter: a vector variable with wrong "
                "index. Indices of vectors should start from 1. Check your "
                "custom filter script.";
#endif
    case CexmcWeirdException :
        return CEXMC_LINE_START "A weird exception occured. "
                "The world must collapse now.";
    default :
        return CEXMC_LINE_START "Unknown exception.";
    }
}

