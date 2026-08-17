#include <memory>

#include "tetra/Tetra.h"
#include "curses/TetraCurses.h"

#include <iostream>

int main() {
    static constexpr std::chrono::milliseconds updatesPerSecond{500};

    const auto board = TetraWorldInterface::create();
    const auto rederer = std::make_shared<CursesTetraBoardRenderer>(*board);

    auto lastStep = std::chrono::system_clock::now();
    bool updateUi{true};
    do {
        const auto now = std::chrono::system_clock::now();

        if (auto inputsVec = rederer->getInputs();
            !inputsVec.empty()) {
            board->inputReceived(inputsVec);
            updateUi = true;
        }

        if ((now - lastStep) >= updatesPerSecond) {
            lastStep = now;
            board->worldStep(rederer->hardPushPressed());
            updateUi = true;
        }

        if (updateUi) {
            rederer->render(board->getCurrentWorld());
        }
        updateUi = false;

        if (board->isGameOver()) {
            rederer->stopThread();
            break;
        }
    } while (true);
    return 0;
}