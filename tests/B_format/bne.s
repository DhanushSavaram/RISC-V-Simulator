main: li a4, 0xff00ff00
	  li a5, 0x7ff00ff0
	  bne a4, a5, L1
	  li a6, 0x24
	  jr ra
L1  : li a4, 0x1234
      li a5, 0x1235
	  bne a4, a5, L2
	  jr ra
L2  : li a4, 0x765
      li a5, 0x765
	  bne a4, a5, L3
	  jr ra
L3  : li a4, 0x23
      li a5, 0x43
	  jr ra

