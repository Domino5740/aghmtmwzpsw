void rf_init_as_tx(void);
void rf_clear_int_flags(void);
void rf_send_byte(unsigned char cByte);
void rf_wait_until_data_sent(void);
void set_bit_in_reg(unsigned char ucAddr,unsigned char ucBitPosition, unsigned char ucBitValue);