# Bank Exercises

## Access to a single account

The following programs implement
deposits and withdrawals issued by multiple ATMs.  All of these
transactions access the same bank account.

* Dep-wd-race: uses no synchronization and has a race condition
* Dep-wd-ok: uses a mutex to serialize transactions to the account

## Access to multiple accounts

Transfer-bad uses mutual exclusion to serialize accesses to each
account.  Sadly, it can deadlock.  Your job is to fix it.