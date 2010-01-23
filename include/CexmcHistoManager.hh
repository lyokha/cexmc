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

#include <vector>
#include <map>
#include "CexmcAngularRange.hh"

class  TFile;
class  TH1;
class  CexmcHistoManagerMessenger;


enum  CexmcHistoType
{
    CexmcEDInLeftCalorimeter_EDT_Histo,
    CexmcEDInRightCalorimeter_EDT_Histo,
    CexmcTPInMonitor_TPT_Histo,
    CexmcRecMasses_EDT_Histo,
    CexmcRecMasses_RT_Histo,
    CexmcRecMassOP_ARReal_RT_Histo,
    CexmcRecMassNOP_ARReal_RT_Histo,
    CexmcRecMassOP_ARRec_RT_Histo,
    CexmcRecMassNOP_ARRec_RT_Histo
};


typedef std::vector< TH1 * >                             CexmcHistoVector;
typedef std::map< CexmcHistoType, CexmcHistoVector * >   CexmcHistosMap;
typedef std::pair< CexmcHistoType, CexmcHistoVector * >  CexmcHistoPair;


class  CexmcHistoManager
{
    public:
        static CexmcHistoManager *  Instance( void );

        static void                 Destroy( void );

    private:
        CexmcHistoManager();

        ~CexmcHistoManager();

    public:
        void  Initialize( void );

        void  SetupARHistos( const CexmcAngularRangeList &  aRanges );

        void  AddARHistos( const CexmcAngularRange &  aRange );

        void  Add( CexmcHistoType  histoType, unsigned int  index, G4int  binX,
                   G4int  binY, G4double  value );

        void  Add( CexmcHistoType  histoType, unsigned int  index,
                   G4double  x );

        void  Add( CexmcHistoType  histoType, unsigned int  index, G4double  x,
                   G4double  y );

        void  List( void ) const;

        void  Print( const G4String &  value );

    private:
        TFile *                              outFile;

        CexmcHistoVector                     edcl_edt;

        CexmcHistoVector                     edcr_edt;

        CexmcHistoVector                     tpmon_tpt;

        CexmcHistoVector                     recmasses_edt;

        CexmcHistoVector                     recmasses_rt;

        CexmcHistoVector                     recmassop_arreal_rt;

        CexmcHistoVector                     recmassnop_arreal_rt;

        CexmcHistoVector                     recmassop_arrec_rt;

        CexmcHistoVector                     recmassnop_arrec_rt;

    private:
        CexmcHistosMap                       histos;

        bool                                 isInitialized;

    private:
        CexmcHistoManagerMessenger *         messenger;

    private:
        static CexmcHistoManager *           instance;
};


#endif

