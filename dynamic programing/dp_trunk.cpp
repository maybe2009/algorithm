#include <stdio.h>
#include <stdint.h>

#define PIPELINE_NUM 2
#define PIPELINE_LENGTH 6
#define INF 999

uint32_t node_cost[PIPELINE_NUM][PIPELINE_LENGTH]{ {7, 9, 3, 4, 8, 4}, {8, 5, 6, 4, 5, 7} };
uint32_t transport_cost[PIPELINE_NUM][PIPELINE_LENGTH]{ { INF, 2, 3, 1, 3, 4}, { INF, 2, 1, 2, 2, 1} };
uint32_t minimal_cost[PIPELINE_NUM][PIPELINE_LENGTH];

uint32_t in1 = 2;
uint32_t in2 = 4;

uint32_t out1 = 3;
uint32_t out2 = 2;

enum LINE {
	S1 = 0,
	S2
};

uint32_t choose[2][PIPELINE_LENGTH];
uint32_t node[PIPELINE_LENGTH];

/*
* move from (index - 1) on src_line to index on dst_line
* index >= 1
*/
uint32_t cost(enum LINE src_line, enum LINE dst_line, uint32_t index) {
    if (index < 1) {
		return INF;
	}

	if (src_line == dst_line) {
		return node_cost[dst_line][index];
	}

	return transport_cost[src_line][index] + node_cost[dst_line][index];
}

/*
* at node N, if we choose line x as choose[N], what is our choose at node N-1?
* It should be :
* if minimal_cost[S1] + cost(S1, x, N-1) < minimal_cost[S2] + cost(S2, x, N-1)
* we choose S1, otherwise S2 as choose[N-1]
*/
/*
* calculate minimal cost of minimal[line][index]
*/
uint32_t minimal(enum LINE line, size_t index) {
	if (index == 0) {
		return minimal_cost[line][0];
	}

	if (line == S1) {
		uint32_t S1_cost = minimal_cost[S1][index - 1] + cost(S1, S1, index);
		uint32_t S2_cost = minimal_cost[S2][index - 1] + cost(S2, S1, index);
		if (S1_cost < S2_cost) {
			choose[S1][index] = S1;
			return S1_cost;
		}
		else {
			choose[S1][index] = S2;
			return S2_cost;
		}
	}
	else {
		uint32_t S1_cost = minimal_cost[S1][index - 1] + cost(S1, S2, index);
		uint32_t S2_cost = minimal_cost[S2][index - 1] + cost(S2, S2, index);

		if (S1_cost < S2_cost) {
			choose[S2][index] = S1;
			return S1_cost;
		}
		else {
			choose[S2][index] = S2;
			return S2_cost;
		}
	}
}

/*
* f* = min(f1n + out1, f2n + out2)
* f1x = min(f1[x-1] + cost(1, 1, x), f2[x-1] + cost(2, 1, x))
* f2x = min(f1[x-1] + cost(1, 2, x), f2[x-1] + cost(2, 2, x))
* f1 = in1 + cost[1], f2 = in2 + cost[2]
*/
uint32_t minimal_all() {
	for (size_t i = 1; i < PIPELINE_LENGTH; i++) {
		minimal_cost[S1][i] = minimal(S1, i);
		minimal_cost[S2][i] = minimal(S2, i);
		printf("i = %u, v0 = %u v1 = %u\n", i, minimal_cost[S1][i], minimal_cost[S2][i]);
	}

	uint32_t line1_min = minimal_cost[S1][PIPELINE_LENGTH - 1] + out1;
	uint32_t line2_min = minimal_cost[S2][PIPELINE_LENGTH - 1] + out2;
	if (line1_min < line2_min) {
		node[PIPELINE_LENGTH - 1] = S1;
		return line1_min;
	}
	else {
		node[PIPELINE_LENGTH - 1] = S2;
		return line2_min;
	}
}

void calculate_node() {
	uint32_t prev;
	for (int i = PIPELINE_LENGTH - 2; i >= 0; i--) {
		prev = node[i + 1];
		node[i] = choose[prev][i + 1];
		printf("node %i = %u \n", i, node[i]);
	}
}

int main() {
	minimal_cost[S1][0] = in1 + node_cost[S1][0];
	minimal_cost[S2][0] = in2 + node_cost[S2][0];
    printf("init %u %u\n", minimal_cost[S1][0], minimal_cost[S2][0]);
	uint32_t shortest = minimal_all();
	printf("The shortest path length is %u\n", shortest);

	calculate_node();
	printf("The path: \n");
	for (size_t i = 0; i < PIPELINE_LENGTH; i++) {
		printf("%u ", node[i]);
	}
    printf("\n");

	return 0;
}