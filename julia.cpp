#include <stdio.h>
#include <cstdlib>
#include <cmath>

const  int height=1200;
const  int width=1200;//resolution
double C;

typedef struct complex_num{double r;double i;}cpln;

//polynomial roots and leading coefficient
cpln z[1000000];
int nz=0;

inline cpln subc(cpln c,cpln d){cpln r={c.r-d.r,c.i-d.i};return r;}
inline cpln mt(cpln c,cpln d){cpln r={c.r*d.r-c.i*d.i,c.r*d.i+c.i*d.r}; return r;}
inline cpln sc(cpln c,double d){c.r*=d;c.i*=d;return c;}
inline double ab(cpln c){return c.r*c.r+c.i*c.i;}

cpln f(cpln w){
  cpln diff[3];
  double optdiff=10000000000;
  for(int i=0;i<nz;i++)
    if(ab(subc(w,z[i]))*C*C<optdiff)
      {optdiff=ab(subc(w,z[i]))*C*C;
	diff[0]=sc(subc(w,z[i]),C);}
  double optdiff2=10000000000;
  for(int i=0;i<nz;i++)
    if(ab(subc(w,z[i]))*C*C<optdiff2 && ab(subc(w,z[i]))*C*C>optdiff)
      {optdiff2=ab(subc(w,z[i]))*C*C;
	diff[1]=sc(subc(w,z[i]),C);}
  double optdiff3=10000000000;
  for(int i=0;i<nz;i++)
    if(ab(subc(w,z[i]))*C*C<optdiff3 && ab(subc(w,z[i]))*C*C>optdiff2)
      {optdiff3=ab(subc(w,z[i]))*C*C;
	diff[2]=sc(subc(w,z[i]),C);}
  return mt(diff[0], mt(diff[1],diff[2]));
}

int chk(double x, double y){
  cpln w={x,y};
  //if(ab(f(w))<1) return 0;
  for(int i=0;i<120;i++){w=f(w); if(ab(w)>40000000) {if(i>8)printf("out%d",i); return i;}}
  return 127;
}

int main(int argc, char*argv[])
{
  FILE* in=fopen("1","rb");
  char pic[1600];
  char buf[50];
  for(int i=0;i<40;i++){fgets(buf, 50, in);for(int j=0;j<40;j++)pic[i*40+j]=(buf[j]==' '?0 : 1 );}
  for(int i=0;i<40;i++){
    for(int j=0;j<40;j++)putchar('0'+pic[i*40+j]);
    putchar('\n');
  }

  for(int i=0;i<40;i++)
    for(int j=0;j<40;j++)
      if(pic[i*40+j]){
	z[nz]={(j-20.0)*0.05,(i-20.0)*0.05}; nz++;
      }
   printf("%d\n", nz);
  
  sscanf(argv[1],"%lf",&C);
  char* data=(char*)malloc(width*height);//pixels of the graph we are drawing

  for(int i=0;i<width;i++){
    if(i%20==0)printf("%d\n",i);
     for(int j=0;j<height;j++)
       data[i+width*j]=chk(((double)i)*2/width-1,((double)j)*2/height-1);
  }  
  
   //output
   FILE* out=fopen("out.pgm","wb");
   fprintf(out, "P2\n%d %d\n255\n",width, height);
   for(int i=0;i<width*height;i++){
      fprintf(out,"%d",data[i]);    
      if(i==width*height-1); else if(i%64==63)fputc('\n',out);else fputc(' ',out);}
    fputc('\n',out);
   fclose(out);
}

