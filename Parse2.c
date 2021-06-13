void parse_float_transmit_uart(float dist)
{
char out [20]; int k=0;
int count=0,i=0;
int n_dist=(int)dist;
int f_dist=((dist-n_dist)*100000);
char n;
	if(f_dist ==0)
	{
		n='0';
	out[i]=n;
		i++;
		count++;
	}
	
while(f_dist)
{

    n = (f_dist % 10)+'0';
    f_dist /= 10;
    out[i]=n;
    count++;
    i++;
}

n='.';
out[i]=n;
i++;
count++;
if(n_dist == 0)
{
n='0';
	out[i]=n;
	count++;
}

    while(n_dist)
{

    n = (n_dist % 10)+'0';
    n_dist /= 10;
    out[i]=n;
    count++;
    i++;
}


for(k=count-1; k>=0;k-- )
{
	UART0_Transmitter(out[k]);
}

UART0_Transmitter('\n');

}
