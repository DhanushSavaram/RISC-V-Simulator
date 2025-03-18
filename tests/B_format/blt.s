main: li a4, 0xff00ff00
	  li a5, 0x7f00ff00
	  blt a4, a5, L1
	  li a6, 0x24
	  jr ra
L1  : li a4, 0x1234
      li a5, 0x7648
	  blt a4, a5, L2
	  jr ra
L2  : li a4, 0x70123456
      li a5, 0x71234567
	  blt a4, a5, L3
	  jr ra
L3  : li a4, 0x72345679
      li a5, 0x12345678
	  blt a4, a5, L4
      jr ra
L4  : li a4, 0x10
      li a5, 0x20
      jr ra