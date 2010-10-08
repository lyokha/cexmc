/*
 * =============================================================================
 *
 *       Filename:  CexmcException.hh
 *
 *    Description:  cexmc exceptions
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
    CexmcSystemException,
    CexmcEventActionIsNotInitialized,
    CexmcCmdLineParseException,
    CexmcPreinitException,
    CexmcFileCompressException,
    CexmcReadProjectIncomplete,
    CexmcProjectExists,
    CexmcCmdIsNotAllowed,
    CexmcBadAngularRange,
    CexmcBadThreshold,
    CexmcBadCalorimeterTriggerAlgorithm,
    CexmcBadOCVetoAlgorithm,
    CexmcBadOCVetoFraction,
    CexmcCalorimeterRegionNotInitialized,
    CexmcCalorimeterGeometryDataNotInitialized,
    CexmcKinematicsException,
    CexmcPoorEventData,
    CexmcIncompatibleGeometry,
    CexmcIncompleteProductionModel,
    CexmcIncompatibleProductionModel,
    CexmcBeamAndIncidentParticlesMismatch,
#ifdef CEXMC_USE_CUSTOM_FILTER
    CexmcCFBadSource,
    CexmcCFParseError,
    CexmcCFUninitialized,
    CexmcCFUninitializedVector,
    CexmcCFUnexpectedContext,
    CexmcCFUnexpectedFunction,
    CexmcCFUnexpectedVariable,
    CexmcCFUnexpectedVariableUsage,
    CexmcCFUnexpectedVectorIndex,
#endif
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


#endif

