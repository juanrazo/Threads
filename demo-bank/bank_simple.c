#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"
#include "mutex.h"

#ifdef NO_YIELD
#  define yield()
#endif

typedef struct {
  int atmId;			/* id of this ATM */
  int cashFlowOut;		/* net amount withdrawn */
} ATM;

typedef struct {
  int accountId; 
  int balance;
  Mutex lock;
} Account;

ATM atms[3] = { {0,0}, {1,0}, {2,0} }; /* 3 atms, zero dollars */
Account accounts[2] = {{0,0}, {1,0}}; /* 2 accounts, zero balances */
  
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

  /* initialize account mutexes (unlocked) */
  for (i = 0; i < 3; i++)
    mutexInit(&(accounts[i].lock), 1);
  
  /* ATM 0 does deposits */
  createThread((void (*)(void *))doDeposits, &atms[0]);

  /* ATMs 1 & 2 do withdrawals */
  for (i = 1; i < 3; i++)	
    createThread((void (*)(void *))doWithdraws, &atms[i]);

  startThreads();		/* start working! */
  printf("Threads Terminated\n");

  for (i = 0; i < 3; i++) {
    printf("ATM %d had flow out %d\n", i, atms[i].cashFlowOut);
    sumFlowOut += atms[i].cashFlowOut;
  }
  printf("final balance = %d, sum flow out = %d\n", accounts[0].balance, sumFlowOut);
}
  

