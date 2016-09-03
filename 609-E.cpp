#include <bits/stdc++.h>
using namespace std;

#define icin(x) (scanf("%d",&x))
#define pb push_back
#define mp make_pair
typedef long long LL;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> pii;
typedef vector<pii> vpii;

class DSU{
public:
	vector<int32_t> parent;
	vector<int32_t> rank;
	vector<int32_t> size;		//Size of set
	int32_t number_of_sets;
	DSU(int32_t _n){
		number_of_sets=_n;
		parent.resize(_n+1);
		rank.resize(_n+1);
		size.resize(_n+1);
		for(int32_t i=1;i<=_n;i++){
			parent[i]=i;rank[i]=1;size[i]=1;
		}
	}
	~DSU(){
		number_of_sets=0;
		parent.clear();rank.clear();size.clear();
	}
	int32_t root(int32_t i){
		return (parent[i]==i) ? i : (parent[i] = root(parent[i]));
	}
	int32_t sizeSet(int32_t i){
		return (parent[i]==i) ? size[i] : size[i] = size[root(i)];
	}
	void unionSet(int32_t i,int32_t j){
		i=root(i);j=root(j);
		if(i==j)return;
		number_of_sets--;
		if(rank[i]<rank[j])swap(i,j);
		if(rank[i]==rank[j])rank[i]++;
		parent[j]=i;
		size[i] += size[j]; 
	}
	bool isSameSet(int32_t i,int32_t j){
		return root(i)==root(j);
	}
};
const int maxn = int(2e5)+1, maxm = int(2e5)+1, log_maxn=18;

vector<vpii> G(maxn);
vector<pair<int, pii> > edge_list, edge_list_copy;

LL kruskal(int n){
	sort(edge_list.begin(), edge_list.end());
	DSU dsu(n);
	int u,v,w;
	LL cost=0;
	for(auto e:edge_list){
		w = e.first;
		u = e.second.first, v = e.second.second;
		if(!dsu.isSameSet(u, v)){
			dsu.unionSet(u, v);
			G[u].pb(mp(w, v));
			G[v].pb(mp(w, u));
			cost += w;
		}
	}
	return cost;
}

pair<int,int> dp[log_maxn][maxn];
int vis[maxn], L[maxn], par[maxn];
int n, m, log_n;

int logBinary(unsigned int n){
	if(n==0)return-1;
	int p = 0;
	if (n>=1<<16){ n>>= 16; p += 16;}
	if (n>=1<< 8){ n>>=  8; p += 8; }
	if (n>=1<< 4){ n>>=  4; p += 4; }
	if (n>=1<< 2){ n>>=  2; p += 2; }
	if (n>=1<< 1){          p += 1; }
	return p;
}
void DFS(int u){
	if(vis[u])return;
	vis[u]=1;
	int v, w;
	for(int i=0;i<G[u].size();i++){
		w=G[u][i].first, v = G[u][i].second;
		if(par[u] == v)
			continue;
		par[v] = u;
		L[v] = L[u] + 1;
		dp[0][v] = mp(u, w);
		DFS(v);
	}
}
void process(){
	pair<int,int> p1,p2;
	for(int i=1;i<=log_n;i++){
		for(int j=1;j<=n;j++){
			if(dp[i-1][j].first != -1){
				p1 = dp[i-1][j];
				p2 = dp[i-1][p1.first];
				dp[i][j].first = p2.first;
				dp[i][j].second = max(p1.second ,p2.second);
			}
		}
	}
}

int LCA(int a,int b){
	if(a==b)
		return 0;
	if(L[a]<L[b])
		swap(a,b);
	int maxi=int(-1e9);
	for(int i=log_n;i>=0;i--){
		if(L[a]-(1<<i)>=L[b]){
			maxi=max(maxi,dp[i][a].second);
			a=dp[i][a].first;
		}
	}
	if(a==b)
		return maxi;
	for(int i = log_n; i >= 0; i--){
		if(dp[i][a].first != dp[i][b].first){
			maxi = max(maxi, dp[i][a].second);
			a = dp[i][a].first;
			maxi = max(maxi, dp[i][b].second);
			b = dp[i][b].first;
		}
	}
	maxi=max(maxi,dp[0][a].second), a=dp[0][a].first;
	maxi=max(maxi,dp[0][b].second), b=dp[0][b].first;
	return maxi;
}

int main(){
	icin(n);icin(m);
	log_n = logBinary(n);

	int u, v, w;
	edge_list.resize(m);
	edge_list_copy.resize(m);
	for(int i=0;i<m;i++){
		icin(u);icin(v);icin(w);
		edge_list[i] = mp(w, mp(u, v));
		edge_list_copy[i] = edge_list[i];
	}
	LL cost = kruskal(n);

	for(int i=0;i<=log_n;i++)
		for(int j=1;j<=n;j++)
			dp[i][j] = mp(-1,0);
	DFS(1);
	process();
	for(auto e:edge_list_copy){
		int a=e.second.first, b = e.second.second, w = e.first;
		printf("%lld\n", cost + w - LCA(a, b)); 
	}
	return 0;
}