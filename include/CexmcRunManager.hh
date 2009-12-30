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

#include <set>
#include <boost/archive/binary_oarchive.hpp>
#include <G4RunManager.hh>
#include "CexmcRunSObject.hh"
#include "CexmcException.hh"
#include "CexmcCommon.hh"

class  CexmcRunManagerMessenger;
class  CexmcPhysicsManager;


typedef std::set< CexmcOutputDataType >  CexmcOutputDataTypeSet;


class  CexmcRunManager : public G4RunManager
{
    public:
        explicit CexmcRunManager( const G4String &  projectId = "",
                                  const G4String &  rProject = "",
                                  G4bool  overrideExistingProject = false );

        virtual ~CexmcRunManager();

    public:
        void  SetPhysicsManager( CexmcPhysicsManager *  physicsManager_ );

        void  SetProductionModelType(
                            CexmcProductionModelType  productionModelType_ );

        void  SetGdmlFileName( const G4String &  gdmlFileName_ );

        void  SetEventCountPolicy( CexmcEventCountPolicy  eventCountPolicy_ );

        void  ReadProject( void );

        void  SaveProject( void );

        void  PrintReadData( void ) const;

        void  PrintReadData( const CexmcOutputDataTypeSet &  outputData ) const;

    public:
        CexmcProductionModelType  GetProductionModelType( void ) const;

        G4String                  GetGdmlFileName( void ) const;

        G4bool                    ProjectIsSaved( void ) const;

        G4bool                    ProjectIsRead( void ) const;

        G4String                  GetProjectsDir( void ) const;

        G4String                  GetProjectId( void ) const;

        boost::archive::binary_oarchive *  GetEventsArchive( void ) const;

    protected:
        void  DoEventLoop( G4int  nEvent, const char *  macroFile,
                           G4int  nSelect );

    private:
        void  DoCommonEventLoop( G4int  nEvent, const G4String &  cmd,
                                 G4int  nSelect );

        void  DoReadEventLoop( G4int  nEvent, const G4String &  cmd,
                               G4int  nSelect );

    private:
        void  ReadPreinitProjectData( void );

    private:
        CexmcProductionModelType    productionModelType;

        G4String                    gdmlFileName;

        G4bool                      saveProject;

        G4String                    projectsDir;

        G4String                    projectId;

        G4String                    rProject;

        CexmcEventCountPolicy       eventCountPolicy;

        CexmcRunSObject             sObject;

    private:
        G4int                       numberOfEventsProcessed;

        G4int                       numberOfEventsProcessedEffective;

    private:
        boost::archive::binary_oarchive *  eventsArchive;

    private:
        CexmcPhysicsManager *       physicsManager;

    private:
        CexmcRunManagerMessenger *  messenger;
};


inline void  CexmcRunManager::SetPhysicsManager(
                                        CexmcPhysicsManager *  physicsManager_ )
{
    physicsManager = physicsManager_;
}


inline void  CexmcRunManager::SetProductionModelType(
                                CexmcProductionModelType  productionModelType_ )
{
    if ( ProjectIsRead() )
        throw CexmcException( CexmcCmdIsNotAllowed );

    productionModelType = productionModelType_;
}


inline void  CexmcRunManager::SetGdmlFileName( const G4String &  gdmlFileName_ )
{
    if ( ProjectIsRead() )
        throw CexmcException( CexmcCmdIsNotAllowed );

    gdmlFileName = gdmlFileName_;
}


inline void  CexmcRunManager::SetEventCountPolicy(
                                CexmcEventCountPolicy  eventCountPolicy_ )
{
    eventCountPolicy = eventCountPolicy_;
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


inline G4bool  CexmcRunManager::ProjectIsSaved( void ) const
{
    return projectId != "";
}


inline G4bool  CexmcRunManager::ProjectIsRead( void ) const
{
    return rProject != "";
}


inline G4String  CexmcRunManager::GetProjectsDir( void ) const
{
    return projectsDir;
}


inline G4String  CexmcRunManager::GetProjectId( void ) const
{
    return projectId;
}


inline boost::archive::binary_oarchive *  CexmcRunManager::GetEventsArchive(
                                                                    void ) const
{
    return eventsArchive;
}


#endif

