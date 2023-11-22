La carpeta fluid tiene la función main del programa
La carpeta ftest tiene el script de los tests funionales
La carpeta sim tiene el código fuente de la biblioteca de la simulación
La carpeta utest tiene las pruebas unitarias, con un archivo .cpp por módulo de la carpeta sim a probar. Algunos de los tests tienen también un .hpp asociado con la creación/destrucción del entorno de pruebas necesario para sus respectivas pruebas

El ejecutable de las pruebas unitarias es ./utest/utest, y el script de las pruebas funcionales es ./ftest/ftest.sh. Este último necesita como primer parámetro la ruta al ejecutable compilado de ./fluid/fluid.cpp
