# ECS150_Project#2: User-level thread libray
The provided code implements a `user-level thread` 
library with `FIFO`  queue features for managing 
`threads`, `semaphores`, and `preemption`.It 
includes APIs for creating and managing 
threads, synchronizing access to shared resources, 
and ensuring fair scheduling through `preemption`. 
The library offers essential functionalities for 
developing multi-threaded applications efficiently 
and preventing concurrency issues.
### Phase 1: queue API
a First-In-First-Out (FIFO) `queue` data structure 
to manage the order of execution for 
threads. This means that threads are scheduled based 
on the order in which they are added to the queue.
FIFO scheduling ensures fairness and prevents thread 
starvation by guaranteeing that threads are executed
in the order they arrive.The purpose of this phase is t
o establish the foundational data structure for managing 
threads within the user-level thread library. The queue API 
serves as the backbone for organizing and scheduling threads, 
ensuring proper execution order and synchronization.

### Phase 2: uthread API
`Thread Management`: The library provides functions for
creating, destroying, enqueuing, dequeuing, deleting, 
iterating, and determining the length of the queue. 
These functions enable comprehensive management 
of threads within the queue, allowing developers to 
control thread execution and synchronization.The Phase
2 of the program extends the functionality of the 
user-level thread library by implementing the thread API.
This API allows users to create, manage, and execute threads 
in the library. It leverages the queue API developed in the 
previous phase to store and schedule threads.
The purpose of Phase 2 is to provide users with an interface 
to create and manage threads within the user-level thread library. 
By implementing the uthread API, users can spawn new threads, 
switch between threads, and control the execution flow of the program.

### Phase 3: semaphore API
`Semaphores`: Semaphores are synchronization primitives
used to control access to shared resources in 
concurrent programs. The library implements semaphore 
functionality to ensure that multiple threads can 
safely access shared resources.The purpose of the Semaphore 
API is to provide a mechanism for coordinating access to shared 
resources among multiple threads. By using semaphores, threads 
can coordinate their activities and ensure mutual exclusion when 
accessing critical sections of code or shared data structures. T
his helps prevent race conditions, which can lead to unpredictable 
behavior and data corruption in concurrent programs.
without encountering race conditions or deadlocks.

### Phase 4: preemption
`Preemption`: Preemption is a feature that allows the 
operating system to interrupt a running thread and 
switch to another thread. The library 
implements preemption functionality to prevent long-running
threads from monopolizing the CPU and ensure 
fair scheduling of threads. This is achieved by configuring 
a timer that triggers periodic interrupts, 
allowing the library to preempt running threads and switch to other 
threads in the queue.Preemption ensures that no single thread can 
indefinitely block other threads from executing. By preempting a 
thread after a certain time quantum or based on specific conditions, 
the scheduler can switch to a different thread, allowing fair access to 
CPU resources among all threads. This prevents scenarios where a single 
thread might hog CPU time, leading to performance degradation and 
unresponsiveness in a multi-threaded application.
## Design Choices:
`phase1` :Linked List Implementation: The queue is implemented 
using a linked list data structure, allowing for
efficient insertion and removal 
of elements while preserving the order of 
execution.Error Handling: Error codes (SUCC and ERR) 
are defined to indicate the success or failure of 
queue operations. Proper error handling 
ensures the robustness and reliability of the library.

`phase2`:Thread Control Block (TCB): Each thread is 
represented by a Thread Control Block (TCB), which 
contains information about the thread's context and 
execution state. This design choice allows for efficient 
anagement and manipulation of threads within the library.
Queue Integration: The uthread API utilizes the queue 
API developed in Phase 1 to maintain a queue of threads. 
This queue manages the execution order of threads, 
ensuring proper synchronization 
and scheduling.

`phase3`:Semaphore Creation (sem_create()): This function 
is responsible for creating a new semaphore
with an initial value. Semaphores are initialized with a 
non-negative integer value, typically representing
the number of available resources or permits.
Semaphore Destruction (sem_destroy()): 
The sem_destroy() function deallocates 
the resources associated with a semaphore when 
it is no longer needed. T
his ensures proper cleanup and prevents memory leaks.
Semaphore Down Operation (sem_down()): The sem_down() 
function, also known as the "wait" or "P" operation, 
decrements the value of the semaphore. 
If the semaphore value becomes negative, the calling thread is blocked 
until resources become available.

`phase4`:Priority-based Preemption: Threads with 
higher priority levels may preempt threads with lower
priority levels. This ensures that critical 
threads are given precedence over less critical ones, 
contributing to better system responsiveness.
Preemption Points: Preemption points are specific 
locations in the codewhere the scheduler can interrupt
the execution of a thread and switch to another. 
These points are strategically placed to ensure that threads 
can be preempted without causing data inconsistency or other issues.

## Testing
To ensure the correctness and functionality of the queue 
API, rigorous testing is required. Test cases should 
cover various scenarios, including enqueueing and 
dequeueing elements, queue destruction, iteration, and error 
handling. Test cases should verify that the queue
behaves as expected under different 
conditions.

To ensure the correctness and functionality of the thread API,
comprehensive testing is necessary. Test cases should cover various 
scenarios, including thread creation, switching between threads, blocking
and unblocking threads, and error handling. Testing should verify that the 
uthread API behaves as expected under different conditions and edge cases.

To ensure the correctness and reliability of the Semaphore API, 
thorough testing is essential. Test cases should cover 
various scenarios, including multiple threads attempting 
to acquire and release semaphores concurrently, handling of edge 
cases such as semaphore destruction while threads are waiting, 
and proper synchronization of access to shared resources. Additionally,
stress testing can help identify potential race conditions or deadlock 
situations.

Testing preemption involves verifying that threads are preempted 
appropriately based on the chosen preemption strategy. Test cases 
should cover scenarios such as threads running for longer than the 
time quantum, higher priority threads preempting lower priority threads, 
and proper handling of preemption points. Stress testing and performance
profiling can also help assess the effectiveness of the preemption mechanism
under various workloads.


## Source
Everything included in lecture slide, project script(
        including the external links and idea form chatgpt).

