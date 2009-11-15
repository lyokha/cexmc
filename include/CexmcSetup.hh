/*
 * =============================================================================
 *
 *       Filename:  CexmcSetup.hh
 *
 *    Description:  physical setup
 *
 *        Version:  1.0
 *        Created:  10.10.2009 23:15:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_SETUP_HH
#define CEXMC_SETUP_HH

#include <G4VUserDetectorConstruction.hh>

class  G4GDMLParser;


class  CexmcSetup : public G4VUserDetectorConstruction
{
    public:
        explicit CexmcSetup( const G4String &  gdmlFile = "default.gdml" );

        G4VPhysicalVolume *  Construct( void );

    private:
        void  SetupSensitiveVolumes( G4GDMLParser &  gdmlParser );

    private:
        G4VPhysicalVolume *  world;

        G4String             gdmlFile;
};


#endif

