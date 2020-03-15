#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Tweet_Description
{
	int id;
	char UTC_time[51];
	char text[141];
	char user_name[21];
	
	struct Tweet_Description * next;
}tweetNode;


void CreateEmptyTweetList(tweetNode **head);
tweetNode * Set_Tweet_Info(char user[21]);
int numNodes(tweetNode * head);
void InsertTweetAtHead(tweetNode **head, char user[21], int id);
void createTweet(tweetNode **head, char user[21], int id);
void SearchTweet(tweetNode *head, char key[141]);
void DisplayTweet(tweetNode *head);
void save_to_file(tweetNode * head, char file_name[100]);
void load_from_file(tweetNode **head, char file_name[100]);
void ClearTweet(tweetNode **head);

int id = 0;

int main(int argc, char * argv[])
{
	char choice;
	char key[141];
	char file_name[100];
	char user[21];
	
	tweetNode *head;
	CreateEmptyTweetList(&head); 
	
	printf("\n");
	printf("Enter username: ");
	scanf(" %[^\n]s",user);
	
	do 
	{
		//menu 
		printf("Press 'c' to create a new tweet\n");
		printf("Press 's' to search for a tweet\n");
		printf("Press 'd' to display tweet\n");
		printf("Press 'f' to save tweets to a file\n");
		printf("Press 'l' to load tweets from a file\n");
		printf("Press 'e' to Exit\n");
		scanf(" %c", &choice);
		
		switch(choice)
		{
			case 'c':
				id++;
				createTweet(&head, user, id);
				break;
			case 's':
				printf("Enter text to search: ");
				scanf(" %[^\n]s", key);
				SearchTweet(head,key);
				break;
			case 'd':
				DisplayTweet(head);
				break;
			case 'f':
				printf("Enter text file name: ");
				scanf(" %[^\n]s",file_name);
				save_to_file(head, file_name);
				break;
			case 'l':
				printf("Enter text file name: ");
				scanf(" %[^\n]s",file_name);
				load_from_file(&head, file_name);
				break;	
		}
	}
	while(choice!= 'e');
			
	return 0;
}

void CreateEmptyTweetList(tweetNode **head){

    *head = NULL;
}

tweetNode * Set_Tweet_Info(char user[21])
{
	tweetNode * tweet;
	time_t mytime;
	struct tm * ptm;
	
	tweet = (tweetNode *) malloc(sizeof(tweetNode)); //allocating memory for tweet
	time (&mytime); // Get local time in time_t
	ptm = gmtime (&mytime); // Find out UTC time
	time_t utctime = mktime(ptm); // Get UTC time as time_t
	
	strcpy(tweet->user_name, user);
	printf("What's happening?: ");
	scanf(" %[^\n]s", tweet->text); //assigning tweet to text	
	strcpy(tweet->UTC_time, ctime(&utctime)); //assigning UTC_time
	tweet->next = NULL;
	
	return tweet;
}

//insert node at the head of tweet
void InsertTweetAtHead(tweetNode **head, char user[21], int id) 
{
	
	tweetNode * tweetInfo = Set_Tweet_Info(user);
	tweetInfo->id = id;
	if(*head == NULL)
	{
		*head = tweetInfo;
	}
	else
	{
		tweetInfo->next = *head;
		*head = tweetInfo;
	}
	
}

//create a tweet and add it at the end of linked list
void createTweet(tweetNode **head, char user[21], int id) 
{	
	
	//if list empty, insert at the head
	if(*head == NULL)
	{
		InsertTweetAtHead(head, user, id);
	}
	else
	{
		tweetNode * tweetInfo = Set_Tweet_Info(user);
		tweetNode * current = *head;
		tweetInfo->id = id;
	
		while(current->next != NULL)
		{
			current = current->next;
		}
		current->next = tweetInfo;
	}
	
}

//search the tweets for whatever the user is looking for and output them
void SearchTweet(tweetNode *head, char key[141]) // works
{
	
	char * p;
	int check = 0;
	if(head == NULL)
	{
		printf("No tweet to search\n");
	}
	else
	{
		while(head != NULL)
		{
			p = strstr(head->text, key);
			if(p)//found tweet that has key
			{
				check = 1;
				printf("Tweet: %s\n",head->text);
			}
			head = head->next; 
		}
		if(check == 0)
		{
			printf("No match found!\n");
		}
	}
	
}

// print all tweets
void DisplayTweet(tweetNode *head) //works
{
    
	if(head == NULL)
	{
		printf("No tweet to display\n");
		return;
	}
	while( head != NULL)
	{
		printf("Username: %s\t\t\tID: %d\n", head->user_name, head->id);
		printf("Tweet:    %s\n", head->text);
		printf("Time:     %s\n", head->UTC_time);
		printf("---------------------------------------------------\n");
		head = head->next;
	}
	
}

//write the linked list to a text file
void save_to_file(tweetNode * head, char file_name[100]) //works
{
	
	FILE * myfile;
	int check = 0;
	int tweet = numNodes(head);
	//open file
	myfile = fopen(file_name, "w");
	if(myfile == NULL)
	{
		fprintf(stderr, "Error when opening file\n");
		exit(1);
	}
	else
	{
		//writing to file
		while(head != NULL)
		{
			fprintf(myfile, "%s %d\n%s\n%s\n", head->user_name, head->id, head->text, head->UTC_time);
			head = head->next;
			check = 1;
		}
		fclose(myfile);
		
		if(check == 1)
		{
			printf("%d Tweets saved successfully!\n", tweet);
		}
	}

}

// delete all the nodes from the linked list
void ClearTweet(tweetNode **head)
{
	tweetNode * current = *head;
	
	while(current != NULL)
	{
		current = current->next;
		free(*head);
		*head = current;
	}

}

//count the number of nodes in the linked list
int numNodes(tweetNode * head) 
{
	int count = 0;
	tweetNode * current = head;
	while(current != NULL)
	{
		count ++;
		current = current->next;
	}
	return count;
}

//overrides the linked list with the new one read from a text file.
void load_from_file(tweetNode **head, char file_name[100]) 
{
	ClearTweet(head);
	FILE * myfile;
	//open file
	myfile = fopen(file_name, "r");
	
	if(myfile == NULL)
	{
		fprintf(stderr, "Error when opening file\n");
		exit(1);
	}
	else
	{	
		tweetNode * temp; 
	
		while(1) 		
		{
			//read data from file
			temp = (tweetNode *) malloc (sizeof(tweetNode));
			if(fscanf(myfile, "%21s %d\n%141[^\n]\n%51[^\n]", temp->user_name, &temp->id, temp->text, temp->UTC_time) == EOF)
			{
				break;
			}
			//print data read from file
			printf("Username: %s\t\tID: %d\n", temp->user_name, temp->id);
			printf("Tweet: %s\n", temp->text);
			printf("Time:  %s\n\n", temp->UTC_time);
			id = temp->id;
			//append data to end of linked list
			tweetNode *last = *head;
			temp->next = NULL;
			if(*head == NULL)
			{
				*head = temp;
			}
			else
			{
				while(last->next != NULL)
				{
					last = last->next;
				}
				last->next = temp;
			}
		}
			
		fclose(myfile);
	}
}