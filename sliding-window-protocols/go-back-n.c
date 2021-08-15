#include <stdio.h>
#include <stdbool.h>

int main()
{
    int windowsize, sent = 0;
    printf("Enter window size : ");
    scanf("%d", &windowsize);
    while (true)
    {
        for (register short i = 0; i < windowsize; ++i)
        {
            printf("Frame %d has been transmitted.\n", sent++);
            if (sent == windowsize)
                break;
        }

        int acknowledgement;
        printf("\nPlease enter the last acknowledgement received : ");
        scanf("%d", &acknowledgement);

        if (acknowledgement == windowsize)
            break;
        else
            sent = acknowledgement;
    }
    return 0;
}
