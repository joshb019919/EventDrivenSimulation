# EventDrivenSimulation
Event driven simulation of customers in a bank line.

# Usage
Given a Customer and Event class, an implementation of an event loop,
and a loader function, customers from a text file can be inserted into
a queue.  The file, processor.h, removes customers from the queue and
inserts events into a priority queue to be processed by the event
loop.

The only file I wrote was processor.h, so that's the only one I'm
uploading.
