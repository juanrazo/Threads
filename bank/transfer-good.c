/** \file transfer-bad.c
 *  \brief This is a simple bank transaction demo using threads.
 *  This demo creates 3 atms and 2 accounts, only one account is
 *  used. Each ATM runs within a seperate thread and all ATMs do
 *  transactions on the same account. The accounts have locks on
 *  them and all threads run concurrently.
 */
#include "stdlib.h"
#include "tthreads.h"
#include "stdio.h"
#include "mutex.h"

/** Account struct, contains account number, balance, and account lock */
typedef struct {
  int accountId;                        /**< account ID (number) */    
  int balance;				/**< account balance */        
  Mutex lock;				/**< account lock */           
} Account;				                               
Account accounts[2];			/**< two unitialized accounts */

/** Initializes 2 accounts.
 *  IDs are position in array, balance
 *  starts at zero and accounts are
 *  unlocked.
 */
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

/** ATM struct, contains and ID and tracks cashflow */
typedef struct { 
  int atmId;	                        /**< id of this ATM */           		
  int cashFlowOut;	       		/**< net amount withdrawn */     
} ATM;					                                 
ATM atms[3] = { {0,0}, {2,0}, {3,0} }; 	/**< 3 atms, zero dollars each */

/** Withdraw money from an account. If the requested amount
 *  is available withdraws the amount from specified account.
 *  Account should be locked prior to calling.
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
  int bal,                              /**< balance of the account */
      retval = 0;			/**< success status */        
  bal = account->balance;		                               
  yield();				                               
  if (bal >= request) {			/* check if enough in account */ 
    bal -= request;			                               
    yield();				                               
    account->balance = bal;		                               
    atm->cashFlowOut += request;	                               
    return 1;		      		/* succeeded */               
  }
  return 0;
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
    mutexLock(&accounts[0].lock);
    doWithdraw(atm, &accounts[0], 7);
    mutexUnlock(&accounts[0].lock);
    yield();
  }
}

/** Deposits money into an account.
 *  Account should be locked prior to calling.
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
    mutexLock(&accounts[0].lock);
    doDeposit(atm, &accounts[0], 9);
    mutexUnlock(&accounts[0].lock);
    yield();
  }
}

/** Initiates a transfer from one account to another if the
 *  balance is available in the source account. 
 *
 *  \param atm The atm where the transfer is occuring
 *  \param from The account where the money is withdrawn
 *  \param to The account where the money is being deposited
 *  \param amt Tha amount being transferred
 *  \return Returns 1 if withdrawal was succesful, 0 otherwise
 */
int doTransfer(ATM *atm, Account *from, Account *to, int amt)
{
  int success = 0;                      /**< success status */
  mutexLock(&from->lock);
  //mutexLock(&to->lock);              This is the cause of the Deadlock
  if (doWithdraw(atm, from, amt)) {
    mutexUnlock(&from->lock);          // Release so the other thread can use
    mutexLock(&to->lock);
    doDeposit(atm, to, amt);
    mutexUnlock(&to->lock);
    success = 1;                        /**< succeeded */
  }
  else{                               // In case doWithdraw() is false
    mutexUnlock(&from->lock);
  }
  //mutexUnlock(&from->lock);
  //mutexUnlock(&to->lock);
  return success;
}

/** Makes 3 transfers for the amount of 10 from 
 *  account 0 to account 1.
 *  
 *  \param atm Where the transfers are to occur.
 */
void
transfer01(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++)
    doTransfer(atm, &accounts[0], &accounts[1], 10);
}

/** Makes 3 transfers for the amount of 10 from 
 *  account 1 to account 0.
 *  
 *  \param atm Where the transfers are to occur.
 */
void
transfer10(ATM *atm)
{
  int i;
  for (i = 0; i < 3; i++)
    doTransfer(atm, &accounts[1], &accounts[0], 10);
}

/** Initilazes everything, starts, threads, and prints results
 */
int main()
{
  int numThreads = 3,                   /**< Number of threads */
      i,
      sumFlowOut = 0,                   /**< Total cashflow from all ATMs */
      sumBalances = 0;                  /**< Total of all account balances */
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
  
