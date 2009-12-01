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

#include <iostream>
#include <G4ParticleDefinition.hh>
#include <G4LorentzVector.hh>
#include <G4UnitsTable.hh>


struct  CexmcProductionModelData
{
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


inline std::ostream &  operator<<( std::ostream &  out,
                                        const CexmcProductionModelData &  data )
{
    std::ostream::fmtflags  savedFlags( out.flags() );

    out.precision( 4 );
    out << std::endl << std::fixed;

    out << "       Incident particle       (LAB) : " <<
           data.incidentParticle->GetParticleName() << " " <<
           G4BestUnit( data.incidentParticleLAB, "Energy" ) << " -- " <<
           G4BestUnit( data.incidentParticleLAB.e(), "Energy" ) << std::endl;
    out << "                               (SCM) : " <<
           data.incidentParticle->GetParticleName() << " " <<
           G4BestUnit( data.incidentParticleSCM, "Energy" ) << " -- " <<
           G4BestUnit( data.incidentParticleSCM.e(), "Energy" ) << std::endl;
    out << "       Nucleus particle        (LAB) : " <<
           data.nucleusParticle->GetParticleName() << " " <<
           G4BestUnit( data.nucleusParticleLAB, "Energy" ) << " -- " <<
           G4BestUnit( data.nucleusParticleLAB.e(), "Energy" ) << std::endl;
    out << "                               (SCM) : " <<
           data.nucleusParticle->GetParticleName() << " " <<
           G4BestUnit( data.nucleusParticleSCM, "Energy" ) << " -- " <<
           G4BestUnit( data.nucleusParticleSCM.e(), "Energy" ) << std::endl;
    out << "       Output particle         (LAB) : " <<
           data.outputParticle->GetParticleName() << " " <<
           G4BestUnit( data.outputParticleLAB, "Energy" ) << " -- " <<
           G4BestUnit( data.outputParticleLAB.e(), "Energy" ) << std::endl;
    out << "                               (SCM) : " <<
           data.outputParticle->GetParticleName() << " " <<
           G4BestUnit( data.outputParticleSCM, "Energy" ) << " -- " <<
           G4BestUnit( data.outputParticleSCM.e(), "Energy" ) << std::endl;
    out << "       Nucleus output particle (LAB) : " <<
           data.nucleusOutputParticle->GetParticleName() << " " <<
           G4BestUnit( data.nucleusOutputParticleLAB, "Energy" ) << " -- " <<
           G4BestUnit( data.nucleusOutputParticleLAB.e(), "Energy" ) <<
           std::endl;
    out << "                               (SCM) : " <<
           data.nucleusOutputParticle->GetParticleName() << " " <<
           G4BestUnit( data.nucleusOutputParticleSCM, "Energy" ) << " -- " <<
           G4BestUnit( data.nucleusOutputParticleSCM.e(), "Energy" ) <<
           std::endl;

    out.flags( savedFlags );

    return out;
}


#endif

