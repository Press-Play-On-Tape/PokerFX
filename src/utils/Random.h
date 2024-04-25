#pragma once

uint16_t rnd = 0xACE1;

uint8_t randomLFSR(uint8_t min, uint8_t max) {
  uint16_t r = rnd;
  r ^= TCNT0; // add some extra timing randomness
  (r & 1) ? r = (r >> 1) ^ 0xB400 : r >>= 1;
  rnd = r;
  return r % (max - min) + min;
}