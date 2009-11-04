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


class  CexmcRunManager : public G4RunManager
{
    public:
        CexmcRunManager();

        virtual ~CexmcRunManager();

    public:
        void  SetProductionModelType( const G4String &  productionModelType_ );

        void  SetGdmlFileName( const G4String &  gdmlFileName_ );

        CexmcProductionModelType  GetProductionModelType( void ) const;

        G4String                  GetGdmlFileName( void ) const;

    private:
        CexmcProductionModelType  productionModelType;

        G4String                  gdmlFileName;

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


inline CexmcProductionModelType
                        CexmcRunManager::GetProductionModelType( void ) const
{
    return productionModelType;
}


inline G4String  CexmcRunManager::GetGdmlFileName( void ) const
{
    return gdmlFileName;
}


#endif

