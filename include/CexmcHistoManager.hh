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
    CexmcEDInLeftCalorimeter_EDT_Histo,
    CexmcEDInRightCalorimeter_EDT_Histo,
    CexmcTPInMonitor_TPT_Histo
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

        void  Add( CexmcHistoType, G4double  x, G4double  y );

        void  List( void ) const;

        void  Print( const G4String &  value );

    private:
        TFile *                              outFile;

        TH2F *                               edcl_edt;

        TH2F *                               edcr_edt;

        TH2F *                               tpmon_tpt;

    private:
        std::map< CexmcHistoType, TH1 * > *  histos;

    private:
        CexmcHistoManagerMessenger *         messenger;

    private:
        static CexmcHistoManager *           instance;

        static bool                          isInitialized;
};


#endif

