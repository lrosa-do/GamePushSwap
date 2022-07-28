
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                 // Required for GUI controls

#define MEMORY_IMPLEMENT
#include "mem.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


   const int screenWidth = 800;
   const int screenHeight = 750;


typedef struct Node
{
    int index;
    int value;
    Color c;
    struct Node* next;
}Node;

typedef struct Stack
{
    int size;
    Node *top;
}Stack;


Stack *createStack()
{
    Stack *stack=(Stack*)mem_malloc(sizeof(Stack));
    stack->top=NULL;
    stack->size=0;
    return stack;
}

int is_emptyStack(Stack *s)
{
    return (s->size == 0);
}


int popStack(Stack *s,Color *c,int *index)
{
    int value=-1;
    if (!s)
        return (value);
    if (!s->top)
        return (value);

    if (s->top->next==NULL)
    {

        value = s->top->value;
        *c = s->top->c;
        *index =s->top->index;

        mem_free(s->top);
        s->top=NULL;

        printf("POP  head: %d\n",value);
        s->size -= 1;
        return value;
    }


    Node* second_last = s->top;
    while (second_last->next->next != NULL)
    second_last = second_last->next;

    value = second_last->next->value;
    *c = second_last->next->c;
    *index =second_last->next->index;

    mem_free(second_last->next);
    second_last->next=NULL;

    printf("POP : %d\n",value);
    s->size -= 1;


    return value;
}

int popFirst(Stack *s,Color *c,int *index)
{
    int value=-1;
    if (!s)
        return (value);
    if (s->top)
    {

        Node* n  = s->top;
        value = n->value;
        *index = n->index;
        *c = n->c;
        if (s->top->next)
            s->top = n->next;
        else
            s->top = NULL;
        mem_free(n);
        n=NULL;
        printf("POP : %d\n",value);
        s->size -= 1;
    }
    return value;
}


void pushStack(Stack *s,int value,Color c)
{
    if(!s)
        return;
    if (!s->top)
    {
        s->top= (Node*) mem_malloc(sizeof(Node));
        s->top->next  = NULL;
        s->top->value = value;
        s->top->index=(s->size+1);
        s->top->c=c;
    } else
    {

       Node *n = (Node*) mem_malloc(sizeof(Node));
       n->value = value;
       n->index=(s->size+1);
       n->next=NULL;
       n->c=c;
       Node *current=s->top;
       while(current->next!=NULL)
            current = current->next;
       current->next=n;
       current->next->next=NULL;


    }
    s->size +=1;
}

//for rotate down remove the
void shiftDown(Stack *s)
{
    Color c;
    int i;
    int v = popFirst(s,&c,&i);
    pushStack(s,v,c);

}
// for rotate up remove to node from the top and add ne node at last
void shiftUp(Stack *s)
{
    Color c;
    int i;
    int v = popStack(s,&c,&i);

    if (s->top)
    {
       Node *n = (Node*) mem_malloc(sizeof(Node));
       n->next  = s->top;
       n->value = v;
       n->c=c;
       n->index=s->size+1;
       s->top=n;
       s->size +=1;

    }
}


void clearStack(Stack *stack)
{
 if(stack)
 {
    Color c;
    int i;
    if (stack->top)
        while(!is_emptyStack(stack))
            popStack(stack,&c,&i);
 }
}
void freeStack(Stack *stack)
{
 if(stack)
 {
  mem_free(stack);
  stack=NULL;
 }
}

void DisplayStack(Stack *s)
{
    if (!s)
        return;
   printf("--------------------------- \n");
   printf("Totals Nodes :%d  is empty: %d \n",s->size,is_emptyStack(s));


    if (!s->top)  return;
    Node * n = s->top;
    int index=1;
    while (n)
    {
        printf(" Node:%d  value:%d \n",index,n->value);
        n = n->next;
        index++;
    }
}

void DisplayStackA(Stack *s)
{
    if (!s)
        return;

  // printf("Totals Nodes :%d  is empty: %d \n",s->size,is_emptyStack(s));

  int isSorted=1;

  if (s->top)
  {
    if (s->top->next)
    {
      for (Node *t=s->top; t->next != NULL; t=t->next)
           if (t->value <= t->next->value)
           {
              isSorted=0;
              break;
            }
        }
    }


   DrawText(TextFormat("Stack A - Nodes(%d) Is Empty(%d)",s->size,is_emptyStack(s)), 20, 5, 10, GREEN);

   if (isSorted==1 && s->size>=10)
    {
      DrawText("Stack A is sorted ", 30, 20, 12, WHITE);
    }

    if (!s->top)  return;
    Node * n = s->top;
    int index=1;
    while (n)
    {
        int value =n->value;
        Color c=n->c;
        float w = 100 +value;
        int offSetX= screenWidth/4+20-(w/2);
        int offSetY= screenHeight-200- (index*32);
        DrawRectangle(offSetX,offSetY,w,30,c);
        DrawText(TextFormat("Value %d",value),offSetX+5,offSetY+10,10,WHITE);
        n = n->next;
        index++;
    }
}


void DisplayStackB(Stack *s)
{
    if (!s)
        return;

  // printf("Totals Nodes :%d  is empty: %d \n",s->size,is_emptyStack(s));

   DrawText(TextFormat("Stack A - Nodes(%d) Is Empty(%d)",s->size,is_emptyStack(s)), screenWidth-200, 5, 10, RED);


    if (!s->top)  return;
    Node * n = s->top;
    int index=1;
    while (n)
    {
        //printf(" Node:%d  value:%d \n",index,n->value);
        int value =n->value;
        Color c=n->c;
        float w = 100 +value;
        int offSetX= screenWidth-200-(w/2);
            int offSetY= screenHeight-200- (index*32);


        DrawRectangle(offSetX,offSetY,w,30,c);
        DrawText(TextFormat("Value %d",value),offSetX+5,offSetY+10,10,WHITE);
        n = n->next;
        index++;
    }
}
int arrayContains(int array[],int value,int count)
{
        for (int i=0;i<count;i++)
        {
            if (array[i]==value)
                return 1;
        }
        return 0;
}

void arrayAdd(int array[],int value,int count)
{
    int index=0;
    while (index<count)
    {
        if (!arrayContains(array,value,count))
        {

          array[index] = value;

        } else
        {
          index++;
        }


    }
}

Stack * a_stack;
Stack * b_stack;
int opetations=0;


void UpdateDrawFrame(void)
{

        BeginDrawing();

            ClearBackground(BLACK);


            //post e bases
            DrawRectangle(screenWidth/2-(screenWidth/4)+10, screenHeight-600, 20, 400, WHITE);
            DrawRectangle(screenWidth/2+(screenWidth/4)-10, screenHeight-600, 20, 400, WHITE);

            DrawRectangleLines(screenWidth/2-(screenWidth/4)+10, screenHeight-600, 20, 400, RED);
            DrawRectangleLines(screenWidth/2+(screenWidth/4)-10, screenHeight-600, 20, 400, RED);

              DrawRectangle(screenWidth/2-(screenWidth/4)-60, screenHeight-180, 500, 160, DARKGRAY);

              DisplayStackA(a_stack);
              DisplayStackB(b_stack);


              if (GuiButton((Rectangle){200,screenHeight-160,60,20},"Push B"))
              {
                    Color c;
                    int i;
                    int v = popStack(a_stack,&c,&i);
                    if (b_stack->size<10)
                    {
                        pushStack(b_stack,v,c);
                        opetations++;
                    }

              }

              if (GuiButton((Rectangle){550,screenHeight-160,60,20},"Push A"))
              {
                    Color c;
                    int i;
                    int v = popStack(b_stack,&c,&i);

                    if (a_stack->size<10)
                    {
                        pushStack(a_stack,v,c);
                        opetations++;
                    }
              }


              if (GuiButton((Rectangle){200,screenHeight-130,60,20},"Swap A"))
              {
                    Color ac;
                    int ai;
                    int av = popStack(a_stack,&ac,&ai);


                    Color bc;
                    int bi;
                    int bv = popStack(a_stack,&bc,&bi);


                     pushStack(a_stack,av,ac);
                     pushStack(a_stack,bv,bc);

                        opetations++;


              }

             if (GuiButton((Rectangle){550,screenHeight-130,60,20},"Swap B"))
              {
                    Color ac;
                    int ai;
                    int av = popStack(b_stack,&ac,&ai);


                    Color bc;
                    int bi;
                    int bv = popStack(b_stack,&bc,&bi);


                     pushStack(b_stack,av,ac);
                     pushStack(b_stack,bv,bc);

                        opetations++;


              }

                if (GuiButton((Rectangle){190,screenHeight-90,80,20},"Rotate  RA"))
              {
                   shiftUp(a_stack);
                   opetations++;
              }

              if (GuiButton((Rectangle){190,screenHeight-60,80,20},"Rotate  RRA"))
              {
                   shiftDown(a_stack);
                   opetations++;
              }

                if (GuiButton((Rectangle){540,screenHeight-90,80,20},"Rotate  RB"))
              {
                   shiftUp(b_stack);
                   opetations++;
              }

              if (GuiButton((Rectangle){540,screenHeight-60,80,20},"Rotate  RRB"))
              {
                   shiftDown(b_stack);
                   opetations++;
              }
                 DrawText(TextFormat("Operations %d",opetations), (screenWidth/2)-MeasureText(TextFormat("Operations %d",opetations),14), 5, 14, WHITE);
                 DrawText("A", (screenWidth/2)-200, 40, 60, WHITE);
                 DrawText("B", (screenWidth/2)+180, 40, 60, WHITE);




        EndDrawing();

    

}

int main(void)
{



    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(screenWidth, screenHeight, "Game PushSwap 42");


    int defaults[]={20,40,60,80,100,120,140,160,180,200};
     Color defaultsColors[]={RED,GREEN,BLUE,PINK,MAROON,GOLD,ORANGE,DARKGREEN,SKYBLUE,LIME,SKYBLUE,DARKPURPLE};

    int array[10];



 a_stack=createStack();
 b_stack=createStack();

 int index=0;

while (1)
{

    int value =defaults[GetRandomValue(0,9)];

    if (!arrayContains(array,value,10))
    {
        array[index]=value;
        index++;
    }
    if(index>=9)
    {
        break;
    }

}

for (int i=0;i<10;i++)
{
pushStack(a_stack,array[i],defaultsColors[i]);
//pushStack(a_stack,defaults[i],defaultsColors[i]);
}






#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif



    clearStack(a_stack);
    freeStack(a_stack);

    clearStack(b_stack);
    freeStack(b_stack);

    mem_dump(0);


    CloseWindow();

    return 0;
}
