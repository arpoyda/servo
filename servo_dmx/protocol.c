#inclue "protocol.h"

uint16_t CRC16_2(uint8_t *buf, uint8_t len){
  uint16_t crc = 0xFFFF;

  for (uint8_t pos = 0; pos < len; pos++)
  {
    crc ^= (uint8_t)buf[pos];          // XOR byte into least sig. byte of crc

    for (uint8_t i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;
}

void modbus_init(uint8_t mode){
  modbus_id = modbus_id_def;
  bitrade = DEFBOUND;
  
  UART_init(115200, 8000000);
}

void USART1_RXHandeler(uint8_t* data, uint32_t rx_len)
{
  uint16_t address, count;
  uint16_t crc;
  uint8_t answ;
  static uint8_t modbus_dataTX[64];
 
  if (len<8) return; // CHECK LATER!------------------------------------------------
  if ((data[4] != modbus_id) && (data[4] !=253)) return;
  
  modbus_dataTX[0] = 0xFF;//CONST = FF
  modbus_dataTX[1] = 0xFF;//CONST = FF
  modbus_dataTX[2] = 0xFD;//CONST = FD
  modbus_dataTX[3] = 0x00;//CONST = 00
  
  len = data[6]<<8 | data[5];
  //address = data[2] << 8 | data[3];
  //count  = data[4] << 8 | data[5];
  crc = data[rx_len-2] | data[rx_len-1]<<8; //fucking other byteorder
  answ = 0;
  
  if (CRC16_2(data, rx_len-2) != crc) return; // CHECK LATER!--------------------------
    
  recivcommand = 1;  

  switch(data[7]){
  case READ_INSTR:
        address = data[9]<<8 | data[8];
        size = data[11]<<8 | data[10]
        
        modbus_dataTX[4] = modbus_id;//ID
        modbus_dataTX[5] = (size+3) & 0xFF;//LEN_L
        modbus_dataTX[6] = ((size+3) >> 8) & 0xFF;//LEN_H
        modbus_dataTX[7] = STATUS_INSTR;
        modbus_dataTX[8] = ERR_CODE;
        int16_t tp;
        int16_t i;
        for(i=0;i<size;i++) {
              if (is_reg(address+i)) {
                  tp = read_register(address+i);
                  modbus_dataTX[i+9] = (uint8_t)tp;
                  answ = 1;
               }
        }
        if (answ) {
          crc = CRC16_2(modbus_dataTX, size+9);
        
          modbus_dataTX[size+9] = (crc & 0x00FF);
          modbus_dataTX[size+10] = (crc >>8);
          uart_tx(modbus_dataTX,size+11);
       }
    break;
    
  case WRITE_INSTR:
    address = data[9]<<8 | data[8];
    
    modbus_dataTX[4] = modbus_id;//ID
    modbus_dataTX[5] = 0x04;//LEN_L
    modbus_dataTX[6] = 0x00;//LEN_H
    modbus_dataTX[7] = STATUS_INSTR;
    modbus_dataTX[8] = ERR_CODE;
        
        
    if (is_reg(address)) {
      int16_t i;
      for (i=0; i<(len-4);i++){
         write_register(address+i, data[i+10]);// CHECK LATER!--------------------------
      }
      answ = 1;// CHECK LATER!----------------------------------------------------------
    }
    
    if (answ) { 
      crc = CRC16_2(modbus_dataTX, 9)
      modbus_dataTX[9] = (crc & 0x00FF);
      modbus_dataTX[10] = (crc >>8);
      uart_dma_tx(modbus_dataTX, 11)
    }
    break;
  }
}

