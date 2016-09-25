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
  
int				/* account should already be locked */
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

void				/* account should already be locked */
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






int main()
{
  int numThreads = 3, i, sumFlowOut = 0;
  initAccounts();

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
  

