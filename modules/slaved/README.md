# slaved  

## project components:
 - `slaved` daemon which is meant to be run on slaves instanced, listening for requests.
 - `master_middlewares.a` to be compiled with the master program, providing functions to interact with the `slaved` daemons, basically: `init_cluster()`, `queue_task()` and `is_done()`.

 ## implementation details:
 ### slaved
> slaved is just a requests dispatcher in a sense that it receive requests and calls a function based on request's type (i.g. `TYPE_REQ_INIT`, `TYPE_REQ_COMPUTATION`, `TYPE_REQ_FINISH`, `TYPE_REQ_ABORT`).
 ### master_middlewares.a
 > master_middlewares.a is where all the stuff happens, parsing configuration file, connecting slaves and firing a `slave_routine()` for every slave and running a load balancer, basically.
 
 the whole interaction can be summed (not very well) in this flow chart:
 ![](https://i.imgur.com/7gGhGew.png)
 #### what I learned (from what I suffered)?
 - threads are cool.
 - to put a mutex in everything.
 - to dequeue a node before enqueuing it to some other queue.
   
   
 <p align="center">
  <img src="https://i.imgur.com/06GvAzp.png" width=420>
</p>

