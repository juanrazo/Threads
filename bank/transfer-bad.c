#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"
#include "mutex.h"

typedef struct {
  int accountId; 
  int balance;
  Mutex lock;
} Account;
Account accounts[2];

void 
initAccounts()
{
  int i;
  for (i = 0; i < 2; i++) {
    Account *a = &accounts[i];
    a->accountId = i;
    a->balance = 0;
    mutexInit(&a->lock, 1);
  }
}
    
typedef struct {
  int atmId;			/* id of this ATM */
  int cashFlowOut;		/* net amount withdrawn */
} ATM;
ATM atms[3] = { {0,0}, {2,0}, {3,0} }; /* 3 atms, zero dollars */
  
int
doWithdraw(ATM *atm, Account *account, int request)
{
  int bal, retval = 0;
  bal = account->balance;
  yield();
  if (bal >= request) {
    bal -= request;
    yield();
    account->balance = bal;
    atm->cashFlowOut += request;
    return 1;			/* succeeded */
  }
  return 0;
}

void 
doWithdraws(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++) {
    mutexLock(&accounts[0].lock);
    doWithdraw(atm, &accounts[0], 7);
    mutexUnlock(&accounts[0].lock);
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
    mutexLock(&accounts[0].lock);
    doDeposit(atm, &accounts[0], 9);
    mutexUnlock(&accounts[0].lock);
    yield();
  }
}


int
doTransfer(ATM *atm, Account *from, Account *to, int amt)
{
  int success = 0;
  mutexLock(&from->lock);
  mutexLock(&to->lock);
  if (doWithdraw(atm, from, amt)) {
    doDeposit(atm, to, amt);
    success = 1;
  }
  mutexUnlock(&from->lock);
  mutexUnlock(&to->lock);
  return success;
}

void
transfer01(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++)
    doTransfer(atm, &accounts[0], &accounts[1], 10);
}

void
transfer10(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++)
    doTransfer(atm, &accounts[1], &accounts[0], 10);
}



int main()
{
  int numThreads = 3, i, sumFlowOut = 0, sumBalances = 0;
  initAccounts();
  doDeposit(&atms[0], &accounts[0], 100);
  doDeposit(&atms[1], &accounts[1], 100);

  makeThreads(numThreads);

  createThread((void (*)(void *))transfer10, &atms[0]);
  createThread((void (*)(void *))transfer01, &atms[1]);

  startThreads();		/* start working! */
  printf("Threads Terminated\n");

  for (i = 0; i < 3; i++) {
    printf("ATM %d had flow out %d\n", i, atms[i].cashFlowOut);
    sumFlowOut += atms[i].cashFlowOut;
  }
  for (i = 0; i < 2; i++) {
    printf("Account %d has final balance %d\n", i, accounts[i].balance);
    sumBalances += accounts[i].balance;
  }
    
  printf("sum balances = %d, sum flow out = %d\n", sumBalances, sumFlowOut);
}
  
