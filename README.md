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

Por ejemplo:

$ ./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r250 -g20 -b210 -R3 -t0 -s1 -l36 -m46 -n67 -W500 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"mi.own.ip" -P"123456" -d"database" -u"postgres" -p"xxxxxxxxx" > example_1.png


