#pragma once
#include "../../Includes.h"
#include "../Math/Math.h"

namespace BigLib {
	namespace NeuralNetworks {

		template <typename ValueType>
		FORCE_INLINE ValueType ActivationFunction(ValueType Input) {
			return Math::ReLU(Input);
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

}


