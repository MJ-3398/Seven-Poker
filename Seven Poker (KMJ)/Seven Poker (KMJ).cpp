#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Windows.h>


#define target 100000 // ��ǥ �ݾ�

struct Poker
{
	char form; // s h d c  (������Ʈ ��Ʈ ���̾� Ŭ�ι�)
	int num; // 1~13  ( 1~ ŷ)
};

// ī�� �����

struct Player {
	char name[50];
	Poker hand[7];
	int cardcount;
	int money;
};

// �÷��̾� ���� , ī�� ��

const char* Symbol(char form)
{
	switch (form)
	{
	case 'S':
		return "��";

	case 'C':
		return "��";

	case 'D':
		return "��";

	case 'H':
		return "��";
	}
}

// ī�� ���� ��ȯ

void Deck(Poker deck[])
{
	char form[] = { 'S','H','C','D' };

	int card = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			deck[card].form = form[i];
			deck[card].num = j;
			card++;
		}
	}


}

// �� �����

void Shuffle(Poker deck[])
{
	srand(time(NULL));
	for (int i = 0; i < 52; i++)
	{
		int r = rand() % 52;
		Poker a = deck[i];
		deck[i] = deck[r];
		deck[r] = a;


		//printf("%c%d ", deck[i].form, deck[i].num);
		//if ((i + 1) % 13 == 0)
		//	printf("\n");    ���� �� print�ϴ� ��

	}


}

// �� ����

void Dealing(Poker deck[], Player players[], int playernumber, int* decklndex, int cardnumber, int a, int b)
{
	for (int i = 0; i < playernumber; i++)
	{
		for (cardnumber = a; cardnumber < b; cardnumber++)
		{
			players[i].hand[cardnumber] = deck[(*decklndex)++];
			players[i].cardcount++;
		}
	}
}

// �� �����ֱ�

void Resetround(Player players[], Poker deck[], int* deckIndex, int* total)
{
	*deckIndex = 0;
	*total = 0;

	players[0].cardcount = 0;
	players[1].cardcount = 0;

	Deck(deck);
	Shuffle(deck);
}

// ���� �ʱ�ȭ �Լ�

void Printplayerhands(Player players[], int numPlayers)
{

	printf("%s�� ī��: ", players[1].name);
	for (int j = 0; j < players[1].cardcount; j++)
	{
		const char* sym = Symbol(players[1].hand[j].form);
		if (j < 2 || j == 6)
		{
			printf("�� ");
		}
		else
		{

			printf("%s%d ", sym, players[1].hand[j].num + 1);
		}
	}
	printf("\n\n%s�� ī��: ", players[0].name);
	for (int j = 0; j < players[0].cardcount; j++)
	{
		const char* sym = Symbol(players[0].hand[j].form);

		printf(" %s%d ", sym, players[0].hand[j].num + 1);
	}
	printf("\n");
}


void Lastprint(Player players[], int numPlayers)
{
	for (int i = 0; i < numPlayers; i++)
	{
		printf("%s�� ī��: ", players[i].name);
		for (int j = 0; j < players[i].cardcount; j++)
		{
			const char* sym = Symbol(players[i].hand[j].form);
			printf("%s%d ", sym, players[i].hand[j].num + 1);
		}
		printf("\n");
	}
}

// �� �� �����ֱ� (printPlayerHands / lastprint)

int Confirm(Poker hand[], int cardcount)
{
	int numcount[14] = { 0 }; // ����
	int formcount[4] = { 0 }; // S:0, H:1, D:2, C:3
	int maxcount = 0; // ���� ���� ( 1 1 1 1 )
	int contcount = 0; // ���� ���� ( 1 2 3 4 )
	int straight = 0; // ��Ʈ����Ʈ 

	int pair = 0;
	int triple = 0;

	for (int i = 0; i < cardcount; i++)
	{
		numcount[hand[i].num]++;
		///////////////////////////////////// ���� ���

		if (hand[i].form == 'S')
		{
			formcount[0]++;
		}
		else if (hand[i].form == 'H')
		{
			formcount[1]++;
		}
		else if (hand[i].form == 'D')
		{
			formcount[2]++;
		}
		else if (hand[i].form == 'C')
		{
			formcount[3]++;
		}
		///////////////////////////////////// ��� ���
	}
	for (int i = 0; i <= 13; i++)
	{
		if (numcount[i] > maxcount)
		{
			maxcount = numcount[i];
		}
		///////////////////////////////////// ���� ���� ���� ���
		if (numcount[i] > 0)
		{
			contcount++;
		}
		else
		{
			contcount = 0;
		}
		if (contcount > straight)
		{
			straight = contcount;
		}
		///////////////////////////////////// ��Ʈ����Ʈ�� ���� ���
	}
	for (int i = 0; i < 4; i++)
	{
		if (straight >= 5 && formcount[i] >= 5)
		{
			if (numcount[0] && numcount[10] && numcount[11] && numcount[12] && numcount[13])
			{
				return 10;
			}
			///////////////////////////////////////////////////////// �ξ� ��Ʈ����Ʈ �÷���
			return 9;
		}
		///////////////////////////////////////////////////////// ��Ʈ����Ʈ �÷���
	}
	if (maxcount == 4)
	{
		return 8;
	}
	////////////////////////////////////////////////////// ��ī��
	for (int i = 0; i <= 13; i++)
	{
		if (numcount[i] == 3)
		{
			triple++;
		}
		if (numcount[i] == 2)
		{
			pair++;
		}
	}
	if (triple == 1 && pair == 1)
	{
		return 7;
	}
	////////////////////////////////////////////////////// Ǯ�Ͽ콺
	for (int i = 0; i < 4; i++)
	{
		if (formcount[i] >= 5)
		{
			return 6;
		}
	}
	////////////////////////////////////////////////////// �÷���
	if (straight == 5)
	{
		return 5;
	}
	////////////////////////////////////////////////////// ��Ʈ����Ʈ
	if (maxcount == 3)
	{
		return 4;
	}
	////////////////////////////////////////////////////// Ʈ����
	if (pair >= 2)
	{
		return 3;
	}
	////////////////////////////////////////////////////// �� ���
	if (pair == 1)
	{
		return 2;
	}
	////////////////////////////////////////////////////// �� ���

	return 1;
	////////////////////////////////////////////////////// ������ ���� ���
}

// ���� ����

void Winner(Player players[], int total)
{

	int scores[2]; // ���� �ֱ��

	for (int i = 0; i < 2; i++)
	{
		scores[i] = Confirm(players[i].hand, players[i].cardcount);
	}
	printf("%s�� ����: %d\n", players[0].name, scores[0]);
	printf("%s�� ����: %d\n", players[1].name, scores[1]);

	if (scores[0] > scores[1])
	{
		system("cls");
		printf("%s �¸�. �ǵ� %d���� ȹ���߽��ϴ�.\n\n", players[0].name, total);
		players[0].money += total;
	}
	else if (scores[0] < scores[1])
	{
		system("cls");
		printf("%s �¸�.  AI�� �ǵ� %d���� ȹ���߽��ϴ�.\n\n", players[1].name, total);
		players[1].money += total;
	}
	else
	{
		printf("���º�! �ǵ��� ���������ϴ�.\n\n");
		players[0].money += total / 2;
		players[1].money += total / 2;
	}

}

// ���� ����


void Reset(Player players[], Poker deck[], int* deckIndex, int* total, char* nam)
{
	*deckIndex = 0;
	*total = 0;

	players[0].cardcount = 0;
	players[1].cardcount = 0;

	players[0].money = 1000;
	players[1].money = 100000;

	strcpy_s(players[0].name, nam);
	strcpy_s(players[1].name, "AI");

	Deck(deck);
	Shuffle(deck);
}

// ���� ����۱�

void manual()
{
	int abc = 2;

	while (1)
	{
		printf("1. ���� ����\n2. �� ����\n3. ���� ����\n0. ���� ����\n���ڸ� �Է��Ͻʽÿ�.\n");
		scanf_s("%d", &abc);
		if (abc == 1)
		{
			printf("�� ������..\n");
			printf("�� ������...\n");
			printf("�� ������....\n");
			printf("�� ������.....\n");
			break;
		}
		else if (abc == 2)
		{
			printf("�� �÷��̾�� 7���� ī�尡 ���������� ��е˴ϴ�. \n");
			printf("ó�� 2���� �����(Ȧī��), �� ��° ī��� ����(����ī��)�� ��е˴ϴ�. \n");
			printf(" ù �������� 1000���̸�, �� 100,000���� ����� �¸��մϴ�.\n");
			printf(" �������� ��� ������ �й��մϴ�.\n");
			printf(" 7��° ī�带 ���� ��. \n");
			printf(" ��� ī�带 ���� ��, �÷��̾���� �ڽ��� ���� 7���� ī�� �� 5���� ������ �ְ��� ������ ����ϴ�. \n");
			printf(" ���� ���� ������ ���� �÷��̾ �¸��ϰ� ���� �ݾ��� �������ϴ�. \n");
			printf(" ��Ȳ �Ǵܰ� ���� ������ �߿��մϴ�.\n");
			printf(" ������ ���� ī�带 ���� ������ �����Ͻñ� �ٶ��ϴ�.\n\n\n");

		}
		else if (abc == 0)
		{
			printf("������ �����մϴ�.\n");
			break;
		}
		else if (abc == 3)
		{
			printf("11 = J / 12 = Q / 13 = K \n\n");
			printf("1. �ο� ��Ʈ����Ʈ �÷���(Royal Flush)\n");
			printf(" - ���� ������ A, K, Q, J, 10 \n\n");
			printf("2. ��Ʈ����Ʈ �÷��� (Straight Flush)\n");
			printf(" - ���� ������ ���ӵ� 5���� ī��\n");
			printf(" - ��: 9��, 8��, 7��, 6��, 5��\n\n");
			printf("3. �� ī�� (Four of a Kind)\n");
			printf(" - ���� ������ ī�� 4��\n");
			printf(" - ��: K��, K��, K��, K��\n\n");
			printf("4. Ǯ �Ͽ콺 (Full House)\n");
			printf(" - 3���� ���� ���ڿ� 2���� ���� ���� ����\n");
			printf(" - ��: 10��, 10��, 10��, 5��, 5��\n\n");
			printf("5. �÷��� (Flush)\n");
			printf(" - ���� ������ 5���� ī�� (���ڴ� �������)\n");
			printf(" - ��: A��, Q��, 8��, 6��, 3��\n\n");
			printf("6. ��Ʈ����Ʈ (Straight)\n");
			printf(" - ���ӵ� ������ 5���� ī�� (���̴� �������)\n");
			printf(" - ��: 7��, 6��, 5��, 4��, 3��\n\n");
			printf("7. Ʈ���� (Three of a Kind)\n");
			printf(" - ���� ������ ī�� 3��\n");
			printf(" - ��: Q��, Q��, Q��\n\n");
			printf("8. �� ��� (Two Pair)\n");
			printf(" - 2���� ���� ���� ī�尡 2��\n");
			printf(" - ��: J��, J��, 9��, 9��\n\n");
			printf("9. �� ��� (One Pair)\n");
			printf(" - ���� ������ ī�� 2��\n");
			printf(" - ��: 5��, 5��\n\n");
			printf("10. ���� ī�� (High Card)\n");
			printf("  - �� ������ �ش����� �ʴ� ���� ���� ī��\n");
			printf("  - ��: A�� (���� ���� ī��)\n");
		}
		else
		{
			printf("�߸��� �Է��Դϴ�. �ٽ� �õ��Ͻʽÿ�.\n");
		}
	}
}
// ���� ���� �� ����

int main()
{
	int betmoney = 0;
	int total = 0;
	int deckIndex = 0;
	char nam[40];
	char choice;
	char End;
	printf("�̸��� �ۼ��ϼ���:");
	scanf_s("%s", nam);
	printf("ȯ���մϴ�, %s \n", nam);


	Poker deck[52];
	Player players[2];

	players[0].cardcount = 0;
	players[1].cardcount = 0;

	players[0].money = 1000;
	players[1].money = 100000;

	strcpy_s(players[0].name, nam);
	strcpy_s(players[1].name, "AI");

	Deck(deck);
	Shuffle(deck);


	while (1)
	{
		manual();
		system("cls");

		while (players[0].money > 0 && players[0].money < target && players[1].money > 0)
		{
#pragma region ī�� �� (3)
			Dealing(deck, players, 2, &deckIndex, 3, 0, 3);  // ī�� �� 3

			Printplayerhands(players, 2);
			printf("�����ϼ���.(���� �ݾ� : %d) \n", players[0].money);

			scanf_s("%d", &betmoney);

			if (betmoney > players[0].money)
			{
				printf("���� �ݾ��� �������� �ʰ��߽��ϴ�. �ִ� %d�������� �����մϴ�.\n", players[0].money);
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI�� �����մϴ�...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("���� �ǵ�: %d��\n", total);
#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region ī�� �� (4)
			Dealing(deck, players, 2, &deckIndex, 3, 3, 4); // ī�� �� 4

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("�����ϼ���.(���� �ݾ� : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("���� �Ұ���.\n ���� �ѱ�ϴ�. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("���� �ݾ��� �������� �ʰ��߽��ϴ�. �ִ� %d�������� �����մϴ�.\n", players[0].money);
				printf("ALL IN���� �����մϴ�.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI�� �����մϴ�...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("���� �ǵ�: %d��\n", total);

#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region ī�� �� (5)
			Dealing(deck, players, 2, &deckIndex, 4, 4, 5); // ī�� �� 5

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("�����ϼ���.(���� �ݾ� : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("���� �Ұ���.\n ���� �ѱ�ϴ�. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("���� �ݾ��� �������� �ʰ��߽��ϴ�. �ִ� %d�������� �����մϴ�.\n", players[0].money);
				printf("ALL IN���� �����մϴ�.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI�� �����մϴ�...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("���� �ǵ�: %d��\n", total);

#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region ī�� �� (6)
			Dealing(deck, players, 2, &deckIndex, 5, 5, 6); // ī�� �� 6

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("�����ϼ���.(���� �ݾ� : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("���� �Ұ���.\n ���� �ѱ�ϴ�. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("���� �ݾ��� �������� �ʰ��߽��ϴ�. �ִ� %d�������� �����մϴ�.\n", players[0].money);
				printf("ALL IN���� �����մϴ�.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI�� �����մϴ�...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("���� �ǵ�: %d��\n", total);

#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region ī�� �� (7)
			Dealing(deck, players, 2, &deckIndex, 6, 6, 7); // ī�� �� 7

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("�����ϼ���.(���� �ݾ� : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("���� �Ұ���.\n ���� �ѱ�ϴ�. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("���� �ݾ��� �������� �ʰ��߽��ϴ�. �ִ� %d�������� �����մϴ�.\n", players[0].money);
				printf("ALL IN���� �����մϴ�.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI�� �����մϴ�...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("���� �ǵ�: %d��\n", total);

#pragma endregion

			Winner(players, total);

			Lastprint(players, 2);

			total = 0;

			printf("\n ���� ����� �ڵ� ����˴ϴ�.�����ϰ� �����ôٸ� 'X' Ű�� �����ʽÿ�.\n");
			printf("���� ��...\n");

			getchar();
			End = getchar();
			if (End == 'X' || End == 'x')
			{
				printf("������ �����մϴ�. �����մϴ�.\n");
				return 0;
			}

			Resetround(players, deck, &deckIndex, &total);
			system("cls");
			continue;
		}
		if (players[0].money >= target)
		{
			printf("�����մϴ�! %s���� ��ǥ �ݾ� %d���� �޼��߽��ϴ�.\n", players[0].name, target);
			printf("������ �ٽ� �����Ϸ��� R�� ��������. �����Ϸ��� �ٸ� Ű�� ��������.\n");

			while ((choice = getchar()) == '\n');
			{
				if (choice == 'R' || choice == 'r')
				{
					Reset(players, deck, &deckIndex, &total, nam);

					while (getchar() != '\n');
					continue;
				}
				else
				{
					break;
				}
			}
		}
		else if (players[0].money == 0)
		{
			printf("%s���� �й��߽��ϴ�.�������� 0���� �Ǿ����ϴ�.\n", players[0].name);
			printf("������ �ٽ� �����Ϸ��� R�� ��������. �����Ϸ��� �ٸ� Ű�� ��������.\n");

			while ((choice = getchar()) == '\n');
			{
				if (choice == 'R' || choice == 'r')
				{
					Reset(players, deck, &deckIndex, &total, nam);

					while (getchar() != '\n');
					continue;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			printf("������ �����մϴ�. �����մϴ�!\n");
			break;
		}
	}
	return 0;
}
