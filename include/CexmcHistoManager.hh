/*
 * =============================================================================
 *
 *       Filename:  CexmcHistoManager.hh
 *
 *    Description:  histograming manager (singleton)
 *
 *        Version:  1.0
 *        Created:  26.11.2009 20:55:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_HISTO_MANAGER_HH
#define CEXMC_HISTO_MANAGER_HH

#include <map>

class  TFile;
class  TH1;
class  TH2F;
class  CexmcHistoManagerMessenger;


enum  CexmcHistoType
{
    CexmcEnergyDepositInLeftCalorimeter,
    CexmcEnergyDepositInRightCalorimeter
};


class  CexmcHistoManager
{
    public:
        static CexmcHistoManager *  Instance( void );

        static void                 Destroy( void );

        static void                 Initialize( void );

    private:
        CexmcHistoManager();

        ~CexmcHistoManager();

    public:
        void  Add( CexmcHistoType  histoType, G4int  binX, G4int  binY,
                   G4double  value );

        void  List( void ) const;

        void  Print( const G4String &  value );

    public:
        TFile *                              outFile;

        TH2F *                               edInLeftCalorimeter;

        TH2F *                               edInRightCalorimeter;

    private:
        std::map< CexmcHistoType, TH1 * > *  histos;

    private:
        CexmcHistoManagerMessenger *         messenger;

    private:
        static CexmcHistoManager *           instance;

        static bool                          isInitialized;
};


#endif

