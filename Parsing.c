while(k<100)//hwa el mafrod el rakam da yb2a < mn 100 bkter 34an adman en lsa fe data
{
	if((array[k]=='G'))
	{
      if(array[k+1]=='P')
      {
         if(array[k+2]=='R')
         {
            if(array[k+3]=='M')
            {
               if(array[k+4]=='C')
               {
                  index=k+6;
                  while(array[index]!='*')
                  {
                     GPSValues[i]=array[index];
                     index++;
                     i++;
                  }
                  index=0;
                  token = strtok(GPSValues, comma);
                  while( token != NULL )
                  {
                     strcpy(parseValue[index], token);
                     token = strtok(NULL, comma);
                     index++;
                  }
                  // rest of the code ..