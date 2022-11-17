#include"Maze.hpp"


Maze::Maze(int width, int height) : maze(width + 2, std::vector<int>(height + 2, 1)), width(width), height(height) {
	srand((unsigned int)time(NULL));
	// used to search for neighbor nodes
	static const vec2 UP2{ 0, -2 };
	static const vec2 RIGHT2{ 2, 0 };
	static const vec2 DOWN2{ 0, 2 };
	static const vec2 LEFT2{ -2, 0 };

	vec2 dirs[] = { UP2, RIGHT2, DOWN2, LEFT2 };	// directions

	// add a Block/Wall on every even position
	for (int i = 0; i < width + 2; i++)
	{
		for (int j = 0; j < height + 2; j++)
		{
			maze[i][j] = -1;
		}
	}
	// assign each non-Block/Wall to a graph
	int id_graph = 0;
	for (int i = 2; i < width; i += 2)
	{
		for (int j = 2; j < height; j += 2)
		{
			id_graph++;
			maze[i][j] = id_graph;
		}
	}

	int nrOfGraphs = id_graph;	// last id found is the number of graphs

	// returns a reference to maze[v.x][v.y]
	auto Mz = [this](const vec2& v)->int& { return maze[v.x][v.y]; };

	vec2 saved_this_pos, saved_neigh_pos;	// used to create connection between them (delete the block between them)
	while (nrOfGraphs > 1)
	{
		// choose a random graph
		const int randX = (((rand()) % (width - 4)) / 2) * 2 + 2;	// gives a random odd number between 2 and n - 2
		const int randY = (((rand()) % (height - 4)) / 2) * 2 + 2;
		const int rand_graph_Id = maze[randX][randY];

		int nrOfNeighboringNodes = 0;	// used for randomly choosing between all neighbors so I can connect the graphs
		// iterate through all the nodes that are in the random graph
		for (int i = 2; i < width; i += 2)
		{
			for (int j = 2; j < height; j += 2)
			{
				vec2 this_pos = vec2(i, j);
				int curr_pos_graph_id = Mz(this_pos);
				if (curr_pos_graph_id == rand_graph_Id)	// if current cell is part of the random graph
				{
					for (const vec2& dir : dirs)	// for each neighbor
					{
						const vec2 neigh_pos(i + dir.x, j + dir.y);
						int neighb_graph_Id = Mz(neigh_pos);
						// if the neighbor node is from a different graph,
						// randomly choose if to connect the rand_Id_graph to this graph
						// by selecting it with a probability of 1/nrOfNeighboringNodes
						if (neighb_graph_Id != -1
							&& neighb_graph_Id != rand_graph_Id)
						{
							// found a neighboring node that is not part of this graph => increment nrOfNeighboringNodes
							nrOfNeighboringNodes++;

							// randomly choose if to connect the neighb_graph_Id to this graph
							if (rand_true(nrOfNeighboringNodes))
							{
								saved_this_pos = this_pos;
								saved_neigh_pos = neigh_pos;
							}
						}
					}
				}
			}
		}

		int id_ng = Mz(saved_neigh_pos);	// id of a the neighbor graph

		Mz((saved_this_pos + saved_neigh_pos) / 2) = rand_graph_Id;	// the block between them = id_graph

		for (int i = 2; i < width; i += 2)
		{
			for (int j = 2; j < height; j += 2)
			{
				if (maze[i][j] == id_ng)	// convert the neighbor graph into this one
				{
					maze[i][j] = rand_graph_Id;
				}
			}
		}

		nrOfGraphs--;
	}
}
