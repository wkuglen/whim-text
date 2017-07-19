#include "whim.h"

struct node* list;
struct node* topNode;
struct node* bottomNode;

void addLine (struct node*, struct line*);

void init (char* filename) {
	//do something with filename

	//set up linked list
	list = topNode = bottomNode = NULL;
	//read line by line
	int lineCount = 0;
	while (1) {
		struct line* nextLine = malloc (sizeof (struct line));
	}
}

void addLine (struct node* above, struct line* nextLine) {
	struct node* insert = malloc (sizeof (struct node));
	insert->data = nextLine;

	if (above == NULL)
	{
		insert->previous = NULL;
		insert->next = list;
		list = insert;
	} else {
	//Get all nodes
	struct node* below = above->next;
	

	//insert into linked list
	above->next = insert;
	insert->previous = above;
	insert->next = below;
	below->previous = insert;
	}
}