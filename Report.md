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

Phase 1: queue API
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
`Thread Management`: The library provides functions for
creating, destroying, enqueuing, dequeuing, deleting, 
iterating, and determining the length of the queue. 
These functions enable comprehensive management 
of threads within the queue, allowing developers to 
control thread execution 
and synchronization.
`Semaphores`: Semaphores are synchronization primitives
used to control access to shared resources in 
concurrent programs. The library implements semaphore 
functionality to ensure that multiple threads can 
safely access shared resources
without encountering race conditions or deadlocks.
`Preemption`: Preemption is a feature that allows the 
operating system to interrupt a running thread and 
switch to another thread. The library 
implements preemption functionality to prevent long-running
threads from monopolizing the CPU and ensure 
fair scheduling of threads. This is achieved by configuring 
a timer that triggers periodic interrupts, 
allowing the library to preempt running threads and switch to other 
threads in the queue.
