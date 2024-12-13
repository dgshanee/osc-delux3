#ifndef HASHMAP_C
#define HASHMAP_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
  int key;
  int val;

  struct node *next;
};

void initNode(struct node *node, int key, int val){
  node->key = key;
  node->val = val;
  node->next = NULL;
}

typedef struct{
  int numElements, capacity;
  
  struct node **arr;
}Hashmap;

void initHashmap(Hashmap *mp){
  mp->numElements = 0;
  mp->capacity = 100;

  mp->arr = (struct node**)malloc(sizeof(struct node*) * mp->capacity);
}

void add(Hashmap *mp, int key, int val){
  struct node *newNode = (struct node*)malloc(sizeof(struct node*));
  initNode(newNode, key, val);

  mp->arr[key] = newNode;
}

int search(Hashmap *mp, int key){
  return (mp->arr[key])->val;
}

#endif