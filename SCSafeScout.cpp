#include "SCSafeScout.h"
#include "SafePathFinder.h"
#include <BWTA.h>
#include "Utility.h"
#include "Const.h"

using namespace BWAPI;

int frame = 0;
SafePathFinder* pathfinder;
UnitInterface* scout;

Position position;
bool side = 1;
int position_count = 0;

void SCSafeScout::onStart() {
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
	BWTA::analyze();
	BWTA::readMap();
	
	pathfinder = new SafePathFinder(BroodwarPtr);

	scout = NULL;
	if (Const::MODE == 0) {
		position = Utility::getTrainPosition(&side);
		pathfinder->LEARNING = true;
	}
	else {
		position = BWAPI::Position(1090, 100);
		position.makeValid();
		pathfinder->LEARNING = false;
	}
	
	Broodwar->sendText("%s", "Black sheep wall");

	if (pathfinder->NO_GUI) {
		Broodwar->setLocalSpeed(0);
		Broodwar->setGUI(false);
	}

	std::ifstream file;
	file.open(Const::PATH_DEBUG);

	if (file.is_open()) {
		file >> position_count;
		file.close();
	}

	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	//Broodwar << "The map is " << Broodwar->mapWidth() << " x " << Broodwar->mapHeight()  << std::endl;
	//Broodwar->enableFlag(Flag::CompleteMapInformation);
	// Set the command optimization level so that common commands can be grouped and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);
	Broodwar->enableFlag(Flag::UserInput);
}

void SCSafeScout::onEnd(bool isWinner) {
	std::ofstream file;
	file.open(Const::PATH_DEBUG);

	if (file.is_open()) {
		file << position_count;
		file.close();
	}

	delete(pathfinder);
	BWTA::cleanMemory();
}

void SCSafeScout::onFrame() {
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self()) return;
	//if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0) return;

	// Display the game frame rate as text in the upper left area of the screen
	/*Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());
	Broodwar->drawTextScreen(200, 40, "Position %d", position_count);
	Broodwar->drawTextScreen(200, 60, "Frame %d", Broodwar->getFrameCount());
	Broodwar->drawTextScreen(200, 80, "Path %d", pathfinder->pathLenght());

	Broodwar->drawTextScreen(10, 0, "X: %d", Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x);
	Broodwar->drawTextScreen(10, 20, "Y: %d", Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y);

	Broodwar->drawTextScreen(10, 40, "XT: %d", (int)floor(((double)(((Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x)) / 32)) + 0.5));
	Broodwar->drawTextScreen(10, 60, "YT: %d", (int)floor(((double)(((Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y)) / 32)) + 0.5));
	*/
	if (pathfinder->GRID) pathfinder->showGrid();
	if (pathfinder->PATH) pathfinder->showPath();

	if (Const::TERRAIN_DATA) {
		pathfinder->drawTerrainData();
		pathfinder->showPolygons();
	}

	if (Const::ENEMY_RANGE) pathfinder->drawEnemiesAttackRange();

	frame++;

	if (setScout()) {

		// new position - mouse click
		if (Broodwar->getKeyState(BWAPI::Key::K_RBUTTON)) {
			position = Utility::getMousePosition(BroodwarPtr);
			pathfinder->changePosition(position);
		}

		// move
		if (pathfinder->MOVE) {
			if (!pathfinder->moveUnit(position, frame)){
				if (Const::MODE == 0) {
					position = Utility::getTrainPosition(&side);
					pathfinder->changePosition(position);
					position_count++;

					/*if (position_count % 10 == 0) {
					Utility::printToFile(Const::PATH_DEBUG, std::to_string(position_count));
					}*/
					//if (position_count == 1000) Broodwar->pauseGame();
				}
				else {

				}
				
			}
		}
	}
}

void SCSafeScout::onSendText(std::string text) {
	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!
	/*if (text == "/analyze") {
		if (analyzed == false) {
			Broodwar << "Analyzing map... this may take a minute" << std::endl;;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
		}
	}*/
	if (text == "g") {
		pathfinder->GRID = !pathfinder->GRID;
	} else if (text == "s") {
		pathfinder->PATH = !pathfinder->PATH;
	} else if (text == "m") {
		pathfinder->MOVE = !pathfinder->MOVE;
	} else if (text == "n") {
		pathfinder->NO_GUI = !pathfinder->NO_GUI;
		if (pathfinder->NO_GUI) {
			Broodwar->setLocalSpeed(0);
			Broodwar->setGUI(false);
		}
		else {
			Broodwar->setLocalSpeed(-1);
			Broodwar->setGUI(true);
		}
	} else if (text == "l") {
		pathfinder->LEARNING = !pathfinder->LEARNING;
	} else if (text == "r") {
		position = Utility::getRandomPosition(Broodwar->mapWidth(), Broodwar->mapHeight());
		pathfinder->changePosition(position);		
	} else if (text == "p") {
		pathfinder->changePosition(position);
	} else if (text == "q") {
		Broodwar->restartGame();
	} else if (text == "f") {
		text = "Black sheep wall";
		Broodwar->sendText("%s", text.c_str());
	} else if (text == "+") {
		pathfinder->dangerWeight += 0.1;
		Broodwar->sendText("%d", (int) (pathfinder->dangerWeight * 10));
	} else if (text == "-") {
		pathfinder->dangerWeight -= 0.1;
		Broodwar->sendText("%d", (int) (pathfinder->dangerWeight * 10));
	} else {
		Broodwar->sendText("%s", text.c_str());
	}
}

void SCSafeScout::onReceiveText(BWAPI::Player player, std::string text) {
  Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void SCSafeScout::onPlayerLeft(BWAPI::Player player) {
  Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void SCSafeScout::onNukeDetect(BWAPI::Position target) {
  if ( target ) {
    Broodwar << "Nuclear Launch Detected at " << target << std::endl;
  }
  else {
    Broodwar->sendText("Where's the nuke?");
  }
  // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void SCSafeScout::onUnitDiscover(BWAPI::Unit unit) {
}

void SCSafeScout::onUnitEvade(BWAPI::Unit unit) {
}

void SCSafeScout::onUnitShow(BWAPI::Unit unit) {
}

void SCSafeScout::onUnitHide(BWAPI::Unit unit) {
}

void SCSafeScout::onUnitCreate(BWAPI::Unit unit) {
  if ( Broodwar->isReplay() ) {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() ) {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void SCSafeScout::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getID() == scout->getID()) Broodwar->restartGame();
}

void SCSafeScout::onUnitMorph(BWAPI::Unit unit) {
  if ( Broodwar->isReplay() ) {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void SCSafeScout::onUnitRenegade(BWAPI::Unit unit) {
}

void SCSafeScout::onSaveGame(std::string gameName) {
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void SCSafeScout::onUnitComplete(BWAPI::Unit unit) {
}

//BWTA
/*
DWORD WINAPI AnalyzeThread() {
	BWTA::analyze();
	analyzed = true;
	analysis_just_finished = true;
	return 0;
}*/

//other
bool SCSafeScout::ignoreUnit(BWAPI::Unit u) {
	if (Const::MODE == 1 && !u->getType().isWorker()) return true;
	//if (u->getType() != BWAPI::UnitTypes::Protoss_Dragoon) return true;
	if (!u->exists()) return true;
	if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised()) return true;
	if (u->isLoaded() || !u->isPowered() || u->isStuck()) return true;
	if (!u->isCompleted() || u->isConstructing()) return true;
	return false;
}

bool SCSafeScout::hasScout() {
	if (scout != NULL) return true;
	return false;
}

bool SCSafeScout::setScout() {
	if (hasScout()) return true;
	for (auto &u : Broodwar->self()->getUnits()) {
		if (ignoreUnit(u)) continue;

		if (!u->getType().isBuilding()) {
			scout = u;
			pathfinder->setUnit(scout, Const::DANGER_WEIGTH);
			return true;
		}
	}
	return false;
}

