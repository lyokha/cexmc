/*
 * =============================================================================
 *
 *       Filename:  CexmcPrimitiveScorer.hh
 *
 *    Description:  primitive scorer with a messenger
 *
 *        Version:  1.0
 *        Created:  24.01.2011 18:22:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PRIMITIVE_SCORER_HH
#define CEXMC_PRIMITIVE_SCORER_HH

#include <G4VPrimitiveScorer.hh>

class  CexmcSensitiveDetectorMessenger;


class  CexmcPrimitiveScorer : public G4VPrimitiveScorer
{
    public:
        explicit CexmcPrimitiveScorer( const G4String &  name );

        virtual ~CexmcPrimitiveScorer();

    public:
        void  InitializeMessenger( void );

    protected:
        void  PrintHeader( G4int  nmbOfEntries ) const;

    private:
        CexmcSensitiveDetectorMessenger *  messenger;
};


#endif

