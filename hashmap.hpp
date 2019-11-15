#ifndef __HASHMAPA_HPP
#define __HASHMAPA_HPP
#include <iostream>
#include <cstdint>
#include <limits>
using namespace std;

//INTERFASE
const int TABLE_SIZE = 1013;

template<typename VT>
struct KeyValueNode{
  string key;
  VT value;
  KeyValueNode<VT> *next;
};

template<typename VT>
class HashMap{
private:
  KeyValueNode<VT> **table; //apuntador a los apuntadores bucket

  int tableSize; //tamaño de la tabla de apuntadores
  int count; //numero de elemenos en la tabla

  VT notfound; //valor que retorna cuando la busqueda no es exitosa

  KeyValueNode<VT> *search_bucket(int index, string key); //busca una llave dentro de el bucket que index indica

//hash function
  unsigned int hash(string key){
    unsigned int hashVal = 0;
    for (char ch : key)
      hashVal += ch;
    return hashVal % tableSize;
  }


public:
  HashMap(VT def);
  ~HashMap();

  int size(); //returna el numero de elementos
  bool empty(); //true si esta vacía
  void clear(); //elimina todos los elementos

  VT get(string key); //chained hash search: busca el elemento que tiene la clave key
  bool search(string key); //retorna true si la llave esta en la tabla
  bool insert(string key, VT value); //chained hash insert: insert un elemento al inicio del bucket
  void remove(string key); //chained hash remove: remueve el elemento con la clave key

  //void distribution(const string &filename);

};


//IMPLEMENTACIÓN

template<typename VT>
HashMap<VT>::HashMap(VT def){
  count = 0;
  tableSize = TABLE_SIZE;
  table = new KeyValueNode<VT> *[tableSize];
  notfound = def;
  for(int i = 0; i < tableSize; i++){
    table[i] = nullptr;
  }
}


template<typename VT>
HashMap<VT>::~HashMap(){
  clear();
  delete table;
}


template<typename VT>
int HashMap<VT>::size(){
  return count;
}

template<typename VT>
bool HashMap<VT>::empty(){
  return count == 0;
}


template<typename VT>
void HashMap<VT>::clear(){
  for(int i = 0; i < tableSize; i++){
    while(table[i] != nullptr){
      KeyValueNode<VT> *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  count = 0;
}


template<typename VT>
KeyValueNode<VT>* HashMap<VT>::search_bucket(int index, string key){
  //retorna el elemento si lo encuentra, sino retorna un nullptr
  KeyValueNode<VT> *temp = table[index];
  //temp+=index;
  while(temp != nullptr){
    if(temp->key == key){
      return temp;
    }
    else{
      temp = temp->next;
    }
  }
  return nullptr;
}



template<typename VT>
//busca el valor con la clave key
VT HashMap<VT>::get(string key){
  KeyValueNode<VT> *nw = search_bucket(hash(key), key);
  if(nw!= nullptr){
    return nw->value;
  }
  else{
    return notfound;
  }
}



template<typename VT>
//retorna true si la llave esta y false si no esta
bool HashMap<VT>::search(string key){
  KeyValueNode<VT> *nw = search_bucket(hash(key), key);
  if(nw != nullptr){
    return true;
  }
  else{
    return false;
  }
}



template<typename VT>
//si la clave ya esta cambio el valor
bool HashMap<VT>::insert(string key, VT value){
  if (!search(key)){
    KeyValueNode<VT> *nw = new KeyValueNode<VT>;
    nw->key = key;
    nw->value = value;
    nw->next = table[hash(key)];
    table[hash(key)] = nw;
    count ++;
    return true;
  }
  else{
    search_bucket(hash(key), key)->value = value;
    return false;
  }
}



template<typename VT>
void HashMap<VT>::remove(string key){
  if(search(key)){
    unsigned int index = hash(key);
    KeyValueNode<VT> *temp = table[index];
    while(temp != nullptr){
      KeyValueNode<VT> *temp2 = temp->next;
      if(temp->key == key){
        KeyValueNode<VT> *aux = temp;
        table[index] = temp->next;
        delete aux;
        temp = nullptr;
      }
      else if(temp2->key == key){
        KeyValueNode<VT> *aux = temp;
        temp->next = temp2->next;
        delete aux;
        temp2 = nullptr;
      }
      else{
        temp = temp2->next;
      }
    }
    count--;
  }
}



#endif
