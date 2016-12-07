#include<stdio.h>
#include<stdlib.h>
#define LH 1
#define EH 0
#define RH -1
#define TRUE 1
#define FALSE 0

typedef int Status;
typedef int ElemType;



//���������
typedef struct BBSTNode
{
    ElemType data;
    int bf;//ƽ�����ӣ�balance factor��
    struct BBSTNode *lchild,*rchild;
} BBSTNode,*BBSTree;

void R_Rotate(BBSTree &p)    //��pΪ���ڵ�Ķ�����������������ת
{
    BBSTree L;
    L=p->lchild;
    p->lchild=L->rchild;
    L->rchild=p;
    p=L;//pָ���µĸ��ڵ�
}
void L_Rotate(BBSTree &p)    //��pΪ���ڵ�Ķ�����������������ת
{
    BBSTree R;
    R=p->rchild;
    p->rchild=R->lchild;
    R->lchild=p;
    p=R;
}
void LeftBalance(BBSTree &T)      //�Զ���ƽ����������ƽ�⴦��
{
    BBSTree L,Lr;
    L=T->lchild;
    switch(L->bf)      //���T��������ƽ��ȣ�������Ӧ��ƽ�⴦��
    {

    case LH:         //�½ڵ������T�����ӵ��������ϣ�������������
        T->bf=L->bf=EH;
        R_Rotate(T);
        break;
        case EH:        /*����ط���Ҫ�ر�ע�⣬����������漰��ɾ����㣬�������ͱ��뿼��*/
        T->bf = EH;
        L->bf = RH;
        R_Rotate(T);
        break;
    case RH:          //�²���ڵ���T�����ӵ��������ϣ���˫������
        Lr=L->rchild;
        switch(Lr->bf)
        {
        case LH:
            T->bf=RH;
            L->bf=EH;
            break;
            case EH:
            T->bf=L->bf=EH;
            break;
        case RH:
            T->bf=EH;
            L->bf=LH;
            break;
        }
        Lr->bf=EH;
        L_Rotate(T->lchild);
        R_Rotate(T);
    }
}
void RightBalance(BBSTree &T)          //�Զ���ƽ����������ƽ�⴦��
{
    BBSTree R,Rl;
    R=T->rchild;
    switch(R->bf)
    {
    case RH:         //�½ڵ����T���Һ��ӵ��������ϣ�Ҫ������������
        T->bf=R->bf=EH;
        L_Rotate(T);
        break;
        case EH:                 /*����ط���Ҫ�ر�ע�⣬����������漰��ɾ���ڵ㣬�������ͱ��뿼��*/
        T->bf = RH;
        R->bf = LH;
        L_Rotate(T);
        break;
    case LH:           //�½ڵ����T���Һ��ӵ��������ϣ�Ҫ��˫������
        Rl=R->lchild;
        switch(Rl->bf)
        {
        case LH:
            T->bf=EH;
            R->bf=RH;
            break;
        case EH:
            T->bf=R->bf=EH;
            break;
        case RH:
            T->bf=LH;
            R->bf=EH;
            break;
        }
        Rl->bf=EH;
        R_Rotate(T->rchild);
        L_Rotate(T);
    }
}

//��ƽ�������T������ֵΪe�Ľ��
Status InsertAVL(BBSTree &T, int e, Status &taller)//����taller��ӳT�������
{
    if(NULL == T)
    {
        T = (BBSTree)malloc(sizeof(BBSTNode));
        T->data = e;
        T->lchild = T->rchild = NULL;
        T->bf = EH;
        taller = TRUE;
    }
    else
    {
        if(e==T->data)//������
        {
            taller = FALSE;
            return FALSE;
        }
        if(e<T->data)
        {
            if(FALSE == InsertAVL(T->lchild,e,taller))//δ����
                return FALSE;
            if(taller)           //�Բ����������������������
            {
                switch(T->bf)
                {
                case LH:             //ԭ�����������������ߣ���Ҫ����ƽ�⴦��
                    LeftBalance(T);
                    taller = FALSE;
                    break;
                case EH:       //ԭ�����������ȸߣ��������������߶�������
                    T->bf=LH;
                    taller = TRUE;
                    break;
                case RH:            //ԭ�����������������ߣ��������������ȸ�
                    T->bf=EH;
                    taller = FALSE;
                    break;
                }
            }
        }
        else
        {
            //Ӧ��T������������Ѱ
            if(FALSE == InsertAVL(T->rchild,e,taller))
                return FALSE;
            if(taller)     //������������������������
            {
                switch(T->bf)
                {
                case LH:          //ԭ�����������������ߣ��������������ȸ�
                    T->bf = EH;
                    taller = FALSE;
                    break;
                case EH:               //ԭ�����������ȸߣ��������������
                    T->bf = RH;
                    taller = TRUE;
                    break;
                case RH:       //ԭ�����������������ߣ�����������ƽ�⴦��
                    RightBalance(T);
                    taller = FALSE;
                    break;
                }
            }
        }
    }
    return TRUE;
}

//��ƽ������������
int Depth(BBSTree T)
{
    if(NULL == T) return 0;
    int l,r;
    l = Depth(T->lchild);
    r = Depth(T->rchild);
    return l>r?(l+1):(r+1);
}

int TreeBf(BBSTree T)
{
    return Depth(T->lchild)-Depth(T->rchild);
}

//ɾ��ƽ�����������ֵΪe�Ľ��
Status DeleteAVL(BBSTree &T, ElemType e, Status &shorter)
{
    if(T == NULL)
    {
        return FALSE;
    }
    else if(e == T->data)		//ɾ��
    {
        BBSTree temp = T;
        if(T->lchild == NULL)
        {
            T = T->rchild;
            free(temp);
            shorter = TRUE;
        }
        else if(T->rchild == NULL)
        {
            T = T->lchild;
            free(temp);
            shorter = TRUE;
        }
        else
        {
            BBSTree p = T->lchild;
            while(p->rchild)
            {
                p = p->rchild;
            }
            T->data = p->data;
            DeleteAVL(T->lchild,p->data,shorter);
            if(shorter == TRUE)
            {
                  switch(T->bf)
                {
                case LH:
                    T->bf = EH;
                    shorter = TRUE;
                    break;
                case EH:				//�����߶Ȳ����͵��������:�����Ҷ��н�㣬ɾ��һ����Ӱ�������߶�
                    T->bf = RH;
                    shorter = FALSE;
                    break;
                case RH:
                    RightBalance(T);		//������ƽ��
                        if(T->rchild->bf == EH)   //ɾ��ʱ������������Һ��ӽ��bfΪ0�������߶Ȳ���
                    {
                        shorter = FALSE;
                    }
                    else
                    {
                        shorter = TRUE;
                    }
                    break;
                }
            }

        }
    }
    else if(e < T->data)		//�������м�������
    {
        if(!DeleteAVL(T->lchild,e,shorter))//ɾ��ʧ��ֱ��return flase
        {
            return FALSE;
        }
        if(shorter)	//�������н��ɾ���ɹ�,���������߶Ƚ���
        {
            switch(T->bf)
            {
            case LH:
                T->bf = EH;
                shorter = TRUE;
                break;
                case EH:	/*�����߶Ȳ����͵��������:�����Ҷ��н�㣬ɾ��һ����Ӱ�������߶�*/
                T->bf = RH;
                shorter = FALSE;
                break;
            case RH:
                RightBalance(T);		//������ƽ��
                    if(T->rchild->bf == EH)/*ɾ��ʱ������������Һ��ӽ��bfΪ0�������߶Ȳ���*/
                {
                    shorter = FALSE;
                }
                else
                {
                    shorter = TRUE;
                }
                break;
            }
        }
    }
    else				//�������м�������
    {
        if(!DeleteAVL(T->rchild,e,shorter))
        {
            return FALSE;
        }
        if(shorter)
        {
            switch(T->bf)
            {
            case LH:
                LeftBalance(T);
                    if(T->lchild->bf == EH)       /*ɾ��ʱ��������������ӽ��bfΪ0�������߶Ȳ���*/
                {
                    shorter = FALSE;
                }
                else
                {
                    shorter = TRUE;
                }
                break;
            case EH:
                T->bf = LH;
                shorter = FALSE;
                break;
            case RH:
                T->bf = EH;
                shorter = TRUE;
                break;
            }
        }
    }
    return TRUE;
}


//��ӡƽ�������
void PrintBBSTree(BBSTree T)
{
    if(T)
    {
        printf("%d",T->data);
        int i = TreeBf(T);
        printf("[%d]",i);
        if(T->lchild||T->rchild)
        {
            printf("(");
            PrintBBSTree(T->lchild);
            printf(",");
            PrintBBSTree(T->rchild);
            printf(")");
        }
    }
}

//�������ƽ�������
void InorderTraverse(BBSTree T)
{
   if(NULL == T) return;
   InorderTraverse(T->lchild);
   printf("%d  ",T->data);
   InorderTraverse(T->rchild);
}

int main()
{
    int i,select,data;
    ElemType A[]= {3,2,1,4,5,6,7,10,9,8};       //Ԥ�Ȳ���һЩ����
    BBSTree T = NULL;
    Status taller = FALSE, shorter = FALSE;
    for(i=0; i<10; i++)
    {
        InsertAVL(T,A[i],taller);
    }
    printf("���ڵ�ƽ�������ֵΪ��\n");
    PrintBBSTree(T);
    printf("\n");
    do
    {

        printf("���� 1 ������\n");
        printf("���� 2 ɾ�����\n");
        printf("���� 3 ��ӡƽ�������\n");
        printf("���� 4 �˳�����\n");
        printf("���� 5 �������ƽ�������\n");
        printf("���������ѡ��\n");
        scanf("%d",&select);
        switch(select)
        {

        case 1:
            printf("��������Ҫ����Ľ��ֵ\n");
            scanf("%d", &data);
            if(FALSE == InsertAVL(T,data,taller))
                printf("�����������������ֵ�Ƿ�����\n");
            else printf("����ɹ���\n");
            break;
        case 2:
            printf("��������Ҫɾ���Ľ��ֵ\n");
            scanf("%d", &data);
            if(FALSE == DeleteAVL(T,data,shorter))
                printf("ɾ����������ɾ������ֵ�Ƿ�����\n");
            else printf("ɾ���ɹ�!\n");
            break;
        case 3:
            printf("����ƽ������������н��Ϊ��\n");
            PrintBBSTree(T);
            printf("\n");
            break;
        case 4:
            break;
        case 5:
            InorderTraverse(T);
            printf("\n");
            break;
        default:
            printf("��������������ѡ��\n");
            break;

        }

    }
    while(select!=4);               //������4ʱ�˳����򣬷������½���ѭ����
    printf("�������˳���лл��\n");
    return 0;
}
