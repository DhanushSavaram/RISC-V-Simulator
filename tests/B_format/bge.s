main: li a4, 0x7f00ff00
	  li a5, 0xff00ff00
	  bge a4, a5, L1
	  li a6, 0x24
	  jr ra
L1  : li a4, 0x7689
      li a5, 0x1234
	  bge a4, a5, L2
	  jr ra
L2  : li a4, 0x12345678
      li a5, 0xf1234567
	  bge a4, a5, L3
	  jr ra
L3  : li a4, 0x12345678
      li a5, 0x12345678
	  bge a4, a5, L4
      jr ra
L4  : li a4, 0x10
      li a5, 0x20
      jr ra