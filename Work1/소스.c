#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct { // ����� ������ �ʵ�
	int id;
	int primeNumber;
	int arrival_time[2];
	int service_time;
}element;

typedef struct Atmqueue{// ATM���
	element data;
	struct Atmqueue* link;
}Atm; 

typedef struct { // ��ũ�忡Ƽ��Ÿ�� �� ťŸ�� 
	Atm* front, * rear;
	int leng, service_time;
	int getout_customer;
}LinkedAtmType;


void init_atmqueue(LinkedAtmType* q) { // �ʱ�ȭ �Լ�
	q->front = NULL;
	q->rear = NULL;
	q->leng = 0;
	q->service_time = 0;
	q->getout_customer = 0;
}

int is_empty_Atm(LinkedAtmType* q) { // ����ִ��� Ȯ�� �Լ� ���̸� ���� Ȯ��
	return (q->leng == 0);		
}

void enqueue_Atm(LinkedAtmType* q, element data) { //�Ϲ����� ��ť
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

element dequeue_Atm(LinkedAtmType* q) { // �Ϲ����� ��ť
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

void dequeuefree_Atm(LinkedAtmType* q) { // ��ť�ε� �������ʵ带 ��ȯ���ϴ� �Լ�-> �������� free�� �� ���
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


int sumAnd_delete_primenumber(LinkedAtmType* q, int* time) { // �Ҽ����ſ� �ð��� �����ִ� �Լ� �ð��迭�� �̸��� �� �ּҸ� ���ڷ� ����
	if (is_empty_Atm(q)) return 0;
	int sum = 0;
	Atm* p= q->front;
	Atm* pre = NULL;
	Atm* temp;
	while (p != q->rear) { // p�� ť�� ��� �ƴҶ� ���� 
		if (p->data.primeNumber == 1) {
			if (p->data.arrival_time[0] == time[0])    // hour�� ���� ��� �׳� �E�� �ٸ� ��� 60�� ���� ����
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
		} // pre��� ���� ������ ���� ���ؼ� �߰��� �ִ� ���� rear�� ����Ű�� ��带 ������ ���
	}
	if (p->data.primeNumber == 1) {
		if (p->data.arrival_time[0] == time[0])    // hour�� ���� ��� �׳� �E�� �ٸ� ��� 60�� ���� ����
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

void vip_insert(LinkedAtmType* q, element data) { // vip�� �Ǿտ� ���� �ϴ� �Լ�
	Atm* temp = (Atm*)malloc(sizeof(Atm));
	if (temp == NULL) exit(1);
	temp->data = data; //������ ����
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

int is_prime(int number) { // �Ҽ� Ȯ���Լ�
	if (number <= 1) return 0;
	if (number == 2) return 1;
	for (int i = 2; i < number; i++) {
		if (number % i == 0) return 0;
	}
	return 1;
}
// ���̰� ���� �۰ų�, atm��Ⱑ ��� �ִ� ���, ���̰� ���� ���� ���� ���ؼ� �������
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

	LinkedAtmType atmq1; // 3���� ť ����
	LinkedAtmType atmq2;
	LinkedAtmType atmq3;

	init_atmqueue(&atmq1); // 3���� ť �ʱ�ȭ
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
			LinkedAtmType* min_q = minimum_lengthq(&atmq1, &atmq2, &atmq3); // �ּҰ��̳� ���� ���� ATM���� �ּҰ��� min_q�� ��ȯ

			if (vip_number == minutes % 10 ) { //10�ۼ�Ʈ Ȯ���� ������� �ٿ� ���� �Ǿ�
				vip_insert(min_q, customer); 
			}
			else
				enqueue_Atm(min_q, customer);
			if ((&atmq1)->service_time > 0) { // ���� ATM�⸦ ����ϴ� ���� �ִٴ� �ǹ�
				(&atmq1)->service_time--;
				if ((&atmq1)->service_time == 0) total_customers++; 
			}
			else {
				if (!is_empty_Atm(&atmq1)) { //ť�� ����� ���°� �ƴ� �� ��� ť�� NULL�� �ƴҶ� = ���̰� 0�� �ƴҶ�
					element data = dequeue_Atm(&atmq1);// dequeue
					(&atmq1)->service_time = data.service_time;
					if (hour == data.arrival_time[0]) { // �ð��� ���� time[0] = hour �ϰ�� -> �׳� ���� ������ �ٸ���� 60�� ���ؼ� ����
						total_wait += minutes - data.arrival_time[1];
					}
					else
						total_wait += minutes +60 - data.arrival_time[1];
					printf("%d�� %d�� - ", hour, minutes);
					printf("%d�� ��(%d�� �ҿ�) 1�� ATM���� ���񽺸� ������\n", data.id, data.service_time);
				}
			}
			if ((&atmq2)->service_time > 0) {// ���� ATM�⸦ ����ϴ� ���� �ִٴ� �ǹ�
				(&atmq2)->service_time--;
				if ((&atmq2)->service_time == 0) total_customers++;
			}
			else {
				if (!is_empty_Atm(&atmq2)) {//ť�� ����� ���°� �ƴ� �� ��� ť�� NULL�� �ƴҶ� = ���̰� 0�� �ƴҶ�
					element data = dequeue_Atm(&atmq2);// dequeue
					(&atmq2)->service_time = data.service_time;
					if (hour == data.arrival_time[0]) { // �ð��� ���� time[0] = hour �ϰ�� -> �׳� ���� ������ �ٸ���� 60�� ���ؼ� ����
						total_wait += minutes - data.arrival_time[1];
					}
					else
						total_wait += minutes + 60 - data.arrival_time[1];
					printf("%d�� %d�� - ", hour, minutes);
					printf("%d�� ��(%d�� �ҿ�) 2�� ATM���� ���񽺸� ������\n", data.id, data.service_time);
				}
			}
			if ((&atmq3)->service_time > 0) {// ���� ATM�⸦ ����ϴ� ���� �ִٴ� �ǹ�
				(&atmq3)->service_time--;
				if ((&atmq3)->service_time == 0) total_customers++;
			}
			else {
				if (!is_empty_Atm(&atmq3)) {//ť�� ����� ���°� �ƴ� �� ��� ť�� NULL�� �ƴҶ� = ���̰� 0�� �ƴҶ� 
					element data = dequeue_Atm(&atmq3);// dequeue
					(&atmq3)->service_time = data.service_time;
					if (hour == data.arrival_time[0]) {// �ð��� ���� time[0] = hour �ϰ�� -> �׳� ���� ������ �ٸ���� 60�� ���ؼ� ����
						total_wait += minutes - data.arrival_time[1];
					}
					else
						total_wait += minutes + 60 - data.arrival_time[1];
					printf("%d�� %d�� - ", hour, minutes);
					printf("%d�� ��(%d�� �ҿ�) 3�� ATM���� ���񽺸� ������\n", data.id, data.service_time);
				}
			}

			int array[2]; array[0] = hour; array[1] = minutes; // �ð��� �迭 �ε����� 0 �� 1 ��
			if ((hour == 9 && minutes != 0 && minutes % 10 == 0)|| (hour == 10 && minutes % 10 == 0) ) { // 9�� ������ ������ 10�� �������� ����
				if(!is_empty_Atm(&atmq1))
					total_wait += sumAnd_delete_primenumber(&atmq1, array); //sum�� ���Ϲ޾� total wait�� �߰�
				if (!is_empty_Atm(&atmq2))
					total_wait += sumAnd_delete_primenumber(&atmq2, array);//sum�� ���Ϲ޾� total wait�� �߰�
				if (!is_empty_Atm(&atmq3))
					total_wait += sumAnd_delete_primenumber(&atmq3, array);//sum�� ���Ϲ޾� total wait�� �߰�
			}
			
			
		}

	}
	printf("�� %d���� ������ ���Ҵ�.\n", total_customers); // 1�� ��  1���� ������ �Ϸ��� ������ ������� ���� ���Ѵ�.(������ �����ִ� ��� ����)
	printf("������ ���ų� �ٿ��� ��Ż�� ������� ��� %d�� ��ٷȴ�\n", total_wait / (total_customers
		+ (&atmq1)->getout_customer + (&atmq2)->getout_customer + (&atmq3)->getout_customer) ); // 2�� ��
	printf("�ٿ��� �������� ����� %d ���̴�\n", (&atmq1)->getout_customer + (&atmq2)->getout_customer + (&atmq3)->getout_customer);
	

	// �ð��� �����µ� ���� �ִ°�� �����Ҵ� ����
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
	printf("���� ��ٸ��� �ִ� ����� %d���̴�.\n", wait_customer);//(3�� ��)
	return 0;
}
