#include <stdio.h>
#include <stdlib.h>
#define SIZE_QUEUE 100	//the maximum size of the news_queue
#define CAPACITY 6		//the capacity of ZNN.com (per sec)
#define NUM_USER 20		//the number of users
#define NUM_LOOP 100	//the number of loops
#define CONTENT_VARIETY  3 //the number of variety of contents


//ContentFidelity is one of the types: video, image, text
typedef enum { Video, Image, Text } ContentFidelity;
//To express constant of ContentFidelity to string
char *enumString[] = { "Video", "Image", "Text" };

typedef struct {
	unsigned int requestedBy;   //the user who requested
	ContentFidelity fidelity;   //content's quality the user requested
} news;

typedef struct {
    news queue[SIZE_QUEUE];
	unsigned int front; //start of queue
	unsigned int rear;  //end of queue
    unsigned int len;   //the number of contents requested in queue
} Queue;

typedef struct {
    unsigned int contentsNum;   //the number of contents user got
    unsigned int score;         //contents' fidelity user got
    unsigned int standbyTime;   //time from request to get
    unsigned int requesting;    //status whether a user is requesting a content now or not  0 == not requesting, 1 == requesting
    unsigned int totalTime;     //sum of standbyTime
}User;

Queue news_queue;
ContentFidelity currentFidelity = Video;
User userInfo[NUM_USER];

/*
User requests a content
*/
void request() {
    printf(">>>>Requesting(ENQUEUE) : ");
    for (int user = 0; user < NUM_USER; user++) {
        //if a user requested already and he or she didn't get one, it can't be requested.
        if (userInfo[user].requesting == 1) {
            continue;
        }
        //a user requests a content at a 50 percent
        if (rand() % 2) {
            printf("%d, ", user);
            news_queue.queue[news_queue.rear].requestedBy = user;
            news_queue.queue[news_queue.rear].fidelity = currentFidelity;
            userInfo[user].requesting = 1;
            
            //if rear reaches end of queue, it goes back to 0(first)
            if (news_queue.rear < SIZE_QUEUE - 1) {
                news_queue.rear++;
            }
            else {
                news_queue.rear = 0;
            }
            news_queue.len++;
        }
    }
    printf("\n");
}

/*
Adjust content's fidelity depending on average contents waiting time
*/
void setFidelity() {
    unsigned int flag = news_queue.front;
    double totalTime = 0;
    double averTime = 0;

    for (int i = 0; i < news_queue.len; i++) {
        totalTime += userInfo[news_queue.queue[flag].requestedBy].standbyTime;

        //if flag reaches end of queue, it goes back to 0(first)
        if (flag < SIZE_QUEUE - 1) {
            flag++;
        }
        else {
            flag = 0;
        }
    }

    //if queue's length is blank, averTime == 0
    if (news_queue.len != 0) {
        averTime = totalTime / news_queue.len;
    }

    //downgrade content's fidelity
    if (averTime >= 3) {
        switch (currentFidelity){
        case Video:
            currentFidelity = Image;
            break;
        default:
            currentFidelity = Text;
            break;
        }
    }
    //upgrade content's fidelity
    else if (averTime <= 2) {
        switch (currentFidelity) {
        case Text:
            currentFidelity = Image;
            break;
        default:
            currentFidelity = Video;
            break;
        }
    }

    printf("Average waiting time: %f, Current Quality: %s\n", averTime, enumString[currentFidelity]);
}

/*
Provide the content to user
*/
void provide() {
    printf("Providing(DEQUEUE): ");
    unsigned int flag;

    //provide the requested content to user up to capacity of Znn.com's server
    for (int i = 0; i < CAPACITY && news_queue.len != 0;) {
        if (i + CONTENT_VARIETY - news_queue.queue[news_queue.front].fidelity <= CAPACITY) {
            printf("(%d, %s), ", news_queue.queue[news_queue.front].requestedBy, enumString[news_queue.queue[news_queue.front].fidelity]);
            
            //add user's informations
            userInfo[news_queue.queue[news_queue.front].requestedBy].score += CONTENT_VARIETY - news_queue.queue[news_queue.front].fidelity;
            userInfo[news_queue.queue[news_queue.front].requestedBy].contentsNum++;
            userInfo[news_queue.queue[news_queue.front].requestedBy].requesting = 0;
            userInfo[news_queue.queue[news_queue.front].requestedBy].standbyTime = 0;
            
            i += CONTENT_VARIETY - news_queue.queue[news_queue.front].fidelity;

            //if front reaches end of queue, it goes back to 0(first)
            if (news_queue.front < SIZE_QUEUE - 1) {
                news_queue.front++;
            }
            else {
                news_queue.front = 0;
            }
            news_queue.len--;
        }
        else {
            break;
        }
    }
    printf("\n");

    flag = news_queue.front;

    //Add waiting time
    for (int i = 0; i < news_queue.len; i++) {
        userInfo[news_queue.queue[flag].requestedBy].standbyTime++;
        userInfo[news_queue.queue[flag].requestedBy].totalTime++;

        //if flag reaches end of queue, it goes back to 0(first)
        if (flag < SIZE_QUEUE - 1) {
            flag++;
        }
        else {
            flag = 0;
        }
    }
}

/*
Print out status of queue
*/
void printQueue() {
    unsigned int flag = news_queue.front;
    printf("In the queue : [");

    for (int i = 0; i < news_queue.len; i++) {
        printf("(%d, %s), ", news_queue.queue[flag].requestedBy, enumString[news_queue.queue[flag].fidelity]);

        //if flag reaches end of queue, it goes back to 0(first)
        if (flag < SIZE_QUEUE - 1) {
            flag++;
        }
        else {
            flag = 0;
        }
    }
    printf("]\n");
}

/*
Print out results of users' average waiting time and average score
*/
void printResult() {
    printf("******Result*****\n");
    printf("User    Avg.wait        Avg.score\n");
    double averWait;
    double averScore;

    for (int i = 0; i < NUM_USER; i++) {
        //if a user didn't get anything, averWait and averScore are zero
        if (userInfo[i].contentsNum == 0) {
            averWait = 0;
            averScore = 0;
        }
        else {
            averWait = (double)userInfo[i].totalTime / userInfo[i].contentsNum;
            averScore = (double)userInfo[i].score / userInfo[i].contentsNum;
        }
        printf("User%d:\t%fs\t%f%\n",  i, averWait, averScore);
    }
}

void main() {
	unsigned int nthLoop;
	for (nthLoop = 0; nthLoop < NUM_LOOP; nthLoop++) {
		printf("\n=======%d seconds======\n", nthLoop);
		//users can request contents every two loops
		if (nthLoop % 2 == 0) {
			setFidelity();
			request();
		}
		printQueue();
		provide();
	}
	printResult();
}