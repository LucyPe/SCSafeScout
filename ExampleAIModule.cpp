#include "ExampleAIModule.h"
#include "SafePathFinder.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

bool analyzed = true;
bool analysis_just_finished;

int frame = 0;
SafePathFinder* pathfinder;
UnitInterface* scout;

bool GRID = false;
bool PATH = true;
bool MOVE = true;

Position pos;

void ExampleAIModule::onStart() {
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
	BWTA::analyze();
	BWTA::readMap();

	pathfinder = new SafePathFinder(BroodwarPtr);
	scout = NULL;
	pos = pathfinder->randomPosition();

	//pos = BWAPI::Position(1800, 1200);
	//Broodwar->setLocalSpeed(0);
	//Broodwar->setGUI(false);

	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	//Broodwar << "The map is " << Broodwar->mapWidth() << " x " << Broodwar->mapHeight()  << std::endl;
	Broodwar->enableFlag(Flag::UserInput);
	//Broodwar->enableFlag(Flag::CompleteMapInformation);
	// Set the command optimization level so that common commands can be grouped and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

}

void ExampleAIModule::onEnd(bool isWinner) {
	delete(pathfinder);
	BWTA::cleanMemory();
}

void ExampleAIModule::onFrame() {
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self()) return;
	//if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0) return;

	// Display the game frame rate as text in the upper left area of the screen
	//Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	//Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

	Broodwar->drawTextScreen(10, 0, "X: %d", Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x);
	Broodwar->drawTextScreen(10, 20, "Y: %d", Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y);

	Broodwar->drawTextScreen(10, 40, "XT: %d", (int)floor(((double)(((Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x)) / 32)) + 0.5));
	Broodwar->drawTextScreen(10, 60, "YT: %d", (int)floor(((double)(((Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y)) / 32)) + 0.5));
	
	if (GRID) pathfinder->showGrid();
	if (PATH) pathfinder->showPath();

	pathfinder->drawTerrainData();
	pathfinder->drawEnemiesAttackRange();
	pathfinder->showPolygons();
	
	frame++;
	if (frame >= 50000) Broodwar->restartGame();

	if (setScout()) {
		// draw enemies positions
		Unitset enemies = scout->getUnitsInRadius(scout->getType().sightRange() + 100);

		// new position
		if (Broodwar->getKeyState(BWAPI::Key::K_RBUTTON)) {
			pos = BWAPI::Position(Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x, Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y);
			pos = pos.makeValid();
			//Broodwar->sendText("pos %d, %d", pos.x, pos.y);
			pathfinder->findPath(scout->getPosition(), pos, scout);
		}

		// no path
		if (!pathfinder->existPath()) {
			//pos = pathfinder->randomPosition();
			//pos = BWAPI::Position(1800, 1200);
			//Broodwar->sendText("pos %d, %d", pos.x, pos.y);
			//pathfinder->findPath(scout->getPosition(), pos, scout);
		}
		// has path
		if (pathfinder->existPath()) {
			if (MOVE && Utility::PositionInRange(pathfinder->nextPosition(), scout->getPosition(), WALK_TILE * 10)) {
				if (frame % 100 == 0) pathfinder->update(scout);
				scout->move(pathfinder->getNextPosition());
				// if enemy is near recalculate path
				if (!enemies.empty())  {
					pathfinder->findPath(scout->getPosition(), pos, scout);
				}
			}
			//if (pathfinder->pathSize() <= 10) Broodwar->restartGame();
		}
	}
}

void ExampleAIModule::onSendText(std::string text) {
	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!
	if (text == "/analyze") {
		if (analyzed == false) {
			Broodwar << "Analyzing map... this may take a minute" << std::endl;;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
		}
	}
	else if (text == "g") {
		GRID = !GRID;
	} else if (text == "s") {
		PATH = !PATH;
	} else if (text == "m") {
		MOVE = !MOVE;
	} else if (text == "r") {
		pos = pathfinder->randomPosition();
		Broodwar->sendText("pos %d, %d", pos.x, pos.y);
		pathfinder->findPath(scout->getPosition(), pos, scout);
	} else if (text == "p") {
		pathfinder->findPath(scout->getPosition(), pos, scout);
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

DWORD WINAPI AnalyzeThread() {
	BWTA::analyze();

	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

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
			return true;
		}
	}
	return false;
}

