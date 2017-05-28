# astana_interactivo


para poder compilar estos proyectos que utilizan openFrameworks es necesario utilizar la ultima version de OF en github.

Si es que no la tienen descargada deben ejecutar la siguiente linea en la Terminal.

`git clone --depth 1 https://github.com/openframeworks/openframeworks`

Este repositorio debe estar ubicado dentro de la carpeta `openFrameworks/apps/`
es decir la ruta se vería así `openFrameworks/apps/astana_interactivo`.

## acceso a blobs
La api de AstanaKinectBlobFinder ha cambiado.
Al hacer que ésta corra en otro thread la coleccion de blobs esta buffereada.
De modo que a éstos se debe acceder mediante un metodo y no accediendo directamente a la variable.
Suponiendo que nuestra instacia de `AstanaKinectBlobFinder` se llama `blobFinder`
si antes se accedia a los blobs asi

    blobFinder.allBlobs
ahora se debe acceder así

    blobFinder.getAllBlobs()
   
Los metodos para acceder a los distintos blobs son los siguientes y su respectiva descripcion .

### `vector< shared_ptr<AstanaBlob> >& getAllBlobs();`
Entrega todos los blobs activos del cuadro. No estan incluidos los blobs recientemente matados y los fantasmas.

### `vector< shared_ptr<AstanaBlob> >& getNewBlobs();`
Entega solamente los blobs nuevos del cuadro.

### `vector< shared_ptr<AstanaBlob> >& getMovedBlobs();`
Entega solamente los blobs que se han movido.
El parametro "Blob Min Vel" se utiliza para determinar el desplazamiento minimo para ser incluido en este grupo.

### `vector< shared_ptr<AstanaBlob> >& getScaledBlobs();`
Entega solamente los blobs que han cambiado de tamaño.
El parametro "Blobs Min Area Diff" se utiliza para determinar el cambio de tamaño (area) minimo para ser incluido en este grupo.

### `vector< shared_ptr<AstanaBlob> >& getMergedBlobs();`
Entrega solamente los blobs que se han fusionado. 
Al fusionarse dos blobs uno desaparece (pasa a estar en killedBlobs) mientras que el otro permanece vigente.
Este grupo solamente contiene el blob vigente, el cual guarda una referencia al blob con el que se fusionó.

    for(auto& blob : blobFinder.getMergedBlobs()){
        blob.mergedWith // puntero al blob con el que se fusiono, el que está muerto.
    }

### `vector< shared_ptr<AstanaBlob> >& getKilledBlobs();`
contiene solamente lso blobs que estaban presentes en el cuadro anterior y que ya no lo están en el actual.

### `vector< shared_ptr<AstanaBlob> >& getGhostBlobs();`
contiene los blobs fantasmas que se mantienen en caso de que este vuelva a aparecer y asi reutilizar su `label`
Estos blobs solo tienen su `boundingRect` y `label`.
No tienen `polyline`.

Estas listas son construidas con punteros inteligentes, asi es mas eficiente. Cada blob tiene una sola instancia y es ésta a la cual apuntan los elementos de las distintas listas.

## Eventos
los eventos que genera AstanaKinectBlobFinder son:
    
    ofEvent<void> newBlobEvent;
    ofEvent<void> killedBlobEvent;
  	ofEvent<void> onMoveBlobEvent;
	  ofEvent<void> onScaleBlobEvent;
	  ofEvent<void> onMergeBlobEvent;

