//Patrick Lyons
//COE 0449
//Spring 2017
//Project 3 Main File

#include<stdio.h>
#include<string.h>
#include<unistd.h>

//Function to find the next free space the allocation will fit into
void* best_fit(int);

//Function to split free space
void* split_free(int);

//Starts node struct
typedef struct node *Node;

struct node
{
  int size;

  //Boolean. 1 = Free, 0 = Allocated
  int free_space;

  struct node *next;
  struct node *prev;
};

//Node Locations
static Node first_node = 0;
static Node final_node = 0;
static Node current_node = 0;


//Extends heap if needed
Node extend_heap(int size)
{
  Node n;

  n = sbrk(0);
  if (sbrk(sizeof(struct node) + size) == (void*) - 1)
    return NULL;

  n->size = size;
  n->free_space = 0;
  n->next = NULL;
  n->prev = final_node;

  final_node->next = n;
  final_node = n;

  return n;
}


static Node init_list(int size)
{

  first_node = sbrk(size + sizeof(struct node));

  first_node->size = size;
  first_node->free_space = 0;
  first_node->next = NULL;
  first_node->prev = NULL;

  final_node = first_node;
  current_node = first_node;

  return first_node;
}

//Allocate space by bestfit algorithm
void *my_bestfit_malloc(int size)
{

  if (first_node == 0)
    return (void*)(init_list(size) + 1);

  else
  {
    Node n = best_fit(size);

    if (n == NULL)
      return (extend_heap(size) + 1);

    else
    {
      current_node = n;
      return n;
    }
  }
} //End of bestfit

void my_free(void *ptr)
{
  Node n = (Node)ptr - 1;
  n->free_space = 1;

  //Combines consecutive free nodes into one large free space
  if (n->prev != NULL)
  {
    if (n->prev->free_space == 1)
    {
      n->prev->size = n->size + sizeof(struct node);
      n->prev->next = n->next;

      if (final_node == n)
        final_node = n->prev;

      if (current_node == n)
        current_node = n->prev;

      n = n->prev;
    }
  }

  if (n->next != NULL)
  {
    if (n->next->free_space == 1)
    {
      n->size += n->next->size + sizeof(struct node);
      n->next = n->next->next;
    }
  }

  //Checks if the last node is free and gets rid of it if so
  if (n == final_node)
  {
    if (final_node == first_node)
    {
      first_node = 0;
      current_node = 0;
      final_node = 0;
      brk((void*)(n));
    }

    else if (final_node == current_node)
    {
      current_node = current_node->prev;
      final_node = final_node->prev;
      final_node->next = NULL;
      brk((void*)(n - 1));
    }

    else
    {
      final_node = final_node->prev;
      final_node->next = NULL;
      brk((void*)(n - 1));
    }
  }

}//End of free

//Allocates the best possible space
void* best_fit(int size)
{

  Node n = current_node->next;
  int space_not_found = 0;


  while(n != NULL)
  {


    do
    {
      //Checks if free space is exactly equal to required allocation
      //If so, takes it immediately
      if (n->free_space == 1 && n->size == size)
      {
        n->free_space = 0;
        n->size;

        current_node = n;
        space_not_found = 1;

        return n;
      }

      //Increments to next free space if available
      else
      {
        if (n->next == NULL)
        {
          if (n == first_node)
            n = NULL;

            //Resets to first node
          else
            n = first_node;
        }

        else
        {
          if (n == current_node)
            n = NULL;
          else
            n = n->next;
        }
      }
    } while(space_not_found == 0);


    //Allocate space when there is enough, and make a new node with the rest of the free space
    if (n->free_space == 1 && n->size > (size + sizeof(struct node)))
    {
      n = split_free(size);
      n->free_space = 0;
      n->size = size;
      return n;
    }

    if (n->next == NULL)
    {
      if (n == first_node)
        n = NULL;

      else
        n = first_node;
    }

    else
    {
      if (n == current_node)
        n = NULL;
      else
        n = n->next;
    }
  }

  //No free space was large enough
  return (void*)(extend_heap(size) + 1);
}

//splits up a large free space into the needed size, and the remaining size
void* split_free(int size){

  Node n = current_node + 1 + (size / sizeof(struct node));

  n->free_space = 0;
  n->next = current_node->next;
  n->prev = current_node;
  n->size = size;


  current_node->next->prev = n;
  current_node->next = n;

  return n;
}
