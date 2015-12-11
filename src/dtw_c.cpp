#include <stdlib.h>
#include <stdio.h>
#include <math.h>

extern "C"{

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))
#define dist(x,y) ((x-y)*(x-y))
#define INF 1e20       //Pseudo Infitinte number for this code

/// Data structure for sorting the query
typedef struct Index
    {   double value;
        int    index;
    } Index;

/// Data structure (circular array) for finding minimum and maximum for LB_Keogh envolop
struct deque
{   int *dq;
    int size,capacity;
    int f,r;
};


/// Sorting function for the query, sort by abs(z_norm(q[i])) from high to low
int comp(const void *a, const void* b)
{   Index* x = (Index*)a;
    Index* y = (Index*)b;
    return abs(y->value) - abs(x->value);   // high to low
}

/// Initial the queue at the begining step of envelop calculation
void init(struct deque *d, int capacity)
{
    d->capacity = capacity;
    d->size = 0;
    d->dq = (int *) malloc(sizeof(int)*d->capacity);
    d->f = 0;
    d->r = d->capacity-1;
}

/// Destroy the queue
void destroy(struct deque *d)
{
    free(d->dq);
}

/// Insert to the queue at the back
void push_back(struct deque *d, int v)
{
    d->dq[d->r] = v;
    d->r--;
    if (d->r < 0)
        d->r = d->capacity-1;
    d->size++;
}

/// Delete the current (front) element from queue
void pop_front(struct deque *d)
{
    d->f--;
    if (d->f < 0)
        d->f = d->capacity-1;
    d->size--;
}

/// Delete the last element from queue
void pop_back(struct deque *d)
{
    d->r = (d->r+1)%d->capacity;
    d->size--;
}

/// Get the value at the current position of the circular queue
int front(struct deque *d)
{
    int aux = d->f - 1;

    if (aux < 0)
        aux = d->capacity-1;
    return d->dq[aux];
}

/// Get the value at the last position of the circular queueint back(struct deque *d)
int back(struct deque *d)
{
    int aux = (d->r+1)%d->capacity;
    return d->dq[aux];
}

/// Check whether or not the queue is empty
int empty(struct deque *d)
{
    return d->size == 0;
}

/// Finding the envelop of min and max value for LB_Keogh
/// Implementation idea is intoruduced by Danial Lemire in his paper
/// "Faster Retrieval with a Two-Pass Dynamic-Time-Warping Lower Bound",
///  Pattern Recognition 42(9), 2009.
void lower_upper_lemire(double *t, int len, int r, double *l, double *u)
{
    struct deque du, dl;
    int i;

    init(&du, 2*r+2);
    init(&dl, 2*r+2);

    push_back(&du, 0);
    push_back(&dl, 0);

    for (i = 1; i < len; i++)
    {
        if (i > r)
        {
            u[i-r-1] = t[front(&du)];
            l[i-r-1] = t[front(&dl)];
        }
        if (t[i] > t[i-1])
        {
            pop_back(&du);
            while (!empty(&du) && t[i] > t[back(&du)])
                pop_back(&du);
        }
        else
        {
            pop_back(&dl);
            while (!empty(&dl) && t[i] < t[back(&dl)])
                pop_back(&dl);
        }
        push_back(&du, i);
        push_back(&dl, i);
        if (i == 2 * r + 1 + front(&du))
            pop_front(&du);
        else if (i == 2 * r + 1 + front(&dl))
            pop_front(&dl);
    }
    for (i = len; i < len+r+1; i++)
    {
        u[i-r-1] = t[front(&du)];
        l[i-r-1] = t[front(&dl)];
        if (i-front(&du) >= 2 * r + 1)
            pop_front(&du);
        if (i-front(&dl) >= 2 * r + 1)
            pop_front(&dl);
    }
    destroy(&du);
    destroy(&dl);
}

double vectorDistance(double *s, double *t, int ns, int nt, int k, int i, int j)
{
    double result=0;
    double ss,tt;
    int x;
    for(x=0;x<k;x++) {
        ss=s[i+ns*x];
        tt=t[j+nt*x];
        result+=dist(ss,tt);
    }
    //result=sqrt(result);
    return result;
}

double dtw_c(double *s, double *t, int w, int ns, int nt, int k)
{
    double d=0;
    int sizediff=ns-nt>0 ? ns-nt : nt-ns;
    double ** D;
    int i,j;
    int j1,j2;
    double cost,temp;

    // printf("ns=%d, nt=%d, w=%d, s[0]=%f, t[0]=%f\n",ns,nt,w,s[0],t[0]);


    if(w!=-1 && w<sizediff) w=sizediff; // adapt window size

    // create D
    D=(double **)malloc((ns+1)*sizeof(double *));
    for(i=0;i<ns+1;i++) {
        D[i]=(double *)malloc((nt+1)*sizeof(double));
    }

    // initialization
    for(i=0;i<ns+1;i++) {
        for(j=0;j<nt+1;j++) {
            D[i][j]=-1;
        }
    }
    D[0][0]=0;

    // dynamic programming
    for(i=1;i<=ns;i++) {
        if(w==-1) {
            j1=1;
            j2=nt;
        }
        else {
            j1= i-w>1 ? i-w : 1;
            j2= i+w<nt ? i+w : nt;
        }
        for(j=j1;j<=j2;j++) {
            cost=vectorDistance(s,t,ns,nt,k,i-1,j-1);

            temp=D[i-1][j];
            if(D[i][j-1]!=-1) {
                if(temp==-1 || D[i][j-1]<temp) temp=D[i][j-1];
            }
            if(D[i-1][j-1]!=-1) {
                if(temp==-1 || D[i-1][j-1]<temp) temp=D[i-1][j-1];
            }

            D[i][j]=cost+temp;
        }
    }

    d=D[ns][nt];

    /* view matrix D */
    //for(i=0;i<ns+1;i++) {
    //    for(j=0;j<nt+1;j++)
    //    {
    //        printf("%8.2f ",D[i][j]);
    //    }
    //    printf("\n");
    //}

    // free D
    for(i=0;i<ns+1;i++) {
        free(D[i]);
    }
    free(D);

    return sqrt(d);
}


double lb_keogh_c(double *s, double *t, int w, int ns, int nt)
{
    double d=0;
    int sizediff=ns-nt>0 ? ns-nt : nt-ns;
    int i,j;
    int start,end;
    double lb_sum;
    double *lower_bound, *upper_bound;
    int n=min(ns,nt);

    lower_bound = (double *)malloc(sizeof(double)*(ns+1));
    //if( lower_bound == NULL ) error(1);
    upper_bound = (double *)malloc(sizeof(double)*(ns+1));
    //if( upper_bound == NULL ) error(1);

    if(w>=n) w=n-1;

    // printf("ns=%d, nt=%d, w=%d, s[0]=%f, t[0]=%f\n",ns,nt,w,s[0],t[0]);

    lower_upper_lemire(t, n, w, lower_bound, upper_bound);

    lb_sum = 0;
    //printf("%f\n", lb_sum);
    for(i=0;i<n;i++) {
        if(s[i] > upper_bound[i])
            lb_sum += dist(s[i],upper_bound[i]);
        else if(s[i] < lower_bound[i])
            lb_sum += dist(s[i],lower_bound[i]);
        //printf("%f %f %f\n", lower_bound[i], upper_bound[i], lb_sum);
    }
    //printf("%f\n", lb_sum);
    free(lower_bound);
    free(upper_bound);
    return sqrt(lb_sum);
}

}
