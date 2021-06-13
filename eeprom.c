void eeprom_init()
{	
	// 1- enable eeprom usage
	SYSCTL_RCGCEEPROM_R  |= SYSCTL_RCGCEEPROM_R0;  //set bit 0 by 1 
	Delay(1000);
	while ((SYSCTL_PREEPROM_R & SYSCTL_RCGCEEPROM_R0 ) == 0) ;    //wait until ready
	
	//3- check for done 
	while((EEPROM_EEDONE_R  & 0x01 ) != 0x00) ;  //wait for the task to be completed
	
	//4- check for errors
	while((EEPROM_EESUPP_R & 0x0C) == 0x01) ;  //continue when they are clear
	
	//5- reset
	SYSCTL_SREEPROM_R = 0x01;
	//delay
	Delay(3000);
	SYSCTL_SREEPROM_R = 0x00;
	while ((SYSCTL_PREEPROM_R ) != 0x01) ; //wait until ready
	
	//delay
	Delay(1000);
	//6- check for done 
	while((EEPROM_EEDONE_R & 0x01 ) != 0x00) ;  //wait for the task to be completed
	
	//7- check for errors
	while((EEPROM_EESUPP_R & 0x0C) == 0x01) ;  //continue when they are clear
 
}

void write(float array_of_eeprom_tohex[],  uint32_t count_eeprom)  
{
	uint32_t i=0x0;   /////////offset 1
	uint32_t j=0x0;
	EEPROM_EEBLOCK_R =0x00;
	EEPROM_EEOFFSET_R = i;  EEPROM_EERDWR_R = count_eeprom;
	i = 0x01;
	while(j<count_eeprom)
	{
		EEPROM_EEOFFSET_R = i;  EEPROM_EERDWR_R = array_of_eeprom_tohex[j];
		i+=0x01;
		j+=0x01;
		if ( i == 0x10 ) 
		{
			EEPROM_EEBLOCK_R = EEPROM_EEBLOCK_R + 0x01;
			i=0x00;
		}
		while(EEPROM_EEDONE_R != 0x00);
	}
}


void read(void)  //uint32_t data_size
{
	uint32_t i=0;
	uint32_t j=0;
	float data;
	
	while(1)
	{
			EEPROM_EEBLOCK_R = j;
			while (i<=(0xF))
			{
		EEPROM_EEOFFSET_R = i;  data=EEPROM_EERDWR_R ;
				parse_float(data);
		i+=0x01;
	}
			j+=0x01;
	    i=0x00;
	
	}
	
}
