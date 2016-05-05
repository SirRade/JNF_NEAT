#include <algorithm>
#include <stdexcept>
#include "neural_network_trainer.h"

NeuralNetworkTrainer::NeuralNetworkTrainer(std::vector<ITrainable*>& population, const TrainingParameters& parameters) :
	parameters(parameters)
{
	SetPopulation(population);
}

NeuralNetworkTrainer::NeuralNetworkTrainer(std::vector<ITrainable*>& population, TrainingParameters&& parameters) :
    parameters(parameters)
{
	SetPopulation(population);
}




void NeuralNetworkTrainer::ResetPopulationToTeachableState()
{
	for (auto& individuum : population) {
		individuum.Reset();
	}
}

void NeuralNetworkTrainer::SetPopulation(std::vector<ITrainable*>& population)
{
    this->population.clear();
	this->population.reserve(population.size());
	Genome standardGenes(parameters);
    for (auto& currTrainable : population) {
		NeuralNetwork network(parameters, standardGenes);
		Individual individual(currTrainable, std::move(network));
		this->population.push_back(std::move(individual));
    }
	CategorizePopulationIntoSpecies();
}


void NeuralNetworkTrainer::TrainUntilFitnessEquals(int fitnessToReach) {
	LetGenerationLive();
	while (GetFittestSpecimen().GetOrCalculateFitness() < fitnessToReach) {
		Repopulate();
		LetGenerationLive();
	}
}

void NeuralNetworkTrainer::TrainUntilGenerationEquals(unsigned int generationsToTrain) {
	LetGenerationLive();
	for (unsigned int i = 0; i < generationsToTrain; ++i) {
		Repopulate();
		LetGenerationLive();
	}
}

Individual& NeuralNetworkTrainer::GetFittestSpecimen() {
	if (population.empty()) {
		throw std::out_of_range("Your population is empty");
	}

	auto compareFitness = [](Individual& lhs, Individual& rhs) {
		return lhs.GetOrCalculateFitness() < rhs.GetOrCalculateFitness();
	};
	// TODO jnf cache this
	return *std::max_element(population.begin(), population.end(), compareFitness);
}

void NeuralNetworkTrainer::LetGenerationLive() {
	for (int i = 0; i < parameters.updatesPerGeneration; ++i) {
		for (auto& individual : population) {
			individual.Update();
		}
	}
}

void NeuralNetworkTrainer::Repopulate() {
	std::vector<Individual> newPopulation;
	newPopulation.reserve(population.size());
	for (auto& currSpecies : species) {
		// TODO jnf Breeding
	}
	population = std::move(newPopulation);
	CategorizePopulationIntoSpecies();
	ResetPopulationToTeachableState();
    // TODO jnf Add Concurrency
}


void NeuralNetworkTrainer::CategorizePopulationIntoSpecies()
{
	for (auto& individual : population) {
		bool isCompatibleWithExistingSpecies = false;
		for (auto& currSpecies : species) {
			if (currSpecies.IsCompatible(individual.GetGenome())) {
				individual.CoupleWithSpecies(currSpecies);
				isCompatibleWithExistingSpecies = true;
				break;
			}
		}
		if (!isCompatibleWithExistingSpecies) {
			Species newSpecies(parameters);
			species.push_back(std::move(newSpecies));
			individual.CoupleWithSpecies(species.back());
		}
	}
	// TODO jnf Clear empty species
}