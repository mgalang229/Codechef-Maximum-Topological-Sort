#include <bits/stdc++.h>

using namespace std;

const int M = (1 << 20) + 5;
const int md = 1e9 + 7;

int sz[M], dp[M], fact[M], inv_fact[M], parent[M];
vector<int> adj[M];
int n;

int pwr(int a, int n, int m) {
	int p = 1;
	while (n) {
		if (n & 1) {
			p = 1LL * p * a % m;
		}
		a = 1LL * a * a % m;
		n >>= 1;
	}
	return p;
}

void subtree_sizes(int u, int par) {
	sz[u] = 1;
	parent[u] = par;
	for (auto v : adj[u]) {
		if (v == par) {
			continue;
		}
		subtree_sizes(v, u);
		sz[u] += sz[v];
	}
}

int get_max(int u) {
	int cur_root = u;
	bool flag = true;
	while (flag) {
		flag = false;
		for (auto v : adj[cur_root]) {
			if (v == parent[cur_root]) {
				continue;
			}
			if (2 * sz[v] > n) {
				cur_root = v;
				flag = true;
				break;
			}
		}
	}
	for (auto v : adj[cur_root]) {
		if (v == parent[cur_root]) {
			continue;
		}
		if (2 * sz[v] == n && v > cur_root) {
			return v;
		}
	}
	return cur_root;
}

int get_second_max(int u) {
	int mx = 0;
	for (auto v : adj[u]) {
		mx = max(mx, sz[v]);
	}
	int node = 0;
	for (auto v : adj[u]) {
		if (sz[v] == mx) {
			node = max(node, v);
		}
	}
	return node;
}

int get_topo_count(int u, int par) {
	dp[u] = 1;
	sz[u] = 1;
	for (auto v : adj[u]) {
		if (v == par) {
			continue;
		}
		get_topo_count(v, u);
		dp[u] = 1LL * dp[u] * dp[v] % md;
		dp[u] = 1LL * dp[u] * inv_fact[sz[v]] % md;
		sz[u] += sz[v];
	}
	dp[u] = 1LL * dp[u] * fact[sz[u] - 1] % md;
	return dp[u];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	int L = 500000;
	fact[0] = 1;
	inv_fact[0] = 1;
	for (int i = 1; i <= L; i++) {
		fact[i] = 1LL * fact[i - 1] * i % md;
		inv_fact[i] = pwr(fact[i], md - 2, md);
	}
	int tt;
	cin >> tt;
	while (tt--) {
		int k;
		cin >> n >> k;
		for (int i = 1; i < n; i++) {
			int u, v;
			cin >> u >> v;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		subtree_sizes(1, 0);
		int max_node = get_max(1);
		int max_count = get_topo_count(max_node, 0);
		int second_max_node = get_second_max(max_node);
		int second_max_count = get_topo_count(second_max_node, 0);
		if (k == 1) {
			cout << max_node << " " << max_count << '\n';
		} else {
			cout << second_max_node << " " << second_max_count << '\n';
		}
		for (int i = 1; i <= n; i++) {
			adj[i].clear();
		}
	}
	return 0;
}
