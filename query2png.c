/*
 * =====================================================================================
 *
 *       Filename:  qwery2png.c
 *
 *    Description:  Este software renderiza la consulta a una base de datos con salida a un archivo con extensión Portable Network Graphics (PNG). 
 *
 *        Version:  1.0
 *        Created:  14/05/15 12:59:15
 *       Revision:  none
 *       Compiler:  GNU Compiller Colection
 *
 *         Author:  Gustavo Magallanes Guijón (Doktor Wattie Ácrata), gustavo.magallanes.guijon@ciencias.unam.mx, gmagallanes@conabio.gob.mx, gustavo.magallanes@conabio.gob.mx
 *   Organization:  Comisión Nacional para el Conocimiento y Uso de la Biodiversidad (CONABIO), Universidad Nacional Autónoma de México (UNAM).
 *
 * =====================================================================================
 */

/* Archivo de cabecera que contiene las definiciones de las macros, las constantes, las declaraciones de funciones de la biblioteca libpng */
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
/* Archivo de cabecera que contiene las definiciones de las macros, las constantes, las declaraciones de funciones de la biblioteca libpq-fe. libpq is the PostgreSQL's official C application interface. */
#include <libpq-fe.h>

/* Se define la estructura pixel_t en la cual se declaran los colores: rojo, verde y azul con un ancho definido de 8 bits. ( 8-bit unsigned type )*/
typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} pixel_t;

/* La siguiente estructura utiliza el tipo size_t Según el 1999 ISO estándar de C (C99), size_t es un entero sin signo el tipo de al menos 16 bits. Cuando se utiliza un size_t objeto, se tiene que estar seguro de que en todos los contextos en que se utiliza, incluyendo la aritmética, de la que desea valores no negativos. */
typedef struct
{
  pixel_t *pixels;
  size_t width;
  size_t height;
} bitmap_t;

struct mem_encode
{
  char *buffer;
  size_t size;

};

/* A continuación se definen los prototipos */
char * str_replace (char * string, char * substr, char * replacement);


void my_png_write_data (png_structp png_ptr, png_bytep data,
			png_size_t length);

void my_png_flush (png_structp png_ptr);

void usage (void);

static pixel_t *pixel_at (bitmap_t * bitmap, int x, int y);

static int save_png_to_file (bitmap_t * bitmap, int tr);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */

int
main (int argc, char *argv[])
{
  int r, 
      g, 
      b,
      r2,
      g2,
      b2,
      tra, 
      radio, 
      anch, 
      larg,
      i, 
      j,
      k,
      k2; 

  int satur;   /* satur = 0 sin efecto de hitmap; con satur=1 con efecto de hitmap */
  
  int *hits;

  float x_m, 
		y_m, 
		X_m, 
		Y_m, 
		
		c_x, 
		c_y,
		p,
		alpha = 0.2;
 
 
  char *quer, 
    *rojo,
    *verde,
    *azul, 
	*rojo2,
    *verde2,
    *azul2,
    *radius,
    *trans,
    *ancho,
    *largo,
    *x_min,
    *y_min,
    *X_max,
    *Y_max, 
	*ip, 
	*port, 
/*  *port = "5435", */
	*password, 
	*user,
   *satura,	
/*  *user = "postgres", */
	*database;

  bitmap_t fruit;

  while ((argc > 1) && (argv[1][0] == '-'))
    {
	//printf("%c\n",argv[1][1]);
      switch (argv[1][1])
	{
  	  /* Query */
	case 'q':	
	  quer = &argv[1][2];
	  quer = str_replace(quer,"\\","");
	  quer = str_replace(quer,"\"","");
	  break;
	  /* ip */
	case 'i':
	  ip = &argv[1][2];
	  break;
	  /* database */

	case 'd':
	  database = &argv[1][2];
	  break;
	  /* user */
	case 'u':
	  user = &argv[1][2];
	  break;
	  /* password */
	case 'p':
	  password = &argv[1][2];
	  break;
	  /* Puerto */
	case 'P':
	  port = &argv[1][2];
	  break;
	  /* rojo */
	case 'r':
	  rojo = &argv[1][2];
	  break;
	  /* verde */
	case 'g':
	  verde = &argv[1][2];
	  break;
	  /* azul */
	case 'b':
	  azul = &argv[1][2];
	  break;
	  /* rojo */
	 case 'l':
	  rojo2 = &argv[1][2];
	  break;
	  /* verde */
	case 'm':
	  verde2 = &argv[1][2];
	  break;
	  /* azul */
	case 'n':
	  azul2 = &argv[1][2];
	  break;
	  /* azul */
	case 's':
	  satura = &argv[1][2];
	  break;
	  /*  transparencia */
	case 't':
	  trans = &argv[1][2];
	  break;
	  /* Radio */
	case 'R':
	  radius = &argv[1][2];
	  break;
	  /* x_min */
	case 'x':
	  x_min = &argv[1][2];
	  break;
	  /* y_min */
	case 'y':
	  y_min = &argv[1][2];
	  break;
	  /* X_max */
	case 'X':
	  X_max = &argv[1][2];
	  break;
	  /* Y_max */
	case 'Y':
	  Y_max = &argv[1][2];
	  break;
	  /* larg */
	case 'W':
	  largo = &argv[1][2];
	  break;
	  /* High */
	case 'H':
	  ancho = &argv[1][2];
	  break;
	  /* Ayuda, help */
	case 'h':
	  usage ();
	  break;
	default:
	  printf ("Wrong Argument, use -h for help: %s\n", argv[1]);
	  usage ();
	}

      ++argv;
      --argc;
    }

  PGconn *conn;
  PGresult *res;  
  conn = PQsetdbLogin (ip, port, NULL, NULL, database, user, password);	/* Con esta sentencia se establece la conexión (GENERAL) */

  if (PQstatus (conn) == CONNECTION_BAD){
    printf ("Unable to connect to database\n");}

  else
    {
      res = PQexec (conn, quer);

      if (res != NULL && PGRES_TUPLES_OK == PQresultStatus (res))
	{
	  /* Dibuja de color el cuadrito */
	  larg = atoi (largo);
	  anch = atoi (ancho);
	  fruit.width = anch;
	  fruit.height = larg;

	  fruit.pixels =
	    calloc (sizeof (pixel_t), fruit.width * fruit.height);
	 
	  /* Termina de dibujar el cuadrito */

	 satur = atoi (satura); 
	  /* Dibuja los puntos */
	   x_m = atof (x_min);
	   y_m = atof (y_min);
	   X_m = atof (X_max);
	   Y_m = atof (Y_max);
	   r = atoi (rojo);
	   g = atoi (verde);
	   b = atoi (azul);
	   if(satur == 1){
		r2 = atoi (rojo2);
		 g2 = atoi (verde2);
		 b2 = atoi (azul2); 
		
  		

		hits =  calloc (sizeof (int), fruit.width * fruit.height);
		int o;
		for(o = 0; o<fruit.width * fruit.height;o++){
			hits[o] = 0;
		}
	   }
	  for (p = PQntuples (res) - 1; p >= 0; p--)
	    {

	      /* Aqu+í va la conversion:
	       * |x_max -x_min| = 256
	       * |x - x_min|    = i
	       * */

	      c_x = atof (PQgetvalue (res, p, 0));
	      c_y = atof (PQgetvalue (res, p, 1));

	      //i = 256*((abs(c_x-(xmin)))/xmax-xmin);
	      //j = 256*((abs(c_y-(ymin)))/ymax-ymin);;

	      radio = atoi (radius);

	      i = (anch) * ((abs (c_x - (x_m))) / (X_m - x_m));	/* Está ajustado a 999 para que baje un pixel y se pueda dibujar toda la bolita. Caso 6 se le restan 2 */
	      j = (larg) * ((abs (c_y - (y_m))) / (Y_m - y_m));
	      j = larg - j;	/* Para que se ajuste al cuadro pues la coordenada (0,0) está en la esquina superior izquierda.  */
	 
	      switch(satur){
			case 0:
		  fruit.pixels[larg*j+i].red = r;	
		  fruit.pixels[larg*j+i].green = g;	
		  fruit.pixels[larg*j+i].blue = b;	

 		  for (k = 0; k<radio; k++){
			for(k2 = 0; k2<=k; k2++){
			if(k!=radio-1 || k2!=radio-1){
			if(larg*(j+k)+(i+k2) < larg*anch){
			fruit.pixels[larg*(j+k)+(i+k2)].red = r;	
			fruit.pixels[larg*(j+k)+(i+k2)].green = g;	
			fruit.pixels[larg*(j+k)+(i+k2)].blue = b;	
			}
			if(larg*(j+k2)+(i+k) < larg*anch){
			fruit.pixels[larg*(j+k2)+(i+k)].red = r;	
			fruit.pixels[larg*(j+k2)+(i+k)].green = g;	
			fruit.pixels[larg*(j+k2)+(i+k)].blue = b;	
			}
			if((larg*(j-k)+(i-k2) < larg*anch)&&(larg*(j-k)+(i-k2) >= 0)){
			fruit.pixels[larg*(j-k)+(i-k2)].red = r;	
			fruit.pixels[larg*(j-k)+(i-k2)].green = g;	
			fruit.pixels[larg*(j-k)+(i-k2)].blue = b;	
			}
			if((larg*(j-k2)+(i-k) < larg*anch)&&(larg*(j-k2)+(i-k) >= 0)){
			fruit.pixels[larg*(j-k2)+(i-k)].red = r;	
			fruit.pixels[larg*(j-k2)+(i-k)].green = g;	
			fruit.pixels[larg*(j-k2)+(i-k)].blue = b;	
			}
			if((larg*(j+k)+(i-k2) < larg*anch)&&(larg*(j+k)+(i-k2) >= 0)){
			fruit.pixels[larg*(j+k)+(i-k2)].red = r;	
			fruit.pixels[larg*(j+k)+(i-k2)].green = g;	
			fruit.pixels[larg*(j+k)+(i-k2)].blue = b;	
			}
			if((larg*(j-k2)+(i+k) < larg*anch)&&(larg*(j-k2)+(i+k) >= 0)){
			fruit.pixels[larg*(j-k2)+(i+k)].red = r;
			fruit.pixels[larg*(j-k2)+(i+k)].green = g;	
			fruit.pixels[larg*(j-k2)+(i+k)].blue = b;	
			}
			if((larg*(j-k)+(i+k2) < larg*anch)&&(larg*(j-k)+(i+k2) >= 0)){
			fruit.pixels[larg*(j-k)+(i+k2)].red = r;
			fruit.pixels[larg*(j-k)+(i+k2)].green = g;
			fruit.pixels[larg*(j-k)+(i+k2)].blue = b;	
			}
			if((larg*(j+k2)+(i-k) < larg*anch)&&(larg*(j+k2)+(i-k) >= 0)){
			fruit.pixels[larg*(j+k2)+(i-k)].red = r;	
			fruit.pixels[larg*(j+k2)+(i-k)].green = g;
			fruit.pixels[larg*(j+k2)+(i-k)].blue = b;	
			}
			}
			}
			} 
		  
			break;
			case 1:
	  
	if (radio == 1){
	
			hits[larg*j+i]++; 
		
			fruit.pixels[larg*j+i].red = round(r*(exp(-alpha*(double)hits[larg*j+i]))+r2*(1-exp(-alpha*(double)hits[larg*j+i])));
			fruit.pixels[larg*j+i].green = round(g*(exp(-alpha*(double)hits[larg*j+i]))+g2*(1-exp(-alpha*(double)hits[larg*j+i])));
			fruit.pixels[larg*j+i].blue = round(b*(exp(-alpha*(double)hits[larg*j+i]))+b2*(1-exp(-alpha*(double)hits[larg*j+i])));

	}

if (radio == 2){

			hits[larg*j+i]++; 
			fruit.pixels[larg*j+i].red = round(r*(exp(-alpha*(double)hits[larg*j+i]))+r2*(1-exp(-alpha*(double)hits[larg*j+i])));
			fruit.pixels[larg*j+i].green = round(g*(exp(-alpha*(double)hits[larg*j+i]))+g2*(1-exp(-alpha*(double)hits[larg*j+i])));
			fruit.pixels[larg*j+i].blue = round(b*(exp(-alpha*(double)hits[larg*j+i]))+b2*(1-exp(-alpha*(double)hits[larg*j+i])));
	  	
		
			hits[larg*j+i+1]++; 
			fruit.pixels[larg*j+i+1].red = round(r*(exp(-alpha*(double)hits[larg*j+i+1]))+r2*(1-exp(-alpha*(double)hits[larg*j+i+1])));
			fruit.pixels[larg*j+i+1].green = round(g*(exp(-alpha*(double)hits[larg*j+i+1]))+g2*(1-exp(-alpha*(double)hits[larg*j+i+1])));
			fruit.pixels[larg*j+i+1].blue = round(b*(exp(-alpha*(double)hits[larg*j+i+1]))+b2*(1-exp(-alpha*(double)hits[larg*j+i+1])));

  
	  		hits[larg*j+i-1]++; 
			fruit.pixels[larg*j+i-1].red = round(r*(exp(-alpha*(double)hits[larg*j+i-1]))+r2*(1-exp(-alpha*(double)hits[larg*j+i-1])));
			fruit.pixels[larg*j+i-1].green = round(g*(exp(-alpha*(double)hits[larg*j+i-1]))+g2*(1-exp(-alpha*(double)hits[larg*j+i-1])));
			fruit.pixels[larg*j+i-1].blue = round(b*(exp(-alpha*(double)hits[larg*j+i-1]))+b2*(1-exp(-alpha*(double)hits[larg*j+i-1])));
  

	  		hits[larg*(j+1)+i]++; 
			fruit.pixels[larg*(j+1)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j+1)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j+1)+i])));
			fruit.pixels[larg*(j+1)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j+1)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j+1)+i])));
			fruit.pixels[larg*(j+1)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j+1)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j+1)+i])));


	  		hits[larg*(j-1)+i]++; 
			fruit.pixels[larg*(j-1)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j-1)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j-1)+i])));
			fruit.pixels[larg*(j-1)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j-1)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j-1)+i])));
			fruit.pixels[larg*(j-1)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j-1)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j-1)+i])));
}

if (radio == 3 ){
if(larg*j+i < larg*anch){


			hits[larg*j+i]++; 
			fruit.pixels[larg*j+i].red = round(r*(exp(-alpha*(double)hits[larg*j+i]))+r2*(1-exp(-alpha*(double)hits[larg*j+i])));
			fruit.pixels[larg*j+i].green = round(g*(exp(-alpha*(double)hits[larg*j+i]))+g2*(1-exp(-alpha*(double)hits[larg*j+i])));
			fruit.pixels[larg*j+i].blue = round(b*(exp(-alpha*(double)hits[larg*j+i]))+b2*(1-exp(-alpha*(double)hits[larg*j+i])));
}
		
if(larg*j+i+1 < larg*anch){
			hits[larg*j+i+1]++; 
			fruit.pixels[larg*j+i+1].red = round(r*(exp(-alpha*(double)hits[larg*j+i+1]))+r2*(1-exp(-alpha*(double)hits[larg*j+i+1])));
			fruit.pixels[larg*j+i+1].green = round(g*(exp(-alpha*(double)hits[larg*j+i+1]))+g2*(1-exp(-alpha*(double)hits[larg*j+i+1])));
			fruit.pixels[larg*j+i+1].blue = round(b*(exp(-alpha*(double)hits[larg*j+i+1]))+b2*(1-exp(-alpha*(double)hits[larg*j+i+1])));
}  
if(larg*j+i-1 < larg*anch){
	  		hits[larg*j+i-1]++; 
			fruit.pixels[larg*j+i-1].red = round(r*(exp(-alpha*(double)hits[larg*j+i-1]))+r2*(1-exp(-alpha*(double)hits[larg*j+i-1])));
			fruit.pixels[larg*j+i-1].green = round(g*(exp(-alpha*(double)hits[larg*j+i-1]))+g2*(1-exp(-alpha*(double)hits[larg*j+i-1])));
			fruit.pixels[larg*j+i-1].blue = round(b*(exp(-alpha*(double)hits[larg*j+i-1]))+b2*(1-exp(-alpha*(double)hits[larg*j+i-1])));
} 
if(larg*(j+1)+i < larg*anch){

	  		hits[larg*(j+1)+i]++; 
			fruit.pixels[larg*(j+1)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j+1)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j+1)+i])));
			fruit.pixels[larg*(j+1)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j+1)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j+1)+i])));
			fruit.pixels[larg*(j+1)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j+1)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j+1)+i])));
}
if(larg*(j-1)+i < larg*anch){
	  		hits[larg*(j-1)+i]++; 
			fruit.pixels[larg*(j-1)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j-1)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j-1)+i])));
			fruit.pixels[larg*(j-1)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j-1)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j-1)+i])));
			fruit.pixels[larg*(j-1)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j-1)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j-1)+i])));
}
if(larg*(j+2)+i < larg*anch){



	  		hits[larg*(j+2)+i]++; 
			fruit.pixels[larg*(j+2)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j+2)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j+2)+i])));
			fruit.pixels[larg*(j+2)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j+2)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j+2)+i])));
			fruit.pixels[larg*(j+2)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j+2)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j+2)+i])));
}
if(larg*(j-2)+i < larg*anch && larg*(j-2)+i>=0){
	  		hits[larg*(j-2)+i]++; 
			fruit.pixels[larg*(j-2)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j-2)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j-2)+i])));
			fruit.pixels[larg*(j-2)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j-2)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j-2)+i])));
			fruit.pixels[larg*(j-2)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j-2)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j-2)+i])));
}
if(larg*j+i+2 < larg*anch && larg*j+i+2 >=0){

			hits[larg*j+i+2]++; 
			fruit.pixels[larg*j+i+2].red = round(r*(exp(-alpha*(double)hits[larg*j+i+2]))+r2*(1-exp(-alpha*(double)hits[larg*j+i+2])));
			fruit.pixels[larg*j+i+2].green = round(g*(exp(-alpha*(double)hits[larg*j+i+2]))+g2*(1-exp(-alpha*(double)hits[larg*j+i+2])));
			fruit.pixels[larg*j+i+2].blue = round(b*(exp(-alpha*(double)hits[larg*j+i+2]))+b2*(1-exp(-alpha*(double)hits[larg*j+i+2])));
} 
if(larg*j+i-2 < larg*anch && larg*j+i-2 >=0){
	  		hits[larg*j+i-2]++; 
			fruit.pixels[larg*j+i-2].red = round(r*(exp(-alpha*(double)hits[larg*j+i-2]))+r2*(1-exp(-alpha*(double)hits[larg*j+i-2])));
			fruit.pixels[larg*j+i-2].green = round(g*(exp(-alpha*(double)hits[larg*j+i-2]))+g2*(1-exp(-alpha*(double)hits[larg*j+i-2])));
			fruit.pixels[larg*j+i-2].blue = round(b*(exp(-alpha*(double)hits[larg*j+i-2]))+b2*(1-exp(-alpha*(double)hits[larg*j+i-2])));
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






if(larg*(j+1)+i+1 < larg*anch && larg*(j+1)+i+1 >=0){
	  		hits[larg*(j+1)+i+1]++; 
			fruit.pixels[larg*(j+1)+i+1].red = round(r*(exp(-alpha*(double)hits[larg*(j+1)+i+1]))+r2*(1-exp(-alpha*(double)hits[larg*(j+1)+i+1])));
			fruit.pixels[larg*(j+1)+i+1].green = round(g*(exp(-alpha*(double)hits[larg*(j+1)+i+1]))+g2*(1-exp(-alpha*(double)hits[larg*(j+1)+i+1])));
			fruit.pixels[larg*(j+1)+i+1].blue = round(b*(exp(-alpha*(double)hits[larg*(j+1)+i+1]))+b2*(1-exp(-alpha*(double)hits[larg*(j+1)+i+1])));
			
			}

if(larg*(j+1)+i-1 < larg*anch && larg*(j+1)+i-1 >=0){
	  		hits[larg*(j+1)+i-1]++; 
			fruit.pixels[larg*(j+1)+i-1].red = round(r*(exp(-alpha*(double)hits[larg*(j+1)+i-1]))+r2*(1-exp(-alpha*(double)hits[larg*(j+1)+i-1])));
			fruit.pixels[larg*(j+1)+i-1].green = round(g*(exp(-alpha*(double)hits[larg*(j+1)+i-1]))+g2*(1-exp(-alpha*(double)hits[larg*(j+1)+i-1])));
			fruit.pixels[larg*(j+1)+i-1].blue = round(b*(exp(-alpha*(double)hits[larg*(j+1)+i-1]))+b2*(1-exp(-alpha*(double)hits[larg*(j+1)+i-1])));
			}


if(larg*(j-1)+i+1 < larg*anch && larg*(j-1)+i+1 >=0){
	  		hits[larg*(j-1)+i+1]++; 
			fruit.pixels[larg*(j-1)+i+1].red = round(r*(exp(-alpha*(double)hits[larg*(j-1)+i+1]))+r2*(1-exp(-alpha*(double)hits[larg*(j-1)+i+1])));
			fruit.pixels[larg*(j-1)+i+1].green = round(g*(exp(-alpha*(double)hits[larg*(j-1)+i+1]))+g2*(1-exp(-alpha*(double)hits[larg*(j-1)+i+1])));
			fruit.pixels[larg*(j-1)+i+1].blue = round(b*(exp(-alpha*(double)hits[larg*(j-1)+i+1]))+b2*(1-exp(-alpha*(double)hits[larg*(j-1)+i+1])));
			}

if(larg*(j-1)+i-1 < larg*anch && larg*(j-1)+i-1 >=0){
	  		hits[larg*(j-1)+i-1]++; 
			fruit.pixels[larg*(j-1)+i-1].red = round(r*(exp(-alpha*(double)hits[larg*(j-1)+i-1]))+r2*(1-exp(-alpha*(double)hits[larg*(j-1)+i-1])));
			fruit.pixels[larg*(j-1)+i-1].green = round(g*(exp(-alpha*(double)hits[larg*(j-1)+i-1]))+g2*(1-exp(-alpha*(double)hits[larg*(j-1)+i-1])));
			fruit.pixels[larg*(j-1)+i-1].blue = round(b*(exp(-alpha*(double)hits[larg*(j-1)+i-1]))+b2*(1-exp(-alpha*(double)hits[larg*(j-1)+i-1])));
			}

			
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*  

if(larg*j+i+3 < larg*anch && larg*j+i+3 >=0){

			hits[larg*j+i+3]++; 
			fruit.pixels[larg*j+i+3].red = round(r*(exp(-alpha*(double)hits[larg*j+i+3]))+r2*(1-exp(-alpha*(double)hits[larg*j+i+3])));
			fruit.pixels[larg*j+i+3].green = round(g*(exp(-alpha*(double)hits[larg*j+i+3]))+g2*(1-exp(-alpha*(double)hits[larg*j+i+3])));
			fruit.pixels[larg*j+i+3].blue = round(b*(exp(-alpha*(double)hits[larg*j+i+3]))+b2*(1-exp(-alpha*(double)hits[larg*j+i+3])));
} 

if(larg*j+i-3 < larg*anch && larg*j+i-3 >=0){

			hits[larg*j+i-3]++; 
			fruit.pixels[larg*j+i-3].red = round(r*(exp(-alpha*(double)hits[larg*j+i-3]))+r2*(1-exp(-alpha*(double)hits[larg*j+i-3])));
			fruit.pixels[larg*j+i-3].green = round(g*(exp(-alpha*(double)hits[larg*j+i-3]))+g2*(1-exp(-alpha*(double)hits[larg*j+i-3])));
			fruit.pixels[larg*j+i-3].blue = round(b*(exp(-alpha*(double)hits[larg*j+i-3]))+b2*(1-exp(-alpha*(double)hits[larg*j+i-3])));
} 


if(larg*(j+3)+i < larg*anch && larg*(j+3)+i>=0){
	  		hits[larg*(j+3)+i]++; 
			fruit.pixels[larg*(j+3)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j+3)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j+3)+i])));
			fruit.pixels[larg*(j+3)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j+3)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j+3)+i])));
			fruit.pixels[larg*(j+3)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j+3)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j+3)+i])));
}


if(larg*(j-3)+i < larg*anch && larg*(j-3)+i>=0){
	  		hits[larg*(j-3)+i]++; 
			fruit.pixels[larg*(j-3)+i].red = round(r*(exp(-alpha*(double)hits[larg*(j-3)+i]))+r2*(1-exp(-alpha*(double)hits[larg*(j-3)+i])));
			fruit.pixels[larg*(j-3)+i].green = round(g*(exp(-alpha*(double)hits[larg*(j-3)+i]))+g2*(1-exp(-alpha*(double)hits[larg*(j-3)+i])));
			fruit.pixels[larg*(j-3)+i].blue = round(b*(exp(-alpha*(double)hits[larg*(j-3)+i]))+b2*(1-exp(-alpha*(double)hits[larg*(j-3)+i])));
}*/


if(larg*(j+2)+i+1 < larg*anch && larg*(j+2)+i+1>=0){
	  		hits[larg*(j+2)+i+1]++; 
			fruit.pixels[larg*(j+2)+i+1].red = round(r*(exp(-alpha*(double)hits[larg*(j+2)+i+1]))+r2*(1-exp(-alpha*(double)hits[larg*(j+2)+i+1])));
			fruit.pixels[larg*(j+2)+i+1].green = round(g*(exp(-alpha*(double)hits[larg*(j+2)+i+1]))+g2*(1-exp(-alpha*(double)hits[larg*(j+2)+i+1])));
			fruit.pixels[larg*(j+2)+i+1].blue = round(b*(exp(-alpha*(double)hits[larg*(j+2)+i+1]))+b2*(1-exp(-alpha*(double)hits[larg*(j+2)+i+1])));
}


if(larg*(j-2)+i+1 < larg*anch && larg*(j-2)+i+1>=0){
	  		hits[larg*(j-2)+i+1]++; 
			fruit.pixels[larg*(j-2)+i+1].red = round(r*(exp(-alpha*(double)hits[larg*(j-2)+i+1]))+r2*(1-exp(-alpha*(double)hits[larg*(j-2)+i+1])));
			fruit.pixels[larg*(j-2)+i+1].green = round(g*(exp(-alpha*(double)hits[larg*(j-2)+i+1]))+g2*(1-exp(-alpha*(double)hits[larg*(j-2)+i+1])));
			fruit.pixels[larg*(j-2)+i+1].blue = round(b*(exp(-alpha*(double)hits[larg*(j-2)+i+1]))+b2*(1-exp(-alpha*(double)hits[larg*(j-2)+i+1])));
}


if(larg*(j-2)+i-1 < larg*anch && larg*(j-2)+i-1>=0){
	  		hits[larg*(j-2)+i-1]++; 
			fruit.pixels[larg*(j-2)+i-1].red = round(r*(exp(-alpha*(double)hits[larg*(j-2)+i-1]))+r2*(1-exp(-alpha*(double)hits[larg*(j-2)+i-1])));
			fruit.pixels[larg*(j-2)+i-1].green = round(g*(exp(-alpha*(double)hits[larg*(j-2)+i-1]))+g2*(1-exp(-alpha*(double)hits[larg*(j-2)+i-1])));
			fruit.pixels[larg*(j-2)+i-1].blue = round(b*(exp(-alpha*(double)hits[larg*(j-2)+i-1]))+b2*(1-exp(-alpha*(double)hits[larg*(j-2)+i-1])));
}


if(larg*(j+2)+i-1 < larg*anch && larg*(j+2)+i-1>=0){
	  		hits[larg*(j+2)+i-1]++; 
			fruit.pixels[larg*(j+2)+i-1].red = round(r*(exp(-alpha*(double)hits[larg*(j+2)+i-1]))+r2*(1-exp(-alpha*(double)hits[larg*(j+2)+i-1])));
			fruit.pixels[larg*(j+2)+i-1].green = round(g*(exp(-alpha*(double)hits[larg*(j+2)+i-1]))+g2*(1-exp(-alpha*(double)hits[larg*(j+2)+i-1])));
			fruit.pixels[larg*(j+2)+i-1].blue = round(b*(exp(-alpha*(double)hits[larg*(j+2)+i-1]))+b2*(1-exp(-alpha*(double)hits[larg*(j+2)+i-1])));
}


if(larg*(j+1)+i+2 < larg*anch && larg*(j+1)+i+2>=0){
	  		hits[larg*(j+1)+i+2]++; 
			fruit.pixels[larg*(j+1)+i+2].red = round(r*(exp(-alpha*(double)hits[larg*(j+1)+i+2]))+r2*(1-exp(-alpha*(double)hits[larg*(j+1)+i+2])));
			fruit.pixels[larg*(j+1)+i+2].green = round(g*(exp(-alpha*(double)hits[larg*(j+1)+i+2]))+g2*(1-exp(-alpha*(double)hits[larg*(j+1)+i+2])));
			fruit.pixels[larg*(j+1)+i+2].blue = round(b*(exp(-alpha*(double)hits[larg*(j+1)+i+2]))+b2*(1-exp(-alpha*(double)hits[larg*(j+1)+i+2])));
}


if(larg*(j+1)+i-2 < larg*anch && larg*(j+1)+i-2>=0){
	  		hits[larg*(j+1)+i-2]++; 
			fruit.pixels[larg*(j+1)+i-2].red = round(r*(exp(-alpha*(double)hits[larg*(j+1)+i-2]))+r2*(1-exp(-alpha*(double)hits[larg*(j+1)+i-2])));
			fruit.pixels[larg*(j+1)+i-2].green = round(g*(exp(-alpha*(double)hits[larg*(j+1)+i-2]))+g2*(1-exp(-alpha*(double)hits[larg*(j+1)+i-2])));
			fruit.pixels[larg*(j+1)+i-2].blue = round(b*(exp(-alpha*(double)hits[larg*(j+1)+i-2]))+b2*(1-exp(-alpha*(double)hits[larg*(j+1)+i-2])));
}


if(larg*(j-1)+i-2 < larg*anch && larg*(j-1)+i-2>=0){
	  		hits[larg*(j-1)+i-2]++; 
			fruit.pixels[larg*(j-1)+i-2].red = round(r*(exp(-alpha*(double)hits[larg*(j-1)+i-2]))+r2*(1-exp(-alpha*(double)hits[larg*(j-1)+i-2])));
			fruit.pixels[larg*(j-1)+i-2].green = round(g*(exp(-alpha*(double)hits[larg*(j-1)+i-2]))+g2*(1-exp(-alpha*(double)hits[larg*(j-1)+i-2])));
			fruit.pixels[larg*(j-1)+i-2].blue = round(b*(exp(-alpha*(double)hits[larg*(j-1)+i-2]))+b2*(1-exp(-alpha*(double)hits[larg*(j-1)+i-2])));
}


if(larg*(j-1)+i+2 < larg*anch && larg*(j-1)+i+2>=0){
	  		hits[larg*(j-1)+i+2]++; 
			fruit.pixels[larg*(j-1)+i+2].red = round(r*(exp(-alpha*(double)hits[larg*(j-1)+i+2]))+r2*(1-exp(-alpha*(double)hits[larg*(j-1)+i+2])));
			fruit.pixels[larg*(j-1)+i+2].green = round(g*(exp(-alpha*(double)hits[larg*(j-1)+i+2]))+g2*(1-exp(-alpha*(double)hits[larg*(j-1)+i+2])));
			fruit.pixels[larg*(j-1)+i+2].blue = round(b*(exp(-alpha*(double)hits[larg*(j-1)+i+2]))+b2*(1-exp(-alpha*(double)hits[larg*(j-1)+i+2])));
}


}







/*  
			for (k =1; k<radio; k++){  
			for(k2 = 0; k2<=k; k2++){

			if(k!=radio-1 || k2!=radio-1){

			if(larg*(j+k)+(i+k2) < larg*anch){

			hits[larg*(j+k)+(i+k2)]++;			
			fruit.pixels[larg*(j+k)+(i+k2)].red = round(r*(exp(-alpha*(double)hits[larg*(j+k)+(i+k2)]))+r2*(1-exp(-alpha*(double)hits[larg*(j+k)+(i+k2)])));
			fruit.pixels[larg*(j+k)+(i+k2)].green = round(g*(exp(-alpha*(double)hits[larg*(j+k)+(i+k2)]))+g2*(1-exp(-alpha*(double)hits[larg*(j+k)+(i+k2)])));
			fruit.pixels[larg*(j+k)+(i+k2)].blue = round(b*(exp(-alpha*(double)hits[larg*(j+k)+(i+k2)]))+b2*(1-exp(-alpha*(double)hits[larg*(j+k)+(i+k2)])));
			}

    	  		if(larg*(j+k2)+(i+k) < larg*anch){
			hits[larg*(j+k2)+(i+k)]++;			
			fruit.pixels[larg*(j+k2)+(i+k)].red = round(r*(exp(-alpha*(double)hits[larg*(j+k2)+(i+k)]))+r2*(1-exp(-alpha*(double)hits[larg*(j+k2)+(i+k)])));
			fruit.pixels[larg*(j+k2)+(i+k)].green = round(g*(exp(-alpha*(double)hits[larg*(j+k2)+(i+k)]))+g2*(1-exp(-alpha*(double)hits[larg*(j+k2)+(i+k)])));
			fruit.pixels[larg*(j+k2)+(i+k)].blue = round(b*(exp(-alpha*(double)hits[larg*(j+k2)+(i+k)]))+b2*(1-exp(-alpha*(double)hits[larg*(j+k2)+(i+k)])));
			} 

  			if((larg*(j-k)+(i-k2) < larg*anch)&&(larg*(j-k)+(i-k2) >= 0)){
			hits[larg*(j-k)+(i-k2)]++;			
			fruit.pixels[larg*(j-k)+(i-k2)].red = round(r*(exp(-alpha*(double)hits[larg*(j-k)+(i-k2)]))+r2*(1-exp(-alpha*(double)hits[larg*(j-k)+(i-k2)])));
			fruit.pixels[larg*(j-k)+(i-k2)].green = round(g*(exp(-alpha*(double)hits[larg*(j-k)+(i-k2)]))+g2*(1-exp(-alpha*(double)hits[larg*(j-k)+(i-k2)])));
			fruit.pixels[larg*(j-k)+(i-k2)].blue = round(b*(exp(-alpha*(double)hits[larg*(j-k)+i-k2]))+b2*(1-exp(-alpha*(double)hits[larg*(j-k)+(i-k2)])));
			}
  
	  		if((larg*(j-k2)+(i-k) < larg*anch)&&(larg*(j-k2)+(i-k) >= 0)){
			hits[larg*(j-k2)+(i-k)]++;			
			fruit.pixels[larg*(j-k2)+(i-k)].red = round(r*(exp(-alpha*(double)hits[larg*(j-k2)+(i-k)]))+r2*(1-exp(-alpha*(double)hits[larg*(j-k2)+(i-k)])));
			fruit.pixels[larg*(j-k2)+(i-k)].green = round(g*(exp(-alpha*(double)hits[larg*(j-k2)+(i-k)]))+g2*(1-exp(-alpha*(double)hits[larg*(j-k2)+(i-k)])));
			fruit.pixels[larg*(j-k2)+(i-k)].blue = round(b*(exp(-alpha*(double)hits[larg*(j-k2)+(i-k)]))+b2*(1-exp(-alpha*(double)hits[larg*(j-k2)+(i-k)])));
			}  

 	  		if((larg*(j+k)+(i-k2) < larg*anch)&&(larg*(j+k)+(i-k2) >= 0)){
			hits[larg*(j+k)+(i-k2)]++;
			fruit.pixels[larg*(j+k)+(i-k2)].red = round(r*(exp(-alpha*(double)hits[larg*(j+k)+(i-k2)]))+r2*(1-exp(-alpha*(double)hits[larg*(j+k)+(i-k2)])));
			fruit.pixels[larg*(j+k)+(i-k2)].green = round(g*(exp(-alpha*(double)hits[larg*(j+k)+(i-k2)]))+g2*(1-exp(-alpha*(double)hits[larg*(j+k)+(i-k2)])));
			fruit.pixels[larg*(j+k)+(i-k2)].blue = round(b*(exp(-alpha*(double)hits[larg*(j+k)+(i-k2)]))+b2*(1-exp(-alpha*(double)hits[larg*(j+k)+(i-k2)])));
			} 
  
			if((larg*(j-k2)+(i+k) < larg*anch)&&(larg*(j-k2)+(i+k) >= 0)){
			hits[larg*(j-k2)+(i+k)]++;			
			fruit.pixels[larg*(j-k2)+(i+k)].red = round(r*(exp(-alpha*(double)hits[larg*(j-k2)+(i+k)]))+r2*(1-exp(-alpha*(double)hits[larg*(j-k2)+(i+k)])));
			fruit.pixels[larg*(j-k2)+(i+k)].green = round(g*(exp(-alpha*(double)hits[larg*(j-k2)+(i+k)]))+g2*(1-exp(-alpha*(double)hits[larg*(j-k2)+(i+k)])));
			fruit.pixels[larg*(j-k2)+(i+k)].blue = round(b*(exp(-alpha*(double)hits[larg*(j-k2)+(i+k)]))+b2*(1-exp(-alpha*(double)hits[larg*(j-k2)+(i+k)])));
			}
  
	  		if((larg*(j-k)+(i+k2) < larg*anch)&&(larg*(j-k)+(i+k2) >= 0)){
			hits[larg*(j-k)+(i+k2)]++;			
			fruit.pixels[larg*(j-k)+(i+k2)].red = round(r*(exp(-alpha*(double)hits[larg*(j-k)+(i+k2)]))+r2*(1-exp(-alpha*(double)hits[larg*(j-k)+(i+k2)])));
			fruit.pixels[larg*(j-k)+(i+k2)].green = round(g*(exp(-alpha*(double)hits[larg*(j-k)+(i+k2)]))+g2*(1-exp(-alpha*(double)hits[larg*(j-k)+(i+k2)])));
			fruit.pixels[larg*(j-k)+(i+k2)].blue = round(b*(exp(-alpha*(double)hits[larg*(j-k)+(i+k2)]))+b2*(1-exp(-alpha*(double)hits[larg*(j-k)+(i+k2)])));
			} 

  			if((larg*(j+k2)+(i-k) < larg*anch)&&(larg*(j+k2)+(i-k) >= 0)){
			hits[larg*(j+k2)+(i-k)]++;			
			fruit.pixels[larg*(j+k2)+(i-k)].red = round(r*(exp(-alpha*(double)hits[larg*(j+k2)+(i-k)]))+r2*(1-exp(-alpha*(double)hits[larg*(j+k2)+(i-k)])));
			fruit.pixels[larg*(j+k2)+(i-k)].green = round(g*(exp(-alpha*(double)hits[larg*(j+k2)+(i-k)]))+g2*(1-exp(-alpha*(double)hits[larg*(j+k2)+(i-k)])));
			fruit.pixels[larg*(j+k2)+(i-k)].blue = round(b*(exp(-alpha*(double)hits[larg*(j+k2)+(i-k)]))+b2*(1-exp(-alpha*(double)hits[larg*(j+k2)+(i-k)])));
			}  
			}
		}
		}	*/	
		  break;
		  }  /* Termina switch */
	      }  /* Termina for (p = PQntuples (res) - 1; p >= 0; p--) */

	  tra = atoi (trans);

	  save_png_to_file (&fruit, tra);
	  free (fruit.pixels);
	  /* Termina la función dibuja */
	  PQclear (res);
	}
    }
  PQfinish (conn);
  /* Termina Postgres */
  return 0;
}				/* ----------  end of function main  ---------- */
static pixel_t *
pixel_at (bitmap_t * bitmap, int x, int y)
{
  return bitmap->pixels + bitmap->height * y + x;
}

static int
save_png_to_file (bitmap_t * bitmap, int tr)
{
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  size_t x, y;
  png_byte **row_pointers = NULL;
  /* "status" contiene el valor de retorno de esta función. La primera esta configurada al valor cual significa 'failure'. Cuando el routine hs rk, it is set to a value which means 'success'. */
  int status = -1;
  int depth = 8;

  png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL)
    {
      goto png_create_write_struct_failed;
    }

  info_ptr = png_create_info_struct (png_ptr);
  if (info_ptr == NULL)
    {
      goto png_create_info_struct_failed;
    }

  if (setjmp (png_jmpbuf (png_ptr)))
    {
      goto png_failure;
    }
  /* Configura los atributos de la imagen */

  png_set_IHDR (png_ptr, info_ptr, bitmap->width, bitmap->height, depth, PNG_COLOR_TYPE_RGBA,	/* http://stackoverflow.com/questions/13911126/how-to-let-png-have-the-transparent-property */
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  /* Aquí se inicailizan las filas en el PNG. */

  row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
  for (y = 0; y < bitmap->height; ++y)
    {
      png_byte *row =
	png_malloc (png_ptr, sizeof (uint8_t) * bitmap->width * 4);
      row_pointers[y] = row;

      for (x = 0; x < bitmap->width; ++x)
	{
	  int hdata = 0;
	  pixel_t *pixel = pixel_at (bitmap, x, y);
	  *row++ = pixel->red;
	  hdata += pixel->red;
	  *row++ = pixel->green;
	  hdata += pixel->green;
	  *row++ = pixel->blue;
	  hdata += pixel->blue;
	  if (hdata == 0)
	    {
	      *row++ = tr;	/* Para controlar la transparencia. Va de 0 a 255 */
	    }
	  else
	    {
	      *row++ = 255;	/* Con esto puede controlarse la transparencia de las bolitas. Pero aun no he puesto la variable, tal vez no se necesita.  */
	    }
	}
    }

  png_set_rows (png_ptr, info_ptr, row_pointers);

   /*  static */
  struct mem_encode state;	/* Funcion de bash */
  /*  initialise - put this before png_write_png() call */
  state.buffer = NULL;		/* funciónde bash */
  state.size = 0;		/* Funciónde bash */

  png_set_write_fn (png_ptr, &state, my_png_write_data, my_png_flush);	/* Función de bash */
  png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

  /* Con lo anterior se ha escrito exitosamente la routina. "status" indica el exito success. */
/*  cleanup */

  status = 0;

/* Para salvar en bash */

  if (state.buffer)		/* Función de bash */
    fwrite (state.buffer, state.size, 1, stdout);
  free (state.buffer);		/* Función de bash */

/* Fin Para salvar en bash*/

  for (y = 0; y < bitmap->height; y++)
    {
      png_free (png_ptr, row_pointers[y]);
    }
  png_free (png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
  png_destroy_write_struct (&png_ptr, &info_ptr);
png_create_write_struct_failed:

  return status;
}

/* Dado el "value" y "max", el máximo valo el cual esperamos "value" para tomar, este retorna un entero entre 0 y 255. */


void
usage (void)
{
  printf ("Usage:\n");
  printf (" -q\"<query>\"\n");
  printf (" -x<x_min>\n");
  printf (" -X<X_max>\n");
  printf (" -y<y_min>\n");
  printf (" -Y<Y_max>\n");
  printf (" -W<Width>\n");
  printf (" -H<Height>\n");
  printf (" -P<Port>\n");
  printf (" -i\"<ip>\"\n");
  printf (" -u\"<usser>\"\n");
  printf (" -p\"<password>\"\n");
  printf (" -d\"<database>\"\n");
  printf (" -r<color red>\n");
  printf (" -g<color green>\n");
  printf (" -b<color blue>\n");
  printf (" -t<transparency>\n");
  printf (" -R<radio>\n");
  printf (" -h<help>\n");
  exit (8);
}

/* Para salvar en bash */
void
my_png_write_data (png_structp png_ptr, png_bytep data, png_size_t length)
{
  /*  with libpng15 next line causes pointer deference error; use libpng12 */
  struct mem_encode *p = (struct mem_encode *) png_get_io_ptr (png_ptr);	/*  was png_ptr->io_ptr */
  size_t nsize = p->size + length;

  /*  allocate or grow buffer */
  if (p->buffer)
    p->buffer = realloc (p->buffer, nsize);
  else
    p->buffer = malloc (nsize);

  if (!p->buffer)
    png_error (png_ptr, "Write Error");

  /*  copy new bytes to end of buffer */
  memcpy (p->buffer + p->size, data, length);
  p->size += length;
}

void
my_png_flush (png_structp png_ptr)
{
}

char * str_replace ( char *string, char *substr, char *replacement ){
  char *tok = NULL;
  char *newstr = NULL;
  char *oldstr = NULL;
  char *head = NULL;
 
  /* if either substr or replacement is NULL, duplicate string a let caller handle it */
  if ( substr == NULL || replacement == NULL ) return strdup (string);
  newstr = strdup (string);
  head = newstr;
  while ( (tok = strstr ( head, substr ))){
    oldstr = newstr;
    newstr = malloc ( strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) + 1 );
    /*failed to alloc mem, free old string and return NULL */
    if ( newstr == NULL ){
      free (oldstr);
      return NULL;
    }
    memcpy ( newstr, oldstr, tok - oldstr );
    memcpy ( newstr + (tok - oldstr), replacement, strlen ( replacement ) );
    memcpy ( newstr + (tok - oldstr) + strlen( replacement ), tok + strlen ( substr ), strlen ( oldstr ) - strlen ( substr ) - ( tok - oldstr ) );
    memset ( newstr + strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) , 0, 1 );
    /* move back head right after the last replacement */
    head = newstr + (tok - oldstr) + strlen( replacement );
    free (oldstr);
  }
  return newstr;
}
