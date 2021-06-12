void parse_float(float dist)
{
//string out;
char out [20];
int count=0,i=0;
int n_dist=(int)dist;
int f_dist=((dist-n_dist)*100000);
char n;
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

    while(n_dist)
{

    n = (n_dist % 10)+'0';
    n_dist /= 10;
    out[i]=n;
    count++;
    i++;
}


for(int i=count-1; i>=0;i-- )
{
    cout<<out[i]<<endl;
    //LCD_DATA(out[i])
}
}
