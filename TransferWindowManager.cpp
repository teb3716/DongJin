#include "TransferWindowManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <vector>
using namespace std;

TransferWindowManager::SoccerTeam::SoccerTeam()
{
	//You don't need to edit this function.
}

TransferWindowManager::SoccerTeam::SoccerTeam(SoccerPlayerData fw, SoccerPlayerData mf, SoccerPlayerData df, SoccerPlayerData gk)
{
	this->fw = fw;
	this->mf = mf;
	this->df = df;
	this->gk = gk;

	this->sum_transfer_fee = fw.m_transfer_fee + mf.m_transfer_fee + df.m_transfer_fee + gk.m_transfer_fee;
	this->sum_ability = fw.m_ability + mf.m_ability + df.m_ability + gk.m_ability;
}

ostream& operator<<(ostream& os, const TransferWindowManager::SoccerTeam& team)
{
	os << team.fw << '\n';
	os << team.mf << '\n';
	os << team.df << '\n';
	os << team.gk << '\n';

	os << "sum_transfer_fee " << team.sum_transfer_fee << '\n';
	os << "sum_ability " << team.sum_ability << '\n';

	return os;
}

TransferWindowManager::TransferWindowManager(string file_dir, int budget)
{
	// Initialize num_POS member vaviables
	num_fw = num_df = num_mf = num_gk = 0;

	// Check if the input budget has proper value
	m_budget = budget;
	assert(m_budget >= 1664);

	// Create fstream to handle input text file
	ifstream in(file_dir);
	string line;

	// Vector of SoccerPlayerData for all player data
	vector<SoccerPlayerData> v_player;

	while (getline(in, line))
	{
		// Read the text file and create new instances
		stringstream ss(line);
		string token;
		int column = 0;

		// Member variables for new instance
		string name; string position;
		int transfer_fee;
		int ability;

		while(getline(ss, token, ',')) {
			// Split into each string and ignore whitespaces
			string strField;
			strField.assign(token);
			strField.erase(remove_if(strField.begin(), strField.end(), ::isspace),strField.end());
			
			// Parse into each fields in SoccerPlayerData
			switch(column) {
				case 0:
					name.assign(strField);
					break;
				case 1:
					position.assign(strField);
					break;
				case 2:
					transfer_fee = atoi(strField.c_str());
					break;
				case 3:
					ability = atoi(strField.c_str());
					break;
			}
			column++;
		}

		// Append new instance to the vector
		SoccerPlayerData newPlayer(name, position, transfer_fee, ability);
		v_player.push_back(newPlayer);

		// Determine the position and insert into decent BST
		if(newPlayer.m_position == "Forward") {
			fwBST.insert(newPlayer);
			num_fw++;
		}
		else if(newPlayer.m_position == "Midfielder") {
			mfBST.insert(newPlayer);
			num_mf++;
		}
		else if(newPlayer.m_position == "Defender") {
			dfBST.insert(newPlayer);
			num_df++;
		}
		else if(newPlayer.m_position == "Goalkeeper") {
			gkBST.insert(newPlayer);
			num_gk++;
		}
	}
}

ostream& operator<<(ostream& os, const TransferWindowManager& manager)
{
	os << "********Forward List********" << '\n';
	os << manager.fwBST;
	os << "****************************" << '\n';

	os << "********Midflder List********" << '\n';
	os << manager.mfBST;
	os << "*****************************" << '\n';

	os << "********Defender List********" << '\n';
	os << manager.dfBST;
	os << "*****************************" << '\n';

	os << "********Goalkeeper List********" << '\n';
	os << manager.gkBST;
	os << "*******************************" << '\n';

	return os;
}

TransferWindowManager::SoccerTeam TransferWindowManager::getBestTeam()
{
	// DEBUG
	vector<SoccerTeam> v_team;

	// FOR: Insert all teams to the vector v_team
	for(int i_fw = 0; i_fw < num_fw; i_fw++) {
		// FW player
		SoccerPlayerData fw = fwBST.at(i_fw);
		// FOR	
		for(int i_mf = 0; i_mf < num_mf; i_mf++) {
			// MF player
			SoccerPlayerData mf = mfBST.at(i_mf);
			// FOR
			for(int i_df = 0; i_df < num_df; i_df++) {
				// DF player
				SoccerPlayerData df = dfBST.at(i_df);
				// FOR
				for(int i_gk = 0; i_gk < num_gk; i_gk++) {
					// GK player
					SoccerPlayerData gk = gkBST.at(i_gk);

					// Create a new SoccerTeam instance
					SoccerTeam newTeam(fw, mf, df, gk);
					v_team.push_back(newTeam);
				}
				// ENDFOR
			}
			// ENDFOR
		}
		// ENDFOR
	}
	// ENDFOR

	// Temporary variables
	int best_ability = 0;
	SoccerTeam best_team;

	for(int i = 0; i < (int)v_team.size(); i++) {
		// IF: Budget in shortage
		if(m_budget < v_team[i].sum_transfer_fee)
			continue;

		// ELSE: Check if best offer
		if(v_team[i].sum_ability >= best_ability)
			best_team = v_team[i];
	}

	// Delete members in best_team
	fwBST.deletion(best_team.fw.m_ability);
	mfBST.deletion(best_team.mf.m_ability);
	dfBST.deletion(best_team.df.m_ability);
	gkBST.deletion(best_team.gk.m_ability);

	return best_team;
}
