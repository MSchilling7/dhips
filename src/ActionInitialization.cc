#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
	SetUserAction(new PrimaryGeneratorAction);

	RunAction *runAction = new RunAction();

	unsigned int output_flags[runAction->n_output_flags];
	output_flags[EKIN] = 1;
	output_flags[EDEP] = 0;
	output_flags[PARTICLE] = 1;
	output_flags[VOLUME] = 1;
	output_flags[POSX] = 1;
	output_flags[POSY] = 1;
	output_flags[POSZ] = 1;
	output_flags[MOMX] = 0;
	output_flags[MOMY] = 0;
	output_flags[MOMZ] = 0;
	runAction->SetOutputFlags(output_flags);

	SetUserAction(runAction);
}
