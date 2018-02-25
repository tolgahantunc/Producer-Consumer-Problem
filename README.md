# Producer-Consumer-Problem (Homework)

Implement a C program that is explained below using WinAPI. In this homework, we want you  to  create  “n>=5”  pipes.  These  pipes  should  send  information  from  parent  process  to  child processes.  The  pipes  should  send  information  about  what  kind  of  operation  it  will  perform (producer, consumer or swapper) and the value it produces if it is a producer process.  

The pipes will carry out three types of data;  
  {char* producer}  
  {char* consumer}  
  {char* swapper}  


Different  from  the  Producer/Consumer  problem  you  have  an  additional  task  called  the “Swapper”.  The  “Swapper”  has  a  task  to  swap  index  values.  However,  while  performing  the swapping task, at least one of the indexes should be full. Deciding the indexes should be random, therefore, your “Swapper” should generate two random indexes, based on your buffer size.  


The  tasks  of  child  processes  should  be  given  and  decided  by  the  parent  process.  The information should be generated multiple times, randomly and passed their tasks through a pipe to its child processes.  


After each process obtain their tasks from the pipes. All the child processes should try to perform their given operation concurrently on buffer. The buffer should be a shared memory that will  give  access  for  the  child  processes  and  your  buffer  size  should  be  equal  to  the  number  of processes that you have created. You should print every operation on the console to show which child is doing which operation on the buffer.  

Your output should look like this:  
 
  child 1 produced 8 as 5 th  entry in the buffer  
  child 2 consumed 8 from 5 th  entry from the buffer  
  child 2 produced 11 as 4 th  entry in the buffer  
  child 3 produced 2 as 9 th  entry in the buffer  
  child 4 swapped 4 th  index with 9 th  index  
  child 1 consumed 2 from 4 th  entry from the buffer  
  child 4 swapped 9 th  index with 3 rd  index  
  child 2 consumed 11 from 3 rd  index  
  .  
  .  
  .  
