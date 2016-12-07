/* Author Greg Johnson */
/* 9.21.16 */
#include "linked_list.h"

/*
operation:		Default comparison method, comparing casted int pointers
				Returns either -1, 0 or 1... But it doesn't mean anything
*/
static int compare_default(void *a, void *b){
	if((intptr_t) a < (intptr_t) b){		// cast to intptr_t for inequality comparison
		return -1;
	}else if((intptr_t) a > (intptr_t) b){	// cast to intptr_t for inequality comparison
		return 1;
	}else{
		return 0;
	}
}

/*
operation:		Default freeing method, doesnt do anything.
*/
static void free_node_default(void *ptr){} // empty by default

/*
operation:		Default printing method.
*/
static void print_node_default(void *ptr){
	printf("default print");
}

/*
operation:		initalize and return a pointer to linked_list
postconditions: head initialized to NULL, size to 0
*/
linked_list* create_linked_list(comparison_func compare, free_func free_node, free_func print){
	linked_list *list = calloc(1, sizeof(struct linked_list));		// Allocate memory
	list->compare = compare? compare : &compare_default;			// assign comparison function
	list->free_node = free_node? free_node : &free_node_default;	// assign fucntion to free nodes
	list->print = print? print : &print_node_default;				// assign printing func
	list->head = NULL;												// Empty list: no head
	list->size = 0;													// Empty list: 0 size
	return list;
}

/*
operation:		initialize and return a pointer to list_node
postconditions:	data copied to data member with strdup to avoid issues, 
				both parent and child list_node initialized to NULL
*/
list_node* create_list_node(void *data){
	list_node *n = calloc(1, sizeof(struct list_node));	// Allocate memory
	n->data = data;					// Set list_node's data
	n->parent = n->child = NULL;	// Initialize both parent and child to NULL
	return n;
}

/*
operation:		Return the list_node in a linked list with given data, if found.
postconditions:	Intended operation, otherwise returning NULL if not found.
*/
list_node* get_list_node(linked_list *list, void *data){
	list_node *n = list->head;					// start search at head
	while(n){									// loop while n is not NULL
		if(list->compare(n->data, data) == 0){	// compare current data with given data
			return n;							// return current list_node: match found
		}
		n = n->child;							// advance to the next node
	}
	return NULL;								// return NULL: no match found
}

/*
operation:	Add a node to the end of the list and re-link accordingly.
*/
void add_list_node(linked_list *list, list_node *n){
	if(list->size == 0){
		list->head = n;					// list is empty, list_node becomes head by default and subsequently:
		list->head->parent = n;			// head's child is itself
		list->head->child = NULL;		// head's parent is itself
	}else{									
		n->parent = list->head->parent;	// set list_node's parent to tail
		n->child = NULL;				// set list_node's child to head
		list->head->parent->child = n;	// set tail's child to list_node
		list->head->parent = n;			// set head's parent to list_node
	}
	list->size++;						// increment size of list by 1
}

/*
operation:		Print the list_nodes and their relationships in a linked list
*/
void print_list(linked_list *list){
	if(!list->head){
		printf("<List is empty>\n");		// list is empty, nothing to print
	}
	list_node *n = list->head;				// start iteration at head
	while(n){								// loop while n is not NULL
		if(n == list->head->parent){				// if n is the tail, print new line after
			list->print(n->data);
			//printf(list->print_format, n->data);	// print the data of n
			printf("\n");							// print the new line character
		}else{										// n is not the tail, normal print
			list->print(n->data);
			//printf(list->print_format, n->data);	// print the data of n
			printf(" <-> ");						// print to show double linkage
		}
		n = n->child;								// advance n to n's child
	}
}

/*
operation: Free every list_node of the linked list, points head to NULL and sets size to 0
*/
void empty_list(linked_list *list){
	if(!list->head) return;
	list_node *parent, *temp = list->head;
	while(temp){					// loop while temp is not NULL to free every node
		parent = temp;				// keep track of last node to free it
		temp = temp->child;			// advance through the list
		list->free_node(parent);	// free the current node (parent)
	}
	list->head = NULL;				// reset the list
	list->size = 0;
}

list_node* get_list_node_at(linked_list *list, int pos){
	if(pos < 0 || pos >= list->size){
		return NULL;
	}
	list_node *node = list->head;
	for(int i = 0; i < pos; i++){
		node = node->child;
	}
	return node;
}

/*
operation:		Insert a list_node in place of another and re-link accordingly.
preconditions:	src (source) will take the place of dst (destination)
postconditions:	If successful, src will be to the left of dst, returns true.
				Otherwise, dst was not found in the list, returns false.
				If the list's head is displaced, src will become the list's head
*/
bool insert_list_node(linked_list *list, list_node *dst, list_node *src){
	if(!contains_list_node(list, dst)){
		return false;				// return false: not contained
	}else{
		dst->parent->child = src;	// set destination's parent's child to source
		src->parent = dst->parent;	// set source's parent to destination's parent
		src->child = dst;			// set source's child to destination
		dst->parent = src;			// set destination's parent to source
		if(dst == list->head){		// head was displaced
			list->head = src;		// set head to src
		}
		list->size++;				// increment size of list by 1
		return true;				// return true: contained, successful
	}
}

/*
operation:		Determine if given list_node is contained in a list.
postconditions:	Returns true if a match is found, false if not.
*/
bool contains_list_node(linked_list *list, list_node *src){
	list_node *n = list->head;	// start search at head
	while(n){					// loop while n is not NULL
		if(n == src){			// compare each list_node with source
			return true;		// return true: match found
		}
		n = n->child;			// advance through list
	}
	return false;				// return false: no match found after 1 iteration
}

/*
operation:		Delete a list_node from a list and re-link accordingly.
postconditions:	Returns true if successfully removed, false if not. If src is list's head
				to be deleted, src's child will become head. src is freed.
*/
bool delete_list_node(linked_list *list, list_node *src){
	if(!contains_list_node(list, src)){
		return false;						// return false: not contained
	}else{
		list_node *tail = list->head;
		while(tail->child) tail = tail->child;
		if(tail == src){
			if(src->parent)
			src->parent->child = NULL;
			list->free_node(src);				// free source's allocated memory
			list->size--;						// decrement size of list by 1
			return true;
		}
		src->parent->child = src->child;	// set source's parent's child to source's child
		src->child->parent = src->parent;	// set source's child's parent to source's parent
		if(src == list->head){				// head was deleted
			list->head = src->child;		// set head to src
			src->parent->child = NULL;
		}
		list->size--;
		return true;						// return true: contained, successful
	}
}