#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct LinkList
{
    struct LinkList *next;
    int value;
} LinkList;

LinkList *GenerateList(int len)
{
    srand(time(NULL));
    LinkList *head = NULL;
    for (int i = 0; i < len; ++i)
    {
        LinkList *p = (LinkList *)malloc(sizeof(LinkList));
        if (head == NULL)
        {
            head = p;
            p->next = NULL;
            p->value = rand() % 100;
        }
        else
        {
            p->next = head;
            p->value = rand() % 100;
            head = p;
        }
    }
    return head;
}

void DestructList(LinkList *head)
{
    while (head != NULL)
    {
        LinkList *p = head;
        head = head->next;
        free(p);
    }
}

void DisplayList(LinkList *head)
{
    while (head != NULL)
    {
        printf("%2d ", head->value);
        head = head->next;
    }
    printf("\n");
}

LinkList *ReverseList(LinkList *head)
{
    LinkList *tail = head;
    LinkList *next = head->next;
    tail->next = NULL;

    while (next != NULL)
    {
        head = next;
        next = next->next;
        head->next = tail;
        tail = head;
    }

    return head;
}

int main()
{
    LinkList *head = GenerateList(10);
    DisplayList(head);
    head = ReverseList(head);
    DisplayList(head);
}