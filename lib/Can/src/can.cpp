
#include "can.h"
#include <Arduino.h>
#include "mcp2515.h"

#include "../../../include/mod_config.h"

extern MCP2515 mcp2515;

// Implementazione della funzione initCAN()
void initCAN() {
  // Reset del modulo
  mcp2515.reset();
  // Imposta il bitrate per la comunicazione CAN
  mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
  // Passa in modalità di configurazione
  mcp2515.setConfigMode();
  // Imposta le maschere di filtro per indirizzi a 29 bit su entrambi i buffer RX
  mcp2515.setFilterMask(MCP2515::MASK0, true, 0xFF00);
  mcp2515.setFilterMask(MCP2515::MASK1, true, 0xFF00);
  // Imposta tutti i filtri per intercettare solo i messaggi destinati al modulo (con CAN_ID spostato a sinistra di 8 bit)
  mcp2515.setFilter(MCP2515::RXF0, true, CAN_ID << 8);
  mcp2515.setFilter(MCP2515::RXF1, true, CAN_ID << 8);
  mcp2515.setFilter(MCP2515::RXF2, true, CAN_ID << 8);
  mcp2515.setFilter(MCP2515::RXF3, true, CAN_ID << 8);
  mcp2515.setFilter(MCP2515::RXF4, true, CAN_ID << 8);
  mcp2515.setFilter(MCP2515::RXF5, true, CAN_ID << 8);
  // Torna in modalità normale per iniziare la comunicazione
  mcp2515.setNormalMode();
}
