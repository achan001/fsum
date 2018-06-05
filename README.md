# fsum
Accurate summation using ***Shewchuk algorithm***
(*assumed* double-precision, round-to-nearest mode)

Filesnames *NOT* end with "0" used a modified Shewchuk algorithm, saving partials only if it overflow 53 bits.

All partials (except last) used up all 53 bits, generated very few partials.  This allowed processing speed about
2X of original.  However, partials might be over-lapping, require adding 0.0 to break it up.

Filename ends with "0" used original Shewchuk algorithm.

All partials is sorted and non-overlapping.  It might generate lots of partials to sum, thus much slower.  
Getting the final sum involved loops (in reverse), slightly more complicated than my modified version  

***NOTE AGAIN:*** above codes *assumed* double-precision, round-to-nearest mode  

For C/C++ codes: see [issue: Minimum Working Example](https://github.com/achan001/fsum/issues/1)  
For Lua codes: see [Setting Float Precision in Lua.c](https://marc.info/?l=lua-l&m=152815885306143&w=2)
