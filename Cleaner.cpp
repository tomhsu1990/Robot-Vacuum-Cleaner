#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>

class Cleaner{
public:
	using Location = std::pair<int, int>;
  	enum direction { N = 0, E, S, W };

	Cleaner(){
		mp.clear();
		pos.clear();
		cleaned.clear();
		dir = -1;
	}
	~Cleaner(){}

	std::vector<int> parseLocation (std::string s) {
		std::vector<int> loc;
		std::string tmp("");
		//std::cerr << it->first << std::endl;
		for (unsigned i=0;i<s.size();++i) {
			if (s[i] == ',') {
				loc.push_back(atoi(tmp.c_str()));
				tmp = "";
			}
			else tmp.append(1, s[i]);
		}
		return loc;
	}

	bool check (int x, int y, int d) {
		std::string rec(std::to_string(x)+","+std::to_string(y)+","+std::to_string(d));
		if (vis.find(rec) != vis.end()) {
			for (auto it=vis.begin();it!=vis.end();++it) {
				std::vector<int> loc = parseLocation(it->first);
				++cleaned[std::to_string(loc[0])+","+std::to_string(loc[1])+","];
				//mp[loc[0]][loc[1]] = 'x';
			}
			vis.clear();
			return false;
		}
		return true;
	}

	bool Move (bool touchWall) {
		// for visualization
		//if (touchWall){
			//std::cout << "move " << glb_pos.first << " " << glb_pos.second << " " << dir << std::endl;
			//mp[glb_pos.first][glb_pos.second] = '*';
		//}
		//showGlobalMap();

		if (dir == N) {
			if (glb_pos.first-1 < 0 ||
				mp[glb_pos.first-1][glb_pos.second] == '+' ||
				cleaned.find(std::to_string(glb_pos.first-1)+","+std::to_string(glb_pos.second)+",") != cleaned.end() ||
				!check(glb_pos.first-1, glb_pos.second, dir))
				return false;
			--glb_pos.first;
			--locl_pos.first;
		}
		else if (dir == E) {
			if (glb_pos.second+1 >= mp[glb_pos.first].size() || 
				mp[glb_pos.first][glb_pos.second+1] == '+' ||
				cleaned.find(std::to_string(glb_pos.first)+","+std::to_string(glb_pos.second+1)+",") != cleaned.end() ||
				!check(glb_pos.first, glb_pos.second+1, dir))
				return false;
			++glb_pos.second;
			++locl_pos.second;
		}
		else if (dir == S) {
			if (glb_pos.first+1 >= mp.size() || 
			    glb_pos.second >= mp[glb_pos.first+1].size() ||
				mp[glb_pos.first+1][glb_pos.second] == '+' ||
				cleaned.find(std::to_string(glb_pos.first+1)+","+std::to_string(glb_pos.second)+",") != cleaned.end() ||
				!check(glb_pos.first+1, glb_pos.second, dir))
				return false;
			++glb_pos.first;
			++locl_pos.first;
		}
		else if (dir == W) {
			if (glb_pos.second-1 < 0 || 
				mp[glb_pos.first][glb_pos.second-1] == '+' ||
				cleaned.find(std::to_string(glb_pos.first)+","+std::to_string(glb_pos.second-1)+",") != cleaned.end() ||
				!check(glb_pos.first, glb_pos.second-1, dir))
				return false;
			--glb_pos.second;
			--locl_pos.second;
		}
		std::string rec(std::to_string(glb_pos.first)+","+std::to_string(glb_pos.second)+","+std::to_string(dir));
		if (touchWall) ++vis[rec];

		return true;
	}

	void turnLeft (int k) {
		//k = k%4;
		dir = (dir-k+4)%4;
	}
	void turnRight (int k) {
		//k = k%4;
		dir = (dir+k)%4;
	}

	void updateLocalMap () {
		if (dir == N) {
			pos.push_back(Cleaner::Location(glb_pos.first+1,glb_pos.second));
		}
		else if (dir == E) {
			pos.push_back(Cleaner::Location(glb_pos.first,glb_pos.second-1));
		}
		else if (dir == S) {
			pos.push_back(Cleaner::Location(glb_pos.first-1,glb_pos.second));
		}
		else if (dir == W) {
			pos.push_back(Cleaner::Location(glb_pos.first,glb_pos.second+1));
		}
	}

	void updateGlocalMap () {
		for (auto it=cleaned.begin();it!=cleaned.end();++it) {
			std::vector<int> loc = parseLocation(it->first);
			mp[loc[0]][loc[1]] = 'x'; // x means cleaned.
		}
	}
	
	void clean () {
		// initialize robot local pose
		locl_pos = Cleaner::Location(0,0);
		//do {
			// turn the direction to N
			while(dir != N) turnRight(1);
			// move forward and touch a wall
			while(Move(false));
			// maintain that left hand touches a wall
			while (true) {
				// assume back is not a possible empty place
				updateLocalMap();
				
				int turn(0);
				while (turn < 4) {
					std::string rec(std::to_string(glb_pos.first)+","+std::to_string(glb_pos.second)+","+std::to_string(dir));
					++vis[rec];
					turnRight(1);
					if (Move(true)) break;
					++turn;
				}
				if(turn == 4) break;
				// face a wall
				do {
					turnLeft(1);
				} while(Move(true)) ;
			}

			// go to a possible un-cleaned position
			// bool find(false);
			// Location nxt;
			// while (true) {
			// 	if (pos.empty()) break;
			// 	else {
			// 		nxt = pos.front(); pos.pop_front();
			// 		if (nxt.first < 0 || nxt.first >= mp.size() || 
			// 			nxt.second < 0 || nxt.second >= mp[nxt.first].size() ||
			// 			mp[nxt.first][nxt.second] == '+')
			// 			continue;
			// 		if (cleaned.find(std::to_string(nxt.first)+","+std::to_string(nxt.second)+",") != cleaned.end())
			// 			continue;
			// 		find = true;
			// 		break;
			// 	}
			// }
			// if (find) {
			// 	// move to nxt as the start of the next round

			// }
		//} while (!pos.empty()) ;

		updateGlocalMap();
	}

	void showGlobalMap () {
		//system("clear");

		char tmp(mp[glb_pos.first][glb_pos.second]);
		if (dir == N)
			mp[glb_pos.first][glb_pos.second] = '^';
		else if (dir == E)
			mp[glb_pos.first][glb_pos.second] = '>';
		else if (dir == S)
			mp[glb_pos.first][glb_pos.second] = 'v';
		else if (dir == W)
			mp[glb_pos.first][glb_pos.second] = '<';	
		for(unsigned i=0;i<mp.size();++i)
			std::cout << mp[i] << std::endl;
		mp[glb_pos.first][glb_pos.second] = tmp;

		//sleep(1);
	}

	void showLocalMap () {

	}

	int dir;
	Location glb_pos, locl_pos;
	std::vector<std::string> mp;
	std::deque<std::deque<char>> dy_mp;
	std::unordered_map<std::string, int> vis;
	std::unordered_map<std::string, int> cleaned;
	// check whether it is visted
	std::deque<Location> pos;
};

int main (int argc, char* argv[]) {
	
	std::string mapName;
	if(argc == 1) mapName = "map.txt";
	else		  mapName = argv[1];
	std::ifstream myMap(mapName);
	if (myMap.is_open()) {
		Cleaner *robot = new Cleaner();
		robot->glb_pos = Cleaner::Location(0,0);

		std::string line;
		while (getline(myMap, line)) {
			robot->mp.push_back(line);
			for(int i=0;i<line.size();++i){
				if(line[i] == '^'){
					robot->glb_pos.second = i;
					robot->dir = Cleaner::N;
				}
				else if(line[i] == '>'){
					robot->glb_pos.second = i;
					robot->dir = Cleaner::E;
				}
				else if(line[i] == 'v'){
					robot->glb_pos.second = i;
					robot->dir = Cleaner::S;
				}
				else if(line[i] == '<'){
					robot->glb_pos.second = i;
					robot->dir = Cleaner::W;
				}
			}
			if(robot->dir < 0) ++robot->glb_pos.first;
		}
		myMap.close();

		robot->clean();
		robot->showGlobalMap();
	}
	else std::cerr << "Unable to open " << mapName << std::endl;

	return 0;
}
