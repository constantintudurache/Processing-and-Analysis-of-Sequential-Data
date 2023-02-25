#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include <math.h>
typedef struct Data{
    unsigned int timestamp;
    float value;
}Data;

typedef struct Node{
    Data data;
    struct Node *next, *prev;
}Node;

typedef struct Filter{
    float value;
    struct Filter *next;
}Filter;

typedef struct Stats{
    int infLimit, elements;
    struct Stats *next;
}Stats;

void isNull(void *ptr){
    if(ptr == NULL){
        puts("allocation failure");
        exit(1);
    }
}

char* charAlloc(int lenght){
    char *str = NULL;
    str = malloc(lenght * sizeof(char));
    isNull(str);
    return str;
}

void addFilterNode(Filter **head, float value){
    Filter *newNode = malloc(sizeof(Filter));

    if(*head == NULL){
        newNode->value = value;
        newNode->next = NULL;
        *head = newNode;
    }
    else{
        Filter *headcopy = *head;
        while(headcopy->next != NULL)
            headcopy = headcopy->next;
        newNode->value = value;
        newNode->next = NULL;
        headcopy->next = newNode;
    }
}

void printFilter(Filter *head){
    while(head != NULL){
        printf("%0.2f\n",head->value);
        head = head->next;
    }
}

void addNode(Node **head, Data data){
    Node* newNode = malloc(sizeof(Node)), *headcopy = *head;
    isNull(newNode);
    if(*head == NULL){
        newNode->data = data;
        newNode->next = NULL;
        newNode->prev = NULL;
        *head = newNode;
    }
    else{
        while(headcopy->next != NULL)
            headcopy = headcopy->next;

        newNode->data = data;
        newNode->next = NULL;
        headcopy->next = newNode;
        newNode->prev = headcopy;
    }

}

Node* generateList( int *capacity){
    Data data;
    Node* head = NULL;
    scanf("%d",capacity);

    for(int i = 1; i <= *capacity; i++){
        scanf("%u%f",&data.timestamp,&data.value);
        addNode(&head,data);
    }

    return head;
}

Node* fileGenerateList(char *filename, int *capacity){
    FILE *input = fopen(filename,"r");
    Data data;
    Node* head = NULL;
    isNull(input);
    fscanf(input,"%d",capacity);

    for(int i = 1; i <= *capacity; i++){
        fscanf(input,"%u%f",&data.timestamp,&data.value);
        addNode(&head,data);
    }

    return head;
}

void deleteList(Node **head){
    Node *current = *head, *next = NULL;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

void printList(Node *head, int capacity){
    printf("%d\n",capacity);
    while(head != NULL){
        printf("%u %0.2f\n",head->data.timestamp,head->data.value);
        head = head->next;
    }

}

float average(Node *headRef){
    float sum = 0;
    for(int i = 1; i <= 5; i++){
        sum += headRef->data.value;
        headRef = headRef->next;
    }
    return sum/5;
}

float auxSum(Node *headRef, float avg){
    float sum = 0;
    for(int i = 1; i <= 5; i++){
        sum += ((headRef->data.value - avg)*(headRef->data.value - avg));
        headRef = headRef->next;
    }
    return sum;
}

float deviation(Node *headRef, float avg){
    float dvt;
    dvt = auxSum(headRef,avg);
    dvt /= 5;
    return sqrt(dvt);
}

int isValid(Node *ptr, int counter){

    for(int i = 1; i <= counter; i++){
        if(ptr == NULL)
            return 0;
        ptr = ptr->next;
    }
    return 1;
}

void deleteNode(Node *address){

    if(address == NULL)
        return;
    address->next->prev = address->prev;
    address->prev->next = address->next;
    free(address);
}

void eraseException(Node *headRef, int *capacity){

    float avg, dvt;
    if(isValid(headRef,5) == 0)
        return;
    while(headRef != NULL){
        if(isValid(headRef,5)){
            avg = average(headRef);
            dvt = deviation(headRef,avg);
            if(headRef->next->next->data.value < (avg - dvt) || headRef->next->next->data.value > (avg + dvt)){
                deleteNode(headRef->next->next);
                (*capacity)--;
            }

        }
        headRef = headRef->next;
    }
}

Node* kMin(Node *headRef, int k){
    int counter = 0;
    Node  *headcopy = headRef, *adr = NULL;
    float prevMin = FLT_MAX , actualMin;

    while(counter < k){
        headcopy = headRef;
        actualMin = FLT_MAX;
        while(headcopy->next != NULL){//printf("a=%0.2f, p=%0.2f\n",actualMin,prevMin);
            if(headcopy->data.value < actualMin && prevMin == FLT_MAX){
                actualMin = headcopy->data.value;
                adr = headcopy;
            }
            else if(headcopy->data.value < actualMin && headcopy->data.value > prevMin){
                actualMin = headcopy->data.value;
                adr = headcopy;
            }
            headcopy = headcopy->next;
        }
        counter++;

        prevMin = actualMin;
    }

    return adr;
}

Filter* midFilter(Node *headRef){
    Node *aux;
    Filter *head = NULL;
    while(isValid(headRef,5)){
        aux = kMin(headRef,3);
        addFilterNode(&head,aux->data.value);
        headRef = headRef->next;
    }
    return head;
}

Filter* avgFilter(Node *headRef){
    Filter *head = NULL;
    while(isValid(headRef,5)){
        addFilterNode(&head,average(headRef));
        headRef = headRef->next;
    }
    return head;
}

void timeFrequencyUnif(Node* headRef){
    if(headRef->next == NULL)
        return;
    headRef = headRef->next;
    while(isValid(headRef,1)){
        if(headRef->data.timestamp - headRef->prev->data.timestamp >= 100 && headRef->data.timestamp - headRef->prev->data.timestamp <= 1000){
            headRef->data.timestamp = (headRef->data.timestamp + headRef->prev->data.timestamp)/2;
            headRef->data.value = (headRef->data.value + headRef->prev->data.value)/2;
        }
        headRef = headRef->next;
    }
}

float sigma(int k){
    float sum = 0;
    for(int j = 0; j < k; j++){
        sum  += ((1. * j / (k-1)) * (1. * j / (k-1)) * 0.9 + 0.1);
    }
    return sum;
}

float w(int i, int k){
    return ((1. * i / (k-1)) * (1. * i / (k-1)) * 0.9 + 0.1)/sigma(k);
}

int prevIsValid(Node* ptr, int counter){
    for(int i = 1; i <= counter; i++){
        if(ptr == NULL)
            return 0;
        ptr = ptr->prev;
    }
    return 1;
}

Node* generateLeft(Node *headRef){
    if(prevIsValid(headRef,3) == 0)
        return NULL;
    Node *left = NULL;
   headRef = headRef->prev->prev;
   for(int i = 1; i <= 3; i++){
    addNode(&left,headRef->data);
    headRef = headRef->next;
   }
    return left;
}

Node* generateRight(Node *headRef){
    if(isValid(headRef,3) == 0)
        return NULL;
    Node *right = NULL;
    headRef = headRef->next->next;
    for(int i = 1; i <= 3; i++){
        addNode(&right,headRef->data);
        headRef = headRef->prev;
    }
    return right;
}

float C(unsigned int timestamp, Node *left, Node* right){
    return (1. * (timestamp - left->next->next->data.timestamp) / (right->next->next->data.timestamp - left->next->next->data.timestamp));
}

float rightSum(Node *right, int i, int k){
    float sum = 0;
    for(int j = i; j <k; j++){
        sum += right->data.value * w(j,k);
        right = right->next;
    }

    return sum;
}

float leftSum(Node *left, int i, int k){
    float sum = 0;
    for(int j = i; j < k; j++){
        sum += left->data.value * w(j,k);
        left = left->next;
    }

    return sum;
}

float f(Node *left, Node *right, unsigned int timestamp){
    return (1-C(timestamp,left,right)) * leftSum(left,0,3) + C(timestamp,left,right) * rightSum(right,0,3);
}

void dataFill(Node *headRef, int i, int k, int *capacity){

    if(headRef == NULL || headRef->next == NULL)
        return;
    headRef = headRef->next;
    while(isValid(headRef,1)){
        if(headRef->data.timestamp - headRef->prev->data.timestamp > 1000){
            while(headRef->data.timestamp - headRef->prev->data.timestamp > 200){
                Node *right = NULL, *left = NULL, *newNode = malloc(sizeof(Node));
                isNull(newNode);

                if(prevIsValid(headRef->prev,3) == 0)
                    continue;

                left = generateLeft(headRef->prev);
                isNull(left);

                if(isValid(headRef,3) == 0)
                    break;

                right = generateRight(headRef);
                isNull(right);

                headRef = headRef->prev;
                newNode->data.value = f(left,right,headRef->data.timestamp + 200);
                newNode->data.timestamp = headRef->data.timestamp + 200;
                headRef->next->prev = newNode;
                newNode->next = headRef->next;
                headRef->next = newNode;
                newNode->prev = headRef;
                (*capacity)++;
                headRef = newNode->next;
                deleteList(&right);
                deleteList(&left);
            }
        }
        headRef = headRef->next;
    }
    
}

int setStatsElements(Node *headRef, int infLim, int delta){
    int counter = 0;
    if(headRef == NULL)
        return 0;

        while(headRef != NULL){
            if(headRef->data.timestamp >= infLim && headRef->data.timestamp < infLim + delta)
                counter++;
            headRef = headRef->next;
        }
    return counter;
}

int firstCondition(Stats *stat1, Stats *stat2){
    if(stat1->elements > stat2->elements)
        return 1;
    else if(stat1->elements < stat2->elements)
        return 0;
    return -1;
}

int secondCondition(Stats *stat1, Stats *stat2){
    if(stat1->infLimit < stat2->infLimit)
        return 1;
   return 0;
}

void addStat(Node *listHead, Stats **statHead, int infLim, int elements, int delta){
    Stats *newStat = malloc(sizeof(Stats)), *aux;

    if(*statHead == NULL)
    {
        newStat->elements = elements;
        if(newStat->elements == 0)
            return;
        newStat->infLimit = infLim;
        newStat->next = NULL;
        *statHead = newStat;
    }
    else{
        newStat->elements = elements;
        if(newStat->elements == 0)
            return;
        newStat->infLimit = infLim;
        if(firstCondition(newStat,*statHead) == 1){
            newStat->next = *statHead;
            *statHead = newStat;
        }
        else if(firstCondition(newStat,*statHead) == 0){
            aux = *statHead;
            while(aux->next != NULL && firstCondition(newStat,aux->next) != 1 )
                aux = aux->next;
            newStat->next = aux->next;
            aux->next = newStat;
        } else if(firstCondition(newStat,*statHead) == -1){
            if(secondCondition(newStat,*statHead)){
                newStat->next = *statHead;
                *statHead = newStat;
            }else{
                aux = *statHead;
                while(aux->next != NULL && secondCondition(newStat,aux->next) == 0 && firstCondition(newStat,aux->next) != 1)
                    aux = aux->next;
                newStat->next = aux->next;
                aux->next = newStat;
            }
        }
    }
}

Stats* statistics(Node *listHead,  int delta, int capacity){

    int counter = 0, elements;
    Stats *statHead = NULL;

    for(int i = 0; counter < capacity; i += delta){
        elements = setStatsElements(listHead,i,delta);
        if(elements == 0)
            continue;

        addStat(listHead,&statHead,i,elements,delta);
        counter += elements;
    }
    return statHead;

}

void deleteStats(Stats **statsHead){
    Stats *current = *statsHead, *next = NULL;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    *statsHead = NULL;
}

void printStats(Stats *head, int delta){

    isNull(head);
    while(head != NULL){
        printf("[%d,%d] %d\n",head->infLimit,head->infLimit + delta, head->elements);
        head = head->next;
    }
}

int isCommand(char *str){

    char c[6][3] = { {"e1"},{"e2"},{"e3"},{"u"},{"c"},{"st"}};
    int ok = 0;
    for(int i = 0; i < 5; i++){
        if(strcmp(str,c[i]) == 0)
            ok = 1;
    }
    if(strstr(str,c[5]))
        ok = 1;
    return ok;
}

int setDelta(char *str){
    int delta;
    char *s= str +2;
    delta = atoi(str+2);
    return delta;
}

int main(int argc, char *argv[]){

    int capacity, i;
    Node *listHead = NULL;


    /*strcpy(buf,"test0.in");
    input = charAlloc(strlen(buf));
    isNull(input);
    strcpy(input,buf);*/
    //listHead = generateList(&capacity);
    listHead = fileGenerateList("test4_gaps10k",&capacity);
    //printList(listHead,capacity);
    //puts("----");
    //argv[0] = ./tema1
    argv[1] = "e1";
    argv[2] = "e2";
    argv[3] = "e3";
    argv[4] = "u";
    argv[5] = "c";
    argv[6] = "st1000";

    for(i = 1; i <= 6; i++){
        if(isCommand(argv[i]) == 0)
            break;
        if(strcmp(argv[i],"e1") == 0){
            eraseException(listHead,&capacity);
            printList(listHead,capacity);
            //puts("-----");
            continue;
        }

        if(strcmp(argv[i],"e2") == 0){
            Filter *filterHead = NULL;
            filterHead = midFilter(listHead);
            //puts("mid filter");
            printFilter(filterHead);
            //puts("-----");
            continue;
        }

        if(strcmp(argv[i],"e3") == 0){
            Filter *filterHead = NULL;
            filterHead = avgFilter(listHead);
            //puts("avg filter");
            printFilter(filterHead);
            //puts("-----");
            continue;
        }

        if(strcmp(argv[i],"u") == 0){
              timeFrequencyUnif(listHead);
              printList(listHead,capacity);
              //puts("-----");;
            continue;
        }

        if(strcmp(argv[i],"c") == 0){
            dataFill(listHead,0,3,&capacity);
            printList(listHead,capacity);
            //puts("aici-----");
            continue;
        }

        if(strstr(argv[i],"st")){
            int delta = setDelta(argv[i]);
            Stats *statsHead = NULL;
            statsHead = statistics(listHead,delta,capacity);
            printStats(statsHead,delta);
            deleteStats(&statsHead);
            //puts("-----");
        }

    }
   //printList(listHead,capacity);
    deleteList(&listHead);
    return 0;
}
