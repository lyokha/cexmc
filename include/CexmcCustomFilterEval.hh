/*
 * =============================================================================
 *
 *       Filename:  CexmcCustomFilterEval.hh
 *
 *    Description:  custom filter eval
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

#ifndef CEXMC_CUSTOM_FILTER_EVAL_HH
#define CEXMC_CUSTOM_FILTER_EVAL_HH

#ifdef CEXMC_USE_CUSTOM_FILTER

#include <vector>
#include "CexmcASTEval.hh"
#include "CexmcCustomFilter.hh"

class  CexmcEventFastSObject;
class  CexmcEventSObject;


class  CexmcCustomFilterEval : public CexmcAST::BasicEval
{
    private:
        typedef std::vector< CexmcCustomFilter::ParseResult >
                                                        ParseResultVector;

    public:
        CexmcEventFastSObject &  GetEventFastSObject( void );

        CexmcEventSObject &      GetEventSObject( void );

    private:
        CexmcASTEval       astEval;

        ParseResultVector  parseResult; 
};


inline CexmcEventFastSObject &  CexmcCustomFilterEval::GetEventFastSObject(
                                                                        void )
{
    return astEval.GetEventFastSObject();
}


inline CexmcEventSObject &  CexmcCustomFilterEval::GetEventSObject( void )
{
    return astEval.GetEventSObject();
}

#endif

#endif

