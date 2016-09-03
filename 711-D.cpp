#include <bits/stdc++.h>
using namespace std;

#define icin(x) (scanf("%d",&x))
#define pb push_back
#define mp make_pair
#define MOD (LL(1e9)+7)
typedef long long LL;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
typedef vector<pii> vpii;

const int maxn=int(2e5)+1;
int a[maxn], vis[maxn]; 
vi cycles;

int p[maxn];

void dfs_cycle(int i){
	if(vis[i]==3)return;
	vis[i]=3;
	cycles.back()++;
	dfs_cycle(a[i]);
}
void dfs(int i){
	if(vis[i])return;
	vis[i]=2;
	if(vis[a[i]]==0){
		dfs(a[i]);
	}
	else if(vis[a[i]]==2){
		cycles.pb(0);
		dfs_cycle(i);
	}
	vis[i]=1;
}

int main(){
	int n;
	icin(n);
	for(int i=1;i<=n;i++){
		icin(a[i]);
	}
	for(int i=1;i<=n;i++){
		if(vis[i]==0)dfs(i);
	}
	p[0]=1;
	for(int i=1;i<=n;i++){
		p[i] = (p[i-1]<<1)%MOD;
	}
	LL res=1;
	int sum=0;
	for(auto k:cycles){
		sum += k;
		res = (res * ((p[k] - 2 + MOD)%MOD))%MOD;;
	}
	res = (res * p[n-sum])%MOD;
	printf("%lld\n",res);
	return 0;
}
