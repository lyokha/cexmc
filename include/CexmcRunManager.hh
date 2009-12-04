/*
 * =============================================================================
 *
 *       Filename:  CexmcRunManager.hh
 *
 *    Description:  run manager
 *
 *        Version:  1.0
 *        Created:  03.11.2009 20:17:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_RUN_MANAGER_HH
#define CEXMC_RUN_MANAGER_HH

#include <G4RunManager.hh>
#include "CexmcCommon.hh"

class  CexmcRunManagerMessenger;


enum  CexmcEventCountPolicy
{
    CexmcCountAllEvents,
    CexmcCountEventsWithInteraction,
    CexmcCountEventsWithTrigger
};


class  CexmcRunManager : public G4RunManager
{
    public:
        CexmcRunManager();

        virtual ~CexmcRunManager();

    public:
        void  SetProductionModelType( const G4String &  productionModelType_ );

        void  SetGdmlFileName( const G4String &  gdmlFileName_ );

        void  SaveResults( G4bool  saveResults_ );

        void  SetResultsDir( const G4String &  resultsDir_ );

        void  SetRunId( const G4String &  runId_ );

        void  SetEventCountPolicy( const G4String &  eventCountPolicy_ );

    public:
        CexmcProductionModelType  GetProductionModelType( void ) const;

        G4String                  GetGdmlFileName( void ) const;

        G4bool                    ResultsAreSaved( void ) const;

        G4String                  GetResultsDir( void ) const;

        G4String                  GetRunId( void ) const;

    protected:
        void  DoEventLoop( G4int  nEvent, const char *  macroFile,
                           G4int  nSelect );

    private:
        void  UpdateScoringMy( void );

    private:
        CexmcProductionModelType  productionModelType;

        G4String                  gdmlFileName;

        G4bool                    saveResults;

        G4String                  resultsDir;

        G4String                  runId;

        CexmcEventCountPolicy     eventCountPolicy;

    private:
        CexmcRunManagerMessenger *  messenger;
};


inline void  CexmcRunManager::SetProductionModelType(
                                        const G4String &  productionModelType_ )
{
    do
    {
        if ( productionModelType_ == "pi0" )
        {
            productionModelType = CexmcPionZeroProduction;
            break;
        }
        if ( productionModelType_ == "eta" )
        {
            productionModelType = CexmcEtaProduction;
            break;
        }
        productionModelType = CexmcUnknownProductionModel;
    } while ( false );
}


inline void  CexmcRunManager::SetGdmlFileName( const G4String &  gdmlFileName_ )
{
    gdmlFileName = gdmlFileName_;
}


inline void  CexmcRunManager::SaveResults( G4bool  saveResults_ )
{
    saveResults = saveResults_;
}


inline void  CexmcRunManager::SetResultsDir( const G4String &  resultsDir_ )
{
    resultsDir = resultsDir_;
}


inline void  CexmcRunManager::SetRunId( const G4String &  runId_ )
{
    runId = runId_;
}


inline void  CexmcRunManager::SetEventCountPolicy(
                                        const G4String & eventCountPolicy_ )
{
    do
    {
        if ( eventCountPolicy_ == "all" )
        {
            eventCountPolicy = CexmcCountAllEvents;
            break;
        }
        if ( eventCountPolicy_ == "interaction" )
        {
            eventCountPolicy = CexmcCountEventsWithInteraction;
            break;
        }
        if ( eventCountPolicy_ == "trigger" )
        {
            eventCountPolicy = CexmcCountEventsWithTrigger;
            break;
        }
    } while ( false );
}


inline CexmcProductionModelType
                        CexmcRunManager::GetProductionModelType( void ) const
{
    return productionModelType;
}


inline G4String  CexmcRunManager::GetGdmlFileName( void ) const
{
    return gdmlFileName;
}


inline G4bool  CexmcRunManager::ResultsAreSaved( void ) const
{
    return saveResults;
}


inline G4String  CexmcRunManager::GetResultsDir( void ) const
{
    return resultsDir;
}


inline G4String  CexmcRunManager::GetRunId( void ) const
{
    return runId;
}


#endif

