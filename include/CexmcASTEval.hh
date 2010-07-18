/*
 * =============================================================================
 *
 *       Filename:  CexmcASTEval.hh
 *
 *    Description:  abstract syntax tree for custom filter eval
 *
 *        Version:  1.0
 *        Created:  17.07.2010 15:43:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_AST_EVAL_HH
#define CEXMC_AST_EVAL_HH

#ifdef CEXMC_USE_CUSTOM_FILTER

#include <map>
#include <string>
#include <boost/variant/variant.hpp>
#include "CexmcAST.hh"
#include "CexmcEventSObject.hh"
#include "CexmcEventFastSObject.hh"
#include "CexmcException.hh"
#include "CexmcCommon.hh"


class  CexmcASTEval : public CexmcAST::BasicEval
{
    private:
        typedef boost::variant< const CexmcEnergyDepositCalorimeterCollection *,
                                const bool * >    VarAddr;

        typedef std::map< std::string, VarAddr >  VarAddrMap;

    public:
        CexmcEventFastSObject &  GetEventFastSObject( void );

        CexmcEventSObject &      GetEventSObject( void );

    private:
        ScalarValueType  GetFunScalarValue( const CexmcAST::Subtree &  ast )
                                                                        const;

        ScalarValueType  GetVarScalarValue( const CexmcAST::Variable &  var )
                                                                        const;

        void             GetEDCollectionValue( const CexmcAST::Node &  node,
                    CexmcEnergyDepositCalorimeterCollection &  edCol ) const;

    private:
        G4double          GetThreeVectorElementByIndex(
                                    const CexmcSimpleThreeVectorStore &  vect,
                                    G4int  index ) const;

        G4double          GetLorentzVectorElementByIndex(
                                    const CexmcSimpleLorentzVectorStore &  vect,
                                    G4int  index ) const;

        const G4double *  GetThreeVectorElementAddrByIndex(
                                    const CexmcSimpleThreeVectorStore &  vect,
                                    G4int  index ) const;

        const G4double *  GetLorentzVectorElementAddrByIndex(
                                    const CexmcSimpleLorentzVectorStore &  vect,
                                    G4int  index ) const;

    private:
        CexmcEventFastSObject  eventFastSObject;

        CexmcEventSObject      eventSObject;

    private:
        VarAddrMap             varAddrMap;

    private:
        static const G4double  constants[];
};


inline CexmcEventFastSObject &  CexmcASTEval::GetEventFastSObject( void )
{
    return eventFastSObject;
}


inline CexmcEventSObject &  CexmcASTEval::GetEventSObject( void )
{
    return eventSObject;
}


inline  G4double  CexmcASTEval::GetThreeVectorElementByIndex(
                                const CexmcSimpleThreeVectorStore &  vect,
                                G4int  index ) const
{
    switch ( index )
    {
    case 1 :
        return vect.x;
    case 2 :
        return vect.y;
    case 3 :
        return vect.z;
    default :
        throw CexmcException( CexmcCFUnexpectedVectorIndex );
        return 0;
    }
}


inline  G4double  CexmcASTEval::GetLorentzVectorElementByIndex(
                                const CexmcSimpleLorentzVectorStore &  vect,
                                G4int  index ) const
{
    switch ( index )
    {
    case 1 :
        return vect.px;
    case 2 :
        return vect.py;
    case 3 :
        return vect.pz;
    case 4 :
        return vect.e;
    default :
        throw CexmcException( CexmcCFUnexpectedVectorIndex );
        return 0;
    }
}


inline  const G4double *  CexmcASTEval::GetThreeVectorElementAddrByIndex(
                                const CexmcSimpleThreeVectorStore &  vect,
                                G4int  index ) const
{
    switch ( index )
    {
    case 1 :
        return &vect.x;
    case 2 :
        return &vect.y;
    case 3 :
        return &vect.z;
    default :
        throw CexmcException( CexmcCFUnexpectedVectorIndex );
        return NULL;
    }
}


inline  const G4double *  CexmcASTEval::GetLorentzVectorElementAddrByIndex(
                                const CexmcSimpleLorentzVectorStore &  vect,
                                G4int  index ) const
{
    switch ( index )
    {
    case 1 :
        return &vect.px;
    case 2 :
        return &vect.py;
    case 3 :
        return &vect.pz;
    case 4 :
        return &vect.e;
    default :
        throw CexmcException( CexmcCFUnexpectedVectorIndex );
        return NULL;
    }
}

#endif

#endif

