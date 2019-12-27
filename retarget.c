
#include <stdio.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sbi.h>
#include <uart8250.h>

void* memset(void* dest, int byte, size_t len)
{
  if ((((uintptr_t)dest | len) & (sizeof(uintptr_t)-1)) == 0) {
    uintptr_t word = byte & 0xFF;
    word |= word << 8;
    word |= word << 16;
    word |= word << 16 << 16;

    uintptr_t *d = dest;
    while (d < (uintptr_t*)(dest + len))
      *d++ = word;
  } else {
    char *d = dest;
    while (d < (char*)(dest + len))
      *d++ = byte;
  }
  return dest;
}

void* memcpy(void* dest, const void* src, size_t len)
{
  if ((((uintptr_t)dest | (uintptr_t)src | len) & (sizeof(uintptr_t)-1)) == 0) {
    const uintptr_t* s = src;
    uintptr_t *d = dest;
    while (d < (uintptr_t*)(dest + len))
      *d++ = *s++;
  } else {
    const char* s = src;
    char *d = dest;
    while (d < (char*)(dest + len))
      *d++ = *s++;
  }
  return dest;
}

int _read(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        ptr[i] = uart8250_getc();
    }

    return len;
}

int _write(int file, char *ptr, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        if (ptr[i] == '\n')
        {
            uart8250_putc('\r');
            //sbi_console_putchar('\r');
        }
        uart8250_putc(ptr[i]);
        //sbi_console_putchar(ptr[i]);
    }

    return len;
}

int _lseek(int file, int ptr, int dir)
{
    //errno = ESPIPE;
    return -1;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int fd)
{
    //errno = ENOTTY;
    return 0;
}


void* _sbrk(int increment)
{
    extern char __heap_end; // From linker script
    static char* heap_end = &__heap_end;

    char* current_heap_end = heap_end;
    heap_end += increment;
    return current_heap_end;
}

void _exit(int return_value)
{
    asm ("fence rw, rw" ::: "memory");
    while (1)
    {
        asm ("wfi" ::: "memory");
    }
}

int _getpid(void)
{
  return 1;
}

int _kill(int pid, int sig)
{
    //errno = EINVAL;
    return -1;
}

int hart_id;
extern int main();
void _init(int _hart_id){

    hart_id = _hart_id;
    int ret = main();
    _exit(ret);

}