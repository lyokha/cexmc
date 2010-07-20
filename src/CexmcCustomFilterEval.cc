/*
 * =============================================================================
 *
 *       Filename:  CexmcCustomFilterEval.cc
 *
 *    Description:  custom filter eval
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

#include <fstream>
#include <string>
#include "CexmcCustomFilterEval.hh"
#include "CexmcException.hh"


CexmcCustomFilterEval::CexmcCustomFilterEval( const G4String &  sourceFileName,
                                  const CexmcEventFastSObject *  evFastSObject,
                                  const CexmcEventSObject *  evSObject ) :
    astEval( evFastSObject, evSObject )
{
    std::string     line;
    std::ifstream   sourceFile( sourceFileName );

    if ( ! sourceFile )
        throw CexmcException( CexmcCFBadSource );

    while ( ! sourceFile.eof() )
    {
        std::getline( sourceFile, line );
        size_t  commentStartPos( line.find_first_of( '#' ) );
        if ( commentStartPos != std::string::npos )
            line.erase( commentStartPos );

        if ( ! line.empty() )
        {
            CexmcCustomFilter::ParseResult  curParseResult;

            std::string::const_iterator   begin( line.begin() );
            std::string::const_iterator   end( line.end() );

            if ( ! CexmcCustomFilter::phrase_parse( begin, end, grammar,
                                CexmcCustomFilter::space, curParseResult ) ||
                 begin != end )
            {
                throw CexmcException( CexmcCFParseError );
            }

#ifdef CEXMC_DEBUG_CF
            G4cout << "Parsed expression AST:" << G4endl;
            curParseResult.expression.Print();
#endif

            switch ( curParseResult.action )
            {
            case CexmcCustomFilter::KeepTPT :
            case CexmcCustomFilter::DeleteTPT :
                parseResultTPT.push_back( curParseResult );
                break;
            case CexmcCustomFilter::KeepEDT :
            case CexmcCustomFilter::DeleteEDT :
                parseResultEDT.push_back( curParseResult );
                break;
            default :
                break;
            }
        }
    }

    sourceFile.close();
}


bool  CexmcCustomFilterEval::EvalTPT( void ) const
{
    for ( ParseResultVector::const_iterator  k( parseResultTPT.begin() );
          k != parseResultTPT.end(); ++k )
    {
        if ( astEval( k->expression ) )
            return k->action == CexmcCustomFilter::KeepTPT;
    }

    return true;
}


bool  CexmcCustomFilterEval::EvalEDT( void ) const
{
    for ( ParseResultVector::const_iterator  k( parseResultEDT.begin() );
          k != parseResultEDT.end(); ++k )
    {
        if ( astEval( k->expression ) )
            return k->action == CexmcCustomFilter::KeepEDT;
    }

    return true;
}


#endif

