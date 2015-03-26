// Copyright 2015, Mark Dyer

#include "BaseComponent.h"
#include "Controller.h"

void BaseComponent::handleMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  _controller->onMessage(cmd, len, buffer);
}
