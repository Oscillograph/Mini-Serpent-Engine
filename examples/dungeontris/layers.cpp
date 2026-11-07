#ifndef DUNGEONTRIS_LAYERS_CPP
#define DUNGEONTRIS_LAYERS_CPP

#include <mse/systems/windows/layers/gui/gui.h>
#include <dungeontris/layers.h>

void GameLogo(mse::Layer* layer)
{
    layer->AddElement(new mse::gui::Text(
                                  layer, 
                                  U"ПОДЗЕМЕТРИС", 
                                  {80, 12, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 255, 255, 255},
                                  2));
    layer->AddElement(new mse::gui::Text(
                                  layer, 
                                  U"ПОДЗЕМЕТРИС", 
                                  {80, 10, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 196, 0, 255},
                                  2));
    layer->AddElement(new mse::gui::Text(
                                  layer, 
                                  U"ПОДЗЕМЕТРИС", 
                                  {80, 9, 180, 20}, 
                                  {0, 0, 0, 0}, 
                                  {255, 128, 0, 255},
                                  2));
}

SimpleUILayer::SimpleUILayer() : mse::Layer()
{
    MSE_LOG("Constructed a simpleUI layer");
}

SimpleUILayer::~SimpleUILayer()
{
    MSE_LOG("Destroyed a simpleUI layer");
}

void SimpleUILayer::OnInit()
{
    mse::gui::Image* backgroundImage = (mse::gui::Image*)(AddElement(new mse::gui::Image(
                     this, 
                     {0, 0, 320, 240}, 
                     "./data/img/background.png", 
                     {0, 0, 320, 240}, 
                     {0, 0, 0, 255})));
}

void SimpleUILayer::OnUpdate(mse::TimeType t)
{}

#endif
