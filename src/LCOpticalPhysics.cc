#include "LCOpticalPhysics.hh"

// Geant4
#include "G4Version.hh"

LCOpticalPhysics::LCOpticalPhysics(const G4String& name) :
        G4VPhysicsConstructor(name) {
}

LCOpticalPhysics::~LCOpticalPhysics() {
}

void LCOpticalPhysics::ConstructParticle() {
    // optical photon
    G4OpticalPhoton::OpticalPhotonDefinition();
}

void LCOpticalPhysics::ConstructProcess() {
    ConstructOp();
}

void LCOpticalPhysics::ConstructOp() {
    theCerenkovProcess = new G4Cerenkov("Cerenkov");
    theScintillationProcess = new G4Scintillation("Scintillation");
    theAbsorptionProcess = new G4OpAbsorption();
    theRayleighScatteringProcess = new G4OpRayleigh();
    theBoundaryProcess = new G4OpBoundaryProcess();

    theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
#if G4VERSION_NUMBER < 1100
    // Removed in Geant4 11: the yield factor is now a material property
    // (SCINTILLATIONYIELD), and the default of 1 is equivalent to this call.
    theScintillationProcess->SetScintillationYieldFactor(1.);
#endif

    auto particleIterator = GetParticleIterator();
    particleIterator->reset();

    while ((*particleIterator)()) {

        G4ParticleDefinition* particle = particleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        if (theCerenkovProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(theCerenkovProcess);
            pmanager->SetProcessOrderingToLast(theCerenkovProcess, idxPostStep);
        }
        if (theScintillationProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(theScintillationProcess);
            pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
            pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
        }
        if (particleName == "opticalphoton") {
            pmanager->AddDiscreteProcess(theAbsorptionProcess);
            pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
            pmanager->AddDiscreteProcess(theBoundaryProcess);
        }
    }
}
