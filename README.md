# cache-simulator
For this project I worked on understanding the impact of cache
memories on program performance. There were two parts to this 
project: Csim.c is a program that simulates the behavior of a 
cache memory, and Trans.c is a program that optimizes the 
transformation functions of specific sized matrices.

This project does not study a real cache, but a small made-up cache
that is assessed according to a sequence of memory references from
real programs, which are replayed against the cache simulator.

Csim.c was a cache simulator that worked for any arbitrary s, E, and 
b. Space was allocated for the cache's structure using malloc.
The cache performance was returned by printing out the hits, misses,
and evictions for the given cache size.

Trans.c was about writing a transpose function that would cause as
few cache misses as possible. This was done for three different cache
sizes. This was possible by using a blocking technique inside the cache
instead of going row by row and column by column.
