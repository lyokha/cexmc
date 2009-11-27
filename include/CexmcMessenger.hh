/*
 * =============================================================================
 *
 *       Filename:  CexmcMessenger.hh
 *
 *    Description:  common messenger stuff (directories etc.)
 *
 *        Version:  1.0
 *        Created:  15.11.2009 12:48:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_MESSENGER_HH
#define CEXMC_MESSENGER_HH

#include <G4String.hh>

class  G4UIdirectory;


class  CexmcMessenger
{
    public:
        static CexmcMessenger *  Instance( void );

        static void              Destroy( void );

    private:
        CexmcMessenger();

        ~CexmcMessenger();

    public:
        static G4String  mainDirName;

        static G4String  geometryDirName;

        static G4String  physicsDirName;

        static G4String  gunDirName;

        static G4String  detectorDirName;

        static G4String  eventDirName;

        static G4String  runDirName;

    private:
        static CexmcMessenger *  instance;

    private:
        G4UIdirectory *  mainDir;

        G4UIdirectory *  geometryDir;

        G4UIdirectory *  physicsDir;

        G4UIdirectory *  gunDir;

        G4UIdirectory *  detectorDir;

        G4UIdirectory *  eventDir;

        G4UIdirectory *  runDir;
};


#endif

