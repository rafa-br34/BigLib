#pragma once
#include "../../Includes.h"

#define MAX(A, B) ((A) > (B) ? (A) : (B))

namespace NeuralNetworks {
	namespace ActivationFunctions {
		template <typename ValueType>
		inline ValueType Step(ValueType Input) {
			return (Input > ValueType(0)) ? ValueType(1) : ValueType(0);
		}

		template <typename ValueType>
		inline ValueType Sigmoid(ValueType Input) {
			return ValueType(1) / (ValueType(1) + exp(-Input));
		}

		template <typename ValueType>
		inline ValueType HyperbolicTangent(ValueType Input) {
			ValueType Value = exp(ValueType(2) * Input);
			return (Value - ValueType(1)) / (Value + ValueType(1));
		}

		template <typename ValueType>
		inline ValueType SiLU(ValueType Input) {
			return Input / (ValueType(1) + exp(-Input));
		}

		template <typename ValueType>
		inline ValueType ReLU(ValueType Input) {
			return MAX(ValueType(0), Input);
		}
	}


	template <typename ValueType>
	inline ValueType ActivationFunction(ValueType Input) {
		return ActivationFunctions::ReLU(Input);
	}

	template <typename ValueType>
	struct Neuron {
		ValueType ActivationValue = {};
		ValueType Bias;

		std::vector<ValueType> Weights = {};
		std::vector<Neuron<ValueType>*> Outputs = {};
	};

	template <typename NeuronType>
	struct Layer {
		std::vector<Neuron<NeuronType>*> Neurons = {};
	};

	template <typename NeuronType>
	class NeuralNetwork {
	public:
		std::vector<Layer<NeuronType>*> Layers = {};

		void Evaluate() {
			for (auto Layer : this->Layers) {
				EvaluateLayer(Layer);
			}
		}
		void EvaluateLayer(Layer<NeuronType>* ToEval) {
			for (auto Neuron : ToEval->Neurons) {

				// Evaluate Each Neuron Connection
				for (size_t i = 0; i < Neuron->Outputs.size(); i++) {
					Neuron->Outputs[i]->ActivationValue = ActivationFunction((Neuron->ActivationValue * Neuron->Weights[i]) + Neuron->Bias);
				}
			}
		}
		Layer<NeuronType>* GetFirstLayer() {
			return this->Layers[0];
		}
		Layer<NeuronType>* GetLastLayer() {
			return this->Layers[this->Layers.size() - 1];
		}

		Layer<NeuronType>* AddLayer(size_t Size) {
			Layer<NeuronType>* NeuralLayer = new Layer<NeuronType>;

			NeuralLayer->Neurons.reserve(Size);

			for (size_t n = 0; n < Size; n++) {

				auto NewNeuron = new Neuron<NeuronType>;

				NeuralLayer->Neurons.push_back(NewNeuron);
			}
			this->Layers.push_back(NeuralLayer);
			return NeuralLayer;
		}


		~NeuralNetwork() {
			for (auto* Layer : this->Layers) {
				for (auto* Neuron : Layer->Neurons) {
					delete Neuron;
				}
				delete Layer;
			}
		}
	};

	template <typename NeuronType>
	NeuralNetwork<NeuronType>* NewNetwork(size_t Layers, size_t* LayersSize) {
		auto NeuralNet = new NeuralNetwork<NeuronType>;

		Layer<NeuronType>* LastLayer = nullptr;
		for (size_t i = 0; i < Layers; i++) {
			auto CurrentLayer = NeuralNet->AddLayer(LayersSize[i]);

			if (LastLayer) {
				for (auto CurrentNeuron : LastLayer->Neurons) {
					for (auto NextNeuron : CurrentLayer->Neurons) {
						CurrentNeuron->Outputs.push_back(NextNeuron);
						CurrentNeuron->Weights.push_back(NeuronType{});
					}
				}
			}

			LastLayer = CurrentLayer;
		}

		return NeuralNet;
	}
}

