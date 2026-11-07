#include <dungeontris/layers.h>

void CharacterSaveUILayer::OnInit()
{}

void CharacterSaveUILayer::OnUpdate(mse::TimeType t)
{}

CharacterUpdateUILayer::CharacterUpdateUILayer() : mse::Layer()
{
	MSE_LOG("Constructed CharacterUpdateUILayer");
}
CharacterUpdateUILayer::~CharacterUpdateUILayer()
{
	MSE_LOG("Destroyed CharacterUpdateUILayer");

	delete shopLayer;
	shopLayer = nullptr;
}
