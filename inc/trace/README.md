
Two Cents for QNX Trace Event
===========

The _NTO_TRACE_THREAD class includes events related to state changes for threads.
-----------------
| Event                    | traceprinter label | IDE label      | Emitted when a thread:       |
|--------------------------|--------------------|----------------|------------------------------|
| _NTO_TRACE_THCONDVAR     | THCONDVAR          | Condvar        | Enters the CONDVAR state     |
| _NTO_TRACE_THCREATE      | THCREATE           | Create Thread  | Is created                   |
| _NTO_TRACE_THDEAD        | THDEAD             | Dead           | Enters the DEAD state        |
| _NTO_TRACE_THDESTROY     | THDESTROY          | Destroy Thread | Is destroyed                 |
| _NTO_TRACE_THINTR        | THINTR             | Interrupt      | Enters the INTERRUPT state   |
| _NTO_TRACE_THJOIN        | THJOIN             | Join           | Enters the JOIN state        |
| _NTO_TRACE_THMUTEX       | THMUTEX            | Mutex          | Enters the MUTEX state       |
| _NTO_TRACE_THNANOSLEEP   | THNANOSLEEP        | NanoSleep      | Enters the NANOSLEEP state   |
| _NTO_TRACE_THNET_REPLY   | THNET_REPLY        | NetReply       | Enters the NET_REPLY state   |
| _NTO_TRACE_THNET_SEND    | THNET_SEND         | NetSend        | Enters the NET_SEND state    |
| _NTO_TRACE_THREADY       | THREADY            | Ready          | Enters the READY state       |
| _NTO_TRACE_THRECEIVE     | THRECEIVE          | Receive        | Enters the RECEIVE state     |
| _NTO_TRACE_THREPLY       | THREPLY            | Reply          | Enters the REPLY state       |
| _NTO_TRACE_THRUNNING     | THRUNNING          | Running        | Enters the RUNNING state     |
| _NTO_TRACE_THSEM         | THSEM              | Semaphore      | Enters the SEM state         |
| _NTO_TRACE_THSEND        | THSEND             | Send           | Enters the SEND state        |
| _NTO_TRACE_THSIGSUSPEND  | THSIGSUSPEND       | SigSuspend     | Enters the SIGSUSPEND state  |
| _NTO_TRACE_THSIGWAITINFO | THSIGWAITINFO      | SigWaitInfo    | Enters the SIGWAITINFO state |
| _NTO_TRACE_THSTACK       | THSTACK            | Stack          | Enters the STACK state       |
| _NTO_TRACE_THSTOPPED     | THSTOPPED          | Stopped        | Enters the STOPPED state     |
| _NTO_TRACE_THWAITCTX     | THWAITCTX          | WaitCtx        | Enters the WAITCTX state     |
| _NTO_TRACE_THWAITPAGE    | THWAITPAGE         | WaitPage       | Enters the WAITPAGE state    |
| _NTO_TRACE_THWAITTHREAD  | THWAITTHREAD       | WaitThread     | Enters the WAITTHREAD state  |


Thread states
-----------------
| STATE_CONDVAR                                                                                                                                            |
|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| The thread is blocked on a condition variable (e.g., it called pthread_cond_wait()).                                                                     |
| STATE_DEAD                                                                                                                                               |
| The thread has terminated and is waiting for a join by another thread.                                                                                   |
| STATE_INTR                                                                                                                                               |
| The thread is blocked waiting for an interrupt (i.e., it called InterruptWait()).                                                                        |
| STATE_JOIN                                                                                                                                               |
| The thread is blocked waiting to join another thread (e.g., it called pthread_join()).                                                                   |
| STATE_MUTEX                                                                                                                                              |
| The thread is blocked on a mutual exclusion lock (e.g., it called pthread_mutex_lock()).                                                                 |
| STATE_NANOSLEEP                                                                                                                                          |
| The thread is sleeping for a short time interval (e.g., it called nanosleep()).                                                                          |
| STATE_NET_REPLY                                                                                                                                          |
| The thread is waiting for a reply to be delivered across the network (i.e., it called MsgReply*()).                                                      |
| STATE_NET_SEND                                                                                                                                           |
| The thread is waiting for a pulse or signal to be delivered across the network (i.e., it called MsgSendPulse(), MsgDeliverEvent(), or SignalKill()).     |
| STATE_READY                                                                                                                                              |
| The thread is waiting to be executed while the processor executes another thread of equal or higher priority.                                            |
| STATE_RECEIVE                                                                                                                                            |
| The thread is blocked on a message receive (e.g., it called MsgReceive()).                                                                               |
| STATE_REPLY                                                                                                                                              |
| The thread is blocked on a message reply (i.e., it called MsgSend(), and the server received the message).                                               |
| STATE_RUNNING                                                                                                                                            |
| The thread is being executed by a processor. The kernel uses an array (with one entry per processor in the system) to keep track of the running threads. |
| STATE_SEM                                                                                                                                                |
| The thread is waiting for a semaphore to be posted (i.e., it called SyncSemWait()).                                                                      |
| STATE_SEND                                                                                                                                               |
| The thread is blocked on a message send (e.g., it called MsgSend(), but the server hasn't yet received the message).                                     |
| STATE_SIGSUSPEND                                                                                                                                         |
| The thread is blocked waiting for a signal (i.e., it called sigsuspend()).                                                                               |
| STATE_SIGWAITINFO                                                                                                                                        |
| The thread is blocked waiting for a signal (i.e., it called sigwaitinfo()).                                                                              |
| STATE_STACK                                                                                                                                              |
| The thread is waiting for the virtual address space to be allocated for the thread's stack (parent will have called ThreadCreate()).                     |
| STATE_STOPPED                                                                                                                                            |
| The thread is blocked waiting for a SIGCONT signal.                                                                                                      |
| STATE_WAITCTX                                                                                                                                            |
| The thread is waiting for a noninteger (e.g., floating point) context to become available for use.                                                       |
| STATE_WAITPAGE                                                                                                                                           |
| The thread is waiting for physical memory to be allocated for a virtual address.                                                                         |
| STATE_WAITTHREAD                                                                                                                                         |
| The thread is waiting for a child thread to finish creating itself (i.e., it called ThreadCreate()).                                                     |

