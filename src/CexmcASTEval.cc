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
    const std::string  CexmcCFVarTPT( "tpt" );
    const std::string  CexmcCFVarMon( "mon" );
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


CexmcASTEval::CexmcASTEval( const CexmcEventFastSObject *  evFastSObject,
                            const CexmcEventSObject *  evSObject ) :
    evFastSObject( evFastSObject ), evSObject( evSObject )
{
}


CexmcAST::BasicEval::ScalarValueType  CexmcASTEval::GetFunScalarValue(
                                        const CexmcAST::Subtree &  ast ) const
{
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
            result += std::accumulate( k->begin(), k->end(), G4double( 0. ) );
        }

        return result;
    }

    bool             evalResult( false );
    ScalarValueType  result( GetBasicFunScalarValue( ast, evalResult ) );

    if ( evalResult )
        return result;

    throw CexmcException( CexmcCFUnexpectedFunction );

    return 0;
}


CexmcAST::BasicEval::ScalarValueType  CexmcASTEval::GetVarScalarValue(
                                        const CexmcAST::Variable &  var ) const
{
    if ( evFastSObject == NULL || evSObject == NULL )
        throw CexmcException( CexmcCFUninitialized );

    /* Variables with initialized address */

    VarAddrMap::const_iterator  found( varAddrMap.find( var.name ) );

    if ( found != varAddrMap.end() )
    {
        const double * const *  addr( boost::get< const double * >(
                                                        &found->second ) );
        if ( addr )
        {
            if ( *addr )
                return **addr;
        }
        else
        {
            const int * const *  addr( boost::get< const int * >(
                                                        &found->second ) );
            if ( addr )
            {
                if ( *addr )
                    return **addr;
            }
            else
            {
                const CexmcEnergyDepositCalorimeterCollection * const *  addr(
                        boost::get<
                            const CexmcEnergyDepositCalorimeterCollection * >(
                                                            &found->second ) );
                if ( addr )
                {
                    if ( *addr )
                    {
                        if ( ( *addr )->size() == 0 )
                            throw CexmcException( CexmcCFUninitializedVector );
                        if ( var.index1 == 0 || var.index2 == 0 )
                            throw CexmcException(
                                            CexmcCFUnexpectedVectorIndex );
                        return ( *addr )->at( var.index1 - 1 ).
                                                        at( var.index2 - 1 );
                    }
                }
                else
                {
                    const bool * const &  addr( boost::get< const bool * >(
                                                            found->second ) );
                    if ( addr )
                        return int( *addr );
                }
            }
        }
    }

    /* Variables without address */

    if ( var.name == CexmcCFVarTPT )
    {
        return int( evSObject->targetTPOutputParticle.trackId != -1 );
    } 

    /* Variables with uninitialized address */

    if ( var.name == CexmcCFVarEvent )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evFastSObject->eventId ) );
        return evFastSObject->eventId;
    }
    if ( var.name == CexmcCFVarOpCosThetaSCM )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evFastSObject->opCosThetaSCM ) );
        return evFastSObject->opCosThetaSCM;
    }
    if ( var.name == CexmcCFVarEDT )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evFastSObject->edDigitizerHasTriggered ) );
        return evFastSObject->edDigitizerHasTriggered;
    } 
    if ( var.name == CexmcCFVarMon )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evFastSObject->edDigitizerMonitorHasTriggered ) );
        return evFastSObject->edDigitizerMonitorHasTriggered;
    }
    if ( var.name == CexmcCFVarMonED )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->monitorED ) );
        return evSObject->monitorED;
    }
    if ( var.name == CexmcCFVarVclED )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->vetoCounterEDLeft ) );
        return evSObject->vetoCounterEDLeft;
    }
    if ( var.name == CexmcCFVarVcrED )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->vetoCounterEDRight ) );
        return evSObject->vetoCounterEDRight;
    }
    if ( var.name == CexmcCFVarClED )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->calorimeterEDLeft ) );
        return evSObject->calorimeterEDLeft;
    }
    if ( var.name == CexmcCFVarCrED )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->calorimeterEDRight ) );
        return evSObject->calorimeterEDRight;
    }
    if ( var.name == CexmcCFVarClEDCol )
    {
        if ( evSObject->calorimeterEDLeftCollection.size() == 0 )
            throw CexmcException( CexmcCFUninitializedVector );
        if ( var.index1 == 0 || var.index2 == 0 )
            throw CexmcException( CexmcCFUnexpectedVectorIndex );
        varAddrMap.insert( VarAddrMapData( var.name,
                                &evSObject->calorimeterEDLeftCollection ) );
        return evSObject->calorimeterEDLeftCollection.at( var.index1 - 1 ).
                                                      at( var.index2 - 1 );
    }
    if ( var.name == CexmcCFVarCrEDCol )
    {
        if ( evSObject->calorimeterEDRightCollection.size() == 0 )
            throw CexmcException( CexmcCFUninitializedVector );
        if ( var.index1 == 0 || var.index2 == 0 )
            throw CexmcException( CexmcCFUnexpectedVectorIndex );
        varAddrMap.insert( VarAddrMapData( var.name,
                                &evSObject->calorimeterEDRightCollection ) );
        return evSObject->calorimeterEDRightCollection.at( var.index1 - 1 ).
                                                       at( var.index2 - 1 );
    }
    if ( var.name == CexmcCFVarIpMonPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->monitorTP.positionLocal, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->monitorTP.positionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->monitorTP.positionWorld, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->monitorTP.positionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->monitorTP.directionLocal, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->monitorTP.directionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->monitorTP.directionWorld, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->monitorTP.directionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarIpMonMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->monitorTP.momentumAmp ) );
        return evSObject->monitorTP.momentumAmp;
    }
    if ( var.name == CexmcCFVarIpMonTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->monitorTP.trackId ) );
        return evSObject->monitorTP.trackId;
    }
    if ( var.name == CexmcCFVarIpTgtPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPIncidentParticle.positionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPIncidentParticle.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPIncidentParticle.positionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPIncidentParticle.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPIncidentParticle.directionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPIncidentParticle.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPIncidentParticle.directionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPIncidentParticle.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpTgtMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->targetTPIncidentParticle.momentumAmp ) );
        return evSObject->targetTPIncidentParticle.momentumAmp;
    }
    if ( var.name == CexmcCFVarIpTgtTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->targetTPIncidentParticle.trackId ) );
        return evSObject->targetTPIncidentParticle.trackId;
    }
    if ( var.name == CexmcCFVarOpTgtPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticle.positionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPOutputParticle.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticle.positionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPOutputParticle.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticle.directionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPOutputParticle.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticle.directionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPOutputParticle.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpTgtMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->targetTPOutputParticle.momentumAmp ) );
        return evSObject->targetTPOutputParticle.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpTgtTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->targetTPOutputParticle.trackId ) );
        return evSObject->targetTPOutputParticle.trackId;
    }
    if ( var.name == CexmcCFVarNpTgtPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPNucleusParticle.positionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPNucleusParticle.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPNucleusParticle.positionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPNucleusParticle.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPNucleusParticle.directionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPNucleusParticle.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPNucleusParticle.directionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->targetTPNucleusParticle.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpTgtMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->targetTPNucleusParticle.momentumAmp ) );
        return evSObject->targetTPNucleusParticle.momentumAmp;
    }
    if ( var.name == CexmcCFVarNpTgtTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->targetTPNucleusParticle.trackId ) );
        return evSObject->targetTPNucleusParticle.trackId;
    }
    if ( var.name == CexmcCFVarOpdp1TgtPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle1.
                                    positionLocal, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle1.
                    positionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle1.
                                    positionWorld, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle1.
                    positionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle1.
                                    directionLocal, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle1.
                    directionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle1.
                                    directionWorld, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle1.
                    directionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp1TgtMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    &evSObject->targetTPOutputParticleDecayProductParticle1.
                    momentumAmp ) );
        return evSObject->targetTPOutputParticleDecayProductParticle1.
                    momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdp1TgtTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    &evSObject->targetTPOutputParticleDecayProductParticle1.
                    trackId ) );
        return evSObject->targetTPOutputParticleDecayProductParticle1.trackId;
    }
    if ( var.name == CexmcCFVarOpdp2TgtPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle2.
                                    positionLocal, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle2.
                    positionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle2.
                                    positionWorld, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle2.
                    positionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle2.
                                    directionLocal, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle2.
                    directionLocal, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->targetTPOutputParticleDecayProductParticle2.
                                    directionWorld, var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                    evSObject->targetTPOutputParticleDecayProductParticle2.
                    directionWorld, var.index1 );
    }
    if ( var.name == CexmcCFVarOpdp2TgtMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    &evSObject->targetTPOutputParticleDecayProductParticle2.
                    momentumAmp ) );
        return evSObject->targetTPOutputParticleDecayProductParticle2.
                    momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdp2TgtTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    &evSObject->targetTPOutputParticleDecayProductParticle2.
                    trackId ) );
        return evSObject->targetTPOutputParticleDecayProductParticle2.trackId;
    }
    if ( var.name == CexmcCFVarOpdpVclPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPLeft.positionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPLeft.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPLeft.positionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPLeft.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPLeft.directionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPLeft.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPLeft.directionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPLeft.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVclMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->vetoCounterTPLeft.momentumAmp ) );
        return evSObject->vetoCounterTPLeft.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdpVclTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->vetoCounterTPLeft.trackId ) );
        return evSObject->vetoCounterTPLeft.trackId;
    }
    if ( var.name == CexmcCFVarOpdpVcrPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPRight.positionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPRight.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPRight.positionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPRight.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPRight.directionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPRight.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->vetoCounterTPRight.directionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->vetoCounterTPRight.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpVcrMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->vetoCounterTPRight.trackId ) );
        return evSObject->vetoCounterTPRight.trackId;
    }
    if ( var.name == CexmcCFVarOpdpVcrTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->vetoCounterTPRight.trackId ) );
        return evSObject->vetoCounterTPRight.trackId;
    }
    if ( var.name == CexmcCFVarOpdpClPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPLeft.positionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPLeft.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPLeft.positionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPLeft.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPLeft.directionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPLeft.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPLeft.directionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPLeft.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpClMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->calorimeterTPLeft.momentumAmp ) );
        return evSObject->calorimeterTPLeft.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdpClTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->calorimeterTPLeft.trackId ) );
        return evSObject->calorimeterTPLeft.trackId;
    }
    if ( var.name == CexmcCFVarOpdpCrPosL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPRight.positionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPRight.positionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrPosW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPRight.positionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPRight.positionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrDirL )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPRight.directionLocal,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPRight.directionLocal,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrDirW )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetThreeVectorElementAddrByIndex(
                        evSObject->calorimeterTPRight.directionWorld,
                        var.index1 ) ) );
        return GetThreeVectorElementByIndex(
                        evSObject->calorimeterTPRight.directionWorld,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpdpCrMom )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->calorimeterTPRight.momentumAmp ) );
        return evSObject->calorimeterTPRight.momentumAmp;
    }
    if ( var.name == CexmcCFVarOpdpCrTid )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->calorimeterTPRight.trackId ) );
        return evSObject->calorimeterTPRight.trackId;
    }
    if ( var.name == CexmcCFVarIpSCM )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.incidentParticleSCM,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                        evSObject->productionModelData.incidentParticleSCM,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarIpLAB )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.incidentParticleLAB,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                        evSObject->productionModelData.incidentParticleLAB,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpSCM )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.nucleusParticleSCM,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                        evSObject->productionModelData.nucleusParticleSCM,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNpLAB )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.nucleusParticleLAB,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                        evSObject->productionModelData.nucleusParticleLAB,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpSCM )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.outputParticleSCM,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                        evSObject->productionModelData.outputParticleSCM,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarOpLAB )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.outputParticleLAB,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                        evSObject->productionModelData.outputParticleLAB,
                        var.index1 );
    }
    if ( var.name == CexmcCFVarNopSCM )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.nucleusOutputParticleSCM,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                    evSObject->productionModelData.nucleusOutputParticleSCM,
                    var.index1 );
    }
    if ( var.name == CexmcCFVarNopLAB )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    GetLorentzVectorElementAddrByIndex(
                        evSObject->productionModelData.nucleusOutputParticleLAB,
                        var.index1 ) ) );
        return GetLorentzVectorElementByIndex(
                    evSObject->productionModelData.nucleusOutputParticleLAB,
                    var.index1 );
    }
    if ( var.name == CexmcCFVarIpId )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->productionModelData.incidentParticle ) );
        return evSObject->productionModelData.incidentParticle;
    }
    if ( var.name == CexmcCFVarNpId )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->productionModelData.nucleusParticle ) );
        return evSObject->productionModelData.nucleusParticle;
    }
    if ( var.name == CexmcCFVarOpId )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                           &evSObject->productionModelData.outputParticle ) );
        return evSObject->productionModelData.outputParticle;
    }
    if ( var.name == CexmcCFVarNopId )
    {
        varAddrMap.insert( VarAddrMapData( var.name,
                    &evSObject->productionModelData.nucleusOutputParticle ) );
        return evSObject->productionModelData.nucleusOutputParticle;
    }
    if ( var.name == CexmcCFVarConst_eV )
    {
        varAddrMap.insert( VarAddrMapData( var.name, &constants[ 0 ] ) );
        return constants[ 0 ];
    }
    if ( var.name == CexmcCFVarConst_keV )
    {
        varAddrMap.insert( VarAddrMapData( var.name, &constants[ 1 ] ) );
        return constants[ 1 ];
    }
    if ( var.name == CexmcCFVarConst_MeV )
    {
        varAddrMap.insert( VarAddrMapData( var.name, &constants[ 2 ] ) );
        return constants[ 2 ];
    }
    if ( var.name == CexmcCFVarConst_GeV )
    {
        varAddrMap.insert( VarAddrMapData( var.name, &constants[ 3 ] ) );
        return constants[ 3 ];
    }
    if ( var.name == CexmcCFVarConst_mm )
    {
        varAddrMap.insert( VarAddrMapData( var.name, &constants[ 4 ] ) );
        return constants[ 4 ];
    }
    if ( var.name == CexmcCFVarConst_cm )
    {
        varAddrMap.insert( VarAddrMapData( var.name, &constants[ 5 ] ) );
        return constants[ 5 ];
    }
    if ( var.name == CexmcCFVarConst_m )
    {
        varAddrMap.insert( VarAddrMapData( var.name, &constants[ 6 ] ) );
        return constants[ 6 ];
    }

    throw CexmcException( CexmcCFUnexpectedVariable );

    return 0;
}


void  CexmcASTEval::GetEDCollectionValue( const CexmcAST::Node &  node,
                        CexmcEnergyDepositCalorimeterCollection &  edCol ) const
{
    if ( evSObject == NULL )
        throw CexmcException( CexmcCFUninitialized );

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

        if ( var.name == CexmcCFVarClEDCol )
        {
            if ( var.index1 != 0 || var.index2 != 0 )
                throw CexmcException( CexmcCFUnexpectedVariableUsage );
            varAddrMap.insert( VarAddrMapData( var.name,
                                &evSObject->calorimeterEDLeftCollection ) );
            edCol = evSObject->calorimeterEDLeftCollection;
            return;
        }
        if ( var.name == CexmcCFVarCrEDCol )
        {
            if ( var.index1 != 0 || var.index2 != 0 )
                throw CexmcException( CexmcCFUnexpectedVariableUsage );
            varAddrMap.insert( VarAddrMapData( var.name,
                                &evSObject->calorimeterEDRightCollection ) );
            edCol = evSObject->calorimeterEDRightCollection;
            return;
        }
    }
}

#endif

