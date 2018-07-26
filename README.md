### rsys_edit 

This is a tool for viewing and (optionally) editing the entries in a [recrypt.sys](http://www.iquebrew.org/index.php?title=Recrypt.sys) file from an iQue Player.

Usage:

```
	rsys_edit -i [recrypt.sys] -v2 [Virage2 dump] (-edit)
```

If editing, the changes will be saved in a separate file in the same directory named "new_recrypt.sys".

Signing uses the ECDSA implementation in [ninty-233](https://github.com/jbop1626/ninty-233).  

AES implementation is from [tiny-AES](https://github.com/kokke/tiny-AES-c) by [kokke](https://github.com/kokke).    
