#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
  public:
	PrimaryGeneratorAction();
	~PrimaryGeneratorAction();

	void GeneratePrimaries(G4Event *anEvent);

  private:
	G4GeneralParticleSource *particleGun;
};

#endif
