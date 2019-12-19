void write_mem8(int addr, int data);
void io_hlt(void);
void HariMain(void)
{
	int i;
	for (i = 0xa0000; i <= 0xaffff; i++)
	{
		write_mem8(i, i & 0x0f);
	}
	while (1)
		io_hlt();
}