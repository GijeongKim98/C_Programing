#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct { // 노드의 데이터 필드
	int id;
	int primeNumber;
	int arrival_time[2];
	int service_time;
}element;

typedef struct Atmqueue{// ATM노드
	element data;
	struct Atmqueue* link;
}Atm; 

typedef struct { // 링크드에티엠타입 즉 큐타입 
	Atm* front, * rear;
	int leng, service_time;
	int getout_customer;
}LinkedAtmType;


void init_atmqueue(LinkedAtmType* q) { // 초기화 함수
	q->front = NULL;
	q->rear = NULL;
	q->leng = 0;
	q->service_time = 0;
	q->getout_customer = 0;
}

int is_empty_Atm(LinkedAtmType* q) { // 비워있는지 확인 함수 길이를 보고 확인
	return (q->leng == 0);		
}

void enqueue_Atm(LinkedAtmType* q, element data) { //일반적인 인큐
	Atm* temp = NULL;
	temp = (Atm*)malloc(sizeof(Atm));
	if (temp == NULL) exit(1);
	temp->data = data;
	temp->link = NULL;
	if (is_empty_Atm(q)) {
		q->front = temp;
		q->rear = temp;
	}
	
	else {
		q->rear->link = temp;
		q->rear = temp;
	}
	q->leng++;
}

element dequeue_Atm(LinkedAtmType* q) { // 일반적인 디큐
	Atm* temp = q->front;
	element data;
	if (is_empty_Atm(q)) exit(1);
	else {
		data = temp->data;
		q->front = q->front->link;
		if (q->front == NULL)
			q->rear = NULL;
		free(temp);
		q->leng--;
		return data;
	}
}

void dequeuefree_Atm(LinkedAtmType* q) { // 디큐인데 데이터필드를 반환안하는 함수-> 마지막에 free할 때 사용
	Atm* temp = q->front;
	if (is_empty_Atm(q)) exit(1);
	else {
		q->front = q->front->link;
		if (q->front == NULL){
			q->rear = NULL;
		}		
		free(temp);
	}
}


int sumAnd_delete_primenumber(LinkedAtmType* q, int* time) { // 소수제거와 시간을 더해주는 함수 시간배열의 이름값 즉 주소를 인자로 받음
	if (is_empty_Atm(q)) return 0;
	int sum = 0;
	Atm* p= q->front;
	Atm* pre = NULL;
	Atm* temp;
	while (p != q->rear) { // p가 큐의 리어가 아닐때 까지 
		if (p->data.primeNumber == 1) {
			if (p->data.arrival_time[0] == time[0])    // hour가 같을 경우 그냥 뺼셈 다를 경우 60을 더해 뺄셈
				sum += time[1] - p->data.arrival_time[1];
			else sum += time[1] + 60 - p->data.arrival_time[1]; 

			if (p == q->front) {
				q->front = p->link;
				temp = p;
				p = p->link;
				free(temp);
				q->leng--;
				q->getout_customer++;
				
			}
			else {
				pre->link = p->link;
				temp = p;
				p = p->link;
				free(temp);
				q->leng--;
				q->getout_customer++;
			}
		}
		else {
			pre = p;
			p = p->link;
		} // pre라는 이전 데이터 값을 구해서 중간에 있는 노드와 rear가 가리키는 노드를 삭제시 사용
	}
	if (p->data.primeNumber == 1) {
		if (p->data.arrival_time[0] == time[0])    // hour가 같을 경우 그냥 뺼셈 다를 경우 60을 더해 뺄셈
			sum += time[1] - p->data.arrival_time[1];
		else sum += time[1] + 60 - p->data.arrival_time[1]; // 60

		if (p == q->front) {
			q->front = NULL;
			q->rear = NULL;
			free(p);
			q->leng--;
			q->getout_customer++;
			return sum;

		}
		else if (pre->link == p) {
			q->rear = pre;
			free(p);
			q->leng--;
			q->getout_customer++;
		}
	}
	return sum;
}

void vip_insert(LinkedAtmType* q, element data) { // vip는 맨앞에 서게 하는 함수
	Atm* temp = (Atm*)malloc(sizeof(Atm));
	if (temp == NULL) exit(1);
	temp->data = data; //데이터 저장
	temp->link = NULL;
	if (q->front == NULL && q->rear == NULL) {
		q->front = temp;
		q->rear = temp;
	}
	else {
		temp->link = q->front;
		q->front = temp;
	}
	q->leng++;
}

int is_prime(int number) { // 소수 확인함수
	if (number <= 1) return 0;
	if (number == 2) return 1;
	for (int i = 2; i < number; i++) {
		if (number % i == 0) return 0;
	}
	return 1;
}
// 길이가 가장 작거나, atm기기가 비워 있는 경우, 길이가 같고 작은 값에 대해선 난수사용
LinkedAtmType* minimum_lengthq(LinkedAtmType* q1, LinkedAtmType* q2, LinkedAtmType* q3) { 
	if (q1->front == NULL && q1->rear == NULL && q1->service_time == 0) return q1;
	else if (q2->front == NULL && q2->rear == NULL && q2->service_time == 0) return q2;
	else if (q3->front == NULL && q3->rear == NULL && q3->service_time == 0) return q3;
	else {
		int arr[3];
		int min = q1->leng;
		arr[0] = q1->leng;
		arr[1] = q2->leng;
		arr[2] = q3->leng;
		if (q1->leng == q2->leng) {
			if (q1->leng == q3->leng) min = arr[rand() % 3];
			else if (q1->leng < q3->leng) min = arr[rand() % 2];
			else min = arr[2];
		}
		else if (q1->leng < q2->leng) {
			if (q1->leng == q3->leng) {
				int k = rand() % 2;
				if (k == 1) min = arr[k + 1];
				else min = arr[k];
			}
			else if (q1->leng < q3->leng) min = arr[0];
			else arr[2];
		}
		else {
			if (q2->leng > q3->leng) min = arr[2];
			else if (q2->leng < q3->leng) min = arr[1];
			else min = arr[(rand() % 2) + 1];
		}
		if (min == q1->leng) return q1;
		else if (min == q2->leng) return q2;
		else  return q3;
	}
}


int main(void) {
	int hour;
	int minutes = 0;
	int total_wait = 0;
	int total_customers = 0;
	int wait_customer = 0;
	int total_getout_customer = 0;

	LinkedAtmType atmq1; // 3개의 큐 선언
	LinkedAtmType atmq2;
	LinkedAtmType atmq3;

	init_atmqueue(&atmq1); // 3개의 큐 초기화
	init_atmqueue(&atmq2);
	init_atmqueue(&atmq3);

	srand((unsigned)time(NULL));
	for (hour = 9; hour < 11; hour++) { 
		int vip_number = rand() % 10;
		for (minutes = 0; minutes < 60; minutes++) {

			element customer;
			customer.id = minutes;
			customer.arrival_time[0] = hour;
			customer.arrival_time[1] = minutes;
			customer.primeNumber = is_prime(minutes);
			customer.service_time = (rand() % 9) + 2;
			LinkedAtmType* min_q = minimum_lengthq(&atmq1, &atmq2, &atmq3); // 최소값이나 고객이 없는 ATM기의 주소값을 min_q로 반환

			if (vip_number == minutes % 10 ) { //10퍼센트 확률로 브아이피 줄에 입장 맨앞
				vip_insert(min_q, customer); 
			}
			else
				enqueue_Atm(min_q, customer);
			if ((&atmq1)->service_time > 0) { // 현재 ATM기를 사용하는 고객이 있다는 의미
				(&atmq1)->service_time--;
				if ((&atmq1)->service_time == 0) total_customers++; 
			}
			else {
				if (!is_empty_Atm(&atmq1)) { //큐가 비워진 상태가 아닐 때 사용 큐가 NULL이 아닐때 = 길이가 0이 아닐때
					element data = dequeue_Atm(&atmq1);// dequeue
					(&atmq1)->service_time = data.service_time;
					if (hour == data.arrival_time[0]) { // 시간에 대해 time[0] = hour 일경우 -> 그냥 뺄셈 하지만 다를경우 60을 더해서 뺄셈
						total_wait += minutes - data.arrival_time[1];
					}
					else
						total_wait += minutes +60 - data.arrival_time[1];
					printf("%d시 %d분 - ", hour, minutes);
					printf("%d번 고객(%d분 소요) 1번 ATM에서 서비스를 시작함\n", data.id, data.service_time);
				}
			}
			if ((&atmq2)->service_time > 0) {// 현재 ATM기를 사용하는 고객이 있다는 의미
				(&atmq2)->service_time--;
				if ((&atmq2)->service_time == 0) total_customers++;
			}
			else {
				if (!is_empty_Atm(&atmq2)) {//큐가 비워진 상태가 아닐 때 사용 큐가 NULL이 아닐때 = 길이가 0이 아닐때
					element data = dequeue_Atm(&atmq2);// dequeue
					(&atmq2)->service_time = data.service_time;
					if (hour == data.arrival_time[0]) { // 시간에 대해 time[0] = hour 일경우 -> 그냥 뺄셈 하지만 다를경우 60을 더해서 뺄셈
						total_wait += minutes - data.arrival_time[1];
					}
					else
						total_wait += minutes + 60 - data.arrival_time[1];
					printf("%d시 %d분 - ", hour, minutes);
					printf("%d번 고객(%d분 소요) 2번 ATM에서 서비스를 시작함\n", data.id, data.service_time);
				}
			}
			if ((&atmq3)->service_time > 0) {// 현재 ATM기를 사용하는 고객이 있다는 의미
				(&atmq3)->service_time--;
				if ((&atmq3)->service_time == 0) total_customers++;
			}
			else {
				if (!is_empty_Atm(&atmq3)) {//큐가 비워진 상태가 아닐 때 사용 큐가 NULL이 아닐때 = 길이가 0이 아닐때 
					element data = dequeue_Atm(&atmq3);// dequeue
					(&atmq3)->service_time = data.service_time;
					if (hour == data.arrival_time[0]) {// 시간에 대해 time[0] = hour 일경우 -> 그냥 뺄셈 하지만 다를경우 60을 더해서 뺄셈
						total_wait += minutes - data.arrival_time[1];
					}
					else
						total_wait += minutes + 60 - data.arrival_time[1];
					printf("%d시 %d분 - ", hour, minutes);
					printf("%d번 고객(%d분 소요) 3번 ATM에서 서비스를 시작함\n", data.id, data.service_time);
				}
			}

			int array[2]; array[0] = hour; array[1] = minutes; // 시간의 배열 인덱스가 0 시 1 분
			if ((hour == 9 && minutes != 0 && minutes % 10 == 0)|| (hour == 10 && minutes % 10 == 0) ) { // 9시 정각을 제외한 10분 간격으로 수행
				if(!is_empty_Atm(&atmq1))
					total_wait += sumAnd_delete_primenumber(&atmq1, array); //sum을 리턴받아 total wait에 추가
				if (!is_empty_Atm(&atmq2))
					total_wait += sumAnd_delete_primenumber(&atmq2, array);//sum을 리턴받아 total wait에 추가
				if (!is_empty_Atm(&atmq3))
					total_wait += sumAnd_delete_primenumber(&atmq3, array);//sum을 리턴받아 total wait에 추가
			}
			
			
		}

	}
	printf("총 %d명이 업무를 보았다.\n", total_customers); // 1번 답  1번은 업무를 완료한 상태의 사람들의 수를 말한다.(업무를 보고있는 사람 제외)
	printf("업무를 보거나 줄에서 이탈한 사람들은 평균 %d분 기다렸다\n", total_wait / (total_customers
		+ (&atmq1)->getout_customer + (&atmq2)->getout_customer + (&atmq3)->getout_customer) ); // 2번 답
	printf("줄에서 빠져나간 사람은 %d 명이다\n", (&atmq1)->getout_customer + (&atmq2)->getout_customer + (&atmq3)->getout_customer);
	

	// 시간이 끝났는데 줄이 있는경우 동적할당 해제
	while ((&atmq1)->front != NULL) {
		dequeuefree_Atm(&atmq1);
		wait_customer++;
	}
	while ((&atmq2)->front != NULL ) {
		dequeuefree_Atm(&atmq2);
		wait_customer++;
	}
	while ((&atmq3)->front != NULL ) {
		dequeuefree_Atm(&atmq3);
		wait_customer++;
	}
	printf("현재 기다리고 있는 사람은 %d명이다.\n", wait_customer);//(3번 답)
	return 0;
}
