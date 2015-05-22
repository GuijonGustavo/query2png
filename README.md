# query2png
Este es un desarrollo de software en el lenguaje de programación C que, a partir de un query a una base de datos en postgreSQL, genera una colección de puntos con efecto de saturación en el color.


Ejemplos:


ttie@gmagallanes:~/Dropbox/Desarrollo/C/ccconabio/query2png$ ./query2png -q"SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);" -r135 -g206 -b250 -R3 -t0 -s1 -l25 -m25 -n112 -W500
 -H500 -x-11320018.139346 -y1203424.573154 -X-9969834.471904 -Y2553608.24059 -i"db0.conabio.gob.mx" -P"54
35" -d"snib" -u"postgres" -p"conabio2008" > lightskyblue2midnightblue.png


