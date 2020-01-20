#include "bootpack.h" 
void fifo8_init(struct FIFO8* fifo,int size,unsigned char*buf)
{
    fifo->buf=buf;
    fifo->flag=0;
    fifo->free=size;
    fifo->next_r=0;
    fifo->next_w=0;
    fifo->size=size;
    return;
}
#define FLAGS_OVERFLOOR 0x0001;
int fifo8_put(struct FIFO8*fifo,unsigned char data)
{
    if(fifo->free==0)
    {
        fifo->flag=FLAGS_OVERFLOOR;
        return -1;
    }else
    {
        fifo->buf[fifo->next_w]=data;
        fifo->free--;
        fifo->next_w=(fifo->next_w+1)%fifo->size;
        return 0;
    }
}
int fifo8_get(struct FIFO8 *fifo)
{
    int data;
    if(fifo->free==fifo->size)
        return -1;
    data=fifo->buf[fifo->next_r];
    fifo->free++;
    fifo->next_r=(fifo->next_r+1)%(fifo->size);
    return data;
}
int fifo_status(struct FIFO8 *fifo)
{
    return fifo->size-fifo->free;
}
