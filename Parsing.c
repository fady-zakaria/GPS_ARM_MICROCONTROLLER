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
                  //finally lets take our latitude and longitude..
		  // if 2 passed string are equale, strcmp will return 0.
		 if(strcmp(parseValue[1],"A")==0)
		  {
		        latitude=atof(parseValue[2]);
	       		longitude=atof(parseValue[4]);
				
			//latitude Calculation
			degrees=latitude/100;
			minutes=latitude-(float)(degrees*100);
			seconds=minutes/60;
			l2=degrees+seconds;

			//longitude Calculation
			degrees=longitude/100;
			minutes=longitude-(float)(degrees*100);
			seconds=minutes/60;
			lg2=degrees+seconds;
				
			if (l2 <30 || lg2 <30)
			{
			    l2 =l1;
			    lg2 = lg1;	
			}
			break;	
		  }
	      }
	      k++;
  }
	
}