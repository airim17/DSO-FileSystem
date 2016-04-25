# DSO-FileSystem

### Estructuras:

#### Superbloque:
    1. Mapa de bits, indicando qué bloques están ocupados. (1 long) (TRANSFORMACION DIV. ENTRE 2)
    2. Mapa de bits, indicando qué i-nodos están ocupados. (1 long) (TRANSFORMACION DIV. ENTRE 2)
    * SE NECESITAN LOS 2 BITMAPS? (Esperar correo Rafael Sotomayor)
    3. Mapa de etiquetas 30X (identificador de etiqueta (1B) + nombre de etiqueta (32B) + número de veces(1B) + lista de ficheros con esa etiqueta (1B) (TRANSFORMACION DIV. ENTRE 2)
    4. Numero de i-nodos (1B)
    5. Número de bloques de datos (1B)
    6. Número máximo de ficheros (1B)
    7. Tamaño de disco (short) (entre 320 - 500).
    8. PrimerBloqueDatos. (1B)
    9. Número mágico (1 int)
    10. Padding (hasta ocupar un bloque)
    
    
#### I-nodo:
    1. ID (1B).
    2. Nombre del fichero (64B).
    3. Tamaño (Short).
    4. Abierto/cerrado (1B).
    5. Posición en el fichero (Short) <-- Se resetea en el openFS / closeFS
    5. Bloque Directo (1B).
    6. Etiquetas (3B).

### Implementación:

#### MKFS(número ficheros, tamaño disco):
1. Comprobar que el tamaño de ficheros NO es superior a 50.
2. Comprobar que el tamaño del disco está entre 320 y 500 (ya se presupone KB).
3. Formatea el tamaño de disco indicado escribiendo todo 0s.
4. Escribe el primer bloque del disco con la información inicial proporcionada, el resto de datos: 0.

#### Mount( ):
1. Se lee el superbloque (primer bloque del disco).
2. Se guardar estos datos en el Superbloque (como variable global).


#### Unmount( ):
1. Escribe en el Superbloque (primer bloque del disco) lo que hay en la variable global.
2. ¿Escribe 0s en el Superbloque global?


#### CreatFS (filename):
1. Si el número de i-nodos > número máximo, indicado: Error.
2. Utilizando el número de inodos del superbloque, comprobar si ya hay uno con ese nombre.
3. Si no lo hay, se le asigna el siguiente i-nodo libre.
4. Actualizar el número de inodos del superbloque.
5. Guardar el nombre en el i-nodo.
6. Guardar todo 0s.
7. Se actualiza el bitmap de I-nodos.
8. SE LE ASOCIA EL SIGUIENTE BLOQUE LIBRE EN LA CREACIÓN?????? (SI NO FUES ASÍ, SI QUE SE NECESITAN 2 BITMAPS).


#### openFS (nombre de fichero):
1. Comprobar que hay un i-nodo con ese nombre, sino: Error.
2. Se cambia el campo del i-nodo y se pone como "Abierto" (1).
3. Devuelve el identificador del i-nodo <b>(como se usa en otros procesos, tiene que marcar el número de bloque)</b>


#### closeFS (descriptor del fihero):
1. Comprueba que el fichero está abierto, sino; Error.
2. Cambia el campo del i-nodo y lo pone como "Cerrado" (0).


#### readFS(descriptor, buffer, numBytes)
1. Se comprueba que el fichero está abierto y si está ocupado(?).
2. Si lo está, se leen tantos bytes como se indica, utilizando bread() y se guardan en el buffer que nos pasan.
3. Se lee la posición del puntero (i-nodo) y se pone a leer desde ahí.
4. Variable que guarde cuandos Bs se han leido, y se devuelve.


#### writeFS(descriptor, buffer, numBytes):
1. Se comprueba que el fichero está abierto (?).
2. Si lo está, se escriben tantos bytes como se indica, utilizando bwrite() cogiendolos del buffer que nos pasan.
3. Se lee la posición del puntero (i-nodo) y se pone a escribir desde ahí.
4. Si numBytes a escribir + posicion puntero > tamaño máximo datos: Error, y no se escribe ninguno.
5. Se actualiza el bitmap de bloques de datos usados. <b>REVISAR</b>
6. Variable que guarde cuandos Bs se han escrito, y se devuelve.

* Asignación de bloque a i-nodo según el bitmap de bloques (suponiendo relación 1:1).

#### lseek (descriptor, offset, whence):
1. Actualiza la posición del puntero de ese i-nodo a la posicion indicada.
2. Si es mayor que el tamaño actual del fichero o menor que 0 da error.
3. Tener en cuenta los tipos de desplazamiento. ("Whence" lo indica).

#### Tag (filename, tag):
1. Se comprueba que dicha etiqueta está en el mapa de etiquetas:<br>
    1.1 Si está, se añade 1 al contador, y se añade el ID del fichero.<br>
    1.2 Si no está y <b>hay hueco en los 30 espacios</b>, se crea, con el contador a 1.<br>
2. Se añade a dicho fichero, el ID de la tag asociada, <b>en caso de haber hueco</b><br>

#### Untag (filename, tag):<br>
1. Se comprueba que dicha etiqueta está en el mapa de etiquetas:<br>
    1.1 Si está, se resta 1 al contador, y se quita el ID del fichero.<br>
    1.2 Si no está: Error.<br>
2. Se quita de dicho fichero, la tag asociada.<br>
3. Si el contador de la tag llega a 0, se elimina.<br>

#### listFS (tag, lista):
1. Se comprueba que la etiqueta existe.<br>
2. Se recorre la lista accediendo a cada uno de los i-nodos.<br>
3. Se comprueba si dichos ficheros están cerrados.<br>
    3.1 Si lo están, se añaden a la lista.<br>
    3.2 Sino, no.<br>
