#include "ExampleAIModule.h"
#include "SafePathFinder.h"
#include <BWTA.h>
#include "Utility.h"
#include "Const.h"

using namespace BWAPI;

/*
bool analyzed = true;
bool analysis_just_finished;*/

int frame = 0;
SafePathFinder* pathfinder;
UnitInterface* scout;

bool GRID = false;
bool PATH = true;
bool MOVE = true;

Position position;
bool side = 1;

void ExampleAIModule::onStart() {
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
	BWTA::analyze();
	BWTA::readMap();

	pathfinder = new SafePathFinder(BroodwarPtr);
	scout = NULL;
	position = Utility::getTrainPosition(&side);

	if (Const::NO_GUI) {
		Broodwar->setLocalSpeed(0);
		Broodwar->setGUI(false);
	}

	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	//Broodwar << "The map is " << Broodwar->mapWidth() << " x " << Broodwar->mapHeight()  << std::endl;
	//Broodwar->enableFlag(Flag::CompleteMapInformation);
	// Set the command optimization level so that common commands can be grouped and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);
	Broodwar->enableFlag(Flag::UserInput);
}

void ExampleAIModule::onEnd(bool isWinner) {
	delete(pathfinder);
	BWTA::cleanMemory();
}

void ExampleAIModule::onFrame() {
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self()) return;
	//if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0) return;

	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

	Broodwar->drawTextScreen(10, 0, "X: %d", Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x);
	Broodwar->drawTextScreen(10, 20, "Y: %d", Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y);

	Broodwar->drawTextScreen(10, 40, "XT: %d", (int)floor(((double)(((Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x)) / 32)) + 0.5));
	Broodwar->drawTextScreen(10, 60, "YT: %d", (int)floor(((double)(((Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y)) / 32)) + 0.5));
	
	if (GRID) pathfinder->showGrid();
	if (PATH) pathfinder->showPath();

	if (Const::TERRAIN_DATA) {
		pathfinder->drawTerrainData();
		pathfinder->showPolygons();
	}

	if (Const::ENEMY_RANGE) pathfinder->drawEnemiesAttackRange();

	frame++;
	//if (Const::LEARNING && frame >= 10000) Broodwar->restartGame();

	if (setScout()) {

		// new position - mouse click
		if (Broodwar->getKeyState(BWAPI::Key::K_RBUTTON)) {
			position = Utility::getMousePosition(BroodwarPtr);
			pathfinder->changePosition(position);
		}

		// move
		if (MOVE) {
			if (!pathfinder->moveUnit(position, frame)){
				position = Utility::getTrainPosition(&side);
				pathfinder->changePosition(position);
			}
		}
	}
}

void ExampleAIModule::onSendText(std::string text) {
	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!
	/*if (text == "/analyze") {
		if (analyzed == false) {
			Broodwar << "Analyzing map... this may take a minute" << std::endl;;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
		}
	}*/
	if (text == "g") {
		GRID = !GRID;
	} else if (text == "s") {
		PATH = !PATH;
	} else if (text == "m") {
		MOVE = !MOVE;
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
	} else {
		Broodwar->sendText("%s", text.c_str());
	}
}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text) {
  Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player) {
  Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target) {
  if ( target ) {
    Broodwar << "Nuclear Launch Detected at " << target << std::endl;
  }
  else {
    Broodwar->sendText("Where's the nuke?");
  }
  // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit) {
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit) {
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit) {
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit) {
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit) {
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

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getID() == scout->getID()) Broodwar->restartGame();
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit) {
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

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit) {
}

void ExampleAIModule::onSaveGame(std::string gameName) {
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit) {
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
bool ExampleAIModule::ignoreUnit(BWAPI::Unit u) {
	if (!u->exists()) return true;
	if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised()) return true;
	if (u->isLoaded() || !u->isPowered() || u->isStuck()) return true;
	if (!u->isCompleted() || u->isConstructing()) return true;
	return false;
}

bool ExampleAIModule::hasScout() {
	if (scout != NULL) return true;
	return false;
}

bool ExampleAIModule::setScout() {
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

