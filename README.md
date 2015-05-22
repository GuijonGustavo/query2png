# query2png
Este es un desarrollo de software en el lenguaje de programación C que, a partir de un query a una base de datos en postgreSQL, genera una colección de puntos con efecto de saturación en el color.
# Instalación
1. Baja la carpeta query2png-master.zip de este repositorio.
2. Es recomendable compilar este desarrollo en el ambiente nativo del sistema operativo en el que se va a trabajar. Por lo que es necesario:
Instalar las siguientes bibliotecas:
libpq-dev
libpng

En Debian y Ubuntu:
aptitude update
aptitude install libpng libpq-dev

#Compilación

Una vez que tengas las librerías necesarias hay que compilar.

En Debian:

gcc -lpng -lpq -lm -o query2png query2png.c -I/usr/include/postgresql/

En Ubuntu:

gcc -o query2png query2png.c -lpng -lpq -lm -I/usr/include/postgresql/

#Run
Ya compilado este software deberás ejecutarlo rellenando los parámetros requeridos:

-q"\<query\>"

-x\<x_min\>

-X\<X_max\>

-y\<y_min\>

-Y\<Y_max\>

-W\<Width\>

-H\<Height\>

-P\<Port\>

-i"\<ip\>"

-u"\<usser\>"

-p"\<password\>"

-d"\<database\>"

-r\<color red\>

-g\<color green\>

-b\<color blue\>

-t\<transparency\>

-R\<radio\>

-h\<help\>

Ejemplos:

## aqua2teal.png

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r0 -g255 -b255 -l0 -m128 -R3 -t0 -s1 -n128 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > aqua2teal.png
(img/aqua2teal.png)

## crimson2maroon.png


$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r220 -g20 -b60 -l128 -m0 -n0 -R3 -t0 -s1 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > crimson2maroon.png
(img/crimson2maroon.png)


## gainsboro2darkslategrey.png

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r220 -g220 -b220 -R3 -t0 -s1 -l47 -m79 -n79 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > gainsboro2darkslategrey.png
(img/gainsboro2darkslategrey.png)


## gold2orangered.png

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r255 -g215 -b0 -R3 -t0 -s1 -l255 -m69 -n0 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > gold2orangered.png
(img/gold2orangered.png)


## hotpink2mediumvioletetred.png

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r255 -g105 -b180 -R3 -t0 -s1 -l199 -m21 -n133 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > hotpink2mediumvioletetred.png


## lightskyblue2midnightblue.png

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r135 -g206 -b250 -R3 -t0 -s1 -l25 -m25 -n112 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > lightskyblue2midnightblue.png
(img/lightskyblue2midnightblue.png)


## lime2forestgreen.png

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r0 -g255 -b0 -R3 -t0 -s1 -l34 -m139 -n34 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > lime2forestgreen.png
(img/lime2forestgreen.png)

## plum2indigo.png

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r221 -g160 -b221 -R3 -t0 -s1 -l75 -m0 -n135 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > plum2indigo.png
(img/plum2indigo.png)


## salmon2firebrick.png 

$./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r250 -g128 -b114 -R3 -t0 -s1 -l178 -m34 -n34 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"dbX.xxxxx.edu.mx" -P"xxxx" -d"xxxxx" -u"xxxxx" -p"xxxxx" > salmon2firebrick.png
(img/salmon2firebrick.png)
