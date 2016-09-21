#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"

// #define yield()

typedef struct {
  int accountId; 
  int balance;
} Account;
Account accounts[2] = {{0,0}, {1,0}}; /* 2 accounts, zero balances */

typedef struct {
  int atmId;			/* id of this ATM */
  int cashFlowOut;		/* net amount withdrawn */
} ATM;
ATM atms[3] = { {0,0}, {1,0}, {2,0} }; /* 3 atms, zero dollars */
  
int
doWithdraw(ATM *atm, Account *account, int request)
{
  int bal = account->balance, success = 0;
  yield();
  if (bal >= request) {
    bal -= request;
    yield();
    account->balance = bal;
    atm->cashFlowOut += request;
    success = 1;
  }
  return success;
}

void 
doWithdraws(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++) {
    doWithdraw(atm, &accounts[0], 7);
    yield();
  }
}

void
doDeposit(ATM *atm, Account *account, int request)
{
  int bal = account->balance;
  yield();
  bal += request;
  account->balance = bal;
  atm->cashFlowOut -= request;
}

void 
doDeposits(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++) {
    doDeposit(atm, &accounts[0], 9);
    yield();
  }
}






int main()
{
  int numThreads = 3, i, sumFlowOut = 0;
  makeThreads(numThreads);

				// ATM 0 does deposits
  createThread((void (*)(void *))doDeposits, &atms[0]);
				// ATMs 1 & 2 do withdrawals
  for (i = 0; i < 2; i++)	
    createThread((void (*)(void *))doWithdraws, &atms[i+1]);

  startThreads();		/* start working! */
  printf("Threads Terminated\n");

  for (i = 0; i < 3; i++) {
    printf("ATM %d had flow out %d\n", i, atms[i].cashFlowOut);
    sumFlowOut += atms[i].cashFlowOut;
  }
  printf("final balance = %d, sum flow out = %d\n", accounts[0].balance, sumFlowOut);
}
  

