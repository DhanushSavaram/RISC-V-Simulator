main: li a4, 0x7f00ff00
	  li a5, 0xff00ff00
	  bltu a4, a5, L1
	  li a6, 0x24
	  jr ra
L1  : li a4, 0x1234
      li a5, 0x7864
	  bltu a4, a5, L2
	  jr ra
L2  : li a4, 0xf1234567
      li a5, 0xf7863525
	  bltu a4, a5, L3
	  jr ra
L3  : li a4, 0xf1230000
      li a5, 0xf0000000
	  bltu a4, a5, L4
      jr ra
L4  : li a4, 0x10
      li a5, 0x20
      jr ra