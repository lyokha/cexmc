/*
 * =============================================================================
 *
 *       Filename:  CexmcASTEval.cc
 *
 *    Description:  abstract syntax tree for custom filter eval
 *
 *        Version:  1.0
 *        Created:  17.07.2010 15:46:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifdef CEXMC_USE_CUSTOM_FILTER

#include <numeric>
#include <boost/variant/get.hpp>
#include "CexmcASTEval.hh"


namespace
{
    const std::string  CexmcCFVarEvent( "event" );
    const std::string  CexmcCFVarOpCosThetaSCM( "op_cosTh_SCM" );
    const std::string  CexmcCFVarEDT( "edt" );
    const std::string  CexmcCFVarMonT( "mont" );
    const std::string  CexmcCFVarMonED( "monED" );
    const std::string  CexmcCFVarVclED( "vclED" );
    const std::string  CexmcCFVarVcrED( "vcrED" );
    const std::string  CexmcCFVarClED( "clED" );
    const std::string  CexmcCFVarCrED( "crED" );
    const std::string  CexmcCFVarClEDCol( "clEDcol" );
    const std::string  CexmcCFVarCrEDCol( "crEDcol" );
    const std::string  CexmcCFVarIpMonPosL( "ip_mon_posl" );
    const std::string  CexmcCFVarIpMonPosW( "ip_mon_posw" );
    const std::string  CexmcCFVarIpMonDirL( "ip_mon_dirl" );
    const std::string  CexmcCFVarIpMonDirW( "ip_mon_dirw" );
    const std::string  CexmcCFVarIpMonMom( "ip_mon_mom" );
    const std::string  CexmcCFVarIpMonTid( "ip_mon_tid" );
    const std::string  CexmcCFVarIpTgtPosL( "ip_tgt_posl" );
    const std::string  CexmcCFVarIpTgtPosW( "ip_tgt_posw" );
    const std::string  CexmcCFVarIpTgtDirL( "ip_tgt_dirl" );
    const std::string  CexmcCFVarIpTgtDirW( "ip_tgt_dirw" );
    const std::string  CexmcCFVarIpTgtMom( "ip_tgt_mom" );
    const std::string  CexmcCFVarIpTgtTid( "ip_tgt_tid" );
    const std::string  CexmcCFVarOpTgtPosL( "op_tgt_posl" );
    const std::string  CexmcCFVarOpTgtPosW( "op_tgt_posw" );
    const std::string  CexmcCFVarOpTgtDirL( "op_tgt_dirl" );
    const std::string  CexmcCFVarOpTgtDirW( "op_tgt_dirw" );
    const std::string  CexmcCFVarOpTgtMom( "op_tgt_mom" );
    const std::string  CexmcCFVarOpTgtTid( "op_tgt_tid" );
    const std::string  CexmcCFVarNpTgtPosL( "np_tgt_posl" );
    const std::string  CexmcCFVarNpTgtPosW( "np_tgt_posw" );
    const std::string  CexmcCFVarNpTgtDirL( "np_tgt_dirl" );
    const std::string  CexmcCFVarNpTgtDirW( "np_tgt_dirw" );
    const std::string  CexmcCFVarNpTgtMom( "np_tgt_mom" );
    const std::string  CexmcCFVarNpTgtTid( "np_tgt_tid" );
    const std::string  CexmcCFVarOpdp1TgtPosL( "opdp1_tgt_posl" );
    const std::string  CexmcCFVarOpdp1TgtPosW( "opdp1_tgt_posw" );
    const std::string  CexmcCFVarOpdp1TgtDirL( "opdp1_tgt_dirl" );
    const std::string  CexmcCFVarOpdp1TgtDirW( "opdp1_tgt_dirw" );
    const std::string  CexmcCFVarOpdp1TgtMom( "opdp1_tgt_mom" );
    const std::string  CexmcCFVarOpdp1TgtTid( "opdp1_tgt_tid" );
    const std::string  CexmcCFVarOpdp2TgtPosL( "opdp2_tgt_posl" );
    const std::string  CexmcCFVarOpdp2TgtPosW( "opdp2_tgt_posw" );
    const std::string  CexmcCFVarOpdp2TgtDirL( "opdp2_tgt_dirl" );
    const std::string  CexmcCFVarOpdp2TgtDirW( "opdp2_tgt_dirw" );
    const std::string  CexmcCFVarOpdp2TgtMom( "opdp2_tgt_mom" );
    const std::string  CexmcCFVarOpdp2TgtTid( "opdp2_tgt_tid" );
    const std::string  CexmcCFVarOpdpVclPosL( "opdp_vcl_posl" );
    const std::string  CexmcCFVarOpdpVclPosW( "opdp_vcl_posw" );
    const std::string  CexmcCFVarOpdpVclDirL( "opdp_vcl_dirl" );
    const std::string  CexmcCFVarOpdpVclDirW( "opdp_vcl_dirw" );
    const std::string  CexmcCFVarOpdpVclMom( "opdp_vcl_mom" );
    const std::string  CexmcCFVarOpdpVclTid( "opdp_vcl_tid" );
    const std::string  CexmcCFVarOpdpVcrPosL( "opdp_vcr_posl" );
    const std::string  CexmcCFVarOpdpVcrPosW( "opdp_vcr_posw" );
    const std::string  CexmcCFVarOpdpVcrDirL( "opdp_vcr_dirl" );
    const std::string  CexmcCFVarOpdpVcrDirW( "opdp_vcr_dirw" );
    const std::string  CexmcCFVarOpdpVcrMom( "opdp_vcr_mom" );
    const std::string  CexmcCFVarOpdpVcrTid( "opdp_vcr_tid" );
    const std::string  CexmcCFVarOpdpClPosL( "opdp_cl_posl" );
    const std::string  CexmcCFVarOpdpClPosW( "opdp_cl_posw" );
    const std::string  CexmcCFVarOpdpClDirL( "opdp_cl_dirl" );
    const std::string  CexmcCFVarOpdpClDirW( "opdp_cl_dirw" );
    const std::string  CexmcCFVarOpdpClMom( "opdp_cl_mom" );
    const std::string  CexmcCFVarOpdpClTid( "opdp_cl_tid" );
    const std::string  CexmcCFVarOpdpCrPosL( "opdp_cr_posl" );
    const std::string  CexmcCFVarOpdpCrPosW( "opdp_cr_posw" );
    const std::string  CexmcCFVarOpdpCrDirL( "opdp_cr_dirl" );
    const std::string  CexmcCFVarOpdpCrDirW( "opdp_cr_dirw" );
    const std::string  CexmcCFVarOpdpCrMom( "opdp_cr_mom" );
    const std::string  CexmcCFVarOpdpCrTid( "opdp_cr_tid" );
    const std::string  CexmcCFVarIpSCM( "ipSCM" );
    const std::string  CexmcCFVarIpLAB( "ipLAB" );
    const std::string  CexmcCFVarNpSCM( "npSCM" );
    const std::string  CexmcCFVarNpLAB( "npLAB" );
    const std::string  CexmcCFVarOpSCM( "opSCM" );
    const std::string  CexmcCFVarOpLAB( "opLAB" );
    const std::string  CexmcCFVarNopSCM( "nopSCM" );
    const std::string  CexmcCFVarNopLAB( "nopLAB" );
    const std::string  CexmcCFVarIpId( "ipId" );
    const std::string  CexmcCFVarNpId( "npId" );
    const std::string  CexmcCFVarOpId( "opId" );
    const std::string  CexmcCFVarNopId( "nopId" );
    const std::string  CexmcCFVarConst_eV( "eV" );
    const std::string  CexmcCFVarConst_keV( "keV" );
    const std::string  CexmcCFVarConst_MeV( "MeV" );
    const std::string  CexmcCFVarConst_GeV( "GeV" );
    const std::string  CexmcCFVarConst_mm( "mm" );
    const std::string  CexmcCFVarConst_cm( "cm" );
    const std::string  CexmcCFVarConst_m( "m" );
}


const G4double CexmcASTEval::constants[] = { eV, keV, MeV, GeV, mm, cm, m };


CexmcAST::BasicEval::ScalarValueType  CexmcASTEval::GetFunScalarValue(
                                        const CexmcAST::Subtree &  ast ) const
{
    bool             evalResult( false );
    ScalarValueType  result( GetBasicFunScalarValue( ast, evalResult ) );

    if ( evalResult )
        return result;

    const CexmcAST::Function &  fun( boost::get< CexmcAST::Function >(
                                                                ast.type ) );

    if ( fun == "Sum" )
    {
        CexmcEnergyDepositCalorimeterCollection  edCol;
        GetEDCollectionValue( ast.children[ 0 ], edCol );

        G4double  result( 0. );

        for ( CexmcEnergyDepositCalorimeterCollection::iterator
                                    k( edCol.begin() ); k != edCol.end(); ++k )
        {
            result += std::accumulate( k->begin(), k->end(), 0 );
        }

        return result;
    }

    throw CexmcException( CexmcCFUnexpectedFunction );

    return 0;
}


CexmcAST::BasicEval::ScalarValueType  CexmcASTEval::GetVarScalarValue(
                                        const CexmcAST::Variable &  var ) const
{
    /* Variables with initialized address */

    const int * const *  addr( boost::get< const int * >( &var.addr ) );

    if ( addr )
    {
        if ( *addr )
            return **addr;
    }
    else
    {
        const double * const *  addr( boost::get< const double * >(
                                                                &var.addr ) );
        if ( *addr )
            return **addr;
    }

    VarAddrMap::const_iterator  found( varAddrMap.find( var.name ) );

    if ( found != varAddrMap.end() )
    {
        const CexmcEnergyDepositCalorimeterCollection * const *  addr(
                boost::get< const CexmcEnergyDepositCalorimeterCollection * >(
                                                            &found->second ) );
        if ( addr )
        {
            if ( *addr )
            {
                if ( var.index1 == 0 || var.index2 == 0 )
                    throw CexmcException( CexmcCFUnexpectedVectorIndex );
                return ( *addr )->at( var.index1 ).at( var.index2 );
            }
        }
        else
        {
            const bool * const &  addr( boost::get< const bool * >(
                                                            found->second ) );
            if ( addr )
                return *addr;
        }
    }

    /* Variables with uninitialized address */

    CexmcAST::Variable &  theVar( const_cast< CexmcAST::Variable & >( var ) );
    VarAddrMap &  theVarAddrMap( const_cast< VarAddrMap & >( varAddrMap ) );

    if ( var.name == CexmcCFVarEvent )
    {
        theVar.addr = &eventFastSObject.eventId;
        return eventFastSObject.eventId;
    }
    if ( var.name == CexmcCFVarOpCosThetaSCM )
    {
        theVar.addr = &eventFastSObject.opCosThetaSCM;
        return eventFastSObject.opCosThetaSCM;
    }
    if ( var.name == CexmcCFVarEDT )
    {
        theVarAddrMap.insert( std::pair< std::string, VarAddr >( var.name,
                        &eventFastSObject.edDigitizerHasTriggered ) );
        return eventFastSObject.edDigitizerHasTriggered;
    } 
    if ( var.name == CexmcCFVarMonT )
    {
        theVarAddrMap.insert( std::pair< std::string, VarAddr >( var.name,
                        &eventFastSObject.edDigitizerMonitorHasTriggered ) );
        return eventFastSObject.edDigitizerMonitorHasTriggered;
    }
    if ( var.name == CexmcCFVarMonED )
    {
        theVar.addr = &eventSObject.monitorED;
        return eventSObject.monitorED;
    }
    if ( var.name == CexmcCFVarVclED )
    {
        theVar.addr = &eventSObject.vetoCounterEDLeft;
        return eventSObject.vetoCounterEDLeft;
    }
    if ( var.name == CexmcCFVarVcrED )
    {
        theVar.addr = &eventSObject.vetoCounterEDRight;
        return eventSObject.vetoCounterEDRight;
    }
    if ( var.name == CexmcCFVarClED )
    {
        theVar.addr = &eventSObject.calorimeterEDLeft;
        return eventSObject.calorimeterEDLeft;
    }
    if ( var.name == CexmcCFVarCrED )
    {
        theVar.addr = &eventSObject.calorimeterEDRight;
        return eventSObject.calorimeterEDRight;
    }
    if ( var.name == CexmcCFVarClEDCol )
    {
        if ( var.index1 == 0 || var.index2 == 0 )
            throw CexmcException( CexmcCFUnexpectedVectorIndex );
        theVarAddrMap.insert( std::pair< std::string, VarAddr >( var.name,
                                &eventSObject.calorimeterEDLeftCollection ) );
        return eventSObject.calorimeterEDLeftCollection.at( var.index1 - 1 ).
                                                        at( var.index2 - 1 );
    }
    if ( var.name == CexmcCFVarCrEDCol )
    {
        if ( var.index1 == 0 || var.index2 == 0 )
            throw CexmcException( CexmcCFUnexpectedVectorIndex );
        theVarAddrMap.insert( std::pair< std::string, VarAddr >( var.name,
                                &eventSObject.calorimeterEDRightCollection ) );
        return eventSObject.calorimeterEDRightCollection.at( var.index1 - 1 ).
                                                         at( var.index2 - 1 );
    }
    if ( var.name == CexmcCFVarIpMonPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.monitorTP.positionLocal, var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.monitorTP.positionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.monitorTP.positionWorld, var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.monitorTP.positionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.monitorTP.directionLocal, var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.monitorTP.directionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.monitorTP.directionWorld, var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.monitorTP.directionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonMom )
    {
        theVar.addr = &eventSObject.monitorTP.momentumAmp;
        return eventSObject.monitorTP.momentumAmp;
    }
    if ( var.name == CexmcCFVarIpMonTid )
    {
        theVar.addr = &eventSObject.monitorTP.trackId;
        return eventSObject.monitorTP.trackId;
    }
    if ( var.name == CexmcCFVarIpTgtPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPIncidentParticle.positionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPIncidentParticle.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPIncidentParticle.positionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPIncidentParticle.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPIncidentParticle.directionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPIncidentParticle.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPIncidentParticle.directionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPIncidentParticle.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtMom )
    {
        theVar.addr = &eventSObject.targetTPIncidentParticle.momentumAmp;
        return eventSObject.targetTPIncidentParticle.momentumAmp;
    }
    if ( var.name == CexmcCFVarIpTgtTid )
    {
        theVar.addr = &eventSObject.targetTPIncidentParticle.trackId;
        return eventSObject.targetTPIncidentParticle.trackId;
    }
    if ( var.name == CexmcCFVarOpTgtPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPOutputParticle.positionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPOutputParticle.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPOutputParticle.positionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPOutputParticle.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPOutputParticle.directionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPOutputParticle.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPOutputParticle.directionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPOutputParticle.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtMom )
    {
        theVar.addr = &eventSObject.targetTPOutputParticle.momentumAmp;
        return eventSObject.targetTPOutputParticle.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpTgtTid )
    {
        theVar.addr = &eventSObject.targetTPOutputParticle.trackId;
        return eventSObject.targetTPOutputParticle.trackId;
    }
    if ( var.name == CexmcCFVarNpTgtPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPNucleusParticle.positionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPNucleusParticle.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPNucleusParticle.positionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPNucleusParticle.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPNucleusParticle.directionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPNucleusParticle.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.targetTPNucleusParticle.directionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.targetTPNucleusParticle.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtMom )
    {
        theVar.addr = &eventSObject.targetTPNucleusParticle.momentumAmp;
        return eventSObject.targetTPNucleusParticle.momentumAmp;
    }
    if ( var.name == CexmcCFVarNpTgtTid )
    {
        theVar.addr = &eventSObject.targetTPNucleusParticle.trackId;
        return eventSObject.targetTPNucleusParticle.trackId;
    }
    if ( var.name == CexmcCFVarOpdp1TgtPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    positionLocal, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    positionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    positionWorld, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    positionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    directionLocal, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    directionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    directionWorld, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle1.
                    directionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtMom )
    {
        theVar.addr = &eventSObject.targetTPOutputParticleDecayProductParticle1.
                    momentumAmp;
        return eventSObject.targetTPOutputParticleDecayProductParticle1.
                    momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdp1TgtTid )
    {
        theVar.addr = &eventSObject.targetTPOutputParticleDecayProductParticle1.
                    trackId;
        return eventSObject.targetTPOutputParticleDecayProductParticle1.trackId;
    }
    if ( var.name == CexmcCFVarOpdp2TgtPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    positionLocal, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    positionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    positionWorld, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    positionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    directionLocal, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    directionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    directionWorld, var.index1 );
        return GetThreeVectorElementByIndex(
                    eventSObject.targetTPOutputParticleDecayProductParticle2.
                    directionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtMom )
    {
        theVar.addr = &eventSObject.targetTPOutputParticleDecayProductParticle2.
                    momentumAmp;
        return eventSObject.targetTPOutputParticleDecayProductParticle2.
                    momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdp2TgtTid )
    {
        theVar.addr = &eventSObject.targetTPOutputParticleDecayProductParticle2.
                    trackId;
        return eventSObject.targetTPOutputParticleDecayProductParticle2.trackId;
    }
    if ( var.name == CexmcCFVarOpdpVclPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPLeft.positionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPLeft.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPLeft.positionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPLeft.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPLeft.directionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPLeft.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPLeft.directionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPLeft.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclMom )
    {
        theVar.addr = &eventSObject.vetoCounterTPLeft.momentumAmp;
        return eventSObject.vetoCounterTPLeft.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdpVclTid )
    {
        theVar.addr = &eventSObject.vetoCounterTPLeft.trackId;
        return eventSObject.vetoCounterTPLeft.trackId;
    }
    if ( var.name == CexmcCFVarOpdpVcrPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPRight.positionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPRight.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPRight.positionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPRight.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPRight.directionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPRight.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.vetoCounterTPRight.directionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.vetoCounterTPRight.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrMom )
    {
        theVar.addr = &eventSObject.vetoCounterTPRight.momentumAmp;
        return eventSObject.vetoCounterTPRight.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdpVcrTid )
    {
        theVar.addr = &eventSObject.vetoCounterTPRight.trackId;
        return eventSObject.vetoCounterTPRight.trackId;
    }
    if ( var.name == CexmcCFVarOpdpClPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPLeft.positionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPLeft.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPLeft.positionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPLeft.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPLeft.directionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPLeft.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPLeft.directionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPLeft.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClMom )
    {
        theVar.addr = &eventSObject.calorimeterTPLeft.momentumAmp;
        return eventSObject.calorimeterTPLeft.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdpClTid )
    {
        theVar.addr = &eventSObject.calorimeterTPLeft.trackId;
        return eventSObject.calorimeterTPLeft.trackId;
    }
    if ( var.name == CexmcCFVarOpdpCrPosL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPRight.positionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPRight.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrPosW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPRight.positionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPRight.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrDirL )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPRight.directionLocal,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPRight.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrDirW )
    {
        theVar.addr = GetThreeVectorElementAddrByIndex(
                        eventSObject.calorimeterTPRight.directionWorld,
                        var.index1 );
        return GetThreeVectorElementByIndex(
                        eventSObject.calorimeterTPRight.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrMom )
    {
        theVar.addr = &eventSObject.calorimeterTPRight.momentumAmp;
        return eventSObject.calorimeterTPRight.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdpCrTid )
    {
        theVar.addr = &eventSObject.calorimeterTPRight.trackId;
        return eventSObject.calorimeterTPRight.trackId;
    }
    if ( var.name == CexmcCFVarIpSCM )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                        eventSObject.productionModelData.incidentParticleSCM,
                        var.index1 );
        return GetLorentzVectorElementByIndex(
                        eventSObject.productionModelData.incidentParticleSCM,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpLAB )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                        eventSObject.productionModelData.incidentParticleLAB,
                        var.index1 );
        return GetLorentzVectorElementByIndex(
                        eventSObject.productionModelData.incidentParticleLAB,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpSCM )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                        eventSObject.productionModelData.nucleusParticleSCM,
                        var.index1 );
        return GetLorentzVectorElementByIndex(
                        eventSObject.productionModelData.nucleusParticleSCM,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpLAB )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                        eventSObject.productionModelData.nucleusParticleLAB,
                        var.index1 );
        return GetLorentzVectorElementByIndex(
                        eventSObject.productionModelData.nucleusParticleLAB,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpSCM )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                        eventSObject.productionModelData.outputParticleSCM,
                        var.index1 );
        return GetLorentzVectorElementByIndex(
                        eventSObject.productionModelData.outputParticleSCM,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpLAB )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                        eventSObject.productionModelData.outputParticleLAB,
                        var.index1 );
        return GetLorentzVectorElementByIndex(
                        eventSObject.productionModelData.outputParticleLAB,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNopSCM )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                    eventSObject.productionModelData.nucleusOutputParticleSCM,
                    var.index1 );
        return GetLorentzVectorElementByIndex(
                    eventSObject.productionModelData.nucleusOutputParticleSCM,
                    var.index1 );
    }
    if ( var.name == CexmcCFVarNopLAB )
    {
        theVar.addr = GetLorentzVectorElementAddrByIndex(
                    eventSObject.productionModelData.nucleusOutputParticleLAB,
                    var.index1 );
        return GetLorentzVectorElementByIndex(
                    eventSObject.productionModelData.nucleusOutputParticleLAB,
                    var.index1 );
    }
    if ( var.name == CexmcCFVarIpId )
    {
        theVar.addr = &eventSObject.productionModelData.incidentParticle;
        return eventSObject.productionModelData.incidentParticle;
    }
    if ( var.name == CexmcCFVarNpId )
    {
        theVar.addr = &eventSObject.productionModelData.nucleusParticle;
        return eventSObject.productionModelData.nucleusParticle;
    }
    if ( var.name == CexmcCFVarOpId )
    {
        theVar.addr = &eventSObject.productionModelData.outputParticle;
        return eventSObject.productionModelData.outputParticle;
    }
    if ( var.name == CexmcCFVarNopId )
    {
        theVar.addr = &eventSObject.productionModelData.nucleusOutputParticle;
        return eventSObject.productionModelData.nucleusOutputParticle;
    }
    if ( var.name == CexmcCFVarConst_eV )
    {
        theVar.addr = &constants[ 0 ];
        return constants[ 0 ];
    }
    if ( var.name == CexmcCFVarConst_keV )
    {
        theVar.addr = &constants[ 1 ];
        return constants[ 1 ];
    }
    if ( var.name == CexmcCFVarConst_MeV )
    {
        theVar.addr = &constants[ 2 ];
        return constants[ 2 ];
    }
    if ( var.name == CexmcCFVarConst_GeV )
    {
        theVar.addr = &constants[ 3 ];
        return constants[ 3 ];
    }
    if ( var.name == CexmcCFVarConst_mm )
    {
        theVar.addr = &constants[ 4 ];
        return constants[ 4 ];
    }
    if ( var.name == CexmcCFVarConst_cm )
    {
        theVar.addr = &constants[ 5 ];
        return constants[ 5 ];
    }
    if ( var.name == CexmcCFVarConst_m )
    {
        theVar.addr = &constants[ 6 ];
        return constants[ 6 ];
    }

    throw CexmcException( CexmcCFUnexpectedVariable );

    return 0;
}


void  CexmcASTEval::GetEDCollectionValue( const CexmcAST::Node &  node,
                        CexmcEnergyDepositCalorimeterCollection &  edCol ) const
{
    const CexmcAST::Subtree *  ast( boost::get< CexmcAST::Subtree >( &node ) );

    if ( ast )
    {
        const CexmcAST::Function &  fun( boost::get< CexmcAST::Function >(
                                                                ast->type ) );

        if ( fun == "Inner" )
        {
            GetEDCollectionValue( ast->children[ 0 ], edCol );
            edCol.pop_back();
            edCol.erase( edCol.begin() );
            for ( CexmcEnergyDepositCalorimeterCollection::iterator
                            k( edCol.begin() ); k != edCol.end(); ++k )
            {
                k->pop_back();
                k->erase( k->begin() );
            }
            return;
        }
        if ( fun == "Outer" )
        {
            GetEDCollectionValue( ast->children[ 0 ], edCol );
            if ( edCol.size() < 3 )
                return;
            for ( CexmcEnergyDepositCalorimeterCollection::iterator
                            k( edCol.begin() + 1 ); k != edCol.end() - 1; ++k )
            {
                if ( k->size() < 3 )
                    continue;
                k->erase( k->begin() + 1, k->end() - 1 );
            }
            return;
        }
    }
    else
    {
        const CexmcAST::Leaf &      leaf( boost::get< CexmcAST::Leaf >(
                                                                    node ) );
        const CexmcAST::Variable &  var( boost::get< CexmcAST::Variable >(
                                                                    leaf ) );

        VarAddrMap::const_iterator  found( varAddrMap.find( var.name ) );

        if ( found != varAddrMap.end() )
        {
            const CexmcEnergyDepositCalorimeterCollection * const *  addr(
                boost::get< const CexmcEnergyDepositCalorimeterCollection * >(
                                                            &found->second ) );
            if ( ! addr )
            {
                throw CexmcException( CexmcCFUnexpectedVariableUsage );
            }
            else
            {
                if ( *addr )
                    edCol = **addr;
                return;
            }
        }

        VarAddrMap &  theVarAddrMap( const_cast< VarAddrMap & >( varAddrMap ) );

        if ( var.name == CexmcCFVarClEDCol )
        {
            if ( var.index1 != 0 || var.index2 != 0 )
                throw CexmcException( CexmcCFUnexpectedVariableUsage );
            theVarAddrMap.insert( std::pair< std::string, VarAddr >( var.name,
                                &eventSObject.calorimeterEDLeftCollection ) );
            edCol = eventSObject.calorimeterEDLeftCollection;
            return;
        }
        if ( var.name == CexmcCFVarCrEDCol )
        {
            if ( var.index1 != 0 || var.index2 != 0 )
                throw CexmcException( CexmcCFUnexpectedVariableUsage );
            theVarAddrMap.insert( std::pair< std::string, VarAddr >( var.name,
                                &eventSObject.calorimeterEDRightCollection ) );
            edCol = eventSObject.calorimeterEDRightCollection;
            return;
        }
    }
}

#endif

