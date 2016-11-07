﻿#pragma once
#include "species.hpp"

namespace Hippocrates {
class Organism;

class SpeciesManager {
private:
	const TrainingParameters& parameters;
	mutable bool areSpeciesSortedByFitness = false;
	std::vector<Species> species;

public:
	using Bodies = std::vector<std::reference_wrapper<IBody>>;

	explicit SpeciesManager(const TrainingParameters& parameters) : parameters{ parameters } {}
	auto operator[](std::size_t index) const -> const Species& { return species[index]; }

	auto CreateInitialOrganisms(Bodies& bodies) -> void;
	auto Repopulate(Bodies& bodies) -> void;
	auto BreedInSpecies(const Species & species) const -> NeuralNetwork;
	auto GetFittestSpecies() -> const Species &;
	auto GetFittestOrganism() -> const Organism&;

	auto LetGenerationLive() -> void;

	auto GetSpeciesCount() const { return species.size(); }
	auto GetPopulationCount() const -> std::size_t;
	auto GetTotalFitness() const -> double;
	auto GetAverageFitness() const -> double;

	auto begin() const { return species.begin(); }
	auto end() const { return species.end(); }

private:
	auto ResetPopulationToTeachableState() -> void;
	auto FillOrganismIntoSpecies(Organism&& organism) -> void;
	auto PrepareSpeciesForPopulation() -> void;
	auto AnalyzeSpeciesPopulation() -> void;
	auto DeleteStagnantSpecies() -> void;
	auto DeleteEmptySpecies() -> void;
	auto SortSpeciesIfNeeded() -> void;
	auto ClearSpeciesPopulation() -> void;
};


}
