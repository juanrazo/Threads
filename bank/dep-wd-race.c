/** \file bank_simple.c
 *  \brief This is a simple bank transaction demo using threads.
 *  This demo creates 3 atms and 2 accounts, only one account is
 *  used. Each ATM runs within a seperate thread and all ATMs do
 *  transactions on the same account. There is no coordination
 *  between the threads and the transactions occur concurrently.
 *
 */
#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"
#include "mutex.h"

#ifdef NO_YIELD
#  define yield()
#endif

/** ATM struct, contains and ID and tracks cashflow */
typedef struct {
  int atmId;			        /**< id of this ATM */
  int cashFlowOut;		        /**< net amount withdrawn */
} ATM;

/** Account struct, contains account number and balance */
typedef struct {
  int accountId;                        
  int balance;
} Account;

ATM atms[3] = { {0,0}, {1,0}, {2,0} };  /**<  3 atms, zero dollars each */
Account accounts[2] = {{0,0}, {1,0}};   /**<  2 accounts, zero balance each */

/** Withdraw money from an account. If the requested amount
 *  is available withdraws the amount from specified account
 *
 *  \param atm The atm attempting the withdrawal
 *  \param account The account being withdrawn from
 *  \param request The amount to withdraw
 *  \return Returns 1 if withdrawal was succesful, 0 otherwise
 *  \sa doWithdraws() doDeposit()
 */
int
doWithdraw(ATM *atm, Account *account, int request)
{
  int bal = account->balance,           /**< balance of the account */
      success = 0;                      /**< success status */
  yield();
  if (bal >= request) {                 /* check if enough in account */
    bal -= request;
    yield();
    account->balance = bal;
    atm->cashFlowOut += request;
    success = 1;                        /* succeeded */
  }
  return success;
}

/** Makes 3 withdrawals from account 0 in the amount of 7
 *  
 *  \param atm The atm where the withdrawals are being made
 *  \sa doWithdraw
 */
void 
doWithdraws(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++) {
    doWithdraw(atm, &accounts[0], 7);
    yield();
  }
}

/** Deposits money into an account.
 *
 *  \param atm The atm where the deposit is being made
 *  \param account The account being deposited to
 *  \param request The amount to deposit
 *  \sa doDeposits() doWithdraws()
 */
void
doDeposit(ATM *atm, Account *account, int request)
{
  int bal = account->balance;
  yield();
  bal += request;
  account->balance = bal;
  atm->cashFlowOut -= request;
}

/** Makes 3 deposits into account 0 in the amount of 9.
 *
 *  \param atm The atm where the deposits are being made
 */
void 
doDeposits(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++) {
    doDeposit(atm, &accounts[0], 9);
    yield();
  }
}

/** Initilazes everything, starts, threads, and prints results
 */
int main()
{
  int numThreads = 3,                   /**< Number of threads */
      i,
      sumFlowOut = 0;                   /**< Total cashflow from all ATMs */
  makeThreads(numThreads);

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
  

