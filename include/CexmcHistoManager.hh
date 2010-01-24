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
    CexmcMomentumIP_TPT_Histo,
    CexmcMomentumIP_RT_Histo,
    CexmcTPInMonitor_TPT_Histo,
    CexmcTPInTarget_TPT_Histo,
    CexmcTPInTarget_RT_Histo,
    CexmcRecMasses_EDT_Histo,
    CexmcRecMasses_RT_Histo,
    CexmcRecMassOP_ARReal_RT_Histo,
    CexmcRecMassNOP_ARReal_RT_Histo,
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
    CexmcDiffOpenAngle_ARReal_RT_Histo
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
        TFile *                       outFile;

        CexmcHistoVector              momip_tpt;

        CexmcHistoVector              momip_rt;

        CexmcHistoVector              tpmon_tpt;

        CexmcHistoVector              tptar_tpt;

        CexmcHistoVector              tptar_rt;

        CexmcHistoVector              recmasses_edt;

        CexmcHistoVector              recmasses_rt;

        CexmcHistoVector              recmassop_arreal_rt;

        CexmcHistoVector              recmassnop_arreal_rt;

        CexmcHistoVector              kecl_arreal_tpt;

        CexmcHistoVector              kecr_arreal_tpt;

        CexmcHistoVector              kecl_arreal_rt;

        CexmcHistoVector              kecr_arreal_rt;

        CexmcHistoVector              aecl_arreal_edt;

        CexmcHistoVector              aecr_arreal_edt;

        CexmcHistoVector              aecl_arreal_rt;

        CexmcHistoVector              aecr_arreal_rt;

        CexmcHistoVector              mecl_arreal_rt;

        CexmcHistoVector              mecr_arreal_rt;

        CexmcHistoVector              keop_lab_arreal_tpt;

        CexmcHistoVector              keop_lab_arreal_rt;

        CexmcHistoVector              aop_scm_arreal_tpt;

        CexmcHistoVector              aop_scm_arreal_rt;

        CexmcHistoVector              recaop_scm_arreal_rt;

        CexmcHistoVector              diffaop_scm_arreal_rt;

        CexmcHistoVector              oa_scm_arreal_tpt;

        CexmcHistoVector              oa_scm_arreal_rt;

        CexmcHistoVector              recoa_scm_arreal_rt;

        CexmcHistoVector              diffoa_scm_arreal_rt;

    private:
        CexmcHistosMap                histos;

        bool                          isInitialized;

    private:
        CexmcHistoManagerMessenger *  messenger;

    private:
        static CexmcHistoManager *    instance;
};


#endif

