#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Windows.h>


#define target 100000 // 목표 금액

struct Poker
{
	char form; // s h d c  (스페이트 하트 다이아 클로버)
	int num; // 1~13  ( 1~ 킹)
};

// 카드 만들기

struct Player {
	char name[50];
	Poker hand[7];
	int cardcount;
	int money;
};

// 플레이어 생성 , 카드 수

const char* Symbol(char form)
{
	switch (form)
	{
	case 'S':
		return "♠";

	case 'C':
		return "♥";

	case 'D':
		return "◆";

	case 'H':
		return "♣";
	}
}

// 카드 형태 변환

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

// 덱 만들기

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
		//	printf("\n");    섞인 덱 print하는 법

	}


}

// 덱 섞기

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

// 패 나눠주기

void Resetround(Player players[], Poker deck[], int* deckIndex, int* total)
{
	*deckIndex = 0;
	*total = 0;

	players[0].cardcount = 0;
	players[1].cardcount = 0;

	Deck(deck);
	Shuffle(deck);
}

// 라운드 초기화 함수

void Printplayerhands(Player players[], int numPlayers)
{

	printf("%s의 카드: ", players[1].name);
	for (int j = 0; j < players[1].cardcount; j++)
	{
		const char* sym = Symbol(players[1].hand[j].form);
		if (j < 2 || j == 6)
		{
			printf("■ ");
		}
		else
		{

			printf("%s%d ", sym, players[1].hand[j].num + 1);
		}
	}
	printf("\n\n%s의 카드: ", players[0].name);
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
		printf("%s의 카드: ", players[i].name);
		for (int j = 0; j < players[i].cardcount; j++)
		{
			const char* sym = Symbol(players[i].hand[j].form);
			printf("%s%d ", sym, players[i].hand[j].num + 1);
		}
		printf("\n");
	}
}

// 손 패 보여주기 (printPlayerHands / lastprint)

int Confirm(Poker hand[], int cardcount)
{
	int numcount[14] = { 0 }; // 숫자
	int formcount[4] = { 0 }; // S:0, H:1, D:2, C:3
	int maxcount = 0; // 같은 숫자 ( 1 1 1 1 )
	int contcount = 0; // 연속 숫자 ( 1 2 3 4 )
	int straight = 0; // 스트레이트 

	int pair = 0;
	int triple = 0;

	for (int i = 0; i < cardcount; i++)
	{
		numcount[hand[i].num]++;
		///////////////////////////////////// 숫자 계산

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
		///////////////////////////////////// 모양 계산
	}
	for (int i = 0; i <= 13; i++)
	{
		if (numcount[i] > maxcount)
		{
			maxcount = numcount[i];
		}
		///////////////////////////////////// 같은 숫자 갯수 계산
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
		///////////////////////////////////// 스트레이트용 숫자 계산
	}
	for (int i = 0; i < 4; i++)
	{
		if (straight >= 5 && formcount[i] >= 5)
		{
			if (numcount[0] && numcount[10] && numcount[11] && numcount[12] && numcount[13])
			{
				return 10;
			}
			///////////////////////////////////////////////////////// 로얄 스트레이트 플러시
			return 9;
		}
		///////////////////////////////////////////////////////// 스트레이트 플러시
	}
	if (maxcount == 4)
	{
		return 8;
	}
	////////////////////////////////////////////////////// 포카드
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
	////////////////////////////////////////////////////// 풀하우스
	for (int i = 0; i < 4; i++)
	{
		if (formcount[i] >= 5)
		{
			return 6;
		}
	}
	////////////////////////////////////////////////////// 플러시
	if (straight == 5)
	{
		return 5;
	}
	////////////////////////////////////////////////////// 스트레이트
	if (maxcount == 3)
	{
		return 4;
	}
	////////////////////////////////////////////////////// 트리플
	if (pair >= 2)
	{
		return 3;
	}
	////////////////////////////////////////////////////// 투 페어
	if (pair == 1)
	{
		return 2;
	}
	////////////////////////////////////////////////////// 원 페어

	return 1;
	////////////////////////////////////////////////////// 족보가 없을 경우
}

// 족보 계산기

void Winner(Player players[], int total)
{

	int scores[2]; // 점수 넣기용

	for (int i = 0; i < 2; i++)
	{
		scores[i] = Confirm(players[i].hand, players[i].cardcount);
	}
	printf("%s의 점수: %d\n", players[0].name, scores[0]);
	printf("%s의 점수: %d\n", players[1].name, scores[1]);

	if (scores[0] > scores[1])
	{
		system("cls");
		printf("%s 승리. 판돈 %d원을 획득했습니다.\n\n", players[0].name, total);
		players[0].money += total;
	}
	else if (scores[0] < scores[1])
	{
		system("cls");
		printf("%s 승리.  AI가 판돈 %d원을 획득했습니다.\n\n", players[1].name, total);
		players[1].money += total;
	}
	else
	{
		printf("무승부! 판돈이 나누어집니다.\n\n");
		players[0].money += total / 2;
		players[1].money += total / 2;
	}

}

// 승패 계산기


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

// 게임 재시작기

void manual()
{
	int abc = 2;

	while (1)
	{
		printf("1. 게임 시작\n2. 룰 설명\n3. 족보 설명\n0. 게임 종료\n숫자를 입력하십시오.\n");
		scanf_s("%d", &abc);
		if (abc == 1)
		{
			printf("덱 섞는중..\n");
			printf("덱 섞는중...\n");
			printf("덱 섞는중....\n");
			printf("덱 섞는중.....\n");
			break;
		}
		else if (abc == 2)
		{
			printf("각 플레이어에게 7장의 카드가 순차적으로 배분됩니다. \n");
			printf("처음 2장은 비공개(홀카드), 세 번째 카드는 공개(오픈카드)로 배분됩니다. \n");
			printf(" 첫 소지금은 1000원이며, 총 100,000원을 만들면 승리합니다.\n");
			printf(" 소지금을 모두 잃으면 패배합니다.\n");
			printf(" 7번째 카드를 받은 후. \n");
			printf(" 모든 카드를 받은 후, 플레이어들은 자신이 가진 7장의 카드 중 5장을 선택해 최고의 족보를 만듭니다. \n");
			printf(" 가장 높은 족보를 가진 플레이어가 승리하고 베팅 금액을 가져갑니다. \n");
			printf(" 상황 판단과 베팅 전략이 중요합니다.\n");
			printf(" 상대방의 오픈 카드를 보고 족보를 예상하시길 바랍니다.\n\n\n");

		}
		else if (abc == 0)
		{
			printf("게임을 종료합니다.\n");
			break;
		}
		else if (abc == 3)
		{
			printf("11 = J / 12 = Q / 13 = K \n\n");
			printf("1. 로열 스트레이트 플러쉬(Royal Flush)\n");
			printf(" - 같은 무늬의 A, K, Q, J, 10 \n\n");
			printf("2. 스트레이트 플러쉬 (Straight Flush)\n");
			printf(" - 같은 무늬의 연속된 5장의 카드\n");
			printf(" - 예: 9♥, 8♥, 7♥, 6♥, 5♥\n\n");
			printf("3. 포 카드 (Four of a Kind)\n");
			printf(" - 같은 숫자의 카드 4장\n");
			printf(" - 예: K♠, K♥, K♣, K◆\n\n");
			printf("4. 풀 하우스 (Full House)\n");
			printf(" - 3장의 같은 숫자와 2장의 같은 숫자 조합\n");
			printf(" - 예: 10♠, 10◆, 10♥, 5♣, 5♥\n\n");
			printf("5. 플러쉬 (Flush)\n");
			printf(" - 같은 무늬의 5장의 카드 (숫자는 상관없음)\n");
			printf(" - 예: A♠, Q♠, 8♠, 6♠, 3♠\n\n");
			printf("6. 스트레이트 (Straight)\n");
			printf(" - 연속된 숫자의 5장의 카드 (무늬는 상관없음)\n");
			printf(" - 예: 7♠, 6♥, 5♣, 4◆, 3♥\n\n");
			printf("7. 트리플 (Three of a Kind)\n");
			printf(" - 같은 숫자의 카드 3장\n");
			printf(" - 예: Q♠, Q♣, Q◆\n\n");
			printf("8. 투 페어 (Two Pair)\n");
			printf(" - 2장의 같은 숫자 카드가 2쌍\n");
			printf(" - 예: J♠, J♥, 9♣, 9♥\n\n");
			printf("9. 원 페어 (One Pair)\n");
			printf(" - 같은 숫자의 카드 2장\n");
			printf(" - 예: 5♠, 5♥\n\n");
			printf("10. 하이 카드 (High Card)\n");
			printf("  - 위 족보에 해당하지 않는 가장 높은 카드\n");
			printf("  - 예: A♠ (가장 높은 카드)\n");
		}
		else
		{
			printf("잘못된 입력입니다. 다시 시도하십시오.\n");
		}
	}
}
// 게임 시작 및 설명

int main()
{
	int betmoney = 0;
	int total = 0;
	int deckIndex = 0;
	char nam[40];
	char choice;
	char End;
	printf("이름을 작성하세요:");
	scanf_s("%s", nam);
	printf("환영합니다, %s \n", nam);


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
#pragma region 카드 수 (3)
			Dealing(deck, players, 2, &deckIndex, 3, 0, 3);  // 카드 수 3

			Printplayerhands(players, 2);
			printf("베팅하세요.(현재 금액 : %d) \n", players[0].money);

			scanf_s("%d", &betmoney);

			if (betmoney > players[0].money)
			{
				printf("베팅 금액이 소지금을 초과했습니다. 최대 %d원까지만 가능합니다.\n", players[0].money);
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI가 베팅합니다...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("현재 판돈: %d원\n", total);
#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region 카드 수 (4)
			Dealing(deck, players, 2, &deckIndex, 3, 3, 4); // 카드 수 4

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("베팅하세요.(현재 금액 : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("베팅 불가능.\n 턴을 넘깁니다. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("베팅 금액이 소지금을 초과했습니다. 최대 %d원까지만 가능합니다.\n", players[0].money);
				printf("ALL IN으로 간주합니다.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI가 베팅합니다...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("현재 판돈: %d원\n", total);

#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region 카드 수 (5)
			Dealing(deck, players, 2, &deckIndex, 4, 4, 5); // 카드 수 5

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("베팅하세요.(현재 금액 : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("베팅 불가능.\n 턴을 넘깁니다. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("베팅 금액이 소지금을 초과했습니다. 최대 %d원까지만 가능합니다.\n", players[0].money);
				printf("ALL IN으로 간주합니다.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI가 베팅합니다...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("현재 판돈: %d원\n", total);

#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region 카드 수 (6)
			Dealing(deck, players, 2, &deckIndex, 5, 5, 6); // 카드 수 6

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("베팅하세요.(현재 금액 : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("베팅 불가능.\n 턴을 넘깁니다. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("베팅 금액이 소지금을 초과했습니다. 최대 %d원까지만 가능합니다.\n", players[0].money);
				printf("ALL IN으로 간주합니다.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI가 베팅합니다...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("현재 판돈: %d원\n", total);

#pragma endregion

			///////////////////////////////////////////////////////////////////////////////
#pragma region 카드 수 (7)
			Dealing(deck, players, 2, &deckIndex, 6, 6, 7); // 카드 수 7

			system("pause");
			system("cls");
			Printplayerhands(players, 2);

			if (players[0].money > 1)
			{
				printf("베팅하세요.(현재 금액 : %d) \n", players[0].money);

				scanf_s("%d", &betmoney);
			}
			else if (players[0].money == 0)
			{
				printf("베팅 불가능.\n 턴을 넘깁니다. \n\n");
				betmoney = 0;
			}
			if (betmoney > players[0].money && players[0].money - betmoney < 0)
			{
				printf("베팅 금액이 소지금을 초과했습니다. 최대 %d원까지만 가능합니다.\n", players[0].money);
				printf("ALL IN으로 간주합니다.");
				betmoney = players[0].money;
			}
			players[0].money -= betmoney;
			total += betmoney;

			printf("AI가 베팅합니다...\n");
			players[1].money -= betmoney;
			total += betmoney;

			printf("현재 판돈: %d원\n", total);

#pragma endregion

			Winner(players, total);

			Lastprint(players, 2);

			total = 0;

			printf("\n 다음 라운드로 자동 진행됩니다.종료하고 싶으시다면 'X' 키를 누르십시오.\n");
			printf("진행 중...\n");

			getchar();
			End = getchar();
			if (End == 'X' || End == 'x')
			{
				printf("게임을 종료합니다. 감사합니다.\n");
				return 0;
			}

			Resetround(players, deck, &deckIndex, &total);
			system("cls");
			continue;
		}
		if (players[0].money >= target)
		{
			printf("축하합니다! %s님이 목표 금액 %d원을 달성했습니다.\n", players[0].name, target);
			printf("게임을 다시 시작하려면 R을 누르세요. 종료하려면 다른 키를 누르세요.\n");

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
			printf("%s님이 패배했습니다.소지금이 0원이 되었습니다.\n", players[0].name);
			printf("게임을 다시 시작하려면 R을 누르세요. 종료하려면 다른 키를 누르세요.\n");

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
			printf("게임을 종료합니다. 감사합니다!\n");
			break;
		}
	}
	return 0;
}
