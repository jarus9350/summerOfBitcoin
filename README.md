# summerOfBitcoin
screening test  for summerOfBitcoin 

# problem approach
according to the problem , the problem seemed to be a modified version of 0/1 knapsack 
where printing is involved thus using dynamic programming for maximum fee
and breadth first search for finding path with maximum fee
then validating the answer 

# the tasks are as follows
  1) taking input from mempool.csv and editing the mp(map storing parents of child if a child has any).
  2) editing the id_f_w(map having structure as tx_id,fee,weight) for fast quering.
  3) making and eddited array(arranged array of type tx_id,fee,weight )so that quering will take O(1) and arranging it so that childrens come first in before the parent.
  4) making a dp 2d array of size (no of tx_id X weight) and find maximum fee collected using the problem of 0/1 knapsack.
  5) now using breadth first search to find different number of solution
  6) now from the given solutions finding the valid solution i.e those whose parents come before child
  7) outputing the correct order in output.txt
  8) if the no correct answer were available the doing the above steps for ( weight - 1) and so on.
