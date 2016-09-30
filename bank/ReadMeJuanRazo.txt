Juan Razo


For this lab the correction I made was under the function doTransfer(). The problem was that when both
threads used the doTransfer() function. For both were locking their "from" account, this was a problem
for the next instruction was to lock the "to" account which in both threads. This was prevented by
locking the "from" account, withdraw the funds and release the lock of the "from" account. Now the
function may continue to lock the "to" account and do the deposit then unlock. This now allows both
threads to work correctly and avoid the Deadlock. For this lab I read section 2.3 from the OS book,
assisted Eric Camacho and Mario Landa on the explination of Deadlock. No code was shared.
