#include <dungeontris/layers.h>

CharacterLoadUILayer::CharacterLoadUILayer() : mse::Layer()
{
	MSE_LOG("Constructed CharacterLoadUILayer");
}
CharacterLoadUILayer::~CharacterLoadUILayer()
{
	MSE_LOG("Destroyed CharacterLoadUILayer");
}

void CharacterLoadUILayer::OnInit()
{}

void CharacterLoadUILayer::OnUpdate(mse::TimeType t)
{}

CharacterSaveUILayer::CharacterSaveUILayer() : mse::Layer()
{
	MSE_LOG("Constructed CharacterSaveUILayer");
}
CharacterSaveUILayer::~CharacterSaveUILayer()
{
	MSE_LOG("Destroyed CharacterSaveUILayer");
}
