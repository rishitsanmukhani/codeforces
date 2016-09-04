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

uint32_t seed = 7;
static uint32_t fastrand(){
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

class Treap{
	struct Node{
		pair<int, int> x;
		int sz, y;
		Node *l, *r;
		Node(pair<int, int> _x){
			x = _x;
			sz = 1;
			y = fastrand();
			l = r = NULL;
		}
		~Node(){
			delete l;
			delete r;
		}
		int size(){
			return sz;
		}
		void recalc(){
			sz = 1;
			if(l) sz += l->size();
			if(r) sz += r->size();
		}
	};
	typedef Node* pNode;

	void split(pNode v, pair<int, int> x, pNode &l, pNode &r) {
		l = r = NULL;
		if(!v) return;
		if(v->x <= x){
			split(v->r, x, v->r, r);
			l = v;
		}else{
			split(v->l, x, l, v->l);
			r = v;
		}
		v->recalc();
	}
	pNode merge(pNode l, pNode r){
		if(!l || !r) return l ? l : r;
		if(l->y < r->y){
			l->r = merge(l->r, r);
			l->recalc();
			return l;
		}else{
			r->l = merge(l, r->l);
			r->recalc();
			return r;
		}
	}
	public:
	pNode root;
	Treap(){
		root = NULL;
	}
	~Treap(){
		delete root;
	}
	void insert(pair<int, int> x){
		pNode l, r;
		split(root, x, l, r);
		root = merge(merge(l, new Node(x)), r);
	}
	void erase(pair<int, int> x){
		pair<int, int> y = x;
		y.second--;

		pNode l, m, r;
		split(root, y, l, m);
		split(m, x, m, r);
		root = merge(l, r);
	}
	int count(int x){
		pair<int, int> y = mp(x, 0);
		pNode l, r;
		split(root, y, l, r);
		int res = 0;
		if(r)res += r->size();
		root = merge(l, r);
		return res;
	}
};

const int maxn=int(1e5)+1;
vector<int> c(maxn), ans(maxn), vis(maxn);
vector<vector<int> > G(maxn);
vector<vector<pair<int, int> > > Q(maxn);

class Node{
public:
	map<int, int> m;
	Treap t;
	void merge(Node& node){
		if(m.size() < node.m.size()){
			m.swap(node.m);
			swap(t.root, node.t.root);
		}
		for(auto p:node.m){
			int c = p.first, cnt = p.second;
			
			if(m.find(c)!=m.end()){
				t.erase(mp(m[c], c));
				m[c] += cnt;
			}
			else{
				m[c] = cnt;
			}
			t.insert(mp(m[c], c));
		}
	}
};
vector<Node> nodes(maxn);

void dfs(int u, int p=-1){
	if(vis[u])return;
	vis[u]=1;
	nodes[u].m[c[u]] = 1;
	nodes[u].t.insert(mp(1, c[u]));
	
	int v;
	for(int i=0;i<G[u].size();i++){
		v = G[u][i];
		if(p == v)
			continue;
		dfs(v, u);
		nodes[u].merge(nodes[v]);
	}
	for(int i=0;i<Q[u].size();i++){
		ans[Q[u][i].second] = nodes[u].t.count(Q[u][i].first);
	}
}


int main(){
	int n,m,a,b;
	icin(n);icin(m);
	
	for(int i=1;i<=n;i++)
		icin(c[i]);
	
	for(int i=1;i<n;i++){
		icin(a);icin(b);
		G[a].pb(b);
		G[b].pb(a);
	}
	
	for(int i=0;i<m;i++){
		icin(a);icin(b);
		Q[a].pb(mp(b, i));
	}

	dfs(1);

	for(int i=0;i<m;i++){
		printf("%d\n",ans[i]);
	}
	return 0;
}