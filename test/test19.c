#include <string.h>
#include <system.h>

int main()
{
    int a;
    a = 5;
    {
        int b;
        b = 4;
        {
            int c;
            c = 3;
            {
                int d;
                d = 2;
                {
                    int e;
                    e = 1;
                    {
                        int f;
                        f = 0;

                        printi(f);
                        printi(e);
                        printi(d);
                    }
                    printi(c);
                }
                printi(b);
            }
            printi(a);
        }
    }
}
