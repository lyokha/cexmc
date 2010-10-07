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
#include <G4AffineTransform.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>

class  G4GDMLParser;
class  G4LogicalVolume;
class  G4VPhysicalVolume;


class  CexmcSetup : public G4VUserDetectorConstruction
{
    public:
        enum  SpecialVolumeType
        {
            Monitor,
            VetoCounter,
            Calorimeter,
            Target
        };

        struct  CalorimeterGeometryData
        {
            CalorimeterGeometryData() :
                nCrystalsInColumn( 1 ), nCrystalsInRow( 1 ), crystalWidth( 0 ),
                crystalHeight( 0 ), crystalLength( 0 )
            {}

            G4int     nCrystalsInColumn;

            G4int     nCrystalsInRow;

            G4double  crystalWidth;

            G4double  crystalHeight;

            G4double  crystalLength;
        };

    public:
        explicit CexmcSetup( const G4String &  gdmlFile = "default.gdml",
                             G4bool  validateGDMLFile = true );

        G4VPhysicalVolume *  Construct( void );

    public:
        const G4AffineTransform &  GetTargetTransform( void ) const;

        const G4AffineTransform &  GetCalorimeterLeftTransform( void ) const;

        const G4AffineTransform &  GetCalorimeterRightTransform( void ) const;

        void   ConvertToCrystalGeometry( const G4ThreeVector &  src,
                    G4int &  row, G4int &  column, G4ThreeVector &  dst ) const;

        const CalorimeterGeometryData &  GetCalorimeterGeometry( void ) const;

        const G4LogicalVolume *  GetVolume( SpecialVolumeType  volume ) const;

    private:
        void   SetupSpecialVolumes( G4GDMLParser &  gdmlParser );

        void   ReadTransforms( const G4GDMLParser & gdmlParser );

        void   ReadCalorimeterGeometryData( const G4LogicalVolume * lVolume );

        void   RotateMatrix( const G4ThreeVector &  pos,
                             G4RotationMatrix &  rm );

    private:
        G4VPhysicalVolume *      world;

        G4String                 gdmlFile;

        G4bool                   validateGDMLFile;

        G4bool                   calorimeterRegionInitialized;

        G4bool                   calorimeterGeometryDataInitialized;

        G4LogicalVolume *        monitorVolume;

        G4LogicalVolume *        vetoCounterVolume;

        G4LogicalVolume *        calorimeterVolume;

        G4LogicalVolume *        targetVolume;

        G4AffineTransform        targetTransform;

        G4AffineTransform        calorimeterLeftTransform;

        G4AffineTransform        calorimeterRightTransform;

        CalorimeterGeometryData  calorimeterGeometry;
};


inline const G4AffineTransform &  CexmcSetup::GetTargetTransform( void ) const
{
    return targetTransform;
}


inline const G4AffineTransform &  CexmcSetup::GetCalorimeterLeftTransform(
                                                                    void ) const
{
    return calorimeterLeftTransform;
}


inline const G4AffineTransform &  CexmcSetup::GetCalorimeterRightTransform(
                                                                    void ) const
{
    return calorimeterRightTransform;
}


inline const CexmcSetup::CalorimeterGeometryData &
                                CexmcSetup::GetCalorimeterGeometry( void ) const
{
    return calorimeterGeometry;
}


inline const G4LogicalVolume *  CexmcSetup::GetVolume(
                                            SpecialVolumeType  volume ) const
{
    switch ( volume )
    {
    case Monitor :
        return monitorVolume;
    case VetoCounter :
        return vetoCounterVolume;
    case Calorimeter :
        return calorimeterVolume;
    case Target :
        return targetVolume;
    default :
        break;
    }

    return NULL;
}


#endif

