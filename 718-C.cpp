#include <bits/stdc++.h>
using namespace std;

#define MOD (int(1e9)+7)
#define icin(x) scanf("%d",&x)
typedef long long LL;

inline int mul(int a, int b){
  LL tmp = LL(a)*b;
  if(tmp>=MOD)tmp%=MOD;
  return tmp;
}
inline int add(int a,int b){
  int tmp = a+b;
  if(tmp>=MOD)tmp%=MOD;
  return tmp;
}
struct Vec{
  int v[2];
  Vec& operator=(const Vec& rhs){
    v[0]=rhs.v[0];
    v[1]=rhs.v[1];
    return *this;
  }
};
struct Matrix{
	int v[2][2];
  Matrix& operator=(const Matrix& rhs){
    for(int i=0;i<2;i++){
      for(int j=0;j<2;j++){
        v[i][j] = rhs.v[i][j];
      }
    }
    return *this;
  }
  void print(){
    for(int i=0;i<2;i++){
      for(int j=0;j<2;j++){
        cout<<v[i][j]<<" ";
      }
      cout<<endl;
    }
  }
};
inline Vec operator+(const Vec& lhs, const Vec& rhs){
  Vec res;
  res.v[0] = add(lhs.v[0], rhs.v[0]);
  res.v[1] = add(lhs.v[1], rhs.v[1]);
  return res;
}
inline Vec operator*(const Matrix& lhs, const Vec& rhs){
  Vec res;
  res.v[0] = add(mul(lhs.v[0][0], rhs.v[0]), mul(lhs.v[0][1], rhs.v[1]));
  res.v[1] = add(mul(lhs.v[1][0], rhs.v[0]), mul(lhs.v[1][1], rhs.v[1]));
  return res;
}
inline Matrix operator+(const Matrix& lhs, const Matrix& rhs){
  Matrix res;
  for(int i=0;i<2;i++){
    for(int j=0;j<2;j++){
      res.v[i][j] = add(lhs.v[i][j], rhs.v[i][j]);
    }
  }
	return res;
}
inline Matrix operator*(const Matrix& lhs, const Matrix& rhs){
	Matrix res;
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			res.v[i][j]=0;
			for(int k=0;k<2;k++){
				res.v[i][j] = add(res.v[i][j], mul(lhs.v[i][k], rhs.v[k][j]));
			}
		}
	}
	return res;
}

Vec F;
Matrix T, I;
vector<Matrix> p(32);
void precompute(){
	F.v[0] = F.v[1] = 1;
	I.v[0][1] = I.v[1][0] = 0;
	I.v[0][0] = I.v[1][1] = 1;
	T.v[0][0] = 0;
	T.v[0][1] = T.v[1][0] = T.v[1][1] = 1;
	p[0]=T;
	for(int i=1;i<32;i++)p[i]=p[i-1]*p[i-1];
}
inline Matrix power(Matrix m, int e){
	Matrix res = I;
	int i=0;
	while(e){
	  if(e&1)res = (res * p[i]);
	  e >>= 1;
	  i++;
	}
	return res;		
}

struct Node{
	Vec sum;
	Matrix lazy;
};

int A[int(1e5)+1];
Node tree[1<<18];

void build(int from,int to,int index){
	if(from==to){
    tree[index].sum = (power(T, A[from]-1)*F);
		tree[index].lazy = I;
		return;
	}
	int mid = (from+to)>>1;
	build( from,mid,index<<1 );
	build( mid+1,to,(index<<1)|1);
	
  tree[index].sum = tree[index<<1].sum + tree[(index<<1)|1].sum;
	tree[index].lazy = I;
}
Vec query(int from,int to,int index,int a,int b,Matrix value){
	if(from==a && to==b){
    return value*tree[index].sum;
	}
  value = value*tree[index].lazy;
	int mid = (from+to)>>1;
	if(a > mid)
	{
		return query(mid+1,to,(index<<1)|1,a,b,value);
	}
	else if(b <= mid){
		return query(from,mid,index<<1,a,b,value);
	}
	else{
		return query(from,mid,index<<1,a,mid,value) + query(mid+1,to,(index<<1)|1,mid+1,b,value);
	}
}

void update(int from,int to,int index,int a,int b,Matrix value){
	if(from==a && to==b){
		tree[index].sum = value*tree[index].sum;
		tree[index].lazy = value*tree[index].lazy;
		return;
	}
	int mid = (from+to)>>1;
	if(a>mid){
		update(mid+1,to,(index<<1)|1,a,b,value);
	}
	else if(b<=mid){
		update(from,mid,index<<1,a,b,value);
	}
	else{
		update(from,mid,index<<1,a,mid,value);
		update(mid+1,to,(index<<1)|1,mid+1,b,value);
	}  
	tree[index].sum = tree[index].lazy*(tree[(index<<1)].sum + tree[(index<<1)|1].sum);
}


int main(){
	precompute();
	int n,m,a;
	icin(n);icin(m);
	for(int i=1;i<=n;i++){
		icin(A[i]);
	}
	build(1,n,1);
	int t, l,r;
	int x;
	for(int i=0;i<m;i++){
		icin(t);
		icin(l);
		icin(r);
		if(t==1){
			icin(x);
			update(1, n, 1, l, r, power(T,x));
		}
		else{
			printf("%d\n",query(1, n, 1, l, r,I).v[0]);
		}
	}
	return 0;
}