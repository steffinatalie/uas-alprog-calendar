#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>
#include <conio.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct node
{
  char data[100]; // to store the appointment strings
  struct node *next; // to link to the next appointment at the same date
} Node;

typedef struct
{
  int year;
  int month;
  int day;
  Node *list; // list of appointments
} Appointment;

typedef struct {
    // Change 'data' to the desired data type for the queue elements
    char data[MAX_SIZE];  
    int year;
    int month;
    int day;
    // Add any additional fields required for the queue elements
} QueueItem;

typedef struct {
    QueueItem items[MAX_SIZE];
    int front;
    int rear;
} Queue;

// function prototypes
int menu(Appointment *appointment_list, int *counter, int current_year, Queue *queue);
void press_any_key();
int dayNumber(int day, int month, int year);
char* getMonthName(int monthNumber);
int numberOfDays(int monthNumber, int year);
void print_calendar(int year, Appointment *appointment_list, int *counter);
int get_index(int y, int m, int d, Appointment* list, int* counter);
int compare_dates(const void *a, const void *b);
void summary(Appointment *appointment_list, int *counter);
void remove_appointment(Appointment *appointment_list, int *counter, int current_year, Queue *queue);
void add_appointment(Appointment *appointment_list, int *counter, int current_year, Queue *queue);
void print_monthly_appointment(Appointment* appointment_list, int y, int m, int *counter);
void recycle_bin(Appointment *appointment_list, int *counter, int current_year, Queue *queue);

// function protoypes: queue things
void initializeQueue(Queue *queue);
bool isFull(Queue *queue);
bool isEmpty(Queue *queue);
void enqueue(Queue *queue, QueueItem item);
QueueItem dequeue(Queue *queue);
void removeItem(Queue *queue, int index);
int printQueue(Queue *queue);

// function prototypes (interface)
void print_header(int i);
void opening();
void closing();
void show_loading_bar();

int main(){
  Appointment *appointment_list = (Appointment *) malloc(sizeof(Appointment));
  int counter = 0; // appointment list counter

  // Initialize queue
  Queue queue;
  initializeQueue(&queue);

  int current_year = 2023;

  opening();

  menu(appointment_list, &counter, current_year, &queue);

  return 0;
}


// to get the starting day 
int dayNumber(int day, int month, int year)
{

	static int t[] = { 0, 3, 2, 5, 0, 3,
					5, 1, 4, 6, 2, 4 };
	year -= month < 3;
	return (year + year / 4
			- year / 100
			+ year / 400
			+ t[month - 1] + day)
		% 7;
}

// for going back to menu
void press_any_key(){
  printf("\nPress any key to go back to menu...");
  getch();
}

// Function that returns the name of the
// month for the given month Number
// January - 0, February - 1 and so on
char* getMonthName(int monthNumber)
{
	char* month;

	switch (monthNumber) {
	case 0:
		month = "January";
		break;
	case 1:
		month = "February";
		break;
	case 2:
		month = "March";
		break;
	case 3:
		month = "April";
		break;
	case 4:
		month = "May";
		break;
	case 5:
		month = "June";
		break;
	case 6:
		month = "July";
		break;
	case 7:
		month = "August";
		break;
	case 8:
		month = "September";
		break;
	case 9:
		month = "October";
		break;
	case 10:
		month = "November";
		break;
	case 11:
		month = "December";
		break;
	}
	return month;
}

// to get the number of days for each month
int numberOfDays(int monthNumber, int year)
{
	// January
	if (monthNumber == 0)
		return (31);

	// February
	if (monthNumber == 1) {
		// If the year is leap then Feb
		// has 29 days
		if (year % 400 == 0
			|| (year % 4 == 0
				&& year % 100 != 0))
			return (29);
		else
			return (28);
	}

	// March
	if (monthNumber == 2)
		return (31);

	// April
	if (monthNumber == 3)
		return (30);

	// May
	if (monthNumber == 4)
		return (31);

	// June
	if (monthNumber == 5)
		return (30);

	// July
	if (monthNumber == 6)
		return (31);

	// August
	if (monthNumber == 7)
		return (31);

	// September
	if (monthNumber == 8)
		return (30);

	// October
	if (monthNumber == 9)
		return (31);

	// November
	if (monthNumber == 10)
		return (30);

	// December
	if (monthNumber == 11)
		return (31);
}

// Function to print the calendar of
// the given year
void print_calendar(int year, Appointment *appointment_list, int *counter)
{

	printf("\n	 Calendar - %d\n\n", year);
	int days;

	// Index of the day from 0 to 6
	int current = dayNumber(1, 1, year);

	// i for Iterate through months
	// j for Iterate through days
	// of the month - i
	for (int i = 0; i < 12; i++) {
		days = numberOfDays(i, year);

		// Print the current month name
		printf("\n -------------%s-------------\n",
			getMonthName(i));

		// Print the columns
		printf("  Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");

		// Print appropriate spaces
		int k;
		for (k = 0; k < current; k++)
			printf("     ");

		for (int j = 1; j <= days; j++) {
			printf("%5d", j);

			if (++k > 6) {
				k = 0;
				printf("\n");
			}
		}

		if (k)
			printf("\n");

    // to print appointments for each month
    print_monthly_appointment(appointment_list, year, i+1, counter);

		current = k;
	}

  int i;
  char c[] = " ";
  printf("\nDisplay next year? (y/n)\n");
  printf(">>> ");
  scanf(" %s", &c);
  if (strcmp(c, "y") == 0){
    // to display next year's calendar
    print_calendar(++year, appointment_list, counter);
  }
  else{
    press_any_key();
    return;
  }

}

// to print appointment in each month
void print_monthly_appointment(Appointment* appointment_list, int y, int m, int *counter){
  int i;
  for (i = 0; i < *counter; i++){
    if (appointment_list[i].year == y && appointment_list[i].month == m){
      printf("\n%d: \n", appointment_list[i].day);
      for (Node* curr = appointment_list[i].list; curr!=NULL; curr= curr->next){
        printf("- %s\n", curr->data);
      }
    }
  }
}

// to get the index of the appointment list 
int get_index(int y, int m, int d, Appointment* list, int* counter){
  int i;
  for (i = 0; i < *counter; i++){
    if (list[i].year == y && list[i].month == m && list[i].day == d){ // if the date is already exist
      return i;
    }
  }

  return *counter; // if the date is not in the list
}

// Function to compare two appointments by date
int compare_dates(const void *a, const void *b){
  const Appointment *appt_a = (const Appointment *)a;
  const Appointment *appt_b = (const Appointment *)b;
  
  // Compare the years
  if (appt_a->year != appt_b->year) {
      return appt_a->year - appt_b->year;
  }
  
  // Compare the months
  if (appt_a->month != appt_b->month) {
      return appt_a->month - appt_b->month;
  }
  
  // Compare the days
  return appt_a->day - appt_b->day;
}


// to print summary of the appointments
void summary(Appointment *appointment_list, int *counter){
  if (*counter == 0){ // if there is no appointment
    printf("There is no appointment\n");
  }

  // sort the appointment
  qsort(appointment_list, *counter, sizeof(Appointment), compare_dates);

  int i, j = 0;
  for (i = 0; i < *counter; i++){
    printf("%d-%d-%d: \n", appointment_list[i].day, appointment_list[i].month, appointment_list[i].year);
    for (Node* curr = appointment_list[i].list; curr!=NULL; curr= curr->next){
      printf("%d)   %s\n", ++j, curr->data);
    }
  }  
}

// to remove the selected appointment
void remove_appointment(Appointment *appointment_list, int *counter, int current_year, Queue *queue){
  system("cls");
  print_header(2);

  if(*counter == 0){ // if there is no appointment
    printf("Nothing to remove\n");
  }
  else{
    summary(appointment_list, counter); // print summary of all the appointments

    int r;
    printf("Which appointment to be removed? (Index)\n");
    printf(">>> ");
    scanf("%d", &r);

    // to get the indexes needed
    int i, j; 
    for (i = 0; i < *counter && r != 0; i++){
      j = 0;
      Node* curr = appointment_list[i].list;
      while(curr!=NULL){
        j++; // counter of the selected appoinment in the date
        if (--r == 0){
          i--; // counter of the element of the appointment list
          break;
        }

        curr = curr->next;
      }
    }

    // if there is only one appointment
    if (*counter == 1 && j == 1 && appointment_list[i].list->next == NULL){

      // put the removed item to queue
      QueueItem *item = (QueueItem*)malloc(sizeof(QueueItem));
      strcpy(item->data, appointment_list[i].list->data);
      item->day = appointment_list[i].day;
      item->month = appointment_list[i].month;
      item->year = appointment_list[i].year;
      enqueue(queue, *item);
      
      Appointment* empty_list = (Appointment*)malloc(sizeof(Appointment));
      appointment_list = empty_list;

      Node* empty_node = (Node*)malloc(sizeof(Node));
      empty_node->next = NULL;

      appointment_list[0].list = empty_node;
      *counter = 0;
    }
    // if there are some appointments in the date
    else if (appointment_list[i].list->next != NULL){
      
      if (j == 1) // to remove the first node
      {
        // put the removed item to queue
        QueueItem *item = (QueueItem*)malloc(sizeof(QueueItem));
        strcpy(item->data, appointment_list[i].list->data);
        item->day = appointment_list[i].day;
        item->month = appointment_list[i].month;
        item->year = appointment_list[i].year;
        enqueue(queue, *item);

        Node* to_remove = appointment_list[i].list;
        appointment_list[i].list = to_remove->next;
        free(to_remove);

      }
      else{ // if the selected is not the first node
        for (Node* curr = appointment_list[i].list; curr!=NULL; curr = curr->next){
        if (--j == 1){
          // put the removed item to queue
          QueueItem *item = (QueueItem*)malloc(sizeof(QueueItem));
          strcpy(item->data, curr->next->data);
          item->day = appointment_list[i].day;
          item->month = appointment_list[i].month;
          item->year = appointment_list[i].year;
          enqueue(queue, *item);

          Node* to_remove = curr->next;
          if (curr->next->next == NULL){ // if the selected is the last node
            curr->next = NULL;
          }
          else{ // if the selected is not the last node
            curr->next = curr->next->next;
          }

          free(to_remove); // free the memory
          break;
        }
      }
      
      
      }
    }
    else{ // if the selected appointment is a single appointment in a date

      // put the removed item to queue
      QueueItem *item = (QueueItem*)malloc(sizeof(QueueItem));
      strcpy(item->data, appointment_list[j].list->data);
      item->day = appointment_list[i].day;
      item->month = appointment_list[i].month;
      item->year = appointment_list[i].year;
      enqueue(queue, *item);
      
      *counter-=1;
      for (j = 0; j < *counter; j++){
        if (j >= i){

          appointment_list[j] = appointment_list[j+1];
        }
      }
    

    }

    

    // print the summary after removal
    summary(appointment_list, counter);
    press_any_key();
    menu(appointment_list, counter, current_year, queue);
  }

  

  press_any_key();
  menu(appointment_list, counter, current_year, queue);

}

// to add a new appointment
void add_appointment(Appointment *appointment_list, int *counter, int current_year, Queue *queue){
  system("cls");
  print_header(1);

  // to get the dates

  int y, m;
  printf("Which year?\n");
  scanf("%d", &y);
  while(y < current_year){
      printf("\nInvalid\n");
      printf("\nWhich year?\n");
      scanf("%d", &y);
  }

  printf("\nWhich month? (1-12)\n");
  scanf("%d", &m);
  while(m > 12 ||  m < 1){
      printf("\nInvalid\n");
      printf("\nWhich month? (1-12)\n");
      scanf("%d", &m);

  }

  int num = numberOfDays(m-1, y), d;
  printf("\nWhich day? (1-%d)\n", num);
  scanf("%d", &d);
  while(d > num || d < 1){
      printf("\nInvalid\n");
      printf("\nWhich day? (1-%d)\n", num);
      scanf("%d", &d);
  }


  // store the appointment in the node
  char str[100];
  printf("\nWhat is the appointment?\n");
  scanf(" %[^\n]s", str);

  Node *new_node = (Node *)malloc(sizeof(Node));
  strcpy(new_node->data, str);
  new_node->next = NULL;

  // check whether the date has already inserted in the appointment list 
  int index = get_index(y, m, d, appointment_list, counter);
  if (index == *counter){ // if the date is not already inserted

    ++*counter;
    Appointment *tmp = (Appointment *) realloc(appointment_list, *counter * sizeof(Appointment));
    if (tmp == NULL){
      printf("\n[Error] out of memory\n");
      return;
    }
    appointment_list = tmp;
    
    // insert the data to the list
    appointment_list[index].year = y;
    appointment_list[index].month = m;
    appointment_list[index].day = d;

    appointment_list[index].list = new_node;
  }
  else{ // if date is already inserted
    // insert appointment at the end of the linked list
    Node* curr = appointment_list[index].list;
    while(curr->next!= NULL){
      curr = curr->next;
    }
    curr->next = new_node;
  }
  printf("\nAdded: ");
  printf("%d-%d-%d: %s\n", d, m, y, new_node->data);

  char c[] = " ";
  printf("\nDo you want to add more? (y/n)\n");
  scanf(" %s", &c);
  if (strcmp(c, "y") == 0){
      add_appointment(appointment_list, counter, current_year, queue);
  }

  // press_any_key();
  menu(appointment_list, counter, current_year, queue);

}


void recycle_bin(Appointment *appointment_list, int *counter, int current_year, Queue *queue){
  int queue_size = printQueue(queue);

  if (queue_size == 0){
    press_any_key();
    menu(appointment_list, counter, current_year, queue);
  }

  int r;
  printf("Which to recover? (Index)\n");
  printf(">>> ");
  scanf("%d", &r);

  r--;

  // add back to the appointment list
  Node *new_node = (Node *)malloc(sizeof(Node));
  strcpy(new_node->data, queue->items[r].data);
  new_node->next = NULL;

  // check whether the date has already inserted in the appointment list 
  int index = get_index(queue->items[r].year, queue->items[r].month, queue->items[r].day, appointment_list, counter);
  if (index == *counter){ // if the date is not already inserted

    ++*counter;
    Appointment *tmp = (Appointment *) realloc(appointment_list, *counter * sizeof(Appointment));
    if (tmp == NULL){
      printf("\n[Error] out of memory\n");
      return;
    }
    appointment_list = tmp;
    
    // insert the data to the list
    appointment_list[index].year = queue->items[r].year;
    appointment_list[index].month = queue->items[r].month;
    appointment_list[index].day = queue->items[r].day;

    appointment_list[index].list = new_node;
  }
  else{ // if date is already inserted
    // insert appointment at the end of the linked list
    Node* curr = appointment_list[index].list;
    while(curr->next!= NULL){
      curr = curr->next;
    }
    curr->next = new_node;
  }

  // remove from queue
  removeItem(queue, ++r);
  printQueue(queue);

  menu(appointment_list, counter, current_year, queue);
}

// main menu
int menu(Appointment *appointment_list, int *counter, int current_year, Queue *queue){
	int input;
	do{
		system("cls");
		printf("Menu:\n");
		printf("1. Add appointment\n");
		printf("2. Remove appointment\n");
		printf("3. Display calendar\n");
		printf("4. Summary\n");
    printf("5. Recycle bin\n");
		printf("6. Exit\n");

		printf(">> ");
		input = getch();

		switch (input)
		{
		case '1':
			add_appointment(appointment_list, counter, current_year, queue);
			break;

		case '2':
			remove_appointment(appointment_list, counter, current_year, queue);
			break;
		
		case '3':
      system("cls");
      print_header(3);
			print_calendar(current_year, appointment_list, counter);
			break;

		case '4':
      system("cls");
      print_header(4);
			summary(appointment_list, counter);
      press_any_key();
			break;

    case '5':
      system("cls");
      print_header(5);
      recycle_bin(appointment_list, counter, current_year, queue);
      break;

		case '6':
      closing();
      exit(0);
		
		default:
			menu(appointment_list, counter, current_year, queue);
			break;
		}
	}while(input != '5');
    
}

void print_header(int i){
  system("cls");

  switch (i)
  {
  case 1:
    printf("=======================================================\n");
    printf("||                                                   ||\n");
    printf("||            >    ADD APPOINTMENT    <              ||\n");
    printf("||                                                   ||\n");
    printf("=======================================================\n"); 
    break;
  
  case 2:
    printf("=======================================================\n");
    printf("||                                                   ||\n");
    printf("||          >    REMOVE APPOINTMENT    <             ||\n");
    printf("||                                                   ||\n");
    printf("=======================================================\n"); 
    break;

  case 3:
    printf("=======================================================\n");
    printf("||                                                   ||\n");
    printf("||                >    CALENDAR    <                 ||\n");
    printf("||                                                   ||\n");
    printf("=======================================================\n"); 
    break;

  case 4:
    printf("=======================================================\n");
    printf("||                                                   ||\n");
    printf("||                >    SUMMARY    <                  ||\n");
    printf("||                                                   ||\n");
    printf("=======================================================\n"); 
    break;

  case 5:
    printf("=======================================================\n");
    printf("||                                                   ||\n");
    printf("||              >    RECYCLE BIN    <                ||\n");
    printf("||                                                   ||\n");
    printf("=======================================================\n"); 
    break;
  }

  
}

void opening(){
    int i;
    char welcome[] = {'W', 'E', 'L', 'C', 'O', 'M', 'E', '!', '\0'};

    system("cls");
    printf("=======================================================\n");
    printf("||                                                   ||\n");
    printf("||                 C A L E N D A R                   ||\n");
    printf("||                                                   ||\n");
    printf("||           Never miss your appointment^^           ||\n");
    printf("||                                                   ||\n");
    printf("=======================================================\n");
    
    usleep(25000);

    show_loading_bar();

    Sleep(1);

    system("cls");

    usleep(25000);

    printf("\n\n\n                  ");
    for(i = 0; i < strlen(welcome); i++){
        printf("%c ", welcome[i]);
        usleep(80000);

    }

    Sleep(1);
}

void closing(){
    int i;
    char thx[] = {'T', 'H', 'A', 'N', 'K', ' ', 'Y', 'O', 'U', '!', '\0'};
    system("cls");

    usleep(25000);

    printf("\n\n\n                  ");
    for(i = 0; i < strlen(thx); i++){
        printf("%c ", thx[i]);
        usleep(80000);

    }
    printf("\n\n\n\n\n\n");
}

void show_loading_bar(){
    int i;
    char a = 177, b = 219;

    // print empty loading bar
    for (i = 0; i < 52; i++)
        printf("%c", a);

    // print the loading bar
    printf("\r");
    printf("  ");
    for (i = 0; i < 50; i++) {
        printf("%c", b);
        // add delay for effect
        Sleep(35);
    }
}

void initializeQueue(Queue *queue) {
    queue->front = 0;
    queue->rear = -1;
}

bool isFull(Queue *queue) {
    return (queue->rear == MAX_SIZE - 1);
}

bool isEmpty(Queue *queue) {
    return (queue->rear < queue->front);
}

void enqueue(Queue *queue, QueueItem item) {
    if (isFull(queue)) {
        printf("Queue is full. Unable to enqueue item.\n");
        return;
    }

    queue->rear++;
    queue->items[queue->rear] = item;
}

QueueItem dequeue(Queue *queue) {
    QueueItem item;

    if (isEmpty(queue)) {
        printf("Queue is empty. Unable to dequeue item.\n");
        // You may choose to return a default or special value to indicate the failure
        // item.node = -1; 
        return item;
    }

    item = queue->items[queue->front];
    queue->front++;

    return item;
}

void removeItem(Queue *queue, int index) {
    Queue tempQueue;
    initializeQueue(&tempQueue);

    int i = 1;
    while (!isEmpty(queue)) {
        QueueItem item = dequeue(queue);

        // Check if the item matches the value to be removed
        if (i != index) {
            enqueue(&tempQueue, item);
        }
    }

    // Rebuild the original queue without the removed item
    while (!isEmpty(&tempQueue)) {
        QueueItem item = dequeue(&tempQueue);
        enqueue(queue, item);
    }
}

int printQueue(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return 0;
    }

    int j = 0;
    for (int i = queue->front; i <= queue->rear; i++) {
        printf("%d) %d-%d-%d: %s \n", ++j, queue->items[i].day, queue->items[i].month, queue->items[i].year,queue->items[i].data);
    }
    printf("\n");

    return j;
}