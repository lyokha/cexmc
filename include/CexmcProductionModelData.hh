/*
 * =============================================================================
 *
 *       Filename:  CexmcProductionModelData.hh
 *
 *    Description:  SCM/LAB lorentz vector of the particles in reaction
 *
 *        Version:  1.0
 *        Created:  01.12.2009 18:01:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexey Radkov (), 
 *        Company:  PNPI
 *
 * =============================================================================
 */

#ifndef CEXMC_PRODUCTION_MODEL_DATA_HH
#define CEXMC_PRODUCTION_MODEL_DATA_HH

#include <iosfwd>
#include <G4ParticleDefinition.hh>
#include <G4LorentzVector.hh>
#include <G4UnitsTable.hh>


struct  CexmcProductionModelData
{
    CexmcProductionModelData();

    CexmcProductionModelData( const G4LorentzVector &  incidentParticleSCM,
                          const G4LorentzVector &  incidentParticleLAB,
                          const G4LorentzVector &  nucleusParticleSCM,
                          const G4LorentzVector &  nucleusParticleLAB,
                          const G4LorentzVector &  outputParticleSCM,
                          const G4LorentzVector &  outputParticleLAB,
                          const G4LorentzVector &  nucleusOutputParticleSCM,
                          const G4LorentzVector &  nucleusOutputParticleLAB,
                          const G4ParticleDefinition *  incidentParticle,
                          const G4ParticleDefinition *  nucleusParticle,
                          const G4ParticleDefinition *  outputParticle,
                          const G4ParticleDefinition *  nucleusOutputParticle );

    G4LorentzVector  incidentParticleSCM;

    G4LorentzVector  incidentParticleLAB;

    G4LorentzVector  nucleusParticleSCM;

    G4LorentzVector  nucleusParticleLAB;

    G4LorentzVector  outputParticleSCM;

    G4LorentzVector  outputParticleLAB;

    G4LorentzVector  nucleusOutputParticleSCM;

    G4LorentzVector  nucleusOutputParticleLAB;

    const G4ParticleDefinition *  incidentParticle;

    const G4ParticleDefinition *  nucleusParticle;

    const G4ParticleDefinition *  outputParticle;

    const G4ParticleDefinition *  nucleusOutputParticle;
};


std::ostream &  operator<<( std::ostream &  out,
                            const CexmcProductionModelData &  data );


#endif

