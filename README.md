# BD2 - Proyecto 1 - Sequential File e  Indexed Sequential Access Method(ISAM)
## Integrantes
|  **#** | **Código** | **Apellidos, Nombre** |
| :---: | :---: | :---: |
|  1 | 201810010 | Barrios Silva, Alonso Winston |
|  2 | 201810010 | Lazo Pampa, David Alejandro |
## Objetivo
Conocer las dos diferentes estrucutras de archivos secuenciales, con la diferencia que uno usa indexacion.
Tener en cuenta las ventajas y desventajas de cada estructura.
## Sequential File
### Introducción
Es la forma más simple de almacenar y recuperar registros de un archivo.
En el archivo secuencial, se almacenan los registros uno tras otro.
El primer registro almacenado se coloca al principio del archivo.
El segundo se almacena inmediatamente después.
![](img/1.png)
### Ejemplo
- [x] Podemos tener un control de registro por orden de llegada de asistentes.
- [x] Gestionando base de datos.
### Ventajas y Desventajas
* Ventajas 
- [x] La ventaja más importante de la técnica de organización secuencial de archivos es la capacidad de acceso al siguiente registro rápidamente.
- [x] No se desperdicia espacio en el dispositivo de almacenamiento.
* Desventajas 
- [x] Hay que realizar consultas secuenciales para acceder a un registro.
- [x] Para insertar nuevos registros, estos tienen que ser al final de todos.
- [x] Para mantener ordenado y compactado el fichero, hay que crear un fichero nuevo a partir del existente.
- [x] Si el sistema quiere leer la posición 10 tendrá que recorrer desde el primer archivo.
### Procedimiento
#### 1. Inserción
Se puede realizar de dos maneras.
1. Crear un nuevo archivo, **de alto coste**.
2. Agregarlo al final, **de bajo coste**.
* Código
```
template <typename ObjType, typename MembType>
void SequentialFile<ObjType, MembType>::add(ObjType registro) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);

    if(file.is_open()) {
        bool find = false;

        file.seekg(0, ios::end);
        long unsigned int size = file.tellg() / (sizeof(ObjType) + 1);

        long unsigned int position = binarySearch<ObjType>(file, size, this->primary_key, registro.*primary_key, find);

        if(!find) {
            ObjType current;
            file.seekg(position * (sizeof(ObjType) + 1));
            file >> current;

            fstream aux_file;
            aux_file.open(this->aux_name, ios::in | ios::out | ios::binary);

            if(aux_file.is_open()) {
                long unsigned int aux_position = 0;
                Next next_current;

                if(current.next.file == 'a') {
                    aux_position = linearSearch(aux_file, this->primary_key, registro.*primary_key, current, find);

                    if(find) {
                        throw "Record already exists";
                    }

                    next_current = current.next; 

                    aux_file.seekg(0, ios::end);
                    current.next.position = (aux_file.tellg() / (sizeof(ObjType) + 1));
                    current.next.file = 'a';
                    
                    aux_file.seekg(aux_position * (sizeof(ObjType) + 1));
                    aux_file << current;
                } else {
                    next_current = current.next; 

                    aux_file.seekg(0, ios::end);
                    current.next.position = aux_file.tellg() / (sizeof(ObjType) + 1);
                    current.next.file = 'a';

                    file.seekg(position * (sizeof(ObjType) + 1));
                    file << current;
                }

                aux_file.seekg(0, ios::end);
                registro.next = next_current;
                aux_file << registro;

                long unsigned int aux_size = aux_file.tellg() / (sizeof(ObjType) + 1);

                aux_file.close();

                if(aux_size == 5) {
                    joinFiles();
                }
            } else {
                cerr << "Can't open file " << this->aux_name << endl;
            }
        } else {
            cerr << "Record already exists\n";
        }

        file.close();
    } else {
        cerr << "Can't open file " << this->name_file << endl;
    }
}
```
#### 2. Eliminación
- Eliminando el total del archivo, se logra dejar libre el espacio del soporte que ocupa.
- Cuando se borra un archivo, este ya no se puede utilizar y no se puede acceder a ningún registro.
* Código
```

```
#### 3. Búsqueda
* Código
```
template <typename ObjType, typename MembType>
ObjType SequentialFile<ObjType, MembType>::search(const string& key) {
    ObjType result;

    fstream file;
    file.open(this->name_file, ios::in | ios::binary);

    if(!file.is_open()) {
        cerr << "Can't open file " << this->name_file << endl;
        throw "Can't open file";
    }

    bool find = false;

    file.seekg(0, ios::end);
    long unsigned int size = file.tellg() / (sizeof(ObjType) + 1);

    long unsigned int position = binarySearch<ObjType>(file, size, this->primary_key, key, find);

    file.seekg(position * (sizeof(ObjType) + 1));
    file >> result;

    file.close();

    if(!find && result.next.file == 'd') {
        cerr << "Record " << key << " doesn't exists in " << this->name_file << endl;
        throw "Record doesn't exists";
    }

    if(find) {
        return result;
    } else {
        fstream aux_file;
        aux_file.open(this->aux_name, ios::in | ios::binary);

        if(!aux_file.is_open()) {
            cerr << "Can't open file " << this->aux_name << endl;
            throw "Can't open file";
        }

        linearSearch(aux_file, this->primary_key, key, result, find);

        aux_file.close();

        if(!find) {
            cerr << "Record " << key << " doesn't exists in " << this->aux_name << endl;
            throw "Record doesn't exists";
        }
        
        return result;
    } 
}
```

## Indexed Sequential Access Method(ISAM)
### Introducción 
A diferencia del Sequential File, el ISAM usa un index, la cual nos permite acceder a un registro en particular y el proceso de secuencial a partir del inicio del archivo en cualquier otro registro del archivo.
Cada registro en el archivo se identifica por medio de un número en este caso el index que se tomaría como la llave primaria.
![](img/3.png)
### Ejemplo
- [x] Si necesitas buscar un nombre en el directorio telefónico buscas el índice de la A-Z.
- [x] Sistema de Nóminas para registrar datos de empleado en específico.
### Ventajas y Desventajas
* Ventajas 
- [x] Permite procesar el archivo secuencial por orden lógico y también procesarlo al azar.
- [x] La organización indexada es conveniente para archivos con mediana volatilidad, actividad variable y tamaño relativamente estable.
- [x] Permite acceso directo al registro.
* Desventajas
- [x] Ocupa más espacios en el disco que el Sequential File, debido al uso del área de índices.
- [x] Los index requieren espacios extra, se necesita una doble búsqueda; una al archivo index y otra a la data.
- [x] Los registros tienen que tener una longitud fija.
- [x] El archivo debe estar separado por un dispositivo de acceso aleatorio: **no se puede utilizar con cintas magnéticas**.
### Procedimiento
#### 1. Inserción
```
template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
void ISAM<ObjType, MembType, T, IndexAmount>::add(ObjType record) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);
    fstream aux_file;
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }

    long int position = 0;
    pair<ObjType, char> result;
    bool is_in_file = search_in_tree(file, aux_file, record.*primary_key, position, result);

    file.close();
    aux_file.close();

    if(is_in_file) {
        cerr << RED << "Record exists\n" << RESET;
        return ;
    }

    file.open(this->name_file, ios::in | ios::out | ios::binary);
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }

    char f = result.second;
    ObjType temp;

    if(f == 'd') {
        file.seekg(position * (sizeof(ObjType) + 1));
        file >> temp;
        aux_file.seekg(0, ios::end);
        long int temp_position = aux_file.tellg() / (sizeof(ObjType) + 1);

        record.next = temp.next;
        temp.next.position = temp_position;
        temp.next.file = 'a';

        swap(record, temp);
        swap(record.next, temp.next);

        aux_file << record;
        file.seekg(position * (sizeof(ObjType) + 1));
        file << temp;
    } else {
        aux_file.seekg(position * (sizeof(ObjType) + 1));
        aux_file >> temp;
        aux_file.seekg(0, ios::end);
        long int temp_position = aux_file.tellg() / (sizeof(ObjType) + 1);

        record.next = temp.next;
        temp.next.position = temp_position;
        temp.next.file = 'a';

        swap(record, temp);
        swap(record.next, temp.next);

        aux_file << record;
        aux_file.seekg(position * (sizeof(ObjType) + 1));
        aux_file << temp;
    }
}
```
#### 2. Eliminación.
```

```
#### 3. Búsqueda
```
template<typename ObjType, typename MembType, typename T, size_t IndexAmount>
ObjType ISAM<ObjType, MembType, T, IndexAmount>::search(const T& key) {
    fstream file;
    file.open(this->name_file, ios::in | ios::out | ios::binary);
    fstream aux_file;
    aux_file.open(this->name_aux, ios::in | ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << RED << "Can't open " << this->name_file << RESET << endl;
        throw new exception;
    }

    if(!aux_file.is_open()) {
        cerr << RED << "Can't open " << this->name_aux << RESET << endl;
        throw new exception;
    }

    long int position = 0;
    pair<ObjType, char> result;
    bool is_in_file = search_in_tree(file, aux_file, key, position, result);

    file.close();
    aux_file.close();

    if(is_in_file) {
        return result.first;
    }

    cerr << RED << "Record " << YELLOW << key << RED << " doesn't exists" << RESET << endl;
    throw new exception;
}
```
### Resultado
#### 1. Inserción
* Sequential File:

* Indexed Sequential Access Method(ISAM):

#### 2. Búsqueda
* Sequential File:

* Indexed Sequential Access Method(ISAM):

#### Métricas
#### Análisis 
#### Comentarios
* Cada trabajo esta separado por carpetas independientes para usarlo simplemente correr el Makelife, escribir "make".
* El trabajo se realizo con un live share y por eso solo uno tiene bastantes commits.
#### Pruebas de uso
### [Video](https://www.youtube.com/watch?v=noAGPQbdsRI)




