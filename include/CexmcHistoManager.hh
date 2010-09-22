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

#ifdef CEXMC_USE_ROOT

#include <vector>
#include <map>
#include <Rtypes.h>
#include <G4String.hh>
#include "CexmcAngularRange.hh"
#include "CexmcCommon.hh"

class  TFile;
class  TH1;
#ifdef CEXMC_USE_ROOTQT
class  TQtWidget;
#endif
class  CexmcHistoManagerMessenger;


enum  CexmcHistoType
{
    CexmcMomentumBP_TPT_Histo,
    CexmcMomentumBP_RT_Histo,
    CexmcTPInMonitor_TPT_Histo,
    CexmcTPInTarget_TPT_Histo,
    CexmcTPInTarget_RT_Histo,
    CexmcRecMasses_EDT_Histo,
    CexmcRecMasses_RT_Histo,
    CexmcAbsorbedEnergy_EDT_Histo,
    CexmcAbsorbedEnergy_RT_Histo,
    CexmcHistoType_ARReal_START,
    CexmcRecMassOP_ARReal_RT_Histo = CexmcHistoType_ARReal_START,
    CexmcRecMassNOP_ARReal_RT_Histo,
    CexmcOPDPAtLeftCalorimeter_ARReal_EDT_Histo,
    CexmcOPDPAtRightCalorimeter_ARReal_EDT_Histo,
    CexmcOPDPAtLeftCalorimeter_ARReal_RT_Histo,
    CexmcOPDPAtRightCalorimeter_ARReal_RT_Histo,
    CexmcRecOPDPAtLeftCalorimeter_ARReal_EDT_Histo,
    CexmcRecOPDPAtRightCalorimeter_ARReal_EDT_Histo,
    CexmcRecOPDPAtLeftCalorimeter_ARReal_RT_Histo,
    CexmcRecOPDPAtRightCalorimeter_ARReal_RT_Histo,
    CexmcKinEnAtLeftCalorimeter_ARReal_TPT_Histo,
    CexmcKinEnAtRightCalorimeter_ARReal_TPT_Histo,
    CexmcKinEnAtLeftCalorimeter_ARReal_RT_Histo,
    CexmcKinEnAtRightCalorimeter_ARReal_RT_Histo,
    CexmcAbsEnInLeftCalorimeter_ARReal_EDT_Histo,
    CexmcAbsEnInRightCalorimeter_ARReal_EDT_Histo,
    CexmcAbsEnInLeftCalorimeter_ARReal_RT_Histo,
    CexmcAbsEnInRightCalorimeter_ARReal_RT_Histo,
    CexmcMissEnFromLeftCalorimeter_ARReal_RT_Histo,
    CexmcMissEnFromRightCalorimeter_ARReal_RT_Histo,
    CexmcKinEnOP_LAB_ARReal_TPT_Histo,
    CexmcKinEnOP_LAB_ARReal_RT_Histo,
    CexmcAngleOP_SCM_ARReal_TPT_Histo,
    CexmcAngleOP_SCM_ARReal_RT_Histo,
    CexmcRecAngleOP_SCM_ARReal_RT_Histo,
    CexmcDiffAngleOP_SCM_ARReal_RT_Histo,
    CexmcOpenAngle_ARReal_TPT_Histo,
    CexmcOpenAngle_ARReal_RT_Histo,
    CexmcRecOpenAngle_ARReal_RT_Histo,
    CexmcDiffOpenAngle_ARReal_RT_Histo,
    CexmcTPInTarget_ARReal_TPT_Histo,
    CexmcTPInTarget_ARReal_RT_Histo,
    CexmcHistoType_ARReal_END = CexmcTPInTarget_ARReal_RT_Histo,
    CexmcHistoType_SIZE
};


class  CexmcHistoManager
{
    private:
        typedef std::vector< TH1 * >                           CexmcHistoVector;

        typedef std::map< CexmcHistoType, CexmcHistoVector >   CexmcHistosMap;

        typedef std::pair< CexmcHistoType, CexmcHistoVector >  CexmcHistoPair;

        struct  CexmcHistoAxisData
        {
            CexmcHistoAxisData() : nBins( 0 ), nBinsMin( 0 ), nBinsMax( 0 )
            {}

            CexmcHistoAxisData( Int_t  nBins, Double_t  nBinsMin,
                                Double_t  nBinsMax ) :
                nBins( nBins ), nBinsMin( nBinsMin ), nBinsMax( nBinsMax )
            {}

            Int_t     nBins;

            Double_t  nBinsMin;

            Double_t  nBinsMax;
        };

        typedef std::vector< CexmcHistoAxisData >              CexmcHistoAxes;

        enum  CexmcHistoImpl
        {
            Cexmc_TH1F,
            Cexmc_TH2F,
            Cexmc_TH3F
        };

        struct  CexmcHistoData
        {
            CexmcHistoData() :
                type( CexmcHistoType_SIZE ), impl( Cexmc_TH1F ),
                isARHisto( false ), isARRec( false ), triggerType( CexmcTPT )
            {}

            CexmcHistoData( CexmcHistoType  type, CexmcHistoImpl  impl,
                            bool  isARHisto, bool  isARRec,
                            CexmcTriggerType  triggerType,
                            const G4String &  name, const G4String &  title,
                            const CexmcHistoAxes &  axes ) :
                type( type ), impl( impl ), isARHisto( isARHisto ),
                isARRec( isARRec ), triggerType( triggerType ), name( name ),
                title( title ), axes( axes )
            {}

            CexmcHistoType    type;

            CexmcHistoImpl    impl;

            bool              isARHisto;

            bool              isARRec;

            CexmcTriggerType  triggerType;

            G4String          name;

            G4String          title;

            CexmcHistoAxes    axes;
        };

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

        void  Add( CexmcHistoType  histoType, unsigned int  index,
                   G4double  x );

        void  Add( CexmcHistoType  histoType, unsigned int  index, G4double  x,
                   G4double  y );

        void  Add( CexmcHistoType  histoType, unsigned int  index, G4double  x,
                   G4double  y, G4double  z );

        void  Add( CexmcHistoType  histoType, unsigned int  index, G4int  binX,
                   G4int  binY, G4double  value );

        void  List( void ) const;

        void  Print( const G4String &  value );

#ifdef CEXMC_USE_ROOTQT
        void  Draw( const G4String &  histoName,
                    const G4String &  histoDrawOptions = "" );
#endif

    private:
        void  AddHisto( const CexmcHistoData &  data,
                    const CexmcAngularRange &  aRange = CexmcAngularRange() );

        void  CreateHisto( CexmcHistoVector &  histoVector,
                           CexmcHistoImpl  histoImpl, const G4String &  name,
                           const G4String &  title,
                           const CexmcHistoAxes &  axes );

    private:
        TFile *                       outFile;

    private:
        CexmcHistosMap                histos;

        bool                          isInitialized;

        G4String                      opName;

        G4String                      nopName;

        G4double                      opMass;

        G4double                      nopMass;

#ifdef CEXMC_USE_ROOTQT
    private:

        TQtWidget *                   rootCanvas;
#endif

    private:
        CexmcHistoManagerMessenger *  messenger;

    private:
        static CexmcHistoManager *    instance;
};

#endif


#endif

