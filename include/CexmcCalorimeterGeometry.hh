/*
 * =============================================================================
 *
 *       Filename:  CexmcCalorimeterGeometry.hh
 *
 *    Description:  calorimeter geometry (static methods)
 *
 *        Version:  1.0
 *        Created:  02.12.2009 22:38:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_CALORIMETER_GEOMETRY_HH
#define CEXMC_CALORIMETER_GEOMETRY_HH

#include <G4ThreeVector.hh>
#include <G4Types.hh>

class  G4AffineTransform;


class  CexmcCalorimeterGeometry
{
    public:
        static void  GetGeometryData( G4int &  nCrystalsInColumn,
                                      G4int &  nCrystalsInRow,
                                      G4double &  crystalWidth,
                                      G4double &  crystalHeight,
                                      G4double &  crystalLength );

        static void  GetCalorimeterLeftTransform(
                                            G4AffineTransform &  transform );

        static void  GetCalorimeterRightTransform(
                                            G4AffineTransform &  transform );

    private:
        CexmcCalorimeterGeometry();
};


#endif

