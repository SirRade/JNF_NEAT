﻿#pragma once
#include "phenotype/network_components/input_adaptor.hpp"
#include "phenotype/network_components/output_adaptor.hpp"
#include "common/utility/source/header/interface.hpp"

namespace Hippocrates::Phenotype::NetworkComponents {

class Connection : public Utility::Interface {
private:
	OutputAdaptor& source;
	InputAdaptor& destination;
	Type::connection_weight_t&& weight;
	bool isRecursive;

public:
	Connection(OutputAdaptor& from, InputAdaptor& to, Type::connection_weight_t&& weight, bool isRecursive = false) : 
	source(from), 
	destination(to), 
	weight(std::move(weight)),
	isRecursive(isRecursive) {  }

	auto GetSourceAdaptor() const -> OutputAdaptor& { return source; }
	auto GetDestinationAdaptor() const -> InputAdaptor& { return destination; }
	auto GetWeight() const -> Type::connection_weight_t { return weight; }
	auto SetWeight(Type::connection_weight_t new_weight) const -> void { weight = std::move(new_weight); }
};

}